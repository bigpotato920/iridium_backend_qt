#include "iridiumdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IridiumDialog w;
    w.show();
    
    return a.exec();
}
