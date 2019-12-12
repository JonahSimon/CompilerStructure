#include <vector>
#include <ostream>
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

char token;
string line2;
bool Error = false;

int index3 = 0;

union expressionStuff{
	int i;
	char ch;
};
vector <int> numbs;
vector <char> ops;

void expression2();
void term2();
void factor2();
void number2();
void digit2();
void parse2();
void GetToken2();
void doOps();

int parseNumber(string line){
	line2 = line;
	index3 = 0;
	Error = false;
	parse2();
	numbs.shrink_to_fit();
	if (numbs.size() <= 2){
			doOps();
		}
	int ans = numbs.back();
	//cout  << endl << ans << endl;
	int result = ans;
	return ans;

}

void doOps(){
		int second = numbs.back();
			numbs.pop_back();
		int first = numbs.back();
			numbs.pop_back();
		char op = ops.back();
			ops.pop_back();

		if (op == '+'){
			numbs.push_back(first+second);
		}

		else if(op == '-'){
			numbs.push_back(first*second);
		}

		else if(op == '/'){
			numbs.push_back(first/second);
		}

		else if(op == '-'){
			numbs.push_back(first-second);
		}

}

/*int main2(){
	line2 = "(2+3)*6";
	// << line2.length();
	parse2();
	if(Error == false)
		// << endl << "The parse was successful.";
	}*/
	
void GetToken2(){
	token = line2[index3];
	// << endl << "<gettoken> " << token << " ";
	index3++;
}

void expression2(){
	// << "<exp> ";
	term2();
	while (token == '+'){
		ops.push_back(token);
		GetToken2();
		term2();
		}
}

void term2(){
	// << "<term> ";
	factor2();
	while(token == '*'){
		ops.push_back(token);
		GetToken2();
		factor2();
		}
}
	
void factor2(){
	// << "<factor> ";
		if (token == '('){
			GetToken2();
			expression2();
			if (token == ')')
				GetToken2();
			else {
				// << endl <<"Etokenrror expected )";
				Error = true;
			}
		}
		else{ 
			number2();
		}
}
	
void number2(){
		// << "<number> ";
		digit2();
		while(token == ('1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'))
			digit2();
}
	
void digit2(){
		// << "<digit>";
		char c = token;
		int numb = stoi(&c);
		numbs.push_back(numb);
	if (isdigit(line2[index3 - 1])){
		GetToken2();
	}
	else {
		// << endl <<"Error expected a digit." << " " << token;
		Error = true;
	}
}
	
void parse2(){
	// << "<parse> ";
	GetToken2();
	expression2();
}
