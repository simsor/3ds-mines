#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GAME_VERSION "0.1"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define N_TILES_X 12
#define N_TILES_Y 12
#define N_MINES 10

#define GRID_WIDTH N_TILES_X * TILE_WIDTH
#define GRID_HEIGHT N_TILES_Y * TILE_HEIGHT

#define GRID_MARGIN_LEFT (SCREEN_WIDTH - GRID_WIDTH) / 2
#define GRID_MARGIN_TOP (SCREEN_HEIGHT - GRID_HEIGHT) / 2

#endif /* CONSTANTS_H */