@echo off
REM Build GDExtension for Android
REM Usage: build_android.bat [debug|release|all]
REM
REM Prerequisites:
REM   - NDK 23.2.8568313 installed in %%ANDROID_HOME%%\ndk\23.2.8568313\
REM   - scons installed (pip install scons)

setlocal enabledelayedexpansion

if not defined ANDROID_HOME set ANDROID_HOME=E:\AndroidSDK

set MODE=%1
if "%MODE%"=="" set MODE=all

if "%MODE%"=="debug" (
    set TARGETS=template_debug
) else if "%MODE%"=="release" (
    set TARGETS=template_release
) else if "%MODE%"=="all" (
    set TARGETS=template_debug template_release
) else (
    echo Usage: %0 [debug^|release^|all]
    exit /b 1
)

for %%t in (%TARGETS%) do (
    for %%a in (arm64 arm32 x86_64) do (
        echo ========================================
        echo Building: platform=android target=%%t arch=%%a
        echo ========================================
        scons platform=android target=%%t arch=%%a ANDROID_HOME=%ANDROID_HOME% -j%NUMBER_OF_PROCESSORS%
        if errorlevel 1 exit /b 1
    )
)

echo.
echo Android build complete!
echo Output files in: addons/index-note/bin\
dir addons\index-note\bin\*.android.*.so 2>nul
