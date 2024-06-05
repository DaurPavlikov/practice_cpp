#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "sprite.hpp"
#include "texture_2d.hpp"

namespace Renderer{
    Sprite::Sprite(const std::shared_ptr<Texture2D> texture,
           const std::shared_ptr<ShaderProgram> shader_program,
           const glm::vec2& position,
           const glm::vec2& size,
           const float rotation)
           : m_texture(std::move(texture))
           , m_shader_program(std::move(shader_program))
           , m_position(position)
           , m_rotation(rotation)
           , m_size(size){
        // 2--3   1
        // | /  / |
        // 1   3--2
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

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vertices_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


        glGenBuffers(1, &m_uv_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_uv_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv), &uv, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Sprite::~Sprite(){
        glDeleteBuffers(1, &m_vertices_vbo);
        glDeleteBuffers(1, &m_uv_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void Sprite::render() const{
        m_shader_program->use();
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(m_position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
        model = glm::scale(model, glm::vec3(m_size, 1.0f));

        glBindVertexArray(m_vao);
        m_shader_program->set_matrix4("model_matrix", model);
        glActiveTexture(GL_TEXTURE0);
        m_texture->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void Sprite::set_position(const glm::vec2& position){
        m_position = position;
    }

    void Sprite::set_rotation(const float rotation){
        m_rotation = rotation;
    }

    void Sprite::set_size(const glm::vec2& size){
        m_size = size;
    }
}