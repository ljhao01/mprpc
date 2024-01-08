#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

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

    void OnConnection(const muduo::net::TcpConnectionPtr&);

    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
};