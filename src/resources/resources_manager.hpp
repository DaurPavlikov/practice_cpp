#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer{
    class ShaderProgram;
    class Texture2D;
}

class ResourcesManager{
public:
    ResourcesManager(const std::string& executable_path);
    ~ResourcesManager() = default;

    ResourcesManager(const ResourcesManager&) = delete;
    ResourcesManager& operator=(const ResourcesManager&) = delete;
    ResourcesManager& operator=(ResourcesManager&&) = delete;
    ResourcesManager(ResourcesManager&&) = delete;

    std::shared_ptr<Renderer::ShaderProgram> load_shader(const::std::string& shader_name,
                                                         const std::string& vertex_path,
                                                         const std::string& fragment_path);
    std::shared_ptr<Renderer::ShaderProgram> get_shader(const std::string& shader_name);

    std::shared_ptr<Renderer::Texture2D> load_texture(const std::string& texture_name, const std::string& texture_path);
    std::shared_ptr<Renderer::Texture2D> get_texture(const std::string& texture_name);

private:
    std::string get_file_path(const std::string& relative_path) const;

    typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
    ShaderProgramsMap m_shader_program;

    typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
    TexturesMap m_textures;

    std::string m_path;
};