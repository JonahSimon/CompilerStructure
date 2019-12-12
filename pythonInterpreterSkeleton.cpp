//Unsure if both streams are needed
#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<regex>
#include<vector>
#include<map>
#include"recDecPar.h"

using namespace std;

typedef enum { /*Types of statement*/ ASSIGN, PRINT, INVALID } StatementType;
typedef enum { /*Types of terminal*/KEYWORD, EQUALS, RESULT, RELATIONAL_OP, ADDITIVE_OP, UNSIGNED_REAL, UNSIGNED_INT, OPEN_PAREN, CLOSE_PAREN, IDENTIFIER, EOL, BAD_TOKEN } TokenType; 
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

    void nextLine(string newline){
        Statetype = INVALID;
        line = newline;
        pos = 0;
        
    }

    auto getValue(string key){

        return Values.find(key);

    }

    void addValue(string key,string value){

        Values.insert({key,value});

    }
    
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
            
		if(regex_match(remaining, match, regex("(print).*"))){
			setType(PRINT);
            return Token(KEYWORD, match[1]);}

        if(regex_match(remaining, match, regex("(=).*"))){
			setType(ASSIGN);
            return Token(EQUALS, match[1]);}

        if(regex_match(remaining, match, regex("([0-9]+[\\+|\\-|\\*|\\/][0-9])+.*"))){
            return Token(RESULT, to_string(parseNumber(match[1])));
        }
        
            
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

};

class Parser{
    Statement statement;
    string error;
    Token token, tempToken;
    int parenDepth;
public:

    void equals(){
        
        token = statement.getToken();
        Token t;
        t = statement.peekyboi();

        if (token.type == IDENTIFIER || token.type == RESULT){
            statement.addValue(tempToken.value,token.value);
        }

        if (t.type == UNSIGNED_INT){
            t = statement.getToken();
            t = statement.peekyboi();
        }

        if (t.type == EOL ){
            statement.nextLine("print(ourstr)");
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
            if (statement.getType() == PRINT){
                cout << statement.getValue(tempToken.value)->second << endl;
            }	
	}

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

        return Statement();
    }
};


int main(){
    Parser parser;
    vector<string> input;
    vector<Statement> program;
    bool error = false;
    parser.parse("ourstr = hello");
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