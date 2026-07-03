# 1.简介

SNMP（简单网络管理协议）是网络设备的“通用遥控器”。它工作在应用层，让管理员能在中央电脑上远程查看、设置和监控路由器、交换机或服务器的状态。它通过 **[\**\*\*![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAACbElEQVR4AaRSTWxMURT+zn3tlIX42UinP0EraSyUnZX4SbRFRJsQlUgqET87XSG2JEKEJatGGiqxkFgQK8KCnSZIpTWdv6edMVpV896837nOudJRdNeX+835znfO+d69867CMp8lDT41JkdTO3fo9MDAGvGfGb6pS3evVoX/iyUNuKmTiFBf788Ub1y8RokEyFLE+n+rZvC0tXXLy2Szlo64WkXV9RDPl1VcnL0QpnKI7KKUkO7apcc3tz0wCf/UDKIYH2MQ3iSb/IgI8cwcosI3hNkpBOMZBKk80vv3DkWlWUTzTj/PmvXHgIdClkJNic7CNAWOhyhXQDiZRzCRhT9urw5zUwPRVAk+93KrWcZgqGWT7rOzFHBBiq8bm3SFuVfx4NtfEXzOIcxkf/ilOVR4l9v5BS+SzeZPNQYhNw+3bNQ+qe+MWx4pOMrqdU0kdBQLymHuWirlEj3jYe2Dh3gPxuB0LkU+54y1FaJBl9RBH3hUgep2YN1+xW8rJurWOaA2rvUw4Cm9ledhDIScyk1SSDgUkNUfKFrlWtaJPdP557un84OuVddvadUl0bGs3gNfcnTYtt/LXM1AEg/qSZkwUoa635fPPBRN0MP8p8aVssZIRdNj/ta1O/GXwTk+yhnGyfykJYOLcdTOtB+zM3TcTvNhwR6/q8agHtiGRc9CvgJoXeBSFp4AOgQrgRbRjAHbvePiWREEkkuMgHt8N0YbgPY64APr+/jbjTE2cO2y9BgDIdx4RyI3mjvLUfNBL7HWEAMTHMd46DpHsN4tUWAMuMCapADz9QwSsOlbVn3hjCPMpU6sn2eYHf8CAAD//6Yx0G0AAAAGSURBVAMAGPoCMI9zsDYAAAAASUVORK5CYII=)\*\**\*⁠华为百科](https://info.support.huawei.com/info-finder/encyclopedia/zh/SNMP.html)** 等官方资源被广泛应用于IT运维中。

核心三要素

- **NMS（网络管理系统）：** 总控制台，安装在管理员的电脑上，负责发号施令。
- **Agent（代理）：** 被管理的设备（如打印机、交换机），负责收集信息和执行命令 **[\**\*\*![img](https://encrypted-tbn3.gstatic.com/faviconV2?url=https://www.ad.siemens.com.cn&client=AIM&size=128&type=FAVICON&fallback_opts=TYPE,SIZE,URL)\*\**\*⁠西门子支持](https://www.ad.siemens.com.cn/club/bbs/post_elite.aspx?b_id=7&a_id=1855438)**。
- **MIB（管理信息库）：** 设备的“说明书”，用树状结构记录所有参数（如CPU温度、流量大小）。每个参数都有唯一的数字地址，叫做 OID（对象标识符） **[\**\*\*![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAACCUlEQVR4AXyS3WuOYRzHP/cVWUleduQlDyWFI2kZG+MvIElGDOcSSTuRiYgs5YSkZZLVSomEpRwIh2gOSOzk8bra1tbWnr3c176/a9e93XtpdX+f3/V7+X5/3+vqcZz3ngbvuSRcEa4JN4Sbwi3htk+5o3hXaBKahYdCi9DqvWMEZsVorA+RhH4a82nzkwJGyJAfytfsnOKxGOEmkowUGzPq1rfesByNSiTmLthTssSaigznrFotg/UmkSR21qyzTSvK4HM9HN0IW5bC4fU6C/vXwIG18KUO2o8o1sLHg9BxSDMFLZK4Mwc1q6DxOZyshPKFcG4H/BuA+ip48lWETtjWBIXFUPkAXv6Ex98lIBdOt+Gdhor/YaAEf39Dn8ivvsHgEKQauvpGxHIR9C2S7Q8/wGu7uQ8CVRugtho6++FEjaa8IGKiuGu5epthzyZYMA8adkPFarhcAU4iTm9Kyydoa4fuPrj4FMrmQ0FXsfi6Q1d7BmdeQEmE023KhbNvIZUbp12s08Md3wmnRG7WQxV7oPWYHu2PuiKZ1eplOsuRlzPL7e0sBoFiF2xv1Bvozvvuw957sPU61D0SyQjC+1+w8oL+tNpqxCCgehAYTDQYP9XGT9oWBqODkhq9RlYM9RiDwDhjll8TsUGDCWXRhGI+t0CmacMZTCSHqQK20UjTo9VMwIgWcxgDAAD////voSoAAAAGSURBVAMAFRcClA2YTOkAAAAASUVORK5CYII=)\*\**\*⁠知乎](https://zhuanlan.zhihu.com/p/183876012)**。

主要工作方式

- **轮询（读/写）：** NMS 主动向 Agent 发送指令，读取参数或修改设备的配置。这就像你去超市买东西，问店员（Agent）商品的价格 **[\**\*\*![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAC6UlEQVR4AXSSa0hUQRiG35ldqc1Ck/IShoUV2U2RNbTcXMm0C0qiJEKSgpfK+iGsVpCwpmV4QX+oaZAEQhRGIYpmikfdWsJdMckfknYjYY2kTM3r2XP6zpoalcO8c+ab837PfHPmcKzW0q2bcN6iV+SW0+y9mu1fQLr1NNJ7LRs0k1+3bhwRfN3fC1qfvs8l3bqhUtORTKOgV/8JWwEYBTUyrFcAdjR4p9l1r/eA6Ok6Cjfnb/g+5YZn/Sd3WN4FVfyY9GyrFPTr8butAGzOEZCRCsiXXg2F7CCpKQFLan9zDA/NiShrMugNj/NboGxIkEVAmoWSeSPFmwjS6rP504tI/1YsKWJ/G4J8LfD1GIbzmp+YWdCEekyLWvJjEcBYAmQ5iRZSwLngxMVRmmONeo4Se3A8oBkJhx7gQmQl8s5cR0lSFrKjbxQqHu4ohUENxupo4Slk6fbwF9/45/1RaOyNwc0nuYMGnYnZnVRboVL5McjRDMxIfu+il/oADptLAGQkAwSh4e9OJe8uNuk6VQtSi2y3V0syU7wiZHsKn8cIR02gFZIYAoUqyw0AXgP4CHoGbu+dVcpmQBh93H3KkzHEyZALAG6S1GIwx8UeT3BVPRm2gzOByLmUnERrsREH2gZc1o1T+N/eCVFt5ag6OErnUXY9BxnlYKpGACZI9g9FDVe1uY9uwVBX5pAyNw+GlstQhZN3eDMwzskMqDSJ/j59Y5xLjnC1YWZeA1Iyg12gTQdTwjtnFwFVe6fsklN7RsQdx5Xt9HoL+pWXOcp1erraELLLjNA93WsBZjDouktBbRFAkwFbUFptR1r7xLSLqNvdPZATU9hVnJTl0LXYgq5TgU3WKP+W8rV8Plcj8hpKcfRlAKiKOfe5E+ahw5drhVQ1nTcsu67MIWN9fti9jrRtoxMe424Sr8gM75xyZNOwAqAAxnARNdpq3NX6Uejn7jIW7+VqOwvGg7Bl0qs67n5eCp2b3i33XwAAAP//iLznZgAAAAZJREFUAwAG/B9p1uNpZAAAAABJRU5ErkJggg==)\*\**\*⁠深信服社区](https://bbs.sangfor.com.cn/forum.php?mod=viewthread&tid=133559)**。
- **告警（Trap/Inform）：** 当设备发生故障时，Agent 会主动向 NMS 报告 华为百科。这就像警报器自动响了。

常用端口与版本

- **端口：** 通常使用 **UDP** 协议。NMS 询问设备使用 **161** 端口，设备主动报警使用 **162** 端口 **[\**\*\*![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAABPUlEQVR4AaSPsU7DQBBE944qJZJbKIC/oAZRUbpAoktlqEgk8Bc4EU5oIgchIb6CBlp+h5oqXu6dOOvwWUIolsc3Ozs7e7ay5ZMEzOvVSbVoFIRsOKAXtHAmAa2xb+WkMEbN2WzRXFV1M4aj0QuD4UwCaFRNs6tGXw/2ssfD/ewFjkavjySATeZL7sWaozzPNwCORu/PAAx3k2IsG13x38C0+uA1mj0kN6BfLdcfYqVlI1AjO16T9PkVwDZvUT0ub4pzz93Hc6c5Kp2HwqEL6DeoYzhv96KHwgcgcNUgclLHQAtAZ4baQhAo/gNmmPU3GBqkGWPIg2attqfBCEcEbIiBBvDEfns7vX4PRjgmUfmcLZsLz91nXq8v0RwVPLF/8BfKaZGpynPY1Bp9QiOgj8EATG7LyMH8YIQ2hG8AAAD//+SjXN0AAAAGSURBVAMA/l3GIWV2148AAAAASUVORK5CYII=)\*\**\*⁠IBM 文档](https://www.ibm.com/docs/zh/networkmanager/4.2.0?topic=ports-list-used-by-product)**。
- **版本对比：**
  - **SNMPv1 / v2c：** 使用明文密码认证，非常简单但容易被黑客窃听，安全性低 **[\**\*\*![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAABSUlEQVR4AaySv0oDQRjEZ78YMEJQtNNcaeIlr2Bjp4hPIIKFla2CjWBAEESw8glEBFsLawtFsBTy71LedoKFCCYGbt05m09JCk8Xfnfzzc0Ny90K/rj+r6BTLR2QKJxb5aaiMDjU0OuEpS1mCGciUTXYjcKSMw514mBWkC63D2gAcWaTGcJ3urXZNYFzJ2k+w8Ulci0FwYymlx/ssUt71PQ+pLhMrZE+MK0Z7+UKDAcN+6KhN5Z/ndRZakkSdDXISZ3hYUjfXOkstQwL/sYTCNa/YXA+qiAx7gg/8lJu2EtNpWkfRxVUWvZGZ6mF/1PjD8sZC7RHTa+7ENxRa8QBF3yYBePMjvhtbQxMv+iHe+K/aswyag09GDzRg8NtYQoT8+341OeBWvP5zQ+LKS17zHCq2/GX5++p14q36Zfbdil4sO/00gKKrHwCAAD//9zmjgoAAAAGSURBVAMAAjrQIfjGFuIAAAAASUVORK5CYII=)\*\**\*⁠Fortinet](https://www.fortinet.com/cn/resources/cyberglossary/simple-network-management-protocol)**。
  - **SNMPv3：** 增加了加密和强密码验证，安全级别最高。

# 2.参考文档1

### 一、什么是SNMP 

 SNMP：“简单网络管理协议”，用于网络管理的协议。SNMP用于网络设备的管理。SNMP的工作方式：管理员需要向设备获取数据，所以SNMP提供了“读”操作；管理员需要向设备执行设置操作，所以SNMP提供了“写”操作；设备需要在重要状况改变的时候，向管理员通报事件的发生，所以SNMP提供了“Trap”操作。

### 二、SNMP背景

SNMP的基本思想：为不同种类的设备、不同厂家生产的设备、不同型号的设备，定义为一个统一的接口和协议，使得管理员可以是使用统一的外观面对这些需要管理的网络设备进行管理。通过网络，管理员可以管理位于不同物理空间的设备，从而大大提高网络管理的效率，简化网络管理员的工作。

### 三、SNMP结构概述

SNMP被设计为工作在TCP/IP协议族上。SNMP基于TCP/IP协议工作，对网络中支持SNMP协议的设备进行管理。所有支持SNMP协议的设备都提供SNMP这个统一界面，使得管理员可以使用统一的操作进行管理，而不必理会设备是什么类型、是哪个厂家生产的。如下图，

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/jmridcdf7l.png)

### 四、SNMP支持的网管操作

对于网络管理，我们面对的数据是设备的配置、参数、状态等信息，面对的操作是读取和设置；同时，因为网络设备众多，为了能及时得到设备的重要状态，还要求设备能主动地汇报重要状态，这就是报警功能。如下图，

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/vqq8lfobjm.png)

-  Get：读取网络设备的状态信息。  
-  Set：远程配置设备参数。  
-  Trap：管理站及时获取设备的重要信息。    

### 五、SNMP的实现结构

在具体实现上，SNMP为管理员提供了一个网管平台(NMS)，又称为管理站，负责网管命令的发出、数据存储、及数据分析。被监管的设备上运行一个SNMP代理(Agent))，代理实现设备与管理站的SNMP通信。如下图，

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/xgw6podasq.png)

A **management information base** (**MIB**) is a database used for managing the entities in a communication network. Most often associated with the **Simple Network Management Protocol** (**SNMP**), the term is also used more generically in contexts such as in OSI/ISO Network management model.

管理站与代理端通过MIB进行接口统一，MIB定义了设备中的被管理对象。管理站和代理都实现了相应的MIB对象，使得双方可以识别对方的数据，实现通信。管理站向代理申请MIB中定义的数据，代理识别后，将管理设备提供的相关状态或参数等数据转换为MIB定义的格式，应答给管理站，完成一次管理操作。

已有的设备，只要新加一个SNMP模块就可以实现网络支持。旧的带扩展槽的设备，只要插入SNMP模块插卡即可支持网络管理。网络上的许多设备，路由器、交换机等，都可以通过添加一个SNMP网管模块而增加网管功能。服务器可以通过运行一个网管进程实现。其他服务级的产品也可以通过网管模块实现网络管理，如Oracle、WebLogic都有SNMP进程，运行后就可以通过管理站对这些系统级服务进行管理。

根据管理者和被管理的设备在网络管理操作中的不同职责，SNMP定义了3种角色。如下图，

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/ll2dyrljpb.png)

网络管理系统：又称管理站、NMS。是系统的控制台，向管理员提供界面以获取与改变设备的配置、信息、状态、操作等信息。管理站与Agent进行通信，执行相应的Set和Get操作，并接收代理发过来的警报(Trap)。 

代理：Agent是网络管理的代理人，负责管理站和设备SNMP操作的传递。介于管理站和设备之间，与管理站通信并相应管理站的请求，从设备获取相应的数据，或对设备进行相应的设置，来响应管理站的请求。代理也需要具有根据设备的相应状态使用MIB中定义的Trap向管理站发送报告的能力。 

代理服务器：Proxy是一种特殊的代理，在不能直接使用SNMP协议的地方，如：异种网络、不同版本的SNMP代理等情况，Proxy代替相关设备向管理站提供一种外观，为设备代理SNMP协议的实现。Proxy做了异种网络或不同版本代理和相应SNMP数据请求的转换工作。如下图， 

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/gi94fp45jd.png)

附：管理信息库MIB：定义了设备上可以使用的管理信息。代理和管理站使用MIB作为统一的数据接口通信。

### 六、SNMP的技术内容

如下图，

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/kkaxdjprr8.png)

### 七、SNMP的发展历史

-  1989年------ SNMPv1  
-  1991年------ RMON(Remote Network Monitoring 远程网络监视)，它扩充了SNMP的功能，包括对LAN的管理及对依附于这些网络的设备的管理。RMON 没有修改和增加SNMPv1，只是增加了SNMP监视子网的能力。  
-  1993年------ SNMPv2(SNMPv1的升级版)  
-  1995年------ SNMPv2正式版，其中规定了如何在基于OSI的网络中使用SNMP  
-  1995年------ RMON扩展为RMON2  
-  1998年------ SNMPv3，一系列文档定义了SNMP的安全性，并定义了将来改进的总体结构，SNMPv3可以和SNMPv2、SNMPv1一起使用。    

### 八、SNMP的技术术语

-  SNMP：Simple Network Management Protocol(简单网络管理协议)，是一个标准的用于管理基于IP网络上设备的协议。  
-  MIB：Management Information Base(管理信息库)，定义代理进程中所有可被查询和修改的参数。  
-  SMI：Structure of Management Information(管理信息结构)，SMI定义了SNMP中使用到的ASN.1类型、语法，并定义了SNMP中使用到的类型、宏、符号等。SMI用于后续协议的描述和MIB的定义。每个版本的SNMP都可能定义自己的SMI。  
-  ASN.1：Abstract Syntax Notation One(抽象语法定义)。用于定义语法的正式语言，在SNMP中定义SNMP的协议数据单元PDU和管理对象MIB的格式。SNMP只使用了ASN.1中的一部分，而且使用ASN.1的语言特性定义了一些自定义类型和类型宏 ，这些组成了SMI。  
-  PDU： Protocol Data Unit(协议数据单元)，它是网络中传送的数据包。每一种SNMP操作，物理上都对应一个PDU。  
-  NMS： Network Management System，网络管理系统，又名网络管理站，简称“管理站”。它是SNMP的总控机，提供统一的用户界面访问支持SNMP的设备，一般提供UI界面，并有统计、分析等功能，是网管系统的总控制台。NMS是网络管理操作的发起者。  
-  Agent： 是SNMP的访问代理，简称“代理”，为设备提供SNMP能力，负责设备与NMS的通信。  
-  Proxy： 代理服务器，对实现不同协议的设备进行协议转换，使非IP协议的设备也能被管理。  
-  Trap： 是由设备主动发出的报警数据，用于提示重要的状态的改变。  
-  BER： Basic Encoding Rule，基本编码规格。描述如何将ASN.1类型的值编码为字符串的方法。它是ASN.1标准的一部分。BER编码将数据分成TLV三部分，T为Tag的缩写，是类型标识；L为Length的缩写，标识类型的长度；V为Value的缩写，标识数据内容。按照TLV的顺序对数据进行编码，生成字节流。SNMP使用BER将SNMP的操作请求和应答编码后进行传输，并用于接收端进行解码。  

### 九、综合上述（总结）

#### 1.SNMP现有几个版本

SNMP共有v1，v2，v3这三个版本：

-  v1和v2都具有基本的读、写MIB功能。  
-  v2增加了警报、批量数据获取、管理站和管理站通信能力。  
-  v3在v2的基础上增加了USM，使用加密的数据和用户验证技术，提高了安全性。  
-  另外，RMON是SNMP的一个重要扩展，为SNMP增加了子网流量、统计、分析能力。现有两个版本： Rmon：提供了OSI七层网络结构中网络层和数据链路层监视能力。Rmon2：提供了OSI七层网络结构中网络层之上各层的监视能力。    

#### 2.ASN.1、BER、SMI、MIB、PDU的关系

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/dzvysdkds6.png)

-  ASN.1：高级的数据描述语言。描述数据的类型、结构、组织、及编码方法。包括符号和语法两部分。SNMP使用ASN.1描述PDU和管理学对象信息库MIB。  
-  BER：ASN.1的基本编码规则。描述具体的ASN.1对象如何编码为比特流在网络上传输。SNMP使用BER作为编码方案，数据首先先经过BER编码，再经由传输层协议(一边是UDP)发往接收方。接收方在SNMP端口上收到PDU后，经过BER解码后，得到具体的SNMP操作数据。  
-  SMI：是SNMP的描述方法。规定了使用ASN.1子类型、符号。ASN.1功能强大，但SNMP只用到了其中很小一部分，对于这一部分内容的描述，限定了范围，即为SMI。SMI规定了使用到的ASN.1类型、宏、符号等。SMI是ASN.1的一个子集和超集。  
-  MIB：是SNMP中使用到的管理信息库。定义了数据格式、类型、顺序、意义等，使用SMI中定义的类型和ASN.1中的基本类型对对象进行描述，是一个使用SMI描述的管理信息库。每一类关心的事件都有一组MIB，比如网络接口有一颗MIB树，TCP有一颗MIB树，UDP也有一颗MIB树。  
-  PDU：是SNMP的协议数据单元。PDU是基本的通信格式，使用ASN.1描述，使用BER编码，通过传输层协议传送。  

#### 3.比较规范的SNMP背景

SNMP作为一种过渡技术以其简单易用的特性被广泛使用，使用的网络产品提供对SNMP的支持。SNMP还扩展了具有远程管理功能的RMON，使得管理人员可以对整个子网进行管理，而不是对整个子网内的设备进行管理。

### 十、Net-SNMP

#### 1.net-snmp简介

使用net-snmp作为学习SNMP的工具，搭建一个net-snmp环境。Net-snmp是一个开源的SNMP项目。最新版本为：5.7.2。net-snmp支持snmpv1、snmpv2、snmpv3，支持基于IPv4和IPv6的SNMP应用程序。

-  Net-snmp提供完整的API用于SNMP应用程序开发，包括C和Perl的API 。  
-  一个功能强大且可扩展的SNMP代理：snmpd开发者可以开发动态模块扩展snmpd，net-snmp内置扩展子代理与主代理的通信协议。  
-  提供众多[命令行工具](https://cloud.tencent.com/developer/techpedia/1411?from_column=20065&from=20065)检查和使用SNMP协议  
-  一个图形化的MIB浏览工具  
-  一个Trap接收进程，用于接收和显示Trap，并将Trap记录到[日志文件](https://cloud.tencent.com/developer/techpedia/1623?from_column=20065&from=20065)中。    

#### 2.net-snmp的安装与配置

官方网站： http://www.net-snmp.org/download.html，net-snmp被很多商业化的Linux包含，但是大多数的Linux使用net-snmp的主代理实现Linux的SNMP支持。

#### 3.支持的平台

net-snmp可以在BSD UNIX、Linux、Windows等操作系统上编译运行。Windows下可以安装一个VMWare虚拟机软件，新建一个虚拟机，在虚拟机上安装Linux用来学习。

#### 4.实验环境 

-  操作系统，CentOS 6.4 X86_64 
-  软件版本，net-snmp 5.5 

#### 5.同步时间

   

   



```javascript
[root@snmp ~]# ntpdate 202.120.2.101
```

#### 6.安装yum源

   

   



```javascript
[root@node1 ~]# rpm -ivh http://download.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm
[root@node1 ~]# rpm -ivh http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.3-1.el6.rf.x86_64.rpm
```

#### 7.查看一下net-snmp相关软件包

   

   



```javascript
[root@node1 ~]# yum list all | grep net-snmp*
net-snmp.x86_64             1:5.5-44.el6_4.4        updates
net-snmp-devel.i686           1:5.5-44.el6_4.4        updates
net-snmp-devel.x86_64          1:5.5-44.el6_4.4        updates
net-snmp-libs.i686            1:5.5-44.el6_4.4        updates
net-snmp-libs.x86_64           1:5.5-44.el6_4.4        updates
net-snmp-perl.x86_64           1:5.5-44.el6_4.4        updates
net-snmp-python.x86_64          1:5.5-44.el6_4.4        updates
net-snmp-utils.x86_64          1:5.5-44.el6_4.4        updates
```

注，有两个重要的安装，net-snmp.x86_64 、net-snmp-utils.x86_64 一个是net-snmp软件包，另一个是snmp的工具包。下面我们来安装一下，

   

   



```javascript
[root@node1 ~]# yum install -y net-snmp net-snmp-utils
```

#### 8.查看一下安装的软件包

```javascript
[root@node1 ~]# rpm -qa | grep net-snmp*
net-snmp-5.5-44.el6_4.4.x86_64
net-snmp-utils-5.5-44.el6_4.4.x86_64
net-snmp-libs-5.5-44.el6_4.4.x86_64
```

#### 9.查看一下软件包具体安装的内容

下面是snmp软件包的内容，

```javascript
[root@node1 ~]# rpm -ql net-snmp-5.5-44.el6_4.4.x86_64
/etc/rc.d/init.d/snmpd #snmp的启动脚本
/etc/rc.d/init.d/snmptrapd
/etc/snmp #snmp配置文件目录
/etc/snmp/snmpd.conf #snmp配置文件
/etc/snmp/snmptrapd.conf
/etc/sysconfig/snmpd
/etc/sysconfig/snmptrapd
/usr/bin/net-snmp-create-v3-user
/usr/bin/snmpconf
/usr/sbin/snmpd
/usr/sbin/snmptrapd
/usr/share/doc/net-snmp-5.5
/usr/share/doc/net-snmp-5.5/AGENT.txt
/usr/share/doc/net-snmp-5.5/COPYING
/usr/share/doc/net-snmp-5.5/ChangeLog.trimmed
/usr/share/doc/net-snmp-5.5/EXAMPLE.conf
/usr/share/doc/net-snmp-5.5/FAQ
/usr/share/doc/net-snmp-5.5/NEWS
/usr/share/doc/net-snmp-5.5/PORTING
/usr/share/doc/net-snmp-5.5/README
/usr/share/doc/net-snmp-5.5/README.agent-mibs
/usr/share/doc/net-snmp-5.5/README.agentx
/usr/share/doc/net-snmp-5.5/README.krb5
/usr/share/doc/net-snmp-5.5/README.mib2c
/usr/share/doc/net-snmp-5.5/README.snmpv3
/usr/share/doc/net-snmp-5.5/README.thread
/usr/share/doc/net-snmp-5.5/TODO
/usr/share/doc/net-snmp-5.5/ipf-mod.pl
/usr/share/doc/net-snmp-5.5/passtest
/usr/share/man/man1/net-snmp-create-v3-user.1.gz #帮助文件
/usr/share/man/man1/snmpconf.1.gz
/usr/share/man/man5/snmp_config.5.gz
/usr/share/man/man5/snmpd.conf.5.gz
/usr/share/man/man5/snmpd.examples.5.gz
/usr/share/man/man5/snmpd.internal.5.gz
/usr/share/man/man5/snmptrapd.conf.5.gz
/usr/share/man/man5/variables.5.gz
/usr/share/man/man8/snmpd.8.gz
/usr/share/man/man8/snmptrapd.8.gz
/usr/share/snmp
/usr/share/snmp/snmpconf-data
/usr/share/snmp/snmpconf-data/snmp-data
/usr/share/snmp/snmpconf-data/snmp-data/authopts
/usr/share/snmp/snmpconf-data/snmp-data/debugging
/usr/share/snmp/snmpconf-data/snmp-data/mibs
/usr/share/snmp/snmpconf-data/snmp-data/output
/usr/share/snmp/snmpconf-data/snmp-data/snmpconf-config
/usr/share/snmp/snmpconf-data/snmpd-data
/usr/share/snmp/snmpconf-data/snmpd-data/acl
/usr/share/snmp/snmpconf-data/snmpd-data/basic_setup
/usr/share/snmp/snmpconf-data/snmpd-data/extending
/usr/share/snmp/snmpconf-data/snmpd-data/monitor
/usr/share/snmp/snmpconf-data/snmpd-data/operation
/usr/share/snmp/snmpconf-data/snmpd-data/snmpconf-config
/usr/share/snmp/snmpconf-data/snmpd-data/system
/usr/share/snmp/snmpconf-data/snmpd-data/trapsinks
/usr/share/snmp/snmpconf-data/snmptrapd-data
/usr/share/snmp/snmpconf-data/snmptrapd-data/authentication
/usr/share/snmp/snmpconf-data/snmptrapd-data/formatting
/usr/share/snmp/snmpconf-data/snmptrapd-data/logging
/usr/share/snmp/snmpconf-data/snmptrapd-data/runtime
/usr/share/snmp/snmpconf-data/snmptrapd-data/snmpconf-config
/usr/share/snmp/snmpconf-data/snmptrapd-data/traphandle
/var/run/net-snmp
```

下面是snmp的常用操作工具，

```javascript
[root@node1 ~]# rpm -ql net-snmp-utils-5.5-44.el6_4.4.x86_64
/usr/bin/encode_keychange
/usr/bin/snmpbulkget #下面是snmp常用工具，下面我们会详细讲解
/usr/bin/snmpbulkwalk
/usr/bin/snmpdelta
/usr/bin/snmpdf
/usr/bin/snmpget
/usr/bin/snmpgetnext
/usr/bin/snmpinform
/usr/bin/snmpnetstat
/usr/bin/snmpset
/usr/bin/snmpstatus
/usr/bin/snmptable
/usr/bin/snmptest
/usr/bin/snmptranslate
/usr/bin/snmptrap
/usr/bin/snmpusm
/usr/bin/snmpvacm
/usr/bin/snmpwalk
/usr/share/man/man1/encode_keychange.1.gz
/usr/share/man/man1/snmpbulkget.1.gz
/usr/share/man/man1/snmpbulkwalk.1.gz
/usr/share/man/man1/snmpcmd.1.gz
/usr/share/man/man1/snmpconf.1.gz
/usr/share/man/man1/snmpdelta.1.gz
/usr/share/man/man1/snmpdf.1.gz
/usr/share/man/man1/snmpget.1.gz
/usr/share/man/man1/snmpgetnext.1.gz
/usr/share/man/man1/snmpinform.1.gz
/usr/share/man/man1/snmpnetstat.1.gz
/usr/share/man/man1/snmpset.1.gz
/usr/share/man/man1/snmpstatus.1.gz
/usr/share/man/man1/snmptable.1.gz
/usr/share/man/man1/snmptest.1.gz
/usr/share/man/man1/snmptranslate.1.gz
/usr/share/man/man1/snmptrap.1.gz
/usr/share/man/man1/snmpusm.1.gz
/usr/share/man/man1/snmpvacm.1.gz
/usr/share/man/man1/snmpwalk.1.gz
/usr/share/man/man5/snmp.conf.5.gz
/usr/share/man/man5/variables.5.gz
```

#### 10.查看一下默认配置文件

注，大家可以看到默认配置文件有很多内容，但其实大部分都是注释。下面我们来讲解一下常用的配置选项，

```javascript
[root@node1 ~]# cat /etc/snmp/snmpd.conf
###############################################################################
#
# snmpd.conf:
#  An example configuration file for configuring the ucd-snmp snmpd agent.
#
###############################################################################
#
# This file is intended to only be as a starting point. Many more
# configuration directives exist than are mentioned in this file. For
# full details, see the snmpd.conf(5) manual page.
#
# All lines beginning with a '#' are comments and are intended for you
# to read. All other lines are configuration commands for the agent.
###############################################################################
# Access Control
###############################################################################
# As shipped, the snmpd demon will only respond to queries on the
# system mib group until this file is replaced or modified for
# security purposes. Examples are shown below about how to increase the
# level of access.
# By far, the most common question I get about the agent is "why won't
# it work?", when really it should be "how do I configure the agent to
# allow me to access it?"
#
# By default, the agent responds to the "public" community for read
# only access, if run out of the box without any configuration file in
# place. The following examples show you other ways of configuring
# the agent so that you can change the community names, and give
# yourself write access to the mib tree as well.
#
# For more information, read the FAQ as well as the snmpd.conf(5)
# manual page.
####
# First, map the community name "public" into a "security name"
#    sec.name source     community
com2sec notConfigUser default    public
####
# Second, map the security name into a group name:
#    groupName   securityModel securityName
group  notConfigGroup v1      notConfigUser
group  notConfigGroup v2c      notConfigUser
####
# Third, create a view for us to let the group have rights to:
# Make at least snmpwalk -v 1 localhost -c public system fast again.
#    name      incl/excl   subtree     mask(optional)
view  systemview  included  .1.3.6.1.2.1.1
view  systemview  included  .1.3.6.1.2.1.25.1.1
####
# Finally, grant the group read-only access to the systemview view.
#    group     context sec.model sec.level prefix read  write notif
access notConfigGroup ""   any    noauth  exact systemview none none
# -----------------------------------------------------------------------------
#下面的是一个例子，具体是关于怎么对本机或其他网络主机赋予权限
# Here is a commented out example configuration that allows less
# restrictive access.
# YOU SHOULD CHANGE THE "COMMUNITY" TOKEN BELOW TO A NEW KEYWORD ONLY
# KNOWN AT YOUR SITE. YOU *MUST* CHANGE THE NETWORK TOKEN BELOW TO
# SOMETHING REFLECTING YOUR LOCAL NETWORK ADDRESS SPACE.
##    sec.name source     community
#com2sec local   localhost    COMMUNITY
#com2sec mynetwork NETWORK/24   COMMUNITY
##   group.name sec.model sec.name
#group MyRWGroup any    local
#group MyROGroup any    mynetwork
#
#group MyRWGroup any    otherv3user
#...
##      incl/excl subtree             mask
#view all  included .1                80
## -or just the mib2 tree-
#view mib2  included .iso.org.dod.internet.mgmt.mib-2 fc
##        context sec.model sec.level prefix read  write notif
#access MyROGroup ""   any    noauth  0   all  none  none
#access MyRWGroup ""   any    noauth  0   all  all  all
###############################################################################
# Sample configuration to make net-snmpd RFC 1213.
# Unfortunately v1 and v2c don't allow any user based authentification, so
# opening up the default config is not an option from a security point.
#
# WARNING: If you uncomment the following lines you allow write access to your
# snmpd daemon from any source! To avoid this use different names for your
# community or split out the write access to a different community and
# restrict it to your local network.
# Also remember to comment the syslocation and syscontact parameters later as
# otherwise they are still read only (see FAQ for net-snmp).
#
# First, map the community name "public" into a "security name"
#    sec.name    source     community
#com2sec notConfigUser  default     public
# Second, map the security name into a group name:
#    groupName    securityModel  securityName
#group  notConfigGroup v1       notConfigUser
#group  notConfigGroup v2c       notConfigUser
# Third, create a view for us to let the group have rights to:
# Open up the whole tree for ro, make the RFC 1213 required ones rw.
#    name      incl/excl    subtree mask(optional)
#view  roview     included    .1
#view  rwview     included    system.sysContact
#view  rwview     included    system.sysName
#view  rwview     included    system.sysLocation
#view  rwview     included    interfaces.ifTable.ifEntry.ifAdminStatus
#view  rwview     included    at.atTable.atEntry.atPhysAddress
#view  rwview     included    at.atTable.atEntry.atNetAddress
#view  rwview     included    ip.ipForwarding
#view  rwview     included    ip.ipDefaultTTL
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteDest
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteIfIndex
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteMetric1
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteMetric2
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteMetric3
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteMetric4
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteType
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteAge
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteMask
#view  rwview     included    ip.ipRouteTable.ipRouteEntry.ipRouteMetric5
#view  rwview     included    ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaIfIndex
#view  rwview     included    ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaPhysAddress
#view  rwview     included    ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaNetAddress
#view  rwview     included    ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaType
#view  rwview     included    tcp.tcpConnTable.tcpConnEntry.tcpConnState
#view  rwview     included    egp.egpNeighTable.egpNeighEntry.egpNeighEventTrigger
#view  rwview     included    snmp.snmpEnableAuthenTraps
# Finally, grant the group read-only access to the systemview view.
#    group     context sec.model sec.level prefix read  write notif
#access notConfigGroup ""   any    noauth  exact roview rwview none
###############################################################################
# System contact information
#联系人信息
# It is also possible to set the sysContact and sysLocation system
# variables through the snmpd.conf file:
syslocation Unknown (edit /etc/snmp/snmpd.conf)
syscontact Root <root@localhost> (configure /etc/snmp/snmp.local.conf)
# Example output of snmpwalk:
#  % snmpwalk -v 1 localhost -c public system
#  system.sysDescr.0 = "SunOS name sun4c"
#  system.sysObjectID.0 = OID: enterprises.ucdavis.ucdSnmpAgent.sunos4
#  system.sysUpTime.0 = Timeticks: (595637548) 68 days, 22:32:55
#  system.sysContact.0 = "Me <me@somewhere.org>"
#  system.sysName.0 = "name"
#  system.sysLocation.0 = "Right here, right now."
#  system.sysServices.0 = 72
###############################################################################
# Logging
#日志信息
# We do not want annoying "Connection from UDP: " messages in syslog.
# If the following option is commented out, snmpd will print each incoming
# connection, which can be useful for debugging.
dontLogTCPWrappersConnects yes
# -----------------------------------------------------------------------------
###############################################################################
# Process checks.
#关于检测进程数目的有个OIDs，可以在enterprises.ucdavis.procTable查看
# The following are examples of how to use the agent to check for
# processes running on the host. The syntax looks something like:
#
# proc NAME [MAX=0] [MIN=0]
#
# NAME: the name of the process to check for. It must match
#     exactly (ie, http will not find httpd processes).
# MAX:  the maximum number allowed to be running. Defaults to 0.
# MIN:  the minimum number to be running. Defaults to 0.
#
# Examples (commented out by default):
#
# Make sure mountd is running
#proc mountd
# Make sure there are no more than 4 ntalkds running, but 0 is ok too.
#proc ntalkd 4
# Make sure at least one sendmail, but less than or equal to 10 are running.
#proc sendmail 10 1
# A snmpwalk of the process mib tree would look something like this:
#
# % snmpwalk -v 1 localhost -c public .1.3.6.1.4.1.2021.2
# enterprises.ucdavis.procTable.prEntry.prIndex.1 = 1
# enterprises.ucdavis.procTable.prEntry.prIndex.2 = 2
# enterprises.ucdavis.procTable.prEntry.prIndex.3 = 3
# enterprises.ucdavis.procTable.prEntry.prNames.1 = "mountd"
# enterprises.ucdavis.procTable.prEntry.prNames.2 = "ntalkd"
# enterprises.ucdavis.procTable.prEntry.prNames.3 = "sendmail"
# enterprises.ucdavis.procTable.prEntry.prMin.1 = 0
# enterprises.ucdavis.procTable.prEntry.prMin.2 = 0
# enterprises.ucdavis.procTable.prEntry.prMin.3 = 1
# enterprises.ucdavis.procTable.prEntry.prMax.1 = 0
# enterprises.ucdavis.procTable.prEntry.prMax.2 = 4
# enterprises.ucdavis.procTable.prEntry.prMax.3 = 10
# enterprises.ucdavis.procTable.prEntry.prCount.1 = 0
# enterprises.ucdavis.procTable.prEntry.prCount.2 = 0
# enterprises.ucdavis.procTable.prEntry.prCount.3 = 1
# enterprises.ucdavis.procTable.prEntry.prErrorFlag.1 = 1
# enterprises.ucdavis.procTable.prEntry.prErrorFlag.2 = 0
# enterprises.ucdavis.procTable.prEntry.prErrorFlag.3 = 0
# enterprises.ucdavis.procTable.prEntry.prErrMessage.1 = "No mountd process running."
# enterprises.ucdavis.procTable.prEntry.prErrMessage.2 = ""
# enterprises.ucdavis.procTable.prEntry.prErrMessage.3 = ""
# enterprises.ucdavis.procTable.prEntry.prErrFix.1 = 0
# enterprises.ucdavis.procTable.prEntry.prErrFix.2 = 0
# enterprises.ucdavis.procTable.prEntry.prErrFix.3 = 0
#
# Note that the errorFlag for mountd is set to 1 because one is not
# running (in this case an rpc.mountd is, but thats not good enough),
# and the ErrMessage tells you what's wrong. The configuration
# imposed in the snmpd.conf file is also shown.
#
# Special Case: When the min and max numbers are both 0, it assumes
# you want a max of infinity and a min of 1.
#
# -----------------------------------------------------------------------------
###############################################################################
# Executables/scripts
#snmpd可以运行一个脚本，并可以得到他的返回值（最后一个）在enterprises.ucdavis.extTable下查看
#
# You can also have programs run by the agent that return a single
# line of output and an exit code. Here are two examples.
#
# exec NAME PROGRAM [ARGS ...]
#
# NAME:   A generic name. The name must be unique for each exec statement.
# PROGRAM: The program to run. Include the path!
# ARGS:   optional arguments to be passed to the program
# a simple hello world
#exec echotest /bin/echo hello world
# Run a shell script containing:
#
# #!/bin/sh
# echo hello world
# echo hi there
# exit 35
#
# Note: this has been specifically commented out to prevent
# accidental security holes due to someone else on your system writing
# a /tmp/shtest before you do. Uncomment to use it.
#
#exec shelltest /bin/sh /tmp/shtest
# Then,
# % snmpwalk -v 1 localhost -c public .1.3.6.1.4.1.2021.8
# enterprises.ucdavis.extTable.extEntry.extIndex.1 = 1
# enterprises.ucdavis.extTable.extEntry.extIndex.2 = 2
# enterprises.ucdavis.extTable.extEntry.extNames.1 = "echotest"
# enterprises.ucdavis.extTable.extEntry.extNames.2 = "shelltest"
# enterprises.ucdavis.extTable.extEntry.extCommand.1 = "/bin/echo hello world"
# enterprises.ucdavis.extTable.extEntry.extCommand.2 = "/bin/sh /tmp/shtest"
# enterprises.ucdavis.extTable.extEntry.extResult.1 = 0
# enterprises.ucdavis.extTable.extEntry.extResult.2 = 35
# enterprises.ucdavis.extTable.extEntry.extOutput.1 = "hello world."
# enterprises.ucdavis.extTable.extEntry.extOutput.2 = "hello world."
# enterprises.ucdavis.extTable.extEntry.extErrFix.1 = 0
# enterprises.ucdavis.extTable.extEntry.extErrFix.2 = 0
# Note that the second line of the /tmp/shtest shell script is cut
# off. Also note that the exit status of 35 was returned.
# -----------------------------------------------------------------------------
###############################################################################
# disk checks
#硬盘分区的定义，语法如下，只有定义了才会取抓值。单位KB
# The agent can check the amount of available disk space, and make
# sure it is above a set limit.
# disk PATH [MIN=100000]
#
# PATH: mount path to the disk in question.
# MIN:  Disks with space below this value will have the Mib's errorFlag set.
#    Default value = 100000.
# Check the / partition and make sure it contains at least 10 megs.
#disk / 10000
# % snmpwalk -v 1 localhost -c public .1.3.6.1.4.1.2021.9
# enterprises.ucdavis.diskTable.dskEntry.diskIndex.1 = 0
# enterprises.ucdavis.diskTable.dskEntry.diskPath.1 = "/" Hex: 2F
# enterprises.ucdavis.diskTable.dskEntry.diskDevice.1 = "/dev/dsk/c201d6s0"
# enterprises.ucdavis.diskTable.dskEntry.diskMinimum.1 = 10000
# enterprises.ucdavis.diskTable.dskEntry.diskTotal.1 = 837130
# enterprises.ucdavis.diskTable.dskEntry.diskAvail.1 = 316325
# enterprises.ucdavis.diskTable.dskEntry.diskUsed.1 = 437092
# enterprises.ucdavis.diskTable.dskEntry.diskPercent.1 = 58
# enterprises.ucdavis.diskTable.dskEntry.diskErrorFlag.1 = 0
# enterprises.ucdavis.diskTable.dskEntry.diskErrorMsg.1 = ""
# -----------------------------------------------------------------------------
###############################################################################
# load average checks
#这个是定义cpu任务的负载信息的，值是平均最大值
# load [1MAX=12.0] [5MAX=12.0] [15MAX=12.0]
#
# 1MAX:  If the 1 minute load average is above this limit at query
#     time, the errorFlag will be set.
# 5MAX:  Similar, but for 5 min average.
# 15MAX: Similar, but for 15 min average.
# Check for loads:
#load 12 14 14
# % snmpwalk -v 1 localhost -c public .1.3.6.1.4.1.2021.10
# enterprises.ucdavis.loadTable.laEntry.loadaveIndex.1 = 1
# enterprises.ucdavis.loadTable.laEntry.loadaveIndex.2 = 2
# enterprises.ucdavis.loadTable.laEntry.loadaveIndex.3 = 3
# enterprises.ucdavis.loadTable.laEntry.loadaveNames.1 = "Load-1"
# enterprises.ucdavis.loadTable.laEntry.loadaveNames.2 = "Load-5"
# enterprises.ucdavis.loadTable.laEntry.loadaveNames.3 = "Load-15"
# enterprises.ucdavis.loadTable.laEntry.loadaveLoad.1 = "0.49" Hex: 30 2E 34 39
# enterprises.ucdavis.loadTable.laEntry.loadaveLoad.2 = "0.31" Hex: 30 2E 33 31
# enterprises.ucdavis.loadTable.laEntry.loadaveLoad.3 = "0.26" Hex: 30 2E 32 36
# enterprises.ucdavis.loadTable.laEntry.loadaveConfig.1 = "12.00"
# enterprises.ucdavis.loadTable.laEntry.loadaveConfig.2 = "14.00"
# enterprises.ucdavis.loadTable.laEntry.loadaveConfig.3 = "14.00"
# enterprises.ucdavis.loadTable.laEntry.loadaveErrorFlag.1 = 0
# enterprises.ucdavis.loadTable.laEntry.loadaveErrorFlag.2 = 0
# enterprises.ucdavis.loadTable.laEntry.loadaveErrorFlag.3 = 0
# enterprises.ucdavis.loadTable.laEntry.loadaveErrMessage.1 = ""
# enterprises.ucdavis.loadTable.laEntry.loadaveErrMessage.2 = ""
# enterprises.ucdavis.loadTable.laEntry.loadaveErrMessage.3 = ""
# -----------------------------------------------------------------------------
###############################################################################
# Extensible sections.
# 与enterprises.ucdavis.extTable的区别
# This alleviates the multiple line output problem found in the
# previous executable mib by placing each mib in its own mib table:
# Run a shell script containing:
#
# #!/bin/sh
# echo hello world
# echo hi there
# exit 35
#
# Note: this has been specifically commented out to prevent
# accidental security holes due to someone else on your system writing
# a /tmp/shtest before you do. Uncomment to use it.
#
# exec .1.3.6.1.4.1.2021.50 shelltest /bin/sh /tmp/shtest
# % snmpwalk -v 1 localhost -c public .1.3.6.1.4.1.2021.50
# enterprises.ucdavis.50.1.1 = 1
# enterprises.ucdavis.50.2.1 = "shelltest"
# enterprises.ucdavis.50.3.1 = "/bin/sh /tmp/shtest"
# enterprises.ucdavis.50.100.1 = 35
# enterprises.ucdavis.50.101.1 = "hello world."
# enterprises.ucdavis.50.101.2 = "hi there."
# enterprises.ucdavis.50.102.1 = 0
# Now the Output has grown to two lines, and we can see the 'hi
# there.' output as the second line from our shell script.
#
# Note that you must alter the mib.txt file to be correct if you want
# the .50.* outputs above to change to reasonable text descriptions.
# Other ideas:
#
# exec .1.3.6.1.4.1.2021.51 ps /bin/ps
# exec .1.3.6.1.4.1.2021.52 top /usr/local/bin/top
# exec .1.3.6.1.4.1.2021.53 mailq /usr/bin/mailq
#可以运行多个脚本
# -----------------------------------------------------------------------------
###############################################################################
# Pass through control.
#
# Usage:
#  pass MIBOID EXEC-COMMAND
#
# This will pass total control of the mib underneath the MIBOID
# portion of the mib to the EXEC-COMMAND.
#
# Note: You'll have to change the path of the passtest script to your
# source directory or install it in the given location.
#
# Example: (see the script for details)
#      (commented out here since it requires that you place the
#      script in the right location. (its not installed by default))
# pass .1.3.6.1.4.1.2021.255 /bin/sh /usr/local/local/passtest
# % snmpwalk -v 1 localhost -c public .1.3.6.1.4.1.2021.255
# enterprises.ucdavis.255.1 = "life the universe and everything"
# enterprises.ucdavis.255.2.1 = 42
# enterprises.ucdavis.255.2.2 = OID: 42.42.42
# enterprises.ucdavis.255.3 = Timeticks: (363136200) 42 days, 0:42:42
# enterprises.ucdavis.255.4 = IpAddress: 127.0.0.1
# enterprises.ucdavis.255.5 = 42
# enterprises.ucdavis.255.6 = Gauge: 42
#
# % snmpget -v 1 localhost public .1.3.6.1.4.1.2021.255.5
# enterprises.ucdavis.255.5 = 42
#
# % snmpset -v 1 localhost public .1.3.6.1.4.1.2021.255.1 s "New string"
# enterprises.ucdavis.255.1 = "New string"
#
# For specific usage information, see the man/snmpd.conf.5 manual page
# as well as the local/passtest script used in the above example.
###############################################################################
# Further Information
#
# See the snmpd.conf manual page, and the output of "snmpd -H".
```

下面是常用的配置选项，

主配置文件：/etc/snmp/snmpd.conf

常用定义项：

(1).首选是定义一个共同体名(community)，这里是public（多数SNMP设备或服务默认都是是用public），及可以访问这个public的用户名（sec  name），这里是notConfigUser。Public相当于用户notConfigUser的密码：） ,source  在net-snmp中用来对来源IP加以控制,即哪些可以获取SNMP信息

   

   



```javascript
#        sec.name  source community
com2sec  notConfigUser  default  public
```

(2).定义一个组名（groupName）这里是notConfigGroup，及组的安全级别，把notConfigGroup这个用户加到这个组中。

sec.model：安全模式，可选值为v1/v2c/usm。

   

   



```javascript
#     groupName  securityModel  securityName
group notConfigGroup  v1  notConfigUser
group notConfigGroup  v2c  notConfigUser
```

(3).定义一个可操作的范围(view)名， 这里是all，范围是 .1 

   

   



```javascript
#     name incl/excl  subtree mask(optional)
view  all included  .1 #表示可以查看.1节点下的所有设备信息
```

注： 

-  incl/excl：对下面的MIB子树是包括还是排除。 
-  subtree：视图中涉及的MIB子树。 
-  mask：掩码  

(4).定义notConfigUser这个组在all这个view范围内可做的操作(即权限），这时定义了notConfigUser组的成员可对.1这个范围做只读操作。 

   

   



```javascript
#      group  context  sec.model  sec.level  prefix  read  write  notif
access notConfigGroup  ""  any  noauth  exact  all none  none
```

注：

-  context：上下文，v1、v2c中始终为空。 
-  sec.model：安全模式，可选值为v1/v2c/usm。 
-  sec.level：安全级别，可选值为auth/noauth/priv，v1、v2c中只能为noauth。 
-  prefix：前缀，指定context如何与PDU中的context匹配，v3使用。 
-  read：授权的读视图。 
-  write：授权的写视图。 
-  notif：授权的trap视图。 

(5).端口： 标准的SNMP服务使用161和162端口，厂商私有的实现一般使用199、391、705和1993端口,net-snmp用udp 161。

#### 11.启动snmp

```javascript
[root@node1 ~]# service snmpd start
正在启动 snmpd：                      [确定]
[root@node1 ~]# netstat -ntulp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address        Foreign Address       State    PID/Program name
tcp    0   0 0.0.0.0:22         0.0.0.0:*          LISTEN   1045/sshd
tcp    0   0 127.0.0.1:25        0.0.0.0:*          LISTEN   1122/master
tcp    0   0 127.0.0.1:6010       0.0.0.0:*          LISTEN   2149/sshd
tcp    0   0 127.0.0.1:6011       0.0.0.0:*          LISTEN   1582/sshd
tcp    0   0 127.0.0.1:199        0.0.0.0:*          LISTEN   12252/snmpd
tcp    0   0 :::22            :::*            LISTEN   1045/sshd
tcp    0   0 ::1:25           :::*            LISTEN   1122/master
tcp    0   0 ::1:6010          :::*            LISTEN   2149/sshd
tcp    0   0 ::1:6011          :::*            LISTEN   1582/sshd
udp    0   0 0.0.0.0:161         0.0.0.0:*                12252/snmpd
```

#### 12.测试一下

```javascript
[root@node1 ~]# snmpd -v #查看一下net-snmp版本
NET-SNMP version: 5.5
Web:        http://www.net-snmp.org/
Email:       net-snmp-coders@lists.sourceforge.net
```

注，启动代理时也可以使用如下参数使snmpd使用【配置文件】启动：

```javascript
[root@node1 ~]# snmpd –C –c /etc/snmp/snmpd.conf
-C 不读取默认配置文件
-c 读取指定的配置文件
```

使用这个参数，我们可以配置很多的配置文件，设置不同的运行环境，便于我们开发和学习。

下面来测试一下，

从代理中读取一个管理对象验证代理是否正常运行。读取MIB库中的RFC1213->system->sysDescr 对象，该对象表示设备的描述信息。使用net-snmp提供的snmp工具读取，命令如下：

```javascript
[root@node1 ~]# snmpget -v 2c -c public localhost sysDescr.0
SNMPv2-MIB::sysDescr.0 = STRING: Linux node1.test.com 2.6.32-358.el6.x86_64 #1 SMP Fri Feb 22 00:31:26 UTC 2013 x86_64
```

注，如果得到了信息，表示net-snmp安装成功。

13.net-snmp工具介绍

   

   



```javascript
[root@node1 ~]# snmp
snmpbulkget  snmpd     snmpget    snmpnetstat  snmptable   snmptrap    snmpvacm
snmpbulkwalk  snmpdelta   snmpgetnext  snmpset    snmptest    snmptrapd   snmpwalk
snmpconf    snmpdf     snmpinform   snmpstatus   snmptranslate snmpusm
```

-  snmpget 模拟snmp的GetRequest操作的工具。用来获取一个或几个管理信息。用来读取管理信息的内容。 
-  snmpgetnext 模拟snmp的GetNextRequest操作的工具。用来获取一个管理信息实例的下一个可用实例数据。一般用来遍历SNMP中的表格数据。 
-  snmpset 模拟snmp的SetRequest操作的工具。用来设置可以写的管理信息。一般用来配置设备或对设备执行操作。 
-  snmpbulkget   模拟snmp的GetBulkRequest操作的工具。用来获取大块的数据。一般在大量读取大块数据时使用，以提高带宽利用率，并且比使用snmpget、snmpgetnext及snmpwalk有更强的容错能力，代理会返回尽可能多的数据，比其它命令更有保证。 
-  snmpwalk 利用GetNextRequest对给定的管理树进行遍历的工具。一般用来对表格类型管理信息进行遍历。 
-  snmptrap 模拟trap的工具。用来发送模拟trap。一般用来[测试管理](https://cloud.tencent.com/product/coding?from_column=20065&from=20065)站安装和配置是否正确，或者用来验证开发的Trap接收程序是否可以正常工作。 
-  snmptrapd 接收并显示trap的工具。一般在代理的开发过程中，接收代理发来的Trap，并将PDU细节打印出来，也来测试Trap发送功能是否正常。 
-  snmpinform 模拟发送InformRequest的工具。跟snmptrap类似，用来发送模拟的带应答的Trap，以测试管理站或自己开发的接收程序。 
-  snmptable 使用GetNextRequest 和 GetBulkRequest操作读取表信息，以列表形式显示的工具。 
-  snmpstatus 从SNMP实体读取几个重要的管理信息以确定设备状态的工具。用来简单测定设备状态。 
-  snmpbulkwalk 利用GetBulkRequest实现对给定管理树进行遍历的工具。对表格类型的管理信息进行遍历读取。 
-  snmpdelta 用来监视Integer类型的管理对象，会及时报告值改变情况的工具。用于监测一个设备或开发中的代理。 
-  snmptest 是一个复杂的工具，可以监测和管理一个网络实体的信息，通过SNMP请求操作与管理实体通信。 
-  snmptranslate 将对象名字和标识符相互转换的工具。用于数据格式的对象标识和可读式字符串的数据名称的转换。类似于域名与IP地址的关系。 
-  snmpusm SNMPv3 USM配置工具。用于SNMPv3的用户管理。 
-  snmpvacm 为一个网络实体创建和维护SNMPv3的基于视图访问控制参数的工具。用于维护SNMPv3的视图访问控制。 
-  snmpconf 生成snmpd配置文件的工具。用于生成snmpd的各种配置文件，用作模板，以生成用户级配置文件。 
-  snmpd   Net-snmp开发的主代理程序，包括众多标准MIB的实现。还可以使用子代理对其进行扩展，是一个功能强大的SNMP代理。Linux、Unix和Windows系统运行snmpd后，直接具备了SNMP协议支持，可以被管理站管理。许多商业化的Linux中使用snmpd作为系统的SNMP代理。 
-  snmpdf 通过SNMP访问并显示网络实体磁盘利用情况的工具。用来监测网络实体的磁盘。 

下面我们来说几个最常用的net-snmp工具，

常用命令：

(1).snmpget命令

```javascript
snmpget [OPTIONS] AGENT OID [OID]...
选项：
-v 1|2c|3  指定SNMP版本
-c COMMUNITY  指定community string
-m MIB[:...]  指定MIB文件
```

注，snmpget 不同于 snmpwalk ，必须在命令行给出 <object>.<instance> 的格式，不能只给出 object。 

案例，snmpget -v 2c -c public localhost system.sysDescr.0 #获取设备的描述信息。

   

   



```javascript
[root@node1 ~]# snmpget -v 2c -c public localhost system.sysDescr.0
SNMPv2-MIB::sysDescr.0 = STRING: Linux node1.test.com 2.6.32-358.el6.x86_64 #1 SMP Fri Feb 22 00:31:26 UTC 2013 x86_64
```

(2).snmpwalk/snmpbulkwalk 命令

   

   



```javascript
snmpwalk/snmpbulkwalk [OPTIONS] AGENT [OID]
选项：
-v 1|2c|3  指定SNMP版本
-c COMMUNITY  指定community string
-m MIB[:...]  指定MIB文件
```

注，snmpbulkwalk命令： 获取snmp服务器的mib-2各种资讯，snmpbulkwalk用于通过SNMPv2 的SNMP GET BULK请求命令与其它网络实体通信，只能用于snmpv2,速度快于snmpwalk 

案例，snmpbulkwalk -v 2c -c public 127.0.0.1 .1.3.6.1.2.1.1 #获取mib-2的system的数据

```javascript
[root@node1 ~]# snmpbulkwalk  -v  2c  -c public 127.0.0.1  .1.3.6.1.2.1.1
SNMPv2-MIB::sysDescr.0 = STRING: Linux node1.test.com 2.6.32-358.el6.x86_64 #1 SMP Fri Feb 22 00:31:26 UTC 2013 x86_64
SNMPv2-MIB::sysObjectID.0 = OID: NET-SNMP-MIB::netSnmpAgentOIDs.10
DISMAN-EVENT-MIB::sysUpTimeInstance = Timeticks: (264551) 0:44:05.51
SNMPv2-MIB::sysContact.0 = STRING: Root <root@localhost> (configure /etc/snmp/snmp.local.conf)
SNMPv2-MIB::sysName.0 = STRING: node1.test.com
SNMPv2-MIB::sysLocation.0 = STRING: Unknown (edit /etc/snmp/snmpd.conf)
SNMPv2-MIB::sysORLastChange.0 = Timeticks: (10) 0:00:00.10
SNMPv2-MIB::sysORID.1 = OID: SNMP-MPD-MIB::snmpMPDMIBObjects.3.1.1
SNMPv2-MIB::sysORID.2 = OID: SNMP-USER-BASED-SM-MIB::usmMIBCompliance
SNMPv2-MIB::sysORID.3 = OID: SNMP-FRAMEWORK-MIB::snmpFrameworkMIBCompliance
SNMPv2-MIB::sysORID.4 = OID: SNMPv2-MIB::snmpMIB
SNMPv2-MIB::sysORID.5 = OID: TCP-MIB::tcpMIB
SNMPv2-MIB::sysORID.6 = OID: IP-MIB::ip
SNMPv2-MIB::sysORID.7 = OID: UDP-MIB::udpMIB
SNMPv2-MIB::sysORID.8 = OID: SNMP-VIEW-BASED-ACM-MIB::vacmBasicGroup
SNMPv2-MIB::sysORDescr.1 = STRING: The MIB for Message Processing and Dispatching.
SNMPv2-MIB::sysORDescr.2 = STRING: The MIB for Message Processing and Dispatching.
SNMPv2-MIB::sysORDescr.3 = STRING: The SNMP Management Architecture MIB.
SNMPv2-MIB::sysORDescr.4 = STRING: The MIB module for SNMPv2 entities
SNMPv2-MIB::sysORDescr.5 = STRING: The MIB module for managing TCP implementations
SNMPv2-MIB::sysORDescr.6 = STRING: The MIB module for managing IP and ICMP implementations
SNMPv2-MIB::sysORDescr.7 = STRING: The MIB module for managing UDP implementations
SNMPv2-MIB::sysORDescr.8 = STRING: View-based Access Control Model for SNMP.
SNMPv2-MIB::sysORUpTime.1 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.2 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.3 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.4 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.5 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.6 = Timeticks: (10) 0:00:00.10
SNMPv2-MIB::sysORUpTime.7 = Timeticks: (10) 0:00:00.10
SNMPv2-MIB::sysORUpTime.8 = Timeticks: (10) 0:00:00.10
```

(3).snmpwalk命令

注，获取snmp服务器的HOST-RESOURCES的各种资讯。

案例，snmpwalk -v 2c -c public localhost system #这条指令用于查看本机系统信息

```javascript
[root@node1 ~]# snmpwalk -v 2c -c public localhost system
SNMPv2-MIB::sysDescr.0 = STRING: Linux node1.test.com 2.6.32-358.el6.x86_64 #1 SMP Fri Feb 22 00:31:26 UTC 2013 x86_64
SNMPv2-MIB::sysObjectID.0 = OID: NET-SNMP-MIB::netSnmpAgentOIDs.10
DISMAN-EVENT-MIB::sysUpTimeInstance = Timeticks: (3335125) 9:15:51.25
SNMPv2-MIB::sysContact.0 = STRING: Root <root@localhost> (configure /etc/snmp/snmp.local.conf)
SNMPv2-MIB::sysName.0 = STRING: node1.test.com
SNMPv2-MIB::sysLocation.0 = STRING: Unknown (edit /etc/snmp/snmpd.conf)
SNMPv2-MIB::sysORLastChange.0 = Timeticks: (10) 0:00:00.10
SNMPv2-MIB::sysORID.1 = OID: SNMP-MPD-MIB::snmpMPDMIBObjects.3.1.1
SNMPv2-MIB::sysORID.2 = OID: SNMP-USER-BASED-SM-MIB::usmMIBCompliance
SNMPv2-MIB::sysORID.3 = OID: SNMP-FRAMEWORK-MIB::snmpFrameworkMIBCompliance
SNMPv2-MIB::sysORID.4 = OID: SNMPv2-MIB::snmpMIB
SNMPv2-MIB::sysORID.5 = OID: TCP-MIB::tcpMIB
SNMPv2-MIB::sysORID.6 = OID: IP-MIB::ip
SNMPv2-MIB::sysORID.7 = OID: UDP-MIB::udpMIB
SNMPv2-MIB::sysORID.8 = OID: SNMP-VIEW-BASED-ACM-MIB::vacmBasicGroup
SNMPv2-MIB::sysORDescr.1 = STRING: The MIB for Message Processing and Dispatching.
SNMPv2-MIB::sysORDescr.2 = STRING: The MIB for Message Processing and Dispatching.
SNMPv2-MIB::sysORDescr.3 = STRING: The SNMP Management Architecture MIB.
SNMPv2-MIB::sysORDescr.4 = STRING: The MIB module for SNMPv2 entities
SNMPv2-MIB::sysORDescr.5 = STRING: The MIB module for managing TCP implementations
SNMPv2-MIB::sysORDescr.6 = STRING: The MIB module for managing IP and ICMP implementations
SNMPv2-MIB::sysORDescr.7 = STRING: The MIB module for managing UDP implementations
SNMPv2-MIB::sysORDescr.8 = STRING: View-based Access Control Model for SNMP.
SNMPv2-MIB::sysORUpTime.1 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.2 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.3 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.4 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.5 = Timeticks: (9) 0:00:00.09
SNMPv2-MIB::sysORUpTime.6 = Timeticks: (10) 0:00:00.10
SNMPv2-MIB::sysORUpTime.7 = Timeticks: (10) 0:00:00.10
SNMPv2-MIB::sysORUpTime.8 = Timeticks: (10) 0:00:00.10
```

(4).snmpdelta命令

注，监控网卡流量等信息， 指定共同体为public，协议为snmpv1，以表格的方式输出结果,显示时间戳。

案例，



```javascript
snmpdelta -c public -v 1 -Cs -CT 127.0.0.1 IF-MIB::ifInUcastPkts.2 IF-MIB::ifOutUcastPkts.2 #监控IF-MIB::ifInUcastPkts(输入流量)，IF-MIB::ifOutUcastPkts(输出流量)，如监控本地的第二块网卡instance为2。
snmpdelta -c public -v 1 -Cs -CT 127.0.0.1 ifInDiscards.2 #监控ifInDiscards(已经丢弃的收到的数据包的总数)，格式同上。
```

(5).snmpnetstat命令

注，查看snmp服务器的端口连接信息， 协议为snmp 2c。

案例，



```javascript
snmpnetstat -v 2c -c public -Can -Cp tcp 127.0.0.1  #获取127.0.0.1的TCP协议的开放端口状态
```

(6).snmptranslate 命令

注，在MIB OID 在数字和文字名称之间进行转换。

例子，

-  snmptranslate  -On -IR HOST-RESOURCES-MIB::hrSystem #用snmptranslate把symbol  格式的HOST-RESOURCES-MIB::hrSystem 转换为 numberic 格式的oid,用-On(输出数字格式的oid),使用  -IR(允许“随机”访问 MIB)   [root@node1 ~]# snmptranslate -On -IR HOST-RESOURCES-MIB::hrSystem .1.3.6.1.2.1.25.1     
-  snmptranslate  -Onf -IR HOST-RESOURCES-MIB::hrSystem #用snmptranslate把symbol  格式的HOST-RESOURCES-MIB::hrSystem 转换为symbol 格式的 oid,用-Onf(输出符号格式的oid) [root@node1 ~]# snmptranslate -Onf -IR HOST-RESOURCES-MIB::hrSystem .iso.org.dod.internet.mgmt.mib-2.host.hrSystem 
-  snmptranslate  -Td iso.org.dod.internet.mgmt.mib-2  #用snmptranslate输出该对象(iso.org.dod.internet.mgmt.mib-2)的详细定义,用-Td(包括文字名称、所属  MIB、类型、状态、读写权限、描述信息，数字格式的 OID)  

>  [root@node1 ~]# snmptranslate -Td iso.org.dod.internet.mgmt.mib-2 SNMPv2-SMI::mib-2 mib-2 OBJECT-TYPE  -- FROM SNMPv2-SMI, RFC1213-MIB ::= { iso(1) org(3) dod(6) internet(1) mgmt(2) 1 }   

-  snmptranslate  -Tp iso.org.dod.internet.mgmt.mib-2  #用snmptranslate打印从指定OID(.1.3.6.1.2.1)开始一直到该OID(.1.3.6.1.2.1)所属的 MIB  子树末端的树型结构信息 
-  snmptranslate -Ta #用snmptranslate导出所有已经加载的 MIB 的数据，每个对象一行 
-  snmptranslate -Tl #用snmptranslate导出所有已经加载的 MIB 的对象，并且输出完整的 oid 路径 
-  snmptranslate -To #用snmptranslate导出已经加载的所有 MIB 的数字格式的 oid ，不含值和类型，仅仅有 oid 而已 
-  snmptranslate -Ts #用snmptranslate只输出文字格式的 OID 名称而已  

### 十一、SNMP的MIB详解

#### 1.MIB管理信息库

IETF规定的管理信息库MIB（由中定义了可访问的网络设备及其属性，由对象识别符（OID：Object   Identifier）唯一指定。MIB是一个树形结构，SNMP协议消息通过遍历MIB树形目录中的节点来访问网络中的设备。下图给出了NMS系统中SNMP可访问网络设备的对象识别树（OID：Object  Identifier）结构。 

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/e5y0uq2wpb.gif)

上图中的节点是从internet开始的，其实上面还有几个节点请看下图。

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/dapjsclp9z.png)

下图给出了对一个DS1线路状态进行查询的OID设置例子。 

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/opacfb0lp5.gif)

#### 2.SNMP消息类型

SNMP中定义了五种消息类型：Get-Request、Get-Response、Get-Next-Request、Set-Request、Trap

-  Get-Request 、Get-Next-Request与Get-Response  SNMP管理站用Get-Request消息从拥有SNMP代理的网络设备中检索信息，而SNMP代理则用Get-Response消息响应。Get-  Next-Request用于和Get-Request组合起来查询特定的表对象中的列元素。如：首先通过下面的原语获得所要查询的设备的接口数： {iso org(3) dod(6) internet(1) mgmt(2) mib(1) interfaces(2) ifNumber(2)} 后再通过下面的原语，进行查询（其中第一次用Get-Request，其后用Get-Next-Request）： {iso org(3) dod(6) internet(1) mgmt(2) mib(1) interfaces(2) ifTable(2)} 
-  Set-Request SNMP管理站用Set-Request 可以对网络设备进行远程配置（包括设备名、设备属性、删除设备或使某一个设备属性有效/无效等）。 
-  Trap  SNMP代理使用Trap向SNMP管理站发送非请求消息，一般用于描述某一事件的发生。 

#### 3.SNMP管理信息库MIB 

管理信息库MIB指明了网络元素所维持的变量（即能够被管理进程查询和设置的信息）。MIB给出了一个网络中所有可能的被管理对象的集合的数据结构。SNMP的管理信息库采用和域名系统DNS相似的树型结构，它的根在最上面，根没有名字。下图画的是管理信息库的一部分，它又称为对象命名（objectnamingtree）。  

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/6oot5bbbum.gif)

管理信息库的对象命名举例，对象命名树的顶级对象有三个，即ISO、ITU-T和这两个组织的联合体。在ISO的下面有4个结点，其中的一个（标号3）是被标识的组织。在其下面有一个美国国防部（Department  of  Defense）的子树（标号是6），再下面就是Internet（标号是1）。在只讨论Internet中的对象时，可只画出Internet以下的子树（图中带阴影的虚线方框），并在Internet结点旁边标注上{1.3.6.1}即可。在Internet结点下面的第二个结点是mgmt（管理），标号是2。再下面是管理信息库，原先的结点名是mib。1991年定义了新的版本MIB-  II，故结点名现改为mib-2，其标识为{1.3.6.1.2.1}，或{Internet(1) .2.1}。这种标识为对象标识符。最初的结点mib将其所管理的信息分为8个类别，见下图，现在mib-2所包含的信息类别已超过40个。

![img](https://ask.qcloudimg.com/http-save/yehe-1729674/ev4b4k78of.png)

应当指出，MIB的定义与具体的网络管理协议无关，这对于厂商和用户都有利。厂商可以在产品（如路由器）中包含SNMP代理软件，并保证在定义新的MIB项目后该软件仍遵守标准。用户可以使用同一网络管理客户软件来管理具有不同版本的MIB的多个路由器。当然，一个没有新的MIB项目的路由器不能提供这些项目的信息。

这里要提一下MIB中的对象{1.3.6.1.4.1}，即enterprises（企业），其所属结点数已超过3000。例如IBM为   11.3.6.1.4.1.2}，Cisco为{1.3.6.1.4.1.9}，Novell为{1.3.6.1.4.1.23}等。世界上任何一个公司、学校只要用电子邮件发往iana-mib@isi.edu进行申请即可获得一个结点名。这样各厂家就可以定义自己的产品的被管理对象名，使它能用  SNMP进行管理。

# 3.参考文档3

## 一、SNMP是什么？

1.SNMP介绍

SNMP是简单网络管理协议，其全称为simple network management Protocol。SNMP是一种简单网络管理协议，属于TCP/IP当中的应用层协议。主要用于网络设备的管理。

1. 构成部分
   SNMP协议主要由两大部分构成：SNMP管理站和SNMP代理。SNMP管理站是一个中心节点，负责收集维护各个SNMP元素的信息，并对这些信息进行处理，最后反馈给网络管理员；而SNMP代理是运行在各个被管理的网络节点之上，负责统计该节点的各项信息，并且负责与SNMP管理站交互，接收并执行管理站的命令，上传各种本地的网络信息。
2. 历史版本
   SNMP目前总共有三个版本，分别是V1,V2,V3。
   SNMP v1是协议最初版本，安全性较差
   SNMP v2通常被指是基于community的SNMP V2
   SNMP v3是最新版本的SNMP。该版本最大的贡献为安全性

## 二、SNMP解读

### 1.实现结构

在具体实现上，SNMP为管理员提供了一个网管平台(NMS)，又称为管理站，负责网管命令的发出、数据存储、及数据分析。被监管的设备上运行一个SNMP代理(Agent))，代理实现设备与管理站的SNMP通信。如下图
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/9f79059a88a2604f52d8c5bcde9fa197.png)
管理站与代理端通过MIB进行接口统一，MIB定义了设备中的被管理对象。管理站和代理都实现了相应的MIB对象，使得双方可以识别对方的数据，实现通信。管理站向代理申请MIB中定义的数据，代理识别后，将管理设备提供的相关状态或参数等数据转换为MIB定义的格式，应答给管理站，完成一次管理操作。

### 2.操作命令

SNMP协议提供了三种用于控制MIB对象的基本操作命令，分别为 Get、Set、Trap：
1.Get：管理站读取代理者处的某一对象的值。
2.Set：管理站设置代理者处的某一对象的值。
3.Trap：代理者主动向管理者报告的重要事件，主要包括设备损坏、意外事件等。

### 3.SNMP报文

SNMP协议定义了数据包的格式，网络管理员与管理代理之间的数据交换，还控制这管理代理的MIB数据对象。
一条SNMP消息包括三个部分，分别是“版本标识符”，“团体名”，“数据单元PDU”所构成

- 版本标识符 用于说明使用的SNMP协议版本
- 团体名 信息安全支持，常用public
- 协议数据单元PDU 这是SNMP协议当中的数据区，即SNMP报文中数据的载体。

PDU中的协议数据单元：
SNMP当中规定了5种协议数据单元，用于管理进程和进程之间的交换

- get-request操作: 从代理进程（目标）处获取一个或多个参数值
- get-next-request操作：从代理处获得下一个或多个使用的参数值
- set-request操作：设置代理进程中的一个或多个参数。
- get-response操作 ：代理处收到请求后进行的相应操作，返回一个或多个参数值。
- trap操作：当有错误或故障发生时代理处主动向上一级上报的消息。
  需要注意的是SNMP管理程序端口为162，SNMP代理程序端口为161。
  ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/f87fbfa2e63da9f70db2c5cb154fb972.png)

### 4 SNMP报文格式

如下图所示，SNMP一共由三个部分组成，分别是公共SNMP首部，get/set首部，变量绑定
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/8a69744d67764eef92f89eedfb82f7c2.png)

