#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "renderer/shader.hpp"
#include "resources/resources_manager.hpp"

GLfloat vertices[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

int g_window_width = 1270;
int g_window_height = 720;

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char** argv){

    /* Initialize the library */
    if (!glfwInit()){
        std::cout << "GLFW initialization failed." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(g_window_width, g_window_height, "Dno Engine", nullptr, nullptr);
    if (!window){
        std::cout << "Window has been terminated." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
        g_window_width = width;
        g_window_height = height;
        glViewport(0, 0, g_window_width, g_window_height);
    });
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()){
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 0, 0, 1);
    {
        ResourcesManager res_manager(argv[0]);
        auto pMainShader = res_manager.load_shader("main_shader", "res/shaders/vertex", "res/shaders/fragment");
        if(!pMainShader){
            std::cerr << "Can't create shader program: " << "main_shader" << std::endl;
            return -1;
        }

        // VBO - Vertex Buffer Object
        GLuint vertices_vbo = 0;
        glGenBuffers(1, &vertices_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        // VAO - Vertex Array Object
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //Enable location in shaders
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            pMainShader->use();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}