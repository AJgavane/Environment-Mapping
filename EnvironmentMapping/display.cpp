#include "display.h"
#include <GL\glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
    // std::cout << "Constructor" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING); // SDL is for window

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // creates 2^8 shades of Red
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); // 4*8 = 32 from above
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Allocate space for 2 frames.

    m_window = SDL_CreateWindow(title.c_str(), 500, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window); // glCOntext is for specific window.
    
    GLenum status = glewInit();                   // starting opengl 

    if (status != GLEW_OK) {
        std::cerr << "GLEW failed to initialize " << std::endl;
    }
    m_isClosed = false;
}

Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT   );
}

bool Display::isClosed() {
    return m_isClosed;
}

void Display::Terminate() {
    m_isClosed = true;
}

void Display::Update()
{
    SDL_GL_SwapWindow(m_window);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            m_isClosed = true;
        }
    }
}