1. 公共SNMP首部
   共包括三个字段：
   1.版本：写入值为当前版本号减一，如SNMP v1协议则填入0.
   2.共同体： 共同体就是一个字符串，作为管理进程和代理进程之间的明文口令，常用的是6个字符“public”。
   3.pdu类型。根据操作类型，填入0-4中的一个数字，对应关系如下表所示。

| PDU类型 | 操作类型         |
| ------- | ---------------- |
| 0       | get-request      |
| 1       | get-next-request |
| 2       | set-request      |
| 3       | get-response     |
| 4       | trap             |

1. get/set首部
   1.请求标识符：
   这是由管理进程设置的一个整数值。主要用于代理进程识别消息顺序，进行response操作
   2.差错状态：
   由代理进程进行响应时填入0-5中的一个数字。具体内容如下表所示
   ![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/382eb0819f6be421a58e6da1388129af.png)

3.差错索引：当出现noSuchName、badValue或readOnly的差错时，由代理进程在回答时设置的一个整数，它指明有差错的变量在变量列表中的偏移。

| 差错索引 | 名字                  | 说明                                           |
| -------- | --------------------- | ---------------------------------------------- |
| 0        | coldstart             | 代理进行了初始化                               |
| 1        | warmstart             | 代理进行了重新初始化                           |
| 2        | linkDown              | 一个接口从工作状态变为故障状态                 |
| 3        | linkUp                | 一个接口从故障状态变为工作状态                 |
| 4        | authenticationFailure | 从SNMP管理进程接收到具有一个无效共同体的报文   |
| 5        | egpNeighborLoss       | 一个EGP相邻路由器变为故障状态                  |
| 6        | enterpriseSpecific    | 代理自定义的事件，需要用后面的“特定代码”来指明 |

