#include "test.pb.h"
#include <iostream>
#include <string>

int main() {

    fixbug::LoginRequest req;
    req.set_name("zhangsan");
    req.set_pwd("123");

    std::string send_str;
    if(req.SerializeToString(&send_str)) {
        std::cout << send_str.c_str() << std::endl;
    }

    // 反序列化
    fixbug::LoginRequest reqB;
    if(reqB.ParseFromString(send_str)) {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }

    return 0;
}