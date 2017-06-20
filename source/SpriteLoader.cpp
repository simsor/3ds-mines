#include "SpriteLoader.hpp"

SpriteLoader* SpriteLoader::m_instance = NULL;

void SpriteLoader::init(SDL_Surface *screen)
{
	SpriteLoader::m_instance = new SpriteLoader(screen);
}

SpriteLoader* SpriteLoader::instance()
{
	if (SpriteLoader::m_instance == NULL) {
		printf("SpriteLoader is not initialized\n");
		return NULL;
	}

	return SpriteLoader::m_instance;
}

SDL_Surface* SpriteLoader::loadOptimized(string filename)
{
	SDL_Surface *tmp = NULL, *img = NULL;

	tmp = IMG_Load(filename.c_str());
	if (tmp == NULL) {
		printf("[SpriteLoader] Could not load %s: %s\n", filename.c_str(), SDL_GetError());
		return NULL;
	}

	img = SDL_ConvertSurface(tmp, m_screen->format, 0);
	SDL_FreeSurface(tmp);
	if (img == NULL) {
		printf("[SpriteLoader] Could not optimize %s: %s\n", filename.c_str(), SDL_GetError());
		return NULL;
	}
	
	return img;
}

SpriteLoader::SpriteLoader(SDL_Surface *screen)
{
	m_screen = screen;

	vector<string> sprite_paths = {
		"/3ds/3ds-mines/tile_unknown.png", "/3ds/3ds-mines/tile_empty.png",
		"/3ds/3ds-mines/tile_flag.png", "/3ds/3ds-mines/tile_mine.png",
		"/3ds/3ds-mines/tile_mine.png",
		"/3ds/3ds-mines/tile_1.png", "/3ds/3ds-mines/tile_2.png",
		"/3ds/3ds-mines/tile_3.png", "/3ds/3ds-mines/tile_4.png",
		"/3ds/3ds-mines/won.png"
	};

	for (auto path : sprite_paths) {
		int idx = path.size() - 1;
		string name = "";
		while (path.at(idx) != '.') {
			idx--;
		}
		
		idx--;
		while (path.at(idx) != '/') {
			name.insert(0, string(1, path.at(idx)));
			idx--;
		}

		SDL_Surface* surf = loadOptimized(path);
		m_sprites.emplace(name, surf);
	}
}

SDL_Surface* SpriteLoader::get(string imageName)
{
	if (m_sprites.count(imageName) != 0) {
		return m_sprites.at(imageName);
	}

	printf("[SpriteLoader] Image not found: %s\n", imageName.c_str());
	return NULL;
}

void SpriteLoader::destroy()
{
    for (auto &pair : m_sprites) {
		SDL_FreeSurface(pair.second);
	}
}
