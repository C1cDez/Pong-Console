#include <cstring>
#include <string>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <windows.h>

#include "board.h"
#include "screen.h"

bool running;
int score;

int ballx, bally;
int vballx, vbally;

int const paddleSize = 5;
int playerPaddleY, robotPaddleY;
int vpaddle = 2;

std::thread keyboardInput;

void start()
{
    running = true;
    score = 0;
    ballx = WIDTH / 2;
    bally = playerPaddleY = robotPaddleY = HEIGHT / 2;
    std::srand(std::time(0));
    vballx = 1;
    vbally = (std::rand() % 2) * 2 - 1;
    keyboardInput = std::thread(listenKeys);
}
bool isRunning()
{
    return running;
}

void render()
{
    for (int i = 0; i < paddleSize; i++)
    {
        int playerY = playerPaddleY - paddleSize / 2 + i;
        int robotY = robotPaddleY - paddleSize / 2 + i;
        setpix(0, playerY, '|');
        setpix(WIDTH - 1, robotY, '|');
    }
    for (int x = 0; x < WIDTH; x++)
    {
        setpix(x, 0, '#');
        setpix(x, HEIGHT - 1, '#');
    }

    std::string scorestr = "Score: " + std::to_string(score);
    int scorestrlen = scorestr.length();
    for (int i = 0; i < scorestrlen; i++)
        setpix(WIDTH / 2 - scorestrlen / 2 + i, 1, scorestr[i]);
    
    setpix(ballx, bally, '@');
}

bool ballHitsPaddle()
{
    return (playerPaddleY - paddleSize / 2) < bally && bally < (playerPaddleY + paddleSize / 2 + 1);
}

void update()
{
    score++;
    robotPaddleY = bally;

    ballx += vballx;
    bally += vbally;

    if (bally <= 1 || bally >= HEIGHT - 2) vbally = -vbally;
    if (ballx >= WIDTH - 1) vballx = -vballx;

    if (ballx <= 1 && ballHitsPaddle()) vballx = -vballx;
    else if (ballx <= 0) running = false;

    if (playerPaddleY <= 1) playerPaddleY = 1;
    if (playerPaddleY >= HEIGHT - 1) playerPaddleY = HEIGHT - 1;
}

void displayLose()
{
    std::string text = "You Lost!";
    int textlen = text.length();

    for (int x = 0; x < WIDTH; x++) 
        setpix(x, HEIGHT / 2 - 1, '-');
    for (int i = 0; i < textlen; i++)
        setpix(WIDTH / 2 - textlen / 2 + i, HEIGHT / 2, text[i]);
    
    setpix(0, HEIGHT / 2, ' '); setpix(WIDTH - 1, HEIGHT / 2, ' ');
    setpix(0, HEIGHT / 2 + 1, ' '); setpix(WIDTH - 1, HEIGHT / 2 + 1, ' ');

    std::string scorestr = "Score: " + std::to_string(score);
    int scorestrlen = scorestr.length();
    for (int i = 0; i < scorestrlen; i++)
        setpix(WIDTH / 2 - scorestrlen / 2 + i, HEIGHT / 2 + 1, scorestr[i]);
    for (int x = 0; x < WIDTH; x++) 
        setpix(x, HEIGHT / 2 + 2, '-');
}

int getkey()
{
	INPUT_RECORD InputRecord;
	DWORD Writtten;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	
	while (true)
	{
	    ReadConsoleInputA(hStdIn, &InputRecord, 1, &Writtten);
	    if (InputRecord.EventType == KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown) break;
	}
	return InputRecord.Event.KeyEvent.wVirtualKeyCode;
}

int waitkey()
{
	int key = 0;
	while (true)
	{
		key = getkey();
		if (key) break;
	}
	return key;
}

void listenKeys()
{
    while (isRunning())
    {
        int key = waitkey();
        if (key == VK_UP || key == 0x57) playerPaddleY -= vpaddle;
        if (key == VK_DOWN || key == 0x53) playerPaddleY += vpaddle;
    }
}
