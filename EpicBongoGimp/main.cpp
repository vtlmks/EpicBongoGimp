#include "gl_core_3_3.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

#include <iostream>


/* -   -  - ---=--=-=-==-===-====<{[ TODO ]}>====-===-==-=-=--=--- -  -   -

Add the following classes
 - ShaderProgram					; should add vertex+fragment shader to a program and we should give it a name  (std::string name, std::string vertexPath, std::string fragmentPath) which
													; returns a shaderID, which we can discard if we want, as we can activate the shaderprogram with   shaderProgram.use(std::string name);
													; The name should really be a hash, so that it's fast.
 - Texture								; load texture, upload to a gl-texture, set a name, same as above  texture.use(std::string name, uint32_t textureChannel);
 - Mesh										; load and upload mesh...
 - 

 - several for UI + text

*/

/* -   -  - ---=--=-=-==-===-====<{[ NOTES ]}>====-===-==-=-=--=--- -  -   -

Change to fullscreen use SDL_SetWindowFullscreen(SDL_Window *win, uint32 flags);
flags: SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0(window)

*/

int main(int argc, char *argv[]) {
	std::cout << "Epic Bongo Gimp V0.0.0 - (c)2015" << std::endl;

	if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER)) {
		std::cout << "Boohoo motherfucker... Culd nut initializ0r sdl2 something something whatever..." << std::endl;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

//SDL_WINDOW_FULLSCREEN
//SDL_WINDOW_FULLSCREEN_DESKTOP
//SDL_WINDOW_BORDERLESS
	auto windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
	auto window = SDL_CreateWindow("Epic Bongo Gimp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
	auto context = SDL_GL_CreateContext(window);

	gl::sys::LoadFunctions();

	SDL_Event event;
	bool running{true};

	gl::ClearColor(.1f, .2f, .4f, 1.f);

	gl::Enable(gl::CULL_FACE);
	gl::CullFace(gl::BACK);

	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					running = false;
					break;
			}
		}
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);



		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}

