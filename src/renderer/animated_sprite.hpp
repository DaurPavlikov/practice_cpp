#pragma once

#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "sprite.hpp"

namespace Renderer{
    class AnimatedSprite : public Sprite{
    public:
        AnimatedSprite(const std::shared_ptr<Texture2D> p_texture,
               const std::string initial_tile,
               const std::shared_ptr<ShaderProgram> p_shader_program,
               const glm::vec2& position = glm::vec2(0.0f),
               const glm::vec2& size = glm::vec2(1.0f),
               const float rotation = 0.0f);
        
        void insert_state(std::string state, std::vector<std::pair<std::string, uint64_t>> frame_duration);
        void render() const override;
        void update(const size_t delta);
        void set_state(const std::string&  new_state);

    private:
        std::map<std::string, std::vector<std::pair<std::string, size_t>>> m_states_map;
        size_t m_current_frame = 0;
        uint64_t m_current_animation_time = 0;
    };
}
