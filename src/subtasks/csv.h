#ifndef SUBTASK_CSV_H
#define SUBTASK_CSV_H
#include <json/json.h>
#include <string>

// The json object is guaranteed to be an array of arrays of strings:
// [
//   ["a", "b", "c"],
//   ["d", "e", "f"],
// ]

std::string json_to_csv(const Json::Value& value);

Json::Value csv_to_json(const std::string& csv);

#endif
