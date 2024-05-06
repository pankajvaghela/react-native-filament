//
// Created by Marc Rousavy on 21.02.24.
//

#include "JFilamentRecorder.h"
#include <android/native_window_jni.h>

namespace margelo {

JFilamentRecorder::JFilamentRecorder(const jni::alias_ref<jhybridobject>& javaThis, int width, int height, int fps, double bitRate)
    : FilamentRecorder(width, height, fps, bitRate), _javaPart(make_global(javaThis)) {}

JFilamentRecorder::~JFilamentRecorder() {
  __android_log_write(ANDROID_LOG_INFO, TAG, "Destroying JFilamentRecorder...");
  if (_nativeWindow != nullptr) {
    ANativeWindow_release(_nativeWindow);
  }
}

void JFilamentRecorder::registerNatives() {
  registerHybrid({
      makeNativeMethod("initHybrid", JFilamentRecorder::initHybrid),
  });
}

void JFilamentRecorder::renderFrame(double timestamp) {
  // TODO: Do anything here??
}

void* JFilamentRecorder::getNativeWindow() {
  if (_nativeWindow == nullptr) {
    static const auto method = javaClassLocal()->getMethod<jobject()>("getNativeWindow");
    jni::local_ref<jobject> nativeWindow = method(_javaPart);
    _nativeWindow = ANativeWindow_fromSurface(jni::Environment::current(), nativeWindow.get());
  }
  return _nativeWindow;
}

std::future<void> JFilamentRecorder::startRecording() {
  auto self = shared<JFilamentRecorder>();
  return std::async(std::launch::async, [self]() {
    static const auto method = self->javaClassLocal()->getMethod<void()>("startRecording");
    method(self->_javaPart);
  });
}

std::future<std::string> JFilamentRecorder::stopRecording() {
  auto self = shared<JFilamentRecorder>();
  return std::async(std::launch::async, [self]() -> std::string {
    static const auto method = self->javaClassLocal()->getMethod<void()>("stopRecording");
    method(self->_javaPart);
    return self->getOutputFile();
  });
}

std::string JFilamentRecorder::getOutputFile() {
  static const auto method = javaClassLocal()->getMethod<std::string()>("getOutputFile");
  jni::local_ref<jstring> path = method(_javaPart);
  return path->toString();
}

bool JFilamentRecorder::getIsRecording() {
  throw std::runtime_error("isRecording is not yet implemented!");
}

jni::local_ref<JFilamentRecorder::jhybriddata> JFilamentRecorder::initHybrid(jni::alias_ref<JFilamentRecorder::jhybridobject> jThis,
                                                                             int width, int height, int fps, double bitRate) {
  __android_log_write(ANDROID_LOG_INFO, TAG, "Initializing JFilamentRecorder...");
  return makeCxxInstance(jThis, width, height, fps, bitRate);
}

} // namespace margelo