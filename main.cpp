#include "LanternTcpConnection.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LanternTcpConnection connection;

    MainWindow w;
    w.setConnection(&connection);
    w.show();
    return a.exec();
}
