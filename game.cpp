#include "game.h"
#include "HexGrid.h"

#define LINES 20
#define COLS 30
#define MAX_DELAY 30

#include <cstdlib>

void startGame(ALLEGRO_DISPLAY *display, ALLEGRO_DISPLAY_MODE *displayMode,
				ALLEGRO_TIMER *timer, ALLEGRO_EVENT_QUEUE *eq, ALLEGRO_FONT *font)
{
	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(al_map_rgb(0, 0, 200));
	al_draw_text(font, al_map_rgb(255, 255, 255), displayMode -> width / 2,
				displayMode -> height / 2, ALLEGRO_ALIGN_CENTRE, "Loading resources");
	al_flip_display();
	
	ALLEGRO_EVENT ev;
	ALLEGRO_MOUSE_STATE mState;
	bool playing = true, redraw = false, running;
	int r, q;
	int delay = 0, maxDelay = MAX_DELAY;
	HexGrid gameArea;
	gameArea.makeGrid(LINES, COLS);
	gameArea.draw();
	al_draw_text(font, al_map_rgb(255, 255, 255), 100, gameArea.tileHeight * LINES * 3 / 4 + 100,
					0, "Left click to change state, [SPACE] to begin");					
	al_flip_display();
	
	while(playing)
	{
		al_wait_for_event(eq, &ev);
		switch(ev.type)
		{
		case(ALLEGRO_EVENT_KEY_DOWN):
			switch(ev.keyboard.keycode)
			{
			case(ALLEGRO_KEY_SPACE):
				playing = false;
				break;
			case(ALLEGRO_KEY_A):
				gameArea.randomize(&rand);
				redraw = true;
				break;
			}
			break;
		case(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN):
			al_get_mouse_state(&mState);
			gameArea.mouseToHex(&r, &q, &mState);
			if(gameArea.isValid(r, q))
			{
				gameArea.flip(r, q);
			}
			redraw = true;
			break;
		}
		if(redraw)
		{
			redraw = false;
			gameArea.draw();
			al_flip_display();
		}
	}
	
	playing = true;
	running = true;
	al_start_timer(timer);
	while(playing)
	{
		al_wait_for_event(eq, &ev);
		switch(ev.type)
		{
		case(ALLEGRO_EVENT_TIMER):
			if(++delay == maxDelay)
			{
				delay = 0;
				redraw = true;
				gameArea.update();
			}
			break;
		case(ALLEGRO_EVENT_KEY_DOWN):
			switch(ev.keyboard.keycode)
			{
			case(ALLEGRO_KEY_SPACE):
				running = !running;
				if(running)
				{
					al_start_timer(timer);
				}
				else
				{
					al_stop_timer(timer);
				}
				break;
			case(ALLEGRO_KEY_ESCAPE):
				playing = false;
				break;
			}
		}		
		
		if(redraw)
		{
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 200));
			gameArea.draw();
			al_flip_display();
		}
	}
}