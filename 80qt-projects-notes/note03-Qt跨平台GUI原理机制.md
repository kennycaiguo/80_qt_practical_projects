# 1.QMessageBox的用法

## 函数原型

![image-20260703130614271](./note03-Qt跨平台GUI原理机制.assets/image-20260703130614271.png)

## 函数详解

**一．QMessageBox是什么？**

QMessageBox类为用户提供了主要的警告信息，用户可以根据需求选择需要的响应；

QMessageBox 还提供了一些常用的按钮，例如"确定"、"取消"、"是"、"否"等，并可以根据需要自定义按钮的文本和数量。通过设置不同的标志和参数，还可以控制消息框的样式、图标和默认按钮等。

**二.用法与代码示例**

**1.简单应用**

QMessageBox::information(this, "警告", "请检查网络连接！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

第一个参数是父控件指针

第二个参数是标题

第三个参数是内容

第四个参数是窗口里面要多少个按钮（默认为OK）

第五个参数指定按下Enter时使用的按钮。（默认为NoButton，此时QMessageBox会自动选择合适的默认值。）

**2.判断返回值方法一**

switch(QMessageBox::information(this,"Warning",tr("Save changes to document?"),

QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Save))

{

case QMessageBox::Save:

qDebug() << " Warning button / Save " << endl;;

break;

case QMessageBox::Discard:

qDebug() << " Warning button / Discard " << endl;;

break;

case QMessageBox::Cancel:

qDebug() << " Warning button / Cancel " << endl;;

break;

default:

break;

}



![img](https://pic3.zhimg.com/v2-87201119aca3a003792d4959a856321e_1440w.jpg)



**3.判断返回值方法二及设置图标和logo**

QMessageBox msgBox;

msgBox.setIcon(QMessageBox::Warning);

msgBox.setText("This is a message box with a warning icon.");

msgBox.setWindowTitle("Warning Example");

msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Yes);

msgBox.setWindowIcon(QIcon("./checked2.png")); **//设置title logo**

msgBox.setIconPixmap(QPixmap("./voloff.png"));**//可以设置告警图标**

int ret = msgBox.exec();

qDebug()<<ret; //返回值为宏值 :QMessageBox::Ok|QMessageBox::Yes

![img](https://picx.zhimg.com/v2-3550e58b9838f5257e853010a5a4d17d_1440w.jpg)



**4.自定义button**

QMessageBox BOX;//实例化消息盒对象

BOX.setWindowTitle("退出");//设置消息对话的标题

BOX.setText("你确定要退出吗");//设置消息盒的提示内容

BOX.addButton("确认",QMessageBox::AcceptRole);//自定义按钮

BOX.addButton("取消",QMessageBox::RejectRole);//自定义按钮

int ret=BOX.exec();//显示消息对话框

qDebug()<<ret; //返回值为宏值QMessageBox::AcceptRole

if(ret==0)

{

qDebug() << " QMessageBox::AcceptRole " << endl;;

}

else if(ret==1)

{

qDebug() << " QMessageBox::RejectRole " << endl;;

}

![img](https://pic3.zhimg.com/v2-67bd363f948e2b2ecea96055914cee70_1440w.jpg)



**三．实用技巧**

1.问题：

qt 主页面弹出一个页面A，然后页面A发出命令让主页面弹出一个弹框，但是弹框被挡在了页面A后面，无法操作

解决方法：

QMessageBox::warning(this->pageA,tr("警告"), tr("模块未连接！"));

设置this改为this->pageA，也就是让这个QMessageBox从pageA页面中弹出来。

## QMessageBox使用小技巧

![image-20260703131301169](./note03-Qt跨平台GUI原理机制.assets/image-20260703131301169.png)

# 2.Qt的GUI运行模式以及消息处理

Qt 的 GUI 运行模式是一个典型的**事件驱动模型**，核心在于“等待事件 → 处理事件”的无限循环。它通过主线程的消息循环来接收用户的点击、按键等操作，并将其分发给对应的窗口或控件。 [[1](https://zhuanlan.zhihu.com/p/602654998), [2](https://github.com/0voice/Awesome_Qt_Learning)]

1. GUI 运行模式：事件循环

- **入口函数**：程序的起点通常是 `main()` 函数。调用 `QApplication::exec()` 开启整个程序的事件循环。
- **运行机制**：程序进入“死循环”，等待操作系统传递事件。没有操作时，它会休眠以节省电量；一旦有鼠标点击或键盘输入，它会立刻被唤醒。
- **非阻塞**：因为有事件循环，GUI 程序不会像普通代码那样按顺序卡死在某一行，界面能保持流畅，随时准备响应用户。 [[1](https://zhuanlan.zhihu.com/p/602654998), [2](https://github.com/0voice/Awesome_Qt_Learning)]
- 消息处理模型：事件与分发

Qt 将所有操作（如重绘窗口、鼠标按下、键盘输入等）都包装成“事件（Event）”对象。它的处理过程非常像接力赛： [[1](https://doc.qt.io/qt-6/zh/qtgui-overview.html), [2](https://zhuanlan.zhihu.com/p/602654998)]

- **系统捕获**：操作系统捕捉到你的鼠标点击。
- **翻译事件**：操作系统把点击翻译成 Qt 能听懂的事件，放入 Qt 的**事件队列**。
- **事件分发**：主线程的事件循环（Event Loop）从队列中取出事件，交给目标控件处理。 [[1](https://zhuanlan.zhihu.com/p/602654998), [2](https://github.com/0voice/Awesome_Qt_Learning)]
- 核心处理方式：信号与槽 (Signal & Slot)

Qt 独创了“信号与槽”机制来处理这些消息，这是一种特殊的观察者模式。你可以把它想象成电台广播与收音机： [[1](https://openvela.csdn.net/694c90da5b9f5f31781a656e.html), [2](https://developer.aliyun.com/article/1253067)]

- **信号**：当控件发生状态变化（如按钮被点击），它会发射（Emit）一个信号。
- **槽**：槽就是普通的函数。用来接收信号并执行具体的操作（如弹出一个提示框）。
- **连接**：你必须使用 `connect` 将信号和槽绑定在一起。 [[1](https://openvela.csdn.net/694c90da5b9f5f31781a656e.html), [2](https://developer.aliyun.com/article/1253067)]

**生活中的类比**：
把“按钮被按下”当作发出的**信号**，把“执行开灯操作”当作**槽函数**。一旦连接好，按下按钮这个动作就会自动触发开灯，无需手动调用。 [[1](https://developer.aliyun.com/article/1253067)]

4. 高级消息处理：事件过滤器 (Event Filter)

有时你想要拦截某个控件的消息，让它不按常规方式处理。

- **做法**：你可以给控件安装一个“事件过滤器”。
- **作用**：就像是给大门安装了安检门，所有进来的事件必须先经过你的检查。如果觉得有问题，你可以在中途拦截并修改它，甚至把它吃掉（不让别人处理）。
- 多线程与消息处理

- ***\*主线程负责 GUI\****：所有界面控件的创建和更新，都必须在主线程中进行。
- ***\*子线程负责耗时任务\****：如果在主线程中做耗时计算（如下载大文件），事件循环就会停顿，界面就会“卡死”。因此，耗时操作应该放在子线程中进行。
- ***\*跨线程通信\****：子线程不能直接修改界面，但可以通过“信号与槽”将结果安全地发送给主线程，由主线程更新界面。 [[1](https://zhuanlan.zhihu.com/p/602654998)]

## Qt的GUI参考文档1

## 一、GUI应用程序的概述

**1、现代[操作系统](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=1&q=操作系统&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLmk43kvZzns7vnu58iLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.Kxpqc270GELGJTUWoheyouodKzzhXjcpl_0jMjelkBg&zhida_source=entity)支持GUI界面**

**（1）、现代操作系统提供原生SDK支持GUI程序开发**

**（2）、GUI[程序开发](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=2&q=程序开发&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLnqIvluo_lvIDlj5EiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MiwiemRfdG9rZW4iOm51bGx9.xp00tdgHTAwGqdm5ur36ezsE5YseOuYvUReAw-XNhx4&zhida_source=entity)是现代操作系统上的主流技术**

**（3）、不同操作系统上的GUI开发原理相同**

**（4）、不同操作系统上的GUI SDK不同**

**2、GUI[应用程序开发](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=1&q=应用程序开发&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLlupTnlKjnqIvluo_lvIDlj5EiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.V2_jEfatPUBG56KzFg2e1jjghrz1tGre2QwJXSDJDQc&zhida_source=entity)原理**

**（1）、GUI应用程序在运行时会创建一个[消息队列](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=1&q=消息队列&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLmtojmga_pmJ_liJciLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.CaP9nUOezZAS7w5h4e0rpznCmWxp4POWsFpEczSPwjE&zhida_source=entity)**

**（2）、系统内核将用户的操作翻译成对应的程序消息**

**（3）、程序运行过程中需要实时处理队列中的消息**

**（4）、当队列中没有消息时，程序将处于停滞状态**



![img](https://pic1.zhimg.com/v2-4ed828b642058f222a3fd90e114cf252_1440w.jpg)



（5）、不同的操作系统支持相同的GUI开发原理



![img](https://pic4.zhimg.com/v2-073d19d8c81b2cf74c0ccea0f78f82fb_1440w.jpg)



**3、GUI程序开发的本质**

**1、GUI程序开发**

**（1）、在代码中用程序创建窗口及窗口元素**

**（2）、在[消息处理函数](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=1&q=消息处理函数&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLmtojmga_lpITnkIblh73mlbAiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.8WQXWbdZNyTNjgShffbDqid1YB5g66ruifVoWN71cDc&zhida_source=entity)中根据程序消息作出不同的相应**

**（3）、经典GUI开发模式（[可视化](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=1&q=可视化&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLlj6_op4bljJYiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.2TSp6LEbKBJZanNLs-ksBV6DXgRZlpZSiK0I3Gk0Mgw&zhida_source=entity)界面开发+消息映射）**



![img](https://pic1.zhimg.com/v2-23fe27d1f749504dbddcb4e091cfce40_1440w.jpg)



**4、GUI程序开发实例**

**（1）、多数操作系统以c 函数的方式提供GUI SDK**

**（2）、以Windows操作系统为例**



![img](https://pic1.zhimg.com/v2-de90a7f73012fe81bd87dd80d91a05f6_1440w.jpg)



**[程序分析](https://zhida.zhihu.com/search?content_id=222055721&content_type=Article&match_order=1&q=程序分析&zd_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ6aGlkYV9zZXJ2ZXIiLCJleHAiOjE3ODMyNzg2MzQsInEiOiLnqIvluo_liIbmnpAiLCJ6aGlkYV9zb3VyY2UiOiJlbnRpdHkiLCJjb250ZW50X2lkIjoyMjIwNTU3MjEsImNvbnRlbnRfdHlwZSI6IkFydGljbGUiLCJtYXRjaF9vcmRlciI6MSwiemRfdG9rZW4iOm51bGx9.cC9UNBsZtznDx88oF71j7rB9fI9EUT1LuEFYQNjz0F4&zhida_source=entity)：**



![动图封面](https://picx.zhimg.com/v2-51e409b11aa51c150090697429a953ed_b.jpg)





```text
#include <windows.h>

#define STYLE_NAME  L"MainForm"
#define BUTTON_ID   919

//主窗口定义
BOOL DefineMainWindow(HINSTANCE hInstance);
//主窗口创建
HWND CreateMainWindow(HINSTANCE hInstance, wchar_t* title);
//主窗口内部元素创建函数
HWND CreateButton(HWND parent, int id, wchar_t* text);
//主窗口显示函数
HWND DisplayMainWindow(HWND hWnd, int nCmdShow);
//主窗口消息处理函数
LRESULT CALLBACK WndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

static HWND MainWindow = NULL; //主窗口句柄

BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG Msg = { 0 };

    //1.自定义主窗口样式
    if (!DefineMainWindow(hInstance)){
        return FALSE;
    }

    //2.创建主窗口
    MainWindow = CreateMainWindow(hInstance, STYLE_NAME);

    if (MainWindow)
    {
        //3.创建主窗口中的控件元素
        CreateButton(MainWindow, BUTTON_ID, L"My Button");

        //4.在屏幕上显示主窗口
        DisplayMainWindow(MainWindow, nCmdShow);
    }
    else
    {
        return FALSE;
    }

    //5.进入消息循环
    while (GetMessage(&Msg,NULL,0,0))
    {
        //6.翻释并转换系统消息
        TranslateMessage(&Msg);
        //7.分发消息到对应的消息处理函数
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

BOOL DefineMainWindow(HINSTANCE hInstance)
{
    static WNDCLASS WndClass = { 0 };//系统结构类型，用于描述窗口样式

    WndClass.style         = 0;
    WndClass.cbClsExtra    = 0;
    WndClass.cbWndExtra    = 0;
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);          //定义窗口背景色
    WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);     //定义鼠标样式
    WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); //定义窗口左上角图标
    WndClass.hInstance     = hInstance;                       //定义窗口样式属于当前应用程序
    WndClass.lpfnWndProc   =(WNDPROC)WndProc;                         //窗口消息处理函数
    WndClass.lpszClassName = STYLE_NAME;                      //窗口样式名
    WndClass.lpszMenuName  = NULL;

    //将定义好的窗口样式注册到系统上
    return RegisterClass(&WndClass);
}

HWND CreateMainWindow(HINSTANCE hInstance, wchar_t* title)
{
    HWND hWnd = NULL;

    hWnd = CreateWindow(STYLE_NAME,         //通过定义好的窗口样式创建主窗口
                        title,              //主窗口标题
                        WS_OVERLAPPEDWINDOW,//创建后主窗口的显示风格
                        CW_USEDEFAULT,      //主窗口左上角x坐标
                        CW_USEDEFAULT,      //主窗口左上角y坐标
                        CW_USEDEFAULT,      //主窗口宽度
                        CW_USEDEFAULT,      //主窗口高度
                        NULL,               //父窗口
                        NULL,               //主窗口菜单
                        hInstance,          //主窗口属于当前应用程序
                        NULL);
    return hWnd;
}

HWND DisplayMainWindow(HWND hWnd, int nCmdShow)
{
    ShowWindow(hWnd, nCmdShow);  //显示窗口
    UpdateWindow(hWnd);          //刷新窗口

    return hWnd;
}

HWND CreateButton(HWND parent, int id, wchar_t* text)
{
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE);

    HWND hWnd = NULL;

    hWnd = CreateWindow(L"button",                            //通过系统预定义的窗口样式创建元素
                        text,                                 //窗口元素标题
                        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//创建后窗口元素的显示风格
                        50,                                   //窗口元素在主窗口左上角x坐标
                        50,                                   //窗口元素在主窗口左上角y坐标
                        200,                                  //窗口元素宽度
                        60,                                   //窗口元素高度
                        parent,                               //父窗口
                        (HMENU)id,                            //窗口元素ID值
                        hInstance,                            //窗口元素属于当前应用程序
                        NULL);
    return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        //调用系统提供的默认消息处理函数
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
```

## 二、小结

**（1）、现代操作系统提供原生的SDK支持GUI程序开发**

**（2）、不同操作系统上的GUI SDK不同**

**（3）、不同操作系统上的GUI开发原理相同**

**（4）、GUI程序开发包括**

**A、在代码中用程序创建窗口及窗口元素**

**B、在消息处理函数中根据程序消息作出不同响应**