#include "../raylib/src/raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PLAY_SIZE 500
#define CELL_SIZE 20.0
#define MAX_BODY 625
#define FPS 60

#define PRE_GAME 0
#define GAME_RUNNING 1
#define GAME_OVER 2

typedef struct
{
    Vector2 coords[MAX_BODY];
    size_t count;
    size_t front;
    size_t rear;
} SnakeBody;

typedef struct
{
    Vector2 head_pos;
    Vector2 body_segment_size;
    int num_body_segments; // number of body segments that isn't the head;
    SnakeBody body;
} Snake;

typedef struct
{
    Vector2 pos;
    bool is_present;
} Apple;

typedef struct
{
    int frame_count;
    int cached_key;
    int direction;
    Snake snake;
    Apple apple;
    int game_status;
    int score;
} Game_Ctx;

void
gen_apple (Game_Ctx *game)
{
    while (!game->apple.is_present)
        {
            int new_x = rand () % PLAY_SIZE + 1;
            int new_y = rand () % PLAY_SIZE + 1;
            /* snap to the nearest coordinate, rounded down always */
            game->apple.pos.x = (int)(new_x / 20) * CELL_SIZE;
            game->apple.pos.y = (int)(new_y / 20) * CELL_SIZE;
            /* check if apple spawned on snake */
            int i = game->snake.body.front;
            bool set_apple = false;

            while (1)
                {
                    if (i == game->snake.body.rear)
                        {
                            set_apple = true;
                            break;
                        }
                    if (game->apple.pos.x == game->snake.body.coords[i].x
                        && game->apple.pos.y == game->snake.body.coords[i].y)
                        {
                            break;
                        }
                    i = (i + 1) % MAX_BODY;
                }
            if (game->apple.pos.x < 500 && game->apple.pos.y < 500
                && game->apple.pos.x >= 0 && game->apple.pos.y >= 0)
                game->apple.is_present = set_apple;
        }
}

bool
has_eaten (Game_Ctx *game)
{
    if (game->snake.head_pos.x == game->apple.pos.x
        && game->snake.head_pos.y == game->apple.pos.y)
        {
            game->score++;
            return true;
        }

    return false;
}

void
move_snake (Game_Ctx *game)
{
    if (game->cached_key == KEY_UP)
        game->snake.head_pos.y -= CELL_SIZE;
    if (game->cached_key == KEY_DOWN)
        game->snake.head_pos.y += CELL_SIZE;
    if (game->cached_key == KEY_LEFT)
        game->snake.head_pos.x -= CELL_SIZE;
    if (game->cached_key == KEY_RIGHT)
        game->snake.head_pos.x += CELL_SIZE;

    game->direction = game->cached_key;

    /* check for illegal coordinates and wrap accordingly */
    if (game->snake.head_pos.x >= PLAY_SIZE)
        game->snake.head_pos.x = 0;
    if (game->snake.head_pos.x < 0)
        game->snake.head_pos.x += 500;

    if (game->snake.head_pos.y >= PLAY_SIZE)
        game->snake.head_pos.y = 0;
    if (game->snake.head_pos.y < 0)
        game->snake.head_pos.y += PLAY_SIZE;
}

/* Functions for Body Updating */
bool
is_body_full (Game_Ctx *game)
{
    return (
        (game->snake.body.front == 0 && game->snake.body.rear == MAX_BODY - 1)
        || (game->snake.body.front == game->snake.body.rear + 1));
}

bool
is_body_empty (Game_Ctx *game)
{
    return (game->snake.body.front == -1);
}

/* Insert element to the front of the deque
 * Called when snake eats an apple
 * Inserts an element behind the head of the snake*/
void
insert_front (Game_Ctx *game, Vector2 key)
{
    if (is_body_full (game))
        {
            // stop game
            return;
        }

    if (game->snake.body.front == -1)
        {
            game->snake.body.front = 0;
            game->snake.body.rear = 0;
        }
    else if (game->snake.body.front == 0)
        {
            game->snake.body.front = MAX_BODY - 1;
        }
    else
        {
            game->snake.body.front = game->snake.body.front - 1;
        }

    game->snake.body.coords[game->snake.body.front] = key;
    game->snake.body.count++;
}

void
delete_rear (Game_Ctx *game)
{
    if (is_body_empty (game))
        return;

    if (game->snake.body.front == game->snake.body.rear)
        {
            game->snake.body.front = -1;
            game->snake.body.rear = -1;
        }
    else if (game->snake.body.rear == 0)
        {
            game->snake.body.rear = MAX_BODY - 1;
        }
    else
        {
            game->snake.body.rear = game->snake.body.rear - 1;
        }
    game->snake.body.count--;
}

