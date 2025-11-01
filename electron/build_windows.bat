@echo off
REM =============================================================
REM ScaleChord VST3 Plugin - Automated Windows Builder
REM =============================================================
REM This script builds the Windows EXE installer and portable executable
REM Run this from the electron/ directory
REM =============================================================

setlocal enabledelayedexpansion

echo.
echo ╔═══════════════════════════════════════════════════════════╗
echo ║       ScaleChord VST3 Plugin - Windows Builder             ║
echo ╚═══════════════════════════════════════════════════════════╝
echo.

REM Check if running from correct directory
if not exist "package.json" (
    echo ERROR: package.json not found!
    echo Please run this script from the electron/ directory
    pause
    exit /b 1
)

REM Check for Node.js
echo Checking Node.js...
node --version >nul 2>&1
if errorlevel 1 (
    echo.
    echo ❌ Node.js not found!
    echo Please install Node.js from: https://nodejs.org
    echo.
    pause
    exit /b 1
) else (
    for /f "tokens=*" %%a in ('node --version') do (
        echo ✓ Node.js found: %%a
    )
)

REM Check for npm
echo.
echo Checking npm...
npm --version >nul 2>&1
if errorlevel 1 (
    echo.
    echo ❌ npm not found!
    echo Please install npm or reinstall Node.js
    echo.
    pause
    exit /b 1
) else (
    for /f "tokens=*" %%a in ('npm --version') do (
        echo ✓ npm found: %%a
    )
)

REM Check for vst3_plugin directory
echo.
echo Checking VST3 plugin...
if not exist "vst3_plugin" (
    echo.
    echo ❌ vst3_plugin directory not found!
    echo Please ensure the plugin binary is copied to: electron/vst3_plugin/
    echo.
    pause
    exit /b 1
) else (
    echo ✓ vst3_plugin directory found
)

REM Install dependencies
echo.
echo Installing npm dependencies...
call npm install --legacy-peer-deps
if errorlevel 1 (
    echo.
    echo ❌ Failed to install dependencies!
    pause
    exit /b 1
) else (
    echo ✓ Dependencies installed successfully
)

REM Build for Windows
echo.
echo Building Windows EXE...
echo.
call npm run build:win
if errorlevel 1 (
    echo.
    echo ❌ Build failed!
    echo Check the output above for errors
    pause
    exit /b 1
) else (
    echo.
    echo ✓ Build completed successfully!
)

REM Show results
echo.
echo ╔═══════════════════════════════════════════════════════════╗
echo ║                   BUILD COMPLETE! 🎉                       ║
echo ╚═══════════════════════════════════════════════════════════╝
echo.
echo Your Windows EXE files are ready:
echo.
if exist "dist\ScaleChord Setup 1.0.0.exe" (
    echo   ✓ dist\ScaleChord Setup 1.0.0.exe (Installer)
    for %%A in ("dist\ScaleChord Setup 1.0.0.exe") do (
        set size=%%~zA
        echo     Size: !size! bytes
    )
) else (
    echo   ℹ dist\ScaleChord Setup 1.0.0.exe (Look for this file)
)

echo.
if exist "dist\ScaleChord 1.0.0 portable.exe" (
    echo   ✓ dist\ScaleChord 1.0.0 portable.exe (Portable)
    for %%A in ("dist\ScaleChord 1.0.0 portable.exe") do (
        set size=%%~zA
        echo     Size: !size! bytes
    )
) else (
    echo   ℹ dist\ScaleChord 1.0.0 portable.exe (Look for this file)
)

echo.
echo NEXT STEPS:
echo   1. Test the installer: double-click the .exe file
echo   2. Verify plugin appears in your DAW
echo   3. Upload to GitHub: https://github.com/your-repo/releases
echo   4. Share with users!
echo.
echo NOTES:
echo   - Installer: Full install experience with menu shortcuts
echo   - Portable: Run from anywhere, no installation needed
echo   - Both include the VST3 plugin and app
echo.
pause
