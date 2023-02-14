#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>

using namespace std;




std::string to_string(std::filesystem::file_time_type const& ftime) {
    std::time_t cftime = std::chrono::system_clock::to_time_t(
        std::chrono::file_clock::to_sys(ftime));
    std::string str = std::asctime(std::localtime(&cftime));
    str.pop_back();  // rm the trailing '\n' put by `asctime`
    return str;
}

string trim(string Path){
    string newPath;
    size_t i = size(Path)-1;
    while(i >= 0 && Path[i] != '/'){
        newPath = Path[i] + newPath;
        i--;
    }
    return newPath;
}


vector<string> ls(vector<string> args){
    bool a = false;
    bool l = false;
    
    for(size_t i = 0; i < size(args); i++){
        if(args[i][0] == '-'){
            for(size_t j = 1; j < size(args[i]); j++){
                if(args[i][j] == 'a')
                    a = true;
                if(args[i][j] == 'l')
                    l = true;
            }
        }
    }
    cout << a << l << endl;

    string Path = "";
    size_t i = 0;
    while(i < size(args) && Path == ""){
        if(args[i][0] != '-')
            Path = args[i];
        i++;
    }

    if(Path == "")
        Path = "./";
    
    const filesystem::path PATH{Path};
    vector<string> all_files;
    for(auto const& dir_entry : filesystem::directory_iterator{PATH})
    {
        string file = (trim(dir_entry.path()));
        if(file[0] == '.'){
            if(a){
                cout << file << endl;
                all_files.push_back(file);
            }
        }
        else{
            cout << to_string(dir_entry.last_write_time()) << file << endl;
            all_files.push_back(file);
        }
    }
    return all_files;

}


int main(void){
    vector<string> args;
    args.push_back("-a");
    args.push_back("-l");
    vector<string> files = ls(args);
    cout << files[0] ;
    return 0;
}
