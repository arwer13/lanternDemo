#include "LanternTcpConnection.h"
#include "Library/LanternCommands.h"
#include "Library/LanternProtocolParser.h"

LanternTcpConnection::LanternTcpConnection(QObject *parent)
        : QObject(parent)
        , _socket(new QTcpSocket(this))
        , _parser(new LanternProtocolParser()) {
    connect(_socket, &QAbstractSocket::stateChanged, this,
        &LanternTcpConnection::onSocketStateChanged);
    connect(_socket,
        static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
        this, &LanternTcpConnection::onSocketError);
    connect(_socket, &QTcpSocket::readyRead, this, &LanternTcpConnection::onNewSocketData);
}

LanternTcpConnection::~LanternTcpConnection() {
}

LanternTcpConnection::State LanternTcpConnection::state() const {
    return _state;
}

void LanternTcpConnection::connectToServer(const QString &host, int port) {
    _socket->connectToHost(host, port);
}

void LanternTcpConnection::disconnectFromServer() {
    _socket->disconnectFromHost();
}

void LanternTcpConnection::onSocketError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    emit errorOccured(_socket->errorString());
}

void LanternTcpConnection::onSocketStateChanged(QAbstractSocket::SocketState state) {
    switch (state) {
    case QAbstractSocket::SocketState::UnconnectedState:
        _state = State::Disconnected;
        break;
    case QAbstractSocket::SocketState::HostLookupState:
    case QAbstractSocket::SocketState::ConnectingState:
        _state = State::Connecting;
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        _state = State::Connected;
        break;
    case QAbstractSocket::SocketState::ClosingState:
        _state = State::Disconnecting;
        break;
    case QAbstractSocket::BoundState:
    case QAbstractSocket::ListeningState:
        _state = State::Disconnected;
        qWarning() << "Unexpected socket state " << state;
        break;
    }
    emit stateChanged(_state);
}

void LanternTcpConnection::onNewSocketData() {
    _parser->parse(_socket->readAll());
    while (auto command = _parser->fetch()) {
        emit commandReceived(command);
    }
}
