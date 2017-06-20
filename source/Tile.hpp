#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>
#include "constants.hpp"
#include "SpriteLoader.hpp"

enum class TileType {
	Empty,
	Mine
};

class Tile {
public:
	Tile(TileType type, int x, int y);

	int x();
	int y();
	
	TileType type();
	void type(TileType type);
	
	bool revealed();
	void revealed(bool val);

	bool flagged();
	void toggle_flagged();
	
	int counter();
	void inc_counter();

	SDL_Rect rect();

	void draw(SDL_Surface *screen);
	void onTouch();

private:
	int m_x, m_y; // x and y coordinates in the grid, not on the screen
	TileType m_type;
	bool m_revealed;
	bool m_flagged;
	int m_counter; // Counts the number of mines around this tile

	SDL_Surface *m_surf_hidden, *m_surf_revealed;
	SDL_Rect m_rect;
};

#endif /* TILE_H */
