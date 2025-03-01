#include <iostream>
#include <windows.h>

#include "screen.h"
#include "board.h"

#define TPS 50

int main()
{
    system("cls");
    init();
    start();
    
    while (isRunning())
    {
        update();
        clear();
        render();
        print();
        Sleep(1'000 / TPS);
    }
    
    displayLose();
    print();

    return 0;
}
