#include "Tile.hpp"

Tile::Tile(TileType type, int x, int y)
{
	this->type(type);
	m_revealed = false;

	m_x = x;
	m_y = y;

    m_rect.x = m_x * TILE_WIDTH + GRID_MARGIN_LEFT;
	m_rect.y = m_y * TILE_HEIGHT + GRID_MARGIN_TOP;
	m_rect.w = TILE_WIDTH;
	m_rect.h = TILE_HEIGHT;

	m_counter = 0;

	m_surf_hidden = SpriteLoader::instance()->get("tile_unknown");
}

int Tile::x()
{
	return m_x;
}

int Tile::y()
{
	return m_y;
}

TileType Tile::type()
{
	return m_type;
}

void Tile::type(TileType type)
{
	m_type = type;

	if (type == TileType::Mine) {
		m_surf_revealed = SpriteLoader::instance()->get("tile_mine");
	} else {
		m_surf_revealed = SpriteLoader::instance()->get("tile_empty");
	}
}

bool Tile::revealed()
{
	return m_revealed;
}

void Tile::revealed(bool val)
{
	m_revealed = val;
}

bool Tile::flagged()
{
	return m_flagged;
}

void Tile::toggle_flagged()
{
	m_flagged = !m_flagged;

	if (flagged()) {
		m_surf_hidden = SpriteLoader::instance()->get("tile_flag");
	} else {
		m_surf_hidden = SpriteLoader::instance()->get("tile_unknown");
	}
}

int Tile::counter()
{
	return m_counter;
}

void Tile::inc_counter()
{
	m_counter++;

	if (m_type == TileType::Empty) {
		switch (m_counter) {
		case 1:
			m_surf_revealed = SpriteLoader::instance()->get("tile_1");
			break;
		case 2:
			m_surf_revealed = SpriteLoader::instance()->get("tile_2");
			break;
		case 3:
			m_surf_revealed = SpriteLoader::instance()->get("tile_3");
			break;
		case 4:
			m_surf_revealed = SpriteLoader::instance()->get("tile_4");
			break;
		}
	}
}

SDL_Rect Tile::rect() {
	return m_rect;
}

void Tile::draw(SDL_Surface *screen)
{
	SDL_Surface *surf = NULL;
	if (revealed()) {
		surf = m_surf_revealed;
	} else {
		surf = m_surf_hidden;
	}

	SDL_BlitSurface(surf, NULL, screen, &m_rect);
}

void Tile::onTouch()
{
	revealed(true);
}
