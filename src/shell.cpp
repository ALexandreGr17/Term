#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>


void ash_loop(void);




std::vector<char> ash_readline(){
	std::vector<char> buffer;
	char c;

	while(true){
		c = getchar();
		if(c == EOF || c == '\n'){
			buffer.push_back('\0');
			return buffer;
		}
		buffer.push_back(c);
	}
}

std::vector<char**> ash_splite_line(std::vector<char> line){
	std::vector<char**> all_lines;
	int TokenSize = 1;
	char** Tokens = (char**)malloc(sizeof(char*) * TokenSize);
	std::vector<char> token;
	int bpos = 0;
	size_t i = 0; 
	while(i < std::size(line)){
		if(line[i] == ';'){
			Tokens[bpos] = reinterpret_cast<char*>(token.data());
			bpos = 0;
		//	printf("%s\n", Tokens[bpos]);
			token.clear();
		//	printf("%s\n", Tokens[bpos]);
			all_lines.push_back(Tokens);
		//	printf("%s\n", all_lines[0][bpos]);
			Tokens = (char**)malloc(sizeof(char*) * TokenSize);
		}
		else if(line[i] == ' '){
			Tokens[bpos] = reinterpret_cast<char*>(token.data());
			bpos++;
			token.clear();
			if(bpos >= TokenSize){
				TokenSize++;
				Tokens = (char**)realloc(Tokens, sizeof(char**) * TokenSize);
				if(!Tokens){
					fprintf(stderr, "ash split line : reallocation error");
				}
			}
		}

		else{
			token.push_back(line[i]);
		}
		i++;
	}
	Tokens[bpos] = reinterpret_cast<char*>(token.data());
	all_lines.push_back(Tokens);
	printf("%s\n", all_lines[0][0]);
	return all_lines;
}



void ash_loop(void){
	std::vector<char> line;
	std::vector<char**> args;
	int status;

	do{
		printf("$");
		line = ash_readline();
		args = ash_splite_line(line);
		std::cout << std::size(args) << std::endl;
		printf("herer >> %s << \n", args[0][1]);


	}while(status);
}

int main(void){
	ash_loop();
	return 0;
}
