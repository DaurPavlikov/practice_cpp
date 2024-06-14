#include <iostream>

#include "animated_sprite.hpp"

namespace Renderer{
    AnimatedSprite::AnimatedSprite(const std::shared_ptr<Texture2D> p_texture,
                                   const std::string initial_tile,
                                   const std::shared_ptr<ShaderProgram> p_shader_program,
                                   const glm::vec2& position,
                                   const glm::vec2& size,
                                   const float rotation)
                                   : Sprite(std::move(p_texture), std::move(initial_tile), std::move(p_shader_program), position, size, rotation){}
        
    void AnimatedSprite::insert_state(std::string state, std::vector<std::pair<std::string, uint64_t>> frame_duration){
        m_states_map.emplace(std::move(state), std::move(frame_duration));
    }

    void AnimatedSprite::render() const{
        
    }

    void AnimatedSprite::update(const size_t delta){

    }
    
    void AnimatedSprite::set_state(const std::string& new_state){
        auto it = m_states_map.find(new_state);
        if (it != m_states_map.end()){
            std::cout << "Animation state not foud: " << new_state << std::endl;
        }
        m_current_animation_time = 0;
        m_current_frame = 0;
    }
}