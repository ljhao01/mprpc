#include "mprpcconfig.h"
#include <iostream>

void MprpcConfig::LoadConfigFile(const char* config_file) {
    FILE* fp = fopen(config_file, "r");
    if(nullptr == fp) {
        std::cout << config_file << "is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 1、注释 2、正确的配置项 = 3、去掉多余的空格
    while(!feof(fp)) {
        char buf[512] = {0};
        fgets(buf, 512, fp);

        std::string read_buf(buf);
        Trim(read_buf);

        // 判断 # 的注释
        if(read_buf[0] == '#' || read_buf.empty()) {
            continue;
        }

        // 判断配置项的空格
        int idx = read_buf.find('=');
        if(idx == -1) {
            // 配置项不合法
            continue;
        }

        std::string key, value;
        key = read_buf.substr(0, idx);
        Trim(key);
        int end_idx = read_buf.find('\n', idx);
        value = read_buf.substr(idx + 1, end_idx - idx - 1);
        Trim(value);
        configMap_.insert({key, value});
    }
}

std::string MprpcConfig::Load(const std::string &key) {
    if(configMap_.find(key) != configMap_.end()) {
        return configMap_[key];
    }
    return "";
}

void MprpcConfig::Trim(std::string &src_buf) {
    int idx = src_buf.find_first_not_of(' ');
    if(idx != -1) {
        // 说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }
    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if(idx != -1) {
        // 说明字符串后面有空格
        src_buf = src_buf.substr(0, idx + 1);
    }
}