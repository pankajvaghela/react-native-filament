//
// Created by Hanno Gödecke on 05.03.24.
//

#pragma once

#include "RigidBodyWrapper.h"
#include "jsi/HybridObject.h"

#include <btBulletDynamicsCommon.h>

namespace margelo {
class DiscreteDynamicWorldWrapper : public HybridObject {
public:
  explicit DiscreteDynamicWorldWrapper(double gravityX, double gravityY, double gravityZ);

  void loadHybridMethods() override;

private:
  void addRigidBody(std::shared_ptr<RigidBodyWrapper> rigidBody);
  void stepSimulation(double timeStep, double maxSubSteps, double fixedTimeStep);

private:
  std::unique_ptr<btBroadphaseInterface> broadphase;
  std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
  std::unique_ptr<btCollisionDispatcher> dispatcher;
  std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
  std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};
} // namespace margelo
