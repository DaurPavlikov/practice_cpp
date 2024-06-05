#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/shader.hpp"
#include "renderer/texture_2d.hpp"
#include "renderer/sprite.hpp"
#include "resources/resources_manager.hpp"

GLfloat vertices[] = {
    0.0f, 70.0f, 0.0f,
    50.0f, -70.0f, 0.0f,
    -40.0f, -60.0f, 0.0f
};
/*
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
*/
GLfloat uv[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::vec2 window_size(1270, 720);

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
    GLFWwindow* window = glfwCreateWindow(window_size.x, window_size.y, "Dno Engine", nullptr, nullptr);
    if (!window){
        std::cout << "Window has been terminated." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
        window_size.x = width;
        window_size.y = height;
        glViewport(0, 0, width, height);
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
        ResourcesManager resources_manager(argv[0]);
        auto main_shader_program = resources_manager.load_shader("main_shader", "res/shaders/sprite.vert", "res/shaders/sprite.frag");
        if(!main_shader_program){
            std::cerr << "Can't create shader program: " << "main_shader" << std::endl;
            return -1;
        }

        auto sprite_shader_program = resources_manager.load_shader("sprite_shader", "res/shaders/sprite.vert", "res/shaders/sprite.frag");
        if(!sprite_shader_program){
            std::cerr << "Can't create shader program: " << "sprite_shader" << std::endl;
            return -1;
        }

        auto loaded_texture = resources_manager.load_texture("tileset", "res/textures/tileset.png");

        auto loaded_sprite = resources_manager.load_sprite("sprite", "tileset", "sprite_shader", 200, 200);
        loaded_sprite->set_position(glm::vec2(300, 200));

        // VBO - Vertex Buffer Object
        GLuint vertices_vbo = 0;
        glGenBuffers(1, &vertices_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        /*
        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        */
        GLuint uv_vbo = 0;
        glGenBuffers(1, &uv_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

        // VAO - Vertex Array Object
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //Enable location in shaders
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        //glEnableVertexAttribArray(1);
        //glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        main_shader_program->use();
        main_shader_program->set_int("texture_0", 0);

        glm::mat4 model_matrix_0 = glm::mat4(1.0f);
        model_matrix_0 = glm::translate(model_matrix_0, glm::vec3(100.0f, 50.0f, 0.0f));

        glm::mat4 model_matrix_1 = glm::mat4(1.0f);
        model_matrix_1 = glm::translate(model_matrix_1, glm::vec3(590.0f, 50.0f, 0.0f));

        //static_cast<float>()
        glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window_size.x), 0.0f, static_cast<float>(window_size.y), -100.0f, 100.0f);

        main_shader_program->set_matrix4("projection_matrix", projection_matrix);

        sprite_shader_program->use();
        sprite_shader_program->set_int("texture_0", 0);
        sprite_shader_program->set_matrix4("projection_matrix", projection_matrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            main_shader_program->use();
            glBindVertexArray(vao);
            loaded_texture->bind();

            main_shader_program->set_matrix4("model_matrix", model_matrix_0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            main_shader_program->set_matrix4("model_matrix", model_matrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            loaded_sprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}