#include <algorithm>
#include <array>
#include <complex>
#include <iostream>

#include <raylib.h>

using namespace std::complex_literals;

using ctype = float;
using cmp = std::complex<ctype>;

// f(z) = z^2 + c
auto mandel(cmp z, cmp c)
{
    return std::pow(z, 2) + c;
}

const int screenWidth = 1280;
const int screenHeight = 960;
const uint8_t N = 250;

// Check if we diverge and are thus in the Mandelbrot set
uint8_t mandelRes(cmp c, cmp z = 0)
{
    for (uint8_t n = 0; n < N; ++n)
    {
        z = mandel(z, c);
        if (std::abs(z) > 2)
        { // Explodes
            return std::numeric_limits<uint8_t>::max() * n / N;
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

auto draw(std::array<uint8_t, screenHeight * screenWidth> &buffer, cmp z = 0)
{
    for (int x = 0; x < screenWidth; ++x)
    {
        for (int y = 0; y < screenHeight; ++y)
        {
            buffer[x + screenWidth * y] = mandelRes(cmp(scaleX(static_cast<ctype>(x)), scaleY(static_cast<ctype>(y))), z);
        }
    }
}

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing");
    ToggleFullscreen();

    const int fps = 30;
    SetTargetFPS(fps);

    auto buffer = std::make_unique<std::array<uint8_t, screenHeight * screenWidth>>();
    draw(*buffer);

    int frame = 0;
    int frame2 = 0;
    int frame3 = 0;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(BLACK);

        //if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            //z += 0.01 + 0i;
            //draw(*buffer, z);
        }

        for (int x = 0; x < screenWidth; ++x)
        {
            for (int y = 0; y < screenHeight; ++y)
            {
                unsigned char val = buffer->at(x + screenWidth * y);
                DrawPixel(x, y, Color{(unsigned char)(val * frame3 % 256), (unsigned char)(val + frame % 256), (unsigned char)(val - frame2 % 256), 255});
            }
        }
        frame++;
        frame2 += 3;
        frame3 += 7;

        DrawText("Mandelbrot set test", 20, 20, 20, DARKGRAY);
        DrawText(std::to_string(GetFPS()).c_str(), screenWidth - 100, 20, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}