# 1.概览

在Qt中调用SNMP（简单网络管理协议），主要有**两种方式**：使用Qt原生网络模块（底层解析）或调用第三方C++库（如Net-SNMP或SNMP++）。最推荐的方法是**集成 Net-SNMP 或 SNMP++ 库**，因为它们提供了现成的数据打包与解析功能。 [[1](https://www.fortinet.com/cn/resources/cyberglossary/simple-network-management-protocol), [2](https://www.cnblogs.com/pied/p/7501248.html), [3](https://blog.csdn.net/han_han_dou_dou/article/details/153576334), [4](https://blog.csdn.net/qq_43611366/article/details/140528721)]

------

一、 核心方案对比

| 方案 [[1](https://blog.csdn.net/tensorflowjs6/article/details/161768474), [2](https://www.reddit.com/r/QtFramework/comments/x99st0/snmp_decoderencoder_using_qt_network/?tl=zh-hans), [3](https://blog.csdn.net/qq_43611366/article/details/140528721), [4](https://blog.csdn.net/han_han_dou_dou/article/details/153576334), [5](https://www.cnblogs.com/pied/p/7501248.html)] | 优点                                         | 缺点                                                     | 适用场景                                               |
| ------------------------------------------------------------ | -------------------------------------------- | -------------------------------------------------------- | ------------------------------------------------------ |
| **方案1：SNMP++ / Agent++**                                  | 面向对象，C++接口友好，支持V3安全加密。      | 需要自行编译第三方源码，Windows下需配置MinGW或MSVC环境。 | 深度网络管理、需要发送复杂Get/Set指令的上位机程序。    |
| **方案2：Net-SNMP (C库)**                                    | 功能极强，支持几乎所有SNMP特性，稳定性极高。 | C语言API较老，调用繁琐，内存管理容易出错。               | 需要Linux/Windows跨平台，且对稳定性要求极高的系统。    |
| **方案3：Qt QUdpSocket**                                     | 无需配置第三方库，纯Qt代码即可实现。         | 需手动处理复杂的 ASN.1 和 BER 编码解码，开发量极大。     | 仅需简单接收 Trap 告警，或轻量级、无需复杂配置的工程。 |

二、 方案1：集成 SNMP++ (主流推荐)

**SNMP++** 是一个强大且易用的 C++ 封装库。 [[1](https://blog.csdn.net/qq_43611366/article/details/140528721), [2](https://blog.csdn.net/han_han_dou_dou/article/details/153576334)]

1. 准备工作

- **下载源码**：从官网下载 SNMP++ 源码。
- **编译库文件**：使用 Qt Creator 的 MinGW 或 MSVC 编译器对源码进行编译，生成动态库或静态库（如 `.a` 或 `.lib` 文件）。 [[1](https://blog.csdn.net/qq_43611366/article/details/140528721), [2](https://blog.csdn.net/tensorflowjs6/article/details/161768474)]
- 配置 Qt 项目 (`.pro`)

将编译好的头文件和库文件引入到你的 `.pro` 工程文件中：

pro

```
INCLUDEPATH += $$PWD/include/snmp_pp
LIBS += -L$$PWD/lib -lsnmp_pp
```

 

3. C++ 调用示例 (发起 Get 请求)

cpp

```
#include <snmp_pp/snmp_pp.h>

void getSnmpData() {
    // 1. 初始化 SNMP++
    int status;
    Snmp::socket_startup();

    // 2. 创建 SNMP 会话和目标对象 (IP: 192.168.1.1, 团体名: public)
    CTarget ctarget(UdpAddress("192.168.1.1"), "public"); 
    Snmp snmp(status);

    // 3. 构造 OID (如 sysDescr.0)
    Oid oid("1.3.6.1.2.1.1.1.0");
    Pdu pdu;
    pdu += Vb(oid);

    // 4. 发送 Get 请求
    status = snmp.get(pdu, ctarget);
    
    if (status == SNMP_CLASS_SUCCESS) {
        Vb vb;
        pdu.get_vb(vb, 0);
        Oid outOid;
        vb.get_oid(outOid);
        
        // 获取返回的数值
        GenAddress addr;
        vb.get_address(addr);
        std::cout << "结果: " << addr.get_printable() << std::endl;
    }

    Snmp::socket_cleanup();
}
```

 

三、 方案2：集成 Net-SNMP (工业级选择)

Net-SNMP 是最权威的开源 SNMP 实现。在 Qt 中可以通过 `extern "C"` 的方式直接调用其 C 接口。 [[1](https://www.cnblogs.com/pied/p/7501248.html), [2](https://blog.csdn.net/han_han_dou_dou/article/details/153576334)]

1. 核心步骤

1. **安装开发包**：在 Linux 下使用 `sudo apt-get install libsnmp-dev`；Windows 下需预编译或下载 Net-SNMP 二进制包。
2. **编写代码逻辑**：使用 `snmp_sess_init()` 初始化会话，`snmp_pdu_create()` 创建 PDU，`snmp_synch_response()` 发送同步请求。 [[1](https://www.cnblogs.com/pied/p/7501248.html)]
3. 代码框架示例

cpp

```
extern "C" {
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
}

void queryNetSnmp() {
    // 1. 初始化 Net-SNMP 库
    init_snmp("qt_snmp_app");

    // 2. 配置会话参数
    snmp_session session;
    snmp_sess_init(&session);
    session.peername = strdup("192.168.1.1");
    session.community = (u_char*)"public";
    session.community_len = strlen("public");
    session.version = SNMP_VERSION_2c;

    // 3. 打开会话
    void *sess_handle = snmp_sess_open(&session);

    // ... 构造 PDU，追加 OID，发送请求 ...

    // 4. 关闭会话
    snmp_sess_close(sess_handle);
}
```

 

四、 方案3：基于 Qt 底层网络 (QUdpSocket)

如果你不想配置任何外部环境，可以纯用 Qt 自带的 `QUdpSocket` 来完成。 [[1](https://www.reddit.com/r/QtFramework/comments/x99st0/snmp_decoderencoder_using_qt_network/?tl=zh-hans)]

1. 工作原理

- SNMP 的端口是 **161**（常规请求）和 **162**（Trap告警）。
- SNMP 数据报文采用 **ASN.1 / BER** 编码格式。
- 你需要手写将数据封装为二进制格式，并通过 `writeDatagram` 发送，再将接收到的二进制流转回字符串。 [[1](https://www.reddit.com/r/QtFramework/comments/x99st0/snmp_decoderencoder_using_qt_network/?tl=zh-hans), [2](https://cloud.tencent.com/developer/article/2148539)]
- 纯 Qt UDP 发送框架

cpp

```
#include <QUdpSocket>

void sendSnmpTrap() {
    QUdpSocket udpSocket;
    QByteArray snmpData;

    // TODO: 组装 SNMP 头部及 PDU 数据 (必须符合 BER 编码)
    // snmpData.append(...); 

    // 发送到目标主机的 162 端口
    udpSocket.writeDatagram(snmpData, QHostAddress("192.168.1.1"), 162);
}
```

 

五、 关键概念避坑指南

1. **OID (对象标识符)**：类似一串数字组成的树状地址（如 `1.3.6.1.2.1...`），代表具体的设备属性，需查询对应设备的 MIB（管理信息库）文档。
2. **社区名 (Community)**：明文密码，常用的有 `public`（只读）和 `private`（读写）。
3. **SNMP 版本**：V1 和 V2c 最常用，明文传输；**V3** 支持加密与身份验证，适合安全性要求高的生产环境。
4. **乱码问题**：设备返回的中文字符通常为 GBK 或 UTF-8 编码，在 Qt 中需使用 `QString::fromLocal8Bit()` 正确显示。

# 2.参考文档1.

# [Qt 使用 net-snmp 包的过程记录](https://www.cnblogs.com/pied/p/7501248.html)

使用 C/C++ 进行 SNMP 开发，网上比较流行的主要是用 net-snmp 和 snmp++ 。在 sourceforge 上以 Qt 和 snmp 为关键词进行搜索，搜到的项目 net-snmp 相关的占了多数，推测，net-snmp 的使用人数可能更多一点。遂决定采用 net-snmp。

仍然是从 sourceforge 开始，随便找了一个规模不大的项目，开始对 net-snmp 进行熟悉。

 

**1. 尝试代码编译**

从项目把代码拉过来： git clone https://git.code.sf.net/p/qt-snmp/code qt-snmp-code

文件里没有 project 文件，使用 qt -project 生成 source.pro，qmake，make。

遇到没有 net-snmp-config.h 头文件的问题，很明显，是因为 net-snmp 库没有安装。

 

**2. 安装 libnetsnmp**

首先，因为这个代码库看起来比较早，所以选择了一个比较早期的 net-snmp 版本（2011）。snmp 已经是非常成熟的协议，所以，并不担心比较早的版本协议实现不完整。

从 sourceforge 下载代码包 net-snmp-5.4.4.tar.gz，并解压，进入代码根目录。

因为 net-snmp 的部分实现严重依赖 perl，所以，需要先安装 perl 的开发包：

然后进行 configure , make , make install.

另外，net-snmp 还依赖 openssl。在这里花了比较长的时间。因为我的交叉编译器只有 openssl 头文件，并没有库文件，而且我编译 net-snmp 又是用的静态库，所以，链接的时候老是提示没有一些加密函数。下面是我重新编译 openssl 的 config 配置(用从 ubuntu 下 的 openssl098_0.9.8o.orig.tar.gz)：

```
CC=arm-linux-gcc ./config no-asm --prefix=/tmp/openssl
```

为 arm 编译 net-snmp 库使用的 configure 选项：

```
 ../configure --host=arm-linux --target=arm-linux --build=i686-linux --disable-shared --disable-scripts -enable-mini-agent --disable-ipv6 --disable-manuals --disable-ucd-snmp-compatibility --enable-as-needed --with-endianness=little --prefix=/tmp/snmp/
```

 

**3. 继续编译 demo 的代码**

因为已经安装了 libnetsnmp，而且代码要用到这个包，所以需要修改 project 文件，添加：

```
LIBS +=-lnetsnmp
```

make，成功。

暂时没有测试的环境，不知道程序是否有效，暂时先阅读一下代码。

 

**4. 阅读项目代码**

读完代码发现，真的只是写了一个最基本的 demo，界面倒是看起来做了一堆。甚至怀疑他这个最基本功能有没有实现，暂时没法测试，学习一下他的过程。

最主要业务代码，是在 snmpGet() 函数里，全文如下：

[![复制代码](https://assets.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
void MainWindow::SnmpGet() {
  init_snmp("snmp get");
  struct snmp_session sessionToPeer;
  snmp_sess_init(&sessionToPeer);

  sessionToPeer.peername = strdup(agentDeviceAddressLineEdit->text().toStdString().c_str());
  /*memory allocated by strdup() will be freed by calling snmp_close() */
  if(snmpVersion1RadioButton->isChecked()) {/* snmp version 1 is obsolete, do nothing about it. */
  }
  if(snmpVersion2RadioButton->isChecked()) {/* only version 2 community is implemented here */
    sessionToPeer.version = SNMP_VERSION_2c;
    sessionToPeer.community = (u_char*) (strdup(communityLineEdit->text().toStdString().c_str()));
    sessionToPeer.community_len = strlen((const char*) sessionToPeer.community);
  }
  if(snmpVersion3RadioButton->isChecked()) { //TODO: implement SNMP version 3 options. more item may be needed to add to combo box.

  }
  sessionToPeer.retries = retriesSpinBox->value();
  sessionToPeer.timeout = timeoutSpinBox->value();
  SOCK_STARTUP;
  struct snmp_session* sessionReturnedByLibrary = snmp_open(&sessionToPeer);
  if(sessionReturnedByLibrary == NULL) {
#ifdef QT_DEBUG
    snmp_sess_perror((const char*) "No Ack!", sessionReturnedByLibrary);
#endif //QT_DEBUG
    SOCK_CLEANUP;
    return;
  }
  struct snmp_pdu* requestPdu = snmp_pdu_create(SNMP_MSG_GET);
  oid requestOid[MAX_OID_LEN];
  size_t requestOidLength = MAX_OID_LEN;
  snmp_parse_oid(".1.3.6.1.2.1.1.1.0", requestOid, &requestOidLength);
  snmp_add_null_var(requestPdu, requestOid, requestOidLength);
  struct snmp_pdu* responsePdu = NULL;
  int snmpStatus = snmp_synch_response(sessionReturnedByLibrary, requestPdu, &responsePdu);
  if(snmpStatus == STAT_SUCCESS and responsePdu->errstat == SNMP_ERR_NOERROR) {
    /* SUCCESS: Print the result variables */
    struct variable_list *snmpVariables;
#ifdef QT_DEBUG
    for(snmpVariables = responsePdu->variables; snmpVariables; snmpVariables = snmpVariables->next_variable) {
      print_variable(snmpVariables->name, snmpVariables->name_length, snmpVariables);
    }
#endif //QT_DEBUG
    /* retrieve response that we're interested. */
#ifdef QT_DEBUG
    int count = 1;
#endif //QT_DEBUG
    for(snmpVariables = responsePdu->variables; snmpVariables != NULL; snmpVariables = snmpVariables->next_variable) {
      if(snmpVariables->type == ASN_OCTET_STR) {
        char* response = (char *) malloc(1 + snmpVariables->val_len);
        memcpy(response, snmpVariables->val.string, snmpVariables->val_len);
        response[snmpVariables->val_len] = '\0';
#ifdef QT_DEBUG
        printf("value #%d is a string: %s\n", count++, response);
#endif //QT_DEBUG
        resultTextEdit->setText(QString(response));
        free(response);
      } else {
#ifdef QT_DEBUG
        printf("value #%d is NOT a string! Ack!\n", count++);
#endif //QT_DEBUG
      }
    }
  } else {
    /* FAILURE: print what goes wrong! */
#ifdef QT_DEBUG
    if(snmpStatus == STAT_SUCCESS) {
      fprintf(stderr, "Error in packet\nReason: %s\n", snmp_errstring(responsePdu->errstat));
    } else if(snmpStatus == STAT_TIMEOUT) {
      fprintf(stderr, "Timeout: No response from %s.\n", sessionToPeer.peername);
    } else {
      snmp_sess_perror("snmp get", sessionReturnedByLibrary);
    }
#endif //QT_DEBUG
  }

  /*
   * Clean up:
   *  1) free the response.
   *  2) close the session.
   */
  if(responsePdu) {
    snmp_free_pdu(responsePdu);
  }
  snmp_close(sessionReturnedByLibrary);

  SOCK_CLEANUP;
}
```

[![复制代码](https://assets.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 **大致步骤：**

1) 对 snmp 协议栈进行初始化，init_snmp();
2) 新建 snmp 会话，对 session 进行初始化, 并对 session 进行基本的设置，比如 session 使用的协议、session 的重试次数以及等待时间等;
3) 使用 snmp_create_pdu( MSG_TYPE) 来组装 request_pdu。查看这个版本的协议，支持的 pdu 类型有：

[![复制代码](https://assets.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
    /*
     * PDU types in SNMPv1, SNMPsec, SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 
     */
#define SNMP_MSG_GET        (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0) /* a0=160 */
#define SNMP_MSG_GETNEXT    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1) /* a1=161 */
#define SNMP_MSG_RESPONSE   (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2) /* a2=162 */
#define SNMP_MSG_SET        (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3) /* a3=163 */

    /*
     * PDU types in SNMPv1 and SNMPsec 
     */
#define SNMP_MSG_TRAP       (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x4) /* a4=164 */

    /     * PDU types in SNMPv2p, SNMPv2c, SNMPv2u, SNMPv2*, and SNMPv3 ＊/
#define SNMP_MSG_GETBULK    (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5) /* a5=165 */
#define SNMP_MSG_INFORM     (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6) /* a6=166 */
#define SNMP_MSG_TRAP2      (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7) /* a7=167 */

    /*
     * PDU types in SNMPv2u, SNMPv2*, and SNMPv3 
     */
#define SNMP_MSG_REPORT     (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x8) /* a8=168 */
```

[![复制代码](https://assets.cnblogs.com/images/copycode.gif)](javascript:void(0);)

为 request_pdu 指定 oid（这里指定固定的 oid，mib 文件并没有使用）。

4) 使用 nmp_synch_response(sessionReturnedByLibrary, requestPdu, &responsePdu) 来出发请求，并获取 resopose_pdu.
5) 当正确的获取 response_pdu 之后，在一个 for 循环中历遍 response_pdu 中的 netsnmp_variable_list，将所有 variables 的 value 都按照字符串打印出来。
6) 到所有的最后，关闭 session，删除 pdu。

以上，完成了一个基本的 snmp_get 请求。



[« ](https://www.cnblogs.com/pied/p/7283669.html)上一篇： [Qt 立体水晶按键实现](https://www.cnblogs.com/pied/p/7283669.html)

# 3.参考文档2： 

### https://juejin.cn/post/7459742046304780351



# 4.参考文档3： Snmp在Windows下的实现----WinSNMP编程原理

在Windows 下实现SNMP协议的编程，可以采用Winsock接口，在161，162端口通过udp传送信息。在Windows 2000中，Microsoft已经封装了SNMP协议的实现,提供了一套可供在Windows下开发基于SNMP的网络管理程序的接口，这就是 WinSNMP API。

3.1 什么是WinSNMP

WinSNMP的目的是为在Windows下开发基于SNMP的网络管程序提供解决方案。它为SNMP网管开发者提供了必须遵循的开放式单一接口规范，它定义了过程调用、数据类型、数据结构和相关的语法。
图3.1显示了一个网络管理站（NMS）和网络管理代理（Agent）之间端到端的SNMP连接中WinSNMP所处的层次。这是一个WinSNMP的参考模型。

 

图3.1WinSNMP参考模型


总的来说，WinSNMP以函数的形式封装了SNMP协议的各部分（在VC++6.0开发环境中体现为wsnmp32.dll、wsnmp32.lib和winsnmp.h），且针对SNMP是使用UDP的特点而设置了消息重传、超时机制等。

3.2 一些基本概念

在WinSNMP编程中，我们需要考虑的基本概念主要有以下几点：
 SNMP支持层次
 Entity/Context转换模式
 本地数据库
 会话
 异步模式
 内存管理
下面我们将分别对它们作介绍。

3.2.1 SNMP支持层次（Levels of SNMP Support）
WinSNMP支持四个层次的SNMP操作：
 Level 0 = 只有消息编码/解码
 Level 1 = Level 0 + 与SNMPv1代理的通信
 Level 2 = Level 1 + 与SNMPv2代理的通信
 Level 3 = Level 2 + 与其它SNMPv2管理站的通信
因为SNMP协议支持SNMPv1与SNMPv2的共存，所以WinSNMP实现能提供对两个版本协议的支持。
SnmpStartup函数能返回当前WinSNMP实现所能提供的最大支持层次。

3.2.2 Entity/Context转换模式（Entity/Context Translation Modes）
WinSNMP应用程序能够让WinSNMP实现把entity和context参数按不同的方式解释：
（1）按字面解释为SNMPv1代理的地址和共同体（community）字符串。
（2）解释为SNMPv2的party和context标识符（context IDs）。
（3）通过查询本地数据库将其转换为各自的SNMPv1或SNMPv2元素。
三种Entity/Context转换模式如下：
SNMPAPI_TRANSLATED = 通过本地数据库查询转换
SNMPAPI_UNTRANSLATED_V1 = 转换为地址和共同体（community）字符串
SNMPAPI_UNTRANSLATED_V2 = SNMPv2的party和context IDs.
我们可以通过SnmpStartup函数获得当前默认的entity/context转换模式，SnmpSetTranslatedMode函数可以用来设置entity/context转换模式。
当在系统中采用SNMPv1协议时，我们可以将其设置为SNMPAPI_UNTRANSLATED_V1，具体实现如下：
HSNMP_ENTITY hAgent;
HSNMP_CONTEXT hView;
LPCSTR entityName = “202.120.86.71”;
smiOCTETS contextName;
contextName.ptr = “public”;
contextName.len = lstrlen (contextName.ptr);
hAgent = SnmpStrToEntity (hSomeSessin, entityName);
hView = SnmpStrToContext (hSomeSession, const &contextName);
通过这样的设置，我们就可以在161端口通过UDP访问IP地址“202.120.86.71”上的SNMP代理了。

3.2.3 本地数据库（Local Database）
本地数据库主要存储重传模式（RetransmitMode）、重试次数（Retry）、超时（timeout）、转换模式（TranslateMode）等值。我们可以对其中的数据进行读（get）、写（set）操作。

3.2.4 会话（session）
会 话是用来管理WinSNMP应用程序和WinSNMP实现之间的连接，由SnmpCreateSession(推荐)或SnmpOpen函数创建。会话是 资源管理的最小单位，也是WinSNMP应用程序和WinSNMP实现之间通信管理的最小单位。一个良好的WinSNMP应用程序应该使用会话结构逻辑地 管理它的操作，并将实现中的资源需求控制在最小。
调用SnmpCreateSession或SnmpOpen函数创建一个会话时，会返回一个“session id”,这是一个句柄（handle）变量，WinSNMP用它来管理自己的资源。应用程序最终应调用SnmpClose函数将会话释放。

3.2.5 异步模式（Asynchronous Model）
当代编程模式的一个很大特点就是消息驱动。WinSNMP采用了异步消息驱动模式，主要基于两个原因：
(1) 异步消息驱动模式非常适合于面向对象理论、SNMP分布式管理模型以及Windows编程、运行环境。
(2) SNMP再管理站和代理之间传送数据没有什么特别的传输机制，它基本上是基于数据报的，没有在远程实体之间建立实际通道（虚电路）。这样的事实使得WinSNMP非常适合采用异步模式。
现代的消息驱动程序必须响应各种重要事件，有些则完全依赖于异步关系。事实上，WinSNMP API中几乎所有函数都有异步成分，有些则是完全异步的。有三个非常重要的异步函数：
 SnmpSendMsg (发送数据)
 SnmpRecvMsg (接收数据)
 SnmpRegister (注册接受trap消息)
WinSNMP的整个编程模式就是基于异步的，我们将在后面做详细介绍。

3.2.6 内存管理（Memory Management）
在Windows编程中，内存管理一向是一个令人头疼的问题。在这里，我们将对WinSNMP的内存管理做一个较为详尽的描述。
WinSNMP包括三种不同的内存“对象”：
 句柄式资源 (HANDLE’d Resources)
 C风格(以NULL结尾)的字符串
 WinSNMP API结构类型

3.2.6.1 句柄式资源 (HANDLE’d Resources)
有五种句柄式资源的变量：
 Sessions
 Entities
 Contexts
 Protocol Data Units (PDUs)
 VarBindLists (VBLs)
所有句柄对象都表示为“HSNMP_<object_tag>”的形式，它为WinSNMP实现（以DLL方式）所拥有。

3.2.6.2 C风格字符串 (C-Stytle Strings)
C 风格的字符串主要用来为通用的字符串表示与Entity和对象标识符(OID)对象之间的转换提供便利。WinSNMP中使用C风格字符串的函数有： SnmpStrToEntity、SnmpEntityToStr、SnmpStrToOid、SnmpOidToStr。
C风格字符串的内存分配、管理和释放完全由应用程序负责。因此我们还需要传递“size”参数给使用它的函数。

3.2.6.3 描述符 (Descriptors)
WinSNMP中有三种结构类型：
 smiOCTETS
 smiOID
 smiVALUE
前两种类型的定义如下：
typedef struct {
   smiUINT32 len; /*unsigned long integer 类型，表示ptr中的字节数*/
   smiLPBYTE ptr; /*指向包含octet string的字节数组的far指针*/
} smiOCTETS；

typedef struct {
smiUINT32  len; /**unsigned long integer 类型，表示ptr中无符号长整形的个数*/
   smiLPUINT32 ptr;  /*指向由OID各个标识符组成的无符号长整形数祖的far指针*/
} smiOID；

smiVALUE稍微复杂一点，它的定义如下：
typedef struct {   /* smiVALUE portion of VarBind */
smiUINT32 syntax;  /* Insert SNMP_SYNTAX_<type> */
union {
smiINT  sNumber; /* SNMP_SYNTAX_INT
     SNMP_SYNTAX_INT32 */
smiUINT32 uNumber; /* SNMP_SYNTAX_UINT32
                SNMP_SYNTAX_CNTR32                  SNMP_SYNTAX_GAUGE32                  SNMP_SYNTAX_TIMETICKS */
smiCNTR64 hNumber; /* SNMP_SYNTAX_CNTR64 */
smiOCTETS string;  /* SNMP_SYNTAX_OCTETS
    SNMP_SYNTAX_BITS
    SNMP_SYNTAX_OPAQUE
    SNMP_SYNTAX_IPADDR
    SNMP_SYNTAX_NSAPADDR */
smiOID  oid;  /* SNMP_SYNTAX_OID */
smiBYTE empty;  /* SNMP_SYNTAX_NULL
    SNMP_SYNTAX_NOSUCHOBJECT
    SNMP_SYNTAX_NOSUCHINSTANCE
    SNMP_SYNTAX_ENDOFMIBVIEW */
     }  value;  /* union */
}  smiVALUE;

当一个应用程序得到一个smiVALUE变量时，首先必须检查它的“syntax”成员，已决定怎样取到它的第二个成员。
当“syntax”成员变量显示“value”值是一个smiOCTETS或smiOID对象时，我们就应该考虑内存管理，约定如下：
(1) 当其作为输入参数时，应用程序负责为变长对象分配内存；
(2) 当其作为输出参数时，由WinSNMP实现(表现为DLL)为变长对象分配
内存。

3.2.6.4 内存的释放
WinSNMP应用程序必须负责释放所有通过调用WinSNMP API函数所分配的资源，主要有以下三类函数：
 SnmpFree<xxx>: 释放Entity、Context、Pdu、Vbl、Descriptor
 SnmpClose  : 关闭会话
 SnmpCleanup : 必须在程序结束之前调用，释放所有资源
应用程序推荐使用上述的顺序来释放所有的WinSNMP资源。

3.3 WinSNMP基本编程模式

WinSNMP API按照SNMP协议封装了各种操作，包括PDU、VarBindList以及协议操作的各项函数。我们可以按照SNMP协议的描述，调用 WinSNMP相关函数，完成一次完整的SNMP。我们下面将以笔者完整的系统(采用SNMPv1协议)为例，具体描述WinSNMP的一般编程模式。我 们分发送请求消息与接受响应消息两部分来实现。

3.3.1 WinSNMP发送请求消息
WinSNMP发送请求消息的过程可以分为四个部分，主要有：WinSNMP的初始化、PDUs的创建、发送信息以及资源的释放。

3.3.1.1 WinSNMP的初始化
(1) 调用SnmpStartup函数启动WinSNMP。
(2) 调用SnmpCreateSession函数创建一个会话session。
(3) 调用SnmpSetRetransmitMode函数设置重传模式。
(4) 调用SnmpSetRetry函数设置重传次数。
(5) 调用SnmpSetTimeout函数设置超时时间。
其中第3、4、5步都是对本地数据库的操作，完成了对WinSNMP相关参数的设置。

3.3.1.2 创建协议数据单元（PDUs）
在创建PDU之前，我们必须先创建变量绑定表(varbindlists)。
(1) 调用SnmpStrToOid函数创建读取对象的OID，例如，我们创建MIB变量ipInReceives(一个实例的OID为1.3.6.1.2.1.4.3.0)，我们可以采用下面的代码：
LPCSTR name="1.3.6.1.2.1.4.3.0"；
smiOID Oid;
SnmpStrToOid(name,&Oid)；

(2) 调用SnmpCreateVbl函数创建变量绑定表。
HSNMP_VBL m_hvbl=SnmpCreateVbl(session,&Oid,NULL);/*NULL表示该OID的值为空*/
(3) 调用SnmpSetVb函数往变量绑定表中添加变量绑定，我们需先创
建一个OID,命名为Oid。
SnmpSetVb(m_hvbl,0,&Oid,NULL)；/*0表示往变量绑定表中添加变量绑定，非0值表示修改此位置的变量绑定*/
创建好了变量绑定表后，我们调用SnmpCreatePdu函数创建协议数据单元，在这个函数中，我们必须设定error_index、error_status、request_id参数，它们都与协议中相应的量对应。
HSNMP_PDU m_hpdu=SnmpCreatePdu(session,SNMP_PDU_GET,
NULL,NULL,NULL,m_hvbl);

3.3.1.3 发送信息
我们首先调用SnmpStrToContext和SnmpStrToEntity函数创建共同体(community)字符串和代理entity,具体实现见3.2.2。
然后，我们调用SnmpSendMsg函数发送信息。
SnmpSendMsg(session,NULL,hAgent,hView,m_hpdu)；

3.3.1.4 资源的释放
最后，我们应该释放所有分配的资源。

3.3.2 WinSNMP接受响应消息
还记得前面的SnmpCreateSession函数吗？它可以说是WinSNMP异步消息驱动模式的一个关键，让我们先来看看它的函数原型：
HSNMP_SESSION SnmpCreateSession(
HWND hWnd,          // handle to the notification window
UINT wMsg,          // window notification message number
SNMPAPI_CALLBACK fCallback,  // notification callback function
LPVOID lpClientData      // pointer to callback function data
);
它提供了两种方式的异步消息驱动，我们可以让WinSNMP在有响应消息到达时发送一个消息给系统，也可以让它自动调用一个函数。笔者采用了第一种方式，实现如下：
session=SnmpCreateSession(m_hWnd,wMsg,NULL,NULL);
我们可以给消息wMsg创建一个消息处理函数，在这个函数里处理消息的接收、信息的提取与处理等事务。
下面我们将具体描述WinSNMP接受响应消息的步骤。
(1) 调用SnmpRecvMsg函数接收数据
(2) 调用SnmpGetPduData函数从PDU中析取出数据，
(3) 调用SnmpCountVbl获得变量绑定列表中变量绑定的个数
(4) 调用SnmpGetVb函数取得PDU变量绑定表中每个变量绑定的OID及其对应的值，可以指明该变量绑定在变量绑定表中的位置。参考实现如下：
int nCount=SnmpCountVbl(varbindlist)；
for(int index=1;i<=nCount;i++)
SnmpGetVb(varbindlist,index,&Oid,value[i]);
其中，index指定了变量绑定的位置，value[i]表示接收到的OID变量的值，是smiLPVALUE类型的，Oid表示接收到的变量绑定的OID。
对于value[i]，我们可以参考3.2.6.3节，按照它的syntax成员，用select case语句，分别转换为字符串或整数类型。
(5) 调用SnmpOidToStr函数将Oid转换为字符串。并将接收到的Oid与发送数据包的各OID做比较，已决定各自值的归属。引用一段代码
if(strcmp(m_sOid[i],m_initOid[1])==0)
  m_sDesr= str[i];
else if (strcmp(m_sOid[i],m_initOid[2])==0)
  m_sSysOid=str[i];
else if (strcmp(m_sOid[i],m_initOid[3])==0)
  m_sSysTime=str[i];
else if (strcmp(m_sOid[i],m_initOid[4])==0)
  m_sName=str[i];
else if (strcmp(m_sOid[i],m_initOid[5])==0)
  {m_sIpin=str[i];
  m_nIpin=nIpin;}
else if(strcmp(m_sOid[i],m_initOid[6])==0)
  m_sIpout=str[i];
当我们比较发送的OID与接收到的OID时，我们就知道了这个str[i]是属于哪个OID的值，应当放在哪里显示，以m_s开头的变量都代表了不同的label,这样，相应的值就在相应的字符串中显示。

通 过这样的步骤，我们就完成了一个简单的SNMP网络管理程序的设计。但是，在具体的应用中，我们应该考虑更多的问题，如内存管理、错误处理等问题，还有很 多问题需要我们在系统开发的过程中去发现、解决。下面，我将描述几个我在系统开发中遇到的问题，有的已经解决，有的还在探索中，希望能为同仁提供参考。

3.4 几个问题

3.4.1 读IP地址
前面讲到，IpAddress是SMIv1的一个应用数据类型，表示IP地址，它的定义为：
IpAddress::=[APPLICATION 0] IMPLICIT OCTET STRING(SIZE(4))
当我们读取一个表示IP地址的OID时，我们应该分别读出IpAddress四个字节的值，再将它们处理成我们平时见到的IP地址的形式。代码如下：
case SNMP_SYNTAX_IPADDR:
strIp.Format("%d",*m_value[i]->value.string.ptr);
  strIp+=".";
  strTemp.Format("%d",*(m_value[i]->value.string.ptr+1));
  strIp+=strTemp;
  strIp+=".";
  strTemp.Format("%d",*(m_value[i]->value.string.ptr+2));
  strIp+=strTemp;
  strIp+=".";
  strTemp.Format("%d",*(m_value[i]->value.string.ptr+3));
  strIp+=strTemp;

3.4.2 GETNEXT操作的实现
GETNEXT是SNMP中用来读取表格变量的一个操作。在WinSNMP中，我们可以通过SnmpCreatePdu(session,SNMP_PDU_GETNEXT,NULL,NULL,NULL,m_hvbl)来创建一个GETNEXT操作的PDU。
关键的问题是我们如何对这个表格作遍历。(1).如何判断表格的结束；(2).在接收到响应消息时如何处理。
我们下面将以笔者系统为例，说明这些问题。我们将获得本机的路由表的一部分。先构造一个函数，代码如下：
void CSnmpManagerDlg::Next(LPTSTR Oid)
{
CString str(Oid);
if(!strcmp(str.Left(20),"1.3.6.1.2.1.4.21.1.7"))
{
 file://处理接收到的数据
  pSnmp.CreateVbl(Oid,NULL);
  pSnmp.CreatePdu(SNMP_PDU_GETNEXT,NULL,NULL,NULL);
  pSnmp.Send("127.0.0.1","public");
}
else 
{
  m_bNext=FALSE;
 file://送去显示
}
}
我们把接收到的OID的前20位与路由next hop MIB变量（"1.3.6.1.2.1.4.21.1.7"）作比较，假如不等，就说明这一列已经结束。把数据送去显示或进一步处理。
我们可以为这一操作创建一个新的会话(session)，或继续使用前面GET操作的会话。创建一个新的会话时，我们为这个会话指定一个消息处理函数，并在这个函数中，处理接收到的数据，以及调用Next(LPTSTR Oid)函数继续发送GETNEXT操作。
假如继续使用以前的会话，我们要依靠标志m_bNext，判断m_bNext的真假以决定是否继续发GETNEXT数据包。
void CSnmpManagerDlg::OnRecv()
{ file://接收、处理消息
if(m_bNext==TRUE)
  Next(m_sOid);
}
这 样，我们就完成了对表格中一列的遍历。同样，我们可以完成对整个表格的遍历，我们只需strcmp(str.Left(18), "1.3.6.1.2.1.4.21.1")，就可以获得整个表格的结束。再在Next(LPTSTR Oid)函数中用switch-case语句按各个MIB变量的值分类，就可以得到整个表格。

3.4.3 对表格变量的SET操作
在整个系统的开发中，我们曾经对SysName变量进行SET操作。证明是可行的。但当我们SET一个表格变量时，报告变量绑定（VB）错误，类型为bad value。可能有两个原因。
(1). 代理进程(Agent)不支持对这些表格变量的SET操作。(具体见RFC1212)
(2). 当SET一个表格变量时，我们应该对表格中的所有变量都赋值，并封装成一个PDU发出去。因为当我们用route add添加路由表时，必须指定所有的参数。并且，表格变量只允许添加与删除两种操作。

