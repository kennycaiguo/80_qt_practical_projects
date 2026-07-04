##  TCP网络调试助手

本次设计的网络调试助手简化界面UI的设计，重点在于网络通信的相关知识点，侧重服务器与客户端的创建以及服务器与客户端的数据交互，同时复习巩固之前UI控件

### 1.1 项目整体开发流程

![img](https://picx.zhimg.com/v2-53b06ddca602a7de7939d04573f00b63_1440w.jpg)

### 1.2 QTtcp服务器的关键流程

工程建立，需要在.pro加入网络权限

![img](https://pic3.zhimg.com/v2-6cc5e14261893939060082b90329f65c_1440w.jpg)

创建一个基于 `QTcpServer` 的服务端涉及以下关键步骤：

**1. 创建并初始化** **`QTcpServer`** **实例：**

- 实例化 `QTcpServer`
- 调用listen 方法在特定端口监听传入的连接

**2. 处理新连接：**

- 为 `newConnection` 信号连接一个槽函数。
- 在槽函数中，使用 `nextPendingConnection` 获取 `QTcpSocket` 以与客户端通信。

**3. 读取和发送数据：**

- 通过连接 `QTcpSocket` 的 `readyRead` 信号来读取来自客户端的数据。
- 使用 `write` 方法发送数据回客户端

**4. 关闭连接：**

- 在适当的时候关闭 `QTcpSocket`

示例代码可能如下：

```text
class MyServer : public QObject {
  Q_OBJECT
public:
	MyServer() 
	{
		QTcpServer *server = new QTcpServer(this);
		connect(server, &QTcpServer::newConnection, this, &MyServer::onNewConnection);
		server->listen(QHostAddress::Any, 1234);
 	}
private slots:
	void onNewConnection() 
	{
    	QTcpSocket *clientSocket = server->nextPendingConnection();
		connect(clientSocket, &QTcpSocket::readyRead, this, &MyServer::onReadyRead);
    	// ...
 	}
	void onReadyRead() 
	{
    	QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    	// 读取数据
    	QByteArray data = clientSocket->readAll();
    	// 处理数据
    	// ...
 	}
};
```

确保在使用 `QTcpServer` 和 `QTcpSocket` 时妥善处理网络错误和异常情况

### 1.3 QTtcp客户端的关键流程

工程建立，需要在.pro加入网络权限

![img](https://pic3.zhimg.com/v2-6cc5e14261893939060082b90329f65c_1440w.jpg)

创建一个基于 `QTcpSocket` 的Qt客户端涉及以下步骤：

**1. 创建** **`QTcpSocket`** **实例：**

- 实例化 `QTcpSocket`

**2. 连接到服务器：**

- 使用 `connectToHost` 方法连接到服务器的IP地址和端口。

**3. 发送数据到服务器：**

- 使用 `write` 方法发送数据。

**4. 接收来自服务器的数据：**

- 为 `readyRead` 信号连接一个槽函数来接收数据。

**5. 关闭连接：**

- 关闭 `QTcpSocket` 连接。

示例代码如下：

```text
class MyClient : public QObject {
	Q_OBJECT
public:
	MyClient() 
	{
    	QTcpSocket *client = new QTcpSocket(this);
    	connect(client , &QTcpSocket::readyRead, this, &MyClient::onReadyRead);
    	client->connectToHost("server_address", 1234);
 	}
private slots:
	void onReadyRead() 
	{
    	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    	QByteArray data = socket->readAll();
    	// 处理接收到的数据
    	// ...
 	}
};
```

这个客户端尝试连接到指定的服务器地址和端口，然后等待和处理来自服务器的数据。记得根据需要管理和处理网络错误和异常情况。

### 1.4 UI界面的设计

**TCP服务端UI界面设计：**

![img](https://pic4.zhimg.com/v2-6c826ecf4d77df542dd38e9680fe3955_1440w.jpg)

**TCP客户端UI界面设计：**

![img](https://pic4.zhimg.com/v2-a40759d183ae297bc651175c43f116eb_1440w.jpg)

### 1.5 [TCP协议](https://zhida.zhihu.com/search?content_id=259553601&content_type=Article&match_order=1&q=TCP协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyODQyNzcsInEiOiJUQ1DljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyNTk1NTM2MDEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.weK08ovQrIfd5M0jKsV8isfLJLXoLLQ4orqdqcgSY5Q&zhida_source=entity)理论知识

以下内容自行阅读和消化，主要在面试之前类似八股文问答，实际编程我们不需要关系这么多， QTcpSocket类底下的API已经做好所有的封装。

TCP（传输控制协议）是一种广泛使用的网络通信协议，设计用于在网络中的计算机之间可靠地传输数据。它是互联网协议套件的核心部分，通常与IP（互联网协议）一起使用，合称为TCP/IP。

**以下是TCP协议的一些基本特点：**

**1. 面向连接：** 在数据传输之前，TCP 需要在发送方和接收方之间建立一个连接。这包括[三次握手](https://zhida.zhihu.com/search?content_id=259553601&content_type=Article&match_order=1&q=三次握手&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyODQyNzcsInEiOiLkuInmrKHmj6HmiYsiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyNTk1NTM2MDEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.y-qE-ue_MPOMJ998CIZyKgRVAchCdN7GZBnV51_sXkw&zhida_source=entity)过程，确保两端都准备好进行数据传输。

**2. 可靠传输：** TCP 提供可靠的数据传输服务，这意味着它保证数据包准确无误地到达目的地。如果发生数据丢失或错误，TCP 会重新发送数据包。

**3. 顺序控制：**TCP 保证数据包的传输顺序。即使数据包在网络中的传输顺序被打乱，接收方也能按照正确的顺重组这些数据。

**4. 流量控制：**TCP 使用窗口机制来控制发送方的数据传输速率，以防止网络过载。这有助于防止接收方被发送方发送的数据所淹没。

**5. 拥塞控制：**TCP 还包括拥塞控制机制，用来检测并防止网络拥塞。当网络拥塞发生时，TCP 会减少其数据传输速率。

**6. 数据分段：**大块的数据在发送前会被分割成更小的段，以便于传输。这些段会被独立发送并在接收端重新组装。

**7. 确认和重传：**接收方对成功接收的数据包发送确认（ACK）信号。如果发送方没有收到确认，它会重传丢失的数据包。

**8. 终止连接：**数据传输完成后，TCP 连接需要被正常关闭，这通常涉及到[四次挥手](https://zhida.zhihu.com/search?content_id=259553601&content_type=Article&match_order=1&q=四次挥手&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyODQyNzcsInEiOiLlm5vmrKHmjKXmiYsiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyNTk1NTM2MDEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.a4rIyhfVAqxt6oMIvjuIt3lpJesujlsvsS4ScmcJMYw&zhida_source=entity)过程。

TCP 适用于需要高可靠性的应用，如网页浏览、文件传输、电子邮件等。然而，由于它的这些特性，TCP在处理速度上可能不如其他协议（如UDP）那么快速。TCP协议中的三次握手和四次挥手是建立和终止连接的重要过程。

**三次握手（建立连接）**

三次握手的主要目的是在两台设备之间建立一个可靠的连接。它包括以下步骤：

**1. SYN：**客户端向服务器发送一个SYN（同步序列编号）报文来开始一个新的连接。此时，客户端进入SYN-SENT状态。

**2. SYN-ACK：**服务器接收到SYN报文后，回复一个SYN-ACK（同步和确认）报文。此时服务器进入SYN-RECEIVED状态。

**3. ACK：**客户端接收到SYN-ACK后，发送一个ACK（确认）报文作为回应，并进入ESTABLISHED（已建立）状态。服务器在收到这个ACK报文后，也进入ESTABLISHED状态。这标志着连接已经建立。

![img](https://pic3.zhimg.com/v2-33b6badc0eef3f06603c4cea06749424_1440w.jpg)

**四次挥手（断开连接）**

四次挥手的目的是终止已经建立的连接。这个过程包括以下步骤：

**1. FIN：**当通信的一方完成数据发送任务后，它会发送一个FIN（结束）报文来关闭连接。发送完FIN报文后，该方进入FIN-WAIT-1状态。

**2. ACK：** 另一方接收到FIN报文后，发送一个ACK报文作为回应，并进入CLOSE-WAIT状态。发送FIN报文的一方在收到ACK后，进入FIN-WAIT-2状态。

**3. FIN：** 在等待一段时间并完成所有数据的发送后，CLOSE-WAIT状态的一方也发送一个FIN报文来请求关闭连接。

**4. ACK：**最初发送FIN报文的一方在收到这个FIN报文后，发送一个ACK报文作为最后的确认，并进入TIME-WAIT状态。经过一段时间后，确保对方接收到了最后的ACK报文，该方最终关闭连接。

![img](https://pic3.zhimg.com/v2-273f0197c03cd88ec3713110bdcdaa56_1440w.jpg)

在这两个过程中，三次握手主要确保双方都准备好进行通信，而四次挥手则确保双方都已经完成通信并同意关闭连接。

### 1.6 Socket网络通信理论

Socket 不是一个协议，而是一种编程接口（API）或机制，用于在网络中实现通信。Socket 通常在应用层和传输层之间提供一个端点，使得应用程序可以通过网络发送和接收数据。它支持多种协议，主要是TCP 和 UDP。

**以下是 Socket 的一些基本特点：**

- **类型：** 有两种主要类型的 Sockets —— TCP Socket（面向连接，可靠）和 UDP Socket（无连接，不可靠）。
- **应用：** 在各种网络应用中广泛使用，如网页服务器、聊天应用、在线游戏等。
- **编程语言支持：** 大多数现代编程语言如 Python, Java, C++, 等都提供 Socket 编程的支持。
- **功能：** 提供了创建网络连接、监听传入的连接、发送和接收数据等功能。
- **QT：** 在QT组件中，QTcpSocket用来管理和实现TCP Socket通信，QUdpSocket用来管理和实现UDP Socket通信

总之，Socket 是实现网络通信的基础工具之一，它抽象化了网络层的复杂性，为开发者提供了一种相对简单的方式来建立和管理网络连接

如果正在学习QT开发的同学，可以看下面这个教程，从基础入门到多个项目实战的讲解

[C++ Qt开发入门到实战项目教程（项目含源码）www.bilibili.com/video/BV1Fyw3e8EFN/](https://link.zhihu.com/?target=https%3A//www.bilibili.com/video/BV1Fyw3e8EFN/)

## 2 网络通信核心代码

`QTcpServer`是 Qt 网络模块的一部分，用于构建 TCP 服务器。它提供了一种机制来异步监听来自客户端的连接。一旦接受了一个连接，服务器就可以与客户端进行数据交换。

### 2.1 TCP服务端连接的核心代码

在类定义时，将服务器对象定义为成员变量，供槽函数调用，在构造函数中对服务器对象进行实例化。

```text
//类定义
QTcpServer *server;

//构造函数内
//1. 创建服务器对象
server = new QTcpServer(this);
```

**启动监听槽函数**

在监听槽函数中，启动监听，在有新的客户端接入时会发出`newConnection`信号，因此通过绑定信号与槽可检测客户端接入。

```text
//监听按钮槽函数 启动监听
void Widget::on_btnListen_clicked()
{
    //设置服务器IP地址和端口号变量 启动监听
    QHostAddress addr("192.168.1.106");
    quint16 port = 8888;

    //2. 启动监听
    bool ret = server->listen(addr, port); //自动获取IP地址
    if(ret == false)
        return;

    //绑定监听客户端接入的信号与槽函数
    connect(server, SIGNAL(newConnection()), this, SLOT(on_newClient_connect()));
}
```

**客户端成功接入槽函数**

在有客户端成功接入时，通过`nextPendingConnection`方法获取客户端对象，将客户端的IP地址与端口号分别打印在数据接收框内，同时绑定数据接收函数（当有数据达到时，会触发`readyRead()`信号）

```text
//检测客户端接入槽函数 对应newConnection信号
void Widget::on_newClient_connect()
{
    //判断是否有新的客户端接入
    if(server->hasPendingConnections())
    {
        //获取客户端对象
        QTcpSocket *tcpSocket = server->nextPendingConnection();

        //打印客户端IP地址与端口号
        qDebug() << "client addr: " << tcpSocket->peerAddress().toString();
        qDebug() << "client port: " << tcpSocket->peerPort();


        //接收消息框显示客户端IP地址与端口号
        ui->textEdit_Rev->append("addr: " + tcpSocket->peerAddress().toString());
        ui->textEdit_Rev->append("port: " +QString::number(tcpSocket->peerPort()));

        //绑定接收readyRead信号与槽函数
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead_handler()));
        
        //将新接入的客户端端口加入选项框
        ui->comboBox_child->addItem(QString::number(tcpSocket->peerPort()));
        ui->comboBox_child->setCurrentText(QString::number(tcpSocket->peerPort()));
    }
}
```

### 2.2 TCP服务端的数据通信核心代码

在上述绑定信号接收槽函数后，当接收到数据后，会自动触发数据接收槽函数对数据进行处理，其中**数据接收槽函数**如下：

```text
//接收数据槽函数
void Widget::on_readyRead_handler()
{
	//因为数据由客户端的QTcpSocket进行发送，以此需要通过 `qobject_cast<>(sender())` 来获取信号发送方
    //模仿 串口调试助手中的on_command_button_clicked 获取信号发送者
    QTcpSocket *tcpSocket = qobject_cast<QTcpSocket *>(sender());
    //接收数据
    QByteArray revData = tcpSocket->readAll();
    //将接收到的数据显示到文本框
    ui->textEdit_Rev->append("client: " + revData);
}
```

**数据发送按钮槽函数**

```text
void Widget::on_btnSend_clicked()
{
    //通过server的findChildren方法获取客户端对象
    QList<QTcpSocket *> clients = server->findChildren<QTcpSocket*>();

    //遍历所有子客户端，调用write方法像所有客户端发送消息
    for(QTcpSocket *temp: clients)
    {
        temp->write(ui->textEdit_Send->toPlainText().toStdString().c_str());
    }
}
```

### 2.3 TCP客户端的核心代码

客户端的设计与服务端类似，定义客户端对象并在构造函数内实例化对象

```text
QTcpSocket *client;

//实例化客户端对象
client = new QTcpSocket(this);
```

**连接按钮槽函数**

在连接按钮槽函数中主要利用`connectToHost`方法将客户端与服务器建立连接，同时绑定`readyRead()`信号与数据接收槽函数

```text
//连接按钮槽函数
void Widget::on_btnConnect_clicked()
{
    //获取行编辑器内IP地址与端口号
    QString addr(ui->lineEdit_addr->text());
    quint16 port = ui->lineEdit_port->text().toInt();

    //请求连接服务器
    client->connectToHost(addr, port);

    //绑定接收readyRead信号与槽函数
    connect(client, SIGNAL(readyRead()), this, SLOT(on_readyRead_handler()));
}
```

**数据接收槽函数**

```text
//接收数据槽函数
void Widget::on_readyRead_handler()
{
    //获取数据 并添加至文本框
    QByteArray revData = client->readAll();
    ui->textEdit_rev->append("server: " + revData);
}
```

**数据发送按钮槽函数**

```text
void Widget::on_btnSend_clicked()
{
    //发送文本框内数据
    QByteArray sendData = ui->textEdit_send->toPlainText().toUtf8();
    client->write(sendData);
}
```

## 3 TCP服务端项目功能优化

### 3.1 自动刷新IP地址

为了程序自主检测系统上可用的IP地址，为了实现自主刷新可用IP地址并添加至选项框，在构造函数中采用`QNetworkInterface`下的`allAddresses()`来检测可用的IP地址，并添加至选项框，同时在启动监听时读取选项框当前IP地址数据即可。

```text
//自动扫描IP地址 存放在列表中
QList<QHostAddress> addrList = QNetworkInterface::allAddresses();
for(QHostAddress addr :  addrList)
{
//IP地址协议为IPv4协议
    if(addr.protocol() == QAbstractSocket::IPv4Protocol)
    {
//将网络添加至选项框
        ui->comboBox_Addr->addItem(addr.toString());
    }
}
```

### 3.2 服务器向不同客户端发数据

通过自定义控件、左键点击选项框触发事件获取当前接口的客户端的端口号，并添置至选项框，最后通过不同的端口号向不同的客户端发送数据。自定义myComboBox类，并继承于QComboBox，同时在UI界面将IP地址选项框提升为自定义控件

**1. 自定义类自定义继承实现：**

![img](https://pica.zhimg.com/v2-83482cba0c9bac2d6e0e9803610f472c_1440w.jpg)

![img](https://pic1.zhimg.com/v2-4f94073b616b1260859b5009a3ac8e56_1440w.jpg)

**2. 重写鼠标左键事件**，触发事件发生自定义信号，最后将事件重新传回原路径执行

```text
//声明
protected:
    void mousePressEvent(QMouseEvent *e) override;
signals:
    void on_ComboBox_clicked();
```

**槽函数的实现：**

```text
void myComboBox::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit on_ComboBox_clicked();
    QComboBox::mousePressEvent(e);
}
```

**3. 绑定信号与槽**，该自定义信号供主页面程序检测，若检测到该信号则跳转到相应的槽函数进行端口号的刷新。

```text
//绑定信号与槽，检测comboBox按下，扫描已接入的客户端
connect(ui->comboBox_child, &myComboBox::on_ComboBox_clicked, this, &Widget::on_refresh_comboBox);
```

选项框按下槽函数：刷新不同客户端的端口号

```text
//刷新不同客户端接入的端口号
void Widget::on_refresh_comboBox()
{
    ui->comboBox_child->clear();
    //通过server获取客户单对象
    QList<QTcpSocket *> clients = server->findChildren<QTcpSocket*>();
    for(QTcpSocket *temp: clients)
    {
        //客户端不为空 且端口号不为0 加入选项框
        ui->comboBox_child->addItem(QString::number(temp->peerPort()));
    }
    ui->comboBox_child->addItem("all");
}
```

**4. UI界面的控件提升：**

![img](https://pica.zhimg.com/v2-c65be0358350816a19886c7049edfb58_1440w.jpg)

**5. 优化发送函数**，根据个人选项，向不同的客户端发送数据，根据用户选择 找出具体客户端进行数据通信, 通过端口查找下标确定

```text
//数据发送槽函数
void Widget::on_btnSend_clicked()
{
    //通过server获取客户单对象
    QList<QTcpSocket *> clients = server->findChildren<QTcpSocket*>();

    if(clients.isEmpty())
        return;


    //选择特定端口发送
    if(ui->comboBox_child->currentText() != "all")
    {
        //根据用户选择 找出具体客户端进行数据通信, 通过端口查找下标确定
        QString currentName = ui->comboBox_child->currentText();
        for(QTcpSocket *temp : clients)
        {
            if(QString::number(temp->peerPort()) == currentName)
            {
                temp->write(ui->textEdit_Send->toPlainText().toStdString().c_str());
            }
        }
    }
    //所有端口发送
    else
    {
        //遍历所有子客户端，调用write方法像所有客户端发送消息
        for(QTcpSocket *temp: clients)
        {
            temp->write(ui->textEdit_Send->toPlainText().toStdString().c_str());
        }
    }

    //光标移至末尾
    ui->textEdit_Rev->moveCursor(QTextCursor::End);
    ui->textEdit_Rev->ensureCursorVisible();
}
```

### 3.3 TextEdit设置特定位置文字颜色

为了设置 textEdit 中特定位置的文字颜色，需要光标级别的设置。通过 `textCursor()` 方法来获取当前的光标位置， 继而通过 cursor 下的 `setCharFormat` 来实现字符格式。

其中设置的函数原型与嵌套关系如下：

```text
QTextCursor:		QTextEdit::textCursor() const
QTextCursor:      	void QTextCursor::setCharFormat(const QTextCharFormat &format)  //方法
QTextCharFormat:	void setForeground(const QBrush &brush)                         //方法
QBrush:           	QBrush(const QColor &color, const QPixmap &pixmap)              //构造函数
QColor:           	QColor(const QColor &color)   									//构造函数
```

将设置特定位置文本颜色功能封装成函数，其参数分别为字体颜色和待显示的文本

```text
void Widget::setInsertColor(Qt::GlobalColor color, QString str)
{
    //获取当前光标位置
    QTextCursor cursor = ui->textEdit_rev->textCursor();
    QTextCharFormat format;
    
	//配置颜色
    format.setForeground(QColor(color));
    cursor.setCharFormat(format);
    cursor.insertText(str + "\n");
}
```

### 3.4 客户端断开检测

当客户端主动断开后，服务器会收到一个 `disconnected()` 信号，通过绑定信号与槽实现对客户端断开连接的检测

```text
//绑定disconnect断开连接信号
connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
```

**客户端断开连接槽函数：**检测到客户端断开后，主要在文本框内进行提示，在端口选项框中移除该客户端的端口号，并将客户端进行删除，

```text
//客户端断开连接信号槽函数
void Widget::on_disconnected()
{
	//通过 qobject_cast 获取信号发送者
    QTcpSocket *tcpSocket = qobject_cast<QTcpSocket *>(sender());
    ui->textEdit_Rev->append("client quit!");

    //通过内容找到选项框索引，并在选项框中移除
    int tempIdx;
    tempIdx = ui->comboBox_child->findText(QString::number(tcpSocket->peerPort()));
    ui->comboBox_child->removeItem(tempIdx);

    //删除客户端
    tcpSocket->deleteLater();

    //判断是否还存在连接着的客户端
    if(server->findChildren<QTcpSocket*>().count() == 0)
        ui->btnSend->setEnabled(false);
}
```

### 3.5 停止监听的实现

停止监听槽函数主要通过服务器的 `findChildren<>()`查找所有接入的客户端并遍历删除即可，同时关闭服务器

```text
//停止监听槽函数
void Widget::on_btnStopListen_clicked()
{
    //获取所有客户端
    QList<QTcpSocket *> clients = server->findChildren<QTcpSocket*>();
    for(QTcpSocket *temp: clients)
    {
        temp->close();
    }
    server->close();
}
```

## 4 TCP客户端项目开发及优化

### 4.1 检测连接状态

客户端在成功接入服务器后会发出 `connected()` 信号，接入失败会发出 `error()` 信号。但实际接入失败发出 `error()` 信号需要一定的时间，因此通过定时器计算接入的时间，若接入时间大于3秒则认为连接超时。

**构造函数内定时器的设置**

```text
//设置定时器 检测连接超时
timer = new QTimer(this);
timer->setSingleShot(true); //触发一次
timer->setInterval(3000);   //设置连接超时时间
//启动定时器
timer->start();
```

**绑定信号与槽**

```text
//成功连接发出connected信号
connect(client, SIGNAL(connected()), this, SLOT(on_connected()));
//绑定定时器溢出槽函数
connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_out()));
```

### 成功接入

连接成功进入 `connected()` 信号的槽函数

```text
void Widget::on_connected()
{
    //连接成功 关闭定时器
    timer->stop();
	
	//文本框提示连接成功
    ui->textEdit_rev->append("连接成功");

    //设置控件属性
    ui->btnDisconnect->setEnabled(true);
    ui->btnSend->setEnabled(true);
    ui->lineEdit_addr->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);
    ui->btnConnect->setEnabled(false);
    this->setEnabled(true);
    
    //光标移至末尾
    ui->textEdit_rev->moveCursor(QTextCursor::End);
    ui->textEdit_rev->ensureCursorVisible();
}
```

### 连接超时

当定时器超过3秒溢出后则认为是连接超时（失败），中止本次连接

```text
void Widget::on_timer_out()
{
    ui->textEdit_rev->append("连接超时");
    client->abort();			//中止客户端连接
    this->setEnabled(true);
    on_btnDisconnect_clicked(); //手动调用断开连接
}
```

### 4.2 其他细节功能

**文本框特定颜色分区**：同TCP服务器功能的3.3

```text
//设置插入数据的颜色
void Widget::setInsertColor(Qt::GlobalColor color, QString str)
{
    //获取当前光标位置
    QTextCursor cursor = ui->textEdit_rev->textCursor();
    QTextCharFormat format;
	
	//配置文本框颜色
    format.setForeground(QColor(color));
    cursor.setCharFormat(format);
    cursor.insertText(str + "\n");
}
```

**设置控件失能与使能**：利用`setEnabled`方法可设置控件的有效性，其中参数为`true`时表使能控件，`false`失能控件

```text
//设置控件属性
ui->btnDisconnect->setEnabled(false);
ui->btnSend->setEnabled(false);
```

**文本框显示最后一行**（即最新的内容）

```text
//光标移至末尾
ui->textEdit_rev->moveCursor(QTextCursor::End);
ui->textEdit_rev->ensureCursorVisible();
```

## 5 项目总结

- TCPServer类关于监听，连接，发送，接收的API
- TCPServer在网络通信中常用的信号
- TCPScoket在QT实现Socket常用的API
- TCPScoket在QT实现Socket常用的信号
- EditText的内容读取方法，内容写入方法，在特定行写入特点颜色的方法

`TCPServer` 、 `TCPScoket` 和 `EditText` 的信息整合到一个表格中：

| 类别          | 功能                 | API方法                                                 | 描述                                                         |
| ------------- | -------------------- | ------------------------------------------------------- | ------------------------------------------------------------ |
| TCPServer     | 监听                 | bool listen(const QHostAddress &address, quint16 port ) | 在指定的 IP 地址和端口上开始监听传入的连接                   |
|               | 连接                 | void close()                                            | 停止服务器监听传入的连接                                     |
|               |                      | QTcpSocket *nextPendingConnection()                     | 返回下一个待处理连接的QTcpSocket 指针                        |
| TCPServer信号 |                      | newConnection()                                         | 当有新连接时发出                                             |
| TCPScoket     | 连接                 | void connectToHost(const QString &host, quint16 port)   | 连接到指定的主机和端口                                       |
|               | 发送                 | qint64 write(const QByteArray &data)                    | 向连接的套接字写入数据                                       |
|               | 接收                 | QByteArray readAll()                                    | 读取可用的所有数据                                           |
|               | 断开                 | void disconnectFromHost()                               | 断开与主机的连接                                             |
| TCPScoket信号 |                      | connected()                                             | 成功连接到主机时发出                                         |
|               |                      | disconnected()                                          | 从主机断开连接时发出                                         |
|               |                      | readyRead()                                             | 当有可读数据时发出                                           |
|               |                      | bytesWritten(qint64 bytes)                              | 成功写入数据时发出                                           |
| EditText      | 读取内容             | String getText()                                        | 获取 EditText 的内容                                         |
|               | 写入内容             | void setText(String text)                               | 设置 EditText 的内容                                         |
|               | 使用光标改变文本颜色 | void changeTextColor(int start, int end, int color)     | 使用光标（cursor）改变EditText 中从 start 位置到 end 位置的文本颜色为color |