##  仿QQ多人聊天室

###  login.cpp

**窗口设置**

```cpp
this->setWindowIcon(QIcon(":/images/qq.png"));
this->setWindowTitle("QQ");
```

这里的路径与以前写的不同，原因是在 Qt 中，以 `:/` 开头的路径表示的是资源文件系统中的路径，而 `./` 则表示普通的文件系统相对路径 



**初始化用户列表**

```cpp
QVector<QToolButton*> vector;   //存放按钮的容器
//用户名列表
QStringList list;
list<<"张三"<<"李四"<<"王五"<<"xrg"<<"cdq"<<"dyt"<<"mgj"<<"lxc"<<"csm";
//图片资源列表
QStringList listIcon;
listIcon<<"spqy"<<"ymrl"<<"qq"<<"xrg"<<"cdq"<<"dyt"<<"mgj"<<"lxc"<<"spqy";

//循环创建按钮对象及其对应姓名
for(int i=0;i<9;i++){
    QToolButton *btn=new QToolButton(this);	//父对象为this 保证了资源释放
    //设置按钮图标
    btn->setIcon(QPixmap(QString(":/images/%1.png").arg(listIcon[i])));
    //设置图标大小为对应图片的大小
    btn->setIconSize(QPixmap(QString(":/images/%1.png").arg(listIcon[i])).size());
    //设置按钮为透明效果（自动凸起）
    btn->setAutoRaise(true);
    //设置网名
    btn->setText(QString("%1").arg(list[i]));
    //设置按钮的显示格式，使文本显示在图标旁边 貌似只有这一种显示方式。。。
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //将按钮添加到垂直布局管理器ui->vlayout中
    ui->vlayout->addWidget(btn);
    vector.push_back(btn);   //每创建一个btn就加入到vector中
    isShow.push_back(false);
}
```

- `QVector<QToolButton*>`用来管理`QToolButton*`，以便后续展示用户名列表
- `QStringList`在Qt用来存储字符串列表的，用途相当于`vector<string>`
- `btn->setIcon(QPixmap(QString(":/images/%1.png").arg(listIcon[i])));`首先调用QString中的arg函数，将listIcon[i]替换%1，例如listIcon[i]"QQ"，那么资源路径就替换成了":/images/QQ.png"，然后通过QPixmap类来加载这个路径下对应的资源，这一行大致可以用作模板
- `btn->setAutoRaise(true);`设置按钮为透明效果（自动凸起）
- `btn->setText(QString("%1").arg(list[i]));`和设置按钮图标大致类似，只不过就是不需要通过`QPixmap`来加载图片资源，直接通过得到的字符串传给`setText`函数即可
- `btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);`让字体显示在图片旁边



**展示用户列表**

```cpp
for(int i=0;i<9;i++){
    connect(vector[i],&QToolButton::clicked,[=](){
        if(true==isShow[i]){
            QMessageBox::warning(this,"警告","该聊天框已被打开");
            return;
        }
        isShow[i]=true;
        //打开对应用户的聊天窗口
        Widget *widget=new Widget(0,vector[i]->text());
        widget->setWindowIcon(vector[i]->icon());
        widget->setWindowTitle(vector[i]->text());
        widget->show();
        //关闭聊天框时将对于的isShow跟改为false
        connect(widget,&Widget::closeWidget,[=](){
            isShow[i]=false;
        });
    });
}
```

主要是信号与槽的应用，监控了按钮的点击事件和聊天框的关闭事件，点开的时候isShow[i]变为true，关闭的时候变为false，这里用循环的目的是动态管理多个聊天框



### widget.cpp

- `udpSocket->bind(port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);` 主要作用是将 `udpSocket` 绑定到指定端口，并通过设置 `ShareAddress` 和 `ReuseAddressHint` 选项，实现地址共享和端口重用功能 



**sendMsg**

```cpp
void Widget::sendMsg(Widget::msgType type)
{
    //用于存储字节数组的类
    QByteArray array;
    //将数据转化成字节流
    QDataStream stream(&array,QIODevice::WriteOnly);    //与array关联 只写
    //将数据类型和姓名存入到数据流中
    stream << type << this->getName();	//符号记住是左移符号
    //判断是哪种消息类型
    switch (type)
    {
    case Msg:
        //如果输入框的内容为空 那么就会弹出警告
        if(ui->msgTxtEdit->toPlainText()=="")
        {
            QMessageBox::warning(this,"警告","发送的聊天内容不能为空!");
            return;
        }
        //有输入内容 将输入的内容读入到数据流中
        stream << this->getMsg();
        break;
    case UserEnter:
        break;
    case UserLeft:
        break;
    }
    //书写报文
    //qDebug() << array; 输出的是个乱七八糟的东西 我咋debug。。。
    udpSocket->writeDatagram(array.data(),array.size(),QHostAddress::Broadcast,this->port); //指定端口 广播
}
```

感觉这一段可以当作是udp通信发送信息的模板了：先定义字节数组类，然后定义一个数据流将这个字节数组类关联起来，然后通过流来读入数据，switch语句就是用来判断消息类型的，消息类型在.h文件中声明成了枚举类。最后书写报文，调用udp的接口`writeDatagram`，四个参数也很好理解，分别是数据、数据大小、通讯方式、端口号。



**receiveMessage**

