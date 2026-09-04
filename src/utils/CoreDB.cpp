#include "CoreDB.hpp"
#include <sstream>
#include <stdlib.h>
#include <cstdio>
#include <algorithm>
db database(4);

Node *CreateNode(std::string key, std::string val)
{
  Node *x = new Node;
  x->next = NULL;
  x->value = val;
  x->key = key;
  return x;
};

static bool Overload_check(HashTable *current){
  return current->hold >= current->CAP;
}

void resize_hash_init(){
  if(Overload_check(&database.htx[0]) && database.isMigrating == false){
    printf("MIGRATE!\n");
    //kalau overload mulai hash migrate
    database.isMigrating = true;
    size_t newCap = database.latestCAP * 2;
    database.latestCAP = newCap;
    database.LastMigrateIndex = 0; //init
    HashTable ht2(newCap);
    database.htx.push_back(ht2);
  }
  return;
}
//relink harus return head ke hash table lama
Node *relinkNode(Node **newChain, Node *x){
  //selalu head
  Node *ReturnNode = x->next;
  x->next = (*newChain);
  (*newChain) = x;
  return ReturnNode;
}

void rehash_one(){
  Node *curr = database.htx[0].table[database.LastMigrateIndex];
  database.htx[0].table[database.LastMigrateIndex] = nullptr;
  while(curr!=nullptr){
    size_t new_hash_key = hash_func(curr->key, database.latestCAP);
    curr = relinkNode(&(database.htx[1].table[new_hash_key]), curr);
  }
  database.LastMigrateIndex++;
  if(database.LastMigrateIndex >= database.htx[0].CAP){
    //migrasi selesai
    database.htx.erase(database.htx.begin());
    database.isMigrating = false;
    database.LastMigrateIndex = -1;
  }
}

// change into push -> Head
Node *pushNode(Node **chain, std::string key, std::string val)
{
  Node *newNode = CreateNode(key, val);
  if (!(*chain))
  {
    return newNode;
  }
  newNode->next = (*chain);
  return newNode;
};

size_t hash_func(const std::string &key, size_t CAP)
{
  return std::hash<std::string>{}(key) % CAP;
};

Node *Search_hash(const std::string &key)
{
  if(database.isMigrating) rehash_one();
  int check = 0; //0 -> old ht, 1-> new ht
  while(check < ((database.isMigrating) ? 2 : 1)){
    HashTable &x = database.htx[check];
    size_t hash_key = hash_func(key, x.CAP);
    if(x.table[hash_key] != nullptr){
      //oke ada item coba cek dulu chain nya ada gak yang kita cari
      Node *curr = x.table[hash_key];
      while(curr && curr->key != key){
        curr = curr->next;
      }
      if(!curr && check == 0){
        //coba pindah ke table sebelah
        check = 1;
        continue;
      }else if(!curr && check == 1){
        break; // udah di table kedua dan gaada
      }else{
        return curr;
        break;
      }
    }else if(x.table[hash_key] == nullptr && check == 0){
      //pindah ke table kedua coba
      check = 1;
      continue;
    }else{
      //udah di table kedua dan kosong
      break;
    }
  }
  return nullptr;
}

// 0 -> error
// 1 -> set
// 2 -> update
// 3 -> still
int Insert(const std::string &key, const std::string &val)
{
  if(database.isMigrating) rehash_one();
  // kalau migrating -> ada ht 2 langsung isi disana otherwise isi ht1
  size_t hash_key = hash_func(
    key, 
    (database.isMigrating) ? database.htx[1].CAP : database.htx[0].CAP
  );

  Node *check = Search_hash(key); //Search_hash(key);
  if (check)
  {
    // misal udah ada nodenya
    if (check->value != val)
    {
      // misal key ada tapi value beda sama command, jadinya mau di update
      check->value = val;
      return 2;
    }
    else
    {
      return 3;
    }
  }
  else
  {
    int mode = database.isMigrating ? 1 : 0;
    database.htx[mode].table[hash_key] = pushNode(&database.htx[mode].table[hash_key], key, val);
    database.htx[mode].hold++;
    if(mode == 0){
      resize_hash_init();
    }
    return 1;
  }
  return 0;
}

bool Delete(const std::string &key)
{
  if(database.isMigrating) rehash_one();
  int check = 0; //0 -> old ht, 1-> new ht
  while(check < ((database.isMigrating) ? 2 : 1)){
    HashTable &x = database.htx[check];
    size_t hash_key = hash_func(key, x.CAP);
    if(x.table[hash_key] != nullptr){
      //oke ada item coba cek dulu chain nya ada gak yang kita cari
      Node *curr = x.table[hash_key];
      Node *prev = nullptr;
      while(curr && curr->key != key){
        prev = curr;
        curr = curr->next;
      }
      if(!curr && check == 0){
        //coba pindah ke table sebelah
        check = 1;
        continue;
      }else if(!curr && check == 1){
        return false; // udah di table kedua dan gaada
      }else{
        //delete
        if(prev == nullptr){
          x.table[hash_key] = curr->next;
        }else{
          prev->next = curr->next;
        }
        delete curr;
        database.htx[check].hold--;
        return true;
      }
    }else if(x.table[hash_key] == nullptr && check == 0){
      //pindah ke table kedua coba
      check = 1;
      continue;
    }else{
      //udah di table kedua dan kosong
      return false;
    }
  }
  return false;
}

// literally ambil -> set nama arco -> ['set', 'nama', 'arco'] as Vector string
std::vector<std::string> cmd_parse(const std::string &req)
{
  std::vector<std::string> tokens;
  std::stringstream ss(req);
  std::string word;
  while (ss >> word)
  {
    tokens.push_back(word);
  }
  return tokens;
}

std::string cmd_exec(const std::vector<std::string> &parsed_cmd)
{
  if (parsed_cmd.empty())
    return "-ERR empty command\r\n"; // RESP Error

  std::string db_operand = parsed_cmd[0];
  // Convert command ke lowercase case-insensitive (SET, set, Set bakal valid semua)
  std::transform(db_operand.begin(), db_operand.end(), db_operand.begin(), ::tolower);

  if (db_operand == "set" && (parsed_cmd.size() == 3 || parsed_cmd.size() == 5)) 
  {
    if(parsed_cmd[2] == "NIL" || parsed_cmd[2] == "ERR") return "-ERR reserved keyword\r\n"; 
    
    int q = Insert(parsed_cmd[1], parsed_cmd[2]);
    if (q == 1 || q == 2 || q == 3)
    {
      return "+OK\r\n"; // RESP Simple String (sukses)
    }
    else
    {
      return "-ERR internal error\r\n"; // RESP Error
    }
  }
  else if (db_operand == "get" && parsed_cmd.size() == 2)
  {
    Node *q = Search_hash(parsed_cmd[1]);
    if (q)
    {
      // RESP Bulk String: $<panjang>\r\n<teks>\r\n
      return "$" + std::to_string(q->value.size()) + "\r\n" + q->value + "\r\n";
    }
    else
    {
      // RESP Null Bulk String 
      return "$-1\r\n";
    }
  }
  else if (db_operand == "del" && parsed_cmd.size() == 2)
  {
    bool del_query = Delete(parsed_cmd[1]);
    if (del_query) {
        // RESP Integer: :<angka>\r\n 
        return ":1\r\n"; 
    } else {
        return ":0\r\n";
    }
  }
  else
  {
    return "-ERR unknown command '" + parsed_cmd[0] + "'\r\n";
  }
}