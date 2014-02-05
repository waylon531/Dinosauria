#include "physics/land.hpp"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#define RES 128

physics::Heightfield::Heightfield(float* hmap, const int xres, const int yres, const float hscale, const float scale)
{
  //shape = new btSphereShape(10);
  float* nhmap = new float[RES*RES];
  for(int x=0; x<RES; x++)
    {
      for(int y=0; y<RES; y++)
	{
	  nhmap[x*RES+y] = hmap[(((x)*xres)/RES) + xres*((y)*xres)/RES];
	}
    }
  shape = new btHeightfieldTerrainShape(RES, RES, nhmap, 1.f, -500.f, 500.f, 1, PHY_FLOAT, true);
  shape->setLocalScaling(btVector3(scale/float(RES),1.f,scale/float(RES)));
  /*shape = new btTriangleMesh;
  for(int x=0; x<xres; x++)
    {
      for(int y=0; y<yres; y++)
	{
	  ((btConvexHullShape*)shape)->addTriangle(btVector3(),
						   btVector3(),
						   btVector3());
	}
	}*/
  btDefaultMotionState* fallMotionState =
    //  new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-scale/2,0,-scale/2)));
    new btDefaultMotionState(btTransform(btQuaternion(btVector3(0,1,0),0.f*M_PI),btVector3(0,0,0)));
  btTransform trans;
  //fallMotionState->getWorldTransform(trans);
  //trans.setRotation(btQuaternion(0,1,0, M_PI));
  //fallMotionState->setWorldTransform(trans);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0,fallMotionState,shape,btVector3(0,0,0));
  body = new btRigidBody(fallRigidBodyCI);
  //body->setDamping(.98, .95);
  body->setRestitution(.9);
}
