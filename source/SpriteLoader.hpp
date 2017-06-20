#ifndef SPRITELOADER_H
#define SPRITELOADER_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::map;
using std::vector;

class SpriteLoader
{
public:
	static void init(SDL_Surface *screen);
	static SpriteLoader* instance();

	SDL_Surface* get(string imageName);
	void destroy();
	
private:
	SpriteLoader(SDL_Surface *screen);
	SDL_Surface* loadOptimized(string filename);

	static SpriteLoader *m_instance;

	map<string, SDL_Surface*> m_sprites;

	SDL_Surface *m_screen;
};

#endif /* SPRITELOADER_H */
