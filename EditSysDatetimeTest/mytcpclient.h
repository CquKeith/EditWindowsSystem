#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "mainwindow.h"
#include "headerstruct.h"

class MainWindow;

/**
 * @brief The MyTcpClient class
 * MainWindow下，与Server握手，进行通信的类
 */
class MyTcpClient : public QTcpSocket
{
    Q_OBJECT  //添加这个宏  用于实现信号和槽的通信
public:
    MyTcpClient(QObject *parent = 0);

protected slots:
    //receive message from tcpserver
    void dataReceived();


signals:
    //send message to MainWindow
    void signal_revMsg(QString msg);

private:
     MainWindow *parent;

     __inline bool setDateTime(ushort year,ushort month,ushort day,ushort hour,ushort minute,ushort second,ushort millisecond) const;
     __inline bool setDateTime(s_DateTime *d);
     __inline bool setDateTime(SYSTEMTIME *systime);

     QString SystemTimeToString(const SYSTEMTIME *systime);
};

#endif // MYTCPCLIENT_H
