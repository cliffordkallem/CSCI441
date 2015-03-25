#include <QApplication>

#include "glwidget.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    GLWidget glWidget;
    glWidget.resize(640,480);
    glWidget.show();
    if(argc >2)
    {
       glWidget.in = argv[1];
        glWidget.input = true;
    }
    return a.exec();
}
