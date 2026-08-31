#ifndef COREDB_H
#define COREDB_H

#include <vector>
#include <string>
#include <unordered_map>

//placeholder map for storing data in memory
static std::unordered_map<std::string, std::string> db;
//literally ambil -> set nama arco -> ['set', 'nama', 'arco'] as Vector string
std::vector<std::string> cmd_parse(const std::string &req);
std::string cmd_exec(const std::vector<std::string> &parsed_cmd);

#endif