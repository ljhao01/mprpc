#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "user.pb.h"

int main(int argc, char **argv) {
    // 先调用框架的初始化函数
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub sub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    //rpc方法的响应
    fixbug::LoginResponse response;

    // 发起rpc方法的调用 同步rpc的调用过程 MprpcChannel::callmethod
    sub.Login(nullptr, &request, &response, nullptr);

    // 一次rpc的调用的完成，读调用结果
    if(response.result().errcode()  == 0) {
        std::cout << "rpc login response success: " << response.success() << std::endl;
    } else {
        std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }

    return 0;
}