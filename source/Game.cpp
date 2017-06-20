#include <cstdlib>
#include <ctime>
#include "Game.hpp"

Game::Game()
{
	m_screen = NULL;
	m_flagMode = false;
	m_gameOver = false;
	m_won = false;
	SDL_Init(SDL_INIT_VIDEO);
	std::srand(std::time(0));

	m_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_BOTTOMSCR | SDL_CONSOLETOP);
	if (m_screen == NULL) {
		exit(1);
	}

	SDL_ShowCursor(0);

	SpriteLoader::init(m_screen);

	m_running = true;
	m_bgColor = SDL_MapRGB(m_screen->format, 255, 255, 255);

	initGameGrid();

	printf("3DS-MINES v%s\n\n", GAME_VERSION);
	printf("Press START to quit\n");
	printf("      SELECT to restart\n\n\n");
	printf("Hold L or R to flag a tile\n");
}

void Game::initGameGrid()
{
	// Create game grid
	for (int y=0; y < N_TILES_Y; y++) {
		for (int x=0; x < N_TILES_X; x++) {
			m_gameGrid.push_back(Tile(TileType::Empty, x, y));
		}
	}

	// Place mines
	int placed_mines = 0;
	while (placed_mines < N_MINES) {
		int idx = getRandomInt(0, m_gameGrid.size());
		Tile &t = m_gameGrid.at(idx);
		if (t.type() == TileType::Empty) {
			t.type(TileType::Mine);
			placed_mines++;

			// We increment the counter for each neighboring tile
			int mine_x = t.x();
			int mine_y = t.y();

			for (int x=mine_x-1; x <= mine_x+1; x++) {
				for (int y=mine_y-1; y <= mine_y+1; y++) {
					if (x == mine_x && y == mine_y) {
						continue;
					}

					if (x < 0 || y < 0 || x >= N_TILES_X || y >= N_TILES_Y) {
						continue;
					}

					Tile &tile_to_inc = m_gameGrid.at(conv_2d_to_1d(x, y, N_TILES_X));
					tile_to_inc.inc_counter();
				}
			}
		}
	}
}

void Game::reset()
{
	m_gameGrid.clear();
	m_gameOver = false;
	m_won = false;

	initGameGrid();
}

void Game::run()
{
	while (m_running) {
		if (m_gameOver) {
			state_gameOverLoop();
		} else {
			state_gameLoop();
		}
	}

	SpriteLoader::instance()->destroy();
	SDL_FreeSurface(m_screen);
	SDL_Quit();
	return;
}

bool Game::pointInRect(int x, int y, SDL_Rect rect)
{
	if (x > rect.x && x < (rect.x + rect.w) &&
		y > rect.y && y < (rect.y + rect.h)) {
		return true;
	} else {
		return false;
	}
}

int Game::getRandomInt(int a, int b)
{
	float tmp = (float) std::rand() / RAND_MAX;
	return (int) ((tmp * (b+a)) - a);
}

int Game::conv_2d_to_1d(int x, int y, int width)
{
	return (y * width) + x;
}

void Game::revealNeighbors(int x, int y)
{
	for (int cx=x-1; cx <= x+1; cx++) {
		for (int cy=y-1; cy <= y+1; cy++) {
			if (cx == x && cy == y) {
				continue;
			}

			if (cx < 0 || cy < 0 || cx >= N_TILES_X || cy >= N_TILES_Y) {
				continue;
			} 
			
			Tile &t = m_gameGrid.at(conv_2d_to_1d(cx, cy, N_TILES_X));
			if (t.revealed()) {
				continue;
			}
			
			if (t.type() == TileType::Empty) {
				t.revealed(true);

				if (t.counter() == 0) {
					revealNeighbors(cx, cy);
				}
			}
		}
	}
}

void Game::revealAllMines()
{
	for (auto &t : m_gameGrid) {
		if (t.type() == TileType::Mine) {
			t.revealed(true);
		}
	}
}

int Game::countUnrevealedTiles()
{
	int c = 0;
	for (auto &t : m_gameGrid) {
		if (!t.revealed()) {
			c++;
		}
	}

	return c;
}

void Game::flag(Tile& tile)
{
	tile.toggle_flagged();
}

void Game::click(Tile& tile)
{
	if (tile.type() == TileType::Mine) {
		// Game over
		tile.revealed(true);
		revealAllMines();
		m_gameOver = true;
		m_won = false;
	} else {
		if (tile.counter() == 0 && !tile.revealed()) {
			// An empty tile, we reveal all neighboring empty tiles
			// recursively
			revealNeighbors(tile.x(), tile.y());
		}
		tile.revealed(true);

		if (countUnrevealedTiles() == N_MINES) {
			// Then the player won the game
			m_gameOver = true;
			m_won = true;
		}
	} 
}

void Game::state_gameLoop()
{
	// Event management
	while (SDL_PollEvent(&m_event)) {
		if (m_event.type == SDL_MOUSEBUTTONDOWN) {
			int mouse_x = m_event.button.x;
			int mouse_y = m_event.button.y;
			
			for (auto &tile : m_gameGrid) {
				if (pointInRect(mouse_x, mouse_y, tile.rect())) {
					if (m_flagMode) {
						flag(tile);
					} else {
						click(tile);
					}
					break;
				}
			}
		} else if (m_event.type == SDL_KEYDOWN) {
			if (m_event.key.keysym.sym == SDLK_l ||
				m_event.key.keysym.sym == SDLK_r) {
				m_flagMode = true;
			} else if (m_event.key.keysym.sym == SDLK_RETURN) {
				m_running = false;
			} else if (m_event.key.keysym.sym == SDLK_ESCAPE) {
				reset();
			}
		} else if (m_event.type == SDL_KEYUP) {
			if (m_event.key.keysym.sym == SDLK_l ||
				m_event.key.keysym.sym == SDLK_r) {
				m_flagMode = false;
			}
		} else if (m_event.type == SDL_QUIT) {
			m_running = false;
		}
	}
	
	// Drawing
	SDL_FillRect(m_screen, NULL, m_bgColor);
	
	for (auto &t : m_gameGrid) {
		t.draw(m_screen);
	}
	
	SDL_Flip(m_screen);
}

void Game::state_gameOverLoop()
{
	SDL_Surface *msg = NULL;

	while (SDL_PollEvent(&m_event)) {
		if (m_event.type == SDL_QUIT) {
			m_running = false;
		} else if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_RETURN) {
			m_running = false;
		} else if (m_event.key.keysym.sym == SDLK_ESCAPE) {
			reset();
		}
	}

	if (m_won) {
		msg = SpriteLoader::instance()->get("won");
		SDL_FillRect(m_screen, NULL, m_bgColor);

		SDL_BlitSurface(msg, NULL, m_screen, NULL);
	
		SDL_Flip(m_screen);
	} else {
		//msg = SpriteLoader::instance()->get("lost");
	}
}
