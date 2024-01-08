#include "mprpcconfig.h"
#include <iostream>

void MprpcConfig::LoadConfigFile(const char* config_file) {
    FILE* fp = fopen(config_file, "r");
    if(nullptr == fp) {
        std::cout << config_file << "is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(!feof(fp)) {
        
    }

}

std::string MprpcConfig::Load(const std::string &key) {
    
}