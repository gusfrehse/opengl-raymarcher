#include <iostream>
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include "shader.hpp"

void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Window * init(int width, int height, int flags)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
	logSDLError(std::cout, "SDL_Init");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
			SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *win = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED,
				       SDL_WINDOWPOS_CENTERED, width, height,
				       flags | SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
	logSDLError(std::cout, "SDL_Init");
    }

    SDL_GLContext context = SDL_GL_CreateContext(win);
    if (context = nullptr)
    {
	logSDLError(std::cout, "SDL_CreateContext");
    }
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
	std::cout << "gladLoadGLLloader error" << std::endl;
    }

    return win;
}

int main(int, char**){
    int windowFlags = SDL_WINDOW_OPENGL;
    const int winWidth = 800;
    const int winHeight = 450;
    SDL_Window *window = init(winWidth, winHeight, windowFlags);

    // gl calls
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, winWidth, winHeight);
    auto shadersSource = Shader::source2shader("test.glsl");
    Shader vertex(shadersSource.first, GL_VERTEX_SHADER);
    Shader fragment(shadersSource.second, GL_FRAGMENT_SHADER);

    ShaderProgram test(vertex, fragment);

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
	while (SDL_PollEvent(&e))
	{
	    if (e.type == SDL_KEYDOWN)
	    {
		switch (e.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		    quit = true;
		    break;
		case 'f':
		    windowFlags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
		    SDL_SetWindowFullscreen(window, windowFlags);
		    break;
		default:
		    break;
		}
	    }
   	    else if (e.type == SDL_QUIT)
	    {
		quit = true;
	    }
	    else if (e.type == SDL_MOUSEBUTTONDOWN)
	    {
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	    }
	}

	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(window);
    }


    SDL_Quit();
    return 0;
}