4.特定代码
知名代理自定义的事件（若trap类型为6），否则为0
5.时间戳
指明自代理进程初始化到trap报告的事件发生所经历的时间，单位为10ms

1. 变量绑定

指明一个或多个变量的名和对应的值。
在get或get-next报文中，变量的值应忽略。
PDU (协议数据单元)是SNMP消息中的数据区， 即Snmp通信时报文数据的载体。

## 三、SNMP抓包分析

### 1.SNMP体验

SNMP协议的实现工具和方法有很多，本人使用**snmptuil**工具和一台Windows10平台的电脑进行模拟。
snmputil是一个汉化版的snmp集成工具
snmputil工具的命令规则是：snmputil [get|getnext|walk] agent community oid [oid …]
[get|getnext|walk]：为消息类型 包括get getnext walk
**agent**：指Snmp代理即你想进行操作的网络设备的ip或名称
**community**：分区域，即密码，默认是public
**oid**：想要操作的MIB数据对象号

SNMP获取系统版本：
OID: .1.3.6.1.2.1.1.1.0
实现命令：snmputil get loaclhost public .1.3.6.1.2.1.1.1.0
结果：
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/ba13b8ec9d7f2500f3ec67a8cd0bff34.png)
获取计算机连续开机多长时间：
OID：.1.3.6.1.2.1.1.3.0
实现命令：snmputil get loaclhost public .1.3.6.1.2.1.1.3.0
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/dcb22c18bca40e83fb5eed9ead47c95e.png)

