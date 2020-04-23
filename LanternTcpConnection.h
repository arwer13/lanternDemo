#pragma once
#include <QObject>
#include <QTcpSocket>
#include <memory>

class LanternProtocolParser;
class LanternCommand;

class LanternTcpConnection : public QObject {
    Q_OBJECT
public:
    LanternTcpConnection(QObject *parent = nullptr);
    virtual ~LanternTcpConnection();
    QAbstractSocket::SocketState state() const;
signals:
    void stateChanged(QAbstractSocket::SocketState);
    void errorOccured(QAbstractSocket::SocketError, QString errorString);
    void commandReceived(std::shared_ptr<LanternCommand> command);
    //    void connected();
    //    void disconnected();
    //    void hostFound();

public slots:
    void connectToServer(const QString &url);
    void disconnectFromServer();
    //    void onDisconnected();
    //    void onError(QAbstractSocket::SocketError err);

protected slots:
    void onSocketError(QAbstractSocket::SocketError);
    void onNewSocketData();

protected:
    QTcpSocket *_socket = nullptr;
    std::unique_ptr<LanternProtocolParser> _parser;
};
