#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QGraphicsVideoItem>
#include<QList>
#include<QUrl>



QT_BEGIN_NAMESPACE
namespace Ui { class VideoPlayer; }
QT_END_NAMESPACE

class VideoPlayer : public QMainWindow
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void restPauseState();

private slots:
    void on_btnOpen_clicked();

    void on_btnPlay_clicked();

    void on_btnPause_clicked();

    void on_btnStop_clicked();

    void on_vol_slider_valueChanged(int value);

private:
    Ui::VideoPlayer *ui;
    QMediaPlayer *m_player;
    QGraphicsVideoItem *m_videoItem;
    QString m_duration;
    QString m_position;
    QList<QUrl> m_playlist;
    QAudioOutput *m_audioOutput ;
    QVideoWidget *m_videoWidget ;




};
#endif // VIDEOPLAYER_H
