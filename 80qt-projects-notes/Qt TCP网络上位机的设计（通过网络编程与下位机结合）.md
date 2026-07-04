### TCP 协议基础

传输控制协议（TCP，Transmission Control Protocol）是一种面向连接的、可靠的、基于字节流的传输层通信协议



TCP 的拥塞控制算法（也称 AIMD 算法）。该算法主要包括四个主要部分：慢启动、拥塞避免、快速重传和快速恢复



TCP 通信必须建立 TCP 连接（客户端和服务器端），Qt 提供 QTcpSocket 类和 QTcpServer 类专门用于建立 TCP 通信程序。服务 器端用 QTcpServer 监听端口及建立服务器；QTcpSocket 用于建立 连接后使用套接字(socket)进行通信



### QTcpServer 和 ***\*QAbstractSocket\**** 主要接口函数

QTcpServer 是从 QOjbect 继承的类用于服务器建立网络监听， 创建网络 socket 连接。QTcpServer 主要接口函数如下：





![image](https://ucc.alicdn.com/pic/developer-ecology/cx4tawbjd3vhy_b312da350097434c8e40c77ac66e7c27.png)

![image](https://ucc.alicdn.com/pic/developer-ecology/cx4tawbjd3vhy_833f39bc80d44a47a65dddc8ae21b88d.png)





QAbstractSocket主要接口函数如下：



![1c354ab98d6ef2f506500bcf8683fc93_bf329e5e5ecd4d83bda3d9cf2732b888.png](https://ucc.alicdn.com/pic/developer-ecology/cx4tawbjd3vhy_b1dcd3404c284a029cbd9a1d99254096.png)



### TCP 应用程序

1.服务端

UI绘图：

![5c2bfdebe0f59a4732bdc942301b1d0b_a3a8c0c7d9dd4f1d9f63cfa573a408f4.png](https://ucc.alicdn.com/pic/developer-ecology/cx4tawbjd3vhy_17ae165beadb43e7b1b80867db77e15d.png)

代码示例：

mainwindow.h

```
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <QMainWindow>
 
#include <QTcpServer> // 专门用于建立TCP连接并传输数据信息
#include <QtNetwork> // 此模块提供开发TCP/IP客户端和服务器的类
 
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
 
class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
 
private:
    Ui::MainWindow *ui;
 
 
    // 自定义如下
private:
    QTcpServer *tcpserver; //TCP服务器
    QTcpSocket *tcpsocket;// TCP通讯socket
    QString GetLocalIpAddress(); // 获取本机的IP地址
 
private slots:
    void clientconnect();
    void clientdisconnect();
    void socketreaddata();
    void newconnection();
 
 
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();
    void on_pushButton_Send_clicked();
 
protected:
    void closeEvent(QCloseEvent *event);
 
};
#endif // MAINWINDOW_H
```

main.cpp

```
#include "mainwindow.h"
 
#include <QApplication>
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
```

mainwindow.cpp

```
#include "mainwindow.h"
#include "ui_mainwindow.h"
 
#include <QMessageBox>
 
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 
    QString strip=GetLocalIpAddress();
    // QMessageBox::information(this,"数据",strip,QMessageBox::Yes);
 
    ui->comboBoxIp->addItem(strip);
 
 
    tcpserver=new QTcpServer(this);
 
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(newconnection()));
 
}
 
MainWindow::~MainWindow()
{
    delete ui;
}
 
 
void MainWindow::on_pushButton_Start_clicked()
{
    QString ip=ui->comboBoxIp->currentText();
    quint16 port=ui->spinBoxPort->value();
 
    QHostAddress address(ip);
    tcpserver->listen(address,port);
 
    ui->plainTextEdit_DispMsg->appendPlainText("$$$$$$$$$$开始监听$$$$$$$$$$");
    ui->plainTextEdit_DispMsg->appendPlainText("$$$$$$$$$$服务器地址$$$$$$$$$$："+
                                               tcpserver->serverAddress().toString());
    ui->plainTextEdit_DispMsg->appendPlainText("$$$$$$$$$$服务器端口$$$$$$$$$$："+
                                               QString::number(tcpserver->serverPort()));
    ui->pushButton_Start->setEnabled(false);
    ui->pushButton_Stop->setEnabled(true);
 
}
 
void MainWindow::on_pushButton_Stop_clicked()
{
    if(tcpserver->isListening())
    {
        tcpserver->close();
        ui->pushButton_Start->setEnabled(true);
        ui->pushButton_Stop->setEnabled(false);
    }
 
}
 
void MainWindow::on_pushButton_Send_clicked()
{
    QString strmsg=ui->lineEdit_InputMsg->text();
    ui->plainTextEdit_DispMsg->appendPlainText("[out]:"+strmsg);
 
    ui->lineEdit_InputMsg->clear();
 
    QByteArray str=strmsg.toUtf8();
    str.append("\n");
    tcpsocket->write(str);
}
 
 
QString MainWindow::GetLocalIpAddress() // 获取本机的IP地址
{
    QString hostname=QHostInfo::localHostName();
    QHostInfo hostinfo=QHostInfo::fromName(hostname);
 
    QString localip="";
 
    QList<QHostAddress> addresslist=hostinfo.addresses();
 
    if(!addresslist.isEmpty())
    {
        for (int i=0;i<addresslist.count();i++)
        {
            QHostAddress addrhost=addresslist.at(i);
            if(QAbstractSocket::IPv4Protocol==addrhost.protocol())
            {
                localip=addrhost.toString();
                break;
            }
 
        }
    }
 
    return localip;
}
 
void MainWindow::clientconnect()
{
    // 客户端连接
    ui->plainTextEdit_DispMsg->appendPlainText("**********客户端socket连接**********");
    ui->plainTextEdit_DispMsg->appendPlainText("**********peer address:"+
                                               tcpsocket->peerAddress().toString());
    ui->plainTextEdit_DispMsg->appendPlainText("**********peer port:"+
                                               QString::number(tcpsocket->peerPort()));
 
}
 
void MainWindow::clientdisconnect()
{
    // 客户端断开连接
    ui->plainTextEdit_DispMsg->appendPlainText("**********客户端socket断开连接**********");
    tcpsocket->deleteLater();
 
}
 
void MainWindow::socketreaddata()
{
    // 读取数据
    while(tcpsocket->canReadLine())
        ui->plainTextEdit_DispMsg->appendPlainText("[in]"+tcpsocket->readLine());
 
}
 
void MainWindow::newconnection()
{
    tcpsocket=tcpserver->nextPendingConnection();
 
    connect(tcpsocket,SIGNAL(connected()),this,SLOT(clientconnect()));
    clientconnect();
 
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(clientdisconnect()));
 
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(socketreaddata()));
 
    connect(tcpsocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(OnSocketStateChanged(QAbstractSocket::SocketState)));
 
 
}
 
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(tcpserver->isListening())
        tcpserver->close();
 
    event->accept();
}
```

2.客户端

UI绘图：

![image](https://ucc.alicdn.com/pic/developer-ecology/cx4tawbjd3vhy_381f4e5981a747b19abf01c69ca0c18a.png)

mainwindow.h

```
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <QMainWindow>
 
 
#include <QTcpSocket>
#include <QHostAddress>
#include <QHostInfo>
 
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
 
class MainWindow : public QMainWindow
{
    Q_OBJECT
 
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
 
private:
    Ui::MainWindow *ui;
 
 
private:
    QTcpSocket *tcpclient; // 客户端tcpclient
    QString getlocalip(); // 获取本机IP地址
 
protected:
    void closeEvent(QCloseEvent *event);
 
private slots:
    void connectfunc();
    void disconnectfunc();
    void socketreaddata();
 
 
 
 
 
    void on_pushButton_Connect_clicked();
    void on_pushButton_Send_clicked();
    void on_pushButton_Disconnect_clicked();
};
#endif // MAINWINDOW_H
```

main.cpp

```
#include "mainwindow.h"
 
#include <QApplication>
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
```

mainwindow.cpp

```
#include "mainwindow.h"
#include "ui_mainwindow.h"
 
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
 
    tcpclient=new QTcpSocket(this);
 
    QString strip=getlocalip();
 
    ui->comboBoxIp->addItem(strip);
 
 
    connect(tcpclient,SIGNAL(connected()),this,SLOT(connectfunc()));
    connect(tcpclient,SIGNAL(disconnected()),this,SLOT(disconnectfunc()));
    connect(tcpclient,SIGNAL(readyRead()),this,SLOT(socketreaddata()));
 
 
}
 
MainWindow::~MainWindow()
{
    delete ui;
}
 
 
void MainWindow::on_pushButton_Connect_clicked()
{
    QString addr=ui->comboBoxIp->currentText();
    quint16 port=ui->spinBoxPort->value();
    tcpclient->connectToHost(addr,port);
}
 
void MainWindow::on_pushButton_Send_clicked()
{
    QString strmsg=ui->lineEdit_InputMsg->text();
    ui->plainTextEdit_DispMsg->appendPlainText("[out]:"+strmsg);
    ui->lineEdit_InputMsg->clear();
 
    QByteArray str=strmsg.toUtf8();
    str.append('\n');
    tcpclient->write(str);
 
}
 
 
void MainWindow::on_pushButton_Disconnect_clicked()
{
    if(tcpclient->state()==QAbstractSocket::ConnectedState)
        tcpclient->disconnectFromHost();
}
 
 
 
 
QString MainWindow::getlocalip() // 获取本机IP地址
{
    QString hostname=QHostInfo::localHostName();
    QHostInfo hostinfo=QHostInfo::fromName(hostname);
 
    QString localip="";
 
    QList<QHostAddress> addlist=hostinfo.addresses();
    if(!addlist.isEmpty())
    {
        for (int i=0;i<addlist.count();i++)
        {
            QHostAddress ahost=addlist.at(i);
            if(QAbstractSocket::IPv4Protocol==ahost.protocol())
            {
                localip=ahost.toString();
                break;
            }
        }
    }
 
    return localip;
}
 
 
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(tcpclient->state()==QAbstractSocket::ConnectedState)
    {
        tcpclient->disconnectFromHost();
    }
    event->accept();
 
}
 
 
void MainWindow::connectfunc()
{
    ui->plainTextEdit_DispMsg->appendPlainText("**********已经连接到服务器端**********");
    ui->plainTextEdit_DispMsg->appendPlainText("**********peer address:"+
                                               tcpclient->peerAddress().toString());
    ui->plainTextEdit_DispMsg->appendPlainText("**********peer port:"+
                                               QString::number(tcpclient->peerPort()));
 
    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_Disconnect->setEnabled(true);
 
}
void MainWindow::disconnectfunc()
{
    ui->plainTextEdit_DispMsg->appendPlainText("**********已断开与服务器端的连接**********");
 
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_Disconnect->setEnabled(false);
 
}
void MainWindow::socketreaddata()
{
    while(tcpclient->canReadLine())
        ui->plainTextEdit_DispMsg->appendPlainText("[in]:"+tcpclient->readLine());
 
}
```

### 上位机通过网络编程与下位机实现通信

**Qt作为上位机**

**51、32单片机或ARM开发板开作为下位机**

**通过网络编程实现通信**

**上位机可作为服务端或客户端，下位机也可作为服务端或客户端，具体按各自的项目需求实现**

![image](https://ucc.alicdn.com/pic/developer-ecology/cx4tawbjd3vhy_b97545fcbf694d88a0f2ebc7c292f220.png)