void
update_body (Game_Ctx *game)
{
    insert_front (game, game->snake.head_pos);

    if (game->apple.is_present)
        {
            delete_rear (game);
        }
}

void
draw_snake (Game_Ctx *game)
{
    /* Draw Snake Head */
    DrawRectangleV (game->snake.head_pos, game->snake.body_segment_size,
                    GREEN);

    int i = game->snake.body.front;

    while (1)
        {
            if (is_body_empty (game))
                break;

            DrawRectangleV (game->snake.body.coords[i],
                            game->snake.body_segment_size, GREEN);

            if (i == game->snake.body.rear)
                break;

            i = (i + 1) % MAX_BODY;
        }
}

bool
has_collided (Game_Ctx *game)
{
    int i = game->snake.body.front;
    while (1)
        {
            if (i == game->snake.body.rear)
                break;

            if (game->snake.body.count < 5)
                return false;

            if (game->snake.head_pos.x == game->snake.body.coords[i].x
                && game->snake.head_pos.y == game->snake.body.coords[i].y)
                return true;

            i = (i + 1) % MAX_BODY;
        }
    return false;
}

void
update_keypress (Game_Ctx *game)
{
    int prev_input = game->cached_key;
    if (IsKeyPressed (KEY_UP) && game->direction != KEY_DOWN)
        game->cached_key = KEY_UP;
    if (IsKeyPressed (KEY_DOWN) && game->direction != KEY_UP)
        game->cached_key = KEY_DOWN;
    if (IsKeyPressed (KEY_LEFT) && game->direction != KEY_RIGHT)
        game->cached_key = KEY_LEFT;
    if (IsKeyPressed (KEY_RIGHT) && game->direction != KEY_LEFT)
        game->cached_key = KEY_RIGHT;
}

Game_Ctx
initialize_game ()
{
    Apple apple = { { -100, -100 }, false };
    SnakeBody snake_body = { 0 };
    snake_body.front = -1;
    snake_body.rear = -1;
    Snake snake = { { 200, 200 }, { CELL_SIZE, CELL_SIZE }, 1, snake_body };
    Game_Ctx game = { 0, 0, 0, snake, apple, PRE_GAME, 0 };
    insert_front (&game, (Vector2){ 200, 200 });
    return game;
}

void
pregame (Game_Ctx *game)
{
    BeginDrawing ();
    ClearBackground (BLACK);
    DrawText ("Press an arrow key to begin", 100, 180, 20, WHITE);
    EndDrawing ();

    if (game->cached_key != 0)
        game->game_status = GAME_RUNNING;
}

void
game_tick (Game_Ctx *game)
{
    if (!game->apple.is_present)
        gen_apple (game);

    game->frame_count = (game->frame_count + 1) % FPS;
    if (game->frame_count == 0 || game->frame_count == 30)
        {
            move_snake (game);

            if (has_collided (game))
                {
                    game->game_status = GAME_OVER;
                    return;
                }

            if (has_eaten (game))
                game->apple.is_present = false;

            update_body (game);
        }

    char buff[100];
    sprintf (buff, "Current Score: %d", game->score);

    BeginDrawing ();

    ClearBackground (BLACK);

    DrawText (buff, 10, 10, 10, WHITE);
    DrawRectangleV (game->apple.pos, game->snake.body_segment_size,
                    RED); // apple
    draw_snake (game);

    EndDrawing ();
}

void
post_game (Game_Ctx *game)
{
    BeginDrawing ();
    ClearBackground (BLACK);
    DrawText ("Game Over!", 190, 180, 20, WHITE);
    DrawText ("Press ENTER to retry", 187, 200, 10, WHITE);
    // draw score readout
    EndDrawing ();

    if (IsKeyPressed (KEY_ENTER))
        {
            *game = initialize_game ();
            game->game_status = GAME_RUNNING;
        }
}

int
main (void)
{
    srand (time (NULL));

    Game_Ctx game = initialize_game ();

    InitWindow (PLAY_SIZE, PLAY_SIZE, "Snake!");
    SetTargetFPS (FPS);

    while (!WindowShouldClose ())
        {
            /* Every frame, we check how many frames have passed
             * For every frame, we check what the most recently input key was
             * if, say, 30 frames have passed, we check the chached key input
             * we apply that to snake input, check of wrapping, and continue
             * from there
             * After we draw, we check if we are on top of an apple, and if
             * we are then we extend body on next move
             * Make sure we draw apple first, and then snake, so if snake moves
             * over apple that take priority*/

            update_keypress (&game);

            if (game.game_status == PRE_GAME)
                pregame (&game);
            else if (game.game_status == GAME_RUNNING)
                game_tick (&game);
            else if (game.game_status == GAME_OVER)
                post_game (&game);
        }

    CloseWindow ();

    return 0;
}
