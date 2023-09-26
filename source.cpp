#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <iomanip>

using namespace std;

string statement;

enum TokenType {ACTION, INTO, FROM, VALUES, WHERE, FILENAME, ID, OPERATION, SORT_BY, ORDER_BY, RELATION, OPEN, CLOSE};

struct Token {
    TokenType type;
    string value;
};

string tokentype_name(Token T) {
    switch (T.type) {
        case 0: return "ACTION";
        case 1: return "INTO";
        case 2: return "FROM";
        case 3: return "VALUES";
        case 4: return "WHERE";
        case 5: return "FILENAME";
        case 6: return "ID";
        case 7: return "OPERATION";
        case 8: return "SORT_BY";
        case 9: return "ORDER_BY";
        case 10: return "RELATION";
        case 11: return "OPEN";
        case 12: return "CLOSE";
    }
    return "";
}

vector<Token> extract(string input) {
    vector<Token> tokens;

    regex tokenRegex(R"(\b(INSERT|SELECT|INTO|FROM|LIKE|VALUES|WHERE|ORDER\sBY|ASC|DESC|[*()]|(\S+\.csv))\b|\\w+\.\\w+|([\"].*?[\"0-9]+|A-Za-z0-9]+)|\b\w+\b|('[^']*')|("%[^%]*%")|[><=*]|\(|\))");
    //regex tokenRegex("INSERT|SELECT|INTO|FROM|LIKE|VALUES|\\w+\.csv|([\"].*?[\"0-9]+|A-Za-z0-9]+)||\\w+");

    sregex_iterator it(input.begin(), input.end(), tokenRegex);
    sregex_iterator end;

    while (it!=end) {
        smatch match = *it;
        string t = match.str();

        if (t.front() == '"' && t.back() == '"') t = t.substr(1, t.length() - 2);

        Token token;
        token.value = t;

        if (token.value == "INSERT" || token.value == "SELECT") token.type = ACTION;
        else if (token.value == "INTO") token.type = INTO;
        else if (token.value == "FROM") token.type = FROM;
        else if (token.value == "VALUES") token.type = VALUES;
        else if (token.value == "WHERE") token.type = WHERE;
        else if (token.value == ">" || token.value == "<" || token.value == "=" || token.value == "LIKE") token.type = OPERATION;
        else if (token.value == "ORDER BY") token.type = ORDER_BY;
        else if (token.value == "ASC" || token.value == "DESC") token.type = SORT_BY;
        else if (token.value == "AND" || token.value == "OR") token.type = RELATION;
        else if (token.value.length() >= 5) {
            if (token.value.substr(token.value.length()-4) == ".csv") token.type = FILENAME;
            else token.type = ID;
        }
        else if (token.value == "(") token.type = OPEN;
        else if (token.value == ")") token.type = CLOSE;
        else token.type = ID;
        tokens.push_back(token);
        ++it;
    }

    return tokens;
}

void display(string fname) {
    vector<vector<string>> content;
	vector<string> row;
	string line, word;
    vector<string> column_name;
 
	ifstream file(fname);

    getline(file, line); 
    stringstream str(line);
    while (getline(str, word, ',')) column_name.push_back(word);

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
 
    for (int i=0; i<column_name.size(); i++) {
        cout<<left<<setw(25)<<column_name[i]<<" ";
    }
    cout<<"\n";
	for (int i=0; i<content.size(); i++)
	{
		for(int j=0;j<content[i].size();j++)
		{
			cout<<left<<setw(25)<<content[i][j]<<" ";
		}
		cout<<"\n";
	}

    file.close();
}

class Expr {
    virtual string toString() const = 0;
};

class RelationalExpr : public Expr {
public:
    string colName;
    short int op;
    string value;
public:
    RelationalExpr(string colName, string op, string value) {
        this->colName = colName;
        this->value = value;
    }
    string toString() const {
        // TODO
        return colName + " " + to_string(op) + value;
    }
};

