@ECHO off

:: Set custom path to asset folder here
SET AssetSource=%UserProfile%\Google Drive\Fisk_ Litet Spelprojekt\Assets

ECHO Copying assets from %AssetSource%
RMDIR "assets\" /S /Q
XCOPY "%AssetSource%" "assets\" /E /Y /Q
CALL :deploy Debug
CALL :deploy Release
GOTO:eof

:deploy
SET Configuration=%~1
SET ConfigPath=bin\%~1
ECHO Deploying %Configuration%
:: Asset folders
MKLINK "%ConfigPath%\Models\" "assets\Models" /J
MKLINK "%ConfigPath%\Textures\" "assets\Textures\" /J
:: Shaders
MKLINK "%ConfigPath%\Shaders\" "src\Shaders\" /J
:: DLLs
MKLINK "%ConfigPath%\glfw3.dll" "libs\glfw-3.0.4\lib\%Configuration%\glfw3.dll" /H
MKLINK "%ConfigPath%\OpenAL32.dll" "libs\openal-soft-1.15.1\lib\Win32\%Configuration%\OpenAL32.dll" /H
IF %~1==Debug (
	MKLINK "%ConfigPath%\glew32d.dll" "libs\glew-1.10.0\bin\%Configuration%\Win32\glew32d.dll" /H
)
IF %~1==Release (
	MKLINK "%ConfigPath%\glew32.dll" "libs\glew-1.10.0\bin\%Configuration%\Win32\glew32.dll" /H
)
GOTO:eof