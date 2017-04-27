#ifndef _HEX_H_
#define _HEX_H_

/*
 *Hex tile for Conway's Game of Life
 */

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

struct Hex
{
	float x, y;
	int r, q;
	ALLEGRO_BITMAP *image;
	bool alive;
	Hex()
	{
		alive = true;
		image = 0;
	}
	void setState(bool _isAlive, ALLEGRO_BITMAP *_image)
	{
		alive = _isAlive;
		image = _image;
	}
	inline void draw()
	{
		al_draw_bitmap(image, x, y, 0);
	}
};

#endif//_HEX_H_