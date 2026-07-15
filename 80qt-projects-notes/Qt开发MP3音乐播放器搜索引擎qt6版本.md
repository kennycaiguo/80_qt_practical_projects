# qt5版本开发的播放器

# Qt开发MP3音乐播放器搜索引擎

程序员老舅，专注C++

## 1 新建项目

新建项目，为[Qwidget](https://zhida.zhihu.com/search?content_id=242984231&content_type=Article&match_order=1&q=Qwidget&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODM3MTU4NzUsInEiOiJRd2lkZ2V0IiwiemhpZGFfc291cmNlIjoiZW50aXR5IiwiY29udGVudF9pZCI6MjQyOTg0MjMxLCJjb250ZW50X3R5cGUiOiJBcnRpY2xlIiwibWF0Y2hfb3JkZXIiOjEsInpkX3Rva2VuIjpudWxsfQ.kwNKpIjNRBHuJD9smnQ4V0NTSHdP3pYOtWC7QfmuAeQ&zhida_source=entity)

![img](https://pica.zhimg.com/v2-f6e4affaff6911e246dfcf45089d27e4_1440w.jpg)

新建res文件，图标放入

![img](https://picx.zhimg.com/v2-8b56520ec5855cd186a66f056d037feb_1440w.jpg)

## 1.1 UI设计

按下图设计ui也可以自己设计，美观我暂时就不考虑了只要实现功能

![img](https://pic2.zhimg.com/v2-b67b04ea885c30f7104a64cbb1e94acb_1440w.jpg)

添加播放进度条，播放的其他按钮，声音调节进度条

![img](https://picx.zhimg.com/v2-bf99df48ee34f095936a43cb7b42a5e1_1440w.jpg)

## 1.2 修改背景

自定义一个paintEvent(),把图片画到界面上就行，我还是不用背景了

```text
void OnlineMp3Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/res/dinyilang.png"));
}
```



## 1.3 生成所有控件的槽函数

按钮只需要点击槽函数，声音大小调节的进度条需要值变化的槽函数

![img](https://pic3.zhimg.com/v2-198137287170c73ee26fc28394e42ac4_1440w.jpg)

播放音乐进度条需要，这三个槽函数

![img](https://pica.zhimg.com/v2-68e152deb731c4eb2548ec939814b016_1440w.jpg)

最后就是这么多槽函数

![img](https://pic3.zhimg.com/v2-34b2db0360819895ca265058dc2ebd36_1440w.jpg)

## 1.4 添加必要库和头文件

在.pro文件添加必要库，network multimedia sql

![img](https://pic3.zhimg.com/v2-0f21684709d8c187b9e141a3feecea8c_1440w.jpg)

添加必要的头文件

```text
#include <QNetworkRequest>          //HTTP的URL管理类
#include <QNetworkAccessManager>    //URL的上传管理
#include <QNetworkReply>            //网页回复数据触发信号的类
#include <QEventLoop>               //提供一种进入和离开事件循环的方法
#include <QJsonArray>               //封装JSON数组
#include <QJsonObject>              //封装JSON对象

#include <QMediaPlayer>             //播放音乐相关
#include <QMediaPlaylist>

#include <QSqlDatabase>             //数据库相关
#include <QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QTime>
#include <math.h>
```

## 2 部分逻辑编写

## 2.1关闭窗口

在ui界面右击关闭按钮，转到槽，使用click事件，编写代码后我们就可以点击按钮关闭界面

```text
void OnlineMp3Widget::on_btn_close_clicked()
{
    this->close();
}
```

## 2.2 鼠标拖动窗口

定义三个函数，点击移动释放，两个变量。

![img](https://pic2.zhimg.com/v2-640daddd8b854d97f7fc66222ad780f1_1440w.jpg)

当鼠标左键按下时，把mousePress标志为真，计算鼠标相对于窗口左上角的偏移量，储存在movePoint中;当鼠标移动时获取鼠标的全局位置，并计算移动距离，通过move函数将窗口移动到新位置

```text
//移动事件
void OnlineMp3Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePress)  // 如果鼠标被按下
    {
        QPoint movepos = event->globalPos();  // 获取鼠标当前的全局位置
        move(movepos - movePoint);  // 移动窗口位置
    }
}

//鼠标释放事件
void OnlineMp3Widget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)  // 避免未使用的参数警告
    mousePress = false;  // 鼠标释放，标记鼠标状态为未按下
}

//鼠标点击事件
void OnlineMp3Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)  // 如果鼠标左键被按下
    {
        mousePress = true;  // 标记鼠标状态为按下
    }
    movePoint = event->globalPos() - pos();  // 计算鼠标相对于窗口左上角的偏移量
}
```

## 3 歌曲数据库搭建,歌曲搜索

## 3.1 数据库搭建

首先连接数据库，如果不存在的话创建一个数据库

```text
// 1、连接数据库，如果不存在则创建
    if (QSqlDatabase::contains("song")) // 检查是否存在名为"song"的数据库连接
    {
        db = QSqlDatabase::database("song"); // 如果存在，则获取该数据库连接
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE"); // 如果不存在，则添加一个SQLite数据库连接
        db.setDatabaseName("song.db"); // 设置数据库文件名为"song.db"
    }
```

使用open打开数据库，使用query作为查询语句，创建歌曲信息表格，表里边有id，歌名，歌手，

```text
// 2、打开数据库，读取数据表
    if (!db.open()) // 尝试打开数据库
    {
        // 打开数据库失败，显示错误信息
        QMessageBox::critical(nullptr, "错误", db.lastError().text());
    }
    else
    {
        // 3、定义查询对象，执行数据库操作
        QSqlQuery query; // 定义数据库查询对象
        QString qstl = "create table if not exists songlist(id integer , songname text , singername text , album_id text)"; // 创建歌曲列表表格的SQL语句
        int ret = query.exec(qstl); // 执行SQL语句
        if (!ret) // 检查SQL执行是否成功
        {
            // SQL执行失败，显示错误信息
            QMessageBox::critical(nullptr, "错误", db.lastError().text());
            qDebug() << db.lastError();
        }
```

创建歌曲记录表格，用于我们搜索后记录，双击后又能再次播放

```text
// 创建歌曲记录表格
        qstl = "create table if not exists songhistory(id integer primary key autoincrement, songname text , singername text)";
        ret = query.exec(qstl);
        if (!ret)
        {
            QMessageBox::critical(nullptr, "错误", db.lastError().text());
        }
```

通过record将搜索记录显示在ListWidget上

[![img](https://picx.zhimg.com/v2-f11c24e25cf0b882477301661fb2186c.png?source=7e7ef6e2&needBackground=1)程序员老舅：十年程序员 对于 Qt开发 学习路线的总结138 赞同 · 362 评论 ](https://zhuanlan.zhihu.com/p/627819927)文章

```text
// 查询歌曲历史记录表中的数据并显示
        qstl = "select * from songhistory";
        if (!query.exec(qstl)) // 执行查询操作
        {
            // 查询失败，显示错误信息
            QMessageBox::critical(nullptr, "错误", db.lastError().text());
        }
        while (query.next()) // 遍历查询结果
        {
            QString songname, singername;
            QSqlRecord rec = query.record(); // 获取查询结果的记录
            int songnamekey = rec.indexOf("songname"); // 获取歌曲名字段在查询结果中的索引
            int singerkey = rec.indexOf("singername"); // 获取歌手名字段在查询结果中的索引
            songname = query.value(songnamekey).toString(); // 获取歌曲名
            singername = query.value(singerkey).toString(); // 获取歌手名

            QString strshow = songname + "--" + singername; // 构造要显示的字符串
            QListWidgetItem *item = new QListWidgetItem(strshow); // 创建列表项
            ui->lw_record->addItem(item); // 添加列表项到列表控件中
        }
```

3.2 搜索歌曲并且双击播放

3.2.1点击搜索on_btn_search_clicked

在我们在搜索框输入歌曲名时，首先需要清空搜索队列，并且删除sonlist数据表中的所有数据；根据歌曲名称，使用酷狗的api构建网络搜索的URL；调用httpAccess函数发起http响应，使用loop等待http请求结束，一旦完成finish信号发出，数据将会储存在JsonData中。

```text
static QString kugouSearchApi = "http://mobilecdn.kugou.com/api/v3/search/song?";
static QString kugouDownldadApi = "https://wwwapi.kugou.com/yy/index.php?";
void OnlineMp3Widget::on_btn_search_clicked()
{
    // 清空搜索队列
    ui->lw_learch->clear();

    // 清理数据库中已经存储的 hash 等数据
    QSqlQuery query;
    QString sql = "delete from songlist;" ;

    if(!query.exec(sql))
    {
        QMessageBox::critical(nullptr,"错误",query.lastError().text());
    }

    // 根据用户输入的 MP3 名称发起操作请求
    QString url = kugouSearchApi + QString("format=json&keyword=%1&page=1&pagesize=20&showtype=1").arg(ui->le_search->text());

    // 发起 HTTP 请求
    httpAccess(url);

    QByteArray JsonData;
    QEventLoop loop;

    // 等待 HTTP 请求完成并获取数据
    auto c = connect(this, &OnlineMp3Widget::finish, [&](const QByteArray &data){
        JsonData = data;
        loop.exit(1);
    });
    loop.exec();
    disconnect(c);

    // 解析获取的 JSON 数据
    hashJsonAnalysis(JsonData);
}
```

## 3.2.2 发送网页请求httpAccess

编写httpAccess函数。首先实例化网络请求，将URL存入,创建一个管理对象manger，用manger发送get请求，查看manger的类里边有完成的信号，使用改信号绑定槽函数netReplay

![img](https://pic2.zhimg.com/v2-05b5fc07dd12fd75a026628d24f6fd4b_1440w.jpg)

```text
void OnlineMp3Widget::httpAccess(QString url)
{
    //实例化网络请求操作事项
    request = new QNetworkRequest;

    //将url网页地址存入request请求中
    request->setUrl(url);

    //实例化网络管理（访问）
    manager = new QNetworkAccessManager;

    //通过get,上传具体的请求
    manager->get(*request);

    //当网页回复消息，出发finish信号，读取数据
    connect(manager,&QNetworkAccessManager::finished,this,&OnlineMp3Widget::netReplay);
}
```

## 3.2.3 读取网络数据netReplay

使用attribute获得响应的状态码，状态码用于指示服务器对请求的处理结果，例如 200 表示请求成功。如果没有发生错误，读取数据并且发送finish信号

```text
// 读取网络数据槽函数
void OnlineMp3Widget::netReply(QNetworkReply *reply)
{
    // 获取响应状态码，200 属于正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << status_code;

    // 重定向目标属性
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error() == QNetworkReply::NoError)
    {
        // 如果没有发生网络错误，则读取响应数据
        QByteArray data = reply->readAll();
        // 发射自定义的 finish 信号，将响应数据传递给槽函数
        emit finish(data);
    }
    else
    {
        // 如果发生了网络错误，则打印错误信息
        qDebug() << reply->errorString();
    }
}
```

https://work.weixin.qq.com/u/vcc57f1882396b7f56?v=4.1.22.28160 (二维码自动识别)

## 3.2.4 解析网页回复数据并存储hashJsonAnalysis

首先我们先打印一下json看看长啥样，把json保存下来打开查看

```text
QFile file("output.json");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(JsonData);
        file.close();
    }
```

可以发现我们需要的数据在data-info-[编号]里边，因此我们要进到里边去读取数据

![img](https://pic1.zhimg.com/v2-61e0a2c43734ce9668028e3fc9abf022_1440w.jpg)

进入data-info-[序号]，读取歌名，歌手名，hash，专辑，存入搜索数据表，并且把歌曲名和歌手名作为item展示在界面上

```text
// 将 JSON 数据解析为 QJsonDocument 对象
    QJsonDocument document = QJsonDocument::fromJson(JsonData);

    if(document.isObject()) // 如果解析后的对象是一个 JSON 对象
    {
        QJsonObject data = document.object(); // 获取 JSON 对象中的"data"字段
        if(data.contains("data")) // 如果"data"字段存在
        {
            QJsonObject objectInfo = data.value("data").toObject(); // 获取"data"字段中的对象
            if(objectInfo.contains("info")) // 如果"info"字段存在
            {
                QJsonArray objectHash = objectInfo.value("info").toArray(); // 获取"info"字段中的数组

                for(int i = 0; i < objectHash.count(); i++) // 遍历数组中的每个元素
                {
                    QString songname, singername, album_id, hash;
                    QJsonObject album = objectHash.at(i).toObject(); // 获取数组元素中的对象

                    // 从对象中获取歌曲名、歌手名、专辑 ID 和哈希值
                    if(album.contains("album_id"))
                    {
                        album_id = album.value("album_id").toString();
                    }
                    if(album.contains("songname"))
                    {
                        songname = album.value("songname").toString();
                    }
                    if(album.contains("singername"))
                    {
                        singername = album.value("singername").toString();
                    }
                    if(album.contains("hash"))
                    {
                        hash = album.value("hash").toString();
                    }

                    // 将解析出的信息插入数据库
                    QSqlQuery query;
                    QString sql = QString("insert into songlist values(%1,'%2','%3','%4','%5')").arg(QString::number(i)).arg(songname).arg(singername).arg(album_id).arg(hash);
                    if(!query.exec(sql)) // 如果插入数据库失败
                    {
                        QMessageBox::critical(nullptr, "插入数据库错误", db.lastError().text());
                    }

                    // 在搜索展示框中显示歌曲名称和歌手名称
                    QString show = songname + "  " + singername;
                    QListWidgetItem *item = new QListWidgetItem(show);
                    ui->lw_search->addItem(item);
                }
            }
        }
    }
```

运行程序搜索后发现报错，插入数据库报错，查看发现数据库只有4个title,而我插入有5个，少了个hash，修改前边代码，添加上hash表头

![img](https://picx.zhimg.com/v2-e1182af8bc1d92b3abf65cfd51d2d047_1440w.jpg)

再次运行发现不报错，也可以显示信息了

## 4 歌曲下载播放，修改新的酷狗搜索下载api

## 4.1 歌曲下载和播放downloadPlayer

构建下载歌曲的url,发起http请求，等待完成并获取数据，musicJsonAnalysis解析Json获取要播放的文件url

```text
// 音乐歌曲的下载和播放
void OnlineMp3Widget::downloadPlayer(QString album_id, QString hash)
{
    // 构建下载歌曲的 URL
    QString url = kugouDownldadApi + QString("r=play/getdata"
                                             "&hash=%1&album_id=%2"
                                             "&dfid=1spkkh3QKS9PeiJupz0oTy5G"
                                             "&mid=de94e92794f31e9cd6ff4cb309d2baa2"
                                             "&platid=4").arg(hash).arg(album_id);

    // 发起 HTTP 请求获取歌曲数据
    httpAccess(url);

    QByteArray JsonData;
    QEventLoop loop;

    // 等待 HTTP 请求完成并获取数据
    auto d = connect(this, &OnlineMp3Widget::finish, [&](const QByteArray &data){
        JsonData = data;
        loop.exit(1);
    });
    loop.exec();
    disconnect(d);

    // 解析要播放的音乐
    QString music = musicJsonAnalysis(JsonData);

    // 设置媒体并播放音乐
    player->setMedia(QUrl(music));
    // 设置音量
    player->setVolume(50);
    // 设置音量滚动条
    ui->hs_sound->setValue(50);
    // 播放音乐
    player->play();
}
```

## 4.2 解析歌曲歌词json文件musicJsonAnalysis

首先也是先下载一下json看一下里边的格式，我们需要进入data字段获取lyrics和play_url。发送显示歌词信号，返回播放地址url

```text
// 解析 JSON 数据，获取音乐播放 URL
QString OnlineMp3Widget::musicJsonAnalysis(QByteArray JsonData)
{
    // 保存 JSON 数据到文件中以便查看
    QFile file("download.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(JsonData);
        file.close();
    }

    // 解析 JSON 数据
    QJsonDocument document = QJsonDocument::fromJson(JsonData);
    if (document.isObject())
    {
        QJsonObject data = document.object();
        if (data.contains("data"))
        {
            QJsonObject objectPlayurl = data.value("data").toObject();
            // 如果包含歌词，发送歌词显示信号
            if (objectPlayurl.contains("lyrics"))
            {
                emit lyricShow(objectPlayurl.value("lyrics").toString());
            }
            // 返回音乐播放 URL
            if (objectPlayurl.contains("play_url"))
            {
                return objectPlayurl.value("play_url").toString();
            }
        }
    }
}
```

4.3 双击搜索列表歌曲播放playSearchMusic

双击搜索列表的歌曲，实现播放，先在构造函数里绑定槽函数，查询到QLIstWidegt有这样的一个点击信号，我们就使用它connect(ui->lw_search,&QListWidget::itemClicked,this,&OnlineMp3Widget::playSearchMusic);

![img](https://pic2.zhimg.com/v2-31e07d4e18b5a5b12b2ee198ad2b8eaf_1440w.jpg)

点击尝试一下，可以输出行号

```text
//获取双击的歌曲索引，就是数据库的数据表的ID号
    int row = ui->lw_search->currentRow();
    qDebug()<<"row"<<row;
```

![img](https://pic3.zhimg.com/v2-335d1bdebab3e02d0498bbd62ff8dd70_1440w.jpg)

随后我们先查询历史记录里有没有这首歌，没有就储存，插入新的数据，然后调用downloadPlayer下载播放歌曲

```text
QSqlQuery query;
    QString sql = QString("select * from songlist where id = %1;").arg(row);
    if (!query.exec(sql))
    {
        QMessageBox::critical(nullptr, "select * from songlist where id =", db.lastError().text());
    }

    // 将选中的音乐的数据信息存入历史数据表
    QString songname, singername, album_id, hash;
    while (query.next())
    {
        QSqlRecord record = query.record();
        int songkey = record.indexOf("songname");
        int singerkey = record.indexOf("singername");
        int albumkey = record.indexOf("album_id");
        int hashkey = record.indexOf("hash");

        songname = query.value(songkey).toString();
        singername = query.value(singerkey).toString();
        album_id = query.value(albumkey).toString();
        hash = query.value(hashkey).toString();

        // 查询历史数据表中是否已经存在该歌曲的记录
        sql = QString("select hash from songhistory where hash = '%1'").arg(hash);
        if (!query.exec(sql))
        {
            QMessageBox::critical(nullptr, "select hash from songhistory where hash =", db.lastError().text());
        }
        // 如果不存在该记录，则将其存入历史数据表
        if (query.next() == NULL)
        {
            sql = QString("insert into songhistory values(NULL, '%1', '%2', '%3', '%4')").arg(songname).arg(singername).arg(album_id).arg(hash);
            if (!query.exec(sql))
            {
                QMessageBox::critical(nullptr, "insert error", db.lastError().text());
            }
            // 将歌手和歌名放入历史歌曲表中显示
            QString show = songname + " " + singername;
            QListWidgetItem *item = new QListWidgetItem(show);
            ui->lw_record->addItem(item);
        }
    }
    // 下载并播放选中的音乐
    downloadPlayer(album_id, hash);
```

## 4.4 双击没有声音

但是此时运行双击歌曲，发现有报错，插入hash报错了。原来是我新建历史表格的数不对，表头只有歌名和歌手，所以插入报错了。修改后再次运行，双击无报错，但没有声音。
排查发现获取到的download的Json数据有问题，可能是api失效了。

![img](https://pic4.zhimg.com/v2-b5875dd522bb1f98b7ecb687a12406fb_1440w.jpg)

发现歌词和播放地址现在都存在这里边

![img](https://pica.zhimg.com/v2-0be21dd84c4e1521bec3322153872e92_1440w.jpg)

他的链接形式

[https://wwwapi.kugou.com/play/songinfo?srcappid=2919&clientver=20000&clienttime=1713771570169&mid=707708a817d80eedd95f2ae68bc57780&uuid=707708a817d80eedd95f2ae68bc57780&dfid=11SITU3au0iw0OdGgJ0EhTvI&appid=1014&platid=4&encode_album_audio_id=j5yn384&token=&userid=0&signature=59068ffba4652d7eb4868a460db73375](https://link.zhihu.com/?target=https%3A//wwwapi.kugou.com/play/songinfo%3Fsrcappid%3D2919%26clientver%3D20000%26clienttime%3D1713771570169%26mid%3D707708a817d80eedd95f2ae68bc57780%26uuid%3D707708a817d80eedd95f2ae68bc57780%26dfid%3D11SITU3au0iw0OdGgJ0EhTvI%26appid%3D1014%26platid%3D4%26encode_album_audio_id%3Dj5yn384%26token%3D%26userid%3D0%26signature%3D59068ffba4652d7eb4868a460db73375)

对比两首不同得歌，这三个参数不一样，而且现在hash已经没用了，使用的是encode_album_audio_id

clienttime:时间戳

encode:音乐

signature：md5加密参数

![img](https://pic1.zhimg.com/v2-9418ed35879a33ebc1266f64dbc8d6c6_1440w.jpg)

主要是signature这个参数，找到他，打开代码来源

![img](https://pica.zhimg.com/v2-4e2153604676b390cd7ac9bb9afd9054_1440w.jpg)

主要是signature这个参数，找到他，打开代码来源

![img](https://pic3.zhimg.com/v2-bf917958f3aa0d2a3ce884a5f1be5340_1440w.jpg)

## 4.4.1 安装openssl（可以不装，没用到）

要使用md5加密的库需要安装openssl，我们直接下载安装版[https://slproweb.com/products/Win32OpenSSL.html](https://link.zhihu.com/?target=https%3A//slproweb.com/products/Win32OpenSSL.html)，下载64位的安装包

![img](https://pic4.zhimg.com/v2-16c1ea9cf27bc2e090c15ed979d90135_1440w.jpg)

这一步我们选择放到bin

![img](https://pic3.zhimg.com/v2-46f70478bda629bd153a6c0434fcb498_1440w.jpg)

打开cmd，输入openssl version，显示版本，安装成功

![img](https://pic3.zhimg.com/v2-6f64952f765eca7ec083ae4dad74027a_1440w.jpg)

## 4.4.2 md5加密函数实现

就是把网页上的所有字符串都拼起来然后进行加密

```text
QString OnlineMp3Widget::getMd5(QString time, QString encode_album_audio_id)
{
    // 构建签名列表
    QStringList signature_list;
    signature_list << "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt"
                   << "appid=1014"
                   << "clienttime=" + time
                   << "clientver=20000"
                   << "dfid=11SITU3au0iw0OdGgJ0EhTvI"
                   << "encode_album_audio_id=" + encode_album_audio_id
                   << "mid=707708a817d80eedd95f2ae68bc57780"
                   << "platid=4"
                   << "srcappid=2919"
                   << "token="
                   << "userid=0"
                   << "uuid=707708a817d80eedd95f2ae68bc57780"
                   << "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt";

    // 将签名列表中的元素连接成一个字符串
    QString string = signature_list.join("");
    //qDebug()<< string;
    //生成 MD5 哈希
    QByteArray hashedData = QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5);

    // 将哈希数据转换为十六进制字符串
    QString md5Hash = hashedData.toHex();

    return md5Hash;
}
```

同时需要修改酷狗api

```text
static QString kugouDownldadApi = "https://wwwapi.kugou.com/play/songinfo?";

 //构建下载歌曲的 URL
    QDateTime time = QDateTime::currentDateTime();
    // 将当前时间转换为自纪元以来的秒数，并将其转换为字符串
    QString currentTimeString = QString::number(time.toSecsSinceEpoch()*1000);
    currentTimeString = "1713782920612";
    QString encode_album_audio_id = "j5yn384";
    QString signaturecode = getMd5(currentTimeString,encode_album_audio_id);
    QString url = kugouDownldadApi + QString("srcappid=2919"
                                             "&clientver=20000"
                                             "&clienttime=%1"
                                             "&mid=707708a817d80eedd95f2ae68bc57780"
                                             "&uuid=707708a817d80eedd95f2ae68bc57780"
                                             "&dfid=11SITU3au0iw0OdGgJ0EhTvI"
                                             "&appid=1014"
                                             "&platid=4"
                                             "&encode_album_audio_id=%2"
                                             "&token="
                                             "&userid=0"
                                             "&signature=%3"
                                             ).arg(currentTimeString).arg(encode_album_audio_id).arg(signaturecode);
```

手动输入网页的时间和歌曲id，进行测试看看md5加密对不对，发现能够对上，并且也能够播放音乐了

![img](https://pic3.zhimg.com/v2-8fafd583e6156af03edb0b7aeb122ebc_1440w.jpg)

4.5 hash已经没用了，替换为encode_album_audio_id，修改搜索歌曲函数

hash已经是老的apil，现在使用encode_album_audio_id即可，我们回到搜索歌曲的Json里,发现没有那个id，所以要自己重新搜索加入。在酷狗的搜索界面我们找到了这个id，把他取出。

![img](https://pic3.zhimg.com/v2-f7872debd5edb7c154b836f4e5a49a54_1440w.jpg)

对比发现更下载歌曲的变化相同，仿照着上边来写

![img](https://pic2.zhimg.com/v2-185c7539bd7532a117c87b062bc995d1_1440w.jpg)

![img](https://picx.zhimg.com/v2-d000393f003f7693184b0b6b118b1f77_1440w.jpg)

更改酷狗搜索api[https://complexsearch.kugou.com/v2/search/song?](https://link.zhihu.com/?target=https%3A//complexsearch.kugou.com/v2/search/song%3F)，修改搜索歌曲

```text
//搜索歌曲
void OnlineMp3Widget::on_btn_search_clicked()
{
    // 清空搜索队列
    ui->lw_search->clear();

    // 清理数据库中已经存储的 hash 等数据
    QSqlQuery query;
    QString sql = "delete from songlist;" ;

    if(!query.exec(sql))
    {
        QMessageBox::critical(nullptr,"错误",query.lastError().text());
    }
    QDateTime time = QDateTime::currentDateTime();
    // 将当前时间转换为自纪元以来的秒数，并将其转换为字符串
    QString currentTimeString = QString::number(time.toSecsSinceEpoch()*1000);

    QString signaturecode = getSearch_Md5(ui->le_search->text(),currentTimeString);
    // 根据用户输入的 MP3 名称发起操作请求
    QString url = kugouSearchApi + QString("callback=callback123"
                                           "&srcappid=2919"
                                           "&clientver=1000"
                                           "&clienttime=%1"
                                           "&mid=707708a817d80eedd95f2ae68bc57780"
                                           "&uuid=707708a817d80eedd95f2ae68bc57780"
                                           "&dfid=11SITU3au0iw0OdGgJ0EhTvI"
                                           "&keyword=%2"
                                           "&page=1"
                                           "&pagesize=30"
                                           "&bitrate=0"
                                           "&isfuzzy=0"
                                           "&inputtype=0"
                                           "&platform=WebFilter"
                                            "&userid=0"
                                           "&iscorrection=1"
                                           "&privilege_filter=0"
                                           "&filter=10"
                                           "&token="
                                           "&appid=1014"
                                           "&signature=%3"
                                           ).arg(currentTimeString).arg(ui->le_search->text()).arg(signaturecode);

    // 发起 HTTP 请求
    httpAccess(url);

    QByteArray JsonData;
    QEventLoop loop;

    // 等待 HTTP 请求完成并获取数据
    auto c = connect(this, &OnlineMp3Widget::finish, [&](const QByteArray &data){
        JsonData = data;
        loop.exit(1);
    });
    loop.exec();
    disconnect(c);

    // 解析获取的 JSON 数据
    hashJsonAnalysis(JsonData);
}
```

打断点跟上边一样，发现要输入这些数据，进行md5的编码

![img](https://pic1.zhimg.com/v2-de8d28ac2fd24ac9a3fc4867c52ff1ce_1440w.jpg)

```text
QString OnlineMp3Widget::getSearch_Md5(QString songname, QString time)
{
    // 构建签名列表
    QStringList signature_list;
    signature_list <<   "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt"
                   <<   "appid=1014"
                   <<   "bitrate=0"
                   <<   "callback=callback123"
                   <<   "clienttime=" + time
                   <<   "clientver=1000"
                   <<   "dfid=11SITU3au0iw0OdGgJ0EhTvI"
                   <<   "filter=10"
                   <<   "inputtype=0"
                   <<   "iscorrection=1"
                   <<   "isfuzzy=0"
                   <<   "keyword=" + songname
                   <<   "mid=707708a817d80eedd95f2ae68bc57780"
                   <<   "page=1"
                   <<   "pagesize=30"
                   <<   "platform=WebFilter"
                   <<   "privilege_filter=0"
                   <<   "srcappid=2919"
                   <<   "token="
                   <<   "userid=0"
                   <<   "uuid=707708a817d80eedd95f2ae68bc57780"
                   <<   "NVPh5oo715z5DIWAeQlhMDsWXXQV4hwt";

    // 将签名列表中的元素连接成一个字符串
    QString string = signature_list.join("");
    //qDebug()<< string;
    //生成 MD5 哈希
    QByteArray hashedData = QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5);

    // 将哈希数据转换为十六进制字符串
    QString md5Hash = hashedData.toHex();

    return md5Hash;
}
```

## 4.6 修改数据库和其他相关代码

先手动删除数据库中的两个表，修改搜索数据库（代码改动较多，可以查看提交）

![img](https://pica.zhimg.com/v2-11151624876c6f71f1f3205d1eb87f52_1440w.jpg)

修改完后运行，查看hashjson，拿到EMixSongID，要进入data-list-EMixSongID

![img](https://pic1.zhimg.com/v2-f25e90825e8957f16af25e93f56d2e88_1440w.jpg)

修改代码，发现Json前边有callback123（），识别不了，需要把这个callback123和外边的一对括号删了，找到括号的下标，提取出中间的Json

```text
//移除callback123()
    // 找到第一个左括号 "(" 的位置
    int leftBracketIndex = JsonData.indexOf('(');
    if (leftBracketIndex != -1)
    {
        // 找到最后一个右括号 ")" 的位置
        int rightBracketIndex = JsonData.lastIndexOf(')');
        if (rightBracketIndex != -1)
        {
            // 提取 JSON 数据，去除包裹的部分
            JsonData = JsonData.mid(leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1);
        }
    }
```

修改下边读取Json，读取FileName，EMixSongID

```text
// 将 JSON 数据解析为 QJsonDocument 对象
    QJsonDocument document = QJsonDocument::fromJson(JsonData);

    if(document.isObject()) // 如果解析后的对象是一个 JSON 对象
    {
        qDebug()<<"boject";
        QJsonObject data = document.object(); // 获取 JSON 对象中的"data"字段
        if(data.contains("data")) // 如果"data"字段存在
        {
            QJsonObject objectInfo = data.value("data").toObject(); // 获取"data"字段中的对象
            qDebug()<<"data";
            if(objectInfo.contains("lists")) // 如果"lists"字段存在
            {
                QJsonArray objectHash = objectInfo.value("lists").toArray(); // 获取"lists"字段中的数组
                qDebug()<<"lists";
                for(int i = 0; i < objectHash.count(); i++) // 遍历数组中的每个元素
                {
                    QString singer_song_name,EMixSongID;
                    QJsonObject album = objectHash.at(i).toObject(); // 获取数组元素中的对象

                    // 从对象中获取歌曲名、歌手名、专辑 ID 和哈希值
                    if(album.contains("FileName"))
                    {
                        singer_song_name = album.value("FileName").toString();
                    }
                    if(album.contains("EMixSongID"))
                    {
                        EMixSongID = album.value("EMixSongID").toString();
                    }
                    // 将解析出的信息插入数据库
                    QSqlQuery query;
                    QString sql = QString("insert into songlist values(%1,'%2','%3');").arg(QString::number(i)).arg(singer_song_name).arg(EMixSongID);
                    if(!query.exec(sql)) // 如果插入数据库失败
                    {
                        QMessageBox::critical(nullptr, "插入数据库错误", db.lastError().text());
                    }

                    // 在搜索展示框中显示歌曲名称和歌手名称
                    QListWidgetItem *item = new QListWidgetItem(singer_song_name);
                    ui->lw_search->addItem(item);
                }
            }
        }
    }
```

再次运行，搜索列表能够显示歌曲

## 4.7 修复双击播放歌曲playSearchMusic

代码与新的数据库对应即可

```text
// 将选中的音乐的数据信息存入历史数据表
    QString  singer_song_name,EMixSongID;
    while (query.next())
    {
        QSqlRecord record = query.record();
        int singer_song_namekey = record.indexOf("FileName");
        int EMixSongIDkey = record.indexOf("EMixSongID");

        singer_song_name = query.value(singer_song_namekey).toString();
        EMixSongID = query.value(EMixSongIDkey).toString();

        // 查询历史数据表中是否已经存在该歌曲的记录
        sql = QString("select EMixSongID from songhistory where EMixSongID = '%1';").arg(EMixSongID);
        if (!query.exec(sql))
        {
            QMessageBox::critical(nullptr, "select hash from songhistory where EMixSongID =", db.lastError().text());
        }
        // 如果不存在该记录，则将其存入历史数据表
        if (query.next() == NULL)
        {
            sql = QString("insert into songhistory values(NULL, '%1', '%2')").arg(singer_song_name).arg(EMixSongID);
            if (!query.exec(sql))
            {
                QMessageBox::critical(nullptr, "insert error", db.lastError().text());
            }
            // 将歌手和歌名放入历史歌曲表中显示
            QListWidgetItem *item = new QListWidgetItem(singer_song_name);
            ui->lw_record->addItem(item);
        }
    }
    // 下载并播放选中的音乐
    downloadPlayer(EMixSongID);
}
```

## 5 歌词显示，进度条

## 5.1 歌词显示

在musicJsonAnalysis函数里我们已经发送了显示歌词的信号，对它进行槽函数绑定，并显示歌词。在构造函数里绑定槽函数

```text
//绑定显示歌词
    connect(this,&OnlineMp3Widget::lyricShow,this,&OnlineMp3Widget::lyricTextShow);

void OnlineMp3Widget::lyricTextShow(QString str)
{
    ui->tb_songtext->setText(str);
}
```

![img](https://pic2.zhimg.com/v2-c6618b2829b2fdb8f4426b1d6d575667_1440w.jpg)

## 5.2 进度条绑定

Player对象里边包含了播放的位置信号，用它绑定进度条的更新。

```text
// 绑定播放器的位置变化信号到更新播放进度函数
connect(player, &QMediaPlayer::positionChanged, this, &OnlineMp3Widget::updateDuration);

//更新播放的进度条和时间
void OnlineMp3Widget::updateDuration(qint64 value)
{
    // 设置播放进度条的范围为 0 到 播放器的总时长
    ui->hs_songtime->setRange(0, player->duration());
    // 设置播放进度条的当前值为当前播放位置
    ui->hs_songtime->setValue(value);
}
```

## 5.3 点击播放历史歌曲playHistoryMusic

播放历史音乐更播放搜索音乐相似，拿到行号就行，从数据库里读取id，进行播放。打开数据库，我们一开始使用的是主键id自增，所以他是从1开始的，获取row行号后还要+1。绑定历史歌曲表的双击信号和播放历史歌曲槽函数。

![img](https://pica.zhimg.com/v2-00b44cd47dd3653220f760856a64c98a_1440w.jpg)

```text
//绑定双击历史列表播放音乐槽函数
    connect(ui->lw_record,&QListWidget::itemDoubleClicked,this,&OnlineMp3Widget::playHistoryMusic);

//播放历史音乐
void OnlineMp3Widget::playHistoryMusic()
{
    // 获取当前列表中双击的歌曲索引，即数据表的 ID 号
    int row = ui->lw_record->currentRow();

    // 执行数据库查询，获取对应 ID 的歌曲信息
    QSqlQuery query;
    QString sql = QString("SELECT * FROM songhistory WHERE id = %1;").arg(row+1);
    if (!query.exec(sql))
    {
        // 如果查询失败，显示错误消息
        QMessageBox::critical(nullptr, "Error executing query", db.lastError().text());
    }

    QString EMixSongID; // 用于存储查询到的歌曲的 EMixSongID

    // 遍历查询结果集中的每一条记录
    while (query.next())
    {
        QSqlRecord record = query.record(); // 获取当前记录
        int EMixSongIDkey = record.indexOf("EMixSongID"); // 获取 EMixSongID 字段在记录中的索引

        // 获取 EMixSongID 字段的值，并存储到 EMixSongID 变量中
        EMixSongID = query.value(EMixSongIDkey).toString();
    }

    // 根据获取到的 EMixSongID 播放选中的音乐
    downloadPlayer(EMixSongID);
}
```

## 5.4 更改音量

之前已经添加过音量调节滑块的槽函数，简单实现即可

```text
//音量调节
void OnlineMp3Widget::on_hs_sound_valueChanged(int value)
{
    //设置播放音量
    player->setVolume(value);
    //显示音量字符串
    ui->lb_sound->setText(QString::number(value));
}
```

## 5.5 拖动进度条播放

我们想在进度条松开的时候修改播放进度，使用一个canchange变量来限制，只需要把进度条的value值给到player的position就可以达到修改播放的功能

![img](https://pic1.zhimg.com/v2-cad74cff023108ad096cc74a70e0f900_1440w.jpg)

```text
//进度条释放
void OnlineMp3Widget::on_hs_songtime_sliderPressed()
{
    canchange = true;
}

//进度条按下
void OnlineMp3Widget::on_hs_songtime_sliderReleased()
{
    canchange = false;
}

//进度条
void OnlineMp3Widget::on_hs_songtime_valueChanged(int value)
{
    //设置时间
    QTime time(0,value/60000,qRound((value%60000)/1000.0));
    ui->lb_time->setText(time.toString("mm:ss"));
    //拖动时不播放，松开播放
    if(canchange == true)
    {
        player->setPosition(value);
    }
}
```

## 5.6 播放上一曲（对于历史歌曲表）

把row定义为全局变量，方便我们进行操作。上一曲就是把row的值减1，如果row的值为-1那么就说明没有上一曲了（此时如果一开始点击的是搜索列表的歌的话，row的值是搜索列表的）

```text
//上一首
void OnlineMp3Widget::on_btn_lastsong_clicked()
{
    // 当前行数减一
    row--;

    // 如果行数为负数，则将其设置为列表的最后一行
    if(row < 0)
    {
        row = ui->lw_record->count();
    }
    // 执行数据库查询
    QSqlQuery query;
    // 构建查询语句，查询指定行的歌曲信息
    QString sql = QString("select * from songhistory where id = %1;").arg(row+1);
    if(!query.exec(sql))
    {
        // 如果查询失败，显示错误信息
        QMessageBox::critical(nullptr,"上一曲",db.lastError().text());
    }

    QString EMixSongID;
    // 循环读取查询结果
    while(query.next())
    {
        // 获取查询结果中"EMixSongID"字段的值
        QSqlRecord lastrecord = query.record();
        int EMixSongIDkey = lastrecord.indexOf("EMixSongID");
        EMixSongID = query.value(EMixSongIDkey).toString();
        qDebug()<<EMixSongID;
    }

    // 调用下载播放器函数，传入EMixSongID参数
    downloadPlayer(EMixSongID);
}
```

点击上一首后闪退，逻辑上应该没有什么问题，发现是点击上一曲后，下载的Json不对了，歌曲的id没有放进去。最终发现是row的下标超了，在点击历史歌曲的第三首歌是2，点击上一首后经过-1，它的值竟然是3；是我重复定义了，row已经是全局变量了，所以把播放函数里的int row的int全部删除，这下正常了

![img](https://pic4.zhimg.com/v2-5ae993cfb3644dbb7c5c2afcac687dbf_1440w.jpg)

## 5.7 播放下一曲（对于历史歌曲表）

更上一曲一样，row–变为row++，此时是要判断row是否超出了歌曲项目的下标，如果超出了就置零。

```text
//下一首
void OnlineMp3Widget::on_btn_nextsong_clicked()
{
    // 当前行数减一
    row++;

    // 如果行数为负数，则将其设置为列表的最后一行
    if(row > ui->lw_record->count())
    {
        row = 0;
    }
    //选中当要播放的行
    ui->lw_record->setCurrentRow(row);
    // 执行数据库查询
    QSqlQuery query;
    // 构建查询语句，查询指定行的歌曲信息
    QString sql = QString("select * from songhistory where id = %1;").arg(row+1);
    if(!query.exec(sql))
    {
        // 如果查询失败，显示错误信息
        QMessageBox::critical(nullptr,"上一曲",db.lastError().text());
    }

    QString EMixSongID;
    // 循环读取查询结果
    while(query.next())
    {
        // 获取查询结果中"EMixSongID"字段的值
        QSqlRecord lastrecord = query.record();
        int EMixSongIDkey = lastrecord.indexOf("EMixSongID");
        EMixSongID = query.value(EMixSongIDkey).toString();
        qDebug()<<EMixSongID;
    }

    // 调用下载播放器函数，传入EMixSongID参数
    downloadPlayer(EMixSongID);
}
```

## 5.8 暂停播放

使用player的状态来即可

```text
//播放和暂停
void OnlineMp3Widget::on_btn_start_stop_clicked()
{
    // 如果播放器状态为播放状态，则暂停播放
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
    }
    // 如果播放器状态为暂停状态，则开始播放
    else if(player->state() == QMediaPlayer::PausedState)
    {
        player->play();
    }
}
```

## 5.9 循环播放

首先对循环按钮进行修改，勾选checkable，这样按钮就会有按下和没按下两个状态，按下是打开循环

![img](https://pic4.zhimg.com/v2-959b7a675e25cb42d571817f6618fb6b_1440w.jpg)

对于按下，我们绑定信号，当歌曲播放完为stop状态时，让他的postion重置到0，重新播放；当我们关闭循环式，断开这个信号连接

```text
void OnlineMp3Widget::on_btn_loop_clicked()
{
    // 将播放模式设置为循环播放
    bool isChecked = ui->btn_loop->isChecked();
    // 输出按钮状态，以便调试
    qDebug() << isChecked;

    // 声明连接对象
    QMetaObject::Connection sig;

    // 如果按钮被选中
    if (isChecked)
    {
        // 连接播放器的 stateChanged 信号到匿名函数
        // 当播放状态改变时执行相应操作
        sig = connect(player, &QMediaPlayer::stateChanged, [=]() {
            // 如果播放器状态变为停止状态
            if (player->state() == QMediaPlayer::StoppedState)
            {
                // 将播放位置设置为0
                player->setPosition(0);
                // 重新开始播放
                player->play();
            }
        });
    }
    else
    {
        // 如果按钮未被选中，断开之前的连接
        disconnect(sig);
    }
}
```

## 6 更换皮肤

首先把更换默认皮肤和自定义皮肤添加到菜单的选项里，然他两绑定对应的槽函数。点击换肤按钮的时候显示这个菜单

```text
//更换皮肤菜单
    QMenu *menuChange;

// 创建默认皮肤动作
    QAction *defaultSkin = new QAction("默认皮肤");
    connect(defaultSkin, &QAction::triggered, this, &OnlineMp3Widget::backgroundtoDefaultSkin);

    // 创建自定义皮肤动作
    QAction *dingyiSkin = new QAction("自定义皮肤");
    connect(dingyiSkin, &QAction::triggered, this, &OnlineMp3Widget::backgroundtoDingyiSkin);

    // 创建换肤菜单
    menuChange = new QMenu(this);
    // 将默认皮肤动作添加到换肤菜单
    menuChange->addAction(defaultSkin);
    // 将自定义皮肤动作添加到换肤菜单
    menuChange->addAction(dingyiSkin);

void OnlineMp3Widget::on_btn_change_clicked()
{
    // 在按钮点击时弹出换肤菜单，位置为当前鼠标位置
    menuChange->exec(QCursor::pos());
}
```

更换默认皮肤，直接自己选一张画上，更换自定义皮肤，打开文件对话框，让用户选择一张；获取当前窗口调色板

```text
void OnlineMp3Widget::backgroundtoDefaultSkin()
{
    // 获取当前窗口的调色板
    QPalette palette = this->palette();
    // 设置窗口背景为默认皮肤图片，并根据窗口大小进行缩放
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/res/123.jpg").scaled(this->size())));
    // 应用新的调色板
    this->setPalette(palette);
}

void OnlineMp3Widget::backgroundtoDingyiSkin()
{
    // 打开文件对话框，让用户选择自定义皮肤图片
    QString filepath = QFileDialog::getOpenFileName(this, "打开文件", ".", "*.jpg *.png *.jpeg");
    // 根据用户选择的图片路径创建 QPixmap 对象
    QPixmap mmp(filepath);
    // 获取当前窗口的调色板
    QPalette palette = this->palette();
    // 设置窗口背景为用户选择的自定义皮肤图片，并根据窗口大小进行缩放
    palette.setBrush(QPalette::Window, QBrush(mmp.scaled(this->size())));
    // 应用新的调色板
    this->setPalette(palette);
}
```



## 7 最小化系统托盘

就是跟qq一样有一个后台保持，主要是利用这个类QSystemTrayIcon

![img](https://pica.zhimg.com/v2-6c3f23c4a9f22e6e10e3062bda942028_1440w.jpg)

```text
//系统托盘实现
    QSystemTrayIcon *mysystemTray;
    //响应系统托盘
    void TrayIconActivate(QSystemTrayIcon::ActivationReason reason);
    void initTrayIcon();        //初始化
    void quitmucisPlayer();     //退出
```

退出功能很简单，直接quit

```text
void OnlineMp3Widget::on_btn_close_clicked()
{
    this->close();
}
```

双击托盘，显示界面和隐藏界面。TrayIconActivate用于响应系统托盘图标的激活事件。

```text
void OnlineMp3Widget::TrayIconActivate(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    // 双击系统托盘图标触发的操作
    case QSystemTrayIcon::DoubleClick:
        // 如果窗口当前为隐藏状态，则显示窗口
        if(isHidden())
        {
            show();
        }
        // 否则隐藏窗口
        else
        {
            hide();
        }
        break;

    default:
        break;
    }
}
```

初始化函数，首先先创建托盘图标，将退出和响应鼠标点击绑定槽函数，创建菜单，将退出写到菜单上。

```text
void OnlineMp3Widget::initTrayIcon()
{
    // 创建系统托盘图标对象
    mysystemTray = new QSystemTrayIcon(this);
    // 设置系统托盘图标
    mysystemTray->setIcon(QIcon(":/res/start.svg"));

    // 连接系统托盘图标的激活事件到槽函数 TrayIconActivate
    connect(mysystemTray, &QSystemTrayIcon::activated, this, &OnlineMp3Widget::TrayIconActivate);

    // 创建退出应用程序的动作
    QAction *actionquit = new QAction("退出");
    // 连接退出动作的触发事件到槽函数 quitmucisPlayer
    connect(actionquit, &QAction::triggered, this, &OnlineMp3Widget::quitmucisPlayer);

    // 创建系统托盘菜单
    QMenu *trayiconmenu = new QMenu(this);
    // 将退出动作添加到系统托盘菜单
    trayiconmenu->addAction(actionquit);
    // 将系统托盘菜单设置为系统托盘图标的上下文菜单
    mysystemTray->setContextMenu(trayiconmenu);
    // 显示系统托盘图标
    mysystemTray->show();
}
```



# 把上面的代码放在qt6编译，出现的坑

## 1.Qt6已经没有QPlayList类了，可能需要用QList< QUrl>来代替

## 2.Qt6中，QMediaPlayer的state()方法和State枚举已经改为：playbackState()和PlaybackState

## 3.QMediaPlayer的'stateChanged' 信号已经改名为playbackStateChanged



## 4.所有的音量功能已经移动到QAudioOutput，需要使用它来设置音量



## 5.QMediaPlayer的 'setMedia'方法改为setSource方法



## 6.Qt5的媒体播放器可以结束一个视频窗口，但是在qt6中，音频输出和视频输出和媒体播放器是相互独立的，你需要在播放器在单独设置音频输出和视频输出



