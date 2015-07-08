#include "gl_core_3_3.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>


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
GLuint LoadShaders(const char *vertex_file_path,const char *fragment_file_path);


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

	GLuint VertexArrayID;
	gl::GenVertexArrays(1, &VertexArrayID);
	gl::BindVertexArray(VertexArrayID);

	gl::ClearColor(.1f, .2f, .4f, 1.f);

	gl::Enable(gl::CULL_FACE);
	gl::CullFace(gl::BACK);

	gl::Enable(gl::DEPTH_TEST);
	gl::DepthFunc(gl::LESS);

	GLuint shaderProgram = LoadShaders("default_vert.glsl", "default_frag.glsl");

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					running = false;
					break;
			}
		}
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
		gl::UseProgram(shaderProgram);


		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}

GLuint LoadShaders(const char *vertex_file_path,const char *fragment_file_path) {
	GLuint VertexShaderID		= gl::CreateShader(gl::VERTEX_SHADER);
	GLuint FragmentShaderID	= gl::CreateShader(gl::FRAGMENT_SHADER);

	std::string VertexShaderCode{};
	std::string FragmentShaderCode{};

	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()) {
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
		VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()) {
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
		FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = gl::FALSE_;
	int InfoLogLength;

	char const *VertexSourcePointer = VertexShaderCode.c_str();
	gl::ShaderSource(VertexShaderID, 1, &VertexSourcePointer , 0);
	gl::CompileShader(VertexShaderID);

	gl::GetShaderiv(VertexShaderID, gl::COMPILE_STATUS, &Result);
	gl::GetShaderiv(VertexShaderID, gl::INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	gl::GetShaderInfoLog(VertexShaderID, InfoLogLength, 0, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	gl::ShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , 0);
	gl::CompileShader(FragmentShaderID);

	gl::GetShaderiv(FragmentShaderID, gl::COMPILE_STATUS, &Result);
	gl::GetShaderiv(FragmentShaderID, gl::INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	gl::GetShaderInfoLog(FragmentShaderID, InfoLogLength, 0, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	GLuint ProgramID = gl::CreateProgram();
	gl::AttachShader(ProgramID, VertexShaderID);
	gl::AttachShader(ProgramID, FragmentShaderID);
	gl::LinkProgram(ProgramID);

	gl::GetProgramiv(ProgramID, gl::LINK_STATUS, &Result);
	gl::GetProgramiv(ProgramID, gl::INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
	gl::GetProgramInfoLog(ProgramID, InfoLogLength, 0, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	gl::DeleteShader(VertexShaderID);
	gl::DeleteShader(FragmentShaderID);
 
	return ProgramID;
}
