#include "MainWindow.h"
#include "LanternTcpConnection.h"
#include "LanternWidget.h"
#include "Library/LanternCommands.h"
#include "ui_MainWindow.h"
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    _lantern = new LanternWidget(this);
    ui->lanternWidget->layout()->addWidget(_lantern);

    _setInitialState();
}

void MainWindow::setConnection(LanternTcpConnection *connection) {
    assert(connection);
    _connection = connection;

    // GUI EVENTS POPAGATION
    connect(ui->btnConnect, &QPushButton::clicked, [this]() {
		const auto [host, port, isParsed] = _parseHostAndPort(ui->serverUrl->text());
		if (!isParsed) {
            ui->connectionStatus->setText(tr("Invalid server and port string"));
            return;
        }
        _connection->connectToServer(host, port);
        ui->errorMessage->clear();
    });
    connect(ui->btnDisconnect, &QPushButton::clicked,
        [this]() { _connection->disconnectFromServer(); });

    // DATA EVENTS PROPAGATION
    connect(_connection, &LanternTcpConnection::commandReceived, this, &MainWindow::onCommand);
    connect(_connection, &LanternTcpConnection::stateChanged, this,
        &MainWindow::onLanternConnectionStateChanged);
    connect(_connection, &LanternTcpConnection::errorOccured, this,
        &MainWindow::onLanternConnectionError);
    onLanternConnectionStateChanged(_connection->state());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCommand(std::shared_ptr<LanternCommand> command) {
    ui->statusbar->showMessage("Got: " + command->toString(), 3000);

	if (command->type() == LanternCommand::typeColor) {
		const auto commandColor = dynamic_cast<const LanternCommandColor *>(command.get());
		_lantern->setColor(commandColor->color());
	} else if (command->type() == LanternCommand::typeOn) {
        _lantern->turnOn();
	} else if (command->type() == LanternCommand::typeOff) {
        _lantern->turnOff();
    } else {
        ui->errorMessage->setText(tr("Got unknown GUI command ") + command->toString());
    }
}

void MainWindow::onLanternConnectionStateChanged(LanternTcpConnection::State state) {
    switch (state) {
    case LanternTcpConnection::State::Disconnected:
        ui->connectionStatus->setText(tr("Not connected"));
        ui->btnConnect->setEnabled(true);
        ui->btnDisconnect->setEnabled(false);
        ui->serverUrl->setEnabled(true);
        break;
    case LanternTcpConnection::State::Connecting:
        ui->connectionStatus->setText(tr("Connecting..."));
        ui->serverUrl->setEnabled(false);
        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        break;
    case LanternTcpConnection::State::Connected:
        ui->connectionStatus->setText(tr("Connected"));
        ui->errorMessage->clear();
        ui->serverUrl->setEnabled(false);
        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        break;
    case LanternTcpConnection::State::Disconnecting:
        ui->connectionStatus->setText(tr("Disconnecting..."));
        ui->serverUrl->setEnabled(false);
        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(false);
        break;
    }
}

void MainWindow::onLanternConnectionError(const QString &message) {
    ui->errorMessage->setText(message);
}

std::tuple<QString, int, bool> MainWindow::_parseHostAndPort(const QString &url) {
	static const auto emptyResult = std::make_tuple(QString(), -1, false);
	static const QRegularExpression pattern(R"((.+):(\d+))");
    const auto match = pattern.match(url.trimmed());
    if (!match.isValid()) {
		return emptyResult;
    }
    bool isOk;
	QString host = match.captured(1);
	int port = match.captured(2).toInt(&isOk);
    if (host.isEmpty() || !isOk)
		return emptyResult;
	return {host, port, true};
}

void MainWindow::_setInitialState() {
    onLanternConnectionStateChanged(LanternTcpConnection::State::Disconnected);
    ui->errorMessage->clear();
}
