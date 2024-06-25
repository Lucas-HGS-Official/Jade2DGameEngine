#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Game();
	~Game();

	void Inicialize();
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

};

#endif
