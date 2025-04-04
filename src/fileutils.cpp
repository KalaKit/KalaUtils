//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//main log macro
#define WRITE_LOG(type, msg) std::cout << "[KALAKIT_FILEUTILS | " << type << "] " << msg << "\n"

//log types
#if KALAUTILS_DEBUG
	#define LOG_DEBUG(msg) WRITE_LOG("DEBUG", msg)
#else
	#define LOG_DEBUG(msg)
#endif
#define LOG_SUCCESS(msg) WRITE_LOG("SUCCESS", msg)
#define LOG_ERROR(msg) WRITE_LOG("ERROR", msg)

#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#endif

#include "fileutils.hpp"

using std::to_string;
using std::runtime_error;
using std::wstring;
using std::exception;
using std::filesystem::exists;
using std::filesystem::directory_iterator;
using std::filesystem::copy_options;
using std::ifstream;

namespace KalaKit
{
    string FileUtils::GetOutputFromBatFile(const char* file)
    {
#ifdef _WIN32
        char buffer[128];
        string result = "";
        string command = "\"" + string(file) + "\"";
        FILE* pipe = _popen(command.c_str(), "r");

        if (!pipe) throw runtime_error("popen() failed!");

        try
        {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            _pclose(pipe);
            throw;
        }

        _pclose(pipe);

        return result;
#elif __linux__
        char buffer[128];
        string result = "";
        string command = "\"" + string(file) + "\"";
        FILE* pipe = popen(command.c_str(), "r");

        if (!pipe) throw runtime_error("popen() failed!");

        try
        {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            pclose(pipe);
            throw;
        }

        pclose(pipe);

        return result;
#endif
    }

    int FileUtils::RunBatFile(const string& file, bool runSeparate)
    {
        string command = runSeparate == true
            ? "start cmd /c \"" + file + "\""
            : "\"" + file + "\"";

        return system(command.c_str());
    }

    void FileUtils::RunApplication(const string& exePath, const string& commands)
    {
#ifdef _WIN32
        string parentFolderPath = path(exePath).parent_path().string();
        wstring wParentFolderPath(parentFolderPath.begin(), parentFolderPath.end());
        wstring wExePath(exePath.begin(), exePath.end());
        wstring wCommands(commands.begin(), commands.end());

        //initialize structures for process creation
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));
        si.cb = sizeof(si);

        // Create the new process
        if (!CreateProcessW
        (
            wExePath.c_str(),          //path to the executable
            const_cast<LPWSTR>(wCommands.c_str()), //command line arguments
            nullptr,                   //process handle not inheritable
            nullptr,                   //thread handle not inheritable
            FALSE,                     //handle inheritance
            0,                         //creation flags
            nullptr,                   //use parent's environment block
            wParentFolderPath.c_str(), //use parent's starting directory
            &si,                       //pointer to STARTUPINFO structure
            &pi                        //pointer to PROCESS_INFORMATION structure
        ))
        {
            //retrieve the error code and print a descriptive error message
            LPVOID lpMsgBuf = nullptr;
            DWORD dw = GetLastError();
            FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&lpMsgBuf, 0, nullptr);
            std::wcout << L"Error: " << reinterpret_cast<LPCWSTR>(lpMsgBuf) << L"\n\n";
            LocalFree(lpMsgBuf);
        }

        //close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
#elif __linux__
        //change working directory to parentFolderPath
        if (chdir(parentFolderPath.c_str()) != 0)
        {
            perror("Error changing directory");
            return;
        }

        //parse the commands into arguments
        vector<string> args;
        size_t pos = 0, found;
        while ((found = commands.find(' ', pos)) != string::npos)
        {
            args.push_back(commands.substr(pos, found - pos));
            pos = found + 1;
        }
        args.push_back(commands.substr(pos));

        //prepare arguments for execvp
        vector<char*> execArgs;
        execArgs.push_back(const_cast<char*>(exePath.c_str()));
        for (auto& arg : args)
        {
            execArgs.push_back(const_cast<char*>(arg.c_str()));
        }
        execArgs.push_back(nullptr);

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Error during fork");
            return;
        }

        if (pid == 0)
        {
            //child process: execute the program
            execvp(execArgs[0], execArgs.data());
            perror("Error during execvp");
            exit(EXIT_FAILURE); //exit if execvp fails
        }
        else
        {
            //parent process: wait for the child to finish
            int status;
            if (waitpid(pid, &status, 0) == -1)
            {
                perror("Error during waitpid");
            }
            else if (WIFEXITED(status))
            {
                cout << "Child exited with status: " << WEXITSTATUS(status) << "\n";
            }
            else
            {
                cout << "Child did not exit normally\n";
            }
        }
