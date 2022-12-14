#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "learn-pbrt/gl_error_checker.h"
#include "learn-pbrt/shader.h"
using namespace std;
// Vertex shader src
const char* vertex_shader_source =
    "#version 330 core\n"                    // version and profile
    "layout (location = 0) in vec3 aPos;\n"  // layout of input data
    "layout (location = 1) in vec3 aClr;\n"
    "out vec4 color;\n"
    "void main()\n"  // shader begin
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   color = vec4(aClr, 1.0);\n"
    "}\0";
const char* fragment_shader_source =
    "#version 330 core\n"
    "in vec4 color;"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    // "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "    FragColor = color;"
    "}\0";

void framebuffer_size_cb(GLFWwindow* window, int w, int h) {
  // glViewport(0, 0, w, h);
}
void process_input(GLFWwindow* window) {
  // Close window on Esc pressed
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();  // init GLFW
  // set the OpenGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // OpenGL 3.x
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // OpenGL x.3

  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE);  // use core profile
  // Get the glfw window
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {  // check
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  // use GLAD to manage OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  // check
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // position of bot-left, and size of viewport
  // It's independent from window size of GLFW
  glViewport(0, 0, 800, 600);
  // Register the rezise callback
  glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

  // Build shader program
  // --------------------
  // Creat a vertex shader and compile it
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  // Creat a fragment shader and compile it
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  // Link shaders to shader program
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  // Delete the shaders
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // vao and vbo
  // -----------
  // Element Buffer Object
  // clang-format off
  float unique_vert[] = {
    // position         // color
    -0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,
     0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,
     0.0f,  0.5f, 0.0f, 0.0, 0.0, 1.0,
  };
  GLuint indices[] = {0, 1, 2};
  // clang-format on
  GLuint EBO, VAO, VBO;
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // buffer VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(unique_vert), unique_vert,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // buffer EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    /**
     * Use color 0.3, 0.3, 0.3 to clear color buffer
     * This is to clear thing in last render loop
     * If called outside the render loop, screen will jitter
     */
    glClearColor(0.3, 0.3, 0.3, 1.0);  // rgba, the value used to clear
    glClear(GL_COLOR_BUFFER_BIT);      // clear the color buffer
    /****** Input ******/
    process_input(window);
    /****** Instruction ******/
    /****** Logic ******/
    glfwPollEvents();
    /****** Render ******/
    glUseProgram(shader_program);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    if (glCheckError() != GL_NO_ERROR) break;
    glfwSwapBuffers(window);  // We use double buffer
  }
  glfwTerminate();
  cout << "done\n";
  return 0;
}
