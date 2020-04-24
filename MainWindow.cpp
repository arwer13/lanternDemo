#include "MainWindow.h"
#include "LanternTcpConnection.h"
#include "LanternWidget.h"
#include "Library/LanternCommands.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
    _lantern = new LanternWidget(this);
    ui->lanternWidget->layout()->addWidget(_lantern);

    _setInitialState();
}

void MainWindow::setConnection(LanternTcpConnection *connection) {
    _connection = connection;
    // GUI EVENTS POPAGATION
    connect(ui->btnConnect, &QPushButton::clicked, [this]() {
        QString host;
        int port;
        if (!_parseHostAndPort(ui->serverUrl->text(), host, port)) {
            ui->connectionStatus->setText(tr("Invalid server and port string"));
            return;
        }
        _connection->connectToServer(host, port);
        ui->connectionError->clear();
    });
    connect(ui->btnDisconnect, &QPushButton::clicked,
        [this]() { _connection->disconnectFromServer(); });

    // DATA EVENTS PROPAGATION
    connect(_connection, &LanternTcpConnection::commandReceived, this, &MainWindow::onCommand);
    connect(_connection, &LanternTcpConnection::stateChanged, this,
        &MainWindow::onLanternConnectionStateChanged);
    connect(_connection, &LanternTcpConnection::errorOccured, this,
        &MainWindow::onLanternConnectionError);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCommand(std::shared_ptr<LanternCommand> command) {
    ui->statusbar->showMessage("Got: " + command->toString(), 3000);

    if (auto commandColor = dynamic_cast<LanternCommandColor *>(command.get())) {
        const auto newColor = commandColor->color();
        _lantern->setColor(newColor);
    } else if (auto commandOn = dynamic_cast<LanternCommandOn *>(command.get())) {
        Q_UNUSED(commandOn)
        _lantern->turnOn();
    } else if (auto commandOff = dynamic_cast<LanternCommandOff *>(command.get())) {
        Q_UNUSED(commandOff)
        _lantern->turnOff();
    }
}

void MainWindow::onLanternConnectionStateChanged(LanternTcpConnection::State state) {
    switch (state) {
    case LanternTcpConnection::State::Disconnected:
        ui->connectionStatus->setText(tr("Not connected"));
        ui->btnDisconnect->setEnabled(false);
        ui->serverUrl->setEnabled(true);
        break;
    case LanternTcpConnection::State::Connecting:
        ui->connectionStatus->setText(tr("Connecting..."));
        ui->serverUrl->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        break;
    case LanternTcpConnection::State::Connected:
        ui->connectionStatus->setText(tr("Connected"));
        ui->connectionError->clear();
        ui->serverUrl->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        break;
    case LanternTcpConnection::State::Disconnecting:
        ui->connectionStatus->setText(tr("Disconnecting..."));
        ui->serverUrl->setEnabled(false);
        ui->btnDisconnect->setEnabled(false);
        break;
    }
}

void MainWindow::onLanternConnectionError(const QString &message) {
    ui->connectionError->setText(message);
}

bool MainWindow::_parseHostAndPort(const QString &url, QString &host, int &port) {
    const auto parts = url.split(':');
    if (parts.size() != 2) {
        return false;
    }
    bool isOk;
    host = parts[0];
    port = parts[1].toInt(&isOk);
    if (host.isEmpty())
        return false;
    return isOk;
}

void MainWindow::_setInitialState() {
    onLanternConnectionStateChanged(LanternTcpConnection::State::Disconnected);
    ui->connectionError->clear();
}
