#include <iostream>
#include <optional>
#include "store.hpp"

int main(void){
    Store db;

    int queryReq;
    std::cout << "how many set query? : ";
    std::cin >> queryReq;
    for(int i = 0; i < queryReq; i++){
        std::string k, v;
        std::cin >> k >> v;
        db.set(k,v);
    }
 
    bool loop = true;
    while(loop){
        std::string searchKey;
        std::cout << "Query data per key :";
        std::cin >> searchKey;
        std::optional<std::string> getQuery = db.get(searchKey);
        if(getQuery.has_value()){
            std::cout << "key:" << searchKey << "val:" << getQuery.value() << "\n";
        }else{
            std::cout << "Key not found!\n";
        }

        std::string looper;
        std::cout << "search another key ? (y/n) : ";
        std::cin >> looper;
        if(looper == "y"){
            continue;
        }else{
            loop = false;
            break;
        }
    }

    return 0;
}