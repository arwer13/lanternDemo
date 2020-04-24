#pragma once
#include "LanternTcpConnection.h"
#include <QMainWindow>
#include <memory>

class LanternWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Attach connection to the MainWindow. Call only once
     * @param connection Musn't be nullptr
     *
     * If called more than once does no check and sets the state and
     * connects to the given connectin events
     */
    void setConnection(LanternTcpConnection *connection);

    virtual ~MainWindow();

public slots:
    void onCommand(std::shared_ptr<LanternCommand> command);
    void onLanternConnectionStateChanged(LanternTcpConnection::State state);
    void onLanternConnectionError(const QString &message);

private:
    Ui::MainWindow *ui;
    LanternTcpConnection *_connection = nullptr;
    LanternWidget *_lantern = nullptr;

    //! Try to read host and port from url string. If fails return false.
    //! Url string format is supposed to be: "<host>:<port>"
    static bool _parseHostAndPort(const QString &url, QString &host, int &port);

    //! Set initial state of the main window
    void _setInitialState();
};
