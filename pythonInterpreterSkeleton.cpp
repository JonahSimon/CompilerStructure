//Unsure if both streams are needed
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>

using namespace std;

typedef enum { /*Types of statement*/ INVALID } StatementType;
typedef enum { /*Types of terminal*/ EOL, BAD_TOKEN } TokenType; 
typedef enum { /*valid dataTypes*/ NONE } DataType; 

class Statement{
    StatementType type;
    string value;
public:
    Statement(StatementType newType = INVALID, string newValue = ""){
        type = newType;
        value = newValue;
    }
    StatementType getType(){ return type; }
};

class Token{
    TokenType type;
    string value;
public:
    Token(TokenType newType = EOL, string newValue = ""){
        type = newType;
        value = newValue;
    }
};

class Tokenizer{
    string line;
    int pos;
    public:

    int getPosition(){ return pos; }

    void start(string newLine){
		line = newLine;
		pos = 0;
	}

    Token peek(){
        // Return next token without iterating pos
        return Token();
    }

    Token getToken(){
        // Return next token and iterate pos
        return Token();
    }
};

class Parser{
    Tokenizer tokenizer;
    string error;
    Token token;
    int parenDepth;
public:

    // Functions for terminals

    Statement parse(string input){

        // Re-initialize the parser
        // and run input through it
        // 
        // Output an error if there's a problem
        // return a Statement

        return Statement();
    }
};

class Variable{
    DataType type;
    string value;
public:
    Variable(DataType newType = NONE, string newValue = ""){
        type = newType;
        value = newValue;
    }
};

class Interpreter{
    map<string, Variable> variables;
public:

    // Functions for statement types (e.g. print, assignment, etc.)

    void interpret(Statement input){

        // Run input through the appropriate function
        // Do nothing if input is INVALID

    } 
};

int main(){
    Parser parser;
    Interpreter interpreter;
    vector<string> input;

    // read in input

    for( string s : input ){
        Statement next = parser.parse(s);
        interpreter.interpret(next);
        if(next.getType() == INVALID) break;
    }

    return 0;
}