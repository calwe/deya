#include "dypch.h"
#include "Deya/Layer.h"

namespace Deya
{
    Layer::Layer(const std::string& debugName)
        : m_DebugName(debugName) {}

    Layer::~Layer() {}
}