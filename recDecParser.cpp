#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

typedef enum { RELATIONAL_OP, ADDITIVE_OP, MULTIPLICATIVE_OP, UNARY_OP, UNSIGNED_INT, UNSIGNED_REAL, OPEN_PAREN, CLOSE_PAREN, EOL, BAD_TOKEN} TokenType;

 class Token {
 public:
 TokenType type;
 string value;
 Token(TokenType newType = EOL, string newValue = ""){
   type = newType;
   value = newValue;
 }
};

class Tokenizer{
  string line;
  int pos;
public:
  int getPosition(){
    return pos;
  }
  void start(string newLine){
    line = newLine;
    pos = 0;
  }
  Token peek(){
	  if (pos == line.length()) return Token(EOL);

    smatch match;
    string remaining = line.substr(pos);

    while(regex_match(remaining, match, regex(" .*"))){
      pos++;
      remaining = line.substr(pos);
    }

    if(regex_match(remaining, match, regex("(<|<=|=|=>|>|<>).*")))
      return Token(RELATIONAL_OP, match[1]);

    if(regex_match(remaining, match, regex("(\\+|-|or).*")))
      return Token(ADDITIVE_OP, match[1]);

    if(regex_match(remaining, match, regex("(\\*|/|div|mod|and).*")))
      return Token(MULTIPLICATIVE_OP, match[1]);

    if(regex_match(remaining, match, regex("(\\+|-|not).*")))
      return Token(UNARY_OP, match[1]);

    if(regex_match(remaining, match, regex("([0-9]+\\.[0-9]+([eE][0-9]+)?).*")))
      return Token(UNSIGNED_REAL, match[1]);

    if(regex_match(remaining, match, regex("([0-9]+).*")))
      return Token(UNSIGNED_INT, match[1]);

    if(regex_match(remaining, match, regex("(\\().*")))
      return Token(OPEN_PAREN, match[1]);

    if(regex_match(remaining, match, regex("(\\)).*")))
      return Token(CLOSE_PAREN, match[1]);

	  return Token(BAD_TOKEN);
  }
  Token next(){
    Token t;
    t = peek();
    pos += t.value.size();
    return t;
  }
};

class ExpressionTree{
  ExpressionTree *left, *right;
  Token operation;
public:
  ExpressionTree(Token t=Token(), ExpressionTree *newLeft = NULL, ExpressionTree *newRight = NULL){
    operation = t;
    left = newLeft;
    right = newRight;
  }
  void showRPN(ostream &out){
    if(left != NULL) left->showRPN(out);
    if(right != NULL) right->showRPN(out);
    out << operation.value << " ";
  }
  bool empty(){
    return(operation.type == EOL && left == NULL && right == NULL);
  }
};

