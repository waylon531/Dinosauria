#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Suites
#include <boost/test/unit_test.hpp>

#include "engine/engine.hpp"
#include "util.hpp"

BOOST_AUTO_TEST_SUITE(utilities)

BOOST_AUTO_TEST_CASE(asString)
{
  BOOST_CHECK_EQUAL(::asString(199), "199");
  BOOST_CHECK_EQUAL(::asString("hello"), "hello");
  BOOST_CHECK_EQUAL(::asString(0.5), "0.5");
}

BOOST_AUTO_TEST_CASE(clamp)
{
  BOOST_CHECK_EQUAL(::clamp(2,0,1), 1);
  BOOST_CHECK_EQUAL(::clamp(-2,0,1), 0);
  BOOST_CHECK_EQUAL(::clamp(.5,0.,1.), .5);
}

BOOST_AUTO_TEST_CASE(lerp)
{
  BOOST_CHECK_EQUAL(::lerp(0.f,1.f,.5), 0.5);
  BOOST_CHECK_EQUAL(::lerp(-1.f,1.f,.5), 0.);
  BOOST_CHECK_EQUAL(::lerp(0.f,1.f,0.), 0.);
  BOOST_CHECK_EQUAL(::lerp(0.f,1.f,1.), 1.);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Component)

BOOST_AUTO_TEST_CASE(constructors)
{
  BOOST_CHECK_NO_THROW(engine::Component c(new engine::Property<int>(14), "test0"));
}

BOOST_AUTO_TEST_CASE(component_id)
{
  engine::Component c(new engine::Property<int>(14), "test0");
  int i = engine::getComponentID("test0");
  engine::Component c2(new engine::Property<int>(14), "test1");
  BOOST_CHECK_EQUAL(c.ID, i);
  BOOST_CHECK(c.ID != c2.ID);
}

BOOST_AUTO_TEST_CASE(data_access)
{
  engine::Component c(new engine::Property<int>(14), "test0");
  BOOST_CHECK_EQUAL(c.getData<int>(), 14);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Entity)

BOOST_AUTO_TEST_CASE(constructors)
{
  BOOST_CHECK_NO_THROW(engine::Entity e(0, "test0"));
}

BOOST_AUTO_TEST_CASE(add_component)
{
  engine::Entity e(0);
  BOOST_CHECK_NO_THROW(e.addComponent(engine::Component(new engine::Property<int>(14), "test0")));
}

BOOST_AUTO_TEST_CASE(get_component)
{
  engine::Entity e(0);
  e.addComponent(engine::Component(new engine::Property<int>(14), "test0"));
  e.addComponent(engine::Component(new engine::Property<int>(15), "test1"));
  BOOST_CHECK_EQUAL(e.getComponent(engine::getComponentID("test0")).getData<int>(), 14);
  BOOST_CHECK_EQUAL(e.getComponent(engine::getComponentID("test1")).getData<int>(), 15);
}

BOOST_AUTO_TEST_CASE(signature)
{
  engine::Entity e(engine::getEntitySig("test0"));
  BOOST_CHECK_EQUAL(e.sig, engine::getEntitySig("test0"));
}

BOOST_AUTO_TEST_CASE(signature_add)
{
  engine::Entity e(0);
  e.addSig(engine::getEntitySig("test0"));
  BOOST_CHECK_EQUAL(e.sig, engine::getEntitySig("test0"));
  e.addSig(engine::getEntitySig("test1"));
  BOOST_CHECK_EQUAL(e.sig, engine::getEntitySig("test1") | engine::getEntitySig("test0"));
}

BOOST_AUTO_TEST_CASE(signature_remove)
{
  engine::Entity e(engine::getEntitySig("test0") | engine::getEntitySig("test1"));
  e.removeSig(engine::getEntitySig("test0"));
  BOOST_CHECK_EQUAL(e.sig, engine::getEntitySig("test1"));
}

BOOST_AUTO_TEST_CASE(signature_check)
{
  engine::Entity e(engine::getEntitySig("test0") | engine::getEntitySig("test1"));
  BOOST_CHECK(e.checkSig(engine::getEntitySig("test0")));
  BOOST_CHECK(e.checkSig(engine::getEntitySig("test1")));
  BOOST_CHECK(e.checkSig(engine::getEntitySig("test0") | engine::getEntitySig("test1")));
  BOOST_CHECK(!(e.checkSig(engine::getEntitySig("test2"))));
  BOOST_CHECK(!(e.checkSig(engine::getEntitySig("test2") | engine::getEntitySig("test0"))));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(EngineManager)

BOOST_AUTO_TEST_CASE(constructors)
{
  BOOST_CHECK_NO_THROW(engine::EngineManager e);
}

BOOST_AUTO_TEST_CASE(add_entity)
{
  engine::EngineManager e;
  BOOST_CHECK_NO_THROW(e.addEntity(engine::Entity(0)));
}

BOOST_AUTO_TEST_CASE(remove_entity)
{
  engine::EngineManager e;
  engine::Entity entity0(0);
  e.addEntity(entity0);
  e.addEntity(engine::Entity(0,"test0"));
  BOOST_CHECK_NO_THROW(e.removeEntity(entity0.id));
  BOOST_CHECK_NO_THROW(e.removeEntity("test0"));
}

class TestSystem: public engine::System
{
public:

  TestSystem()
  {
    filter = engine::getEntitySig("test0") | engine::getEntitySig("test1");
  }

  ~TestSystem()
  {
  }

  void runSystem(std::vector<engine::Entity>& e)
  {
    for(int i=0; i<e.size(); i++)
      {
	e[i].getComponent("foo").getData<int>()++;
      }
  }
};

BOOST_AUTO_TEST_CASE(add_system)
{
  engine::EngineManager e;
  TestSystem* sys = new TestSystem;
  BOOST_CHECK_NO_THROW(e.addSystem(sys));
  delete sys;
}

BOOST_AUTO_TEST_CASE(update)
{
  engine::EngineManager e;
  TestSystem* sys = new TestSystem;
  e.addSystem(sys);
  engine::Entity e0(engine::getEntitySig("test0") | engine::getEntitySig("test1"), "e0");
  engine::Entity e1(engine::getEntitySig("test0") | engine::getEntitySig("test1") | engine::getEntitySig("test2"), "e1");
  engine::Entity e2(engine::getEntitySig("test1"), "e2");
  engine::Entity e3(engine::getEntitySig("test2"), "e3");
  e0.addComponent(engine::Component(new engine::Property<int>(0), "foo"));
  e1.addComponent(engine::Component(new engine::Property<int>(0), "foo"));
  e2.addComponent(engine::Component(new engine::Property<int>(0), "foo"));
  e3.addComponent(engine::Component(new engine::Property<int>(0), "foo"));
  e.addEntity(e0);
  e.addEntity(e1);
  e.addEntity(e2);
  e.addEntity(e3);
  e.update();
  BOOST_CHECK_EQUAL(e0.getComponent("foo").getData<int>(), 1);
  BOOST_CHECK_EQUAL(e1.getComponent("foo").getData<int>(), 1);
  BOOST_CHECK_EQUAL(e2.getComponent("foo").getData<int>(), 0);
  BOOST_CHECK_EQUAL(e3.getComponent("foo").getData<int>(), 0);
  delete sys;
}
  
BOOST_AUTO_TEST_SUITE_END()
