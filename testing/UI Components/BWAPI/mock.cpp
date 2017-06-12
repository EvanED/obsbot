#include "BWAPI.h"
#include <cstdarg>
#include <cstdio>

namespace BWAPI {

void Game::drawTextScreen(int x, int y, const char * s, ...) const
{
	printf("drawTextScreen(%d, %d, \"%s\" => \"", x, y, s);
	va_list args;
	va_start(args, s);
	vprintf(s, args);
	va_end(args);
	puts("\")\n");
}

void Game::drawTextScreen(Position p, const char * s, ...) const
{
	printf("drawTextScreen(Position(%d, %d), \"%s\" => \"", p.x, p.y, s); 
	va_list args;
	va_start(args, s);
	vprintf(s, args);
	va_end(args);
	puts("\")\n");
}

void Game::drawBoxScreen(int x, int y, int x2, int y2, Color c, bool fill) const
{
	printf("drawBoxScreen(%d, %d, %d, %d, __, %s)\n", x, y, x2, y2, (fill ? "fill" : "empty"));
}

std::unique_ptr<Game> Broodwar(new Game());
}
