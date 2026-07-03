# 一.TCP协议的原理以及编程模型

## 课程截图

![image-20260703134154752](./note04-基于tcp的客户端服务器架构.assets/image-20260703134154752.png)

![image-20260703134426492](./note04-基于tcp的客户端服务器架构.assets/image-20260703134426492.png)

![image-20260703134534847](./note04-基于tcp的客户端服务器架构.assets/image-20260703134534847.png)



![image-20260703134649665](./note04-基于tcp的客户端服务器架构.assets/image-20260703134649665.png)

## tcp的3次握手和4次挥手

TCP 的 3 次握手用于**建立连接**，4 次挥手用于**断开连接**。它们通过发送特殊的控制信号（如 SYN、FIN、ACK），确保双方的收发通道畅通、数据传输完整。以下为您详细拆解这两个过程。 [[1](https://www.cnblogs.com/three-fighter/p/14802786.html), [2](https://cloud.tencent.com/developer/article/2622452), [3](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]

一、 TCP 3 次握手（建立连接）

握手就像两个人通电话打招呼，必须确认对方能听见自己说话。 [[1](https://github.com/febobo/web-interview/issues/151)]

- **第 1 次握手**：客户端向服务端发送连接请求（包含 SYN 标志和随机初始序列号）。
  - *状态*：客户端进入 `SYN_SENT` 状态。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]
- **第 2 次握手**：服务端收到请求，回复确认信息（包含 SYN、ACK 标志，以及自己的初始序列号）。
  - *状态*：服务端进入 `SYN_RCVD` 状态。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]
- **第 3 次握手**：客户端收到确认，再次向服务端回复确认信息（包含 ACK 标志）。
  - *状态*：双方进入 `ESTABLISHED` 状态，连接建立成功。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]

> **为什么需要 3 次？**
> 这是为了防止由于网络延迟，旧的连接请求突然到达服务端而建立错误连接。3 次握手能确保双方都明确“对方已收到自己的信息”，缺一不可。 [[1](https://javaguide.cn/cs-basics/network/tcp-connection-and-disconnection.html)]

二、 TCP 4 次挥手（断开连接）

TCP 是全双工的（双向通道），因此每一方必须单独关闭自己的发送通道。挥手就像双方说“再见”，需要双方各自确认。 [[1](https://javaguide.cn/cs-basics/network/tcp-connection-and-disconnection.html), [2](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]

- **第 1 次挥手**：主动方（如客户端）发送结束请求（包含 FIN 标志），表示自己没有数据要发送了。
  - *状态*：客户端进入 `FIN_WAIT_1` 状态。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]
- **第 2 次挥手**：被动方（如服务端）收到请求，回复确认信息（包含 ACK 标志）。
  - *状态*：服务端进入 `CLOSE_WAIT` 状态。此时客户端已经不能发数据了，但服务端仍可以发。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]
- **第 3 次挥手**：服务端处理完所有剩余数据后，向客户端发送结束请求（包含 FIN 标志）。
  - *状态*：服务端进入 `LAST_ACK` 状态。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]
- **第 4 次挥手**：客户端收到请求，回复确认信息（包含 ACK 标志）。
  - *状态*：客户端进入 `TIME_WAIT` 状态。等待一段时间后彻底关闭。 [[1](https://blog.sunlan.me/2024/05/18/浅谈TCP三次握手和四次挥手/)]

> **为什么要等待 2MSL？**
> 在第 4 次挥手后，主动关闭方必须等待 2 倍的报文最大生存时间（MSL）。这是为了确保对方能收到最后的确认包。如果确认包丢失，对方重发 FIN，主动方还能再次回复。

# 二.QTcpServer类和QTcpSocket类

## 1.官方示例： https://doc.qt.io/qt-6/zh/examples-network.html



## 参考文档1.

CP/[IP协议](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=IP协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJJUOWNj-iuriIsInpoaWRhX3NvdXJjZSI6ImVudGl0eSIsImNvbnRlbnRfaWQiOjIzMTE1MTYwOSwiY29udGVudF90eXBlIjoiQXJ0aWNsZSIsIm1hdGNoX29yZGVyIjoxLCJ6ZF90b2tlbiI6bnVsbH0.sppto27LyqTPql81XYC06w1SGlMvGJi_cpoclReXNLs&zhida_source=entity)并不是单纯的俩个协议，而是一个很大的协议栈的总称。TCP/IP 协议栈是构成网络通信的核心骨架，它定义了电子设备如何连入因特网，以及数据如何在它们之间进行传输。TCP/IP 协议采用4层结构，分别是应用层、传输层、网络层和链路层，每一层都呼叫它的下一层所提供的协议来完成自己的需求。下面我们来看TCP/IP的四层结构。网络协议有[OSI七层协议](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=OSI七层协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJPU0nkuIPlsYLljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.o_yqSHoFDWAK09_z7ATq22TxdZ7Q8te3bNh-lZGLX9M&zhida_source=entity)和TCP/IP四层协议，还有一个五层协议，其实四层协议可以看做是七层协议的简化版。

## **一、TCP/IP整体概念**

### **1.1物理介质**

物理介质就是把电脑连接起来的物理手段，常见的有光纤、双绞线，以及无线电波，它决定了电信号(0和1)的传输方式，物理介质的不同决定了电信号的传输带宽、速率、传输距离以及抗干扰性等等。 [TCP/IP协议栈](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=TCP%2FIP协议栈&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJUQ1AvSVDljY_orq7moIgiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.AOYdYvd-IzS5PGf3dG33AuyUD7K-VdmkBpJH7Q2oRuY&zhida_source=entity)分为四层，每一层都由特定的协议与对方进行通信，而协议之间的通信最终都要转化为 0 和 1 的电信号，通过物理介质进行传输才能到达对方的电脑，因此物理介质是网络通信的基石。

下面我们先通过一张图先来大概了解一下TCP/IP协议的基本框架以及数据的处理流程：

![img](https://pic2.zhimg.com/v2-6e5c0c7bb2a175f84f28be8ddba7c759_1440w.jpg)



当通过http发起一个请求时，应用层、传输层、网络层和链路层的相关协议依次对该请求进行包装并携带对应的首部，最终在链路层生成以太网数据包，以太网数据包通过物理介质传输给对方主机，对方接收到数据包以后，然后再一层一层采用对应的协议进行拆包，最后把应用层数据交给应用程序处理。

网络通信就好比送快递，商品外面的一层层包裹就是各种协议，协议包含了商品信息、收货地址、收件人、联系方式等，然后还需要配送车、配送站、快递员，商品才能最终到达用户手中。

一般情况下，快递是不能直达的，需要先转发到对应的配送站，然后由配送站再进行派件。

配送车就是物理介质，配送站就是网关， 快递员就是路由器，收货地址就是IP地址，联系方式就是MAC地址。

快递员负责把包裹转发到各个配送站，配送站根据收获地址里的省市区，确认是否需要继续转发到其他配送站，当包裹到达了目标配送站以后，配送站再根据联系方式找到收件人进行派件。

### **1.2链路层**

网络通信就是把有特定意义的数据通过物理介质传送给对方，单纯的发送 0 和 1 是没有意义的，要传输有意义的数据，就需要以字节为单位对 0 和 1 进行分组，并且要标识好每一组电信号的信息特征，然后按照分组的顺序依次发送。以太网规定一组电信号就是一个数据包，一个数据包被称为一帧， 制定这个规则的协议就是[以太网协议](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=以太网协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiLku6XlpKrnvZHljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.nm5Twi4CuzpdHmCNbxmpLRLzpxFVjJ4MUUCMmnCoDdA&zhida_source=entity)。

以太网规协议定，接入网络的设备都必须安装网络适配器，即网卡， 数据包必须是从一块网卡传送到另一块网卡。而网卡地址就是数据包的发送地址和接收地址，也就是帧首部所包含的MAC地址，MAC地址是每块网卡的身份标识，就如同我们身份证上的身份证号码，具有全球唯一性。

有了MAC地址以后，以太网采用广播形式，把数据包发给该子网内所有主机，子网内每台主机在接收到这个包以后，都会读取首部里的目标MAC地址，然后和自己的MAC地址进行对比，如果相同就做下一步处理，如果不同，就丢弃这个包。

所以链路层的主要工作就是对电信号进行分组并形成具有特定意义的数据帧，然后以广播的形式通过物理介质发送给接收方。

### 1.3**网络层**

对于上面的过程，肯定会产生下面几个疑问：

```text
1.发送者如何知道接收者的MAC地址？

2.发送者如何知道接收者和自己同属一个子网？

3.如果接收者和自己不在同一个子网，数据包如何发给对方？
```

为了解决这些问题，网络层引入了三个协议，分别是IP协议、[ARP协议](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=ARP协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJBUlDljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.mhqhy157GHumAECaZ1fYO-LfewMVG08aqd568BZ2PSU&zhida_source=entity)、路由协议。 IP协议制定了IP地址来判断俩个主机是否属于同一个子网。 ARP协议根据IP地址获取MAC地址。 路由协议根据信道情况，选择并设定路由，以最佳路径来转发数据包。

所以，网络层的主要工作是定义网络地址，区分网段，子网内MAC寻址，对于不同子网的数据包进行路由。

### 1.4**传输层**

链路层定义了主机的身份，即MAC地址， 而网络层定义了IP地址，明确了主机所在的网段，有了这两个地址，数据包就从可以从一个主机发送到另一台主机。但实际上数据包是从一个主机的某个应用程序发出，然后由对方主机的应用程序（进程）接收。而每台电脑都有可能同时运行着很多个应用程序（进程），所以当数据包被发送到主机上以后，是无法确定哪个应用程序（进程）要接收这个包。传输层提供了进程间的逻辑通信，传输层向高层用户屏蔽了下面网络层的核心细节，使应用程序看起来像是在两个传输层实体之间有一条端到端的逻辑通信信道。

### **1.5应用层**

理论上讲，有了以上三层协议的支持，数据已经可以从一个主机上的应用程序传输到另一台主机的应用程序了，但此时传过来的数据是字节流，不能很好的被程序识别，操作性差。因此，应用层定义了各种各样的协议来规范数据格式，常见的有 HTTP、FTP、SMTP 等。

### **1.6四层协议整体流程**

链路层：对0和1进行分组，定义数据帧，确认主机的物理地址，传输数据； 网络层：定义IP地址，确认主机所在的网络位置，并通过IP进行MAC寻址，对外网数据包进行路由转发； 传输层：定义端口，确认主机上应用程序的身份，并将数据包交给对应的应用程序； 应用层：定义数据格式，并按照对应的格式解读数据。

**用一句话来概括就是：**当你输入一个网址并按下回车键的时候，首先，应用层协议对该请求包做了格式定义；紧接着传输层协议加上了双方的端口号，确认了双方通信的应用程序；然后网络协议加上了双方的IP地址，确认了双方的网络位置；最后链路层协议加上了双方的MAC地址，确认了双方的物理位置，同时将数据进行分组，形成数据帧，采用广播方式，通过传输介质发送给对方主机。而对于不同网段，该数据包首先会转发给网关路由器，经过多次转发后，最终被发送到目标主机。目标机接收到数据包后，采用对应的协议，对帧数据进行组装，然后再通过一层一层的协议进行解析，最终被应用层的协议解析并交给服务器处理。

## 二、传输层

传输层（Transport Layer）是ISO OSI协议的第四层协议，实现端到端的数据传输。该层是两台计算机经过网络进行数据通信时，第一个端到端的层次，具有缓冲作用。当网络层服务质量不能满足要求时，它将服务加以提高，以满足高层的要求；当网络层服务质量较好时，它只用很少的工作。传输层还可进行复用，即在一个网络连接上创建多个逻辑连接。

传输层在终端用户之间提供透明的数据传输，向上层提供可靠的数据传输服务。传输层在给定的链路上通过流量控、分段/重组和差错控制。一些协议是面向链接的。这就意味着传输层能保持对分段的跟踪，并且重传那些失败的分段。

**传输层作用**

- 传输层实现应用进程间的端到端(end-to-end)通信
- 向应用层提供通信服务

**多路分解与复用**

- 多路复用：所有应用进程的数据通过传输层传输到IP层；
- 多路分解：传输层收到的数据交付给相应的应用进程。

### **2.1[用户数据报协议UDP](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=用户数据报协议UDP&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiLnlKjmiLfmlbDmja7miqXljY_orq5VRFAiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.yKcs0Y0QeGZARHIdmCehaiCNbuV8411cjjhVdP8Us8M&zhida_source=entity)**

端到端的、尽力而为的、无连接的数据报传输服务 1.无连接的 2.尽最大努力交付，即不保证可靠交付 3.面向报文的（在IP的功能上简单扩展了端到端） 4.没有拥塞控制 5.支持一对一、一对多、多对一和多对多的交互通信（需要组播的通信都是建立在UDP之上）

![img](https://pic3.zhimg.com/v2-bd923acc1da3381d4ab02a14f00f8918_1440w.jpg)

### **2.2[传输控制协议TCP](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=传输控制协议TCP&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiLkvKDovpPmjqfliLbljY_orq5UQ1AiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.okK5okIXdBdXb46cbace4QiwyRm7W1EhmS3XH54xohY&zhida_source=entity)**

端到端的、可靠的、面向连接的字节流服务 a).面向连接：先建立逻辑连接，进行双向数据流传输，通信结束后撤销连接 b).面向字节流 c).点对点的全双工通信 d).可靠传输：对一个连接上传输的每个字节编号，通过接收确认和重传来保证可靠传输 c).流量控制：防止发送方发出的数据超出接收方的接收能力。

![img](https://pica.zhimg.com/v2-9537b4538943ff19529a7fca95b9c118_1440w.jpg)

多路复用：源、目的端口 连接管理：序号、确认号、SYN、FIN 可靠传输：序号、确认号 流量控制：接收窗口 拥塞控制：未在TCP首部中体现（序号、确认号、接收窗口）。

### **2.3连接管理**

- 每条TCP连接是一对点到点的字节流
- 每条TCP连接者两个端点，即套接字(sokect)={IP : port}
- 每条TCP连接由两个端点唯一标识，TCP连接={socket1, socket2} = {{IP1 : port1}, {IP2 : port2}}
- TCP连接有3个阶段：连接建立、数据传输、连接释放

**1）TCP连接建立的目的**

①使通信双方确知对方的存在  ②双方确定自己的初始序列号，并通知对方  ③允许双方协商一些参数（最大报文长度、窗口大小等）  ④对传输实体资源进行分配

**2）TCP连接建立的方式**

采用客户端服务器方式（C/S），主动发起连接建立的应用进程叫做客户端，被动等待连接建立的叫服务器端。

**3）连接建立（三次握手）**

①服务器进程B被动打开连接，进入LISTEN（收听）状态，等待客户端发出请求  ②客户进程A主动打开连接，向B发送连接请求报文段（报文段不挟带数据），SYN=1，序号=x，进入SYN-SENT（同步已发送）状态  ③服务器进程B收到请求后，向A发送确认报文段（报文段不挟带数据），SYN=1，ACK=1，确认号=x+1，序号=y，进入SYN-RCVD（同步收到）状态  ④客户进程A收到确认后，向B发送确认报文段（报文段可以携带数据，不携带数据时不消耗序号，下一个序号依然是x+1），ACK=1，确认号=y+1，序号=x+1，进入ESTABLISHED（已建立连接）状态，B收到确认后，也进入ESTABLISHED状态



![img](https://pic3.zhimg.com/v2-57dd8f8959dc43ebbd8f445cbb4caa96_1440w.jpg)

为什么A需要向B发送最后一个确认报文段：为了防止“已失效的连接请求报文段”突然又传到B发生错误，以至于B一直等待A发送数据，B的资源被浪费。

**4）连接释放（四次挥手）**

①A，B都处于ESTABLISHED状态；  ②客户进程A主动关闭连接，向B发送连接释放请求报文段（报文段不挟带数据），FIN=1，序列号=u，进入FIN-WAIT-1（终止等待1）状态；  ③B收到A的连接释放报文段后，应答确认，ACK=1，确认号=u+1，序号=v，进入CLOSE-WAIT (关闭等待)状态，B仍然可以向A发送数据，A进入FIN-WAIT-2（终止等待2）状态；  ④若B已经没有向A的数据，其应用进程通知TCP连接释放，B向A发送连接释放报文段，FIN=1，ACK=1，确认号=u+1，序号=w，进入LAST-ACK（最后确认）状态；  ⑤A收到B的链接释放报文段后，应答确认，ACK=1，确认号=w+1，序号=u+1，进入TIME-TIME-WAIT（时间等待）状态，B收到A的确认后，进入CLOSED状态

![img](https://pic4.zhimg.com/v2-5e6d57df0c6b47c7b68e0d8c827b131b_1440w.jpg)

A必须经过时间等待计时器设置的时间2MSL（默认2分钟）后，进入CLOSED状态：确保A发送的最后一个ACK报文段能够到达B；防止“已失效的连接请求报文段”出现在本连接中。

> **【文章福利**】小编推荐自己的Linux C++技术交流群:【[1106675687](https://link.zhihu.com/?target=https%3A//jq.qq.com/%3F_wv%3D1027%26k%3D4JdTdtmV)】整理了一些个人觉得比较好的学习书籍、视频资料共享在群文件里面，有需要的可以自行添加哦！！！前100名进群领取，额外赠送大厂面试题。

![img](https://pic2.zhimg.com/v2-f31c96917880f8f8bf9424836a142001_1440w.jpg)

**资料领取直通车：**[大厂面试题锦集+视频教程](https://link.zhihu.com/?target=https%3A//docs.qq.com/doc/DTlhVekRrZUdDUEpy)

**Linux服务器学习网站：**[C/C++Linux服务器开发/后台架构师](https://link.zhihu.com/?target=https%3A//ke.qq.com/course/417774%3FflowToken%3D1028592)

**可靠传输**

- 发送方的TCP：维护一个发送缓冲区
- 维护3个指针：LastByteAcked、LastByteSent、LastByteWritten
- 发送窗口=min（通知窗口，拥塞窗口）
- 累积确认(Cumulative ACK) ：对按序到达的最后一个报文段进行确认
- 选择确认(Selective ACK) ：确认接收到的不连续的数据块的边界（使用首部的SACK选项，不影响确认号字段的使用）

![img](https://pic1.zhimg.com/v2-306135b1cd135816f7e11f95e3f053f4_1440w.jpg)

**流量控制**

目的：为了防止发送方给慢接收方发数据造成接受崩溃，缓冲区溢出 原理：接收方通知发送方自己的接受窗口大小，发送方的发送窗口≤接收方的接受窗口
问题：B向A发送了零窗口报文段后，B的接受缓存有了一些存储空间，于是B向A发送了rwnd=400的报文段，然而报文段在传送过程中丢失，这样A一直等待B发送的非零窗口通知，B一直等待A发送数据，从而形成死锁局面。

解决：TCP为每一个连接设置一个持续计时器，只要TCP链接一方收到零窗口通知，就启动持续计时器，计时器到期，发送零窗口探测报文段，而对方就在确认这个探测报文段时给出现在的窗口值，①如果窗口仍然是零，那么重新设置持续计时器；②否则死锁的僵局就可以打破。

### 2.4**自适应重传**

**1）超时重传**

报文段的往返时间：RTT 加权平均往返时间：RTTS RTT的偏差加权平均值：RTTD 超时重传时间：RTO RTTS = (1 - α) * RTTS + α * 新的RTT样本值 （α一般为1/8） RTTD = (1 - β) * RTTD + β * | RTTS - 新的RTT样本值 | （β一般为1/4） RTO = RTTS + 4 * RTTD

Karn算法： ①每次超时重传一个报文段时，停止计算新RTT样本值 ②每次超时重传一个报文段时，就把超时重传时间RTO增大y倍（一般为2倍） ③当不发生报文段重传时，才计算RTTS和超时重传时间RTO

**2）快速重传**

原因：超时重传作为TCP最基本的重传机制，效率较低。

![img](https://pica.zhimg.com/v2-ea4ee41d7f9d2cf3cdc6034d8d8cef08_1440w.jpg)

超时的粗粒度实现方法导致连接在等待一个定时器超时时，很长一段时间连接无效。

基本思想： ①接收方：当报文段到达，立刻回复ACK，即使该序号已被确认过 ②发送方：收到一个重复ACK（同一个确认的再一次重传称为重复确认），就知道接收方必定收到乱序到达的报文段，表明其前面的分组可能丢失。收到3个重复ACK时，立刻触发重传。

![img](https://picx.zhimg.com/v2-00bb7f8d0a4695c121f09247a21a22b7_1440w.jpg)

### 2.5拥塞控制

**1）窗口大小**

MaxWindow = min (cwnd, AdvertisedWindow) 拥塞窗口cwnd (Congestion Windows)：拥塞控制算法决定，可以同时发出的最大字节数以防止造成网络拥塞 通知窗口 (AdvertisedWindow)：接收方决定，可以同时发出的最大字节数以防止超出接收方的接收能力。

**2）拥塞控制算法**

①慢启动 把初始拥塞窗口 cwnd 设置为不超过2至4个SMSS（最大报文段长度），在每收到一个对新的报文段的确认后，把cwnd增加1个 SMSS 的数值数（每经过一个传输轮次，cwnd就加倍） 设置一个慢开始门限ssthresh 当cwnd < ssthresh时，使用慢开始算法 当cwnd ≥ ssthresh时，使用拥塞避免算法。

![img](https://pic1.zhimg.com/v2-c2d8e86ba90479375ada97d06c5c9fc0_1440w.jpg)

②拥塞避免（加法增大）：每经过一个往返时间RTT把发送方的cwnd加1，使得cwnd慢性增加 ③快重传（乘法减小）：收到3个重复ACK立即触发重传 ④快恢复（在快重传之后）

1. ssthresh减小为当前cwnd的一半：ssthresh = cwnd / 2
2. 新拥塞窗口 cwnd = 新的 ssthresh
3. 执行拥塞避免 (AIMD)，使cwnd缓慢线性增大

## 三、**应用层**

### **3.1概念**

5层因特网协议栈：应用层 -> 传输层 -> 网络层 -> 数据链路层 -> 物理层

7层OSI参考模型：应用层 ->表示层 -> 会话层 -> 传输层 -> 网络层 -> 数据链路层 -> 物理层

**为什么我们要在计算机网络中分层次？**

因为如果两台计算机能够相互通信的话，实际实现起来是非常困难操作的。我们分层的目的就是**为了将困难的问题简单化**，并且如果我们分层了，我们在使用的时候就可以**仅仅关注我们需要关注的层次，而不用理会其他层**

如果需要改动设计的时候，我们只需要把变动的层替换即可，并不用涉及到其他的层次。这与我们程序设计中的低耦合是一个概念。

**每层作用如下：**

- **物理层：**通过媒介传输比特,确定机械及电气规范（比特Bit），通过电频的高低来传输0和1这样的电信号
- **数据链路层**：将比特组装成帧和点到点的传递（帧Frame），将无规则的0和一同通过一套规则来分组传输，于是出现了以太网协议。**以太网协议**规定：**一组电信号构成一个数据包，这个数据包称为帧**，每一个帧由标头（Head）和数据（Data）两部分组成，**标头部分的大小为18字节**
- **网络层**：负责数据包从源到宿的传递和网际互连（包PackeT）
- **传输层**：提供端到端的可靠报文传递和错误恢复（段Segment）
- **会话层**：建立、管理和终止会话（会话协议数据单元SPDU）
- **表示层**：对数据进行翻译、加密和压缩（表示协议数据单元PPDU）
- **应用层**：允许访问OSI环境的手段（应用协议数据单元APDU）

![img](https://picx.zhimg.com/v2-01aef48b94c1886c102618d552ca059b_1440w.jpg)

**每一层的协议如下：**

- 物理层：RJ45、CLOCK、IEEE802.3 （中继器，集线器，网关）
- 数据链路：PPP、FR、HDLC、VLAN、MAC （网桥，交换机）
- 网络层：IP、ICMP、ARP、RARP、OSPF、IPX、RIP、IGRP、 （路由器）
- 传输层：TCP、UDP、SPX
- 会话层：NFS、SQL、NETBIOS、RPC
- 表示层：JPEG、MPEG、ASII
- 应用层：FTP、DNS、Telnet、SMTP、HTTP、WWW、NFS

### 3.2**[DNS域名系统](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=DNS域名系统&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJETlPln5_lkI3ns7vnu58iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.uGmyXXVWuvubAN4ERF6MVzYVc_qUsweu1OEyQHoJ5jQ&zhida_source=entity)**

**NS（Domain Name System）提供了什么服务？**

- 一个由分层的DNS服务器（DNS Server）实现的分布式数据库
- 使得主机能够查询分布式数据库的应用协议
- **DNS协议是运行在UDP之上，用53号端口**

**分布式、层次数据库：**

- 根DNS服务器：全球有四百多个根DNS服务器，由13个不同的组织管理，根服务器提供TLD服务器的IP地址。
- 顶级域(TLD)DNS服务器：顶级域（如：com、org、net和org）和所有的国家顶级域（uk、fr、cn、jp）都有TLD服务器。TLD服务器提供了权威DNS服务器的IP地址
- 权威DNS服务器：**再因特网上具有公共可访问的主机**（就是你的服务器主机地址），权威DNS服务器将主机名映射为IP地址
- 本地DNS服务器：不属于DNS服务器的层次结构，但是也是必不可少的。**当我们发出DNS请求时，该请求先被发送到本地DNS服务器，它起着代理的作用**

**域名解析过程：**

假设我们（主机是[http://a.xyz.com](https://link.zhihu.com/?target=http%3A//a.xyz.com)）获取[http://www.baidu.com](https://link.zhihu.com/?target=http%3A//www.baidu.com)的IP地址，同时本地DNS服务器为[http://dns.xyz.com](https://link.zhihu.com/?target=http%3A//dns.xyz.com)。首先我们的主机向本地DNS服务器发送一个DNS查询报文（报文包含被查询的主机名），然后本地DNS服务器将该报文转发到根DNS服务器，根DNS服务器发现了com前缀，然后向本地DNS服务器返回com的顶级域（TLD）DNS服务器的IP地址列表。本地DNS服务区再向这些TLD服务器发送查询报文，TLD服务器注意到了[http://baidu.com](https://link.zhihu.com/?target=http%3A//baidu.com)前缀，然后将负责[http://www.baidu.com](https://link.zhihu.com/?target=http%3A//www.baidu.com)的权威DNS服务器的IP地址返回给本地DNS服务器，最后，本地DNS服务器向该IP地址进行发送报文查询，权威DNS服务器返回了[http://www.baidu.com](https://link.zhihu.com/?target=http%3A//www.baidu.com)的IP地址给本地DNS服务器，本地DNS服务器再将该DNS发送到我们主机，我们开始访问该IP地址。

**递归查询过程如下：**

![img](https://pic1.zhimg.com/v2-cbbcebc96f33bb57e7621acd80ed3626_1440w.jpg)

**迭代查询过程如下：**

![img](https://pic2.zhimg.com/v2-d130bf07b4b1e9d88fba21dc7a173f3f_1440w.jpg)

理论上来讲，任何DNS查询既可以是递归也可以是迭代的。**在实践中，查询通常是从请求主机到本地DNS服务器的查询时递归的，其余查询时迭代的**

- **主机向本地域名服务器的查询一般都是采用递归查询**：如果主机所询问的本地域名服务器不知道被查询域名的 IP 地址，那么本地域名服务器就以 DNS 客户的身份，向其他根域名服务器继续发出查询请求报文。
- **本地域名服务器向根域名服务器的查询通常是采用迭代查询**：当根域名服务器收到本地域名服务器的迭代查询请求报文时，要么给出所要查询的 IP 地址，要么告诉本地域名服务器：“你下一步应当向哪一个域名服务器进行查询”。然后让本地域名服务器进行后续的查询。

### 3.3**[FTP协议](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=FTP协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJGVFDljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzExNTE2MDksImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.q3WryoFaPL8BfZakhB_rJM9YQBnziwJzU8QQaYlzYZA&zhida_source=entity)以及端口**

文件传输协议FTP（File Transfer Protocol）是世界上使用最广泛的文件传输协议。FTP 提供交互式的访问，**允许客户指明文件的类型与格式，并允许文件具有存取权限**

**网络环境下复制文件的复杂性：**

- 计算机存储数据的格式的不同
- 文件的目录结构和文件命名的规定不同
- 对于相同的文件存取功能，操作系统使用的命令不同
- 访问控制方法不同

因此，FTP协议出现了。

**FTP协议连接过程**：

1. 打开熟知端口（端口号为 21），使客户进程能够连接上
2. 等待客户进程发出连接请求
3. 启动从属进程来处理客户进程发来的请求。从属进程对客户进程的请求处理完毕后即终止，但从属进程在运行期间根据需要还可能创建其他一些子进程
4. 回到等待状态，继续接受其他客户进程发来的请求。主进程与从属进程的处理是并发地进行
5. 当客户进程向服务器进程发出建立连接请求时，要寻找连接服务器进程的熟知端口 (21)，同时还要告诉服务器进程自己的另一个端口号码，用于建立数据传送连接
6. 接着，**服务器进程用自己传送数据的熟知端口 (20) 与客户进程所提供的端口号码建立数据传送连接**
7. 由于 FTP 使用了两个不同的端口号，**所以数据连接（20）与控制连接（21）不会发生混乱**

**FTP是使用了两个TCP连接的**：

- 使协议更加简单和跟容易实现
- 在传输文件时还可以利用控制连接（如：客户发送请求终止传输）

![img](https://picx.zhimg.com/v2-8ec7acf7cf9a8493a5a424b2c0741d4d_1440w.jpg)

### 3.4**[DHCP动态主机配置协议](https://zhida.zhihu.com/search?content_id=231151609&content_type=Article&match_order=1&q=DHCP动态主机配置协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk1MzksInEiOiJESENQ5Yqo5oCB5Li75py66YWN572u5Y2P6K6uIiwiemhpZGFfc291cmNlIjoiZW50aXR5IiwiY29udGVudF9pZCI6MjMxMTUxNjA5LCJjb250ZW50X3R5cGUiOiJBcnRpY2xlIiwibWF0Y2hfb3JkZXIiOjEsInpkX3Rva2VuIjpudWxsfQ.pKF1Tlc0J1cMAU_QjgQql_ZoBMDoc_0SEigPZLPPQfo&zhida_source=entity)**

DHCP：DHCP（动态主机配置协议）是一个局域网的网络协议。指的是由服务器控制一段IP地址范围，客户机登录服务器时就可以自动获得服务器分配的IP地址和子网掩码。互联网广泛使用的**动态主机配置协议 DHCP (Dynamic Host Configuration Protocol) 提供了即插即用连网 (plug-and-play networking) 的机制。**

DHCP是使用UDP协议工作，**他的用途如下：**

1. 1. 为内部网络或网络服务供应商**自动分配IP地址**
   2. 为用户或者内部网络管理员作为对所有计算机做中央管理的手段
   3. 为内部网络用户接受IP租约

并不是每个网络上都有DHCP服务器，这样会使得DHCP服务器的数量太多了。现在是**每个网络至少有一个DHCP中继代理，他配置了DHCP服务器的IP地址信息**

在生活中，DHCP必不可少，为我们带来了便利，当通过WiFi连上一个陌生的子网，但是我们并没有做重新为主机配置IP地址的工作，这样子就可直接上网！假如没有DHCP协议的帮助：

![img](https://pic4.zhimg.com/v2-4f61b6f0482bdd087ae5259ccdf8986b_1440w.jpg)

**而在DHCP协议的支持下：**

![img](https://pic3.zhimg.com/v2-9b7ffa60523ea38ba14d4e00133a393a_1440w.jpg)

**DHCP工作流程**：

- DHCP服务器管理着一个包含一系列IP地址的地址池
- 每当一台新的主机加入时，DHCP服务器就从其当前可用地址池中分配一个任意的地址给它
- 而每当一台主机离开的时候，其IP地址就被回收到地址池中

### 3.5**简单网络管理协议SNMP**

简单网络管理协议SNMP（Simple Net Manage Protocol）是**TCP/IP协议簇**的一个**应用层协议，网络管理包括对硬件、软件和人力的使用、综合与协调，**以便对网络资源进行监视、测试、配置、分析、评价和控制，这样就能以合理的价格满足网络的一些需求，如实时运行性能，服务质量等。网络管理常简称为网管。

![img](https://pic2.zhimg.com/v2-7bb7a51798c99376ff187630de895383_1440w.jpg)

**含义：**

- 网络管理协议简称为网管协议。
- 需要注意的是，并不是网管协议本身来管理网络。**网管协议是管理程序和代理程序之间进行通信的规则。**

**CS模式：**

- 管理程序和代理程序**按客户服务器方式工作。**
- 管理程序**运行 SNMP 客户程序，向某个代理程序发出请求（或命令），代理程序运行 SNMP 服务器程序，返回响应（或执行某个动作）。**

**功能：**

- SNMP 最重要的指导思想就是要**尽可能简单**。
- SNMP 的**基本功能包括监视网络性能、检测分析网络差错和配置网络设备等。**

**过程：**

- 整个系统必须有一个管理站。
- 管理进程和代理进程利用 SNMP 报文进行通信，而 SNMP 报文又使用 UDP 来传送。
- 若网络元素使用的不是 SNMP 而是另一种网络管理协议，SNMP 协议就无法控制该网络元素。这时可使用委托代理 (proxy agent)。委托代理能提供如协议转换和过滤操作等功能对被管对象进行管理。
- SNMP 定义了管理站和代理之间所交换的分组格式。所交换的分组包含各代理中的对象（变量）名及其状态（值）。
- **SNMP 负责读取和改变这些数值。**

### **3.6电子邮件协议SMTP、POP3、IMAP**

SMTP是因特网电子邮件中主要的应用层协议，只用TCP可靠数据传输服务，SMTP的端口号是25号。因特网电子邮件的主要组成部分：

- 用户代理（user agent）
- 邮件服务器（mail server）
- 简单邮件传输协议（Simple Mail Transfer Protocol ）

![img](https://picx.zhimg.com/v2-f4422b82605ae102849bf90d8930b42f_1440w.jpg)

**邮件发送过程：**从发送的用户代理开始，传输邮件到发送方的邮件服务器，再传输到接收方的邮件服务器，然后在这里被分发到接收方的邮箱中。如果用户要读取该邮件时，使用用户名和口令来鉴别用户。

POP3（第三版的邮局协议Post Office Protocol - Version 3）：

- POP3是一个极为简单的邮件访问协议（由于其简单，所以权限有限）
- 当用户打开了一个到邮件服务器的端口110上的TCP连接后，POP3就开始工作了，按照三个阶段：特许、事务处理以及更新

IMAP（因特网邮件访问协议Internet Mail Access Protocol）：

- 允许用户代理获取报文某些部分的命令
- IMAP协议为用户提供了创建文件夹及移动文件的命令

***注意：***

- 不要将邮件读取协议 POP 或 IMAP 与邮件传送协议 SMTP 弄混。
- 发信人的用户代理向源邮件服务器发送邮件，以及源邮件服务器向目的邮件服务器发送邮件，都是使用 SMTP 协议。
- 而 POP3 协议或 IMAP 协议则是用户从目的邮件服务器上读取邮件所使用的协议。

## 四、网络层协议

IP协议属于网络层协议，所有的TCP, UDP, ICMP, IGMP数据都通过IP数据报传输。IP提供了一种不可靠，无连接的数据包交付服务。依赖其他层的协议进行差错控制。 不可靠: IP数据报不保证能成功的到达目的地，如果出现错误则选择丢弃该数据，然后发送ICMP消息报给信源端 无连接: IP不提供任何后续数据报的状态信息，每个数据报处理都是独立的。如果一个信源发送了连续的两个数据报，每个数据报选择独立的路由，两个数据可能不同时到达。IP通信双方都不长久地维持对方的任何信息。这样上层协议每次发送数据的时候，都必须明确指定对方的IP地址。

### 4.1**ipv4数据报**

![img](https://pic1.zhimg.com/v2-7b2c7c4c42e858205cb70bb88421f248_1440w.jpg)

1.版本号：占四位，就是IP协议的版本，通信双方的IP协议必须要达到一致，IPv4的版本就是4.

2.首部长度：占四位，因为长度为四比特，所以首部长度的最大值为1111，15，又因为首部长度代表的单位长度为32个字（也就是4个字节），所以首部长度的最小值就是0101，当然，也确实如此，大部分的ip头部中首部字节都是0101.也就是5*4=20个字节，如果是最大值15的话，ip首部的最大值就是60个字节，所以记好了，ipv4首部长度的最大值就是60，当然当中我们又能发现，IPv4的首段长度一定是4字节的整数倍，要是不是怎么办呢？别急，后面的填充字段会自动填充补齐到4字节的整数倍的。

3.区分服务：这个没有什么用处，也没有什么好讲的了，只要自动这玩意占八位，一个字节就可以了。

4.总长度：占16位，这个的意思就是ip数据报中首部和数据的总和的长度，因为占16位，所以很好理解，总长度的最大值就是2的16次方减一，65535，这玩意也对应着还有一个很简单的概念，最大传输单元mtu，意味着一个IP数据报的最大长度就只能装下65535个字节，要是传输的长度超过这个怎么办，很简单，分片。

5.标识：占16位，标识这玩意很好理解，IP在存储器中维持一个计数器，每产生一个 数据报，计数器就加1,并将此值赋给标识字段。但这个标识并不是平常的序号，因为IP是 无连接服务，数据报不存在按序接收的问题。当数据报由于长度超过网络的MTU而必须分 片时，这个标识字段的值就被复制到所有的数据报片的标识字段中，等到重组的时候，相同标识符的值的数据报就会被重新组装成一个数据报。

6.标志：占三位，一般有用的是前两位， 最低位叫做MF，MF=1表示后面还有若干个数据报，MF=0表示这已经是最后一个数据报了。 中间位叫做DF，DF表示不能进行分片，DF=0才可以进行分片操作。

7.片偏移：占13位，片偏移就是，在原来的数据报分片以后，该片在原分组中的相对位置，片偏移中的基本单位是8字节，所以，也就是说，只要是分片，每个分片的长度都是8字节的整数倍，最后一个分片不够八字节的一样是填充。

8.生存时间ttl：占8位，（time to live），表明数据报在网络中的寿命，这个值被设定成跳数，顾名思义，就是这个数据报可以经过多少个路由器的数量，每经过一个路由器，该值就减一，减到为零的时候就被抛弃，显而易见，这个跳数的最大值就是2的8次方减一，255.

9.协议：就是用来指明数据报携带了哪种协议，占8位。

10.首部效验和：占16位，这个字段用来效验数据报首段，下面给出简单的计算方法：

首先在发送端的时候，将效验和全部置为0，然后把数据报首段数据全部进行反码相加，得到的值为效验和，放入首段效验和里面，然后接收端将数据报首段数据和效验和一起全部反码相加，最后若是得到零，则保留，若是不为零，则说明数据报在传输的过程中发生了改变，则丢弃该数据报。

11.IP源地址：占32位，将IP地址看作是32位数值则需要将网络字节顺序转化位主机字节顺序。转化的方法是：将每4个字节首尾互换，将2、3字节互换。

12.目的地址：也占32位，转换方法和来源IP地址一样。

13.选项：可变长的可选信息，最多包含40字节。选项字段很少被使用。可用的IP可选项有： a. 记录路由: 记录数据包途径的所有路由的IP，这样可以追踪数据包的传递路径 b. 时间戳: 记录每个路由器数据报被转发的时间或者时间与IP地址对，这样就可以测量途径路由之间数据报的传输的时间 c. 松散路由选择: 指定路由器的IP地址列表数据发送过程中必须经过所有的路由器 d. 严格路由选择: 数据包只能经过被指定的IP地址列表的路由器 e. 上层协议(如TCP/UDP)的头部信息

13.到了可变部分IPv4的头部基本上就已经讲完了，增加头部的可变选项实际上就是增加了数据报的功能，可变选项在实际上是很少用到的。

### 4.2**分片**

当IP数据报的长度超过帧的MTU时，它将被分片传输。分片可能发生在发送端，也可能发生在中转路由器上，而且可能在传输过程中多次分片，但只有在最终的目标机器上，这些分片才会被内核中的IP模块重新组装。 IP头部中的如下三个字段给IP的分片和重组提供了足够的信息：数据报标识、标志和片偏移。一个IP数据报的每个分片都具有自己的IP头部，它们具有相同的标识值，但具有不同的片偏移。并且除了最后一个分片外，其他分片都将设置MF标志。此外，每个分片的IP头部的总长度字段将被设置位该分片的长度。

### **4.3IP路由**

路由是什么： 我们知道，IP地址是网络世界里的门牌号。你可以通过IP地址访问远在天边的网站,那么数据是如何到达网站的呢？靠的就是路径上每个节点的路由。 路由，简单的说就是指导IP报文该去哪的指示牌。

一般说来，主机会在以下两个时机进行路由查询

- 1.收到报文时，查询路由决定是上送本机(LOCAL IN)，或者从哪个出接口转发(FORWARD)
- 2.本机发送报文时，查询报文出接口 注意，转发需要开启 net/ipv4/ip_forward

路由表长什么样 以一个典型的主机为例，tristan有一个外部网卡enp1s0和一个内部还回网卡lo。

![img](https://pic3.zhimg.com/v2-7b6d000c8f1455b64b528e0d0d449a5a_1440w.jpg)

通过route -n我们可以看到主机上简要的路由表信息(当然通过ip route也可以)，那么上面的路由信息中的每一表项代表什么意思呢？

- 如果报文的目的IP地址在164.69.136.0/24这个网段，那么它应该从enp1s0进行转发。
- 如果报文的目的IP地址在169.254.0.0/8这个网段，那么它应该从enp1s0进行转发。
- 其他情况下(0.0.0.0/0)，报文从enp1s0转发，下一跳IP地址是192.168.99.254

### 4.4**IP转发**

当主机收到一个数据报时，首先检查目的地址：

- 如果是自己（自己某一个接口所配置的IP地址或IP广播或者组播地址），则交给协议字段或者IPv6头部的下一个头部字段指定的协议模块处理。

- 如果不是：

- - 如果IP层被配置为路由器，则转发该数据报。
  - 否则默默丢弃，必要时生成ICMP报文给发送者。

转发不会改变数据报的IP地址，只是通过设置链路层地址来完成交付的过程：

- 发送者定义好源IP和目的IP，如果目的IP不在本地，则将链路层的目的MAC地址设置为路由器，由路由器代为转发。
- 每一跳路由器在转发时，都会将源MAC地址设置为自己，目的MAC地址设置为下一跳路由器。

### 4.5**IP地址介绍**

ip地址组成 : IP地址由4部分数字组成，每部分数字对应于8位二进制数字，各部分之间用小数点分开 这是点分2进制 如果换算为10进制我们称为点分10进制.每个ip地址由两部分组成网络地址(NetID)和主机地址(HostID).网络地址表示其属于互联网中的哪一个网络，而主机地址则表示其属于该网络中的哪一台主机。

![img](https://pic1.zhimg.com/v2-a1c940e5dcd828b128e85a9c5122ef68_1440w.jpg)

A类地址:范围从0-127，0是保留的并且表示所有IP地址，而127也是保留的地址，并且是用于测试环回用的。因此 A类地址的范围其实是从1-126之间。 　　如：10.0.0.1，第一段号码为网络号码，剩下的三段号码为本地计算机的号码。转换为2进制来说，一个A类IP地址由1字节的网络地址和3字节主机地址组成，网络地址的最高位必须是“0”， 地址范围从1.0.0.1 到126.0.0.0。可用的A类网络有126个，每个网络能容纳1千多万个主机（2的24次方的-2主机数目）。 以子网掩码来进行区别：：255.0.0.0 127.0.0.0到127.255.255.255是保留地址，用做循环测试用的

B类地址：范围从128-191，如172.168.1.1，第一和第二段号码为网络号码，剩下的2段号码为本地计算机的号码。转换为2进制来说，一个B类IP地址由2个字节的网络地址和2个字节的主机地址组成，网络地址的最高位必须是“10”，地址范围从128.0.0.0到191.255.255.255。可用的B类网络有16382个，每个网络能容纳6万多个主机 。(2的16次方-2) 以子网掩码来进行区别：255.255.0.0 169.254.0.0到169.254.255.255是保留地址。如果你的IP地址是自动获取IP地址，而你在网络上又没有找到可用的DHCP[服务器](https://link.zhihu.com/?target=https%3A//cloud.tencent.com/product/cvm%3Ffrom%3D20065%26from_column%3D20065)，这时你将会从169.254.0.0到169.254.255.255中临时获得一个IP地址。

C类地址：范围从192-223，如192.168.1.1，第一，第二，第三段号码为网络号码，剩下的最后一段号码为本地计算机的号码。转换为2进制来说，一个C类IP地址由3字节的网络地址和1字节的主机地址组成，网络地址的最高位必须是“110”。范围从192.0.0.0到223.255.255.255。C类网络可达209万余个，每个网络能容纳254个主机。(2的8次方-2) 以子网掩码来进行区别： 255.255.255.0

D类地址：范围从224-239，D类IP地址第一个字节以“1110”开始，它是一个专门保留的地址。它并不指向特定的网络，目前这一类地址被用在多点广播（Multicast）中。多点广播地址用来一次寻址一组计算机，它标识共享同一协议的一组计算机。 224.0.0.0-239.255.255.255 组播地址

E类地址：范围从240-254，以“11110”开始，为将来使用保留。 全零（“0．0．0．0”）地址对应于当前主机。全“1”的IP地址（“255．255．255．255”）是当前子网的广播地址。 240.0.0.0-255.255.255.254 保留地址

子网掩码就是为了区分ip地址的中的网络号和主机号的。

私有地址 所谓的私有地址就是在互联网上不使用，而被用在局域网络中的地址 在A类地址中，10.0.0.0到10.255.255.255是私有地址 在B类地址中，172.16.0.0到172.31.255.255是私有地址。 在C类地址中，192.168.0.0到192.168.255.255是私有地址。

## 五、手写TCP/IP用户态协议栈（纯C语言）

### **5.1DPDK基础知识**

- 1、dpdk环境搭建与多队列网卡
- 2、dpdk网卡绑定与arp
- 3、dpdk发送过程的实现
- 4、dpdk发送过程调试
- 5、dpdk-arp实现
- 6、arp 调试流程
- 7、dpdk-icmp实现
- 8、dpdk-icmp流程 调试与checksum实现
- 9、arp-table的实现

### **5.2协议栈之udp/tcp的实现**

- 1、arp request实现
- 2、arp调试流程
- 3、协议栈架构设计优化
- 4、udp实现之udp系统api的设计
- 5、udp实现之sbuf与rbuf的环形队列
- 6、udp实现之发送流程与并发解耦
- 7、udp实现之架构设计与调试
- 8、tcp 三次握手实现之dpdk tcp流程架构设计
- 9、tcp三次握手实现之dpdk tcp11个状态实现
- 10、tcp三次握手实现之dpdk代码调试

### **5.3协议栈之tcp的实现**

- 1、tcp数据传输之ack与seqnum的确认代码实现以及滑动窗口
- 2、tcp数据传输之ack与seqnum代码实现以及滑动窗口
- 3、tcp协议api实 现之bind, listen的实现
- 4、tcp协议api实现之accept的实现
- 5、tcp协议api实现之send, recv的实现
- 6、tcp协议api实 现之close的实现
- 7、tcp协议栈调试之段错误与逻辑流程
- 8、tcp协议栈调试之ringbuffer内存错误.
- 9、dpdk kni的原理与kni启动
- 10、重构网络协议分发的流程

### **5.4协议栈的组件功能**

- 1、kni抓包调试tcpdump
- 2、dpdk kni mempool错误与内存泄漏
- 3、基于熵的ddos检测的数学理论
- 4、dpdk ddos熵计算代码实现
- 5、dpdkddosattach检测准确度调试
- 6、ddos attack测试工具hping3
- 7、dpdk布谷鸟hash原理与使用

### **5.5协议栈之tcp并发实现**

- 1、tcp并发连接的设计
- 2、tcp并发epoll的实现
- 3、tcp并 发协议栈与epoll的回调与并发测试
- 4、bpf与bpftrace系统，网络挂载实现
- 5、bpf与 bpftrace应用程序ntyco的挂载监控

### **5.6DPDK网络基础组件**

- 1、mempool与mbuf的源码分析讲解
- 2、dpdk-ringbuffer源码分 析
- 3、dpdk-igb_ uio源码分 析
- 4、dpdk-kni源码分析
- 5、rcu的实现与互斥锁，自旋锁，读写锁

**解决问题**

- 1、苦读网络书籍没有实际项目运用
- 2、简历没有合适的网络项目可写
- 3、有C基础，纯兴趣爱好

## 参考文档2



## Qt的TCP编程模型

Qt的TCP通信基于底层的Socket API，具有**面向连接、可靠、基于字节流**的特点。它使用 **[⁠QTcpServer](https://doc.qt.io/qt-6/qtcpserver.html)** 类监听连接，使用 **[⁠QTcpSocket](https://doc.qt.io/qt-6/qtcpsocket.html)** 类进行数据收发，核心是通过**信号与槽机制**实现非阻塞的异步网络通信。 [[1](https://doc.qt.io/qt-6/zh/qtnetwork-programming.html), [2](https://blog.csdn.net/weixin_62621696/article/details/140668150), [3](https://www.cnblogs.com/qiutian-hao/articles/20164168)]

------

一、 TCP 核心原理

1. **面向连接**：通信前必须像打电话一样建立连接（通过“三次握手”），保证双方在线。
2. **可靠传输**：发送的数据会按顺序到达，如果有丢失或损坏，协议会自动要求重发。
3. **基于字节流**：数据像流水一样传输，没有明显的“边界”。因此，开发者必须自己处理**粘包**和**拆包**（即区分每次发送的完整数据包）。 [[1](https://jaminzhang.github.io/network/The-Difference-Between-TCP-And-UDP-Protocol/), [2](https://www.51cto.com/article/792452.html), [3](https://my.oschina.net/emacs_7994310/blog/19211889), [4](https://blog.csdn.net/weixin_62621696/article/details/140668150)]

二、 Qt TCP 编程模型

Qt 采用**事件循环（Event Loop）**和**信号与槽（Signals and Slots）**模型。这意味着所有网络操作（如连接、接收）都不会卡住界面，而是通过触发事件来处理。 [[1](https://www.cnblogs.com/qiutian-hao/articles/20164168)]

1. 服务器端模型（**QTcpServer**）

- **监听端口**：调用 `listen()` 开始等待客户端连接。
- **处理连接**：当有新连接时，触发 `newConnection()` 信号。
- **获取套接字**：在槽函数中调用 `nextPendingConnection()` 获取用于通信的 QTcpSocket。 [[1](https://zhuanlan.zhihu.com/p/681127921)]
- 客户端模型（**QTcpSocket**）

- **发起连接**：调用 `connectToHost()` 向服务器发起连接。
- **连接状态**：连接成功会触发 `connected()` 信号。 [[1](https://www.cnblogs.com/8765h/p/17320948.html)]
- 数据通信模型（两者通用）

- **发送数据**：调用 `write()` 函数发送字节数组。
- **接收数据**：当有新数据到来时，触发 `readyRead()` 信号。在槽函数中调用 `readAll()` 或 `read()` 读取数据。 [[1](https://www.cnblogs.com/qiutian-hao/articles/20164168)]

三、 经典编程步骤示例

步骤 1：服务端监听 (Server)

cpp

```
// 创建服务器对象
QTcpServer *server = new QTcpServer(this);

// 监听本地所有IP的 8888 端口
server->listen(QHostAddress::Any, 8888);

// 关联新连接信号
connect(server, &QTcpServer::newConnection, this, [server]() {
    // 获取客户端套接字
    QTcpSocket *socket = server->nextPendingConnection();
    
    // 连接成功提示
    qDebug() << "客户端已连接：" << socket->peerAddress().toString();

    // 关联读取数据的信号
    connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray data = socket->readAll();
        qDebug() << "收到数据：" << data;
    });
});
```

 

步骤 2：客户端连接与通信 (Client)

cpp

```
// 创建套接字对象
QTcpSocket *socket = new QTcpSocket(this);

// 连接服务器
socket->connectToHost("127.0.0.1", 8888);

// 关联连接成功信号
connect(socket, &QTcpSocket::connected, [socket]() {
    qDebug() << "连接服务器成功！";
    // 发送数据
    socket->write("Hello Server");
});

// 关联读取数据信号
connect(socket, &QTcpSocket::readyRead, [socket]() {
    QByteArray data = socket->readAll();
    qDebug() << "收到服务器消息：" << data;
});
```

 

四、 关键技术注意事项（进阶）

1. **防粘包机制**：
   因为 TCP 是字节流，多次发送的短数据可能会被合并成一次接收，或者一次长数据被分多次接收。
   - **解决方法**：在发送的数据前加上一个**包头**（记录数据长度的数值）。接收时先读取包头，等凑齐了该长度的字节数，再从缓冲区提取完整数据。
2. **数据的序列化**：
   建议使用 **[⁠QDataStream](https://doc.qt.io/qt-6/qdatastream.html)** 类把复杂的 C++ 数据结构（如结构体、QString、图像）转换成二进制字节流进行收发，简单且跨平台

## Qt的TCP编程模型参考文档1

## 一、背景

QTcpServer是Qt网络模块中的一个网络通信类，用于创建TCP服务器，允许应用程序监听并处理传入的TCP连接请求。QTcpServer的作用：

1. QTcpServer提供了一个简单而强大的方式来实现服务器端的网络通信，轻松地创建TCP服务器应用程序。
2. QTcpServer能够处理多个客户端同时连接，通过多线程或事件循环等机制实现并发处理，提高服务器端的性能和效率。
3. QTcpServer封装了[TCP协议](https://zhida.zhihu.com/search?content_id=239491811&content_type=Article&match_order=1&q=TCP协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzk4NDYsInEiOiJUQ1DljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMzk0OTE4MTEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.6GF8x2founXODuR9YBhi5ECvihHE8dXzMJ0O_DorEgA&zhida_source=entity)的复杂细节，提供了更高级别的接口，简化了网络编程的复杂性。
4. 通过QTcpServer可以构建稳定可靠的网络服务，如实时通讯、远程监控、数据传输等涉及网络通信的应用场景。

示例：使用QTcpServer实现一个基本的TCP服务器。

```cpp
// main.cpp
#include <QCoreApplication>
#include "mytcpserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    MyTcpServer server;
    server.startServer();

    return a.exec();
}

// mytcpserver.h
#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class MyTcpServer : public QTcpServer {
    Q_OBJECT

public:
    MyTcpServer(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *clientSocket;
};

#endif // MYTCPSERVER_H

// mytcpserver.cpp
#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent), clientSocket(nullptr) {
    connect(this, &MyTcpServer::newConnection, this, &MyTcpServer::onNewConnection);
}

void MyTcpServer::startServer() {
    if (!this->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server could not start!";
    } else {
        qDebug() << "Server started!";
    }
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor) {
    clientSocket = new QTcpSocket(this);
    if (!clientSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Error in setting socket descriptor";
        return;
    }

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::onDisconnected);

    qDebug() << "Client connected";
}

void MyTcpServer::onNewConnection() {
    qDebug() << "New connection available";
}

void MyTcpServer::onReadyRead() {
    QByteArray data = clientSocket->readAll();
    qDebug() << "Data received: " << data;
}

void MyTcpServer::onDisconnected() {
    qDebug() << "Client disconnected";
}
```

使用Qt的构建工具qmake和make（或者使用Qt Creator集成开发环境）编译。在项目文件夹中创建一个.pro文件（例如：mytcpserver.pro），内容如下：

```text
QT       += core network

CONFIG   += c++11

TARGET = mytcpserver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mytcpserver.cpp

HEADERS += mytcpserver.h
```

执行编译命令：

```text
qmake mytcpserver.pro
make
```

可以看到，使用QTcpServer很容易就实现了一个TCP服务器，而且它使用异步事件的方式处理TCP客户端的连接，那么它是如何实现的异步机制呢？想了解QT的socket是基于什么模型来实现的，博主同样非常的感兴趣，所以看了QT关于TcpServer实现的相关源码，现在将所了解的内容记录下来。

## 二、QTcpServer的基本原理

## 2.1、TCP协议简介

TCP（Transmission Control Protocol，传输控制协议）是因特网协议套件中的一部分，它位于传输层，提供可靠的、面向连接的数据传输服务。

TCP协议的特点：

1. 面向连接：在进行数据传输之前，TCP在通信双方之间建立连接，之后才会开始数据的传输。连接建立包括三步握手，以确保通信的正常进行。
2. 可靠性：TCP协议通过序号、确认和重传等机制来确保数据的可靠传输。如果一个数据包未能正确传输，TCP会进行重传以保证数据的完整性。
3. 流控制：TCP协议通过滑动窗口机制来进行流控制，确保发送方和接收方之间的数据传输速率相匹配，避免数据包的过载和丢失。
4. 拥塞控制：TCP通过拥塞窗口和拥塞避免等机制来控制网络拥塞，避免过多的数据流量导致的网络拥堵，从而保证网络的稳定性和可靠性。
5. 面向字节流：TCP是面向字节流的协议，它不会将数据分割成固定大小的数据包，而是按照应用程序传送的字节流来进行数据传输。

TCP协议提供了一种高可靠性的数据传输方式，适用于要求数据传输可靠性和顺序性的应用场景，如文件传输、电子邮件发送等。但是，与UDP相比，TCP在数据传输过程中有较高的开销。

## 2.2、QTcpServer的概念

QTcpServer是Qt框架中用于实现TCP服务器的类。它提供了一种简单而高效的方式，用于监听传入的TCP连接请求，从而可以与客户端建立连接并实现数据交换。

QTcpServer的主要作用：

1. QTcpServer可以通过调用listen()方法，在指定的IP地址和端口上开始监听传入的连接请求。
2. 监听到连接请求后通过nextPendingConnection()方法接受这些请求，获得一个QTcpSocket对象，用于与客户端进行数据交换。
3. 通过重写incomingConnection()方法，在建立新连接时执行自定义的处理操作。
4. QTcpServer可以管理多个TCP连接，并在需要的时候进行数据交换或断开连接。
5. 通过信号和槽机制，QTcpServer可以处理连接建立、断开、数据到达等事件，实现灵活的连接管理和数据处理。

QTcpSocket同样是Qt框架中用于实现TCP网络通信的重要类。QTcpServer与QTcpSocket的关系：

1. QTcpServer是用于实现TCP服务器的类，它负责监听传入的TCP连接请求，并与客户端建立连接；QTcpSocket则是用于实现TCP客户端的类，它负责与服务器建立连接并进行数据交换。
2. 当QTcpServer监听到传入的连接请求时，它会返回一个QTcpSocket对象，该对象用于与客户端进行数据交换。这个QTcpSocket对象是表示与客户端建立的连接的句柄，通过它可以实现数据的发送和接收。
3. QTcpServer可以管理多个QTcpSocket连接，接受多个客户端的连接请求，每个连接都有一个对应的QTcpSocket对象。
4. QTcpSocket对象在与服务器建立连接后，可以向服务器发送数据，也可以接收来自服务器的数据。服务器端的QTcpServer则可以接受来自客户端的数据，并向客户端发送数据。

## 三、QTcpServer源码解析

Qt源码下载：

```bash
git clone https://code.qt.io/qt/qt5.git                     # cloning the repo
cd qt5
git checkout 5.14.2                                         # checking out the specific release or branch
perl init-repository
```

## 3.1、QTcpServer的构造函数

先从QTcpServer的构造函数来看，下面是QTcpServer的构造函数原型：

```cpp
QTcpServer::QTcpServer(QObject *parent)
    : QObject(*new QTcpServerPrivate, parent)
{
    Q_D(QTcpServer);
#if defined(QTCPSERVER_DEBUG)
    qDebug("QTcpServer::QTcpServer(%p)", parent);
#endif
    d->socketType = QAbstractSocket::TcpSocket;
}
```

首先创建了一个QTcpServerPrivate的参数类。在QT源码中，每个类都有一个参数类，类名就是原类名加上Private。这个类主要放着QTcpServer类会用到的一些成员对象，而QTcpServer类里面只会定义方法，不会有成员对象。`QTcpServerPrivate`类的定义：

```cpp
// qtcpserver_p.h
class Q_NETWORK_EXPORT QTcpServerPrivate : public QObjectPrivate,
                                           public QAbstractSocketEngineReceiver
{
    Q_DECLARE_PUBLIC(QTcpServer)
public:
    QTcpServerPrivate();
    ~QTcpServerPrivate();

    QList<QTcpSocket *> pendingConnections;

    quint16 port;
    QHostAddress address;

    QAbstractSocket::SocketType socketType;
    QAbstractSocket::SocketState state;
    QAbstractSocketEngine *socketEngine;

    QAbstractSocket::SocketError serverSocketError;
    QString serverSocketErrorString;

    int maxConnections;

#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy proxy;
    QNetworkProxy resolveProxy(const QHostAddress &address, quint16 port);
#endif

    virtual void configureCreatedSocket();

    // from QAbstractSocketEngineReceiver
    void readNotification() override;
    void closeNotification() override { readNotification(); }
    void writeNotification() override {}
    void exceptionNotification() override {}
    void connectionNotification() override {}
#ifndef QT_NO_NETWORKPROXY
    void proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *) override {}
#endif

};
```

然后`QTcpServer`构造函数内部实现就很简单了。`Q_D(QTcpServer)`宏实际上就是取到QTcpServerPrivate对象的指针赋给变量d:

```cpp
#define Q_D(Class) Class##Private * const d = d_func()
#define Q_Q(Class) Class * const q = q_func()
```

`d->socketType = QAbstractSocket::TcpSocket`把套接字类型设置为Tcp。

至此，QTcpServer构造函数的工作结束。

## 3.2、调用listen函数启动tcpserver

一旦调用listen函数，tcpserver就开始运行了。接下来，连接、接收数据和发送数据的完成都可以通过信号来接收。那么，QT具体是如何实现等待连接和等待接收数据的呢？而且对于不同的平台又是如何实现的呢？我们来分析一下listen函数究竟都做了些什么工作。

（1）首先判断是否已是监听状态，是的话就直接返回。

```cpp
Q_D(QTcpServer);
if (d->state == QAbstractSocket::ListeningState) {
    qWarning("QTcpServer::listen() called when already listening");
    return false;
}
```

（2）接着设置协议类型，IP地址、端口号。

```cpp
QAbstractSocket::NetworkLayerProtocol proto = address.protocol();
    QHostAddress addr = address;

#ifdef QT_NO_NETWORKPROXY
    static const QNetworkProxy &proxy = *(QNetworkProxy *)0;
#else
    QNetworkProxy proxy = d->resolveProxy(addr, port);
#endif
```

（3）然后创建了一个socketEngine对象，它的类型是QAbstractSocketEngine。QAbstractSocketEngine定义了很多与原始套接字机制相似的函数，比如bind、listen、accept等方法，还实现了waitForRead、writeDatagram、read等函数。所以当我们调用QSocket的读写方法时，实际上是由QAbstractSocketEngine类来实现的。不过，QAbstractSocketEngine本身是一个抽象类，不能直接实例化。在listen函数中，我们调用了QAbstractSocketEngine类的静态函数createSocketEngine来创建对象。

```cpp
delete d->socketEngine;
d->socketEngine = QAbstractSocketEngine::createSocketEngine(d->socketType, proxy, this);
if (!d->socketEngine) {
    d->serverSocketError = QAbstractSocket::UnsupportedSocketOperationError;
    d->serverSocketErrorString = tr("Operation on socket is not supported");
    return false;
}
```

重点看一下createSocketEngine具体是怎么实现的：

```cpp
QAbstractSocketEngine *QAbstractSocketEngine::createSocketEngine(qintptr socketDescripter, QObject *parent)
{
    QMutexLocker locker(&socketHandlers()->mutex);
    for (int i = 0; i < socketHandlers()->size(); i++) {
        if (QAbstractSocketEngine *ret = socketHandlers()->at(i)->createSocketEngine(socketDescripter, parent))
            return ret;
    }
    return new QNativeSocketEngine(parent);
}
```

在类似递归的所有条件判断之后，最终返回一个QNativeSocketEngine对象。QNativeSocketEngine继承了QAbstractSocketEngine类，并实现了QAbstractSocketEngine的所有功能。在这个类的具体代码中可以看到一些做平台判断的代码，以及与平台相关的套接字函数。QNativeSocketEngine的实现并不只是一个文件，它包括qnativesocketengine_unix.cpp、qnativesocketengine_win.cpp和qnativesocketengine_winrt.cpp。因此，当在Windows平台编译程序时，编译器会包含qnativesocketengine_win.cpp文件，在Linux下编译时会包含qnativesocketengine_unix.cpp文件。QT通过一个抽象类和不同平台的子类来实现跨平台的套接字机制。

（4）继续回到TcpServer的listen函数，创建了一个socketEngine对象后开始调用bind、listen等函数来完成最终的socket设置。

```cpp
#ifndef QT_NO_BEARERMANAGEMENT
    //copy network session down to the socket engine (if it has been set)
    d->socketEngine->setProperty("_q_networksession", property("_q_networksession"));
#endif
    if (!d->socketEngine->initialize(d->socketType, proto)) {
        d->serverSocketError = d->socketEngine->error();
        d->serverSocketErrorString = d->socketEngine->errorString();
        return false;
    }
    proto = d->socketEngine->protocol();
    if (addr.protocol() == QAbstractSocket::AnyIPProtocol && proto == QAbstractSocket::IPv4Protocol)
        addr = QHostAddress::AnyIPv4;

    d->configureCreatedSocket();

    if (!d->socketEngine->bind(addr, port)) {
        d->serverSocketError = d->socketEngine->error();
        d->serverSocketErrorString = d->socketEngine->errorString();
        return false;
    }

    if (!d->socketEngine->listen()) {
        d->serverSocketError = d->socketEngine->error();
        d->serverSocketErrorString = d->socketEngine->errorString();
        return false;
    }
```

（5）接着开始设置信号接收，setReceiver传入TcpServerPrivate对象，从函数名可以看出是设置一个接收信息的对象，所以当套接字有新信息时，就会回调TcpServerPrivate对象的相关函数来实现消息通知。设置完消息接收对象以后，调用setReadNotificationEnabled(true)来启动消息监听。

```cpp
d->socketEngine->setReceiver(d);
d->socketEngine->setReadNotificationEnabled(true);
```

setReadNotificationEnabled函数的实现如下：

```cpp
void QNativeSocketEngine::setReadNotificationEnabled(bool enable)
{
    Q_D(QNativeSocketEngine);
    if (d->readNotifier) {
        d->readNotifier->setEnabled(enable);
    } else if (enable && d->threadData->hasEventDispatcher()) {
        d->readNotifier = new QReadNotifier(d->socketDescriptor, this);
        d->readNotifier->setEnabled(true);
    }
}
```

这个函数是创建了一个QReadNotifier对象，QReadNotifier的定义如下：

```cpp
class QReadNotifier : public QSocketNotifier
{
public:
    QReadNotifier(qintptr fd, QNativeSocketEngine *parent)
        : QSocketNotifier(fd, QSocketNotifier::Read, parent)
    { engine = parent; }

protected:
    bool event(QEvent *) override;

    QNativeSocketEngine *engine;
};

bool QReadNotifier::event(QEvent *e)
{
    if (e->type() == QEvent::SockAct) {
        engine->readNotification();
        return true;
    } else if (e->type() == QEvent::SockClose) {
        engine->closeNotification();
        return true;
    }
    return QSocketNotifier::event(e);
}
```

QReadNotifier其实就是继承了QSocketNotifier。QSocketNotifier是一个消息处理类，主要用来监听文件描述符的活动。也就是说，当文件描述符状态发生变化时，就会触发相应的信息。它可以监听三种状态：Read（读）、Write（写）、Exception（异常）。而我们这里用到的QReadNotifier主要是监听Read事件，也就是说当套接字句柄有可读消息时（连接信息也是可读信息的一种），就会调用event函数。在event函数中，我们回调了engine->readNotification()函数。readNotification函数的实现如下：

```cpp
void QTcpServerPrivate::readNotification()
{
    Q_Q(QTcpServer);
    for (;;) {
        if (pendingConnections.count() >= maxConnections) {
#if defined (QTCPSERVER_DEBUG)
            qDebug("QTcpServerPrivate::_q_processIncomingConnection() too many connections");
#endif
            if (socketEngine->isReadNotificationEnabled())
                socketEngine->setReadNotificationEnabled(false);
            return;
        }

        int descriptor = socketEngine->accept();
        if (descriptor == -1) {
            if (socketEngine->error() != QAbstractSocket::TemporaryError) {
                q->pauseAccepting();
                serverSocketError = socketEngine->error();
                serverSocketErrorString = socketEngine->errorString();
                emit q->acceptError(serverSocketError);
            }
            break;
        }
#if defined (QTCPSERVER_DEBUG)
        qDebug("QTcpServerPrivate::_q_processIncomingConnection() accepted socket %i", descriptor);
#endif
        q->incomingConnection(descriptor);

        QPointer<QTcpServer> that = q;
        emit q->newConnection();
        if (!that || !q->isListening())
            return;
    }
}
```

在这个函数里面调用了socketEngine->accept()来获取套接字句柄，然后将其传递给q->incomingConnection(descriptor)来创建QTcpSocket对象。最后发送了emit q->newConnection()信号。如果使用过QTcpServer，那么对这个信号应该很熟悉。

因此，QT通过内部消息机制实现了套接字的异步通信。并且，对外提供的函数既支持同步机制，也支持异步机制。调用者可以选择通过信号槽机制来实现异步，也可以调用类似waitforread、waitforconnect等函数来实现同步等待。实际上，waitforread等同步函数是通过函数内部的循环来检查消息标志。当标志为可读或者函数超时时则返回。

## 3.3、QSocketNotifier的实现

在前面提到了使用QSocketNotifier，可以在套接字有可读或可写信号时调用event函数来实现异步通知。但是，QSocketNotifier又是如何知道套接字什么时候发生变化的呢？QSocketNotifier的实现和QT的消息处理机制是密切相关的。要完全讲清楚这一点，就必须涉及到QT的消息机制。

这里只把比较关键的代码抽取出来分析一下。首先，不同平台的消息处理机制都是不一样的，所以QSocketNotifier在不同平台下的实现也是不一样的。我们主要看一下Linux平台下是如何实现的。

（1）QSocketNotifier类的声明和定义，用于通知状况以支持异步 IO（输入/输出）操作。

```cpp
class QSocketNotifierPrivate;
class Q_CORE_EXPORT QSocketNotifier : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSocketNotifier)

public:
    enum Type { Read, Write, Exception };

    QSocketNotifier(qintptr socket, Type, QObject *parent = nullptr);
    ~QSocketNotifier();

    qintptr socket() const;
    Type type() const;

    bool isEnabled() const;

public Q_SLOTS:
    void setEnabled(bool);

Q_SIGNALS:
    void activated(int socket, QPrivateSignal);

protected:
    bool event(QEvent *) override;

private:
    Q_DISABLE_COPY(QSocketNotifier)
};
```

QSocketNotifier类同时声明了一个嵌套类QSocketNotifierPrivate，这个类用来实现QSocketNotifier的私有方法和属性。QSocketNotifier类继承自QObject类，并且使用了Q_OBJECT宏来支持信号和槽机制以及元对象特性。

QSocketNotifier类还提供了一些函数，用于获取套接字句柄、设置套接字的类型（可读、可写、异常）、获取状态等。可以通过setEnable(bool)槽函数来设置通知器的状态（开启或关闭）。通过activated(int socket, QPrivateSignal)信号来通知有关套接字状态的改变。采用了Q_DISABLE_COPY宏来禁止类的复制构造函数和赋值运算符的使用。

（2）SocketNotifier构造函数：

```cpp
QSocketNotifier::QSocketNotifier(qintptr socket, Type type, QObject *parent)
    : QObject(*new QSocketNotifierPrivate, parent)
{
    Q_D(QSocketNotifier);
    d->sockfd = socket;
    d->sntype = type;
    d->snenabled = true;

    if (socket < 0)
        qWarning("QSocketNotifier: Invalid socket specified");
    else if (!d->threadData->hasEventDispatcher())
        qWarning("QSocketNotifier: Can only be used with threads started with QThread");
    else
        d->threadData->eventDispatcher.loadRelaxed()->registerSocketNotifier(this);
}
```

QSocketNotifier的构造函数需要传入一个套接字句柄以及要监听的类型，比如读、写或错误。然后在构造函数里调用了QSocketNotifierPrivate的registerSocketNotifier函数，将自己注册进去。这样当有消息触发时，就能调用这个对象的event函数了。

（3）registerSocketNotifier函数：

```cpp
/*****************************************************************************
 QEventDispatcher implementations for UNIX
 *****************************************************************************/

void QEventDispatcherUNIX::registerSocketNotifier(QSocketNotifier *notifier)
{
    Q_ASSERT(notifier);
    int sockfd = notifier->socket();
    QSocketNotifier::Type type = notifier->type();
#ifndef QT_NO_DEBUG
    if (notifier->thread() != thread() || thread() != QThread::currentThread()) {
        qWarning("QSocketNotifier: socket notifiers cannot be enabled from another thread");
        return;
    }
#endif

    Q_D(QEventDispatcherUNIX);
    QSocketNotifierSetUNIX &sn_set = d->socketNotifiers[sockfd];

    if (sn_set.notifiers[type] && sn_set.notifiers[type] != notifier)
        qWarning("%s: Multiple socket notifiers for same socket %d and type %s",
                 Q_FUNC_INFO, sockfd, socketType(type));

    sn_set.notifiers[type] = notifier;
}
```

在这个函数里面主要是将对象和套接字句柄sockfd作为映射放入socketNotifiers里面。

```cpp
QHash<int, QSocketNotifierSetUNIX> socketNotifiers;
```

（4）processEvents函数处理所有消息，Linux平台的实现如下：

```cpp
bool QEventDispatcherUNIX::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    Q_D(QEventDispatcherUNIX);
    d->interrupt.storeRelaxed(0);

    // we are awake, broadcast it
    emit awake();
    QCoreApplicationPrivate::sendPostedEvents(0, 0, d->threadData);

    const bool include_timers = (flags & QEventLoop::X11ExcludeTimers) == 0;
    const bool include_notifiers = (flags & QEventLoop::ExcludeSocketNotifiers) == 0;
    const bool wait_for_events = flags & QEventLoop::WaitForMoreEvents;

    const bool canWait = (d->threadData->canWaitLocked()
                          && !d->interrupt.loadRelaxed()
                          && wait_for_events);

    if (canWait)
        emit aboutToBlock();

    if (d->interrupt.loadRelaxed())
        return false;

    timespec *tm = nullptr;
    timespec wait_tm = { 0, 0 };

    if (!canWait || (include_timers && d->timerList.timerWait(wait_tm)))
        tm = &wait_tm;

    d->pollfds.clear();
    d->pollfds.reserve(1 + (include_notifiers ? d->socketNotifiers.size() : 0));

    if (include_notifiers)
        for (auto it = d->socketNotifiers.cbegin(); it != d->socketNotifiers.cend(); ++it)
            d->pollfds.append(qt_make_pollfd(it.key(), it.value().events()));

    // This must be last, as it's popped off the end below
    d->pollfds.append(d->threadPipe.prepare());

    int nevents = 0;

    switch (qt_safe_poll(d->pollfds.data(), d->pollfds.size(), tm)) {
    case -1:
        perror("qt_safe_poll");
        break;
    case 0:
        break;
    default:
        nevents += d->threadPipe.check(d->pollfds.takeLast());
        if (include_notifiers)
            nevents += d->activateSocketNotifiers();
        break;
    }

    if (include_timers)
        nevents += d->activateTimers();

    // return true if we handled events, false otherwise
    return (nevents > 0);
}
```

可以看到一个处理套接字相关的函数qt_safe_poll。看看它的内部实现：

```cpp
int qt_safe_poll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts)
{
    if (!timeout_ts) {
        // no timeout -> block forever
        int ret;
        EINTR_LOOP(ret, qt_ppoll(fds, nfds, nullptr));
        return ret;
    }

    timespec start = qt_gettime();
    timespec timeout = *timeout_ts;

    // loop and recalculate the timeout as needed
    forever {
        const int ret = qt_ppoll(fds, nfds, &timeout);
        if (ret != -1 || errno != EINTR)
            return ret;

        // recalculate the timeout
        if (!time_update(&timeout, start, *timeout_ts)) {
            // timeout during update
            // or clock reset, fake timeout error
            return 0;
        }
    }
}
```

qt_safe_poll调用了qt_ppoll，qt_ppoll的定义如下：

```cpp
static inline int qt_ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts)
{
#if QT_CONFIG(poll_ppoll) || QT_CONFIG(poll_pollts)
    return ::ppoll(fds, nfds, timeout_ts, nullptr);
#elif QT_CONFIG(poll_poll)
    return ::poll(fds, nfds, timespecToMillisecs(timeout_ts));
#elif QT_CONFIG(poll_select)
    return qt_poll(fds, nfds, timeout_ts);
#else
    // configure.json reports an error when everything is not available
#endif
}
```

这里通过QT_CONFIG的标志来判断采用哪种实现。`qt_poll`是QT自己的函数，在早期的版本中可能都是用select模式。但是从QT5.7开始，采用了poll模式。博主使用的是QT5.14.2版本，也是采用的poll模式。选择poll模式的原因是因为select模式监听的套接字长度是用的定长数组，无法在运行时扩展。一旦套接字数量超过FD_SETSIZE就会返回错误，在Linux默认的设置中，FD_SETSIZE是1024。

qt_poll的实现如下，内部使用的是select：

```cpp
int qt_poll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts)
{
    if (!fds && nfds) {
        errno = EFAULT;
        return -1;
    }

    fd_set read_fds, write_fds, except_fds;
    struct timeval tv, *ptv = 0;

    if (timeout_ts) {
        tv = timespecToTimeval(*timeout_ts);
        ptv = &tv;
    }

    int n_bad_fds = 0;

    for (nfds_t i = 0; i < nfds; i++) {
        fds[i].revents = 0;

        if (fds[i].fd < 0)
            continue;

        if (fds[i].events & QT_POLL_EVENTS_MASK)
            continue;

        if (qt_poll_is_bad_fd(fds[i].fd)) {
            // Mark bad file descriptors that have no event flags set
            // here, as we won't be passing them to select below and therefore
            // need to do the check ourselves
            fds[i].revents = POLLNVAL;
            n_bad_fds++;
        }
    }

    forever {
        const int max_fd = qt_poll_prepare(fds, nfds, &read_fds, &write_fds, &except_fds);

        if (max_fd < 0)
            return max_fd;

        if (n_bad_fds > 0) {
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            ptv = &tv;
        }

        const int ret = ::select(max_fd, &read_fds, &write_fds, &except_fds, ptv);

        if (ret == 0)
            return n_bad_fds;

        if (ret > 0)
            return qt_poll_sweep(fds, nfds, &read_fds, &write_fds, &except_fds);

        if (errno != EBADF)
            return -1;

        // We have at least one bad file descriptor that we waited on, find out which and try again
        n_bad_fds += qt_poll_mark_bad_fds(fds, nfds);
    }
}
```

其实Linux还有更高效的IO多路复用器，叫做epoll。

（5）activateSocketNotifiers函数处理事件：

```cpp
int QEventDispatcherUNIXPrivate::activateSocketNotifiers()
{
    markPendingSocketNotifiers();

    if (pendingNotifiers.isEmpty())
        return 0;

    int n_activated = 0;
    QEvent event(QEvent::SockAct);

    while (!pendingNotifiers.isEmpty()) {
        QSocketNotifier *notifier = pendingNotifiers.takeFirst();
        QCoreApplication::sendEvent(notifier, &event);
        ++n_activated;
    }

    return n_activated;
}
```

在processEvents函数中调用了qt_safe_poll来检查是否有套接字事件。如果有事件需要处理，就会调用activateSocketNotifiers函数，而在这个函数中会通过`QCoreApplication::sendEvent(notifier, &event)`将消息反馈给`QSocketNotifier`。

通过这个过程，可以了解到Qt在Linux下使用select或者poll来实现输入输出复用的具体流程。但是具体采用哪种方式取决于使用的Qt版本。

## 总结

针对Qt 5.14.2的QTcpServer源码分析，QTcpServer的异步事件默认采用的poll模式（通过QT_CONFIG的标志来判断采用哪种实现，在早期的版本中可能都是用select模式。但是从QT5.7开始，采用了poll模式），poll模式解决了select模式监听的套接字长度是定长数组的问题，但是对事件的响应还是通过轮询的方式。

## Qt的TCP编程模型参考文档2-CP/IP网络通信（以及文件传输）

TCP/IP通信（即SOCKET通信）是通过网线将**服务器Server端**和**客户机Client端**进行连接，在遵循ISO/OSI模型的四层层级构架的基础上通过TCP/IP协议建立的通讯。控制器可以设置为服务器端或客户端。

关于TCP/IP协议可详看：[TCP/IP协议详解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/33889997)

 总的来说，TCP/IP通讯有两个部分：

- **客户端**和**服务器**
- **QTcpServer（监听套接字）**和**QTcpSocket（通讯套接字）**

监听套接字，顾名思义，监听关于各种通讯的状态，一旦进行通讯，监听套接字会启动通讯套接字，进行通讯

客户端使用connectToHost函数主动连接服务器后，服务器会触发newConnectio这个槽函数，并进行取出QTcpServer（监听套接字），将相关内容取出并赋给QTcpSocket（通讯套接字）。
客户端向服务器发送数据，触发readyRead（），进行处理，彼此传递时，原理都是这样的。

**对双方来说都起作用的部分：**

1. 一旦建立连接，就会触发connected，服务器特殊一点，触发的是newConnectio
2. 互传数据也是一样的，一旦接受到，就会触发readyread

服务器中，需要监听套接字以及通讯套接字，监听套接字用于监听客户端是否给服务器发送请求

本篇博文做了初步的学习与尝试，编写了一个客户端和服务器基于窗口通信以及文件传输的小例程。

------

###  **一，客户端**

客户端的代码比服务器稍简单，总的来说，使用QT中的**QTcpSocket类**与服务器进行通信只需要以下5步：

**（1）创建QTcpSocket套接字对象**

```
    socket = new QTcpSocket(this);
```

**（2）使用这个对象连接服务器**

```
    QString ip = ui.lineEdit_ip->text();//获取ip
    int port = ui.lineEdit_2->text().toInt();//获取端口数据
    socket->connectToHost(ip, port);
```

**（3）使用write函数向服务器发送数据**

```
    QByteArray data = ui.lineEdit_3->text().toUtf8();//获取lineEdit控件中的数据并发送给服务器
    socket->write(data);
```

 **（4）当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据**

```
 connect(socket, &QTcpSocket::readyRead, this, &QTcpClinet::ReadData);
void QTcpClinet::ReadData()
{
    QByteArray buf = socket->readAll();
    ui.textEdit->append(buf);
}
```

**（5）断开与服务器的连接（关于close()和disconnectFromHost()的区别，可以按F1看帮助）**

```
socket->disconnectFromHost();
```

**客户端例程：（新建一个qt项目QTcpClinet（客户机））**

- **ui界面**

![img](https://img2020.cnblogs.com/blog/2282730/202108/2282730-20210826162917843-732412414.png)

本地回路ip：127.0.0.1 可以连接到本地ip（电脑内部循环的ip）

如果要和局域网其他ip连接 -> 在运行（win+R）+cmd+ipconfig ->ipv4地址 查看本机ip

- **QTcpClinet.h**

```
#include <QtWidgets/QWidget>
#include "ui_QTcpClinet.h"
#include"QTcpSocket.h"
#pragma execution_character_set("utf-8")
class QTcpClinet : public QWidget
{
    Q_OBJECT

public:
    QTcpClinet(QWidget *parent = Q_NULLPTR);
    ~QTcpClinet();
public slots:
    void on_btn_connect_clicked();
    void ReadData();
    void on_btn_push_clicked();
private:
    Ui::QTcpClinetClass ui;
    QTcpSocket* socket;//创建socket指针
};
```

- **QTcpClinet.cpp**

```
#include "QTcpClinet.h"

QTcpClinet::QTcpClinet(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    socket = new QTcpSocket(this);
}

QTcpClinet::~QTcpClinet()
{
    delete this->socket;//回收内存
}

void QTcpClinet::on_btn_connect_clicked()
{
  if (ui.btn_connect->text()==tr("连接服务器"))
  {
    QString ip = ui.lineEdit_ip->text();//获取ip
    int port = ui.lineEdit_2->text().toInt();//获取端口数据
    //取消已有的连接
    socket->abort();
    //连接服务器
    socket->connectToHost(ip, port);
    bool isconnect = socket->waitForConnected();//等待直到连接成功
    //如果连接成功
    if (isconnect)
    {
        ui.textEdit->append("The connection was successful!!");
        ui.btn_push->setEnabled(true);//按钮使能
        //修改按键文字
        ui.btn_connect->setText("断开服务器连接");
        //接收缓冲区（服务器）信息
        connect(socket, &QTcpSocket::readyRead, this, &QTcpClinet::ReadData);
    }
    else
    {
        ui.textEdit->append("The connection falied!!");
    }
  }
  else
  {
      //断开连接
      socket->disconnectFromHost();
      ui.btn_connect->setText("连接服务器");
      ui.btn_push->setEnabled(false);//关闭发送按钮使能
  }

}

//接收缓冲区信息函数
void QTcpClinet::ReadData()
{
    QByteArray buf = socket->readAll();
    ui.textEdit->append(buf);
}
//发送按钮事件
void QTcpClinet::on_btn_push_clicked()
{
    QByteArray data = ui.lineEdit_3->text().toUtf8();//获取lineEdit控件中的数据并发送给服务器
    socket->write(data);
    //判断是否写入成功
    bool iswrite = socket->waitForBytesWritten();
    if (iswrite)
    {
        //写入成功
    }
    else
    {
        //没有写入成功
    }
}
```

### **二，服务器（需要一直运行哦）**

服务器除了使用到了**QTcpSocket类**，还需要用到**QTcpSever类**。即便如此，也只是比客户端复杂一点点，用到了6个步骤：

**（1）创建QTcpSever对象**

```
    server = new QTcpServer(this);
```

**（2）侦听一个端口，使得客户端可以使用这个端口访问服务器**

```
    server->listen(QHostAddress::Any, 6677);//监听所有ip和6677端口
```

**（3）当服务器被客户端访问时，会发出newConnection()信号，因此为该信号添加槽函数，并用一个QTcpSocket对象接受客户端访问**

```
connect(server, &QTcpServer::newConnection, this, &TcpServer::ClientConnect);
void TcpServer::ClientConnect()
{
    //解析所有客户连接
    while (server->hasPendingConnections())
    {
        //连接上后通过socket（QTcpSocket对象）获取连接信息
        socket = server->nextPendingConnection();
        QString str = QString("[ip:%1,port:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());//监听客户端是否有消息发送
        connect(socket, &QTcpSocket::readyRead, this, &TcpServer::ReadData1);
    }
}
```

**（4）使用socket的write函数向客户端发送数据**

```
socket->write(data);
```

**（5）当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据**

```
//监听客户端是否有消息发送
connect(socket, &QTcpSocket::readyRead, this, &TcpServer::ReadData1);
//获取客户端向服务器发送的信息
void TcpServer::ReadData1()
{
    QByteArray buf = socket->readAll();//readAll最多接收65532的数据
    QString str = QString("[ip:%1,port:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    ui.textEdit_server->append(str +QString(buf));
    //socket->write("ok");//服务器接收到信息后返回一个ok
}
```

**（6）取消侦听**

```
server->close();
```

 **服务器例程：（添加一个新的qt项目TcpServer（服务器））**

- **ui界面**

**![img](https://img2020.cnblogs.com/blog/2282730/202108/2282730-20210826202602015-2042680269.png)**

-  **TcpServer.h**

```
#include <QtWidgets/QWidget>
#include"ui_TcpServer.h"
#include"qtcpserver.h"
#include"qtcpsocket.h"

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = Q_NULLPTR);
    ~TcpServer();
public slots:
    void on_btn_server_clicked();
    void on_btn_listen_clicked();
private:
    Ui::TcpServerClass ui;
    QTcpServer* server;
    QTcpSocket* socket;//一个客户端对应一个socket
    void ClientConnect();
    void ReadData1();
    
};
```

- **TcpServer.cpp**

```
#include "TcpServer.h"
#include"qstring.h"
#include"qdebug.h"
#pragma execution_character_set("utf-8")
TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    server = new QTcpServer(this);
   //客户机连接信号槽
    connect(server, &QTcpServer::newConnection, this, &TcpServer::ClientConnect);
}

TcpServer::~TcpServer()
{
    server->close();
    server->deleteLater();
}

void TcpServer::on_btn_listen_clicked()
{
    if (ui.btn_listen->text()=="侦听")
    {
        //从输入框获取端口号
        int port = ui.lineEdit_port->text().toInt();
        //侦听指定端口的所有ip
        if (!server->listen(QHostAddress::Any, port))
        {
            //若出错，则输出错误信息
            qDebug() << server->errorString();
            return;
        }
        //修改按键文字
        ui.btn_listen->setText("取消侦听");    
    }
    else
    {
        socket->abort();
        //取消侦听
        server->close();
        //修改按键文字
        ui.btn_listen->setText("侦听");
    }
}

void TcpServer::ClientConnect()
{
    //解析所有客户连接
    while (server->hasPendingConnections())
    {
        //连接上后通过socket获取连接信息
        socket = server->nextPendingConnection();
        QString str = QString("[ip:%1,port:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());
        //提示连接成功
        ui.textEdit_server->append(str+"Connect to the server");
        //复选框选项为连接服务器的ip
        ui.comboBox->addItem(str);
        //将socket地址放入combobox属性内
        //ui.comboBox->setItemData(ui.comboBox->count()-1, QVariant((int)socket));
        //监听客户端是否有消息发送
        connect(socket, &QTcpSocket::readyRead, this, &TcpServer::ReadData1);
    }
}

//获取客户端向服务器发送的信息
void TcpServer::ReadData1()
{
    QByteArray buf = socket->readAll();//readAll最多接收65532的数据
    QString str = QString("[ip:%1,port:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    ui.textEdit_server->append(str +QString(buf));
}

//服务器向客户端发送信息
void TcpServer::on_btn_server_clicked()
{
  if(ui.comboBox->count()== 0)return;
  //QTcpSocket* skt=  (QTcpSocket*)ui.comboBox->itemData(ui.comboBox->currentIndex()).value<int>();
  socket->write(ui.lineEdit1->text().toUtf8());
}
```

**注意**：write中需要写入char类型的元素或QByteArray类型的元素

![img](https://img2020.cnblogs.com/blog/2282730/202108/2282730-20210826204430904-933620540.png)

**效果展示：**

![img](https://img2020.cnblogs.com/blog/2282730/202108/2282730-20210826203045302-1130708011.gif)

### **三，TCP/IP文件传输**

上文实现了消息的传输，由于**socket->readAll();（readAll最多接收65532的数据）**，因此对于大文件的传输用此方法是不可取的。

**TCP/IP文件传输的思路：**

1. 客户端和服务器连接
2. 客户端选择文件，并发送文件给服务器（发送的是文件的帧头，格式：文件名&大小）
3. 服务器触发readyRead，然后解析文件帧头（获取文件名和大小），并返回客户端一个ok消息
4. 客户端触发readyRead，然后发送文件数据，通过progressBar显示进度
5. 服务器再次触发readyRead，接收文件数据，并保存（通过ishead判断接收的是文件帧头还是文件数据）

**代码实现：**

新建服务器项目（TcpServer）

- **TcpServer.h**

```
#pragma once

#include <QtWidgets/QWidget>
#include "ui_TcpServer.h"
#include"qtcpserver.h"
#include"qtcpsocket.h"
#pragma execution_character_set("utf-8")
class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = Q_NULLPTR);
    void hasConnect();
private:
    Ui::TcpServerClass ui;
    QTcpServer* server;
    QTcpSocket* socket;
    bool ishead;
    QString fileName;
    int fileSize;//接收文件的总大小
    int recvSize;//当前接收文件的大小
    QByteArray filebuf;//当前接收的文件数据
};
```

- **TcpServer.cpp**

```
#include "TcpServer.h"
#include"qfile.h"
TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
{
    ishead = true;
    ui.setupUi(this);
    server = new QTcpServer(this);
    //监听1122端口的ip
    server->listen(QHostAddress::Any, 1122);
    //如果有用户连接触发槽函数
    connect(server, &QTcpServer::newConnection, this, &TcpServer::hasConnect);
}

void TcpServer::hasConnect()
{
    while (server->hasPendingConnections()>0)//判断当前连接了多少人
    {
        //用socket和我们的客户端连接，一个客户端对应一个套接字socket
        socket = server->nextPendingConnection();
        //服务器界面上输出客户端信息
        ui.textEdit->append(QString("%1：新用户连接").arg(socket->peerPort()));
        //如果客户端发送信息过来了，触发匿名函数
        connect(socket, &QTcpSocket::readyRead, [=]() {
            QByteArray buf = socket->readAll();
            //用一个标志位ishead判断是头还是数据位
            if (ishead)
            {
                //如果是头，解析头（文件名，文件大小）
                QString str = QString(buf);
                ui.textEdit->append(str);
                QStringList strlist = str.split("&");
                fileName = strlist.at(0);//解析帧头文件名
                fileSize = strlist.at(1).toInt();//解析帧头文件大小
                ishead = false;//下次接收到的文件就是我们的数据
                recvSize = 0;
                filebuf.clear();
                socket->write("ok");
            }
            else
            {
                //根据文件名和文件大小接收和保存文件
                filebuf.append(buf);
                recvSize += buf.size();//每接收一次文件，当前文件大小+1
                //当接收文件大小等于总文件大小，即文件数据接收完毕
                if (recvSize>=fileSize)
                {
                    //保存文件
                    QFile file(ui.lineEdit->text() + fileName);
                    file.open(QIODevice::WriteOnly);
                    file.write(filebuf);
                    file.close();
                    ishead = true;
                }
            }
            });
    }
}
```

新建客户端项目（QTcpClient）

- **QTcpClient.h**

```
#include <QtWidgets/QWidget>
#include"ui_QTcpClient.h"
#include"qtcpsocket.h"
#pragma execution_character_set("utf-8")
class QTcpClient : public QWidget
{
    Q_OBJECT

public:
    QTcpClient(QWidget *parent = Q_NULLPTR);
public slots:
    void on_btn_connect_clicked();
    void on_btn_choose_clicked();
    void on_btn_open_clicked();
    
private:
    Ui::QTcpClientClass ui;
    QTcpSocket* socket;
};
```

- **QTcpClient.cpp**

```
#include "QTcpClient.h"
#include"qfiledialog.h"
#include"qfileinfo.h"
QTcpClient::QTcpClient(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    socket = new QTcpSocket(this);
    
}
void QTcpClient::on_btn_connect_clicked()
{
    QString ip = ui.lineEdit_ip->text();//获取ip
    int port = ui.lineEdit_port->text().toInt();//获取端口数据
    socket->connectToHost(ip, port);//连接服务器
    //等待连接成功
    if (socket->waitForConnected())
    {
        ui.textEdit->append("<font color='green'>连接服务器成功！</font>");    
        ui.btn_open->setEnabled(true);
        
        //如果服务器发送信息到客户端，触发匿名函数
        connect(socket, &QTcpSocket::readyRead, [=]() {
            //读取服务器发送的信息（即缓冲区信息）
            QByteArray buf = socket->readAll();
            if (buf=="ok")
            {
                QFile file = (ui.label_path->text());
                if (!file.open(QIODevice::ReadWrite))
                {
                    //读取文件失败
                    return;
                }
                qint64 currentlen = 0;//当前已经发送的大小
                qint64 allLength = file.size();//总文件大小
                do
                {
                    char data[1024];
                    qint64 msize = file.read(data, 1024);//读文件放入打他数组中，返回读取到的大小
                    socket->write(data, msize);//把读取到的data数据发送给服务器
                    currentlen += msize;//实时获取当前发送的文件大小
                    ui.progressBar->setValue(currentlen *100 / allLength);//更新界面进度条
                } while (currentlen < allLength);//当发送文件等于文件大小时，发送完毕，循环结束
            }
            });

    }
    else
    {
        ui.textEdit->append("<font color='red'>连接服务器失败！</font>");
    }
}
//选择文件事件
void QTcpClient::on_btn_choose_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "打开文件", "", "(*.*)");
    ui.label_path->setText(path);
}
//发送文件事件
void QTcpClient::on_btn_open_clicked()
{
    QFileInfo info(ui.label_path->text());
    //用QFileInfo：：fileName，size获取文件名和大小 格式：文件名&大小
    //服务器用该格式解析文件名和大小
    QString head = QString("%1&%2").arg(info.fileName()).arg(info.size());
    //将该格式发送给服务器 toUtf8：QString转QByteArray或char类型
    socket->write(head.toUtf8()); 
}
```

**效果展示：**

![img](https://img2020.cnblogs.com/blog/2282730/202108/2282730-20210827172234074-999439735.gif)

 

 

参考博文：[(5条消息) Qt网络编程之TCP通信（二）文件传输_毓树麟风的博客-CSDN博客](https://blog.csdn.net/qq_34578785/article/details/106992357)

​         [(5条消息) QTCpSocket文件传输_Apollon_krj的博客-CSDN博客](https://blog.csdn.net/Apollon_krj/article/details/72876746)

## QtTCP编程参考文档3： https://developer.aliyun.com/article/1560252