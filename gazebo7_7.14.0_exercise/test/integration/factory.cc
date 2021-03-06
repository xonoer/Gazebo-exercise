/*
 * Copyright (C) 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include <string.h>
#include "gazebo/math/Helpers.hh"
#include "gazebo/transport/TransportTypes.hh"
#include "gazebo/transport/Node.hh"

#include "gazebo/rendering/RenderEngine.hh"
#include "gazebo/rendering/Camera.hh"
#include "gazebo/sensors/SensorsIface.hh"
#include "gazebo/sensors/CameraSensor.hh"
#include "gazebo/test/ServerFixture.hh"
#include "images_cmp.h"
#include "gazebo/test/helper_physics_generator.hh"

using namespace gazebo;
class FactoryTest : public ServerFixture,
                    public testing::WithParamInterface<const char*>
{
  public: void BoxSdf(const std::string &_physicsEngine);
  public: void Box(const std::string &_physicsEngine);
  public: void Sphere(const std::string &_physicsEngine);
  public: void Cylinder(const std::string &_physicsEngine);
  public: void Clone(const std::string &_physicsEngine);
};


class LightFactoryTest : public ServerFixture
{
  /// \brief Light factory publisher.
  protected: transport::PublisherPtr lightFactoryPub;
};

///////////////////////////////////////////////////
// Verify that sdf is retained by entities spawned
// via factory messages. A change between 1.6, 1.7
// caused entities to lose their sdf data
// (see issue #651)
void FactoryTest::BoxSdf(const std::string &_physicsEngine)
{
  ignition::math::Pose3d setPose;
  Load("worlds/empty.world", true, _physicsEngine);
  physics::WorldPtr world = physics::get_world("default");
  ASSERT_TRUE(world != NULL);

  unsigned int entityCount = 6;

  for (unsigned int i = 0; i < entityCount; ++i)
  {
    std::ostringstream name;
    name << "test_box_" << i;
    setPose.Set(ignition::math::Vector3d(0, 0, i+0.5),
        ignition::math::Quaterniond(0, 0, 0));
    SpawnBox(name.str(), ignition::math::Vector3d(1, 1, 1), setPose.Pos(),
        setPose.Rot().Euler());
  }

  // This loop must be separate from the previous loop to cause
  // the failure.
  for (unsigned int i = 0; i < entityCount; ++i)
  {
    std::ostringstream name;
    name << "test_box_" << i;

    physics::ModelPtr model = world->GetModel(name.str());
    ASSERT_TRUE(model != NULL);
    msgs::Model msg;
    model->FillMsg(msg);
    EXPECT_TRUE(msg.has_pose());
    // gzerr << msg.DebugString() << '\n';
  }
}

/////////////////////////////////////////////////
TEST_P(FactoryTest, BoxSdf)
{
  BoxSdf(GetParam());
}

/////////////////////////////////////////////////
void FactoryTest::Box(const std::string &_physicsEngine)
{
  ignition::math::Pose3d setPose, testPose;
  Load("worlds/empty.world", true, _physicsEngine);

  for (unsigned int i = 0; i < 100; i++)
  {
    std::ostringstream name;
    name << "test_box_" << i;
    setPose.Set(ignition::math::Vector3d(0, 0, i+0.5),
        ignition::math::Quaterniond(0, 0, 0));
    SpawnBox(name.str(), math::Vector3(1, 1, 1), setPose.Pos(),
        setPose.Rot().Euler());
    testPose = GetEntityPose(name.str()).Ign();
    EXPECT_TRUE(math::equal(testPose.Pos().X(), setPose.Pos().X(), 0.1));
    EXPECT_TRUE(math::equal(testPose.Pos().Y(), setPose.Pos().Y(), 0.1));
    EXPECT_TRUE(math::equal(testPose.Pos().Z(), setPose.Pos().Z(), 0.1));
  }
}

/////////////////////////////////////////////////
TEST_P(FactoryTest, Box)
{
  Box(GetParam());
}

/////////////////////////////////////////////////
void FactoryTest::Sphere(const std::string &_physicsEngine)
{
  ignition::math::Pose3d setPose, testPose;
  Load("worlds/empty.world", true, _physicsEngine);

  for (unsigned int i = 0; i < 100; i++)
  {
    std::ostringstream name;
    name << "test_sphere_" << i;
    setPose.Set(ignition::math::Vector3d(0, 0, i+0.5),
        ignition::math::Quaterniond(0, 0, 0));
    SpawnSphere(name.str(), setPose.Pos(), setPose.Rot().Euler());
    testPose = GetEntityPose(name.str()).Ign();
    EXPECT_TRUE(math::equal(testPose.Pos().X(), setPose.Pos().X(), 0.1));
    EXPECT_TRUE(math::equal(testPose.Pos().Y(), setPose.Pos().Y(), 0.1));
    EXPECT_TRUE(math::equal(testPose.Pos().Z(), setPose.Pos().Z(), 0.1));
  }
}

/////////////////////////////////////////////////
TEST_P(FactoryTest, Sphere)
{
  Sphere(GetParam());
}

/////////////////////////////////////////////////
void FactoryTest::Cylinder(const std::string &_physicsEngine)
{
  ignition::math::Pose3d setPose, testPose;
  Load("worlds/empty.world", true, _physicsEngine);

  for (unsigned int i = 0; i < 100; i++)
  {
    std::ostringstream name;
    name << "test_cylinder_" << i;
    setPose.Set(
        ignition::math::Vector3d(0, 0, i+0.5),
        ignition::math::Quaterniond(0, 0, 0));
    SpawnCylinder(name.str(), setPose.Pos(), setPose.Rot().Euler());
    testPose = GetEntityPose(name.str()).Ign();
    EXPECT_TRUE(math::equal(testPose.Pos().X(), setPose.Pos().X(), 0.1));
    EXPECT_TRUE(math::equal(testPose.Pos().Y(), setPose.Pos().Y(), 0.1));
    EXPECT_TRUE(math::equal(testPose.Pos().Z(), setPose.Pos().Z(), 0.1));
  }
}

/////////////////////////////////////////////////
TEST_P(FactoryTest, Cylinder)
{
  Cylinder(GetParam());
}

/////////////////////////////////////////////////
void FactoryTest::Clone(const std::string &_physicsEngine)
{
  ignition::math::Pose3d testPose;
  Load("worlds/pr2.world", true, _physicsEngine);

  // clone the pr2
  std::string name = "pr2";
  msgs::Factory msg;
  ignition::math::Pose3d clonePose;
  clonePose.Set(ignition::math::Vector3d(2, 3, 0.5),
      ignition::math::Quaterniond(0, 0, 0));
  msgs::Set(msg.mutable_pose(), clonePose);
  msg.set_clone_model_name(name);
  this->factoryPub->Publish(msg);

  // Wait for the pr2 clone to spawn
  std::string cloneName = name + "_clone";
  this->WaitUntilEntitySpawn(cloneName, 100, 100);

  EXPECT_TRUE(this->HasEntity(cloneName));
  testPose = GetEntityPose(cloneName).Ign();
  EXPECT_TRUE(math::equal(testPose.Pos().X(), clonePose.Pos().X(), 0.1));
  EXPECT_TRUE(math::equal(testPose.Pos().Y(), clonePose.Pos().Y(), 0.1));
  EXPECT_TRUE(math::equal(testPose.Pos().Z(), clonePose.Pos().Z(), 0.1));

  // Verify properties of the pr2 clone with the original model.
  physics::WorldPtr world = physics::get_world("default");
  ASSERT_TRUE(world != NULL);

  // Check model
  physics::ModelPtr model = world->GetModel(name);
  ASSERT_TRUE(model != NULL);
  physics::ModelPtr modelClone = world->GetModel(cloneName);
  ASSERT_TRUE(modelClone != NULL);
  EXPECT_EQ(model->GetJointCount(), modelClone->GetJointCount());
  EXPECT_EQ(model->GetLinks().size(), modelClone->GetLinks().size());
  EXPECT_EQ(model->GetSensorCount(), modelClone->GetSensorCount());
  EXPECT_EQ(model->GetPluginCount(), modelClone->GetPluginCount());
  EXPECT_EQ(model->GetAutoDisable(), modelClone->GetAutoDisable());

  // Check links
  physics::Link_V links = model->GetLinks();
  physics::Link_V linkClones = modelClone->GetLinks();
  for (unsigned int i = 0; i < links.size(); ++i)
  {
    physics::LinkPtr link = links[i];
    physics::LinkPtr linkClone = linkClones[i];

    EXPECT_EQ(link->GetSensorCount(), linkClone->GetSensorCount());
    EXPECT_EQ(link->GetKinematic(), linkClone->GetKinematic());

    // Check collisions
    physics::Collision_V collisions = link->GetCollisions();
    physics::Collision_V collisionClones = linkClone->GetCollisions();
    EXPECT_EQ(collisions.size(), collisionClones.size());
    for (unsigned int j = 0; j < collisions.size(); ++j)
    {
      physics::CollisionPtr collision = collisions[j];
      physics::CollisionPtr collisionClone = collisionClones[j];
      EXPECT_EQ(collision->GetShapeType(), collisionClone->GetShapeType());
      EXPECT_EQ(collision->GetMaxContacts(), collisionClone->GetMaxContacts());

      // Check surface
      physics::SurfaceParamsPtr surface = collision->GetSurface();
      physics::SurfaceParamsPtr cloneSurface = collisionClone->GetSurface();
      EXPECT_EQ(surface->collideWithoutContact,
          cloneSurface->collideWithoutContact);
      EXPECT_EQ(surface->collideWithoutContactBitmask,
          cloneSurface->collideWithoutContactBitmask);
    }

    // Check inertial
    physics::InertialPtr inertial = link->GetInertial();
    physics::InertialPtr inertialClone = linkClone->GetInertial();
    EXPECT_EQ(inertial->GetMass(), inertialClone->GetMass());
    EXPECT_EQ(inertial->GetCoG(), inertialClone->GetCoG());
    EXPECT_EQ(inertial->GetPrincipalMoments(),
        inertialClone->GetPrincipalMoments());
    EXPECT_EQ(inertial->GetProductsofInertia(),
        inertialClone->GetProductsofInertia());
  }

  // Check joints
  physics::Joint_V joints = model->GetJoints();
  physics::Joint_V jointClones = modelClone->GetJoints();
  for (unsigned int i = 0; i < joints.size(); ++i)
  {
    physics::JointPtr joint = joints[i];
    physics::JointPtr jointClone = jointClones[i];
    EXPECT_EQ(joint->GetAngleCount(), jointClone->GetAngleCount());
    for (unsigned j = 0; j < joint->GetAngleCount(); ++j)
    {
      EXPECT_EQ(joint->GetUpperLimit(j), jointClone->GetUpperLimit(j));
      EXPECT_EQ(joint->GetLowerLimit(j), jointClone->GetLowerLimit(j));
      EXPECT_EQ(joint->GetEffortLimit(j), jointClone->GetEffortLimit(j));
      EXPECT_EQ(joint->GetVelocityLimit(j), jointClone->GetVelocityLimit(j));
      EXPECT_EQ(joint->GetStopStiffness(j), jointClone->GetStopStiffness(j));
      EXPECT_EQ(joint->GetStopDissipation(j),
          jointClone->GetStopDissipation(j));
      EXPECT_EQ(joint->GetLocalAxis(j), jointClone->GetLocalAxis(j));
      EXPECT_EQ(joint->GetDamping(j), jointClone->GetDamping(j));
    }
  }
}

/////////////////////////////////////////////////
TEST_P(FactoryTest, Clone)
{
  Clone(GetParam());
}

// Disabling this test for now. Different machines return different
// camera images. Need a better way to evaluate rendered content.
// TEST_F(FactoryTest, Camera)
// {
/*
  if (rendering::RenderEngine::Instance()->GetRenderPathType() ==
      rendering::RenderEngine::NONE)
    return;

  math::Pose setPose, testPose;
  Load("worlds/empty.world");
  setPose.Set(math::Vector3(-5, 0, 5), math::Quaternion(0, GZ_DTOR(15), 0));
  SpawnCamera("camera_model", "camera_sensor2", setPose.pos,
      setPose.rot.GetAsEuler());

  unsigned char *img = NULL;
  unsigned int width;
  unsigned int height;
  GetFrame("camera_sensor2", &img, width, height);
  ASSERT_EQ(width, static_cast<unsigned int>(320));
  ASSERT_EQ(height, static_cast<unsigned int>(240));

  unsigned int diffMax = 0;
  unsigned int diffSum = 0;
  double diffAvg = 0;
  ImageCompare(&img, &empty_world_camera1,
      width, height, 3, diffMax, diffSum, diffAvg);
  // PrintImage("empty_world_camera1", &img, width, height, 3);
  ASSERT_LT(diffSum, static_cast<unsigned int>(100));
  ASSERT_EQ(static_cast<unsigned int>(0), diffMax);
  ASSERT_EQ(0.0, diffAvg);
  */