class LogicalExpr : public Expr {
public:
    RelationalExpr* expr1;
    short int op;
    RelationalExpr* expr2;
public:
    string toString() const {
        // TODO
        return "";
    }
};

class SortBy {
public:
    string columnName;
    short int order;
public:
    string toString() const {
        // TODO
        return "";
    }
};

class Command {
public:
    virtual string toString() const = 0;
    virtual void do_action() = 0;
};

class InsertCommand : public Command {
public:
    string filename;
    vector<string> columnNames;
    vector<string> values;
public:
    InsertCommand(vector<Token> T) { 
        this->filename = "";
        this->columnNames.clear();
        this->values.clear();

        if (T.size() <= 3 || T[0].value != "INSERT" || T[1].type != INTO || T[2].type != FILENAME) this->error();
        else if (T[2].type == FILENAME) {
            this->filename = T[2].value;
            int i = 4, j = 0;
            if (T[3].type == OPEN && T[4].type == ID) {
                while (T[i].type == ID) this->columnNames.push_back(T[i++].value);
                if (T[i].type == CLOSE && T[i+=1].type == VALUES && T[i+=1].type == OPEN) {
                    i+=1;
                    while (T[i].type == ID) this->values.push_back(T[i++].value);
                }
                else this->error();
            }
            else if (T[i].type == VALUES) {
                i += 1;
                if (T[i].type == ID) {
                    while (T[i].type == ID) this->values.push_back(T[i++].value);
                }
                else this->error();
            }
            else this->error();
        }
        if (this->check_condition()) this->do_action();
    }
    void error() {
        cout<<"ERROR!";
        this->filename = "";
        this->columnNames.clear();
        this->values.clear();
    }
    string toString() const {
        string ans = "";
        if (this->filename != "") ans = ans + this->filename + "----";
        if (!this->columnNames.empty()) {
            ans = ans + "\n";
            for (int i=0; i<this->columnNames.size(); i++) ans = ans + this->columnNames[i] + "-----";
        }
        if (!this->values.empty()) {
            ans = ans + "\n";
            for (int i=0; i<this->values.size(); i++) ans = ans + this->values[i] + "-----";
            ans = ans + "\n";
        }
        return ans;
    }
    void do_action() {
        bool newfile = false;
        ofstream file(this->filename, ios::app);
        if (!file) {
            file.open(this->filename);
        } 

        file.seekp(0, ios::end);
        streampos file_size = file.tellp();
        //if (file.tellp() != 0) file<<"\n"; //co dang o dau dong khong
        if (file_size ==0) { 
            if (!this->columnNames.empty()) {
                
                for (int i=0; i<this->columnNames.size(); i++) {
                    if (i == 0) file<<this->columnNames[i];
                    else file<<","<<this->columnNames[i];
                }
            }
        }
        if (file.tellp() != 0) file<<"\n";

        if (this->columnNames.size() == this->values.size()) {
            for (int i=0; i<this->values.size(); i++) {
                if (i == 0) file<<this->values[i];
                else file<<","<<this->values[i];
            }
        }
        else {
            this->columnNames.clear();
            this->values.clear();

            string s = statement, word, cN, vL;
            size_t pos = s.find('(');
            if (pos != string::npos) s = s.substr(pos+1); 
            pos = s.find(')');
            if (pos != string::npos) cN = s.substr(0, pos);
            pos = s.find('(');
            if (pos != string::npos) s = s.substr(pos+1); 
            pos = s.find(')');
            if (pos != string::npos) vL = s.substr(0, pos);

            stringstream ss1(cN);
            while (getline(ss1, word, ',')) this->columnNames.push_back(word);
            stringstream ss2(vL);
            while (getline(ss2, word, ',')) this->values.push_back(word);

            for (int i=0; i<this->values.size(); i++) {
                if (i == 0) file<<this->values[i];
                else file<<","<<this->values[i];
            }
        }

        file.close();
        display(this->filename);
    }
    bool check_condition() {
        return (this->filename != "") && !this->values.empty();
    }
};

