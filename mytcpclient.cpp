#include "mytcpclient.h"

MyTcpClient::MyTcpClient(QObject *parent)
{
    this->parent = (MainWindow*)parent;

    connect(this,&MyTcpClient::readyRead,this,&MyTcpClient::dataReceived);

}

/**
 * @brief MyTcpClient::dataReceived
 * tcp收到消息后会触发该函数
 * 接收到的消息应该是服务器发过来的时间
 */
void MyTcpClient::dataReceived()
{
    /*
     * 继承自 QIODevice 的方法，返回可用数据的长度
    */
   quint64 s1= QDateTime::currentMSecsSinceEpoch();
   while(bytesAvailable()>0)
   {
       QByteArray datagram;
       datagram.resize(bytesAvailable());
       read(datagram.data(),datagram.size());

       SYSTEMTIME *msg = (SYSTEMTIME*)datagram.data();

//       qDebug()<<msg->year<<msg->month<<msg->day<<msg->hour<<msg->minute<<msg->second<<msg->millionSecond;


       QTime endQTime = QTime::currentTime();
       int delay = parent->getStartQTime().msecsTo(endQTime);

       msg->wMilliseconds += delay ;
       if(msg->wMilliseconds > 1000){
           msg->wSecond += 1;
           msg->wMilliseconds -= 1000;
       }
//       qint64 startSetDateTime = QDateTime::currentMSecsSinceEpoch();
       bool succ = setDateTime(msg);

       quint64 currentTime = QDateTime::currentMSecsSinceEpoch();

//       s2 = QDateTime::currentMSecsSinceEpoch();
       int program_run_delay = currentTime - s1;
       parent->appendMsgToTextBrower(tr("program_run_delay:%1").arg(program_run_delay));
//       qint64 endSetDateTime = QDateTime::currentMSecsSinceEpoch();
       if(!succ){
           emit signal_revMsg("同步失败~");
       }
       else{
           emit signal_revMsg(tr("收到的时间(+delay/2)：%1 当前时间戳：%2 起始时间戳：%3,delay:%4")
//                                 ",setDateTime耗时:%5")
                              .arg(SystemTimeToString(msg))
                              .arg(currentTime)
                              .arg(parent->getStartMilliSeconds())
                              .arg(delay));
//                              .arg(endSetDateTime-startSetDateTime));
       }

   }
}
/**
 * @brief MyTcpClient::setDateTime
 * 设置本地时间
 * 使用此函数，须获得管理员权限才能设置成功
 * @param d
 * @return 是否设置成功
 */
bool MyTcpClient::setDateTime(s_DateTime *d)
{
    return setDateTime(d->year,d->month,d->day,d->hour,d->minute,d->second,d->millionSecond);
}

bool MyTcpClient::setDateTime(SYSTEMTIME *systime)
{
    return SetLocalTime(systime);
}

/**
 * @brief MyTcpClient::SystemTimeToString
 * 将SYSTEMTIME 转为QString返回
 * @param systime
 * @return
 */
QString MyTcpClient::SystemTimeToString(const SYSTEMTIME *systime)
{
    return QString::number(systime->wHour)+":"+
           QString::number(systime->wMinute)+":"+
           QString::number(systime->wSecond)+":"+
           QString::number(systime->wMilliseconds);
}
bool MyTcpClient::setDateTime(ushort year, ushort month, ushort day, ushort hour, ushort minute, ushort second, ushort millisecond) const
{
    SYSTEMTIME st;
    GetLocalTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    st.wYear = year;
    st.wMonth = month;
    st.wDay = day;
    st.wHour = hour;
    st.wMinute = minute;
    st.wSecond = second;
    st.wMilliseconds = millisecond;
    return SetLocalTime(&st);
}
