#include <string>
#include <optional>
#include <unordered_map>


class Store {
    private :
        std::unordered_map<std::string, std::string> data;
    public :
        void set(std::string k, std::string v){
            //set k-v di memory
            data[k] = v;
        }
        std::optional<std::string> get(std::string k){
            //get k-v di memory
            auto query = data.find(k);
            if(query != data.end()){
                return query->second;
            }else{
                return std::nullopt;
            }
        }
        bool del(std::string k){
            //del k-v di memory
            size_t deletion = data.erase(k);
            if(deletion == 1){
                return true;
            }else{
                return false;
            }
        }
};