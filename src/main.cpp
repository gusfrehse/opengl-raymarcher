#include <iostream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "glad/glad.h"
#include "shader.hpp"
#include "camera.hpp"

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
    if (context == nullptr)
    {
	logSDLError(std::cout, "SDL_CreateContext");
    }
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
	std::cout << "gladLoadGLLloader error" << std::endl;
    }

    return win;
}

glm::vec2 mainQuad[] = {
    glm::vec2(-1.0f, -1.0f), // bottom left
    glm::vec2(-1.0f,  1.0f), // top left
    glm::vec2( 1.0f,  1.0f), // top right
    glm::vec2( 1.0f, -1.0f)  // bottom right
};

GLuint mainQuadIndices[] = {
    0, 1, 3,
    1, 2, 3
};

int main(int, char**){
    int windowFlags = SDL_WINDOW_OPENGL;
    const int winWidth = 800;
    const int winHeight = 800;
    SDL_Window *window = init(winWidth, winHeight, windowFlags);

    // GL calls
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glViewport(0, 0, winWidth, winHeight);
    auto shadersSource = Shader::source2shader("test.glsl");
    Shader vertex(shadersSource.first, GL_VERTEX_SHADER);
    Shader fragment(shadersSource.second, GL_FRAGMENT_SHADER);

    ShaderProgram test(vertex, fragment);

    // Setting vao, vbo, and ebo
    GLuint mainVAO, mainVBO, mainEBO;

    glGenVertexArrays(1, &mainVAO);
    glGenBuffers(1, &mainVBO);
    glGenBuffers(1, &mainEBO);

    glBindVertexArray(mainVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mainQuad), mainQuad, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mainQuadIndices),
		 mainQuadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
			  2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f),
		  glm::vec3(0.0f, 0.0f,-1.0f),
		  glm::vec3(0.0f, 1.0f, 0.0f));


    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
	while (SDL_PollEvent(&e))
	{
	    if (e.type == SDL_KEYDOWN)
	    {
		// TODO variable speed at camera class
		float speed = 0.1f;
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
		    camera.translate(camera.pos + speed * camera.front);
		    break;
		case SDLK_a:
		    camera.translate(camera.pos - speed * camera.right);
		    break;
		case SDLK_s:
		    camera.translate(camera.pos - speed * camera.front);
		    break;
		case SDLK_d:
		    camera.translate(camera.pos + speed * camera.right);
		    break;
		case SDLK_ESCAPE:
		    quit = true;
		    break;
		default:
		    break;
		}
	    }
   	    else if (e.type == SDL_QUIT)
	    {
		quit = true;
	    }
	    else if (e.type == SDL_MOUSEMOTION)
	    {
		// TODO: make variable sensitivity
		constexpr float sensitivity = 0.01;
		camera.rotate(e.motion.xrel * -sensitivity, camera.up);
		camera.rotate(e.motion.yrel * -sensitivity, camera.right);
	    }
	}

	glClear(GL_COLOR_BUFFER_BIT);
	test.use();
	test.set("cameraPosition", camera.pos);
	test.set("cameraFront", camera.front);
	test.set("cameraUp", camera.up);
	test.set("cameraRight", camera.right);
	test.set("cameraPosition", camera.pos);
	glBindVertexArray(mainVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(window);
    }


    SDL_Quit();
    return 0;
}
