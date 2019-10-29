//Unsure if both streams are needed
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>

using namespace std;

typedef enum { /*Types of statement*/ INVALID } StatementType;
typedef enum { /*Types of terminal*/ IDENTIFIER, EOL, BAD_TOKEN } TokenType; 
typedef enum { /*valid dataTypes*/ NONE } DataType; 

class Token{
    TokenType type;
    string value;
public:
    Token(TokenType newType = EOL, string newValue = ""){
        type = newType;
        value = newValue;
    }
    TokenType getType(){ return type; }
    string getValue(){ return value; }
};

class Statement{
    StatementType type;
    vector<Token>* tokens;
    int pos;
public:
    Statement(StatementType newType = INVALID){
        type = newType;
        tokens = new vector<Token>;
        pos = 0;
    }

    StatementType getType(){ return type; }
    void setPos(int newPos){ pos = newPos; }
    void addToken(Token newToken){ tokens->push_back(newToken); }

    Token peek(){

        // Return next token without iterating pos

        return Token();
    }

    Token getToken(){

        // Return next token and iterate pos

        return Token();
    }

    Token lastToken(){

        // Decrement pos then return current token

        return Token();
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
    //
    // The Parser doesn't care if a Variable has been declared,
    // it only cares if a valid IDENTIFIER is used in the right place
    //
    // If an IDENTIFIER is used before being declared as a Variable
    // the Interpreter should handle it a runtime error

    Statement parse(string input){

        // Re-initialize the parser
        // and run input through it
        // 
        // Output an error and return an INVALID Statement
        // if there's a problem
        //
        // Otherwise, return the parsed Statement

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
    DataType getType(){ return type; }
};

class Interpreter{
    // string = identifier, Variable = value
    map<string, Variable> variables;
    vector<Statement> program;
    string error;
    // Increment/Decrement line for loops and conditional statements
    int line;
    // Used to keep track of loop/conditional level
    int tabDepth;
public:

    // Functions for statement types (e.g. print, assignment, etc.)

    void interpret(vector<Statement> input){

        // Run input through the appropriate function
        // Do nothing if input is INVALID
        //
        // Output an error and stop running if there's a problem

    } 
};

int main(){
    Parser parser;
    Interpreter interpreter;
    vector<string> input;
    vector<Statement> program;
    bool error = false;

    // Read input

    for(int i=0; i < input.size() && !error; i++)
        program.push_back(parser.parse(input[i]));

    if(!error)
        interpreter.interpret(program);

    return 0;
}