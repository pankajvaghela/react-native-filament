//
// Created by Marc Rousavy on 21.02.24.
//

#pragma once

#include "Surface.h"
#include "SurfaceProvider.h"
#include <core/utils/EntityWrapper.h>
#include <filament/Engine.h>
#include <filament/SwapChain.h>
#include <gltfio/AssetLoader.h>
#include <gltfio/MaterialProvider.h>

#include "CameraWrapper.h"
#include "RendererWrapper.h"
#include "SceneWrapper.h"
#include "ViewWrapper.h"
#include "jsi/HybridObject.h"

namespace margelo {

using namespace filament;

class EngineWrapper : public HybridObject {
public:
  explicit EngineWrapper();
  ~EngineWrapper();

  void setSurfaceProvider(std::shared_ptr<SurfaceProvider> surfaceProvider);

  void loadHybridMethods() override;

private:
  void setSurface(std::shared_ptr<Surface> surface);
  void destroySurface();
  // Wrapper methods
  std::shared_ptr<RendererWrapper> createRenderer();
  std::shared_ptr<SceneWrapper> createScene();
  std::shared_ptr<CameraWrapper> createCamera();
  std::shared_ptr<ViewWrapper> createView();

  // Custom simplification methods
  std::shared_ptr<EntityWrapper> createDefaultLight();

private:
  std::shared_ptr<Engine> _engine;
  std::shared_ptr<SurfaceProvider> _surfaceProvider;
  std::shared_ptr<Listener> _listener;

  // Internals that we might need to split out later
  filament::gltfio::MaterialProvider* _materialProvider;
  filament::gltfio::AssetLoader* _assetLoader;
  std::shared_ptr<SwapChain> _swapChain;
};

} // namespace margelo
