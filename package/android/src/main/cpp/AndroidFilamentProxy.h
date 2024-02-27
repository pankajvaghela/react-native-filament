//
// Created by Marc Rousavy on 20.02.24.
//

#pragma once

#include "FilamentProxy.h"
#include "java-bindings/JFilamentProxy.h"
#include <fbjni/fbjni.h>
#include <jni.h>

namespace margelo {

using namespace facebook;

/**
 * Implementation for the abstract class FilamentProxy, which uses the JNI Hybrid Class "JFilamentProxy" underneath.
 */
class AndroidFilamentProxy : public FilamentProxy {
public:
  explicit AndroidFilamentProxy(jni::alias_ref<JFilamentProxy::javaobject> filamentProxy);
  ~AndroidFilamentProxy();

private:
  std::shared_ptr<FilamentBuffer> getAssetByteBuffer(std::string path) override;
  std::shared_ptr<FilamentView> findFilamentView(int id) override;
  std::shared_ptr<Choreographer> createChoreographer() override;

private:
  jni::global_ref<JFilamentProxy::javaobject> _proxy;
};

} // namespace margelo
