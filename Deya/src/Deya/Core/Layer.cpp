#include "dypch.h"
#include "Deya/Core/Layer.h"

namespace Deya
{
    Layer::Layer(const std::string& debugName)
        : m_DebugName(debugName) {}

    Layer::~Layer() {}
}