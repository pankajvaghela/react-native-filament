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
#include "SwapChainWrapper.h"
#include "ViewWrapper.h"
#include "jsi/HybridObject.h"
#include <camutils/Manipulator.h>
#include <core/utils/ManipulatorWrapper.h>

namespace margelo {

using namespace filament;
using namespace camutils;

using ManipulatorBuilder = Manipulator<float>::Builder;

class EngineWrapper : public HybridObject {
public:
  explicit EngineWrapper();
  ~EngineWrapper();

  void setSurfaceProvider(std::shared_ptr<SurfaceProvider> surfaceProvider);

  void loadHybridMethods() override;

private:
  void setSurface(std::shared_ptr<Surface> surface);
  void destroySurface();
  void surfaceSizeChanged(int width, int height);

  // Custom simplification methods
  void createDefaultLight();

private:
  std::shared_ptr<Engine> _engine;
  std::shared_ptr<SurfaceProvider> _surfaceProvider;
  std::shared_ptr<Listener> _listener;

  // Internals that we might need to split out later
  filament::gltfio::MaterialProvider* _materialProvider;
  filament::gltfio::AssetLoader* _assetLoader;

private:
  // Internals we create, but share the access with the user
  std::shared_ptr<RendererWrapper> _renderer;
  std::shared_ptr<SwapChainWrapper> _swapChain;
  std::shared_ptr<SceneWrapper> _scene;
  std::shared_ptr<ViewWrapper> _view;
  std::shared_ptr<CameraWrapper> _camera;
  std::shared_ptr<ManipulatorWrapper> _cameraManipulator;

private:
  std::shared_ptr<RendererWrapper> createRenderer();
  std::shared_ptr<SwapChainWrapper> createSwapChain(std::shared_ptr<Surface> surface);
  std::shared_ptr<SceneWrapper> createScene();
  std::shared_ptr<ViewWrapper> createView();
  std::shared_ptr<CameraWrapper> createCamera();
  std::shared_ptr<ManipulatorWrapper> createCameraManipulator(int windowWidth, int windowHeight);

private:
  // Getters for shared objects
  std::shared_ptr<RendererWrapper> getRenderer() {
    return _renderer;
  }
  std::shared_ptr<SwapChainWrapper> getSwapChain() {
    return _swapChain;
  }
  std::shared_ptr<SceneWrapper> getScene() {
    return _scene;
  }
  std::shared_ptr<ViewWrapper> getView() {
    return _view;
  }
  std::shared_ptr<CameraWrapper> getCamera() {
    return _camera;
  }
  std::shared_ptr<ManipulatorWrapper> getCameraManipulator() {
    return _cameraManipulator;
  }
};

} // namespace margelo