#endif
    }

    bool FileUtils::ContainsString(const string& filePath, const string& targetString)
    {
        ifstream file(filePath);
        if (!file.is_open())
        {
            LOG_ERROR("Error opening file: " << filePath);
            return false;
        }

        string line;
        while (getline(file, line)) {
            if (line.find(targetString) != string::npos)
            {
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    void FileUtils::MoveOrRenameTarget(const string& originPath, const string& targetPath)
    {
        string output;
        bool isSameDirectory = path(originPath).parent_path() == path(targetPath).parent_path();

        if (!exists(originPath))
        {
            LOG_ERROR("Source path " + path(originPath).string() + " does not exist!");
            return;
        }
        if (exists(path(targetPath)))
        {
            string type = isSameDirectory ? "rename" : "move";

            LOG_ERROR("Cannot " + type + " origin '" + path(originPath).string() + "' to target '" + path(targetPath).string() + "' because target already exists!");
            return;
        }

        try
        {
            rename(path(originPath), path(targetPath));

            string type = isSameDirectory ? "Renamed '" : "Moved '";
            LOG_DEBUG(type + path(originPath).string() + "' to '" + path(targetPath).string() + "'.");
        }
        catch (const exception& e)
        {
            LOG_ERROR("FileUtils::MoveOrRenameTarget: " + string(e.what()) + ".");
        }
    }

    void FileUtils::CopyTarget(const string& originPath, const string& targetPath)
    {
        string output;

        if (!exists(path(originPath)))
        {
            LOG_ERROR("Error: Source path '" + path(originPath).string() + "' does not exist!");
            return;
        }

        try
        {
            if (is_directory(path(originPath)))
            {
                copy(path(originPath), path(targetPath), copy_options::recursive | copy_options::overwrite_existing);

                LOG_DEBUG("Copied folder '" + path(originPath).string() + "' to '" + path(targetPath).string() + "'.");
            }
            else if (is_regular_file(path(originPath)))
            {
                copy_file(path(originPath), path(targetPath), copy_options::overwrite_existing);

                LOG_DEBUG("Copied file '" + path(originPath).string() + "' to '" + path(targetPath).string() + "'.");
            }
        }
        catch (const exception& e)
        {
            LOG_ERROR("FileUtils::CopyTarget: " + string(e.what()) + ".");
        }
    }

    void FileUtils::DeleteTarget(const string& targetPath)
    {
        string output;
        if (!exists(path(targetPath)))
        {
            LOG_ERROR("Error: Cannot delete file or folder '" + path(targetPath).string() + "' because it does not exist!");
            return;
        }

        try
        {
            if (is_regular_file(path(targetPath))) remove(path(targetPath));
            else if (is_directory(path(targetPath)))
            {
                remove_all(path(targetPath));
            }

            LOG_DEBUG("Deleted '" + path(targetPath).string() + "'.");
        }
        catch (const exception& e)
        {
            LOG_ERROR("FileUtils::DeleteTarget: " + string(e.what()) + ".");
        }
    }

    void FileUtils::CreateNewFolder(const string& folderPath)
    {
        string output;
        if (exists(path(folderPath)))
        {
            LOG_ERROR(path(folderPath).string() + " already exists!");

            return;
        }

        if (is_regular_file(path(folderPath)))
        {
            LOG_ERROR(path(folderPath).string() + " must be a folder!");

            return;
        }

        try
        {
            create_directory(path(folderPath));

            LOG_DEBUG("Created new folder at '" + path(folderPath).string() + "'.");
        }
        catch (const exception& e)
        {
            LOG_ERROR("FileUtils::CreateNewFolder: " + string(e.what()) + ".");
        }
    }

    string FileUtils::AddIndex(
        const path& parentFolderPath,
        const string& fileName,
        const string& extension)
    {
        string newFilePath = (path(parentFolderPath) / (fileName + extension)).string();

        if (exists(newFilePath))
        {
            //extract the actual base name (before the last '(')
            string baseName = fileName;
            size_t openParentPos = fileName.find_last_of('(');
            if (openParentPos != string::npos)
            {
                baseName = fileName.substr(0, openParentPos - 1);
            }

            //find highest available index for this exact base name
            int highestNumber = 1;
            for (const auto& entry : directory_iterator(parentFolderPath))
            {
                string entryName = path(entry).stem().string();

                //extract the base name of the existing entry
                string entryBaseName = entryName;
                size_t entryOpenParentPos = entryName.find_last_of('(');
                if (entryOpenParentPos != string::npos)
                {
                    entryBaseName = entryName.substr(0, entryOpenParentPos - 1);
                }

                //only compare entries with the exact same base name
                if (entryBaseName == baseName)
                {
                    string entryValue = GetValueBetweenParentheses(entryName);
                    if (!entryValue.empty() && all_of(entryValue.begin(), entryValue.end(), ::isdigit))
                    {
                        int entryConvertedValue = stoi(entryValue);
                        if (entryConvertedValue >= highestNumber)
                        {
                            highestNumber = entryConvertedValue + 1;
                        }
                    }
                }
            }

            //remove existing index (if any) before appending new one
            string cleanedFileName = fileName;
            size_t cleanOpenParentPos = fileName.find_last_of('(');
            size_t cleanCloseParentPos = fileName.find_last_of(')');

            if (cleanOpenParentPos != string::npos && cleanCloseParentPos != string::npos && cleanCloseParentPos > cleanOpenParentPos)
            {
                string potentialNumber = fileName.substr(cleanOpenParentPos + 1, cleanCloseParentPos - cleanOpenParentPos - 1);
                if (all_of(potentialNumber.begin(), potentialNumber.end(), ::isdigit))
                {
                    cleanedFileName = fileName.substr(0, cleanOpenParentPos - 1);
                }
            }

            //ensure cleaned filename is not empty
            if (cleanedFileName.empty()) cleanedFileName = fileName;

            //create new indexed filename
            string newFileName = cleanedFileName + " (" + to_string(highestNumber) + ")" + extension;
            newFilePath = (path(parentFolderPath) / newFileName).string();
        }

        return newFilePath;
    }
    string FileUtils::GetValueBetweenParentheses(const string& input)
    {
        size_t start = input.find('(');
        size_t end = input.find(')', start);

        if (start != string::npos
            && end != string::npos
            && end > start)
        {
            return input.substr(start + 1, end - start - 1);
        }

        return "";
    }
}