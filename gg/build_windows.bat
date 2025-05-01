@echo off
echo Building gg for Windows...

:: Check if we have MinGW or MSVC available
where g++ >nul 2>&1
if %ERRORLEVEL% == 0 (
    echo Found g++ compiler, using MinGW...
    goto use_mingw
) else (
    echo g++ not found, checking for MSVC...
    where cl >nul 2>&1
    if %ERRORLEVEL% == 0 (
        echo Found cl compiler, using MSVC...
        goto use_msvc
    ) else (
        echo No C++ compiler found.
        echo Please install MinGW-w64 or Visual Studio with C++ components.
        exit /b 1
    )
)

:use_mingw
echo Creating directories...
mkdir obj 2>nul
mkdir bin 2>nul

echo Compiling with MinGW...
g++ -std=c++17 -Wall -I./include -DNCURSES_DISABLED -o obj/main.o -c src/main.cpp
g++ -std=c++17 -Wall -I./include -DNCURSES_DISABLED -o obj/stubs.o -c src/stubs.cpp

echo Linking...
g++ -o bin/gg.exe obj/main.o obj/stubs.o -lpthread

if exist bin\gg.exe (
    echo Build successful! Binary located at bin\gg.exe
    echo.
    echo Try running: bin\gg.exe --help
) else (
    echo Build failed!
)
exit /b 0

:use_msvc
echo Creating directories...
mkdir obj 2>nul
mkdir bin 2>nul

echo Compiling with MSVC...
cl /EHsc /std:c++17 /I".\include" /D"NCURSES_DISABLED" /Fo"obj\\" /c src\main.cpp
cl /EHsc /std:c++17 /I".\include" /D"NCURSES_DISABLED" /Fo"obj\\" /c src\stubs.cpp

echo Linking...
link /OUT:"bin\gg.exe" obj\main.obj obj\stubs.obj

if exist bin\gg.exe (
    echo Build successful! Binary located at bin\gg.exe
    echo.
    echo Try running: bin\gg.exe --help
) else (
    echo Build failed!
)
exit /b 0
