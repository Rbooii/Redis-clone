#ifndef COREDB_H
#define COREDB_H

#include <vector>
#include <string>
#include <unordered_map>

typedef struct Node {
    std::string key;
    std::string value;
    struct Node *next;
} Node;

typedef struct HashTable {
    size_t CAP;
    std::vector<Node*> table;
    HashTable(size_t cap) : CAP(cap), table(cap, nullptr) {};
} Hashtable;

Node *CreateNode(std::string key, std::string val);
Node *pushNode(Node **chain, std::string key, std::string val);
void InitHash();
size_t hash_func(const std::string &key, size_t CAP);
int Insert(std::string &key, std::string &val);
std::string Search_hash(std::string &key);


extern HashTable db_hash;

//placeholder map for storing data in memory
//static std::unordered_map<std::string, std::string> db;
//literally ambil -> set nama arco -> ['set', 'nama', 'arco'] as Vector string
std::vector<std::string> cmd_parse(const std::string &req);
std::string cmd_exec(const std::vector<std::string> &parsed_cmd);

#endif