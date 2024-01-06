#pragma once
#include "google/protobuf/service.h"

//mprpc框架的基础类，负责框架的一些初始化操作，使用单例模式
class MprpcApplication {
public:
    static void Init(int argc, char** argv);
    static MprpcApplication& GetInstance();
private:
    MprpcApplication() {}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};
