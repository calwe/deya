#include "dypch.h"
#include "Deya/Maths/Maths.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Deya::Math 
{
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
    {
        // Custom function based glm::decompose in matrix_decompose.inl

        using namespace glm;
        using T = float; // glm::decompose is originally templated

        mat4 LocalMatrix(transform);

        // Normalize
        if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
            return false;

        // First, isolate perspective. !MESSY!
        if (
            epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
        {
            // Clear the perspective partition
            LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
            LocalMatrix[3][3] = static_cast<T>(1);
        }

        // Next take care of translation (easy)
        outTranslation = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

        vec3 Row[3], Pdum3;

        // Now get scale and shear
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // Compute X scale factor and normalzie first row
        outScale.x = length(Row[0]);
        Row[0] = detail::scale(Row[0], static_cast<T>(1));
        outScale.y = length(Row[1]);
        Row[1] = detail::scale(Row[1], static_cast<T>(1));
        outScale.z = length(Row[2]);
        Row[2] = detail::scale(Row[2], static_cast<T>(1));

        outRotation.y = asin(-Row[0][2]);
        if (cos(outRotation.y) != 0)
        {
            outRotation.x = atan2(Row[1][2], Row[2][2]);
            outRotation.z = atan2(Row[0][1], Row[0][0]);   
        }
        else
        {
            outRotation.x = atan2(-Row[2][0], Row[1][1]);
            outRotation.z = 0;
        }

        return true;
    }
}