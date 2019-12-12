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

void parseNumber(string line){
	line2 = line;
	index3 = 0;
	Error = false;
	parse2();
	if (numbs.size() <= 2){
			doOps();
		}
	int ans = numbs.back();
	cout << endl << ans << endl;

}

void doOps(){
		int first = numbs.back();
			numbs.pop_back();
		int second = numbs.back();
			numbs.pop_back();
		char op = ops.back();
			ops.pop_back();

		if (op == '+'){
			numbs.push_back(first+second);
		}

		if(op == '-'){
			numbs.push_back(first*second);
		}

		if(op == '/'){
			numbs.push_back(second/first);
		}

		if(op == '-'){
			numbs.push_back(second-first);
		}

}

/*int main2(){
	line2 = "(2+3)*6";
	cout << line2.length();
	parse2();
	if(Error == false)
		cout << endl << "The parse was successful.";
	}*/
	
void GetToken2(){
	token = line2[index3];
	cout << endl << "<gettoken> " << token << " ";
	index3++;
}

void expression2(){
	cout << "<exp> ";
	term2();
	while (token == '+'){
		ops.push_back(token);
		GetToken2();
		term2();
		}
}

void term2(){
	cout << "<term> ";
	factor2();
	while(token == '*'){
		ops.push_back(token);
		GetToken2();
		factor2();
		}
}
	
void factor2(){
	cout << "<factor> ";
		if (token == '('){
			GetToken2();
			expression2();
			if (token == ')')
				GetToken2();
			else {
				cout << endl <<"Error expected )";
				Error = true;
			}
		}
		else 
			number2();
}
	
void number2(){
		cout << "<number> ";
		digit2();
		while(token == ('1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'))
			digit2();
}
	
void digit2(){
		cout << "<digit>";
	if (isdigit(line2[index3 - 1])){
		numbs.push_back((int)token);
		GetToken2();
	}
	else {
		cout << endl <<"Error expected a digit." << " " << token;
		Error = true;
	}
}
	
void parse2(){
	cout << "<parse> ";
	GetToken2();
	expression2();
}
