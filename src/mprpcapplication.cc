#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>

void ShowArgHelp() {
    std::cout << "format: command -i <configfile>" << std::endl;
}

void MprpcApplication::Init(int argc, char** argv) {
    if(argc < 2) {
        ShowArgHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc, argv, "i:")) != -1) {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            std::cout << "invalid args!" << std::endl;
            ShowArgHelp();
            exit(EXIT_FAILURE);
        case ':':
            std::cout << "need <configfile> " << std::endl;
            ShowArgHelp();
            exit(EXIT_FAILURE); 
        default:
            break;
        }
    }
    // 开始加载配置文件
}

MprpcApplication& MprpcApplication::GetInstance() {
    static MprpcApplication app;
    return app;
}