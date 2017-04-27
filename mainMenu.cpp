#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "constants.h"
#include "game.h"
#include <cstdio>
#include <cstdlib>

int main()
{
	//variables
	ALLEGRO_DISPLAY *display;
	ALLEGRO_DISPLAY_MODE displayMode;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *eq;
	ALLEGRO_EVENT ev;
	ALLEGRO_PATH *path;
	ALLEGRO_FONT *mainFont;
	bool exit;
	
	//initialization
	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(1000, 900);
	timer = al_create_timer(1.0 / FPS);
	eq = al_create_event_queue();
	path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_change_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
	mainFont = al_load_ttf_font("arial.ttf", 30, 0);
	al_get_display_mode(al_get_num_display_modes() - 1, &displayMode);
	
	al_clear_to_color(al_map_rgb(0, 255, 0));
	al_draw_text(mainFont, al_map_rgb(0, 0, 0), displayMode.width / 2, displayMode.height / 2,
				 ALLEGRO_ALIGN_CENTRE, "Press [SPACE] to continue");
	al_draw_text(mainFont, al_map_rgb(0, 0, 0), displayMode.width / 2, displayMode.height / 2 + 35,
				 ALLEGRO_ALIGN_CENTRE, "Press [ESC] to exit");
	al_flip_display();
	
	al_register_event_source(eq, al_get_keyboard_event_source());
	while(1)
	{
		while(al_is_event_queue_empty(eq));
		al_get_next_event(eq, &ev);
		if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{
			exit = true;
			break;
		}
		else if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
		{
			exit = false;
			break;
		}
	}
	al_register_event_source(eq, al_get_mouse_event_source());
	al_register_event_source(eq, al_get_timer_event_source(timer));
	
	//start the game
	if(!exit)
	{
		srand(826597 * (int)al_get_time());
		startGame(display, &displayMode, timer, eq, mainFont);
	}
	
	//exit
	al_destroy_event_queue(eq);
	al_destroy_timer(timer);
	al_destroy_path(path);
	al_destroy_display(display);
	return 0;
}