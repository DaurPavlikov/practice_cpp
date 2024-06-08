#pragma once

#include <string>
#include <memory>

#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace Renderer{
    class Texture2D;
    class ShaderProgram;
    class Sprite{
    public:
        Sprite(const std::shared_ptr<Texture2D> texture,
               const std::string initial_tile,
               const std::shared_ptr<ShaderProgram> shader_program,
               const glm::vec2& position = glm::vec2(0.0f),
               const glm::vec2& size = glm::vec2(1.0f),
               const float rotation = 0.0f);
        ~Sprite();
        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;

        void render() const;
        void set_position(const glm::vec2& position);
        void set_rotation(const float rotation);
        void set_size(const glm::vec2& size);

    private:
        std::shared_ptr<Texture2D> m_texture;
        std::shared_ptr<ShaderProgram> m_shader_program;
        glm::vec2 m_position;
        glm::vec2 m_size;
        float m_rotation;
        GLuint m_vao;
        GLuint m_vertices_vbo;
        GLuint m_uv_vbo;
    };
}
