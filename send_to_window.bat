@echo off

set "UTILS_ROOT=%~dp0"

set "INSTALL_RELEASE=%UTILS_ROOT%install-release"
set "INSTALL_DEBUG=%UTILS_ROOT%install-debug"
set "BUILD_RELEASE=%UTILS_ROOT%build-release"
set "BUILD_DEBUG=%UTILS_ROOT%build-debug"

:: Remove old build and install folders
if exist "%INSTALL_RELEASE%" rmdir /S /Q "%INSTALL_RELEASE%"
if exist "%INSTALL_DEBUG%" rmdir /S /Q "%INSTALL_DEBUG%"
if exist "%BUILD_RELEASE%" rmdir /S /Q "%BUILD_RELEASE%"
if exist "%BUILD_DEBUG%" rmdir /S /Q "%BUILD_DEBUG%"

:: Build and install
cmd /c "build_windows_release.bat"
cmd /c "build_windows_debug.bat"

set "ORIGIN_RELEASE_DLL=%INSTALL_RELEASE%\bin\KalaUtils.dll"
set "ORIGIN_RELEASE_LIB=%INSTALL_RELEASE%\lib\KalaUtils.lib"
set "ORIGIN_DEBUG_DLL=%INSTALL_DEBUG%\bin\KalaUtilsD.dll"
set "ORIGIN_DEBUG_LIB=%INSTALL_DEBUG%\lib\KalaUtilsD.lib"

if not exist "%ORIGIN_RELEASE_DLL%" (
	echo Failed to find origin release dll from '%ORIGIN_RELEASE_DLL%'!
	pause
	exit /b 1
)
if not exist "%ORIGIN_RELEASE_LIB%" (
	echo Failed to find origin release lib from '%ORIGIN_RELEASE_LIB%'!
	pause
	exit /b 1
)
if not exist "%ORIGIN_DEBUG_DLL%" (
	echo Failed to find origin debug dll from '%ORIGIN_DEBUG_DLL%'!
	pause
	exit /b 1
)
if not exist "%ORIGIN_DEBUG_LIB%" (
	echo Failed to find origin debug lib from '%ORIGIN_DEBUG_LIB%'!
	pause
	exit /b 1
)

set "TARGET_ROOT=%UTILS_ROOT%..\KalaWindow\_external_shared\KalaUtils"

if not exist "%TARGET_ROOT%" (
	echo Failed to find target root from '%TARGET_ROOT%'!
	pause
	exit /b 1
)

set "TARGET_RELEASE_DLL=%TARGET_ROOT%\release\KalaUtils.dll"
set "TARGET_RELEASE_LIB=%TARGET_ROOT%\release\KalaUtils.lib"
set "TARGET_DEBUG_DLL=%TARGET_ROOT%\debug\KalaUtilsD.dll"
set "TARGET_DEBUG_LIB=%TARGET_ROOT%\debug\KalaUtilsD.lib"

:: Copy dll files to target path
copy /Y "%ORIGIN_RELEASE_DLL%" "%TARGET_RELEASE_DLL%"
copy /Y "%ORIGIN_RELEASE_LIB%" "%TARGET_RELEASE_LIB%"
copy /Y "%ORIGIN_DEBUG_DLL%" "%TARGET_DEBUG_DLL%"
copy /Y "%ORIGIN_DEBUG_LIB%" "%TARGET_DEBUG_LIB%"

:: Copy header files and inl files to target path
xcopy /E /Y /I "%INSTALL_RELEASE%\include" "%TARGET_ROOT%\"

echo Successfully installed KalaUtils!

pause
exit /b 0