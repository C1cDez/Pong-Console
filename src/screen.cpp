#include <iostream>
#include <cstring>
#include <windows.h>

#include "screen.h"

char buffer[WIDTH * HEIGHT + 1];

HANDLE console;
DWORD dwBytesWritten;

void init()
{
    console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    dwBytesWritten = 0;
}

void clear()
{
    memset(buffer, ' ', WIDTH * HEIGHT + 1);
    buffer[WIDTH * HEIGHT] = 0;
}

void print()
{
    WriteConsoleOutputCharacter(console, buffer, WIDTH * HEIGHT, {0, 0}, &dwBytesWritten);
}

void setpix(int x, int y, char c)
{
    if ((x > -1 && x < WIDTH) && (y > -1 && y < HEIGHT))
        buffer[y * WIDTH + x] = c;
}
