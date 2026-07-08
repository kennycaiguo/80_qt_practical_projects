# 1.内容提要

![image-20260703183510395](./note06-开发Qt版的MP4播放器.assets/image-20260703183510395.png)

# 2.QMediaPlayer类以及应用



# 3.QGraphicsView控件实战



# 4.QVideoWidget/QGraphicsScene

## 课堂演练

### 1.新建一个qt widget项目，起名qtvideoplayer,继承自QMainWindow，然后我们把我们的主窗口命名为VideoPlayer

![image-20260703193151029](./note06-开发Qt版的MP4播放器.assets/image-20260703193151029.png)



### 2.找一些图片，然后在源码文件夹里面新建一个images文件夹，把他们放入这个文件夹，然后找一张ico图标放到源码文件夹里面

![image-20260703193919996](./note06-开发Qt版的MP4播放器.assets/image-20260703193919996.png)

### 3.打开ui文件，把菜单删除

![image-20260703194031605](./note06-开发Qt版的MP4播放器.assets/image-20260703194031605.png)

### 4.然后我们开始搭建播放器界面，屏幕我们使用GraphicsView控件

![image-20260703195411776](./note06-开发Qt版的MP4播放器.assets/image-20260703195411776.png)

### 5.给窗口添加5个按钮

![image-20260703200041868](./note06-开发Qt版的MP4播放器.assets/image-20260703200041868.png)

### 6.调整按钮的大小然后在右边添加一个slider，然后选择按钮和滑块，给他们添加视频布局

![image-20260703200608864](./note06-开发Qt版的MP4播放器.assets/image-20260703200608864.png)

### 7.然后添加四个label和一个滑块，把当前播放标签右边的标签的内容清空，然后把他的frameShape属性改为StyledPanel，把下面的滑块右边的标签的frameShape属性改为StyledPanel

![image-20260703202138222](./note06-开发Qt版的MP4播放器.assets/image-20260703202138222.png)



### 8.给项目添加一个resource.qrc文件，然后把图片添加进来

![image-20260704120249401](./note06-开发Qt版的MP4播放器.assets/image-20260704120249401.png)

### 9.打开ui文件，给每一个按钮添加对应的图片

![image-20260704120704796](./note06-开发Qt版的MP4播放器.assets/image-20260704120704796.png)

### 10.给音量滑块设置一个默认值60

![image-20260704120947127](./note06-开发Qt版的MP4播放器.assets/image-20260704120947127.png)



### 11.打开.pro文件，给项目添加多媒体支持

![image-20260704121911584](./note06-开发Qt版的MP4播放器.assets/image-20260704121911584.png)

### 12.在videoplayer.h里面添加相应的头文件，需要注意，Qt6已经没有QMediaPlayList这个头文件了。需要用QList< Qurl>来代替

![image-20260704123357857](./note06-开发Qt版的MP4播放器.assets/image-20260704123357857.png)

### 13.然后我们可以添加一些简单的播放代码，首先，需要在播放器的构造函数里面初始化我们的m_player指向的对象，

![image-20260704170112004](./note06-开发Qt版的MP4播放器.assets/image-20260704170112004.png)

### 然后给打开按钮添加代码，获取用户选择的文件显示在当前文件旁边的标签上

![image-20260704165605057](./note06-开发Qt版的MP4播放器.assets/image-20260704165605057.png)

### 然后我们在播放按钮的槽函数里面添加播放代码，播放的步骤是先创建QMediaPlaer的对象标签返回他的指针，这个我们已经做了，然后我们需要创建一个QVideoWidget对象，标签返回他的指针，这里使用我们的graphicsView控件来作为QVideoWidget对象的父窗口，然后注意必须给QVideoWidget对象设置为可见，并且设置他的大小，否则你就看不到画面，然后还需要给播放器对象设置视频源，再调用他的播放方法

![image-20260704170809283](./note06-开发Qt版的MP4播放器.assets/image-20260704170809283.png)

### 此时播放功能实现了，效果如下

![image-20260704170929366](./note06-开发Qt版的MP4播放器.assets/image-20260704170929366.png)



### 14.然后我们给播放器添加暂停功能，也是给暂停按钮添加点击事件的槽函数，我们的暂停功能是可以在暂停的位置继续播放的。我们需要感觉当前播放器的播放状态来判断，如果当前正在播放，我们就把视频暂停，同时设置暂停按钮的图标为播放图标，文本为Resume，也就是继续，如果当前播放器处于暂停状态，我们就从暂停的位置继续播放，同时把暂停按钮的图标设置为暂停图标，把暂停按钮的文本设置为暂停。

![image-20260704180154503](./note06-开发Qt版的MP4播放器.assets/image-20260704180154503.png)

### 15.然后我们给停止按钮添加点击事件的槽函数，也就是实现点击停止播放功能，需要注意的是我们要考虑有时候，用户可能点击了暂停按钮后，又点击了停止按钮，此时我们需要先把停止按钮的状态恢复为最初的状态，然后再停止播放

![image-20260704180520933](./note06-开发Qt版的MP4播放器.assets/image-20260704180520933.png)

### 16.其实我们的播放代码也需要优化，首先，为了避免多次创建对象，我们可以把音频输出对象和视频窗口部件对象定义为成员变量，

![image-20260704181135845](./note06-开发Qt版的MP4播放器.assets/image-20260704181135845.png)

### 然后在构造函数里面创建对象，

![image-20260704181439642](./note06-开发Qt版的MP4播放器.assets/image-20260704181439642.png)

### 在播放代码里面调用他的方法，

