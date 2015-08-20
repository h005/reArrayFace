#include <QCoreApplication>
#include <QString>
#include "ufface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString filename = "E:/off/4.off";

    UFface *ufface = new UFface(filename);
    ufface->unionFinal();
    ufface->reArrange();
    ufface->unionFinal();

    ufface->printOut(filename);
    printf("print done\n");
    return a.exec();
}
