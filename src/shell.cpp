#include <iostream>
#include <vector>
#include <unistd.h>
//#include <sys/wait.h>
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
            //	printf("%s\n", Tokens[bpos]);
            token.clear();
            //	printf("%s\n", Tokens[bpos]);
            all_lines.push_back(Tokens);
            //	printf("%s\n", all_lines[0][bpos]);
            Tokens.clear();
        }
        else if(line[i] == ' '){
            Tokens.push_back(token);
            token.clear();
        }

        else{
            token.push_back(line[i]);
        }
        i++;
    }
    Tokens.push_back(token);
    all_lines.push_back(Tokens);
    printf("%s\n", all_lines[0][0].c_str());
    return all_lines;
}



void ash_loop(void){
    std::vector<char> line;
    std::vector<std::vector<std::string>> args;
    int status;

    do{
        printf("$");
        line = ash_readline();
        args = ash_splite_line(line);
        std::cout << std::size(args) << std::endl;
        printf("herer >> %s << \n", args[0][0].c_str());


    }while(status);
}

int main(void){
    ash_loop();
    return 0;
}
