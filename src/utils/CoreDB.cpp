#include "CoreDB.hpp"
#include <sstream>

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
        db[parsed_cmd[1]] = parsed_cmd[2];
        return "OK SET ";
    }else if(db_operand == "get" && parsed_cmd.size() == 2){
        auto q = db.find(parsed_cmd[1]);
        if(q != db.end()) return q->second;
        return "NULL";
    }else if(db_operand == "del" && parsed_cmd.size() == 2){
        bool delQ = db.erase(parsed_cmd[1]);
        return (delQ) ? "1" : "0";
    }else{
        return "ERR";
    }
}