#pragma once

#include "d3d8_include.h"
#include "../d3d9/d3d9_bridge.h"
#include "../util/config/config.h"

namespace dxvk {
  struct D3D8Options {

    /// Some games rely on undefined behavior by using undeclared vertex shader inputs.
    /// The simplest way to fix them is to simply modify their vertex shader decl.
    /// 
    /// This option takes a comma-separated list of colon-separated number pairs, where
    /// the first number is a D3DVSDE_REGISTER value, the second is a D3DVSDT_TYPE value.
    ///   e.g. "0:2,3:2,7:1" for float3 position : v0, float3 normal : v3, float2 uv : v7
    std::vector<std::pair<D3DVSDE_REGISTER, D3DVSDT_TYPE>> forceVsDecl;

    /// Specialized drawcall batcher, typically for games that draw a lot of similar
    /// geometry in separate drawcalls (sometimes even one triangle at a time).
    ///
    /// May hurt performance outside of specifc games that benefit from it.
    bool batching = false;

    D3D8Options() {}
    D3D8Options(const Config& config) {
      auto forceVsDeclStr     = config.getOption<std::string>("d3d8.forceVsDecl",  "");
      batching                = config.getOption<bool>       ("d3d8.batching",               batching);

      parseVsDecl(forceVsDeclStr);
    }

    void parseVsDecl(const std::string& decl);
  };
}
