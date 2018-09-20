# Urho3DCompute
Compute shaders for Urho3D, presently only contains OpenCL support.

This dump is rather barren, the dump is *mirror-deep* though and I'll add the appropriate fetching and CMake scripts in time. The intent is to be 100% copy-pastable with some basic CMake patching.

---

Compute here is fairly raw and this implementation favors subscribing to graphics/renderer events to do compute or using OpenCL to do compute whenever (based on driver scheduler).

In general compute is a raw task so there will not be much showing up here. At the most some helpers for tying Compute into the `RenderPath` will get added.

The common interface follows the OpenCL model.

## Status

- OpenCL
  - Fully functional
- DirectX11
  - Pretty much done, requires testing before adding
- OpenGL
  - Erhmagerd ... requires fallbacks for resolving the mismatches of incomplete compute support that is common
  - Compute support without SSBO support is ridiculous common on hardware
    - Interface was never really designed to handle *hacks* like those