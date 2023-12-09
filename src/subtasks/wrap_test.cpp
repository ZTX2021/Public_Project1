#include <gtest/gtest.h>
#include <json/json.h>

const Json::Value el0 = Json::Value("0");
const Json::Value el1 = Json::Value(1);

TEST(Wrap, withConstChar) {
  Json::Value arr = Json::arrayValue;
  arr.append(el0);
  arr.append(el1);
  Json::Value obj = "data" / arr;
  ASSERT_EQ(obj["data"][0], el0);
  ASSERT_EQ(obj["data"][1], el1);
}

TEST(Wrap, withString) {
  Json::Value arr = Json::arrayValue;
  arr.append(el0);
  arr.append(el1);
  Json::Value obj = std::string("data") / arr;
  ASSERT_EQ(obj["data"][0], el0);
  ASSERT_EQ(obj["data"][1], el1);
}
