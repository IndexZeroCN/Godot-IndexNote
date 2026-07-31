@echo off
setlocal enabledelayedexpansion

REM ============================================================
REM  GDExtension Full Build Script
REM  Builds all Windows + Android targets
REM ============================================================

if not defined ANDROID_HOME set ANDROID_HOME=E:\AndroidSDK

set PASS=0
set FAIL=0
set TOTAL=0

echo.
echo ============================================================
echo  Phase 1: Windows Builds
echo ============================================================
echo.

REM --- Windows targets ---
for %%t in (editor template_debug template_release) do (
    set /a TOTAL+=1
    echo [%TOTAL%] Building: platform=windows target=%%t arch=x86_64
    scons platform=windows target=%%t arch=x86_64 -j%NUMBER_OF_PROCESSORS% --disk-cache=none
    if errorlevel 1 (
        echo [FAIL] platform=windows target=%%t
        set /a FAIL+=1
    ) else (
        echo [PASS] platform=windows target=%%t
        set /a PASS+=1
    )
    echo.
)

echo ============================================================
echo  Phase 2: Android Builds
echo ============================================================
echo.

REM Check NDK
if not exist "%ANDROID_HOME%\ndk\23.2.8568313\" (
    echo [WARNING] NDK 23.2.8568313 not found at %ANDROID_HOME%\ndk\23.2.8568313\
    echo Please install NDK 23.2.8568313 via Android Studio SDK Manager
    echo Skipping Android builds...
    goto :summary
)

REM --- Android targets ---
for %%t in (template_debug template_release) do (
    for %%a in (arm64 arm32 x86_64) do (
        set /a TOTAL+=1
        echo [!TOTAL!] Building: platform=android target=%%t arch=%%a
        scons platform=android target=%%t arch=%%a ANDROID_HOME=%ANDROID_HOME% -j%NUMBER_OF_PROCESSORS%
        if errorlevel 1 (
            echo [FAIL] platform=android target=%%t arch=%%a
            set /a FAIL+=1
        ) else (
            echo [PASS] platform=android target=%%t arch=%%a
            set /a PASS+=1
        )
        echo.
    )
)

:summary
echo ============================================================
echo  Build Summary
echo ============================================================
echo   Total : %TOTAL%
echo   Passed: %PASS%
echo   Failed: %FAIL%
echo.

REM List outputs
echo Output files:
echo --- Windows ---
dir /b addons\index-note\bin\*.windows.*.dll 2>nul
echo.
echo --- Android ---
dir /b addons\index-note\bin\*.android.*.so 2>nul

if %FAIL% gtr 0 (
    echo.
    echo Some builds FAILED. Check output above for errors.
    exit /b 1
)

echo.
echo All builds completed successfully!
exit /b 0
