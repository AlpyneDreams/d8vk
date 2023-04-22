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

    /// Remap DEFAULT pool vertex and index buffers above this size to the MANAGED pool
    /// to improve performance by avoiding waiting for games that frequently lock (large) buffers.
    /// 
    /// This implicitly disables direct buffer mapping. Some applications may need this option
    /// disabled for certain (smaller) buffers to keep from overwriting in-use buffer regions.
    uint32_t managedBufferPlacement = 0;

    D3D8Options() {}
    D3D8Options(const Config& config) {
      useShadowBuffers        = config.getOption<bool>      ("d3d8.useShadowBuffers",       useShadowBuffers);
      int32_t minManagedSize  = config.getOption<int32_t>   ("d3d8.managedBufferPlacement", managedBufferPlacement);
      managedBufferPlacement  = config.getOption<bool>      ("d3d8.managedBufferPlacement", true) ? minManagedSize : UINT32_MAX;
    }
  };
}
