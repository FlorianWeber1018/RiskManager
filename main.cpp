#include "risk_manager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Risk_Manager w;
    w.show();


    return a.exec();
}
