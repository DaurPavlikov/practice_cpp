#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace Renderer {
    class ShaderProgram{
    public:
        ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader);
        ~ShaderProgram();
        bool isCompiled() const {return m_is_compiled;}
        void use() const;
        void set_int(const std::string& name, const GLint value);
        void set_matrix4(const std::string& name, const glm::mat4& matrix);

        ShaderProgram() = delete;
        ShaderProgram(ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

    private:
        bool createShader(const std::string& source, const GLenum shader_type, GLuint& shader_id);
        bool m_is_compiled = false;
        GLuint m_id = 0;
    };
}