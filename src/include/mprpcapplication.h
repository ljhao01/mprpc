#pragma once
#include "mprpcconfig.h"

//mprpc框架的基础类，负责框架的一些初始化操作，使用单例模式
class MprpcApplication {
public:
    static void Init(int argc, char** argv);
    static MprpcApplication& GetInstance();
private:
    static MprpcConfig config_;
    MprpcApplication();
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};
