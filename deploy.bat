@ECHO off

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
MKLINK "%ConfigPath%\Sounds\" "assets\Sounds\" /J
:: Shaders
MKLINK "%ConfigPath%\Shaders\" "src\Shaders\" /J
:: DLLs
COPY "libs\glfw-3.0.4\lib\%Configuration%\glfw3.dll" "%ConfigPath%\glfw3.dll"
COPY "libs\FMOD\lib\fmodex.dll" "%ConfigPath%\fmodex.dll"
IF %~1==Debug (
	COPY "libs\glew-1.10.0\bin\%Configuration%\Win32\glew32d.dll" "%ConfigPath%\glew32d.dll"
)
IF %~1==Release (
	COPY "libs\glew-1.10.0\bin\%Configuration%\Win32\glew32.dll" "%ConfigPath%\glew32.dll"
)
GOTO:eof