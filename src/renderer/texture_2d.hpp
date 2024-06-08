#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace Renderer{
    class Texture2D{
    public:

        struct Tile{
            glm::vec2 left_bottom_uv;
            glm::vec2 right_top_uv;

            Tile(const glm::vec2& _left_bottom_uv, const glm::vec2& _right_top_uv)
                : left_bottom_uv(_left_bottom_uv)
                , right_top_uv(_right_top_uv){}

            Tile()
                : left_bottom_uv(0.0f)
                , right_top_uv(1.0f){}
        };

        Texture2D(const GLuint width, GLuint height,
                  const unsigned char* data, const unsigned int channels = 4,
                  const GLenum filter = GL_LINEAR, const GLenum wrap_mode = GL_CLAMP_TO_EDGE);
        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&& texture_2d);
        Texture2D(Texture2D&& texture_2d);
        ~Texture2D();

        void add_tile(std::string name, const glm::vec2& left_bottom_uv, const glm::vec2& right_top_uv);
        const Tile& get_tile(const std::string& name) const;
        unsigned int width() const {return m_width;}
        unsigned int height() const {return m_height;}
        void bind() const;

    private:
        GLuint m_id;
        GLenum m_mode;
        unsigned int m_width;
        unsigned int m_height;
        std::map<std::string, Tile> m_tile;
    };
}