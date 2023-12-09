#include "json/json.h"
#include <iostream>
/** \brief Write a Value object to a string.
 * Example Usage:
 * $g++ stringWrite.cpp -ljsoncpp -std=c++11 -o stringWrite
 * $./stringWrite
 * {
 *     "action" : "run",
 *     "data" :
 *     {
 *         "number" : 1
 *     }
 * }
 */
int main() {
  Json::Value root;
  Json::Value data;
  root["action"] = "run";
  data["number"] = 1;
  root["data"] = data;

  Json::StreamWriterBuilder builder;
  const std::string json_file = Json::writeString(builder, root);
  std::cout << json_file << std::endl;
  return EXIT_SUCCESS;
}