// }


/////////////////////////////////////////////////
TEST_F(LightFactoryTest, SpawnLight)
{
  Load("worlds/empty.world");

  physics::WorldPtr world = physics::get_world("default");
  ASSERT_TRUE(world != nullptr);

  // create lights using ~/factory/light topic
  this->lightFactoryPub = this->node->Advertise<msgs::Light>("~/factory/light");
  std::string light1Name = "new1_light";
  std::string light2Name = "new2_light";

  msgs::Light msg;
  msg.set_name(light1Name);
  ignition::math::Pose3d light1Pose = ignition::math::Pose3d(0, 2, 1, 0, 0, 0);
  msgs::Set(msg.mutable_pose(), light1Pose);
  this->lightFactoryPub->Publish(msg);

  msg.set_name(light2Name);
  ignition::math::Pose3d light2Pose = ignition::math::Pose3d(1, 0, 5, 0, 2, 0);
  msgs::Set(msg.mutable_pose(), light2Pose);
  this->lightFactoryPub->Publish(msg);

  // wait for light to spawn
  int sleep = 0;
  int maxSleep = 50;
  while ((!world->Light(light1Name) || !world->Light(light2Name)) &&
      sleep++ < maxSleep)
  {
    common::Time::MSleep(100);
  }

  // verify lights in world
  physics::LightPtr light1 = world->Light(light1Name);
  physics::LightPtr light2 = world->Light(light2Name);
  ASSERT_NE(nullptr, light1);
  ASSERT_NE(nullptr, light2);

  EXPECT_EQ(light1Name, light1->GetScopedName());
  EXPECT_EQ(light1Pose, light1->GetWorldPose().Ign());

  EXPECT_EQ(light2Name, light2->GetScopedName());
  EXPECT_EQ(light2Pose, light2->GetWorldPose().Ign());
}

INSTANTIATE_TEST_CASE_P(PhysicsEngines, FactoryTest, PHYSICS_ENGINE_VALUES);

/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
