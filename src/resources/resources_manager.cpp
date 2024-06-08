#include "resources_manager.hpp"
#include "../renderer/shader.hpp"
#include "../renderer/texture_2d.hpp"
#include "../renderer/sprite.hpp"

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

    std::shared_ptr<Renderer::Texture2D> new_texture = m_textures.emplace(texture_name, std::make_shared<Renderer::Texture2D>(
        width,
        height,
        pixels,
        channels,
        GL_NEAREST,
        GL_CLAMP_TO_EDGE)).first->second;
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

std::shared_ptr<Renderer::Sprite> ResourcesManager::load_sprite(const std::string& sprite_name,
                                                                const std::string& texture_name,
                                                                const std::string& shader_name,
                                                                const unsigned int sprite_width,
                                                                const unsigned int sprite_height,
                                                                const std::string& tile_name){
    auto texture = get_texture(texture_name);
    if (!texture){
        std::cerr << "Can't find texture " << texture_name << " for the sprite: " << sprite_name << std::endl;
    }

    auto shader = get_shader(shader_name);
    if (!shader){
        std::cerr << "Can't find shader " << shader_name << " for the sprite: " << sprite_name << std::endl;
    }

    std::shared_ptr<Renderer::Sprite> new_sprite = m_sprites.emplace(texture_name, std::make_shared<Renderer::Sprite>(
        texture,
        tile_name,
        shader,
        glm::vec2(0.0f, 0.0f),
        glm::vec2(sprite_width, sprite_height))).first->second;
    return new_sprite;
}

std::shared_ptr<Renderer::Sprite> ResourcesManager::get_sprite(const std::string& sprite_name){
    SpritesMap::const_iterator it = m_sprites.find(sprite_name);
    if(it != m_sprites.end()){
        return it->second;
    }
    std::cerr << "Can't find sprite: " << sprite_name << std::endl;
    return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourcesManager::load_texture_atlas(const std::string& texture_name,
                                                                          const std::string& texture_path,
                                                                          const std::vector<std::string> tile,
                                                                          const unsigned int tile_sheet_width,
                                                                          const unsigned int tile_sheet_height){
    auto p_texture = load_texture(std::move(texture_name), std::move(texture_path));
    if (p_texture){
        const unsigned int texture_width = p_texture->width();
        const unsigned int texture_height = p_texture->height();
        unsigned int tile_offset_x = 0;
        unsigned int tile_offset_y = texture_height;
        for (const auto& current_tile_name : tile){
            glm::vec2 left_bottom_uv(static_cast<float>(tile_offset_x) / texture_width,
                                     static_cast<float>(tile_offset_y - tile_sheet_height) / texture_height);
            glm::vec2 right_top_uv(static_cast<float>(tile_offset_x + tile_sheet_width) / texture_width,
                                   static_cast<float>(tile_offset_y) / texture_height);
            p_texture->add_tile(std::move(current_tile_name), left_bottom_uv, right_top_uv);
            tile_offset_x += tile_sheet_width;
            if (tile_offset_x >= texture_width){
                tile_offset_x = 0;
                tile_offset_y -= tile_sheet_height;
            }
        }
    }
    return p_texture;
}