### 2.SNMP抓包分析

抓包软件使用wireshark对SNMP消息进行抓包
以下为SNMP获取系统版本：snmputil get loaclhost public .1.3.6.1.2.1.1.1.0所捕捉到的消息
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/ec52931c094397153f6fc3b5eaaace15.png)
18 00 00 即代表IPv6 Src: localhost, Dst: localhost
00 30 消息长度总共为48
11 Next Header: UDP (17)
80 Hop Limit: 128
00 00 00 00 00 00 00 00 00 00 00 01 Source Address: ::1（localhost）
00 00 00 00 00 00 00 00 00 00 00 01 Destination Address: ::1（localhost）
eb cd Source Port（发送端口）: 60365
00 a1 Destination Port:(目标端口) 161
00 30 消息长度 48
de e4 Checksum: 0xdee4 [unverified]

以下为SNMP报文内容
30 26 02 01 00 04 06 70 75 62 6c 69 63
a0 19 02 01 01 02 01 00 02 01 00 30 0e 30 0C
06 08 2b 06 01 02 01 01 01 00 05 00

30 表示SNMP协议报文
26 消息长度为38个字节
02 01 00 协议版本v1 (前两个字节02表示INTEGER类型01是指1个字节长度）
04 参数类型(OCTSTR)
06 群体(community)名长度
70 75 62 6c 69 63 群体名public的assic码值
a0 PUD类型get-request
19 pdu的长度为25个OctStr(后面的内容31字节)
02 01 01 request ID
02 01 00 表示error-state为0
02 01 00 表示error-index为0
30 0e 表示后面变量绑定是SEQUENCE类型14个字节长度
30 0C 表示变量名1
06 表示该字段是OID类型
08 OID长度8字节
2b 06 01 02 01 01 01 00 1.3.6.1.2.1.1.1.0
05 00 表示NULL

get-response回复消息分析
![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/0a431a1f6d794a2d646593dc5dac0a55.png)
18 00 00 即代表IPv6
60 03 d1 55 flow labell 03 d1 55
00 b8 消息长度184字节
11 Next Header: UDP (17)
80 Hop Limit: 128
00 00 00 00 00 00 00 00 00 00 00 01 Source Address: ::1（localhost）
00 00 00 00 00 00 00 00 00 00 00 01 Destination Address: ::1（localhost）
00 a1 Source Port（发送端口）: 161
eb cd Destination Port:(目标端口) ：60365
00 b8 消息长度184字节
le 63 Checksum: 0x1e63 [unverified]

以下为SNMP报文：
30 表示SNMP协议报文
81 消息长度为129个字节
02 01 00 协议版本v1
04 参数类型(OCTSTR)
06 群体(community)名长度
70 75 62 6c 69 63 群体名public的assic码值
a2 PUD类型get-response
81 pdu的长度为129个OctStr(后面的内容31字节)
9f
02 01 01 request id
02 01 00 表示error-state为0
02 01 00 表示error-index为0
30 81 表示后面变量绑定是SEQUENCE类型129个字节长度
93 30 81 90 表示变量名1
06 表示该字段是OID类型
08 OID长度8字节
2b 06 01 02 01 01 01 00 1.3.6.1.2.1.1.1.0
后续则表示
.Hardware: Intel64 Family 6 Mode
1158 Stepping 10 AT/AT COMPATIBLE - Software: Windows V ersion 6.3 (Build 19044Multipro cessor Free )

## 总结

本文对SNMP的简单构造、消息类型、支持操作进行了简单介绍，然后使用snmputil进行了简单的snmp信息实例，并使用wireshark对消息进行了抓包，最后对SNMP消息进行分析，对SNMP消息进行了拆解，说明了各个部分的功能及作用。