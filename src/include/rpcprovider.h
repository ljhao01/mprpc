#pragma once
#include "google/protobuf/service.h"
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <google/protobuf/descriptor.h>

// 框架提供的专门服务发布rpc服务的对象类
class RpcProvider{
public:
    // 这里框架是给外部使用的，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service*);
                       
    // 启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    // 组合了TcpServer
    std::unique_ptr<muduo::net::TcpServer>  tcpserverPtr_;
    // 组合EventLoop
    muduo::net::EventLoop eventLoop_;

    // 服务类型信息
    struct ServiceInfo{
        google::protobuf::Service *service_;  // 保存服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methodMap_;      // 保存服务方法
    };

    //存储注册成功的服务对象和服务方法的所有信息
    std::unordered_map<std::string, ServiceInfo> serviceMap_;

    void OnConnection(const muduo::net::TcpConnectionPtr&);

    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
};