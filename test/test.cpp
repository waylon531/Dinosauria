#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Suites
#include <boost/test/unit_test.hpp>

#include "engine/engine.hpp"

BOOST_AUTO_TEST_SUITE(Component)

BOOST_AUTO_TEST_CASE(constructors)
{
  engine::Component c(new engine::Property<int>(14), "test0");
}

BOOST_AUTO_TEST_CASE(component_id)
{
  engine::Component c(new engine::Property<int>(14), "test0");
  int i = engine::getComponentID("test0");
  engine::Component c2(new engine::Property<int>(14), "test1");
  BOOST_REQUIRE(c.ID == i);
  BOOST_REQUIRE(c.ID != c2.ID);
}

BOOST_AUTO_TEST_CASE(data_access)
{
  engine::Component c(new engine::Property<int>(14), "test0");
  BOOST_REQUIRE(c.getData<int>() == 14);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Entity)

BOOST_AUTO_TEST_CASE(constructors)
{
  engine::Entity e("test0");
}

BOOST_AUTO_TEST_CASE(add_component)
{
  engine::Entity e("test0");
  e.addComponent(engine::Component(new engine::Property<int>(14), "test0"));
}

BOOST_AUTO_TEST_CASE(get_component)
{
  engine::Entity e("test0");
  e.addComponent(engine::Component(new engine::Property<int>(14), "test0"));
  e.addComponent(engine::Component(new engine::Property<int>(15), "test1"));
  BOOST_REQUIRE(e.getComponent("test0").getData<int>() == 14);
  BOOST_REQUIRE(e.getComponent("test1").getData<int>() == 15);
}

BOOST_AUTO_TEST_SUITE_END()
