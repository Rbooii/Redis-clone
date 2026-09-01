#include "CoreDB.hpp"
#include <sstream>
#include <stdlib.h>
HashTable db_hash(8);

Node *CreateNode(std::string key, std::string val){
    Node *x = new Node;
    x->next = NULL;
    x->value = val;
    x->key = key;
    return x;
};

void pushNode(Node **chain, std::string key, std::string val){
    Node *newNode = CreateNode(key, val);
    if(!(*chain))
    {
        (*chain) = newNode;
        return;
    }
    Node *current = (*chain); 
    while(current->next){
        current = current->next;
    }
    current->next = newNode;
    return;
};

void InitHash(){}

size_t hash_func(const std::string &key, size_t CAP){
    return std::hash<std::string>{}(key) % CAP;
};

void Insert(const std::string &key, const std::string &val){
    size_t hash_key = hash_func(key, db_hash.CAP);
    pushNode(&db_hash.table[hash_key], key, val);
}

Node *Search_hash(const std::string &key) {
    size_t hash_key = hash_func(key, db_hash.CAP);
    if(db_hash.table.at(hash_key) == nullptr) return NULL;
    int idx = 0;
    Node *curr = db_hash.table[hash_key];
    while(curr && curr->key != key){
        curr = curr->next;
        idx++;
    }
    if(!curr){
        return NULL;
    }else{
        return curr;
    }
    return NULL;
}

bool Delete(const std::string &key){
    Node *x = Search_hash(key);
    if(!x){ 
        return false;
    }

    
}

//literally ambil -> set nama arco -> ['set', 'nama', 'arco'] as Vector string
std::vector<std::string> cmd_parse(const std::string &req){
    std::vector<std::string> tokens;
    std::stringstream ss(req);
    std::string word;
    while(ss>>word){
        tokens.push_back(word);
    }
    return tokens;
}

std::string cmd_exec(const std::vector<std::string> &parsed_cmd){
    if(parsed_cmd.empty()) return "EMPTY";
    const std::string &db_operand = parsed_cmd[0]; //get,set,del 
    if(db_operand == "set" && parsed_cmd.size() == 3){
        Insert(parsed_cmd[1], parsed_cmd[2]);        
        return "OK SET ";
    }else if(db_operand == "get" && parsed_cmd.size() == 2){
        Node *q = Search_hash(parsed_cmd[1]);
        if(q){
            return q->value;
        }else{
            return "NIL";
        }
    }else if(db_operand == "del" && parsed_cmd.size() == 2){
        // bool delQ = db.erase(parsed_cmd[1]);
        // return (delQ) ? "1" : "0";
    }else{
        return "ERR";
    }
}