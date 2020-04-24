#include "LanternTcpConnection.h"
#include "Library/LanternCommands.h"
#include "Library/LanternProtocolParser.h"

LanternTcpConnection::LanternTcpConnection(QObject *parent)
    : QObject(parent) {
    _socket = new QTcpSocket(this);
    _parser.reset(new LanternProtocolParser());

    connect(_socket, &QAbstractSocket::stateChanged, this,
        &LanternTcpConnection::onSocketStateChanged);
    // TODO: Why had to convert signature?
    connect(_socket,
        static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
        this, &LanternTcpConnection::onSocketError);
    connect(_socket, &QTcpSocket::readyRead, this, &LanternTcpConnection::onNewSocketData);
}

LanternTcpConnection::~LanternTcpConnection() {
}

QAbstractSocket::SocketState LanternTcpConnection::state() const {
    return _socket->state();
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
        emit stateChanged(State::Disconnected);
        break;
    case QAbstractSocket::SocketState::HostLookupState:
    case QAbstractSocket::SocketState::ConnectingState:
        emit stateChanged(State::Connecting);
        break;
    case QAbstractSocket::SocketState::ConnectedState:
        emit stateChanged(State::Connected);
        break;
    case QAbstractSocket::SocketState::ClosingState:
        emit stateChanged(State::Disconnecting);
        break;
    case QAbstractSocket::BoundState:
    case QAbstractSocket::ListeningState:
        emit stateChanged(State::Disconnected);
        qWarning() << "Unexpected socket state " << state;
        break;
    }
}

void LanternTcpConnection::onNewSocketData() {
    const auto chunk = _socket->readAll();
    _parser->parse(chunk);
    while (auto command = _parser->fetch()) {
        emit commandReceived(command);
    }
}
