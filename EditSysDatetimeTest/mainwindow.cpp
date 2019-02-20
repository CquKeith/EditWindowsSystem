#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QDateTime dt = QDateTime::currentDateTime();
//    qDebug()<<dt;

//    setDate(2018,9,10);        //设置日期
//    setTime(16,12,34,133);

//    qDebug()<<QDateTime::currentDateTime();
    tcpServer = new Server(this);
    tcpClient = new MyTcpClient(this);

    connect(tcpServer,&Server::signal_MsgToUI,[=](QString msg){
       ui->textBrowser->append(msg);
    });

    connect(tcpClient,&MyTcpClient::connected,[=]{
//        qDebug()<<"成功连上服务器";
        ui->textBrowser->append(tr("成功连上 %1::%2").arg(ui->lineEdit_serverIP->text()).arg(ui->lineEdit_serverPort->text()));
        ui->pbConnect->setText("断开");
    });
    connect(tcpClient,&MyTcpClient::disconnected,[=]{
//        qDebug()<<"与服务器失去连接了";
        ui->textBrowser->append(tr("与服务器失去连接了"));
        ui->pbConnect->setText("连接");
        tcpClient->close();
    });
    connect(tcpClient,&MyTcpClient::signal_revMsg,[=](QString msg){
        ui->textBrowser->append(msg);
    });

    getLocalIpList();

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::appendMsgToTextBrower(QString content)
{
    ui->textBrowser->append(content);
}
/**
 * @brief MainWindow::setDate
 * @param year
 * @param mon
 * @param day
 * @return
 */
bool MainWindow::setDate(int year, int mon, int day)
{
    SYSTEMTIME st;
//    GetLocalTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    st.wYear = year;
    st.wMonth = mon;
    st.wDay = day;
//    qDebug()<<st.wYear<<st.wMonth<<st.wDay;

    return SetLocalTime(&st);    //Win32 API 设置系统时间
}
/**
 * @brief MainWindow::setTime
 * @param hour
 * @param minute
 * @param second
 * @param milliseconds 毫秒0-999
 * @return
 */
bool MainWindow::setTime(int hour, int minute, int second,int milliseconds)
{
    SYSTEMTIME st;
//    GetLocalTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    st.wHour = hour;
    st.wMinute = minute;
    st.wSecond = second;
    st.wMilliseconds = milliseconds;
//    qDebug() <<st.wHour << st.wMinute <<st.wSecond << st.wMilliseconds;


    return SetLocalTime(&st);    //Win32 API 设置系统时间
}
/**
 * @brief MainWindow::getLocalIpList
 */
void MainWindow::getLocalIpList()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();

    ui->comboBox_serverIPList->clear();
    foreach (QHostAddress addr, list) {
        //        if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr.toString().contains("192.")){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol){
            ui->comboBox_serverIPList->addItem(addr.toString());
        }
    }
}


/**
 * @brief MainWindow::on_pb_startListen_clicked
 */
void MainWindow::on_pb_startListen_clicked()
{
    if(!tcpServer->isListening()){
      bool success = tcpServer->startListen(ui->lineEdit_listenPort->text().toInt());
      if(success){
          appendMsgToTextBrower("server is listening...");
          ui->pb_startListen->setText("停止");
      }
    }else{
        tcpServer->close();
        tcpServer->clearClientList();
        appendMsgToTextBrower("server is closed");
        ui->pb_startListen->setText("开启监听");
    }

}
/**
 * @brief MainWindow::on_pushButton_synchronous_clicked
 * 跟服务器连接并获取时间
 */
void MainWindow::on_pushButton_synchronous_clicked()
{
//    if(!tcpClient->isOpen()){
//        tcpClient->connectToHost(QHostAddress(ui->lineEdit_serverIP->text()),ui->lineEdit_serverPort->text().toInt());
//        return;
//    }
    setStartMilliSeconds(QDateTime::currentMSecsSinceEpoch());
    startQTime = QTime::currentTime();
    //请求连接
    tcpClient->write("R");

}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if(checked){
        ui->stackedWidget->setCurrentIndex(0);
    }else{
        ui->stackedWidget->setCurrentIndex(1);
    }
}

/**
 * @brief MainWindow::on_pb_connect_clicked
 * 连接服务器或者断开
 */
void MainWindow::on_pbConnect_clicked()
{
    if(tcpClient->isOpen()){
        tcpClient->disconnectFromHost();
    }else{
        ui->statusBar->showMessage("");

        tcpClient->connectToHost(QHostAddress(ui->lineEdit_serverIP->text()),ui->lineEdit_serverPort->text().toInt());
        tcpClient->waitForConnected(1000);
//        if(!tcpClient->isOpen()){
//           QMessageBox::warning(this,"tips",tcpClient->errorString());
//        }
        if(tcpClient->error() != QTcpSocket::UnknownSocketError){
            ui->statusBar->showMessage(tcpClient->errorString());
            tcpClient->close();
        }else{
            ui->statusBar->showMessage("");
        }


    }
}
/**
 * @brief MainWindow::on_pb_refreshIPCombobox_clicked
 * 重新获取IP地址
 */
void MainWindow::on_pb_refreshIPCombobox_clicked()
{
    getLocalIpList();
}