class SelectCommand : public Command {
public:
    vector<string> columnNames;
    void* dataSource;
    Expr* expr;
    SortBy* sort;

public:
    SelectCommand(vector<Token> T) {
        this->columnNames.clear();
        this->dataSource = nullptr;
        this->expr = nullptr;
        this->sort = nullptr;

        if (T[2].type == ID && T[1].type == OPEN) {
            int i = 2;
            for ( ; T[i].type == ID; i++) this->columnNames.push_back(T[i].value);
            if (T[i].type == CLOSE && T[i+=1].type == FROM) {
                i+=1;

                if (T[i].type == FILENAME) this->dataSource = &T[i++].value;
                else if (T[i].type == OPEN && T[i+=1].value == "SELECT") { 
                    int open = 1, close = 0, from = 0, where = 0; 
                    vector<Token> source; 
                    for (; i<T.size(); i++) {
                        if (T[i].type == OPEN) open++;
                        if (T[i].type == CLOSE) close++;
                        if (T[i].type == FROM) from++;
                        if (T[i].type == WHERE) where++;
                        if (from >= 2 || where >=2) break;
                        source.push_back(T[i]);
                    }
                    if (open != close) this->error();
                    else this->dataSource = new SelectCommand(source);
                }
                else this->error();
    
                if (i < T.size()) {
                    vector<Token> condition;
                    while (i!=T.size()) {
                        condition.push_back(T[i++]);
                    }
                    this->buile_condition(condition);
                }
                else {
                    this->expr = nullptr;
                    this->sort = nullptr;
                }
                
            }
            else this->error();
        }
        else this->error();
        //this->toString();
    }
    void error() {
        cout<<"ERROR!";
        this->columnNames.clear();
        this->dataSource = nullptr;
        this->expr = nullptr;
        this->sort = nullptr;
    } 
    string toString() const {
        string s = "";
        int i = 0;
        while (!this->columnNames.empty()) {
            cout<<this->columnNames[i]<<endl;
            i++;
        }
        return "";
    }
    void do_action() {

    }
    void buile_condition(vector<Token> T) {
        int i = 1;
        if (T[0].type == WHERE) {
            for ( ; i<T.size(); i++) {
                if (T[i].type == OPEN) i++;
                if (T[i].type == ID) {
                    string coln = T[i].value; cout<<coln<<endl;

                    if (T[i+=1].type == OPERATION && T[i+=1].type == ID) {
                        string value = T[i].value; cout<<value<<endl;
                        string op = T[i-=1].value; cout<<op<<endl;
                        i+=1;
                        
                        //this->expr = new RelationalExpr(coln, op, value);
                    }
                    else if (T[i+=1].type == OPERATION && T[i+=1].type == OPERATION && T[i+=1].type == ID) {
                        string value = T[i].value; cout<<value<<endl;
                        string op = T[i-=1].value; cout<<op<<endl;
                        i+=1;
                    }
                    else this->error();
                }
            }
        }
        if (T[i++].type == ORDER_BY) {
                
        }
    }
};

void build_parse_tree(vector<Token> T) {
    bool is_insert_cm = false;
    bool is_select_cm = false;

    Command* command;
    if (T[0].type == ACTION) {
        if (T[0].value == "INSERT") {
            is_insert_cm = true;
            command = new InsertCommand(T);
        }
        else if (T[0].value == "SELECT") {
            is_select_cm = true;
            command = new SelectCommand(T);
        }
    }
    else cout<<"ERROR!";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout<<"Error"<<std::endl;
        return 1;
    }
    
    statement = argv[1];

    vector<Token> tokens = extract(statement);

int i = 0;
    for (Token token : tokens) {
        cout<<i++<<"---" << tokentype_name(token) << "----" <<token.value <<"\n";
    }

    build_parse_tree(tokens);

    cout<<endl;
    return 0;
}
