#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>

typedef struct Token Token;

struct Token{
	char* type;
	std::vector<char*> val;
};



std::vector<std::string> get_configFile(void){
	std::ifstream ashrc (".ashrc");
	std::vector<std::string> all_lines;
	if(ashrc){
		std::string line;
		while(getline(ashrc, line)){
			all_lines.push_back(line);
		}
	}
	return all_lines;

}

std::vector<Token> split_conf_line(std::vector<std::string> lines){
	std::vector<Token> tokens;
	for( auto l : lines){
		std::vector<char> buffer;
		Token* t = (Token*)malloc(sizeof(Token));
		for(auto i : l){
			if(i == ';'){
				t->val.push_back(reinterpret_cast<char*>(buffer.data()));
				buffer.clear();
			}
			else if(i == ':'){
				t->type = reinterpret_cast<char*>(buffer.data());
				buffer.clear();
			}
			else if(i != ' '){
				buffer.push_back(i);
			}
		tokens.push_back(*t);
		}
	}
	return tokens;
}


int main(void){
	std::vector<std::string> all_lines = get_configFile();
        std::cout << all_lines[0] << std::endl;
        std::vector<Token> t = split_conf_line(all_lines);
}

