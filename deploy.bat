@ECHO off

:: Set custom path to asset folder here
SET AssetSource=%UserProfile%\Google Drive\Fisk_ Litet Spelprojekt\Assets

ECHO Copying assets from %AssetSource%
XCOPY "%AssetSource%" "assets\" /S /Y /Q
CALL :deploy Debug
CALL :deploy Release
GOTO:eof

:deploy
ECHO Deploying %~1
SET ConfigPath=bin\%~1
MKLINK "%ConfigPath%\assets\" "assets\" /J
MKLINK "%ConfigPath%\glfw3.dll" "libs\glfw-3.0.4\lib\%~1\glfw3.dll" /H
MKLINK "%ConfigPath%\OpenAL32.dll" "libs\openal-soft-1.15.1\lib\Win32\%~1\OpenAL32.dll" /H
IF %~1==Debug (
	MKLINK "%ConfigPath%\glew32d.dll" "libs\glew-1.10.0\bin\%~1\Win32\glew32d.dll" /H
)
IF %~1==Release (
	MKLINK "%ConfigPath%\glew32.dll" "libs\glew-1.10.0\bin\%~1\Win32\glew32.dll" /H
)
GOTO:eof