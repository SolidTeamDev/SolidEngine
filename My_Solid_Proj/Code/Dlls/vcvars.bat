@echo off
setlocal enabledelayedexpansion

for /f "usebackq tokens=*" %%i in (`D:\Project\SolidEngine\Build\Release\Dlls\vswhere.exe -latest -find VC\Auxiliary\Build\vcvarsall.bat`) do (
  echo %%i
  exit /b !errorlevel!
)