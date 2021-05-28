#pragma once

#include <glm/glm.hpp>

namespace Deya { 
namespace Math
{
    /**
     * @brief Returns a ton of information based on a transformation matrix
     * 
     * @param transform Transform matrix to be "decomposed" 
     * @param outTranslation The translation variable that will store the translation from the transform matrix
     * @param outRotation The rotation variable that will store the rotation from the transform matrix
     * @param outScale The scale variable that will store the scale from the transform matrix 
     */
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}}