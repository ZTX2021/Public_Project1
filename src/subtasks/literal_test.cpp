#include <gtest/gtest.h>
#include <json/json.h>

TEST(Literal, simple) {
  Json::Value t = R"({"int":0, "array":[1, null]})"_json;
  ASSERT_EQ(t["int"], 0);
  ASSERT_EQ(t["array"][0], 1);
  ASSERT_EQ(t["array"][1], Json::Value(Json::nullValue));
}
