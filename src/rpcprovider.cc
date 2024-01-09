#include "rpcprovider.h"
#include "mprpcapplication.h"


void RpcProvider::NotifyService(google::protobuf::Service *service) {
    ServiceInfo service_info;

    // 获取服务对象的描述信息
    auto pserviceDesc = service->GetDescriptor();
    // 获取服务的名字
    auto serivce_name = pserviceDesc->name();
    // 获取服务对象service的方法的数量
    int methodCnt = pserviceDesc->method_count();

    for(int i = 0; i < methodCnt; i++) {
        auto pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.methodMap_.insert({method_name, pmethodDesc});
    }
    service_info.service_ = service;
    serviceMap_.insert({serivce_name, service_info});

}

void RpcProvider::Run() {
    std::string ip = MprpcApplication::GetInstance().GetMprpcConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetMprpcConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    // 创建TcpServer对象
    muduo::net::TcpServer server(&eventLoop_, address, "RpcProvider");
    // 绑定连接回调和消息读写回调方法 分离网络代码和业务代码
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                    std::placeholders::_2, std::placeholders::_3));

    // 设置muduo库的线程数量
    server.setThreadNum(4);

    std::cout << "Rpc Provider start serivce at ip:" << ip << " port:" << port << std::endl;
    
    server.start();
    eventLoop_.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn) {

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp time) {

}