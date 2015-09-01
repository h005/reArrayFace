#include <QCoreApplication>
#include <QString>
#include "ufface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QString filename = "E:/off/1.off";
    QString filename = "E:/Qt/viewPoint/3Dfeature/build-3Dfeature6-Desktop_Qt_5_4_0_MSVC2013_OpenGL_32bit-Debug/output.off";

    UFface *ufface = new UFface(filename);

    ufface->unionFinal();

    ufface->printOut(filename);
    printf("print done\n");
    return a.exec();
}
