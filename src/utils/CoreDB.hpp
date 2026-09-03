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
    size_t hold = 0;
    std::vector<Node*> table;
    HashTable(size_t cap) : CAP(cap), table(cap, nullptr) {};
} Hashtable;

typedef struct db {
    size_t latestCAP;
    std::vector<HashTable> htx;
    bool isMigrating = false;
    int LastMigrateIndex = -1;
    db(size_t f_cap) : latestCAP(f_cap), htx(1, latestCAP){};
} db;

Node *CreateNode(std::string key, std::string val);
Node *pushNode(Node **chain, std::string key, std::string val);
Node *relinkNode(Node **newChain, Node *x);
size_t hash_func(const std::string &key, size_t CAP);
int Insert(const std::string &key, const std::string &val);
Node *Search_hash(const std::string &key);

void InitHash();
void resize_hash_init();
void rehash_one();

extern db database;

//placeholder map for storing data in memory
//static std::unordered_map<std::string, std::string> db;
//literally ambil -> set nama arco -> ['set', 'nama', 'arco'] as Vector string
std::vector<std::string> cmd_parse(const std::string &req);
std::string cmd_exec(const std::vector<std::string> &parsed_cmd);

#endif