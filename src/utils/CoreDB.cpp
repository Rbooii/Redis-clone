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

// change into push -> Head
Node *pushNode(Node **chain, std::string key, std::string val){
    Node *newNode = CreateNode(key, val);
    if(!(*chain))
    {
        return newNode;
    }
    newNode->next = (*chain);
    return newNode;
};

void InitHash(){}

size_t hash_func(const std::string &key, size_t CAP){
    return std::hash<std::string>{}(key) % CAP;
};

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

// 0 -> error
// 1 -> set
// 2 -> update
// 3 -> still
int Insert(const std::string &key, const std::string &val){
    size_t hash_key = hash_func(key, db_hash.CAP);
    
    Node *check = Search_hash(key);
    if(check){
        //misal udah ada nodenya
        if(check->value != val){
            //misal key ada tapi value beda sama command, jadinya mau di update
            check->value = val;
            return 2;
        }else{
            return 3;
        }
    }else{
        db_hash.table[hash_key] = pushNode(&db_hash.table[hash_key], key, val); 
        return 1;
    }
    return 0; 
}



bool Delete(const std::string &key){
    size_t h_k = hash_func(key, db_hash.CAP);
    if(!db_hash.table[h_k]) return false;
    Node *curr = db_hash.table[h_k];
    Node *prev = nullptr;
    while(curr && curr->key != key){
        prev = curr;
        curr = curr->next;
    }
    if(!curr) return false;

    //if head
    if(prev == nullptr){
        db_hash.table[h_k] = curr->next;
    }else{
        prev->next = curr->next;
    }
    
    delete curr;
    return true;
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
        int q = Insert(parsed_cmd[1], parsed_cmd[2]);        
        if(q == 1){return "OK SET";}
        else if(q==2){return "OK UPDATE";}
        else if(q==3){return "OK STILL";}
        else return "ERR";
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

        bool del_query = Delete(parsed_cmd[1]);
        return del_query ? "OK DELETE" : "NIL";
    }else{
        return "ERR";
    }
}