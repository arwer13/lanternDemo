#pragma once

#include <QMainWindow>

class LanternTcpConnection;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setConnection(LanternTcpConnection *connection);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LanternTcpConnection *_connection = nullptr;
};
