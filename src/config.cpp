#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "config.h"

typedef struct Token Token;

struct Token{
	char* type;
	char* val;
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
                                char* val = (char*)malloc(sizeof(char) * std::size(buffer));
                                for(size_t j = 0; j < std::size(buffer); j++)
                                    val[j] = buffer[j];

				t->val = val;
				buffer.clear();
                                break;
			}
			else if(i == ':'){
                                char* type = (char*)malloc(sizeof(char) * std::size(buffer));
                                for(size_t j = 0; j < std::size(buffer); j++)
                                    type[j] = buffer[j];

				t->type = type;
				buffer.clear();
			}
			else if(i != ' '){
				buffer.push_back(i);
			}
		}
                tokens.push_back(*t);
	}
	return tokens;
}



/*
int main(void){
	std::vector<std::string> all_lines = get_configFile();
        for(auto l : all_lines ){
            std::cout << l<< std::endl;
        }
        std::vector<Token> tokens = split_conf_line(all_lines);
        std::cout << std::size(tokens) << std::endl;
        for(auto t : tokens){
            std::cout << t.type << " "  << t.val << std::endl;
        }
}
*/
