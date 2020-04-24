#include "LanternTcpConnection.h"
#include "MainWindow.h"
#include <QApplication>
#include <QFile>

static QString loadMainStyleSheet() {
    QFile file(":/qss/main.qss");
    file.open(QFile::ReadOnly);
    return QString::fromLatin1(file.readAll());
}

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    application.setStyleSheet(loadMainStyleSheet());

    MainWindow w;

    LanternTcpConnection connection;
    w.setConnection(&connection);

    w.show();
    return application.exec();
}
