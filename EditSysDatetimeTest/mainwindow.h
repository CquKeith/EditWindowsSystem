#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QDebug>
#include <QNetworkInterface>
#include <QException>
#include <QMessageBox>

#include "server.h"
//#include "tcpclientsocket.h"
#include "mytcpclient.h"

namespace Ui {
class MainWindow;
}

class Server;
//class TcpClientSocket;
class MyTcpClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    qint64 getStartMilliSeconds() const {return startMilliSeconds;}
    void setStartMilliSeconds(const qint64 &value){startMilliSeconds = value;}

    void appendMsgToTextBrower(QString content);

    QTime getStartQTime() const{return startQTime;}
    void setStartQTime(const QTime &value){startQTime = value;}

private slots:
    void on_pb_startListen_clicked();
    void on_pushButton_synchronous_clicked();

    void on_radioButton_toggled(bool checked);

    void on_pbConnect_clicked();

    void on_pb_refreshIPCombobox_clicked();

private:
    Ui::MainWindow *ui;
    Server * tcpServer;
    MyTcpClient * tcpClient;

    qint64 startMilliSeconds;

    QTime startQTime;

    bool setDate(int year,int mon,int day);
    bool setTime(int hour, int minute, int second, int milliseconds);
    void getLocalIpList();

};

#endif // MAINWINDOW_H
