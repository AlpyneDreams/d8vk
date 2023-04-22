#pragma once

#include "d3d8_include.h"
#include "../d3d9/d3d9_bridge.h"
#include "../util/config/config.h"

namespace dxvk {
  struct D3D8Options {
    /// Treat D24S8 and D16 as early NVIDIA shadow buffers that test
    /// reference values in the range [0..2^N - 1] where N is bit depth.
    /// Also emulates hardware shadow filtering using a bilinear 2x2 PCF.
    bool useShadowBuffers = false;

    /// Remap DEFAULT pool vertex and index buffers to MANAGED to improve
    /// performance by avoiding waiting for games that frequently lock their buffers.
    ///
    /// This implicitly disables direct buffer mapping. Some applications may
    /// need this option disabled to keep from overwriting in-use buffer regions.
    bool managedBufferPlacement = true;

    D3D8Options() {}
    D3D8Options(const Config& config) {
      useShadowBuffers        = config.getOption("d3d8.useShadowBuffers",       useShadowBuffers);
      managedBufferPlacement  = config.getOption("d3d8.managedBufferPlacement", managedBufferPlacement);
    }

  };

}
