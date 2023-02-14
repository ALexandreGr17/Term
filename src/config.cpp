#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include "config.h"

typedef struct Token Token;

struct Token{
        std::string type;
        std::string val;
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
		Token* t = (Token*)malloc(sizeof(*t));
		for(auto i : l){
			if(i == ';'){
                                std::string val(buffer.begin(), buffer.end());
				t->val = val;
				buffer.clear();
                                break;
			}
			else if(i == ':'){
                                std::string type(buffer.begin(), buffer.end());
                                std::cout << t->type;
				buffer.clear();
			}
			else if(i != ' '){
				buffer.push_back(i);
			}
		}
                //tokens.push_back(*t);
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
 //       std::cout << std::size(tokens) << std::endl;
        for(auto t : tokens){
//            std::cout << t.type << " "  << t.val << std::endl;
        }
}
*/
