#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <common/core/Engine.hpp>
#include <stdio.h>
#include <stdlib.h>

// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

// This is the callback we'll bse registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we
// press ESC
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    Engine::bgColor[0] = 0.5f;
    Engine::bgColor[1] = 0.3f;
    Engine::bgColor[2] =
        Engine::bgColor[2] > 1.0f ? 0.0f : Engine::bgColor[2] + 0.1;
  } else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
    Engine::bgColor[0] = 0.0f;
    Engine::bgColor[1] = 0.0f;
    Engine::bgColor[2] = 0.0f;
  }
}

GLfloat vertices[] = {0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.0f,
                      0.0f, 1.0f,  0.0f, 0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  1.0f};

int main() {
  Engine::init(640, 480, "OpenGL | Triangle Interpolation with Shader");

  auto engine = *Engine::instance();

  engine.setKeyCallback(key_callback);
  engine.setErrorCallback(error_callback);

  engine.loadShader("shader.vertex", "shader.fragment");

  engine.loadVerticeBuffer(vertices, [&]() -> void {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int colorLoc = glGetUniformLocation(engine.program, "customColor");
    unsigned int transformLoc =
        glGetUniformLocation(engine.program, "transform");
  });

  engine.loop([&]() -> void {
    glUseProgram(engine.program);
    glBindVertexArray(engine.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  });

  engine.destroy();

  return 0;
}