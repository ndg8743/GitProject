@echo off
:: gg - Automated installation script for Windows
:: Supports local MSYS2 installation as a 'msys2' subfolder or global C:\msys64

echo ========================================
echo gg - Git Learning CLI Tool Installer
echo ========================================
echo.

:: ADMIN CHECK REMOVED

:: Determine script root folder
set "ROOT=%~dp0"

:: Check for MSYS2/MinGW
echo Checking for MSYS2/MinGW...
if exist "%ROOT%msys2\mingw64\bin\g++.exe" (
    echo [OK] Local MSYS2 found in %ROOT%msys2.
    call set "PATH=%ROOT%msys2\mingw64\bin;%PATH%"
) else if exist "C:\msys64\mingw64\bin\g++.exe" (
    echo [OK] Global MSYS2 found (C:\msys64).
    call set "PATH=C:\msys64\mingw64\bin;%PATH%"
) else (
    echo [NOT FOUND] MSYS2 not found.
    echo.
    echo Please install MSYS2 in one of the following ways:
    echo 1) Download and extract the MSYS2 installer into a subfolder named 'msys2' here
    echo 2) Or install system-wide to C:\msys64
    pause
    exit /b 1
)

:: Now build the gg tool

echo.
echo Building gg...
call make clean
call make

echo.
if %errorLevel% equ 0 (
    echo.
    echo ========================================
    echo [OK] gg has been successfully built!
    echo ========================================
    echo.
    echo To run, use: bin\gg [command] [options]
    echo See README.md for details.
) else (
    echo [ERROR] Build failed. Please check the messages above.
    exit /b 1
)

echo.
pause