```cpp
void Widget::receiveMessage()
{
    qint64 size=udpSocket->pendingDatagramSize();
    //int mysize= static_cast<int>(size);
    QByteArray array=QByteArray(size,0);
    udpSocket->readDatagram(array.data(),size);
    QDataStream stream(&array,QIODevice::ReadOnly); //与array关联 只读
    int mytype;
    QString name,msg;//用户名 聊天内容
    //获取当前的时间 格式设置为：yyyy-mm-dd hh:mm:ss
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    stream >> mytype;
    switch (mytype) {
    case Msg:
        stream >> name >> msg;
        ui->msgBrowser->setTextColor(QColor(Qt::blue));
        ui->msgBrowser->append("["+name+"]"+time);
        ui->msgBrowser->append(msg);
        break;
    case UserLeft:
        stream >> name;
        userLeft(name,time);
        break;
    case UserEnter:
        stream >> name;
        userEnter(name);
        break;
    }
}
```

本质上和sendMsg是逆过程，这里主要是接收数据报并拆开数据报变成数据的过程。



**聊天框的八个功能**

```cpp
//字体大小
void (QComboBox:: *sizeBtn)(const QString &text)=&QComboBox::currentTextChanged;
connect(ui->sizeCbx,sizeBtn,this,[=](const QString &text){
    ui->msgTxtEdit->setFontPointSize(text.toDouble());
    ui->msgTxtEdit->setFocus();
});

//加粗
connect(ui->boldBtn,&QToolButton::clicked,this,[=](bool checked){
    //是否按下
    if(checked){
        ui->msgTxtEdit->setFontWeight(QFont::Bold);
    }else{
        ui->msgTxtEdit->setFontWeight(QFont::Normal);
    }
});

//倾斜
connect(ui->italicTbtn,&QToolButton::clicked,this,[=](bool checked){
    ui->msgTxtEdit->setFontItalic(checked);
    ui->msgTxtEdit->setFocus();
});

//下划线
connect(ui->underlineTbtn,&QToolButton::clicked,this,[=](bool checked){
    ui->msgTxtEdit->setFontUnderline(checked);
    ui->msgTxtEdit->setFocus();
});

//清空
connect(ui->clearTbtn,&QToolButton::clicked,this,[=](){
    ui->msgBrowser->clear();
});

//字体颜色
connect(ui->colorTbtn,&QToolButton::clicked,this,[=](){
    QColor color=QColorDialog::getColor(color,this);
    ui->msgTxtEdit->setTextColor(color);
});

//保存聊天记录
connect(ui->saveTbtn,&QToolButton::clicked,this,[=](){
    if(ui->msgBrowser->toPlainText().isEmpty()){
        QMessageBox::warning(this,"警告","保存内容不能为空");
        return;
    }
    //获取保存文件绝对路径/文件名(窗口标题,文件名,类型)
    QString fileName=QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","(*.txt)");
    if(!fileName.isEmpty()){
        QFile file(fileName);
        file.open(QIODevice::WriteOnly|QFile::Text);
        QTextStream stream(&file);
        stream<<ui->msgBrowser->toPlainText();
        file.close();
    }
});
```



**getMsg**

```cpp
QString Widget::getMsg()
{
    //最开始的代码是这样的 它获取的文本是html格式 以至于我在接收数据的时候想"["+name+"]："+msg输出会报格式错误
    //QString msg=ui->msgTxtEdit->toHtml();  
    QString msg=ui->msgTxtEdit->toPlainText();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}
```

昨天在测试receiveMessage函数的时候发现改变输出样式会报错，原来就是因为在获取数据的时候msg的格式是html的格式，所以会报格式错误，如果想获取纯文本用QString来接收的话就是用`toPlainText`函数



**userEnter**

```cpp
void Widget::userEnter(QString username)
{
    //首先检查用户列表中是否已经存在该用户 如果不存在 才插入进去
    bool IsEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
       if(IsEmpty)
       {
           QTableWidgetItem *table=new QTableWidgetItem(username);
           //旧用户的视角：头插 新用户的视角：追加
           ui->tableWidget->insertRow(0);
           ui->tableWidget->setItem(0,0,table);
           //设置颜色
           ui->msgBrowser->setTextColor(QColor(Qt::gray));
           ui->msgBrowser->append(username+"已上线");
           //更改在线人数条目
           ui->userNumLbl->setText(QString("在线人数:%1").arg(ui->tableWidget->rowCount()));
           //调用sendMsg 用户进入的信息
           sendMsg(UserEnter);
       }
}
```



**userLeft**

```cpp
void Widget::userLeft(QString username, QString time)
{
    //还是先判空 存在才能删除列表
    bool isEmpty=ui->tableWidget->findItems(username,Qt::MatchExactly).isEmpty();
    if(!isEmpty){
        //找到是第几行 并删除
        int row=ui->tableWidget->findItems(username,Qt::MatchExactly).first()->row();
        ui->tableWidget->removeRow(row);
        ui->msgBrowser->append(username+"用户于"+time+"离开");
        ui->userNumLbl->setText(QString("在线人数:%1").arg(ui->tableWidget->rowCount()));
        //sendMsg(UserLeft);    不能有这一行
    }
}
```

大致和用户进入的代码一致，问题就在于用户离开不能有sendMsg的函数调用，因为用户点击退出之后就相当于断开连接了，如果再次调用的话会导致多次收到重复的信号，所以这么做是不对的。可以看到closeEvent这个函数，这和关闭窗口绑定了的槽函数，这个函数里面已经调用了sendMsg(UserLeft)了，所以在userLeft函数中就没有必要再调用这个函数了