#include "xpath.h"
#include <gtest/gtest.h>

TEST(XPath, objectKey) {
  Json::Value obj = Json::objectValue;
  Json::Value val = Json::Value("hello");
  obj["data"] = val;
  Json::Value obj2 = Json::objectValue;
  obj2["data2"] = obj;
  auto vec = search_by_xpath(obj2, "data2");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], obj);
  vec = search_by_xpath(obj2, "data2/data");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], val);
  vec = search_by_xpath(obj2, "data2/data2");
  ASSERT_EQ(vec.size(), 0);
}

TEST(XPath, arrayKey) {
  Json::Value arr = Json::arrayValue;
  Json::Value val = Json::Value("hello");
  Json::Value val2 = Json::Value(1);
  arr.append(val);
  arr.append(val2);
  Json::Value obj = Json::objectValue;
  obj["data"] = arr;
  auto vec = search_by_xpath(obj, "data");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], arr);
  vec = search_by_xpath(obj, "data/0");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], val);
  vec = search_by_xpath(obj, "data/1");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], val2);
  vec = search_by_xpath(obj, "data/2");
  ASSERT_EQ(vec.size(), 0);
}

TEST(XPath, parent) {
  Json::Value obj = Json::objectValue;
  Json::Value val = Json::Value("hello");
  obj["data"] = val;
  auto vec = search_by_xpath(obj, "data/..");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], obj);
}

TEST(XPath, allChild) {
  Json::Value obj = Json::objectValue;
  Json::Value obj2 = Json::objectValue;
  Json::Value val = Json::Value("hello");
  obj["data"] = val;
  obj2["data2"] = obj;
  auto vec = search_by_xpath(obj2, "data2/*");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], val);
  vec = search_by_xpath(obj2, "data/*");
  ASSERT_EQ(vec.size(), 0);
}

TEST(XPath, filterEffect) {
  Json::Value obj = Json::objectValue;
  Json::Value val = Json::Value("hello");
  obj["data"] = val;
  obj["int"] = Json::Value(1);
  Json::Value obj2 = Json::objectValue;
  obj2["data2"] = val;
  obj2["int"] = Json::Value(2);
  Json::Value arr = Json::arrayValue;
  arr.append(obj);
  arr.append(obj2);
  auto vec = search_by_xpath(arr, "*/data2/../int");
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], Json::Value(2));
}
