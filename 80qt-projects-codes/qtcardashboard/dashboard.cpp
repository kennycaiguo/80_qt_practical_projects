#include "dashboard.h"


Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口大小300，300
    setFixedSize(300,300);
    //创建定时器
    timer = new QTimer(this);
    //给定时器绑定信号
    connect(timer,&QTimer::timeout,this,&Dashboard::updatePointer);
    //启动定时器
    timer->start(1000);
    // 设置指针角度
    pointerAngle = 0;
}

Dashboard::~Dashboard()
{
}

void Dashboard::updatePointer()
{
   //更新指针角度
   pointerAngle += 6;
   if(pointerAngle > 360){
       pointerAngle -=360;
   }
   //调用重绘方法
   update();
}

void Dashboard::paintEvent(QPaintEvent *event)
{
    //绘制仪表盘
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //移动到中心点，
    painter.translate(width()/2,height()/2);
    //绘制椭圆
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(30,30,30));
    painter.drawEllipse(-125,-125,250,250);

    //修改画刷，在绘制里面一点的椭圆
    painter.setBrush(QColor(50,50,50));
    painter.drawEllipse(-120,-120,240,240);

    for(int i=0;i<60;++i){
        int angle = i * 6;
        double dx = std::sin(angle * M_PI/180);
        double dy = -std::cos(angle * M_PI/180);
        if(i%5==0){
            painter.setPen(QPen(QColor(255,255,255),4));
            painter.drawLine(100*dx,100*dy,110*dx,110*dy);
        }else{
            painter.setPen(QPen(QColor(255,255,255),2));
            painter.drawLine(105*dx,105*dy,110*dx,110*dy);
        }
    }

    //绘制指针
    painter.rotate(pointerAngle);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,50,50));
    painter.drawPolygon(QPolygonF(QVector<QPointF>() << QPointF(0, -3) << QPointF(100, 0) << QPointF(0, 3)));

}

