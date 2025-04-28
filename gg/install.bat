@echo off
:: gg - Automated installation script for Windows
:: This script checks for and installs required dependencies before building the project

echo ========================================
echo gg - Git Learning CLI Tool Installer
echo ========================================
echo.

:: Check if running as administrator
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo This script requires administrator privileges to install dependencies.
    echo Please right-click this file and select "Run as administrator".
    pause
    exit /b 1
)

:: Check for MSYS2/MinGW - we need this for C++ and ncurses on Windows
echo Checking for MSYS2/MinGW...
if exist "C:\msys64\mingw64\bin\g++.exe" (
    echo [OK] MSYS2/MinGW found.
    set PATH=C:\msys64\mingw64\bin;%PATH%
) else if exist "C:\msys64\mingw32\bin\g++.exe" (
    echo [OK] MSYS2/MinGW found.
    set PATH=C:\msys64\mingw32\bin;%PATH%
) else (
    echo [NOT FOUND] MSYS2/MinGW not found.
    echo.
    echo *** Installing MSYS2 (this will download about 100MB) ***
    echo.
    
    :: Use PowerShell to download MSYS2 installer
    powershell -Command "& {$ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest -Uri 'https://github.com/msys2/msys2-installer/releases/download/2023-05-26/msys2-x86_64-20230526.exe' -OutFile '%TEMP%\msys2-installer.exe'}"
    
    if %errorLevel% neq 0 (
        echo Failed to download MSYS2 installer.
        echo Please install MSYS2 manually from https://www.msys2.org/
        pause
        exit /b 1
    )
    
    echo Running MSYS2 installer...
    echo Please follow the installer prompts. Use default settings.
    %TEMP%\msys2-installer.exe
    
    echo.
    echo After MSYS2 installation is complete, we need to install required packages.
    echo A new MSYS2 terminal will open. Please run the following commands:
    echo.
    echo    pacman -Syu --noconfirm
    echo    pacman -S --noconfirm mingw-w64-x86_64-gcc mingw-w64-x86_64-ncurses make
    echo.
    echo After running these commands, close the MSYS2 terminal and run this script again.
    echo.
    
    :: Start MSYS2 shell for the user
    start C:\msys64\msys2.exe
    
    pause
    exit /b 1
)

:: Check for G++ compiler
echo Checking for G++ compiler...
g++ --version >nul 2>&1
if %errorLevel% equ 0 (
    echo [OK] G++ compiler found.
) else (
    echo [NOT FOUND] G++ compiler not found.
    echo Installing G++ via MSYS2...
    
    C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm mingw-w64-x86_64-gcc"
    
    if %errorLevel% neq 0 (
        echo Failed to install G++.
        echo Please open MSYS2 and run: pacman -S --noconfirm mingw-w64-x86_64-gcc
        pause
        exit /b 1
    )
    
    echo [OK] G++ installed.
    set PATH=C:\msys64\mingw64\bin;%PATH%
)

:: Check for ncurses library
echo Checking for ncurses library...
if exist "C:\msys64\mingw64\include\ncurses\curses.h" (
    echo [OK] ncurses library found.
) else (
    echo [NOT FOUND] ncurses library not found.
    echo Installing ncurses via MSYS2...
    
    C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm mingw-w64-x86_64-ncurses"
    
    if %errorLevel% neq 0 (
        echo Failed to install ncurses.
        echo Please open MSYS2 and run: pacman -S --noconfirm mingw-w64-x86_64-ncurses
        pause
        exit /b 1
    )
    
    echo [OK] ncurses installed.
)

:: Check for make
echo Checking for make...
make --version >nul 2>&1
if %errorLevel% equ 0 (
    echo [OK] make found.
) else (
    echo [NOT FOUND] make not found.
    echo Installing make via MSYS2...
    
    C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm make"
    
    if %errorLevel% neq 0 (
        echo Failed to install make.
        echo Please open MSYS2 and run: pacman -S --noconfirm make
        pause
        exit /b 1
    )
    
    echo [OK] make installed.
)

:: Create directories if they don't exist
echo Setting up build directories...
if not exist "obj" mkdir obj
if not exist "bin" mkdir bin

:: Build the project
echo Building gg...
make clean
make

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
