#include <algorithm>
#include <array>
#include <complex>
#include <iostream>

#include <raylib.h>

using ctype = float;
using cmp = std::complex<ctype>;

// f(z) = z^2 + c
auto mandel(cmp z, cmp c)
{
    return std::pow(z, 2) + c;
}

const int screenWidth = 1400;
const int screenHeight = 800;
const int N = 50;

// Check if we diverge and are thus in the Mandelbrot set
unsigned char mandelRes(cmp c)
{
    cmp z = 0;
    for (int n = 0; n < N; ++n)
    {
        z = mandel(z, c);
        if(std::abs(z) > 2) { // Explodes
            return 256 * n/N;
        }
    }
    return 0;
}


// Scale to [-2, 1]
ctype scaleX(ctype x)
{
    return 3 * (x / screenWidth) - 2;
}

// Scale to [-1, 1]
ctype scaleY(ctype y)
{
    return 2 * (y / screenHeight) - 1;
}

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing");

    const int fps = 60;
    SetTargetFPS(fps);

    auto buffer = std::make_unique<std::array<char,screenHeight*screenWidth>>();
    for (int x = 0; x < screenWidth; ++x)
    {
        for (int y = 0; y < screenHeight; ++y)
        {
            buffer->at(x + screenWidth * y) = mandelRes(cmp(scaleX(x), scaleY(y)));
        }
    }

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int x = 0; x < screenWidth; ++x)
        {
            for (int y = 0; y < screenHeight; ++y)
            {
                unsigned char val = buffer->at(x + screenWidth * y);
                DrawPixel(x, y, Color{val,val,val,255});
            }
        }

        DrawText("Mandelbrot set test", 20, 20, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}