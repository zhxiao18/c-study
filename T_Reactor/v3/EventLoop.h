#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

class Acceptor;//前向声明
class TcpConnection;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &con)>;

class EventLoop
{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    //事件循环
    void loop();
    //事件不循环
    void unloop();

    void setNewConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);
private:
    //在其中封装::epoll_wait
    void waitEpollFd();

    //处理新的连接
    void handleNewConnection();

    //进行消息的收发（处理）
    void handleMessage(int fd);

    //创建文件描述符_epfd
    int createEpollFd();

    //将文件描述符放在红黑树上进行监听
    void addEpollReadFd(int fd);

    //将文件描述符从红黑树上摘除
    void delEpollReadFd(int fd);

private:
    int _epfd;//由epoll_create创建文件描述符
    bool _isLooping;//标识循环是否在进行
    Acceptor &_acceptor;//为了执行Acceptor类中accept函数
    vector<struct epoll_event> _evtList;//存放满足条件的事件
    map<int, TcpConnectionPtr> _conns;//存放的是文件描述符与TcpConnection的键值对

    TcpConnectionCallback _onConnection;//连接建立
    TcpConnectionCallback _onMessage;//消息到达
    TcpConnectionCallback _onClose;//连接断开

};

#endif
