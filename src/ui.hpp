#pragma once

#include "SDL3_ttf/SDL_ttf.h"

void showDeathScreen(TTF_Font *font);
void showQuestionScreen(TTF_Font *font);
void showWonScreen(TTF_Font *font, float timeTaken);