![image-20260704182645014](./note06-开发Qt版的MP4播放器.assets/image-20260704182645014.png)

### 当用户暂停了应该视频，然后他又点击开始也就是他想从头开始观看，此时我们也需要把暂停按钮恢复，我们把恢复暂停按钮的代码封装一个函数，方便调用，

![image-20260704182802985](./note06-开发Qt版的MP4播放器.assets/image-20260704182802985.png)

### 然后我们来优化停止按钮的代码

![image-20260704182823082](./note06-开发Qt版的MP4播放器.assets/image-20260704182823082.png)

## 17.继续优化代码，我们添加设置音量的代码，用音量滑块当前的值转化为float后再除于滑块的最大值获取到一个小数然后设置到音频输出对象里面

![image-20260704191915793](./note06-开发Qt版的MP4播放器.assets/image-20260704191915793.png)

### 18.然后我们给音量滑块添加一个处理valueChanged信号的槽函数

![image-20260704201500187](./note06-开发Qt版的MP4播放器.assets/image-20260704201500187.png)

### 经过测试发现音量滑块读取可以调整音量了。

### 19.然后我们需要实现显示播放进度的功能，

#### 首先，我们需要在播放器的构造函数里面给m_player绑定两个信号处理

![image-20260708110045421](./note06-开发Qt版的MP4播放器.assets/image-20260708110045421.png)

#### 然后我们需要创建这两个槽函数

![image-20260708105850841](./note06-开发Qt版的MP4播放器.assets/image-20260708105850841.png)

#### 然后，我们给进度滑块添加一个sliderMoved信号处理，使用的槽函数是播放器的setPostion槽

![image-20260708115222558](./note06-开发Qt版的MP4播放器.assets/image-20260708115222558.png)

#### 最后，我们需要在更新播放器的播放位置的时候同时更新我们的播放百分比，我们在updatePosition槽函数里面添加下面的代码

![image-20260708121135997](./note06-开发Qt版的MP4播放器.assets/image-20260708121135997.png)

## 至此，一个功能比较完善的基本播放器就完成了，我们还可以开发一个qtbettervideoplayer,把显示当前播放的歌曲名称的label变为LineEdit控件，方便我们粘贴网络视频的链接，还有就是添加一个播放列表和一个复选框，勾选它在打开文件的同时会开始播放，取消勾选，就只显示选中的文件名，点击播放按钮才播放，还有，播放视频是时候，窗口的标题显示QtBetterVideoPlayer--歌曲名称，停止播放，窗口的标题显示QtBetterVideoPlayer等等，以后有时间再来完成



# 扩展，

## 参考文档1：网址：https://zhuanlan.zhihu.com/p/628172987

## 参考文档2：https://developer.aliyun.com/article/1560358

## 参考文档3： https://developer.aliyun.com/article/1350389

## 参考源码： https://github.com/lmshao/Aurora

## 参考源码2 ： https://github.com/vlc-qt

## 参考源码3：https://github.com/vlc-qt/examples

## 参考源码4：https://github.com/yundiantech/videoplayer

# 扩展2：QMediaPlayer实现显示播放进度的功能

使用 `QMediaPlayer` 显示播放进度，核心在于通过信号槽机制获取**总时长**和**当前进度**，并将其与进度条（`QSlider`）和标签（`QLabel`）绑定。以下是具体的实现步骤和核心代码： [[1](https://blog.csdn.net/weixin_55735677/article/details/131247801), [2](https://developer.aliyun.com/article/934273), [3](https://www.kafuuchino.fun/archives/24)]

1. 核心信号与属性

- **`durationChanged(qint64 duration)`**：当媒体总时长发生改变时触发，用于初始化进度条的最大值。
- **`positionChanged(qint64 position)`**：当当前播放位置改变时触发，用于实时更新进度条的值和时间显示。 [[1](https://www.kafuuchino.fun/archives/24)]
- 实现代码示例 (以 C++ 为例)

步骤一：连接信号槽

在你的主窗口或播放器类中，将 `QMediaPlayer` 的信号绑定到对应的更新函数： [[1](https://www.kafuuchino.fun/archives/24)]

cpp

```
// 假设 player 是你的 QMediaPlayer 对象， progressSlider 是进度条
connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
```

 

步骤二：编写槽函数更新 UI

cpp

```
// 更新总时长，设置进度条的最大值
void MainWindow::updateDuration(qint64 duration) {
    progressSlider->setMaximum(duration);
}

// 更新当前播放进度
void MainWindow::updatePosition(qint64 position) {
    // 只有在用户没有拖动进度条时才更新，防止进度条卡顿
    if (!progressSlider->isSliderDown()) {
        progressSlider->setValue(position);
    }
}
```

 

步骤三：实现进度条拖拽快进

当用户手动拖动进度条时，需要将播放器跳转到对应的时间点。首先需连接 `QSlider` 的信号： [[1](https://blog.csdn.net/weixin_55735677/article/details/131247801)]

cpp

```
connect(progressSlider, &QSlider::sliderMoved, this, &MainWindow::seekPosition);
```

 

cpp

```
// 实现快进/快退
void MainWindow::seekPosition(int position) {
    player->setPosition(position);
}
```

 

3. 可选扩展：显示文本时间格式

为了提升用户体验，通常会将获取到的毫秒数（`qint64`）格式化为 MM:SS 显示在标签上：

cpp

```
// 格式化时间函数
QString formatTime(qint64 timeMs) {
    qint64 seconds = timeMs / 1000;
    qint64 minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
```

 

