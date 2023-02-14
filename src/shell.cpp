#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "config.h"



struct Token{
    char* type;
    char* val;
};

void ash_loop(void);
char* PathAbs;



/* ######################################################
 * #                Builtin function                    #
 * ######################################################
*/


char *builtin_str[] = {
        "cd",
        "help",
        "exit"
};

/*Change Directory function*/
int ash_cd(char** args){
       if(args[1] == NULL){
               fprintf(stderr, "ash: expected argument to \" cd \" \n");
       }else{
               if(chdir(args[1]) != 0){
                       perror("ash");
               }
       }
       getcwd(PathAbs, 1024);
       return 1;
} 

/*help fuction*/
int ash_help(char** args){
        printf("Alexandre GRANDON's ash\n");
        printf("Type name and arguments, and hit enter .\n");
        printf("the fcommandeLineollowing are builtin\n");
        for(int i = 0; i < sizeof(builtin_str)/sizeof(char*); i++){
                printf("   %s\n", builtin_str[i]);
        }       
        printf("use man for infomation on other program\n");
        return 1;
} 

/*exit function*/
int ash_exit(char** args){ exit(EXIT_SUCCESS);}




int (*builtin_fun[]) (char **args){
        &ash_cd,
        &ash_help,
        &ash_exit
}; 

/* #####################################################
 * #                Config Function                    #
 * ####################################################
*/

std::string commandeLine = "$";
std::string color_path;
std::string Answercolor;


char *conf_opt[] = {
    "CommandLine",
    "PathColor",
    "AnswerColor"
};


void conf_cmd_line(char* param){ commandeLine = param;}
void conf_color_path(char* param){ color_path = param;}
void conf_color_answer(char* param){ Answercolor = param;}


void (*conf_func[]) (char* param){
    &conf_cmd_line,
    &conf_color_path,
    &conf_color_answer
};


bool compString(std::string s1, std::string s2){
    size_t i = 0;
    if(std::size(s1)!= std::size(s2)){
        return false;
    }
    while(i < std::size(s1)-1 && s1[i] == s2[i])
        i++;
    return i >= std::size(s1)-1;
}


void Color(std::string color){
	if(compString(color, "GREEN ")){
		printf("\033[0;32m");
	}
	else if(compString(color, "BLUE ")){
		printf("\033[0;34m");
	}
	else if(compString(color, "RED ")){
		printf("\033[0;31m");
	}
}

void resetColor(void){
	printf("\033[0m");
}





/* ################################################
 * #            Lexing Parsing commande           #
 * ################################################
*/

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
            token.push_back('\0');
            Tokens.push_back(token);
            token.clear();
            all_lines.push_back(Tokens);
            Tokens.clear();
        }
        else if(line[i] == ' '){
            token.push_back('\0');
            Tokens.push_back(token);
            token.clear();
        }

        else if(line[i] != '\0'){
            token.push_back(line[i]);
        }
        i++;
    }
    if(std::size(token)>=1){
        token.push_back('\0');
    	Tokens.push_back(token);
    }
    all_lines.push_back(Tokens);
    return all_lines;
}



/* ##########################################
 * #            Execute Command             #
 * ##########################################
*/


bool ash_launch(char** args){
	pid_t pid, wpid;
	int status;
	pid = fork();
       
	if(pid == 0){
      
                int stat = execvp (args[0], args);
     
		if(stat == -1){
			printf("ash: command not found\n");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0){
                printf("ash: command not found\n");
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
                char** args = (char**)malloc(sizeof(char*) * (std::size(commande[i])+1));
		for(size_t j = 0; j < std::size(commande[i]); j++){
			args[j] = reinterpret_cast<char*>(commande[i][j].data());
		}
                args[std::size(commande[i])] = NULL;
		for(int j = 0; j < sizeof(builtin_str) / sizeof(char*); j++){
			if(strcmp(args[0], builtin_str[j]) == 0){
				return (*builtin_fun[j])(args);
			}
		}
		status = status && ash_launch(args);
	}
	
	return status;
}	


/* #########################################
 * #            Shell Function             #
 * #########################################
 */
		

void ash_loop(void){
    std::vector<char> line;
    std::vector<std::vector<std::string>> args;
    int status;

    do{
        Color(color_path);
	printf("%s\n", PathAbs);
        resetColor();
        std::cout << "      " << commandeLine;
        line = ash_readline();
        args = ash_splite_line(line);
	status = ash_execute(args);
        line.clear();
        args.clear();


    }while(status);
}


void ash_init(void){
    std::vector<std::string> all_lines = get_configFile();
    std::vector<Token> tokens = split_conf_line(all_lines);

    for(auto t : tokens){
        for(int i = 0; i < sizeof(conf_opt)/sizeof(conf_opt[0]); i++){
            if(strcmp(t.type, conf_opt[i]) == 0){
                conf_func[i](t.val);
            }
        }
    }
}


int main(void){
        //ash_init();
	PathAbs	= (char*)malloc(1024 * sizeof(char*));
	getcwd(PathAbs, 1024);
    	ash_loop();
	return 0;
}
