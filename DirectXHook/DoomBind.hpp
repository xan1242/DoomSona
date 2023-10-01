#include <iostream>

#ifndef DOOMBIND_MOD_H
#define DOOMBIND_MOD_H

uint32_t* (__cdecl* GetScreenBuffer)();
uint32_t(__cdecl* GetScreenX)();
uint32_t(__cdecl* GetScreenY)();

#endif