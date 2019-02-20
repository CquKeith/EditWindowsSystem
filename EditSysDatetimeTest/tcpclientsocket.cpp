#include "tcpclientsocket.h"
#include <QMessageBox>
#include <QDebug>

TcpClientSocket::TcpClientSocket(QObject *parent)

{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    //有客户端断开连接的时候触发
//    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

    this->parent = (Server*)parent;
}

/**
 * @brief TcpClientSocket::dataReceived
 * 此函数的服务器接收到客户端发来的消息
 * 应该是客户端请求服务器时间的消息
 */
void TcpClientSocket::dataReceived()
{

    QByteArray datagram;
    datagram.resize(bytesAvailable());
    read(datagram.data(),datagram.size());

//    s_DateTime msg;
    SYSTEMTIME msg;
    GetLocalTime(&msg);
//    parent->getSysTime(msg);

    quint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    this->write((char*)&msg,sizeof(SYSTEMTIME));

    MainWindow *mainwindow = (MainWindow*)parent->parent();
    mainwindow->appendMsgToTextBrower(tr("%1(%2):%3 has sycnchronoused  %4  %5")
                                      .arg(peerName())
                                      .arg(peerAddress().toString())
                                      .arg(peerPort())
                                      .arg(currentTime)
                                      .arg(QDateTime::fromMSecsSinceEpoch(currentTime).toString("yyyy-MM-dd hh:mm:ss:zzz")));
//    emit signal_revMsg(tr("%1(%2):%3 has sycnchronoused  %4  %5")
//                       .arg(peerName())
//                       .arg(peerAddress().toString())
//                       .arg(peerPort())
//                       .arg(currentTime)
//                       .arg(QDateTime::fromMSecsSinceEpoch(currentTime).toString("yyyy-MM-dd hh:mm:ss:zzz")));

}

void TcpClientSocket::slotDisconnected()
{
    //    qDebug()<<__FUNCTION__<<this->socketDescriptor();

}

bool TcpClientSocket::setDateTime(s_DateTime *d)
{
    return setDateTime(d->year,d->month,d->day,d->hour,d->minute,d->second,d->millionSecond);
}

bool TcpClientSocket::setDateTime(uint year, uchar month, uchar day, uchar hour, uchar minute, uchar second, uchar millisecond) const
{
    SYSTEMTIME st;
//    GetLocalTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    st.wYear = year;
    st.wMonth = month;
    st.wDay = day;
    st.wHour = hour;
    st.wMinute = minute;
    st.wSecond = second;
    st.wMilliseconds = millisecond;
    return SetLocalTime(&st);
}
