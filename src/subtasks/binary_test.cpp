#include "binary.h"
#include <cctype>
#include <gtest/gtest.h>
#include <locale>
#include <string>

std::vector<std::uint8_t> vec = {0x01, 0x02, 0x03, 0x00};
const void* data = static_cast<const void*>(vec.data());
const std::size_t len = vec.size();

TEST(Binary, equal) {
  auto v1 = binary_to_json(data, len);
  auto vec2 = vec;
  auto v2 = binary_to_json(static_cast<const void*>(vec2.data()), len);
  ASSERT_EQ(v1, v2);
}

TEST(Binary, lengthNotEqual) {
  auto v1 = binary_to_json(data, len);
  auto v2 = binary_to_json(data, len - 1);
  ASSERT_NE(v1, v2);
}

TEST(Binary, dataNotEqual) {
  auto v1 = binary_to_json(data, len);
  auto old = vec[0];
  vec[0] = 0xff;
  auto v2 = binary_to_json(data, len);
  vec[0] = old;
  ASSERT_NE(v1, v2);
}

bool is_printable(std::string str) {
  for (auto c : str) {
    if (!std::isprint(c, std::locale("C")) && c != '\n' && c != '\r' &&
        c != '\t') {
      return false;
    }
  }
  return true;
}

TEST(Binary, serialize) {
  auto v1 = binary_to_json(data, len);
  Json::StreamWriterBuilder builder;
  std::string str = Json::writeString(builder, v1);
  ASSERT_TRUE(is_printable(str));
  Json::CharReaderBuilder reader;
  std::istringstream iss(str);
  Json::Value v2;
  std::string err;
  ASSERT_TRUE(Json::parseFromStream(reader, iss, v2, err));
  auto vec2 = json_to_binary(v2);
  for (std::size_t i = 0; i < len; ++i) {
    ASSERT_EQ(vec[i], vec2[i]);
  }
}

TEST(Binary, nestedSerialize) {
  auto v1 = binary_to_json(data, len);
  Json::Value v1_wrap(Json::objectValue);
  v1_wrap["data"] = v1;
  Json::StreamWriterBuilder builder;
  std::string str = Json::writeString(builder, v1_wrap);
  ASSERT_TRUE(is_printable(str));
  Json::CharReaderBuilder reader;
  std::istringstream iss(str);
  Json::Value v2;
  std::string err;
  ASSERT_TRUE(Json::parseFromStream(reader, iss, v2, err));
  auto vec2 = json_to_binary(v2["data"]);
  for (std::size_t i = 0; i < len; ++i) {
    ASSERT_EQ(vec[i], vec2[i]);
  }
}
