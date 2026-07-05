#include "videoplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("QT_QUICK_BACKEND", "software");
    qputenv("QT_XCB_FORCE_SOFTWARE_OPENGL", "1");
    VideoPlayer w;
    w.show();
    return a.exec();
}
