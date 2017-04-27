#ifndef _HEXGRID_H_
#define _HEXGRID_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "Hex.h"
#include "auxiliary.h"

#include <cstdio>

#define NUM_HEXES 2
enum DIRECTION {EAST, SOUTHEAST, SOUTHWEST, WEST, NORTHWEST, NORTHEAST, NUM_DIRECTIONS};

inline int getDistance(int r1, int q1, int r2, int q2)
{
	int x1 = q1 - (r1 - (r1 & 1))/2;
	int x2 = q2 - (r2 - (r2 & 1))/2;
	//z1 = r1; z2 = r2;
	return MAX3(ABS(x1 - x2), ABS(r1 - r2), ABS(x1 + r1 - x2 - r2));
}
struct HexGrid
{
	int *deltaR;
	int **deltaQ;
	Hex **grid;
	int lines, cols;
	int tileWidth, tileHeight;
	ALLEGRO_BITMAP **hexes;
	ALLEGRO_BITMAP *mouseMap;
	int population;
	HexGrid()
	{
		population = 0;
		grid = 0;
		hexes = 0;
		deltaR = new int [6];
		deltaQ = new int *[2];
		deltaQ[0] = new int [6];
		deltaQ[1] = new int [6];
		deltaR[0] = 0;
		deltaR[1] = 1;
		deltaR[2] = 1;
		deltaR[3] = 0;
		deltaR[4] = -1;
		deltaR[5] = -1;
		deltaQ[0][0] = 1;
		deltaQ[0][1] = 0;
		deltaQ[0][2] = -1;
		deltaQ[0][3] = -1;
		deltaQ[0][4] = -1;
		deltaQ[0][5] = 0;
		deltaQ[1][0] = 1;
		deltaQ[1][1] = 1;
		deltaQ[1][2] = 0;
		deltaQ[1][3] = -1;
		deltaQ[1][4] = 0;
		deltaQ[1][5] = 1;
	}
	~HexGrid()
	{
		int i;
		for(i = 0; i < lines; ++i)
		{
			delete[] grid[i];
		}
		delete[] grid;
		
		for(i = 0; i < NUM_HEXES; ++i)
		{
			al_destroy_bitmap(hexes[i]);
		}
		delete[] hexes;
		delete[] deltaR;
		delete[] deltaQ[0];
		delete[] deltaQ[1];
		delete[] deltaQ;
	}
	void makeGrid(int _lines, int _cols)
	{
		
		hexes = new ALLEGRO_BITMAP *[2];
		hexes[0] = al_load_bitmap("tiles/dead.png");
		hexes[1] = al_load_bitmap("tiles/alive.png");
		mouseMap = al_load_bitmap("tiles/mmap.png");
		tileWidth = al_get_bitmap_width(hexes[0]);
		tileHeight = al_get_bitmap_height(hexes[0]);
		int q;
		lines = _lines;
		cols = _cols;
		grid = new Hex *[lines];
		for(int r = 0; r < lines; ++r)
		{
			grid[r] = new Hex [cols];
			for(q = 0; q < cols; ++q)
			{
				grid[r][q].r = r;
				grid[r][q].q = q;
				grid[r][q].setState(false, hexes[false]);
				grid[r][q].x = q * tileWidth + (r & 1) * (tileWidth / 2);
				grid[r][q].y = r * (3 * tileHeight / 4);
			}
		}
	}
	inline bool isValid(int _r, int _q)
	{
		return _r > -1 && _r < lines && _q > -1 && _q < cols;
	}
	inline void moveInDir(int *_r, int *_q, int _dir, int _dist)
	{
		//mi-e lene sa schimb acum
		*_q += deltaQ[*_r & 1][_dir] * _dist;
		*_r += deltaR[_dir] * _dist;
	}
	void draw()
	{
		int i = 0, j;
		for(; i < lines; ++i)
		{
			for(j = 0; j < cols; ++j)
			{
				(grid[i] + j) -> draw();
			}
		}
	}
	void mouseToHex(int *_hy, int *_hx, ALLEGRO_MOUSE_STATE *_mState)
	{	
		*_hy = (int)(_mState -> y / (tileHeight * 0.75));
		*_hx = (_mState -> x - (*_hy & 1) * tileWidth / 2) / tileWidth;
		int x = (_mState -> x - (*_hy & 1) * (tileWidth / 2)) % tileWidth;
		int y = _mState -> y % (int)(tileHeight * 0.75);
		ALLEGRO_COLOR modifier = al_get_pixel(mouseMap, x, y);
		unsigned char r, g, b;
		al_unmap_rgb(modifier, &r, &g, &b);
		*_hx += (g - 1) + (*_hy & 1 & b);
		*_hy -= (r & 1);
	}
	void flip(int _r, int _q)
	{
		Hex *that = &grid[_r][_q];
		that -> setState(!that -> alive, hexes[!that -> alive]);
		if(that -> alive)
		{
			++population;
		}
		else
		{
			--population;
		}
	}
	void randomize(int (*rand)())
	{
		int r, q;
		int val;
		for(int r = 0; r < lines; ++r)
		{
			for(q = 0; q < cols; ++q)
			{
				val = rand();
				grid[r][q].setState(val & 1, hexes[val & 1]);
			}
		}
	}
	void update()
	{
		bool changeState[lines][cols];
		int r, q, dir;
		int destR, destQ;
		int numNeighbours;
		for(r = 0; r < lines; ++r)
		{
			for(q = 0; q < cols; ++q)
			{
				numNeighbours = 0;
				for(dir = 0; dir < NUM_DIRECTIONS; ++dir)
				{
					destR = r + deltaR[dir];
					destQ = q + deltaQ[r & 1][dir];
					if(isValid(destR, destQ))
					{
						if(grid[destR][destQ].alive)
						{
							numNeighbours++;
						}
					}
				}
				if(grid[r][q].alive)
				{
					switch(numNeighbours)
					{
					case(0):
					case(1):
						changeState[r][q] = true;
						break;
					case(2):
						changeState[r][q] = false;
						break;
					case(3):
					case(4):
					case(5):
					case(6):
						changeState[r][q] = true;
						break;
					}
				}
				else
				{
					switch(numNeighbours)
					{
					case(0):
					case(1):
						changeState[r][q] = false;
						break;
					case(2):
						changeState[r][q] = true;
						break;
					case(3):
					case(4):
					case(5):
					case(6):
						changeState[r][q] = false;
						break;
					}
				}
			}
		}
		for(r = 0; r < lines; ++r)
		{
			for(q = 0; q < cols; ++q)
			{
				if(changeState[r][q])
				{
					flip(r, q);
					changeState[r][q] = false;
				}
			}
		}
	}
};

#endif//_HEXGRID_H_