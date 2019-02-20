#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H
#include <QObject>
#include <QTcpSocket>
#include "server.h"
#include "headerstruct.h"

/**
 * TcpServer接收到新的客户端A的连接后，会新建一个TcpClientSocket对象与A“相连”
 * TcpServer与A进行通信，实质上就是通过这个TcpClientSocket实现的
 */

class Server;
class TcpClientSocket : public QTcpSocket
{
     Q_OBJECT  //添加这个宏  用于实现信号和槽的通信
public:
    TcpClientSocket(QObject *parent = 0);

protected slots:
    //从客户端收到消息的处理
    void dataReceived();
    //用户下线的时候
    void slotDisconnected();

signals:
//    void dataToServer(QString,int);
//    void signal_discon(int);

    //接收到服务器发送的数据
    void signal_revMsg(QString msg);

private:
    Server *parent;

    __inline bool setDateTime(uint year,uchar month,uchar day,uchar hour,uchar minute,uchar second,uchar millisecond) const;
    __inline bool setDateTime(s_DateTime *d);
};

#endif // TCPCLIENTSOCKET_H
