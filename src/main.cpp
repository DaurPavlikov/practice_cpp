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

//   x     y     z
const GLfloat vertices[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
};
//   u     v
const GLfloat uv[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
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
        auto main_shader_program = resources_manager.load_shader("main_shader", "res/shaders/texture.vert", "res/shaders/texture.frag");
        if(!main_shader_program){
            std::cerr << "Can't create shader program: " << "main_shader" << std::endl;
            return -1;
        }

        auto sprite_shader_program = resources_manager.load_shader("sprite_shader", "res/shaders/sprite.vert", "res/shaders/sprite.frag");
        if(!sprite_shader_program){
            std::cerr << "Can't create shader program: " << "sprite_shader" << std::endl;
            return -1;
        }

        auto tileset = resources_manager.load_texture("tileset", "res/textures/tileset.png");
        auto sprite_texture = resources_manager.load_texture("sara_sprite", "res/textures/SaraFullSheet.png");

        std::vector<std::string> tiles_names = {"gray_bricks", "roof", "roof2", "planks", "grass"};
        auto p_teleset = resources_manager.load_texture_atlas("default_tileset", "res/textures/tileset.png", std::move(tiles_names), 128, 128);

        auto tile = resources_manager.load_sprite("tileset", "default_tileset", "sprite_shader", 256, 256, "grass");

        auto sprite = resources_manager.load_sprite("sprite", "sara_sprite", "sprite_shader", 416 * 2, 672 * 2);

        sprite->set_position(glm::vec2(300, 200));
        tile->set_position(glm::vec2(300, 200));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window_size.x), 0.0f, static_cast<float>(window_size.y), -100.0f, 100.0f);
/*
        main_shader_program->use();
        main_shader_program->set_int("texture_0", 0);
        main_shader_program->set_matrix4("projection_matrix", projection_matrix);
        
        glm::mat4 model_matrix_0 = glm::mat4(1.0f);
        model_matrix_0 = glm::translate(model_matrix_0, glm::vec3(0.0f, 0.0f, 0.0f));
        model_matrix_0 = glm::scale(model_matrix_0, glm::vec3(1000.0f, 1000.0f, 1000.0f));

        //static_cast<float>()
        glm::mat4 projection_matrix = glm::ortho(0.0f, static_cast<float>(window_size.x), 0.0f, static_cast<float>(window_size.y), -100.0f, 100.0f);

        main_shader_program->set_matrix4("projection_matrix", projection_matrix);
        */
        sprite_shader_program->use();
        sprite_shader_program->set_int("texture_0", 0);
        sprite_shader_program->set_matrix4("projection_matrix", projection_matrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            tile->render();
            //sprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}