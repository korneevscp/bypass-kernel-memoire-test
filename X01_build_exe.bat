@echo off
cd /d "%~dp0"

echo === build X01 ===
echo.

rem je cherche gcc un peu partout parce que je me souviens jamais ou il est
set GCC=

where gcc >nul 2>nul && set GCC=gcc

if not defined GCC if exist "C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" set GCC="C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe"
if not defined GCC if exist "C:\Program Files (x86)\CodeBlocks\MinGW\bin\gcc.exe" set GCC="C:\Program Files (x86)\CodeBlocks\MinGW\bin\gcc.exe"
if not defined GCC if exist "C:\MinGW\bin\gcc.exe" set GCC="C:\MinGW\bin\gcc.exe"
if not defined GCC if exist "C:\TDM-GCC-64\bin\gcc.exe" set GCC="C:\TDM-GCC-64\bin\gcc.exe"
if not defined GCC if exist "C:\msys64\mingw64\bin\gcc.exe" set GCC="C:\msys64\mingw64\bin\gcc.exe"

if not defined GCC (
    echo pas trouve gcc, install codeblocks ou met gcc dans le path
    pause
    exit /b 1
)

echo gcc trouve : %GCC%
echo.

%GCC% -o X01_shellcode_bash.exe X01_shellcode_bash.c
if errorlevel 1 goto err

%GCC% -o X01_exec_bash.exe X01_exec_bash.c
if errorlevel 1 goto err

%GCC% -o X01_kernel_adresses.exe X01_kernel_adresses.c
if errorlevel 1 goto err

echo ok c'est compilé
dir /b *.exe
echo.
echo (pour tester le shellcode faut du linux : wsl2 ou vm)
pause
exit /b 0

:err
echo bouffe de compilation, regarde en haut
pause
exit /b 1
