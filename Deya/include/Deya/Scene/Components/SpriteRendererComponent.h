#pragma once

#include <glm/glm.hpp>
#include "Deya/Renderer/Texture.h"

namespace Deya
{
    enum class SpriteRendererTypes
    {
        Colour = 0,
        Image
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Colour{ 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> Texture;        
        SpriteRendererTypes Type = SpriteRendererTypes::Colour;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& colour)
            : Colour(colour) {}
    };
}
