# heptapod (一个C++11风格的RPC库)
### 介绍
#### 网络
底层基于ASIO的异步多线程模型开发，因此io效率极高。另外，利用strand模型把来自同一个connection的收发包处理strand化，使得逻辑开发无需考虑多线程，方便易用，而同时，多个connection之间又可以并发的处理，cpu利用率也极高。 
#### 封包
不同于其它的一些rpc框架，我使用了msgpack封包，而不是protobuf，主要原因是，protobuf需要预先生成存根，而我想要使这个库的接口更加干净（clean）一点，使用纯C++即可完成rpc存根的声明，因此最终选择了msgpack这种格式。为了达到目的（使用纯c++），实现中使用了大量模板技巧，包括偏特化、typetraits等
#### 使用示例
可以参考example中的echo例子:

使用时需要首先定义一个Service，一个Service即该server提供的rpc服务存根，Service必须继承自hpt::RpcConnection这个模板类，注意这是个模板类，需要把Service本身作为模板参数传进去（少用模板的可能对此用法比较疑惑，可以查看modern c++ design一书，大量使用了这样的技巧，不过不看也无所谓，如果作为使用者，只需要知道这样做就行了），然后，改类必须定义一个名为InitServiceMap的static method，函数中需要用Bind函数声明该service对客户端暴露的可调用接口，比如Bind("Echo", &EchoService::Echo)即声明了一个名为Echo的rpc调用服务，客户端可以使用CallRemote("Echo", xxxx)来调用服务端的该函数。

server端


``` c++
#include <iostream>
#include "rpc_connection.h"
#include "rpc_server.h"

class EchoService : public hpt::RpcConnection<EchoService>
{
public:
    static void InitServiceMap()
    {
        Bind("Echo", &EchoService::Echo);
    }

    void OnConnected() override
    {
    }
    
    void Echo(const std::string& s)
    {
        std::cout << s << std::endl;
        CallRemote("Echo", "Hello from server");
    }
};

int main(int argc, char* argv[])
{
    hpt::RpcServer<EchoService> server;
    server.Start("127.0.0.1", 8001);
    server.WaitSignal();
    server.Stop();
    return 0;
}

```

客户端：
```
class EchoService : public hpt::RpcConnection<EchoService>
{
public:
    static void InitServiceMap()
    {
        Bind("Echo", &EchoService::Echo);
    }

    void OnConnected() override
    {
        CallRemote("Echo", "Client connected");
    }
    
    void Echo(const std::string& s)
    {
        std::cout << s << std::endl;
        CallRemote("Echo", "Hello from client");
    }
};


int main(int argc, char* argv[])
{
    hpt::RpcClient<EchoService> client;
    client.Connect("127.0.0.1", 8001);
    client.WaitSignal();
    client.Shutdown();
    return 0;
}

```

这个例子中，客户端声明了一个rpc函数Echo，服务端也声明了一个rpc函数Echo，客户端在OnConnected的时候，会主动发起调用服务端的Echo，服务端收到后显示内容，然后再调用客户端的Echo，实际上述例子是一个ping-pong的网络通信例子，非常简单，使用heptapod库，定义一个service，几行即可实现
