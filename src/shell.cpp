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

std::vector<std::vector<std::string>> ash_splite_line(std::vector<char> line){
    std::vector<std::vector<std::string>> all_lines;
    std::vector<std::string> Tokens;
    std::string token;
    size_t i = 0;
    while(i < std::size(line)){
        if(line[i] == ';'){
            Tokens.push_back(token);
            token.clear();
            all_lines.push_back(Tokens);
            Tokens.clear();
        }
        else if(line[i] == ' '){
            Tokens.push_back(token);
            token.clear();
        }

        else if(line[i] != '\0'){
            token.push_back(line[i]);
        }
        i++;
    }
    std::cout << std::size(token) << "  " << std::size(Tokens) << std::endl;
    if(std::size(token)>=1){
    	Tokens.push_back(token);
    }
    all_lines.push_back(Tokens);
    printf(">>%s<<\n", all_lines[0][0].c_str());
    return all_lines;
}



bool ash_launch(char** args){
	pid_t pid, wpid;
	int status;
	pid = fork();
	if(pid == 0){
		printf(">>%s<<\n", args[0]);
		if(execvp(args[0], args) == -1){
			printf("ash: command not found\n");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0){
		perror("ash");
	}
	else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return true;
}


bool ash_execute(std::vector<std::vector<std::string>> commande){
	bool status = true;
	for(size_t i = 0; i < std::size(commande); i++){
		char** args = (char**)malloc(sizeof(char*) * std::size(commande[i]));
		for(size_t j = 0; j < std::size(commande[i]); j++){
			args[j] = reinterpret_cast<char*>(commande[i][j].data());
		}
		status = status && ash_launch(args);
	}
	
	return status;
}	


		

void ash_loop(void){
    std::vector<char> line;
    std::vector<std::vector<std::string>> args;
    int status;

    do{
        printf("$");
        line = ash_readline();
        args = ash_splite_line(line);
	status = ash_execute(args);


    }while(status);
}

int main(void){
    ash_loop();
    return 0;
}
