#include "json/json.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Concat, withSpaces) {
  std::istringstream iss(R"({"name" +"1": "San"+ "Zhang"})");
  Json::CharReaderBuilder reader;
  Json::Value v;
  std::string err;
  ASSERT_TRUE(Json::parseFromStream(reader, iss, v, err));
  ASSERT_EQ(v["name1"], "SanZhang");
}
