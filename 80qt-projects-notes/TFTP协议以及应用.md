# 1.概述

什么是TFTP协议？

TFTP是“Trivial File Transfer Protocol”的缩写。它比我们平时听到的FTP（文件传输协议）简单得多。 [[1](https://www.h3c.com/cn/d_200904/632641_473262_0.htm)]

**核心特点：**

- **基于UDP：** 它使用无连接的UDP协议来发送数据，就像扔皮球一样，发出去就不管对方有没有准备好。
- **无身份验证：** 它没有用户名和密码的检查过程，直接连接即可读写文件。
- **资源消耗小：** 因为协议本身很小，代码量少，非常适合写在一些硬件芯片或嵌入式系统里。 [[1](https://docs.oracle.com/cd/E55849_01/html/E54255/makehtml-id-38.html), [2](https://www.cisco.com/c/zh_cn/support/docs/routers/10000-series-routers/48700-tftp-server.pdf), [3](https://support.huawei.com/enterprise/zh/doc/EDOC1100278540/1670fa0b), [4](https://zhuanlan.zhihu.com/p/575030131), [5](https://www.ibm.com/docs/zh/i/7.6.0?topic=services-trivial-file-transfer-protocol), [6](https://www.h3c.com/cn/d_200904/632641_473262_0.htm)]

它是如何工作的？

把TFTP比作**“在超市自助取货”**：

1. **请求（Request）：** 客户端向TFTP服务器喊一声（发送请求），说：“我想要拿/放这个文件。”
2. **连接（Connection）：** 服务器同意后，会指定一个临时的通信端口与客户端对接。
3. **传输（Transfer）：** 采用“一问一答”的模式（停止-等待机制）。服务器发送一个数据块（通常是512字节），客户端收到后回复确认信息；服务器收到确认后，才会发下一个数据块。 [[1](https://cloud.tencent.com/developer/article/1441564), [2](https://zhuanlan.zhihu.com/p/575030131)]

TFTP的常见应用

因为TFTP速度快且占用资源极少，它在局域网内有着非常核心的地位：

- **路由器和交换机的固件/配置更新：**
  当网管需要给几百台网络设备批量升级系统，或者备份设备的配置文件时，通常会搭建一个TFTP服务器。设备开机后可以直接通过TFTP下载系统镜像，极大地提高了维护效率。 [[1](https://blog.csdn.net/weixin_74021557/article/details/131260340), [2](https://www.cisco.com/c/zh_cn/support/docs/routers/10000-series-routers/48700-tftp-server.pdf)]
- **无盘工作站启动（PXE网络引导）：**
  很多电脑主机没有安装硬盘（无盘工作站）。当它们开机时，会通过局域网连接到服务器，利用TFTP协议把操作系统引导文件下载到本地内存中并启动电脑。 [[1](https://blog.csdn.net/weixin_74021557/article/details/131260340)]
- **物联网与嵌入式设备开发：**
  开发智能硬件或开发板时，由于硬件系统太小，无法运行复杂的网络协议栈，工程师通常会使用TFTP将编译好的测试程序快速推送到设备中运行。

## 参考文档1： https://github.com/RT-Thread-packages/netutils/tree/master/tftp



## 参考文档2：

> **TFTP（Trivial File Transfer Protocol,简单文件传输协议）**是TCP/IP协议族中的一个用来在客户机与服务器之间进行简单文件传输的协议，提供不复杂、开销不大的文件传输服务。端口号为69。
> 相较于FTP，TFTP的设计就是以传输小文件为目标，协议实现简单很多。FTP是一个传输文件的简单协议，它基于[UDP协议](https://zhida.zhihu.com/search?content_id=244832324&content_type=Article&match_order=1&q=UDP协议&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyODE0ODcsInEiOiJVRFDljY_orq4iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyNDQ4MzIzMjQsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.k5hcx5YbYajAz9dmc7pzXeD1VBp6mwpkw8xak1evc10&zhida_source=entity)而实现。它不具备通常的FTP的许多功能，它只能从文件服务器上获得或写入文件，不能列出目录，不进行认证，它传输8位数据。TFTP代码所占的内存较小，广泛应用于没有硬盘的嵌入式设备。



![img](https://picx.zhimg.com/v2-a4e2001c43c471f2a0c17938ec381e51_1440w.jpg)





### 2.1 TFTP报文格式



![img](https://picx.zhimg.com/v2-9ac05a6202fca4944823db9917aaa92d_1440w.jpg)



TFTP共定义了5种类型的包，包的类型由数据包前两个字节确定，我们称之为Opcode（操作码）字段。

| TFTP报文类型 | 英文名称               | Opcode值 |
| ------------ | ---------------------- | -------- |
| 读文件请求包 | RRQ（Read request）    | 1        |
| 写文件请求包 | WRQ（Write requst）    | 2        |
| 文件数据包   | DATA                   | 3        |
| 回应包       | ACK（Acknowledgement） | 4        |
| 错误信息包   | ERROR                  | 5        |

若为错误信息包，会包含差错码，TFTP包括以下7中差错码。

| 差错码 | 说明           |
| ------ | -------------- |
| 0      | 不识别         |
| 1      | 无法找到文件   |
| 2      | 访问无效       |
| 3      | 磁盘空间已满   |
| 4      | 无效的TFTP操作 |
| 5      | 不识别的端口   |
| 6      | 文件已存在     |
| 7      | 没有此用户     |



### 2.2 TFTP数据传输

TFTP的传输模式与FTP相同，支持二进制和ASCII码两种模式。

- **二进制模式：**用于传输程序文件。是TFTP默认传输模式，在二进制传输模式下，TFTP协议不会对数据进行任何转换，直接将数据块传输给对方。
- **ASCII码模式：**用于传输文本文件。在ASCII码传输模式下，TFTP协议会将数据块中的换行符转换为本地的换行符，并将文件末尾的空格去掉。



### 2.3 TFTP交互流程

**2.3.1 读文件流程**



![img](https://pica.zhimg.com/v2-e0d01a4f042d85dbf664ef4b2c9f2536_1440w.jpg)



1. RRQ：客户端发送一个数据包给服务器，其中包含了要读取的文件名。
2. DATA：服务器发回第一个512字节数据块，并对其标号为1
3. ACK：客户端返回服务器一个标号为1的确认数据包
4. DATA：服务器发送标号为2包含512字节的数据块
5. ACK：客户端收到2号数据块后发生确认数据包
6. DATA：服务器发送标号为3的包含176字节的数据块
7. ACK：客户端收到后回发标号为3的确认数据包
8. 服务器收到确认数据包后，确认文件发送完毕

**2.3.2 写文件流程**



![img](https://pic3.zhimg.com/v2-8134b2eb86923283b1347af5e5a07b80_1440w.jpg)



1. WRQ：客户端发送一个写请求数据包，里面包含了要写的文件名称
2. ACK：服务器发送确认数据包，在数据包中它使用编号0
3. DATA：客户端发送编号为1，包含512字节的数据包
4. ACK：服务器返回编号为1的确认数据包
5. DATA：客户端发送编号为2，包含512字节的数据包
6. ACK：服务器返回编号为2的确认数据包
7. DATA：客户端发送编号为3，包含176字节的数据包，等待服务器返回确认数据包。
8. ACK：服务器接受3号数据包后，返回确认数据包，由于该数据包数据少于512字节，服务器知道这是最后一个数据包。
9. 客户端收到3号确认数据包后，知道文件传输完毕，中断连接。

## 参考文档2： tftp协议详解

### 01. tftp协议简介

TFTP（Trivial File Transfer Protocol,简单文件传输协议）是TCP/IP协议族中的一个用来在客户机与服务器之间进行简单文件传输的协议，提供不复杂、开销不大的文件传输服务，端口号为69。

TFTP通常基于**UDP**协议而实现，但是也不能确定有些TFTP协议是基于其它传输协议完成的。TFTP协议的设计目的主要是为了进行小文件传输，因此它不具备通常的FTP的许多功能，例如，它只能从文件服务器上获得或写入文件，不能列出目录，不进行认证。

TFTP代码所占的内存较小，这对于较小的计算机或者某些特殊用途的设备来说是很重要的，这些设备不需要硬盘，只需要固化了TFTP、UDP和IP的小容量只读存储器即可。因此，随着嵌入式设备在网络设备中所占的比例的不断提升，TFTP协议被越来越广泛的使用。

### 02. tftp包格式

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9e484b72aff95e8d4dc70db3aa6ff2a1.png#pic_center)

#### 2.1 tftp包格式概述

TFTP共定义了六种类型的包，包的类型由数据包前两个字节确定，我们称之为Opcode（操作码）字段。这五种类型的数据包分别是：

- 读文件请求包：Read request，简写为RRQ，对应Opcode字段值为1

- 写文件请求包：Write requst，简写为WRQ，对应Opcode字段值为2

- 文件数据包：Data，简写为DATA，对应Opcode字段值为3

- 回应包：Acknowledgement，简写为ACK，对应Opcode字段值为4

- 错误信息包：Error，简写为ERROR，对应Opcode字段值为5

  RRQ和WRQ的数据包格式一样，只不过某些值域设置有差别，剩下的三种数据包格式各不相同。

- 请求恢复包: Acknowledgement,简写为ACK，对应Opcode字段值为6

#### 2.2 读/写请求包(RRQ/WRQ=1/2)

RRQ和WRQ数据包的格式：
\1. 操作码（2字节），它用来表示当前数据包的类型（取值1表示该数据包是读请求，2表示该数据包是写请求）；

\2. 可变长字段，它用来表示要读取或上传的文件名，它使用ASCII码并以 \0 表示结尾；

\3. Mode，也是可变长字段，用来表示传输文件的数据类型，如果传输的是字符串文件，那么它填写字符串”netascii”，如果传输的是二进制文件，那么它填写字符串”octet”,这些字符串都以 \0 结尾。

\4. 可选字段（timeout、blksize、tsize等，以 \0 结尾）

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/20c11ac61aac0ce27d6cb00b7ef447f9.png#pic_center)

**数据抓包分析**

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/60d72ef85a34dc63363b1c567ec29079.png#pic_center)

**协议分析**

```shell
00 01                                       (操作码)
31 2e 63 00                                 (1.c(文件名), 以 \0 结尾 即 00)
6f 63 74 65 74 00                           (octet(文件传输方式), 以 \0 结尾 即 00)
62 6c 6b 73 69 7a 65 00                     (blksize(每次传输文件大小), 以 \0 结尾 即 00)
35 31 32 00                                 (字符串), 以 \0 结尾 即 00)
74 73 69 7a 65 00                           (tsize(文件大小), 以 \0 结尾 即 00)
30 00                                       (0(字符串), 以 \0 结尾 即 00)
AI写代码shell1234567
```

#### 2.3 数据包(DATA=3)

传输数据块的DATA数据包，
\1. 操作码（2字节）
\2. 块编号（2字节）
\3. 数据

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/fa6fb6b2c5f4aa0ae2b3c466edb2670a.png#pic_center)

**数据抓包分析**
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/acc0fd14fd69ea6c4cd0ead9586e7be5.png#pic_center)

**协议分析**

```shell
00 03                 (操作码)
00 01                 (block number)
.....                 (DATA)
AI写代码shell123
```

#### 2.4 回应包(ACK=4)

回应包是对收到的文件数据进行回应
\1. 操作码（2字节）
\2. 数据包序（2字节）

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/e6af3766e3d3fb9814e42f214fff4275.png#pic_center)

**数据抓包分析**
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/8161f9395cefb9266e522564caaaea63.png#pic_center)

**协议分析**

```shell
00 04             (操作码)
00 01             (block number)
AI写代码shell12
```

#### 2.5 错误信息包(Error=5)

错误ERROR数据包
\1. 操作码（2字节）
\2. 错误码（2字节）
\3. 错误具体原因（可变长）

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/1289eef1043c6261e1124280ab5ebc91.png#pic_center)

#### 2.6 请求响应包（ACK=6）

请求响应包是对请求数据的响应，为可选包，某种情况下是可以不需要的，视具体情况而定。
\1. 操作码（2字节）
\2. 可选字段（timeout、blksize、tsize等，以 \0 结尾 以及相关的值）

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/7aa522242fe61ac89d9dad064d8397b6.png#pic_center)

**数据抓包分析**
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/c5340685e0c4eba0d34094d4f20d5208.png#pic_center)

**协议分析**

```shell
00 06                                  (操作码)
62 6c 6b 73 69 7a 65 00                (blksize, 以 \0 结尾 即 00)
35 31 32 00                            (512(字符串), 以 \0 结尾 即 00)
74 73 69 7a 65 00                      (tsize(文件大小), 以 \0 结尾 即 00)
31 39 34 37 00                         (1947(字符串), 以 \0 结尾 即 00)
AI写代码shell12345
```

### 03. tftp工作过程

#### 3.1 tftp下载过程

下图为TFTP请求下载的过程图，上传与下载本质上是文件的传输方向不一样。

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/ffdc3fd1d86168315a3d64066eb0076b.png#pic_center)

#### 3.2 tftp上传过程

TFTP的工作都是由客户端发起一个RRQ或者WRQ开始的。这里以WRQ（写请求）为例，讲述读写的工作过程，以及错误处理等内容。
\1. server在端口为69的UDP上等待client发出写文件请求包

\2. client通过UDP发送符合TFTP请求格式的WRQ包给server。从UDP包角度看，该UDP包的源端口由client随意选择，而目标端口则是server的69。

\3. server收到client的这个请求包后，需发送ACK给client。对于写请求包，server发送的ACK包确认号为0。

\4. client发送DATA数据给server，server接收数据并写文件

\5. 当client发送的DATA数据长度小于512字节时，server认为这次WRQ请求完成

### 04. TFTP的传输模式

\1. Netascii：这是8位的ASCII码形式，一般用来传输字符数据

\2. Octet：这是8位源数据类型，一般用来传输二进制数据

\3. Mail：它将返回的数据直接返回给用户而不是保存为文件，但该模式已经不再支持

### 05. 讨论

```bash
1.tftp服务介绍
	1）TFTP（Trivial File Transfer Protocol,简单文件传输协议）是TCP/IP协议族中的一个用来在客户机与服务器之间进行简单文件传输的协议，提供不复杂、开销不大的文件传输服务。
	2）tftp服务的端口号为69。
	3）TFTP是一个传输文件的简单协议，它基于UDP协议而实现，但是我们也不能确定有些TFTP协议是基于其它传输协议完成的。
	4）此协议设计的时候是进行小文件传输的。因此它不具备通常的FTP的许多功能，它只能从文件服务器上获得或写入文件，不能列出目录，不进行认证，它传输8位数据。
	5）传输中有三种模式：
		1》netascii，这是8位的ASCII码形式；
		2》另一种是octet，这是8位源数据类型；
		3》最后一种mail已经不再支持，它将返回的数据直接返回给用户而不是保存为文件。
	
2.tftp传输过程
	1）任何传输起自一个读取或写入文件的请求，这个请求也是连接请求。
	2）如果服务器批准此请求，则服务器打开连接，数据以定长512字节传输。
	3）每个数据包包括一块数据，服务器发出下一个数据包以前必须得到客户对上一个数据包的确认。
	4）如果一个数据包的大小小于512字节，则表示传输结束。如果数据包在传输过程中丢失，发出方会在超时后重新传输最后一个未被确认的数据包。
	5）通信的双方都是数据的发出者与接收者，一方传输数据接收应答，另一方发出应答接收数据。
	6）大部分的错误会导致连接中断，错误由一个错误的数据包引起。这个包不会被确认，也不会被重新发送，因此另一方无法接收到。如果错误包丢失，则使用超时机制。
	7）错误主要是由三种情况引起的：
		1》不能满足请求。
		2》收到的数据包内容错误，而这种错误不能由延时或重发解释。
		3》对需要资源的访问丢失（如硬盘满）。
	8）TFTP只在一种情况下不中断连接，这种情况是源端口不正确，在这种情况下，指示错误的包会被发送到源机。

3.tftp特点
	1）由于TFTP使用UDP，而UDP使用IP，IP可以还使用其它本地通信方法。因此一个TFTP包中会有以下几段：本地媒介头，IP头，数据报头，TFTP头，剩下的就是TFTP数据了。
	2）TFTP在IP头中不指定任何数据，但是它使用UDP中的源和目标端口以及包长度域。
	3）由于TFTP使用的包标记（TID）在这里被用做端口，因此TID必须介于0到65,535之间。
	4）TFTP头中包括两个字节的操作码，这个码指出了包的类型，大体上的TFTP包格式为：| Local Medium | Internet | Datagram | TFTP |
	
4.tftp优点
	1）TFTP可用于UDP环境；比如当需要将程序或者文件同时向许多机器下载时就往往需要使用到TFTP协议。
	2）TFTP代码所占的内存较小，这对于较小的计算机或者某些特殊用途的设备来说是很重要的，这些设备不需要硬盘，只需要固化了TFTP、UDP和IP的小容量只读存储器即可。当电源接通后，设备执行只读存储器中的代码，在网络上广播一个TFTP请求。网络上的TFTP服务器就发送响应，其中包括可执行二进制程序。设备收到此文件后将其放入内存，然后开始运行程序。这种方式增加了灵活性，也减少了开销。
	
5.初始链接
	1）初始连接时候需要发出WRQ（请求写入远程系统）或RRQ（请求读取远程系统），收到一个确定应答，一个确定可以写出的包或应该读取的第一块数据。
	2）通常确认包包括要确认的包的包号，每个数据包都与一个块号相对应，块号从1开始而且是连续的。因此对于写入请求的确定是一个比较特殊的情况，因此它的包的包号是0。
	3）如果收到的包是一个错误的包，则这个请求被拒绝。
	4）创建连接时，通信双方随机选择一个TID，因为是随机选择的，因此两次选择同一个ID的可能性就很小了。
	5）每个包包括两个TID，发送者ID和接收者ID。这些ID用于在UDP通信时选择端口，请求主机选择ID的方法上面已经说过了，在第一次请求的时候它会将请求发到TID 69，也就是服务器的69端口上。应答时，服务器使用一个选择好的TID作为源TID，并用上一个包中的TID作为目的ID进行发送。这两个被选择的ID在随后的通信中会被一直使用。
	6）下例是一个写入的例子，其中WRQ，ACK和DATA代表写入请求，确认和数据。
		1》主机A向主机B发出WRQ，其中端口为69。
		2》B机向A机发出ACK，块号为0，包括B和A的TID
			此时连接建立，第一个数据包以序列号1从主机开始发出。以后两台主机要保证以开始时确定的TID进行通信。
			如果源ID与原来确定的ID不一样，这个包会被认识为发送到了错误的地址而被抛弃。错误的包是被发送到正确端口的，但是包本身有错误。
			设想发送方发出一个请求，这个请求在网络的那个设备中被复制成两个包，接收方先后接收到两个包。接收方会认为为这是两个独立的请求，会返回两个应答。
			当这两个应答其中之一被接收到时，连接已经建立。第二个应答再到达时，这个包会被抛弃，而不会因为接收到第二个应答包而导致第一个建立的连接失败。
6.正常终止
	1）传输的结束由DATA数据标记，其包括0-511个字符。这个包可以被其它数据包确认。
	2）接收方在发出对最后数据包的确认后可以断开连接，当然，适当的等待是比较好的，如果最后的确定包丢失可以再次传输。
	3）如果发出确认后仍然收到最后数据包，可以确定最后的确认丢失。发送最后一个DATA包的主机必须等待对此包的确认或超时。
	4）如果响应是ACK，传输完成。如果发送方超时并不准备重新发送并且接收方有问题或网络有问题时，发送也正常结束。当然实现时也可以是非正常结束，但无论如何连接都将被关闭。
	
7.早终结
	如果请求不能被满足，或者在传输中发生错误，需要发送ERROR包。这仅是一种传输友好的方式，这种包不会被确认也不会被重新传输，因此这种包可能永远不会被接收到。因此需要用超时来侦测错误。

8.TFTP数据包
	TFTP支持五种类型的包，已经说明这五种类型的包：
	包头中包括了这个包所指定的操作码。
		opcode operation
		1．Read request (RRQ)
		2．Write request (WRQ)
		3．Data (DATA)
		4．Acknowledgment (ACK)
		5．Error (ERROR)

9.tftp命令
	man tftp
	tftp - Trivial File Transfer Protocol client
	tftp是一个客户端，是一个不重要轻量级的文件传输协议，用于传输文件到远程主机上或从远程机器上下载数据。
	tftp [-4][-6][-v][-l][-m mode] [host [port]] [-c command]

10.in.tftpd命令
	man in.tftpd
	tftpd - Trivial File Transfer Protocol server
	in.tftpd [options...]  directory...
	
11.tftp实现
	tftp包有：tftp，tftp-server
	rpm -ql tftp
		/usr/bin/tftp
		/usr/share/doc/tftp-0.49
		/usr/share/doc/tftp-0.49/CHANGES
		/usr/share/doc/tftp-0.49/README
		/usr/share/doc/tftp-0.49/README.security
		/usr/share/doc/tftp-0.49/README.security.tftpboot
		/usr/share/man/man1/tftp.1.gz
	rpm -ql tftp-server
		/etc/xinetd.d/tftp
		/usr/sbin/in.tftpd
		/var/lib/tftpboot（库）
		/usr/share/doc/tftp-server-0.49
		/usr/share/doc/tftp-server-0.49/CHANGES
		/usr/share/doc/tftp-server-0.49/README
		/usr/share/doc/tftp-server-0.49/README.security
		/usr/share/doc/tftp-server-0.49/README.security.tftpboot
		/usr/share/man/man8/in.tftpd.8.gz
		/usr/share/man/man8/tftpd.8.gz

bash1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374757677787980818283848586878889909192939495969798
```
