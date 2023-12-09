#ifndef SUBTASK_BINARY_H
#define SUBTASK_BINARY_H
#include <json/json.h>
#include <vector>

Json::Value binary_to_json(const void* data, std::size_t len);

std::vector<std::uint8_t> json_to_binary(const Json::Value& value);

#endif
