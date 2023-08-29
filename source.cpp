#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>


// void insert(std::string command) {

// }

void display(std::string file_name) {
    std::fstream file(file_name, std::ios::in);

}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout<<"Error"<<std::endl;
        return 1;
    }
    
    std::string statement = argv[1];

    size_t pos = statement.find("INSERT INTO ");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    } 

    statement = statement.substr(pos+12);
    
    pos = statement.find("(");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    }

    std::string file_name = statement.substr(0, pos);
    statement = statement.substr(pos+1);

    pos = statement.find(")");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    }

    std::string table_name = statement.substr(0, pos);
    statement = statement.substr(pos+1);

    pos = statement.find("VALUES ");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    }

    statement = statement.substr(pos+7);

    std::cout<<statement<<std::endl;

    pos = statement.find("(");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    }

    std::string file_name = statement.substr(0, pos);
    statement = statement.substr(pos+1);

    pos = statement.find(")");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    }
    return 0;
}
