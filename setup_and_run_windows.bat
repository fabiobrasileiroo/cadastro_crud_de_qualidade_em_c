@echo off
SETLOCAL

REM Check if MSYS2 is installed
IF NOT EXIST "%ProgramFiles%\MSYS2" (
    echo "MSYS2 not found. Installing..."
    powershell -Command "Start-Process msys2-x86_64-20210725.exe -ArgumentList '/SILENT' -Wait"
)

REM Update MSYS2 packages
"%ProgramFiles%\MSYS2\usr\bin\bash" -c "pacman -Syuu --noconfirm"

REM Install GTK+ 3.0
"%ProgramFiles%\MSYS2\usr\bin\bash" -c "pacman -S --noconfirm mingw-w64-x86_64-gtk3"

REM Set MSYS2 environment variables
SET MSYS2_PATH_TYPE=inherit
SET PATH=%PATH%;%ProgramFiles%\MSYS2\mingw64\bin

REM Compile the C program
gcc melhordeTodos.c -o program `pkg-config --cflags --libs gtk+-3.0`

REM Check if the compilation was successful
IF %ERRORLEVEL% EQU 0 (
    echo "Compilation successful. Executable created: program"
    program.exe
) ELSE (
    echo "Compilation failed."
)

ENDLOCAL
