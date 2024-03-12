#include <iostream>
#include <string>
#include "../user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
/*
UserService原来是一个本地方法，提供两个进程内的本地方法Login和GetFriendLists
*/

// 使用在rpc发布端（rpc服务的提供者）
class UserService : public fixbug::UserServiceRpc {
public:    
    bool Login(std::string name, std::string pwd) {
        std::cout << "doing local service : login" << std::endl;
        std::cout << "name" << name << " pwd:" << pwd << std::endl; 
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd) {
        std::cout << "doing local service : Register" << std::endl;
        std::cout << "id:" << id << "name:" << name << " pwd:" << pwd << std::endl; 
        return true;
    }

    // 重写基类UserServiceRpc的虚函数,下面的方法都是框架直接调用的
    // 1、caller ===>  Login(LoginRequest) ===> muduo ====>  callee
    // 2、callee ===>  Login(LoginRequest) ===> 交付到下面的重写的方法上
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done) {
        // 框架给业务上报了请求业务的参数LoginRequeset，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入response
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(1);
        code->set_errmsg("Login do error!");
        response->set_success(login_result);

        // 执行回调操作，执行响应对象数据的序列化和网络发送（都是由框架完成的）
        done->Run(); 
    }
};


int main(int argc, char ** argv) {
    // 调用框架初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务节点 Run以后，进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();

    return 0;
}
