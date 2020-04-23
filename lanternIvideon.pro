QT += core widgets network
CONFIG += c++11

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Library/LanternCommands.cpp \
    Library/LanternProtocolParser.cpp \
    LanternTcpConnection.cpp

HEADERS += \
    MainWindow.h \
    Library/LanternCommands.h \
    Library/LanternProtocolParser.h \
    LanternTcpConnection.h

FORMS += \
    MainWindow.ui
