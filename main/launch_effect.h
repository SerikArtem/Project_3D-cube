#ifndef LAUNCH_H
#define LAUNCH_H

#include "cube.h"


// Oбщее количество эффектов.
// Используется в основном цикле switch в
// функции launch_effect (int effect) в файле launch_effect.c
// Установить константу на один больше, чем переключателей в основном цикле switch.
#define EFFECTS_TOTAL 11

void launch_effect (int effect);

#endif

