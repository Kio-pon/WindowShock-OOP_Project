@echo off
echo Building WindowShock...
g++ src/*.cpp  -I"include" -I"C:/msys64/msys64/include" -L"C:/msys64/msys64/lib" -DSFML_STATIC -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype -lharfbuzz -lopengl32 -lwinmm -lgdi32 -o windowshock
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)
echo Build successful! Running game...
windowshock.exe
