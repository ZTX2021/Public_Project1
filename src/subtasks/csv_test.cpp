#include "csv.h"
#include <algorithm>
#include <cctype>
#include <gtest/gtest.h>
#include <sstream>

void remove_spaces(std::string& str) {
  str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}

TEST(CSV, toCSV) {
  // [[0, 1], [2, 3]]
  Json::Value row0(Json::arrayValue);
  row0.append(Json::Value("0"));
  row0.append(Json::Value("1"));
  Json::Value row1(Json::arrayValue);
  row1.append(Json::Value("2"));
  row1.append(Json::Value("3"));
  Json::Value arr(Json::arrayValue);
  arr.append(row0);
  arr.append(row1);
  std::istringstream iss(json_to_csv(arr));
  std::string line;
  std::getline(iss, line);
  remove_spaces(line);
  ASSERT_EQ(line, "0,1");
  std::getline(iss, line);
  remove_spaces(line);
  ASSERT_EQ(line, "2,3");
}

TEST(CSV, fromCSV) {
  Json::Value arr = csv_to_json(std::string("0,1\nnull,3\n"));
  ASSERT_EQ(arr.size(), 2);
  ASSERT_EQ(arr[0].size(), 2);
  ASSERT_EQ(arr[0][0], Json::Value("0"));
  ASSERT_EQ(arr[0][1], Json::Value("1"));
  ASSERT_EQ(arr[1].size(), 2);
  ASSERT_EQ(arr[1][0], Json::Value("null"));
  ASSERT_EQ(arr[1][1], Json::Value("3"));
}
