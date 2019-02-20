#ifndef SERVER_H
#define SERVER_H
#include <QList>

#include <QObject>
#include <QTcpServer>
#include "tcpclientsocket.h"
#include "mainwindow.h"
#include "headerstruct.h"

class MainWindow;
class TcpClientSocket;
class Server : public QTcpServer
{
     Q_OBJECT  //添加Q_OBJECT宏用于信号的槽的通信
public:
    Server(QObject *parent);
    ~Server();
    //用于与每一个客户端连接的TcpClientSock
     QList<TcpClientSocket *> tcpClientSocketList;

     bool startListen(int port);
     void getSysTime(s_DateTime &msg);
     void clearClientList();


protected:
    //重载 TcpServer中的这个虚函数，当新连进来一个客户端时自己调用
    void incomingConnection(qintptr socketDescriptor);

private:
    int port;
    MainWindow *ui_parent;




private slots:
    void slotDisconnected(int description);
signals:
    void signal_MsgToUI(QString msg);
};

#endif // SERVER_H
