#include <gtest/gtest.h>
#include <json/json.h>

TEST(Cast, int) {
  int val = 1;
  Json::Value int_val = val;
  ASSERT_EQ(val, static_cast<int>(int_val));
}

TEST(Cast, double) {
  double val = 3.14;
  Json::Value double_val = 3.14;
  ASSERT_EQ(val, static_cast<double>(double_val));
}

TEST(Cast, string) {
  std::string val = "hello";
  Json::Value string_val = val;
  ASSERT_EQ(val, static_cast<std::string>(string_val));
}

TEST(Cast, array) {
  Json::Value array_val = Json::arrayValue;
  Json::Value el0 = 1;
  Json::Value el1 = "hello";
  Json::Value el2;
  array_val.append(el0);
  array_val.append(el1);
  array_val.append(el2);
  auto arr = static_cast<std::vector<Json::Value>>(array_val);
  ASSERT_EQ(arr.size(), 3);
  ASSERT_EQ(arr[0], el0);
  ASSERT_EQ(arr[1], el1);
  ASSERT_EQ(arr[2], el2);
}

TEST(Cast, object) {
  Json::Value object_val = Json::objectValue;
  Json::Value el0 = 1;
  Json::Value el1 = "hello";
  Json::Value el2;
  object_val["el0"] = el0;
  object_val["el1"] = el1;
  object_val["el2"] = el2;
  auto obj = static_cast<std::map<std::string, Json::Value>>(object_val);
  ASSERT_EQ(obj.size(), 3);
  ASSERT_EQ(obj["el0"], el0);
  ASSERT_EQ(obj["el1"], el1);
  ASSERT_EQ(obj["el2"], el2);
}
