#ifndef _GAME_H_
#define _GAME_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void startGame(ALLEGRO_DISPLAY *display, ALLEGRO_DISPLAY_MODE *displayMode,
				ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *eq, ALLEGRO_FONT *font);

#endif//_GAME_H_