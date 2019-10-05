#include <algorithm>
#include <array>
#include <execution>
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

const int screenWidth = 1920;
const int screenHeight = 1280;
const uint8_t N = 250;

// Check if we diverge and are thus in the Mandelbrot set
uint8_t mandelRes(cmp c, cmp z = 0)
{
    for (uint8_t n = 0; n < N; ++n)
    {
        z = mandel(z, c);
        if (std::norm(z) > 4) // The algorithm is std::abs(z) > 2, but this is faster
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
    std::transform(std::execution::par, buffer.begin(), buffer.end(), buffer.begin(), [&](auto &p) {
        const size_t i = &p - &buffer[0];
        const auto x = i % screenWidth;
        const auto y = i / screenHeight;
        return mandelRes(cmp(scaleX(static_cast<ctype>(x)), scaleY(static_cast<ctype>(y))), z);
    });
}

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic shapes drawing");
    //ToggleFullscreen();

    const int fps = 60;
    SetTargetFPS(fps);

    auto buffer = std::make_unique<std::array<uint8_t, screenHeight * screenWidth>>();
    draw(*buffer);

    cmp z = 0;
    int frame = 0;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(BLACK);

        //if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            draw(*buffer, z += 0.0001 + 0i);
        }

        for (int x = 0; x < screenWidth; ++x)
        {
            for (int y = 0; y < screenHeight; ++y)
            {
                unsigned char val = buffer->at(x + screenWidth * y);
                DrawPixel(x, y, Color{(unsigned char)(val * frame % 256), (unsigned char)(val + frame % 256), (unsigned char)(val - frame % 256), 255});
            }
        }
        frame++;

        DrawText("Mandelbrot set test", 20, 20, 20, DARKGRAY);
        DrawText(std::to_string(GetFPS()).c_str(), screenWidth - 100, 20, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}