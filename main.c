#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "raylib.h"
#include "zaitypes.h"


#define WIDTH                        800
#define HEIGHT                      600
#define NUM_OF_PADDLES      2
#define PADDLE_WIDTH          10
#define PADDLE_HEIGHT        100
#define PADDLE_Y                  (HEIGHT / 2 - 50)

typedef struct  {
    Rectangle rect;
    Color color;
} Paddle;

typedef struct  {
    Vector2 position;
    f32 radius;
    Color color;
} Ball;

const f32 VELOCITY = 3.5f;

void paddle_movement(Paddle *paddle)
{   

    

    if(IsKeyUp(KEY_UP))  {

        printf("Moving up.\n");
        paddle->rect.y += VELOCITY;

    }
    
    if(IsKeyUp(KEY_DOWN)) {
    
        printf("Moving down.\n");
        paddle->rect.y -= VELOCITY;
    
    }

}

void detect_paddle_collision(Paddle *paddle)
{
    // check for top screen collision
    if(paddle->rect.y  < 0)
    {
        paddle->rect.y = 0;
    }

    // check for bottom screen collision
    if(paddle->rect.y > HEIGHT - PADDLE_HEIGHT)
    {
        paddle->rect.y = HEIGHT - PADDLE_HEIGHT;
    }

}


void ball_movement(Ball *ball)
{

     srand(time(NULL));

    // Generate a random angle between 30 and 150 degrees
    i32 angle = rand() % 121 + 30;

    // Generate a random speed between 3.0 and 7.0
    double ball_speed = 3.0 + (double)rand() / RAND_MAX * 4.0;

    // Calculate the x and y components of the ball's velocity vector
    double ball_velocity[2] = {
        ball_speed * cos(angle * PI / 180.0),
        ball_speed * sin(angle * PI / 180.0)

    };

    ball->position.x += ball_velocity[0];
    ball->position.y += ball_velocity[1];

}

#if 0 
void detect_ball_collision(Ball *ball)
{

}
#endif

int main(void)
{       

    // Initliaize Entities
    Paddle paddles[NUM_OF_PADDLES] =
    {   

        { {50,PADDLE_Y,PADDLE_WIDTH, PADDLE_HEIGHT},  PURPLE},
        { {WIDTH - 100,PADDLE_Y,PADDLE_WIDTH, PADDLE_HEIGHT},PURPLE}

    };

    Ball ball = {{WIDTH / 2, WIDTH / 2 - 100}, 10.0f, WHITE};

    InitWindow(WIDTH, HEIGHT, "@zaidev_ currently in development");
    SetTargetFPS(60);                  
    printf("Testing print\n");

    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        paddle_movement(&paddles[0]);
        ball_movement(&ball);
        detect_paddle_collision(&paddles[0]);

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawRectangle(paddles[0].rect.x, paddles[0].rect.y, paddles[0].rect.width, paddles[0].rect.height, RED);
        DrawRectangle(paddles[1].rect.x, paddles[1].rect.y, paddles[1].rect.width, paddles[1].rect.height, PURPLE);
        DrawCircle(ball.position.x, ball.position.y, ball.radius, ball.color);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}