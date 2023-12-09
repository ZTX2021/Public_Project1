#ifndef SUBTASK_XPATH_H
#define SUBTASK_XPATH_H

#include <json/json.h>
#include <vector>

std::vector<Json::Value> search_by_xpath(const Json::Value& value,
                                         const std::string& path);

#endif
