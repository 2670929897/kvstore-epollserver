#include "epoll_entry.h"
using namespace std;

//#define BUFFER_SIZE 1024

typedef int (*RCALLBACK)(int fd);

int epfd = 0;
int recv_cb(int fd);
int send_cb(int fd);
int accept_cb(int fd);
/*struct conn_item{
    int fd;
    int idx;
    char buffer[BUFFER_SIZE];

    RCALLBACK accept_callback;
    RCALLBACK recv_callback;
    RCALLBACK send_callback;
};*/

struct epoll_event eventsList[1024];
struct conn_item conn_list[BUFFER_SIZE] = {0};

void setup_event(int fd,int event,int flag){  //将fd加入epfd之中

    if(flag){
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = fd;
        epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev);
    } else{
        struct epoll_event ev;
        ev.events = event;
        ev.data.fd = fd;
        epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
    }

}

int accept_cb(int fd){    //创建监听连接，并创建conn_list的新对象与监听fd连接

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientfd = accept(fd,(struct sockaddr *)&clientAddr,&clientAddrLen);
    setup_event(clientfd,EPOLLIN,1);
    
    conn_list[clientfd].fd = clientfd;
    conn_list[clientfd].idx = 0;
    memset(conn_list[clientfd].buffer,0,BUFFER_SIZE);
    conn_list[clientfd].recv_callback = recv_cb;
    conn_list[clientfd].send_callback = send_cb;

    return clientfd;
}

int recv_cb(int fd){  //接收数据，并修改fd的监听事件
    char *buffer = conn_list[fd].buffer;
    int idx = conn_list[fd].idx;
    int count = recv(fd,buffer+idx,BUFFER_SIZE-idx,0);

    if(count == 0){

        cout << "断开连接 " << eventsList[fd].data.fd << endl;
        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
        close(fd);
        return -1;
    }

    conn_list[fd].idx = count;
    int len = idx + count;
    while (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n')) {
        buffer[len - 1] = '\0';
        len--;
    }
    setup_event(fd,EPOLLOUT,0);
    conn_list[fd].idx -= conn_list[fd].idx;
    kvstore_request(&conn_list[fd]);

    return count > 0 ? count : 0;
}

int send_cb(int fd){  //发送数据，并修改fd的监听事件
    
    char *buffer = conn_list[fd].buffer;
    int idx = conn_list[fd].idx;
    int count = send(fd,buffer,idx,0);

    setup_event(fd,EPOLLIN,0);

    return count > 0 ? count : 0;
}



int epoll_entry(void){

    int listenfd = socket(AF_INET,SOCK_STREAM,0);     //创建服务器端监听fd句柄
    int clientfd = 0;

    struct sockaddr_in listenAddr;                    //存储服务端ip及端口号
    listenAddr.sin_family = AF_INET;                  //协议族
    listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);   //存储服务端ip，htonl()将主机字节序变为网络字节序
    listenAddr.sin_port = htons(8080);             //存储服务端端口号

    if(bind(listenfd,(struct sockaddr *)&listenAddr,sizeof(listenAddr)) == -1){
        perror("bind error");
        exit(1);
        close(listenfd);
    }

    if(listen(listenfd,5) == -1){
        perror("listen error");
        exit(1);
        close(listenfd);
    }
    
    epfd = epoll_create(1);

    setup_event(listenfd,EPOLLIN,1);  //将listenfd加入事件队列和epfd
    conn_list[listenfd].accept_callback = accept_cb;

    while(1){

        int nready = epoll_wait(epfd,eventsList,1024,-1);

        for(int i = 0; i < nready; i++){

            int connfd = eventsList[i].data.fd;  //轮询事件队列取出事件对应的fd

            if(connfd == listenfd){

                //int clientfd = accept_cb(listenfd);
                int clientfd = conn_list[connfd].accept_callback(connfd);
                cout << "已连接 " << clientfd << endl;

            } else if(eventsList[i].events & EPOLLIN){
               
                int count = conn_list[connfd].recv_callback(connfd);
                //cout << "recv " << connfd << ' ' << conn_list[connfd].buffer << endl;

            } else if(eventsList[i].events & EPOLLOUT){

                int count = conn_list[connfd].send_callback(connfd);
                //cout << "send " << connfd << ' ' << conn_list[connfd].buffer << endl;

            }
        }
    }
}