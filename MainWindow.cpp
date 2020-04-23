#include "MainWindow.h"
#include "LanternTcpConnection.h"
#include "Library/LanternCommands.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

void MainWindow::setConnection(LanternTcpConnection *connection) {
    _connection = connection;
    // GUI EVENTS POPAGATION
    connect(ui->btnConnect, &QPushButton::clicked,
        [this]() { _connection->connectToServer(ui->serverUrl->text()); });
    connect(ui->btnDisconnect, &QPushButton::clicked,
        [this]() { _connection->disconnectFromServer(); });

    // DATA EVENTS PROPAGATION
    connect(_connection, &LanternTcpConnection::commandReceived,
        [this](std::shared_ptr<LanternCommand> command) {
            ui->statusbar->showMessage("Got: " + command->toString());
        });
}

MainWindow::~MainWindow() {
    delete ui;
}
