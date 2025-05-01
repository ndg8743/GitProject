@echo off
echo Building gg for Windows...

REM Create directories
if not exist bin mkdir bin
if not exist obj mkdir obj

REM Try to compile with g++ (MinGW)
echo Attempting to build with g++ (MinGW)...
g++ -std=c++17 -Wall -Iinclude -DNCURSES_DISABLED src\main.cpp src\stubs.cpp -o bin\gg.exe

if exist bin\gg.exe (
    echo Build successful!
    echo Running gg --help:
    echo ====================================
    bin\gg.exe --help
    goto :eof
)

REM Try with MSVC if g++ failed
echo Attempting to build with MSVC...
cl /nologo /Febin\gg.exe /Foobj\ /std:c++17 /EHsc /Iinclude /DNCURSES_DISABLED src\main.cpp src\stubs.cpp

if exist bin\gg.exe (
    echo Build successful!
    echo Running gg --help:
    echo ====================================
    bin\gg.exe --help
    goto :eof
)

echo Failed to build with available compilers.
echo Please ensure you have MinGW-w64 or MSVC installed.
