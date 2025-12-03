@echo off
echo Building WindowShock...


:: Check if mingw32-make exists
where mingw32-make >nul 2>nul
if %errorlevel% neq 0 (
    echo mingw32-make not found! Falling back to slow build...
    g++ src/*.cpp -I"include" -I"C:/msys64/msys64/include" -L"C:/msys64/msys64/lib" -DSFML_STATIC -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lharfbuzz -lopengl32 -lwinmm -lgdi32 -o windowshock
) else (
    :: Use make with parallel jobs equal to processor count
    mingw32-make -j%NUMBER_OF_PROCESSORS%
)

if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)
echo Build successful! Running game...
windowshock.exe
