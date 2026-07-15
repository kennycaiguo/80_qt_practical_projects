#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <cmath>

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();
private:
    QTimer *timer;
    double pointerAngle;

private slots:
    void updatePointer();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};
#endif // DASHBOARD_H
