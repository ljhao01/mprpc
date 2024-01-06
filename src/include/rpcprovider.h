#pragma once
#include "google/protobuf/service.h"

// 框架提供的专门服务发布rpc服务的对象类
class RpcProvider{
public:
    // 这里框架是给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service){}
                       
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run(){}
};