#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "friend.pb.h"

int main(int argc, char **argv) {
    // 先调用框架的初始化函数
    MprpcApplication::Init(argc, argv);

    fixbug::FriendServiceRpc_Stub sub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::GetFriendListRequest req;
    req.set_userid(1000);

    /// response方法的调用
    fixbug::GetFriendListResponse rsp;
    sub.GetFriendList(nullptr, &req, &rsp, nullptr);
    if(rsp.result().errcode()  == 0) {
        std::cout << "rpc GetFriendList response success!" << std::endl;
        int sz = rsp.friends_size();
        for(int i = 0; i < sz; i++) {
            std::cout << "index: "<<(i+1) << " name:" << rsp.friends(i) << std::endl;
        }

    } else {
        std::cout << "rpc GetFriendList response error: " << rsp.result().errmsg() << std::endl;
    }

    return 0;
}
