#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void insert(std::string statement, std::string &file_name) {
    size_t pos = statement.find("(");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return;
    }

    file_name = statement.substr(0, pos-1);
    statement = statement.substr(pos+1);

    pos = statement.find(")");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return;
    }

    std::string table_name = statement.substr(0, pos);
    statement = statement.substr(pos+1);

    pos = statement.find("VALUES ");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return;
    }

    statement = statement.substr(pos+7);

    pos = statement.find(")");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return;
    }

    std::string value_name = statement.substr(1, pos-1);

    //std::cout<<table_name<<file_name<<value_name<<std::endl;
}

void display(std::string fname) {
    std::vector<std::vector<std::string>> content;
    std::vector<std::string> row;
    std::string line, word;
 
    std::ifstream file(fname);
    getline(file, line);
    if(file.is_open())
    {
	while(getline(file, line))
		{
			std::stringstream str(line);
 
			while(getline(str, word, ',')) row.push_back(word);

            content.push_back(row);

            row.clear();
		}
    }
    else std::cout<<"Could not open the file\n";
 
    for(int i=0;i<content.size();i++)
	{
	    for(int j=0;j<content[i].size();j++)
	    {
		std::cout<<content[i][j]<<" ";
	    }
	    std::cout<<"\n";
	}

    file.close();
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout<<"Error"<<std::endl;
        return 1;
    }
    
    std::string statement = argv[1];
    std::string file_name = "";

    size_t pos = statement.find("INSERT INTO ");
    if (pos == std::string::npos) {
        std::cout<<"Wrong command!"<<std::endl;
        return 1;
    } 
    else {
        statement = statement.substr(pos+12);
        insert(statement, file_name);
    }
    
    display(file_name);

    return 0;
}
