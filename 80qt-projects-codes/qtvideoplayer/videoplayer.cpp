#include "videoplayer.h"
#include "ui_videoplayer.h"
#include<QFileDialog>
#include<QDebug>
#include<QAudioOutput>

VideoPlayer::VideoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoPlayer)
{
    ui->setupUi(this);
    //初始化我们的播放器，音频输出和视频窗口部件成员变量
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_videoWidget = new QVideoWidget(ui->graphicsView);
    m_videoWidget->setGeometry(0,0,ui->graphicsView->width(),ui->graphicsView->height());

    //给播放器绑定两个信号处理方便显示播放进度
    connect(m_player, &QMediaPlayer::durationChanged, this, &VideoPlayer::updateDuration);
    connect(m_player, &QMediaPlayer::positionChanged, this, &VideoPlayer::updatePosition);
   //拖动滑块改变播放器的播放位置
    // 核心逻辑公式：播放器进度 = 滑块位置
    connect(ui->prog_slider, &QSlider::sliderMoved, m_player, &QMediaPlayer::setPosition);

}

VideoPlayer::~VideoPlayer()
{
    delete ui;
    delete m_player;
    delete m_audioOutput;
    delete m_videoWidget;
}


void VideoPlayer::on_btnOpen_clicked()
{
    QString path = QFileDialog::getOpenFileName(nullptr,"Choose Video","F:/videos","all files(*.*);;mp4 video(*.mp4)");
    // qDebug()<<path;
    ui->lbl_current->setText(path);
}


void VideoPlayer::on_btnPlay_clicked()
{
    //对象的创建放在构造函数，只需要创建一次
    //为了保险起见，如果对象为空我们就创建
    if(m_audioOutput == nullptr){
        m_audioOutput = new QAudioOutput(this);
    }
    if(m_videoWidget == nullptr){
         m_videoWidget = new QVideoWidget(ui->graphicsView);
    }
    //有了，就直接使用
    m_videoWidget->setVisible(true);
    m_videoWidget->setGeometry(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    //恢复停止按钮的状态,用来解决用户点击了暂停按键后，又点击开始播放，此时视频从头开始播放，暂停按钮如果不恢复原来状态，用户体验就非常糟糕
    restPauseState();
    // 2. 绑定管线
    m_player->setAudioOutput(m_audioOutput);
    m_player->setVideoOutput(m_videoWidget);
    //设置开机音量，是一个0-1直接的数值，我们需要利用滑块当前的值除于滑块的最大值来获取音量比例，然后设置到音频输出对象里面
    //注意这个值是float类型的，所以，你必须把其中一个书转化为float类型，否则你得到的是0
    float vol = float(ui->vol_slider->value())/ui->vol_slider->maximum();
    m_player->audioOutput()->setVolume(vol);
    //3.设置视频源
    m_player->setSource(ui->lbl_current->text());
    m_player->play();//这里的播放的从头开始的
}


void VideoPlayer::on_btnPause_clicked()
{
    //注意：暂停按钮可以做暂停和继续播放的功能，需要工具当前的媒体状态来判断
    //如果当前真正播放，我们就暂停视频并且修改图标为播放图标，文本为继续
    if(m_player->playbackState()==QMediaPlayer::PlayingState){
        m_player->pause();
        ui->btnPause->setIcon(QIcon(":/images/start.png"));
        ui->btnPause->setText("Resume");
      //如果当前是
    } else if(m_player->playbackState()==QMediaPlayer::PausedState){
        ui->btnPause->setIcon(QIcon(":/images/pause.png"));
        ui->btnPause->setText("Pause");
        m_player->play();
    }
}


void VideoPlayer::restPauseState()
{
    if(m_player->playbackState()==QMediaPlayer::PausedState){
        ui->btnPause->setIcon(QIcon(":/images/pause.png"));
        ui->btnPause->setText("Pause");
    }
}

void VideoPlayer::on_btnStop_clicked()
{
    //如果暂停后用户又按下停止，此时播放器停止播放，也需要把暂停按钮恢复到起始状态才对
    //注意，设置按钮状态的代码必须在stop()函数的调用的前面，否则没有效果
    restPauseState();
    m_player->stop();
}


void VideoPlayer::on_vol_slider_valueChanged(int value)
{
    //设置音量，当用户移动滑块，就会改变他的值，然后就会触发valueChanged信号，我们就可以根据这个信号来设置音量
    //是一个0-1直接的数值，我们需要利用滑块当前的值除于滑块的最大值来获取音量比例，然后设置到音频输出对象里面
    //注意这个值是float类型的，所以，你必须把其中一个书转化为float类型，否则你得到的是0
    float vol = float(value)/ui->vol_slider->maximum();
    m_player->audioOutput()->setVolume(vol);
}

void VideoPlayer::updateDuration(qint64 duration)
{   //获取视频的总长度并且把它设置为进度滑块的最大值
    // qDebug()<<"duration:"<<duration;
    ui->prog_slider->setMaximum(duration);
}

void VideoPlayer::updatePosition(qint64 position)
{
    // qDebug()<<"position:"<<position;
    // 只有在用户没有拖动进度条时才更新，防止进度条卡顿
    if(!ui->prog_slider->isSliderDown()){
        ui->prog_slider->setValue(position);
    }
    int mx = ui->prog_slider->maximum();
    float prog =position*100/mx;
    QString str=QString::number(prog) + "%";
    ui->lbl_prog->setText(str);
}

