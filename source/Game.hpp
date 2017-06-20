#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <vector>
#include "constants.hpp"
#include "SpriteLoader.hpp"
#include "Tile.hpp"

using std::vector;

class Game {
public:
	Game();
	void run();

private:
	bool pointInRect(int x, int y, SDL_Rect rect);
	int getRandomInt(int a, int b);
	int conv_2d_to_1d(int x, int y, int width);

	void initGameGrid();
	void reset();
	void revealNeighbors(int x, int y);
	void revealAllMines();
	int countUnrevealedTiles();
	void flag(Tile &tile);
	void click(Tile &tile);

	// Game states
	void state_gameLoop();
	void state_gameOverLoop();
	
	SDL_Surface *m_screen;
	bool m_running;
	bool m_gameOver;
	bool m_won;
	bool m_flagMode;
	Uint32 m_bgColor;

	SDL_Event m_event;

	vector<Tile> m_gameGrid;
};

#endif /* GAME_H */
