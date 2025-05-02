@echo off
:: gg - Automated installation script for Windows
:: This script checks for and installs required dependencies before building the project

echo ========================================
echo gg - Git Learning CLI Tool Installer
echo ========================================
echo.

:: ADMIN CHECK REMOVED

:: Check for MSYS2/MinGW - we need this for C++ and ncurses on Windows
echo Checking for MSYS2/MinGW...
if exist "C:\msys64\mingw64\bin\g++.exe" (
    echo [OK] MSYS2/MinGW found.
    call set "PATH=C:\msys64\mingw64\bin;%PATH%"
) else if exist "C:\msys64\mingw32\bin\g++.exe" (
    echo [OK] MSYS2/MinGW found.
    call set "PATH=C:\msys64\mingw32\bin;%PATH%"
) else (
    echo [NOT FOUND] MSYS2/MinGW not found.
    echo.
    echo Installing MSYS2 ^(this will download about 100MB^)...
    powershell -Command "Invoke-WebRequest -Uri https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20210725.exe -OutFile msys2-installer.exe"
    start /wait msys2-installer.exe
    del msys2-installer.exe
    echo.
    echo Running initial MSYS2 update...
    C:\msys64\usr\bin\bash -lc "pacman -Syu --noconfirm"
    echo Installing MinGW toolchain and ncurses...
    C:\msys64\usr\bin\bash -lc "pacman -S --noconfirm mingw-w64-x86_64-toolchain mingw-w64-x86_64-ncurses make"
)

echo.
echo Building gg...
call make clean
call make

echo.
if %errorLevel% equ 0 (
    echo.
    echo ========================================
    echo [OK] gg has been successfully installed!
    echo ========================================
    echo.
    echo Now you can run gg using:
    echo   bin\gg [command] [options]
    echo.
    echo Try some of these commands to get started:
    echo   bin\gg bruh                    # Initialize a new repository
    echo   bin\gg add [file]              # Stage a file
    echo   bin\gg kermit -m "message"     # Commit changes
    echo   bin\gg learn dag               # Learn about DAGs
    echo.
    echo For more information, see the README.md file.
) else (
    echo [ERROR] Build failed. Please check the error messages above.
    exit /b 1
)

pause
