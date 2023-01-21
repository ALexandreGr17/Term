#ifndef CONFIG_H
#define CONFIG_H

typedef struct Token Token;
std::vector<std::string> get_configFile(void);
std::vector<Token> split_conf_line(std::vector<std::string> lines);

#endif
