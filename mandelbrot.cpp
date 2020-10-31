#include <algorithm>
#include <array>
#include <complex>
#include <execution>
#include <iostream>
#include <memory>

#include <raylib.h>

using namespace std::complex_literals;

using ctype = long double;
using cmp = std::complex<ctype>;

// f(z) = z^2 + c
auto mandel(cmp z, cmp c)
{
    return std::pow(z, 2) + c;
}

constexpr int screenWidth = 1024*2;
constexpr int screenHeight = 768*2;
constexpr uint8_t N = 50;
using screen = std::array<uint8_t, screenHeight * screenWidth>;

// Check if we diverge and are thus in the Mandelbrot set
uint8_t mandelRes(cmp c, cmp z = 0)
{
    for (uint8_t n = 0; n < N; ++n)
    {
        z = mandel(z, c);
        if (std::norm(z) > 4) // The algorithm is std::abs(z) > 2, but this is faster
        {                     // Explodes
            return std::numeric_limits<uint8_t>::max() * n / N;
        }
    }
    return 0;
}

ctype x_offset = 0;
ctype y_offset = 0;
ctype zoom = 1;

// Scale to [-2, 1]
ctype scaleX(ctype x)
{
    return zoom * 3 * (x / screenWidth) - zoom * 2 + x_offset;
}

// Scale to [-1, 1]
ctype scaleY(ctype y)
{
    return zoom * 2 * (y / screenHeight) - zoom * 1 + y_offset;
}

auto calc(screen &buffer, cmp z = 0)
{
    std::transform(std::execution::par_unseq, buffer.begin(), buffer.end(), buffer.begin(), [&](auto &p) {
        const size_t i = &p - &buffer[0];
        const auto x = i % screenWidth;
        const auto y = i / screenHeight;
        return mandelRes(cmp(scaleX(static_cast<ctype>(x)), scaleY(static_cast<ctype>(y))), z);
    });
}

void draw(int xp, int yp, int w, int h, int frame, const screen &buffer)
{
    for (int x = xp; x < xp + w; ++x)
    {
        for (int y = yp; y < yp + h; ++y)
        {
            unsigned char val = buffer.at(x + screenWidth * y);
            DrawPixel(x, y, Color{(unsigned char)(val * frame % 256), (unsigned char)(val + frame % 256), (unsigned char)(val - frame % 256), 255});
        }
    }
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Mandelbrot testing");
    ToggleFullscreen();

    constexpr int fps = 60;
    SetTargetFPS(fps);

    auto buffer = std::make_unique<screen>();

    // cmp z = 0;
    bool first = true;
    int frame = 0;
    bool changed = true;
    ClearBackground(BLACK);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        changed = true;
        if (IsKeyDown(KEY_UP))
        {
            y_offset -= zoom * 0.05f;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            y_offset += zoom * 0.05f;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            x_offset += zoom * 0.05f;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            x_offset -= zoom * 0.05f;
        }
        else if (IsKeyDown(KEY_KP_ADD))
        {
            zoom /= 1.02;
        }
        else if (IsKeyDown(KEY_KP_SUBTRACT))
        {
            zoom *= 1.02;
        }
        else if (!first)
        {
            changed = false;
        }
        first = false;

        BeginDrawing();
        if (changed)
        {
            calc(*buffer);
            draw(0, 0, screenWidth, screenHeight, frame, *buffer);
            DrawText("Mandelbrot set test", 20, 20, 20, DARKGRAY);
        }
        else
        {
#ifdef _WIN32
            // For some reason we need to draw all on win, but not lin or we flicker
            draw(0, 0, screenWidth, screenHeight, frame, *buffer);
#else
            draw(screenWidth - 100, 20, 100, 20, frame, *buffer);
#endif
        }

        DrawText(std::to_string(GetFPS()).c_str(), screenWidth - 100, 20, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}