#include "LanternTcpConnection.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LanternTcpConnection connection;

    MainWindow w;
    w.setConnection(&connection);

    QObject::connect(&connection, &LanternTcpConnection::stateChanged,
        [](QAbstractSocket::SocketState state) { qDebug() << state; });
    QObject::connect(&connection, &LanternTcpConnection::errorOccured,
        [](QAbstractSocket::SocketError error, QString errorMessage) {
            qDebug() << error << errorMessage;
        });

    w.show();
    return a.exec();
}
