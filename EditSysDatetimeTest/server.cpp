#include "server.h"
#include "tcpclientsocket.h"
#include <QMessageBox>
#include <QDebug>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    port = 7890;
    ui_parent = (MainWindow*)parent;
//    if(!this->listen(QHostAddress::AnyIPv4,7890))
//    {
//        QMessageBox::critical(0,tr("服务器"),tr("无法启动服务器：%1.").arg(this->errorString()));

    //    }
}

Server::~Server()
{

}
/**
 * @brief Server::startListen
 * @param port
 */
bool Server::startListen(int port)
{
    this->port = port;
    if(!this->listen(QHostAddress::AnyIPv4,7890))
    {
        QMessageBox::critical(ui_parent,tr("服务器"),tr("无法启动服务器：%1.").arg(this->errorString()));
        return false;
    }
    return true;
}
/*
 * 当有新的用户连接进来的时候 触发这个函数，参数是socket的描述符
 *  此函数 重构了父类QTcpServer类中的虚函数，当newConnection信号发射时，自动进入此函数
*/
void Server::incomingConnection(qintptr socketDescriptor)
{

    TcpClientSocket *tcpclientsocket = new TcpClientSocket(this);
//    connect(tcpclientsocket,SIGNAL(dataToServer(QString,int)),ui_counter,SLOT(dealMessage(QString,int)));

//    connect(tcpclientsocket,SIGNAL(signal_discon(int)),this,SLOT(slotDisconnected(int)));

    tcpclientsocket->setSocketDescriptor(socketDescriptor);
   // 在服务器端中记录刚刚登陆进来的客户端
    tcpClientSocketList.append(tcpclientsocket);

//    s_DateTime msg;
//    getSysTime(msg);

//    quint64 currentTime = QDateTime::currentMSecsSinceEpoch();
//    tcpclientsocket->write((char*)&msg,sizeof(s_DateTime));
//    qDebug()<<tcpclientsocket->peerName();
//    ui_parent->appendMsgToTextBrower(tr("%1(%2):%3 has sycnchronoused  %4  %5")
//                                     .arg(tcpclientsocket->peerName())
//                                     .arg(tcpclientsocket->peerAddress().toString())
//                                     .arg(tcpclientsocket->peerPort())
//                                     .arg(currentTime)
//                                     .arg(QDateTime::fromMSecsSinceEpoch(currentTime).toString("yyyy-MM-dd hh:mm:ss:zzz")));
//    emit signal_MsgToUI(tr("%1(%2):%3 has sycnchronoused  %4  %5")
//                        .arg(tcpclientsocket->peerName())
//                        .arg(tcpclientsocket->peerAddress().toString())
//                        .arg(tcpclientsocket->peerPort())
//                        .arg(currentTime)
//                        .arg(QDateTime::fromMSecsSinceEpoch(currentTime).toString("yyyy-MM-dd hh:mm:ss:zzz")));

//    tcpclientsocket->close();


}

void Server::clearClientList()
{
    for(TcpClientSocket *tcpclientsocket:tcpClientSocketList){
        tcpclientsocket->close();
    }
}
/**
 * @brief Server::getSysTime
 * @return
 */
void Server::getSysTime(s_DateTime & msg)
{
    SYSTEMTIME st;
    GetLocalTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    msg.year = st.wYear;
    msg.month = st.wMonth;
    msg.day = st.wDay;
    msg.hour = st.wHour;
    msg.minute = st.wMinute;
    msg.second = st.wSecond;
    msg.millionSecond = st.wMilliseconds;

}
void Server::slotDisconnected(int descriptor)
{
    Q_UNUSED(descriptor)
}

