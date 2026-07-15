#ifndef CARDISP_H
#define CARDISP_H

#include <QWidget>

#include <QPainter>
#include <QtMath>

class CarDisp : public QWidget
{
    Q_OBJECT

public:
    CarDisp(QWidget *parent = nullptr);
    ~CarDisp();

public:
    void setvalues(qreal iValues);
    void paintEvent(QPaintEvent *event) override;

private:
    qreal qValues;
    const static int radius;
    const static int maxv;
    const static int minv;
};
#endif // CARDISP_H
