#include "texture_2d.hpp"

namespace Renderer{
    Texture2D::Texture2D(const GLuint width, GLuint height,
                         const unsigned char* data,
                         const unsigned int channels,
                         const GLenum filter,
                         const GLenum wrap_mode)
                         : m_width(width), m_height(height){
        switch (channels){
            case 4:
                m_mode = GL_RGBA;
                break;
            case 3:
                m_mode = GL_RGB;
                break;
            default:
                m_mode = GL_RGBA;
                break;
        }
        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
        //second - mipmap
        glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture2D& Texture2D::operator=(Texture2D&& texture_2d){
        glDeleteTextures(1, &m_id);
        m_id = texture_2d.m_id;
        texture_2d.m_id = 0;
        m_mode = texture_2d.m_mode;
        m_width = texture_2d.m_width;
        m_height = texture_2d.m_height;
        return *this;
    }

    Texture2D::Texture2D(Texture2D&& texture_2d){
        m_id = texture_2d.m_id;
        texture_2d.m_id = 0;
        m_mode = texture_2d.m_mode;
        m_width = texture_2d.m_width;
        m_height = texture_2d.m_height;
    }

    Texture2D::~Texture2D(){
        glDeleteTextures(1, &m_id);
    }

    void Texture2D::bind() const{
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}