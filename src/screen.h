#pragma once

#ifndef SCREEN_H_
#define SCREEN_H_

#define WIDTH 120
#define HEIGHT 30

#define ANSI(code) ((char) 0x1b) << "[" << (code)

void init();
void clear();
void print();

void setpix(int x, int y, char c);

#endif
