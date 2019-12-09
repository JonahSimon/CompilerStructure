//Unsure if both streams are needed
#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<vector>
#include<map>

using namespace std;

typedef enum { /*Types of statement*/ ASSIGN, PRINT, INVALID } StatementType;
typedef enum { /*Types of terminal*/KEYWORD, EQUALS, RELATIONAL_OP, ADDITIVE_OP, UNSIGNED_REAL, UNSIGNED_INT, OPEN_PAREN, CLOSE_PAREN, IDENTIFIER, EOL, BAD_TOKEN } TokenType; 
typedef enum { /*valid dataTypes*/ NONE } DataType; 

class Token{
	
public:

	TokenType type;
	string value;
	
    Token(TokenType newType = EOL, string newValue = ""){
        type = newType;
        value = newValue;
    }
    
};

class Statement{
	StatementType Statetype;
    string line;
    map<string,string> Values;
    vector<Token>* tokens;
    unsigned int pos;
    
public:

	Statement (StatementType state = INVALID){Statetype = state;}

    // Changes what the line we are working on is and resets pos to 0
    void nextLine(string newline){

        Statetype = INVALID;
        line = newline;
        pos = 0;
        
    }

    // Get values from the map 
    auto getValue(string key){

        return Values.find(key);

    }

    // Add values to map
    void addValue(string key,string value){

        Values.insert({key,value});

    }
    
    // initialize first statement line
    void start(string newline){
		
		Statetype = INVALID;
		line = newline;
		tokens = new vector<Token>;
		pos = 0;
	}
	
	StatementType getType(){ return Statetype; }
    void setPos(int newPos){ pos = newPos; }
    void addToken(Token newToken){ tokens->push_back(newToken); }
    void setType(StatementType type) {Statetype = type;}

    Token peek(){
        if (pos == line.length()) return Token(EOL);
        smatch match;
        string remaining = line.substr(pos);

        while(regex_match(remaining, match, regex(" .*"))){
        pos++;
        remaining = line.substr(pos);
        }

        /*if(regex_match(remaining, match, regex("(<|<=|=|=>|>|<>).*")))
            return Token(RELATIONAL_OP, match[1]);*/
            
		if(regex_match(remaining, match, regex("(print).*"))){
			setType(PRINT);
            return Token(KEYWORD, match[1]);}

        if(regex_match(remaining, match, regex("(=).*"))){
			setType(ASSIGN);
            return Token(EQUALS, match[1]);}
            
        if(regex_match(remaining, match, regex("(\\+|-|or).*")))
            return Token(ADDITIVE_OP, match[1]);

        if(regex_match(remaining, match, regex("([0-9]+\\.[0-9]+([eE][0-9]+)?).*")))
            return Token(UNSIGNED_REAL, match[1]);

        if(regex_match(remaining, match, regex("([0-9]+).*")))
            return Token(UNSIGNED_INT, match[1]);

        if(regex_match(remaining, match, regex("(\\().*")))
            return Token(OPEN_PAREN, match[1]);

        if(regex_match(remaining, match, regex("(\\)).*")))
            return Token(CLOSE_PAREN, match[1]);

        if(regex_match(remaining, match, regex("(([a-z]|[A-Z])+).*")))
            return Token(IDENTIFIER, match[1]);

        return Token(BAD_TOKEN);
    }

    // peek ahead no increment no added token
    Token peekyboi(){
        Token t;
        t = peek();
        return t;
    }

	// Get next token and increment pos while adding token to vector
    Token getToken(){
		Token t;
		t = peek();
		pos += t.value.size();
		addToken(t);
        return t;
    }
	
	// find the previous token. HAVE NOT TESTED THIS YET
    Token lastToken(Token t){
		pos -= t.value.size();
		t = peek();
        return t;
    }
};

class Parser{
    Statement statement;
    string error;
    Token token, tempToken;
    int parenDepth;
public:


    // Functions for terminals
    
    void unaryOp(){
        if(error != ""){

        }
        else{
        }
    }

    void additiveOp(){
        if(error != ""){

        }
        else{
        }
    }

    /*
    void relationalOp(){
        if(){

        }
        else{
        }
    }*/

    void equals(){
        
        token = statement.getToken();
        Token t;
        t = statement.peekyboi();

        if (token.type == IDENTIFIER){
            statement.addValue(tempToken.value,token.value);
        }

        if (t.type == EOL){
            statement.nextLine("print(ourstring)");
            token  = statement.getToken();
            
            if (token.type == KEYWORD){
                keyword();
            }
            
            if (token.type == IDENTIFIER){
                identifier();
            }
        }



    }
    
    void keyword (){
            token = statement.getToken();
            if (token.type == OPEN_PAREN){
                open_paren();
            }
			
	}
		
	void identifier (){
        
			token = statement.getToken();

            if (token.type == EQUALS){
                equals();
            }

            else if (token.type == CLOSE_PAREN){
                close_paren();
            }		
	}
		
	void open_paren (){
        token = statement.getToken();
		Token t;
        t = statement.peekyboi();
        if (t.type == IDENTIFIER){
            tempToken = t;
            identifier();
        }
				
	}
	
	void close_paren (){
		// TO DO FINISH THIS FUNCTION
            if (statement.getType() == PRINT){
            cout << statement.getValue(tempToken.value)->second << endl;
            }

		
	}
    //
    // The Parser doesn't care if a Variable has been declared,
    // it only cares if a valid IDENTIFIER is used in the right place
    //
    // If an IDENTIFIER is used before being declared as a Variable
    // the Interpreter should handle it a runtime error

    Statement parse(string input){
        statement.start(input);
		token = statement.getToken();

        if (token.type == KEYWORD){
            keyword();
        }

        if (token.type == IDENTIFIER){
            tempToken = token;
            identifier();
        }
		//Call something  or check what it is and then call something

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

    parser.parse("ourstring = Hello");
    // Read input
    /*
    for(unsigned int i=0; i < input.size() && !error; i++){
        program.push_back(parser.parse(input[i]));
	
		if(program.front().getType() == INVALID) error = true;
    }

    if(!error)
        interpreter.interpret(program);
    */
    return 0;
}