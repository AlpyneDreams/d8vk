#pragma once

#include "d3d8_include.h"
#include "../d3d9/d3d9_bridge.h"
#include "../util/config/config.h"

namespace dxvk {
  struct D3D8Options {
    /// Treat D24S8 and D16 as early NVIDIA shadow buffers that test
    /// reference values in the range [0..2^N - 1] where N is bit depth.
    bool useShadowBuffers = false;

    D3D8Options() {}
    D3D8Options(const Config& config) {
      useShadowBuffers = config.getOption("d3d8.useShadowBuffers", useShadowBuffers);
    }

  };

}
