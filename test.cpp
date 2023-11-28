#include <GL/gl3w.h>
#include <cstddef>
#include <iostream>
#include <GLFW/glfw3.h>

// g++ -lgl3w -lglfw test.cpp

const char* PASS_THROUGH_VERTEX_SHADER = R"glsl(
#version 150 core

in vec3 position;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
}
)glsl";

const char* FRAGMENT_SHADER = R"glsl(
#version 150 core

out vec4 outColor;

void main() {
    outColor = vec4(1.0, 1.0, 0.0, 1.0);
}
)glsl";


void checkShaderStatus(GLuint shader) {

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::cout << buffer << std::endl;
    }

}


int main() {
    
    GLFWwindow* window;

    const GLfloat verticies[9] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window = glfwCreateWindow(
        500,
        500,
        "Test Window",
        NULL,
        NULL
    );

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (gl3wInit()) {
        std::cout << "GL3W Init error: " << gl3wInit() << std::endl;
        return -1;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint bufferHandle;
    glGenBuffers(1, &bufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
    glNamedBufferData(bufferHandle, sizeof(verticies), verticies, GL_STATIC_DRAW);    

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &PASS_THROUGH_VERTEX_SHADER, NULL);
    glCompileShader(vertexShader);
    checkShaderStatus(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER, NULL);
    glCompileShader(fragmentShader);
    checkShaderStatus(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    while (!glfwWindowShouldClose(window)) {

        glViewport(0, 0, 500, 500);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
