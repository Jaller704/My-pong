#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

bool init(char* title,int w_width, int w_height, int x_pos, int y_pos, int flags);

bool check_collision(int rect1_x, int rect1_y, int rect1_w, int rect1_h, int rect2_x, int rect2_y, int rect2_w, int rect2_h);
string winner = "0";
string checkWin();

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* player1_win = NULL;
SDL_Surface* player2_win = NULL;

const int w_width = 640;
const int w_height = 480;
const int FPS = 30;
SDL_Rect playerPos;
SDL_Rect player2Pos;
SDL_Rect ballRect;


int main(int argc, char* argv[])
{
//    SDL_Rect playerPos;
    playerPos.x = 300;
    playerPos.y = 465;

    playerPos.w = 149;
    playerPos.h = 15;

    /// Player 2
//    SDL_Rect player2Pos;
    player2Pos.x = 200;
    player2Pos.y = 0;

    player2Pos.w = 149;
    player2Pos.h = 15;

    bool b[4] = {0, 0, 0, 0};

    /// Ball
//    SDL_Rect ballRect;
    ballRect.x = playerPos.x;
    ballRect.y = playerPos.y - 30;

    int ball_vel_x = 3;
    int ball_vel_y = 3;

    ballRect.w = 10;
    ballRect.h = 10;

    player1_win = SDL_LoadBMP("Data/img/player1_won.bmp");
    player2_win = SDL_LoadBMP("Data/img/player2_won.bmp");

    if(init("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, SDL_WINDOW_SHOWN) == true)
    {
        bool appQuit = false;
        SDL_Event event;

        while(appQuit == false)
        {
            if(SDL_PollEvent(&event))
            {
                if(event.type == SDL_QUIT)
                {
                    appQuit = true;
                }

                if(event.type == SDL_KEYUP)
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        b[0] = 0;
                        break;

                    case SDLK_RIGHT:
                        b[1] = 0;
                        break;

                    case SDLK_a:
                        b[2] = 0;
                        break;

                    case SDLK_d:
                        b[3] = 0;
                        break;

                    }
                }

                if(event.type == SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        b[0] = 1;
                        break;

                    case SDLK_RIGHT:
                        b[1] = 1;
                        break;

                    case SDLK_a:
                        b[2] = 1;
                        break;

                    case SDLK_d:
                        b[3] = 1;
                        break;

                    case SDLK_ESCAPE:
                        appQuit = true;
                        break;
                    }
                }

            }
        /// Moving players if they had pressed a key.
        /// Player 1
        if(b[0] == true)
        {
            playerPos.x = playerPos.x - 3;
        }
        if(b[1] == true)
        {
            playerPos.x = playerPos.x + 3;
        }

        /// Player 2
        if(b[2] == true)
        {
            player2Pos.x = player2Pos.x - 3;
        }

        if(b[3] == true)
        {
            player2Pos.x = player2Pos.x + 3;
        }

        /// Collision with borders for player 1 (the one on the botttom)
        if(playerPos.x < 0)
        {
            playerPos.x = 0;
        }

        if(playerPos.x > 491)
        {
            playerPos.x = 491;
        }

        /// Collision Detection for player 2 (the one on the top)
        /// For borders
        if(player2Pos.x < 0)
        {
            player2Pos.x = 0;
        }
        if(player2Pos.x > 491)
        {
            player2Pos.x = 491;
        }

        /// Things for the ball
        //ballRect.x += ball_vel_x;
        //ballRect.y += ball_vel_y;

        if(ballRect.x < 0)
        {
            ball_vel_x = -ball_vel_x;
        }
        else if(ballRect.x > 630)
        {
            ball_vel_x = -ball_vel_x;
        }

        if(ballRect.y < 0)
        {
            ball_vel_y = -ball_vel_y;
        }
        else if(ballRect.y > 480)
        {
            ball_vel_y = -ball_vel_y;
        }

        /// If ball collide with the first player ( bottom )
        if(check_collision(playerPos.x, playerPos.y, playerPos.w, playerPos.h, ballRect.x, ballRect.y, ballRect.w, ballRect.h) == true)
        {
            ball_vel_y = -ball_vel_y;
        }

        /// If ball collide with the second player ( top )
        if(check_collision(player2Pos.x, player2Pos.y, player2Pos.w, player2Pos.h, ballRect.x, ballRect.y, ballRect.w, ballRect.h) == true)
        {
            ball_vel_y = -ball_vel_y;
        }

        /// Checking for win!
        /// If there is no collision ball with the top player and ball's position y is less than 0 that means
        /// it is under the player 1's paddle.
        ///Player 2
        if(checkWin() == "Player 1")
        {
            SDL_BlitSurface(player1_win, NULL, screen, NULL);
        }

        /// Player 1
        if(checkWin() == "Player 2")
        {
            SDL_BlitSurface(player2_win, NULL, screen, NULL);
        }

            /// While there is no winner ball_vel_x ++ and ball_vel_y++ and also blitting
            if(winner == "0")
            {
            ballRect.x += ball_vel_x;
            ballRect.y += ball_vel_y;

            SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00,0x00,0x00));

            SDL_FillRect(screen, &playerPos, SDL_MapRGB(screen->format, 0, 255, 0));

            SDL_FillRect(screen, &ballRect, SDL_MapRGB(screen->format, 123,0,0));

            SDL_FillRect(screen, &player2Pos, SDL_MapRGB(screen->format, 255, 90, 10));
            }

            /// Play again! :P
            if(checkWin() != "0" && b[4] == true)
            {
                winner = "0";
            }

            SDL_UpdateWindowSurface(window);

            /// Managing FPS
            if(1000/FPS > SDL_GetTicks()-SDL_GetTicks())
            {
                SDL_Delay(300/FPS - (SDL_GetTicks()-SDL_GetTicks()));
            }
        }
    }

    SDL_Quit();
    return 0;
}

bool init(char* title,int w_width, int w_height, int x_pos, int y_pos, int flags)
{
    bool success = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error while initializing SDL - " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        window = SDL_CreateWindow(title, w_width, w_height, x_pos, y_pos, flags);

        screen = SDL_GetWindowSurface(window);
        success = true;
    }

    return success;
}

bool check_collision(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
    bool collide = true;

    if(ay + ah < by)
    {
        collide = false;
    }
    else if(ay > by + bh)
    {
        collide = false;
    }
    else if(ax + aw < bx)
    {
        collide = false;
    }
    else if(ax > bx + bw)
    {
        collide = false;
    }


    return collide;
}

string checkWin()
{

        if(check_collision(player2Pos.x, player2Pos.y, player2Pos.w, player2Pos.h, ballRect.x, ballRect.y, ballRect.w, ballRect.h) == false && ballRect.x > 0 && ballRect.y < 0 )
        {
//            cout << "Player 1 WIN!";
            winner = "Player 1";
        }

        /// Player 1
        if(check_collision(playerPos.x, playerPos.y, playerPos.w, playerPos.h, ballRect.x, ballRect.y, ballRect.w, ballRect.h) == false && ballRect.x > 0 && ballRect.y > 480 )
        {
//            cout << "Player 2 WIN!";
            winner = "Player 2";
        }
    return winner;
}
