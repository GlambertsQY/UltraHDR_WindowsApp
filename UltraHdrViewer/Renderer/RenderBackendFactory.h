#pragma once

#include <memory>

#include "IRenderBackend.h"

namespace UltraHdrViewer::Renderer
{
    std::unique_ptr<IRenderBackend> CreateRenderBackend(RenderBackendKind kind);
}
