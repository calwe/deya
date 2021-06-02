#pragma once

#include "Deya/Scene/SceneCamera.h"

namespace Deya
{
    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true; // TODO: move to scene?
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };
}