class Parser{
  Tokenizer tokenizer;
  string error;
  int parenDepth;
public:
  void expression(ExpressionTree &tree){
    ExpressionTree *subtree = new ExpressionTree();
    ExpressionTree *left = NULL;
    Token last;
    Token next = tokenizer.peek();
    while(error == "" && next.type != EOL && next.type != CLOSE_PAREN){
      additiveExpression(*subtree);
      next = tokenizer.peek();
      if(error == "" && next.type == RELATIONAL_OP){
        next = tokenizer.next();

        if(left != NULL)
          left = new ExpressionTree(last, left, subtree);
        else
          left = subtree;

        subtree = new ExpressionTree();
        last = next;
        next = tokenizer.peek();
        if(next.type == EOL)
          error = "Expected an additive expression";
      }
    }
    if(next.type == CLOSE_PAREN && parenDepth == 0)
      error = "close parenthesis not opened";
    if(left != NULL)
      tree = ExpressionTree(last, left, subtree);
    else
      tree = *subtree;
  }
  void additiveExpression(ExpressionTree &tree){
    ExpressionTree *subtree = new ExpressionTree();
    ExpressionTree *left = NULL;
    Token last;
    Token next = tokenizer.peek();
    while(error == "" && next.type != EOL && next.type != CLOSE_PAREN && next.type != RELATIONAL_OP){
      multiplicativeExpression(*subtree);
      next = tokenizer.peek();
      if(error == "" && (next.type == ADDITIVE_OP)){
          next = tokenizer.next();

          if(left != NULL)
            left = new ExpressionTree(last, left, subtree);
          else
            left = subtree;

          subtree = new ExpressionTree();
          last = next;
          next = tokenizer.peek();
          if(next.type == EOL)
            error = "Expected a multiplicative expression";
        }
    }
    if(left != NULL)
      tree = ExpressionTree(last, left, subtree);
    else
      tree = *subtree;
  }
  void multiplicativeExpression(ExpressionTree &tree){
    ExpressionTree *subtree = new ExpressionTree();
    ExpressionTree *left = NULL;
    Token last;
    Token next = tokenizer.peek();
    while(error == "" && next.type != EOL && next.type != CLOSE_PAREN && next.type != ADDITIVE_OP && next.type != RELATIONAL_OP){
      unaryExpression(*subtree);
      if(error == ""){
        next = tokenizer.peek();
        if(next.type == MULTIPLICATIVE_OP){
          next = tokenizer.next();

          if(left != NULL)
            left = new ExpressionTree(last, left, subtree);
          else
            left = subtree;

          subtree = new ExpressionTree();
          last = next;
          next = tokenizer.peek();
          if(next.type == EOL)
            error = "Expected a unary expression";
        }
      }
    }
    if(left != NULL)
      tree = ExpressionTree(last, left, subtree);
    else
      tree = *subtree;
  }
  void unaryExpression(ExpressionTree &tree){
    Token next = tokenizer.peek();
    if(error == ""){
      if(next.type == UNARY_OP){
        ExpressionTree *subtree = new ExpressionTree();
        next = tokenizer.next();
        unaryExpression(*subtree);
        if(error == "")
          tree = ExpressionTree(next, NULL, subtree);
      }
      else{
        primaryExpression(tree);
      }
    }
  }
  void primaryExpression(ExpressionTree &tree){
    Token next = tokenizer.next();
    if(error == ""){
		if (next.type == OPEN_PAREN) {
      parenDepth++;
			ExpressionTree subtree;
			expression(subtree);
			if (error == "") {
				next = tokenizer.next();
				if (next.type != CLOSE_PAREN)
					error = "Open parenthesis not closed";
				else {
					parenDepth--;
					tree = subtree;
				}
			}
		}
		else if (next.type == UNSIGNED_INT)
			tree = ExpressionTree(next);

		else if (next.type == UNSIGNED_REAL)
			tree = ExpressionTree(next);

		else if (next.type == BAD_TOKEN)
			error = "Token not recognized";
		else if(next.type != CLOSE_PAREN)
			error = "Expected a primary expression";
    }
  }
  ExpressionTree scan(string input){
    ExpressionTree tree;
    tokenizer.start(input);
    error = "";
    parenDepth = 0;
    expression(tree);
    if(error == ""){
      cout << "Expression parsed correctly" << endl;
      return tree;
    }
    else{
      cerr << error << " :" << tokenizer.getPosition() << endl;
    }
    return ExpressionTree();
  }
};

vector<string> testExpressions;
void loadTest() {
  //Some expressions to try that should work
  testExpressions.push_back("1+2");
  testExpressions.push_back("( 1 + 2 ) * 3");
  testExpressions.push_back("((1+2)*3)");
  testExpressions.push_back("1.0E10 + 2.0E10");
  testExpressions.push_back("(1.0E10+2.0E10)*3.0E10");
  testExpressions.push_back("( ( 1.0E10 + 2.0E10 ) * 3.0E10 )");
  testExpressions.push_back("(1+2)*3");
  testExpressions.push_back("( ( 1.0E10 + 2.0E10 ) * 3.010 )");
  testExpressions.push_back("1<2");
  testExpressions.push_back("not 5 and not 7");
  //Some that should error out is some informative way
  testExpressions.push_back("1+");
  testExpressions.push_back("(1+2*3");
  testExpressions.push_back("1.0E+2.0E10");
  testExpressions.push_back("(1.0E10+2.0E)*3.0E10");
  testExpressions.push_back("5-6/");
  testExpressions.push_back("1+2)*3");
  testExpressions.push_back("4/4/5/6))*45))");
  testExpressions.push_back("(1 + 2) <>");
  testExpressions.push_back(")");
}

int main(){
  Parser parser;
  loadTest();
  for (unsigned i = 0; i < testExpressions.size(); i++){
    cout << "Test case " << i + 1 << ": " << testExpressions[i] << endl;
    ExpressionTree tree = parser.scan(testExpressions[i]);
    if(!tree.empty()){
      cout << "RPN: ";
      tree.showRPN(cout);
    }
    cout << endl << endl;
  }
  return 0;
}
