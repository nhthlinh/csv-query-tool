#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

enum TokenType {ACTION, INTO, FROM, VALUES, ID, LPAREN, RPAREN, COMMA, ERROR};

struct Token {
    TokenType type;
    string value;
};

vector<Token> extract(string input) {
    vector<Token> tokens;

    /*
    \\b: ký tự thoát
    (INSERT|SELECT|INTO|FROM|VALUES|\\w+): tìm kiếm các từ INSERT SELECT INTO FROM VALUES  hoặc bất kỳ từ nào chứa ký tự word
    |: toán tử OR
    \\(|\\)
    */ 

    regex tokenRegex("\\b(INSERT|SELECT)\\sINTO\\.csv$\\s([\\w\\.]+)\\sVALUES\\s\\(([^)]+)\\|FROM\\s([\\w\\.]+)|([A-Za-z_][A-Za-z_0-9]*)|^\\d+(,\\d+)*$");
    smatch match;

    string::const_iterator start = input.cbegin();
    string::const_iterator end = input.cend();

    while (regex_search(start, end, match, tokenRegex)) {

        for (int i=1; i<match.size(); ++i) {
            if (!match[i].str().empty()) {
                Token token;
                token.value = match[i].str();

                if (token.value == "INSERT" || token.value == "SELECT") token.type = ACTION;
                else if (token.value == "INTO") token.type = INTO;
                else if (token.value == "FROM") token.type = FROM;
                else if (token.value == "VALUES") token.type = VALUES;
                else token.type = ID;
                else {
                switch (token.value[0]) {
                    case '(' : token.type = LPAREN; break;
                    case ')' : token.type = RPAREN; break;
                    case ',' : token.type = COMMA; break;
                }

                tokens.push_back(token);
                cout<<token.value<<" "<<to_string(token.type)<<endl;
                start = match[i].second;
                
                break;
            } 
        }
    }
    return tokens;
}

void display(string fname) {
    vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	ifstream file(fname);
    getline(file, line);
	if(file.is_open())
	{
		while(getline(file, line))
		{
            stringstream str(line);
 
			while(getline(str, word, ',')) row.push_back(word);

            content.push_back(row);

            row.clear();
		}
    }
	else cout<<"Could not open the file\n";
 
	for(int i=0;i<content.size();i++)
	{
		for(int j=0;j<content[i].size();j++)
		{
			cout<<content[i][j]<<" ";
		}
		cout<<"\n";
	}

    file.close();
}

// class Expr {
//     virtual string toString() const = 0;
// };

// class RelationalExpr : public Expr {
// public:
//     string colName;
//     short int operator;
//     string value;
// public:
//     string toString() const {
//         // TODO
//     }
// };

// class LogicalExpr : public Expr {
// public:
//     RelationalExpr* expr1;
//     short int operator;
//     RelationalExpr* expr2;
// public:
//     string toString() const {
//         // TODO
//     }
// };

// class SortBy {
// public:
//     string columnName;
//     short int order;
// public:
//     string toString() const {
//         // TODO
//     }
// };

// class Command {
// public:
//     virtual string toString() const = 0;
// };

// class InsertCommand : public Command {
// public:
//     string filename;
//     vector<string> columnNames;
//     vector<string> values;
// public:
//     string toString() const {
//         // TODO
//     }
// };

// // class SelectCommand : public Command {
// public:
//     vector<string> columnNames;
//     void* dataSource;
//     Expr* expr;
//     SortBy* sort;

// public:
//     string toString() const {
//         // TODO
//     }
// };

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout<<"Error"<<std::endl;
        return 1;
    }
    
    string statement = argv[1];
    string file_name = "";

    vector<Token> tokens = extract("SELECT id, name, email, phone FROM ( SELECT id, name, email, age, phone FROM member.csv WHERE age > 2 ) WHERE name LIKE '%Vinh%'");
    // for (Token token:tokens) {
    //     cout<< token.type << " " <<token.value <<"\n";
    // }

    //display(file_name);

    return 0;
}
