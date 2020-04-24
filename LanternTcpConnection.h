#pragma once
#include <QObject>
#include <QTcpSocket>
#include <memory>

class LanternProtocolParser;
class LanternCommand;

/**
 * @brief Class for communication in Lantern Ptorocol through TCP connection
 */
class LanternTcpConnection : public QObject {
    Q_OBJECT
public:
    LanternTcpConnection(QObject *parent = nullptr);
    virtual ~LanternTcpConnection();

    enum class State {
        Disconnected,
        Connecting,
        Connected,
        Disconnecting,
    };

    State state() const;

signals:
    void stateChanged(State state);
    void errorOccured(QString errorString);
    void commandReceived(std::shared_ptr<LanternCommand> command);

public slots:
    void connectToServer(const QString &host, int port);
    void disconnectFromServer();

protected slots:
    void onSocketError(QAbstractSocket::SocketError errorType);
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void onNewSocketData();

protected:
    QTcpSocket *_socket = nullptr;
    std::unique_ptr<LanternProtocolParser> _parser;
    State _state = State::Disconnected;
};
