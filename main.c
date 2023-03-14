#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "raylib.h"
#include "zaitypes.h"


#define WIDTH                        800
#define HEIGHT                      600
#define NUM_OF_PADDLES      2
#define PADDLE_WIDTH          10
#define PADDLE_HEIGHT        100
#define PADDLE_Y                  (HEIGHT / 2 - 50)
#define FINAL_SCORE 5

typedef enum {
    PADDLE_LEFT,
    PADDLE_RIGHT
} PaddleType;

typedef enum {
    PLAYER1_WON,
    PLAYER2_WON,
    PLAYING
} GameState;

typedef struct  {
    Rectangle rect;
    PaddleType paddle_type;
    Color color;
} Paddle;

typedef struct  {
    Vector2 position;
    f32 speed;
    f32 radius;
    f32 direction_x;
    f32 direction_y;
    Color color;
} Ball;


typedef struct {
    
    i32 player_1_score;
    i32 player_2_score;
    bool player_1_scored;
    bool player_2_scored;
    GameState gamestate;
} ScoreBoard;

Color test_color = RED;
const f32 VELOCITY = 3.5f;
i32 player_1_score = 0;
i32 player_2_score = 0;

bool collided(Ball *ball, Paddle *paddle) {
    return CheckCollisionCircleRec(ball->position, ball->radius, paddle->rect);
}

// got this formula from google lmao (fly_direction)
void ball_collision_direction(Ball *ball, Paddle *paddle, bool positive) {
    f32 fly_direction = ((ball->position.y - paddle->rect.y) / paddle->rect.height) - 0.5f;

    // TODO: Clean this code
    if(positive)
    {
         ball->direction_x = fabsf(ball->direction_x);
    } else {
         ball->direction_x = -fabsf(ball->direction_x);
    }

    ball->direction_y = fly_direction;
}

void paddle_movement(Paddle *player_1, Paddle *player_2) {   


    if(IsKeyUp(KEY_W))     player_1->rect.y += VELOCITY;
    if(IsKeyUp(KEY_S))     player_1->rect.y -= VELOCITY;

    if(IsKeyUp(KEY_UP))     player_2->rect.y += VELOCITY;
    if(IsKeyUp(KEY_DOWN)) player_2->rect.y -= VELOCITY;

}

void detect_paddle_collision(Paddle *paddle) {
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

f32 random_angle() {
    return (f32) rand() / RAND_MAX * 2 * PI;
}


Vector2 random_direction() {
    f32 angle = random_angle();
    Vector2 direction = {cos(angle), sin(angle)};
    return direction;
}

void reset_ball(Ball *ball)
{
    // reset ball position
    ball->position.x = WIDTH / 2;
    ball->position.y = WIDTH / 2 - 100;

    // randomize an angle when ball resets
    Vector2 angle = random_direction(); 
    ball->direction_x = angle.x;
    ball->direction_y = angle.y;

}

void update_ball(Ball *ball, Paddle paddles[NUM_OF_PADDLES], ScoreBoard *score) 
{


    ball->position.x += ball->speed * ball->direction_x;
    ball->position.y += ball->speed * ball->direction_y; 

    // player 1 scored
    if(ball->position.x > WIDTH)
    {
        reset_ball(ball);
        score->player_1_scored = true;
    }


    // player 2 score
    if(ball->position.x < 0)
    {
        reset_ball(ball);
        score->player_2_scored = true;
    }

    if(ball->position.y  - ball->radius < 0 ) {
        ball->direction_y *= -1.0f;
    }
    // collide with top or bottom wall ?
    if(ball->position.y   + ball->radius > HEIGHT) {
        ball->direction_y *= -1.0f;
    }

    for(u8 i = 0; i < NUM_OF_PADDLES; i++) {
        if(collided(ball, &paddles[i]))
        {    
            paddles[i].color = GREEN;
            switch(paddles[i].paddle_type)
            {
                case PADDLE_LEFT:
                     ball_collision_direction(ball, &paddles[i], true);
                    break;
                case PADDLE_RIGHT:
                     ball_collision_direction(ball, &paddles[i], false);
                    break;
            }

         }
    }   

}

int main(void) {       
    char ui_buffer[7];
    // Initliaize Entities
    Paddle paddles[NUM_OF_PADDLES] =
    {   

        { {50,PADDLE_Y,PADDLE_WIDTH, PADDLE_HEIGHT}, PADDLE_LEFT, PURPLE},
        { {WIDTH - 100,PADDLE_Y,PADDLE_WIDTH, PADDLE_HEIGHT}, PADDLE_RIGHT, PURPLE}

    };

    ScoreBoard score_board = {0,0,false,false, PLAYING};

    // Randomize x and y ball direction
    #if 1
    Vector2 initial_ball_angle = random_direction();
    Ball ball = {{WIDTH / 2, WIDTH / 2 - 100}, 5.0f,  10.0f, initial_ball_angle.x, initial_ball_angle.y, WHITE};
    #endif

    #if 0
    Ball ball = {{WIDTH / 2, WIDTH / 2 - 100}, 5.0f,  10.0f, 1.0f, 0, WHITE};
    #endif

    InitWindow(WIDTH, HEIGHT, "@zaidev_ currently in development");
    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        snprintf(ui_buffer, sizeof(ui_buffer), "%d|%d", player_1_score, player_2_score);
        #if 1
        if(IsKeyPressed(KEY_R))
        {
            ball.position.x = WIDTH / 2;
            ball.position.y = WIDTH / 2 - 100;
            Vector2 rand_direction = random_direction();
            ball.direction_x = rand_direction.x;
            ball.direction_y = rand_direction.y;
        }
        #endif

        paddle_movement(&paddles[0], &paddles[1]);

        for(u8 i = 0; i < NUM_OF_PADDLES; i++) detect_paddle_collision(&paddles[i]);
        update_ball(&ball, paddles, &score_board);
        
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        if(score_board.player_1_scored)
        {
            score_board.player_1_score++;
            score_board.player_1_scored = false;
        }

        if(score_board.player_2_scored)
        {
            score_board.player_2_score++;
            score_board.player_2_scored = false;
        }

         
        snprintf(ui_buffer, sizeof(ui_buffer), "%d|%d", score_board.player_1_score, score_board.player_2_score);
        DrawText(ui_buffer,  WIDTH /2 - 50,0, 50, WHITE);

        if(score_board.player_1_score == FINAL_SCORE) score_board.gamestate = PLAYER1_WON; 
        if(score_board.player_2_score == FINAL_SCORE) score_board.gamestate = PLAYER2_WON; 

        if(score_board.gamestate == PLAYING) {
            DrawRectangle(paddles[0].rect.x, paddles[0].rect.y, paddles[0].rect.width, paddles[0].rect.height, paddles[0].color);
            DrawRectangle(paddles[1].rect.x, paddles[1].rect.y, paddles[1].rect.width, paddles[1].rect.height, paddles[1].color);
            DrawCircle(ball.position.x, ball.position.y, ball.radius, ball.color);
        } else if(score_board.gamestate == PLAYER1_WON) {
            DrawText("PLAYER 1 WON", WIDTH /2 - 200, WIDTH / 2, 50, WHITE);
        } else if(score_board.gamestate == PLAYER2_WON) {
            DrawText("PLAYER 2 WON", WIDTH /2 -200, WIDTH / 2, 50, WHITE);
        }

        EndDrawing();

    }

    CloseWindow();        // Close window and OpenGL context


    return 0;


}


