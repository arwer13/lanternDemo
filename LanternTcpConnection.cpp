#include "LanternTcpConnection.h"
#include "Library/LanternCommands.h"
#include "Library/LanternProtocolParser.h"

LanternTcpConnection::LanternTcpConnection(QObject *parent)
    : QObject(parent) {
    _socket = new QTcpSocket(this);
    _parser.reset(new LanternProtocolParser());

    connect(_socket, &QAbstractSocket::stateChanged, this, &LanternTcpConnection::stateChanged);
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

void LanternTcpConnection::connectToServer(const QString &urlString) {
    const auto parts = urlString.split(':');
    if (parts.size() != 2) {
        // TODO: Handle the error and maybe parse in different place
        return;
    }
    const auto host = parts[0];
    const auto port = parts[1].toInt();
    qDebug() << host << port;
    _socket->connectToHost(host, port);
}

void LanternTcpConnection::disconnectFromServer() {
    _socket->disconnectFromHost();
}

void LanternTcpConnection::onSocketError(QAbstractSocket::SocketError error) {
    emit errorOccured(error, _socket->errorString());
}

void LanternTcpConnection::onNewSocketData() {
    const auto chunk = _socket->readAll();
    _parser->parse(chunk);
    while (auto command = _parser->fetch()) {
        emit commandReceived(command);
    }
}
