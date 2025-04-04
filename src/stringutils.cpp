//Copyright(C) 2025 Lost Empire Entertainment
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it under certain conditions.
//Read LICENSE.md for more information.

//main log macro
#define WRITE_LOG(type, msg) std::cout << "[KALAKIT_STRINGUTILS | " << type << "] " << msg << "\n"

//log types
#if KALAUTILS_DEBUG
	#define LOG_DEBUG(msg) WRITE_LOG("DEBUG", msg)
#else
	#define LOG_DEBUG(msg)
#endif
#define LOG_SUCCESS(msg) WRITE_LOG("SUCCESS", msg)
#define LOG_ERROR(msg) WRITE_LOG("ERROR", msg)

#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>

#include "stringutils.hpp"

using std::any_of;
using std::getline;
using std::copy_if;
using std::istringstream;
using std::ifstream;

namespace KalaKit
{
	string StringUtils::StringReplace(const string& original, const string& search, const string& replacement)
	{
		string result = original;
		size_t pos = 0;
		while ((pos = result.find(search, pos)) != string::npos)
		{
			result.replace(pos, search.length(), replacement);
			pos += replacement.length();
		}
		return result;
	}

	string StringUtils::CharReplace(const string& original, const char& search, const char& replacement)
	{
		string result = original;
		size_t pos = 0;
		while ((pos = result.find(search, pos)) != string::npos)
		{
			result[pos] = replacement;
			pos += 1;
		}
		return result;
	}

	vec3 StringUtils::StringToVec3(const vector<string>& original)
	{
		vec3 output{};

		istringstream issX(original[0]);
		istringstream issY(original[1]);
		istringstream issZ(original[2]);

		issX >> output.x;
		issY >> output.y;
		issZ >> output.z;

		return output;
	}

	vector<string> StringUtils::Split(const string& input, char delimiter)
	{
		vector<string> tokens;
		string token;
		istringstream tokenStream(input);
		while (getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	vector<string> StringUtils::RemoveExcept(const vector<string>& originalVector, const string& instance)
	{
		auto containsInstance = [&instance](const string& s)
			{
				return s.find(instance) != string::npos;
			};

		//check if any element contains the specified instance
		bool foundInstance = any_of(originalVector.begin(), originalVector.end(), containsInstance);

		if (!foundInstance)
		{
			return originalVector;
		}

		//create a new vector with elements not containing the specified instance
		vector<string> modifiedVector;
		copy_if(
			originalVector.begin(),
			originalVector.end(),
			back_inserter(modifiedVector),
			containsInstance);
		{
			return modifiedVector;
		};
	}

	vector<string> StringUtils::RemoveDuplicates(const vector<string>& originalVector)
	{
		//create a copy of the original vector
		vector<string> modifiedVector = originalVector;

		//sort the vector to bring duplicates together
		sort(modifiedVector.begin(), modifiedVector.end());

		//remove adjacent duplicates
		modifiedVector.erase(
			unique(modifiedVector.begin(),
				   modifiedVector.end()),
				   modifiedVector.end());

		return modifiedVector;
	}

	bool StringUtils::CanConvertStringToFloat(const string& value)
	{
		size_t pos;
		float convertedValue = stof(value, &pos);

		return pos == value.length();
	}

	bool StringUtils::CanConvertStringToInt(const string& value)
	{
		size_t pos;
		int convertedValue = stoi(value, &pos);

		return pos == value.length();
	}

	bool StringUtils::IsValidSymbolInPath(const char& c)
	{
		return c == '-'
			|| c == '_'
			|| c == ' '
			|| (c >= '0' && c <= '9')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= 'a' && c <= 'z');
	}
}