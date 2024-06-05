#include "resources_manager.hpp"
#include "../renderer/shader.hpp"
#include "../renderer/texture_2d.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourcesManager::ResourcesManager(const std::string& executable_path){
    size_t found = executable_path.find_last_of("/\\");
    m_path = executable_path.substr(0, found);
}

std::string ResourcesManager::get_file_path(const std::string& relative_path) const{
    std::fstream file;
    file.open(m_path + "/" + relative_path.c_str(), std::ios::in | std::ios::binary);
    if(!file.is_open()){
        std::cerr << "Failed to open file: " << relative_path << std::endl;
        return std::string{};
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourcesManager::load_shader(const::std::string& shader_name, const std::string& vertex_path, const std::string& fragment_path){
    std::string vertex_string = get_file_path(vertex_path);
    if(vertex_string.empty()){
        std::cerr << "No vertex shader." << std::endl;
        return nullptr;
    }

    std::string fragment_string = get_file_path(fragment_path);
    if(fragment_string.empty()){
        std::cerr << "No fragment shader." << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::ShaderProgram>& new_shader = m_shader_program.emplace(shader_name, std::make_shared<Renderer::ShaderProgram>(vertex_string, fragment_string)).first->second;
    if (new_shader->isCompiled()){
        return new_shader;
    }
    std::cerr << "Can't load shader program:\n" << "Vertex: " << vertex_path << "\n" << "Fragment: " << fragment_path << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourcesManager::get_shader(const std::string& shader_name){
    ShaderProgramsMap::const_iterator it = m_shader_program.find(shader_name);
    if(it != m_shader_program.end()){
        return it->second;
    }
    std::cerr << "Can't find shader: " << shader_name << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourcesManager::load_texture(const std::string& texture_name, const std::string& texture_path){
    int channels = 0;
    int width = 0;
    int height = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(std::string(m_path + "/" + texture_path).c_str(), &width, &height, &channels, 0);
    if (!pixels){
        std::cerr << "Can't load texture: " << texture_path << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::Texture2D> new_texture = m_textures.emplace(texture_name, std::make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
    stbi_image_free(pixels);
    return new_texture;
}

std::shared_ptr<Renderer::Texture2D> ResourcesManager::get_texture(const std::string& texture_name){
    TexturesMap::const_iterator it = m_textures.find(texture_name);
    if(it != m_textures.end()){
        return it->second;
    }
    std::cerr << "Can't find textures: " << texture_name << std::endl;
    return nullptr;
}