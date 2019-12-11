//Unsure if both streams are needed
#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<vector>
#include<map>
#include<fstream>

using namespace std;

typedef enum { /*Types of statement*/ ASSIGN, PRINT, INVALID } StatementType;
typedef enum { /*Types of terminal*/KEYWORD, EQUALS, RELATIONAL_OP, ADDITIVE_OP, UNSIGNED_REAL, UNSIGNED_INT, QUOTE, OPEN_PAREN, CLOSE_PAREN, IDENTIFIER, EOL, BAD_TOKEN } TokenType;
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
  auto getValue(string key){ return Values.find(key); }

    // Add values to map
  void addValue(string key,string value){ Values.insert({key,value}); }

    // initialize first statement line
  void start(string newline){
		Statetype = INVALID;
		line = newline;
		pos = 0;
	}

	StatementType getType(){ return Statetype; }
  void setPos(int newPos){ pos = newPos; }
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

		if(regex_match(remaining, match, regex("(print|and|or|not|elif|else|if|while|break).*"))){
			setType(PRINT);
      return Token(KEYWORD, match[1]);
		}

    if(regex_match(remaining, match, regex("(=).*"))){
			setType(ASSIGN);
      return Token(EQUALS, match[1]);
		}

    if(regex_match(remaining, match, regex("(\\+|-|or).*")))
			return Token(ADDITIVE_OP, match[1]);

    if(regex_match(remaining, match, regex("([0-9]+\\.[0-9]+([eE][0-9]+)?).*")))
    	return Token(UNSIGNED_REAL, match[1]);

    if(regex_match(remaining, match, regex("([0-9]+).*")))
      return Token(UNSIGNED_INT, match[1]);

    if(regex_match(remaining, match, regex("(\").*")))
      return Token(QUOTE, match[1]);

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

      if (token.type == KEYWORD){ keyword(); }

      if (token.type == IDENTIFIER){ identifier(); }
    }
  }

  void keyword (){
    if(token.value == "print"){
      token = statement.getToken();
      if (token.type == OPEN_PAREN){ open_paren(); }
      else error = "expected open paren";
    }
	}

	void identifier (){
		token = statement.getToken();

    if (token.type == EQUALS){ equals(); }

    else if (token.type == CLOSE_PAREN){ close_paren(); }
	}

  void string_literal(){
  	token = statement.getToken();
  }

	void open_paren (){
  	token = statement.getToken();
		Token t;
    t = statement.peekyboi();
    if(token.type == QUOTE){}
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

  void parse(string input){
  	statement.start(input);
		token = statement.getToken();

    if (token.type == KEYWORD){ keyword(); }

    if (token.type == IDENTIFIER){
    	tempToken = token;
      identifier();
    }
  }
};

vector<string> testExpressions;
void loadTests(){
  string test;
  string tests[13] = {
  	"tests/test1.txt",
    "tests/test2.txt",
    "tests/test3.txt",
    "tests/test4.txt",
    "tests/test5.txt",
    "tests/test6.txt",
    "tests/test7.txt",
    "tests/test8.txt",
    "tests/test9.txt",
    "tests/test10.txt",
    "tests/test11.txt",
    "tests/test12.txt"
  };
  ifstream inf;
  for(int i = 0; i < 12; i++){
  	inf.open(tests[i]);
    getline(inf,test);
    testExpressions.push_back(test);
    inf.close();
  }
}

int main(){
  Parser parser;
  Interpreter interpreter;
  vector<string> input;
  vector<Statement> program;
  bool error = false;

  loadTests();
  for(int i = 0; i < testExpressions.size(); i++){
  	cout << "test " << i + 1 << ": " << testExpressions[i] << endl;
    parser.parse(testExpressions[i]);
  }

	return 0;
}
