#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"

/*
service_name => service描述（用结构体表示）
                        => service* 记录服务对象
                           method_name => method方法对象
*/

void RpcProvider::NotifyService(google::protobuf::Service *service) {
    ServiceInfo service_info;

    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pserviceDesc = service->GetDescriptor();
    // 获取服务的名字
    std::string serivce_name = pserviceDesc->name();
    // 获取服务对象service的方法的数量
    int methodCnt = pserviceDesc->method_count();

    for(int i = 0; i < methodCnt; i++) {
        // 获取了服务对象指定下标的服务方法的描述（抽象描述）
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
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
    if(!conn->connected()) {
        conn->shutdown();
    }
}
/*
在框架内部，RpcProvider和RpcConsumer协商好通信用的protobuf格式
service_name method_name args 定义proto的message类型，进行数据头的序列化和反序列化
eg:16UserServiceLoginzhang san123456

header_size(4个字节) + head_str + args_str
*/
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn, 
                            muduo::net::Buffer *buffer, 
                            muduo::Timestamp time) {
    // 网络接受远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString();
    std::cout << recv_buf << std::endl;
    // 从字符流中读取前4个字节调用请求的字符流
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);
    // 根据haed_size读取数据，反序列化数据，得到rpc请求的详细信息
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;
    std::string service_name, method_name;
    uint32_t args_size;

    if(rpcHeader.ParseFromString(rpc_header_str)) {
        // 反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        //反序列化失败
        std::cout << "rpc_header_str:" << rpc_header_str << " parse error" << std::endl;
        return;
    }

    // 获取rpc方法中参数的字符流数据
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    std::cout << "===============================================================" << std::endl;
    std::cout << "header_size:" << header_size << std::endl;
    std::cout << "rpc_header_str:" << rpc_header_str << std::endl; 
    std::cout << "service_name:" << service_name << std::endl; 
    std::cout << "method_name:" << method_name << std::endl; 
    std::cout << "args_size:" << args_size << std::endl; 
    std::cout << "===============================================================" << std::endl;


    // 获取service对象和method方法
    auto it = serviceMap_.find(service_name);
    if(it == serviceMap_.end()) {
        std::cout << service_name << "is not exist!" << std::endl; 
        return;
    }
    
    auto mit = it->second.methodMap_.find(method_name);
    if(mit == it->second.methodMap_.end()) {
        std::cout << service_name << method_name << "is not exist!" << std::endl; 
        return;
    }

    google::protobuf::Service *service = it->second.service_;           //service对象    UserService
    const google::protobuf::MethodDescriptor *method = mit->second;     //method方法     login()

    // 生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str)) {
        std::cout << "request parse error, content:" << args_str<< std::endl;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    // 给下面的CallMethod方法的调用，绑定一个Closure的回调函数
    google::protobuf::Closure *done = google::protobuf::NewCallback<RpcProvider, 
                                                                    const muduo::net::TcpConnectionPtr&,
                                                                    google::protobuf::Message*>
                                                                    (this, &RpcProvider::SendRpcResponse, conn, response);
    service->CallMethod(method, nullptr, request, response, done);
}

// Closure回调操作，用于序列化rpc的响应和网络发送
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response) {
    std::string response_str;
    if(response->SerializeToString(&response_str)) {
        // 序列化成功后，通过网络将rpc方法执行的结果发送给rpc的调用方
        conn->send(response_str);
        conn->shutdown();   //rpcprovider主动断开连接 
    } else {
        std::cout << "serilaize response_str error!" << std::endl;
    }
    conn->shutdown();
}