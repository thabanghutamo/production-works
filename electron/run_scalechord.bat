@echo off
REM ScaleChord VST3 Plugin - Windows Launcher
REM This batch script launches the ScaleChord application
REM Place this file in the same directory as the ScaleChord app

setlocal enabledelayedexpansion

REM Get script directory
set SCRIPT_DIR=%~dp0

REM Check if electron-exe exists
if exist "%SCRIPT_DIR%ScaleChord Plugin.exe" (
    echo Launching ScaleChord...
    start "" "%SCRIPT_DIR%ScaleChord Plugin.exe" %*
    exit /b 0
) else if exist "%SCRIPT_DIR%dist\win-unpacked\ScaleChord Plugin.exe" (
    echo Launching ScaleChord...
    start "" "%SCRIPT_DIR%dist\win-unpacked\ScaleChord Plugin.exe" %*
    exit /b 0
) else (
    echo.
    echo ============================================
    echo ERROR: ScaleChord executable not found!
    echo ============================================
    echo.
    echo Expected to find:
    echo   - "%SCRIPT_DIR%ScaleChord Plugin.exe"
    echo   - "%SCRIPT_DIR%dist\win-unpacked\ScaleChord Plugin.exe"
    echo.
    echo SOLUTION: Build the EXE first by running:
    echo   npm install
    echo   npm run build:win
    echo.
    pause
    exit /b 1
)
