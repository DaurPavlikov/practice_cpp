#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

namespace Renderer{
    ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader){
        GLuint vertex_shader_id;
        if (!createShader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_id)){
            std::cerr << "VERTEX SHADER: Compile time error" << std::endl;
        }

        GLuint fragment_shader_id;
        if (!createShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_id)){
            std::cerr << "FRAGMENT SHADER: Compile time error" << std::endl;
            glDeleteShader(vertex_shader_id);
            return;
        }

        m_id = glCreateProgram();
        glAttachShader(m_id, vertex_shader_id);
        glAttachShader(m_id, fragment_shader_id);
        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success){
            GLchar info_log[1024];
            glGetShaderInfoLog(m_id, 1024, nullptr, info_log);
            std::cerr << "SHADER LINK ERROR: Linking time error:\n" << info_log << std::endl;
        }else{
            m_is_compiled = true;
        }

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
        
    }

    bool ShaderProgram::createShader(const std::string& source, const GLenum shader_type, GLuint& shader_id){
        shader_id = glCreateShader(shader_type);
        const char* code = source.c_str();
        glShaderSource(shader_id, 1, &code, nullptr);
        glCompileShader(shader_id);

        GLint success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success){
            GLchar info_log[1024];
            glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);
            std::cerr << "SHADER ERROR: Compile time error:\n" << info_log << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::~ShaderProgram(){
        glDeleteProgram(m_id);
    }

    void ShaderProgram::use() const{
        glUseProgram(m_id);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept{
        glDeleteProgram(m_id);
        m_id = shaderProgram.m_id;
        m_is_compiled = shaderProgram.m_is_compiled;
        shaderProgram.m_id = 0;
        shaderProgram.m_is_compiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept{
        m_id = shaderProgram.m_id;
        m_is_compiled = shaderProgram.m_is_compiled;
        shaderProgram.m_id = 0;
        shaderProgram.m_is_compiled = false;
    }

    void ShaderProgram::set_int(const std::string& name, const GLint value){
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }

    void ShaderProgram::set_matrix4(const std::string& name, const glm::mat4& matrix){
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
}