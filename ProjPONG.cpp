#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x, y;
    float speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle((int)x, (int)y, radius, Yellow);
    }

    void Update()
    {
        x += speed_x;
        y += speed_y;

        if (y + radius >= (float)GetScreenHeight() || y - radius <= 0.0f)
        {
            speed_y *= -1.0f;
        }

        if (x + radius >= (float)GetScreenWidth())
        {
            cpu_score++;
            ResetBall();
        }
        if (x - radius <= 0.0f)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x = GetScreenWidth() / 2.0f;
        y = GetScreenHeight() / 2.0f;

        float speed_choices[2] = { -1.0f, 1.0f };
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y <= 0.0f)
            y = 0.0f;
        if (y + height >= (float)GetScreenHeight())
            y = GetScreenHeight() - (float)height;
    }

public:
    float x, y;
    int width, height;
    float speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{ x, y, (float)width, (float)height }, 0.8f, 0, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
            y -= speed;
        if (IsKeyDown(KEY_DOWN))
            y += speed;

        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(float ball_y)
    {
        if (y + height / 2.0f > ball_y)
            y -= speed;
        if (y + height / 2.0f <= ball_y)
            y += speed;

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    cout << "Starting The Game\n";

    const int screen_width = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2.0f;
    ball.y = screen_height / 2.0f;
    ball.speed_x = 7.0f;
    ball.speed_y = 7.0f;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10.0f;
    player.y = screen_height / 2.0f - player.height / 2.0f;
    player.speed = 6.0f;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10.0f;
    cpu.y = screen_height / 2.0f - cpu.height / 2.0f;
    cpu.speed = 6.0f;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Collisions
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, (float)ball.radius, Rectangle{ player.x, player.y, (float)player.width, (float)player.height }))
        {
            ball.speed_x *= -1.0f;
        }

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, (float)ball.radius, Rectangle{ cpu.x, cpu.y, (float)cpu.width, (float)cpu.height }))
        {
            ball.speed_x *= -1.0f;
        }

        // Drawing
        ClearBackground(Dark_Green);

        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150.0f, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);

        ball.Draw();
        cpu.Draw();
        player.Draw();

        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}