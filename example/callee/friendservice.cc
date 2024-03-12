#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendSerivce : public fixbug::FriendServiceRpc{
public:  
    // 本地方法
    std::vector<std::string> GetFriendList(uint32_t userid) {
        std::cout << "do GetFriendList service! userid: " << userid << std::endl;
        std::vector<std::string> res;
        res.push_back("gao yang");
        res.push_back("liu hong");
        res.push_back("wang shuo");
        return res;
    }

    // 重写基类的虚函数方法
    void GetFriendList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendListRequest* request,
                       ::fixbug::GetFriendListResponse* response,
                       ::google::protobuf::Closure* done) {
        // 取数据
        uint32_t userid = request->userid();

        // 调用本地业务
        std::vector<std::string> friendList = GetFriendList(userid);

        // 封装到response
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for(auto& f : friendList) {
            std::string *p = response->add_friends();
            *p = f;
        }
        done->Run();
    }

};

int main(int argc, char ** argv) {
     // 调用框架初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendSerivce());

    // 启动一个rpc服务节点 Run以后，进程进入阻塞状态，等待远程rpc调用请求
    provider.Run();

    return 0;
}