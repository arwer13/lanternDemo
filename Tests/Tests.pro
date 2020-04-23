LIBRARY_ROOT = $$PWD/../Library
TEST_ROOT = $$PWD

#TEMPLATE = app
TARGET = LanternTests
QT += core testlib

INCLUDEPATH += $$LIBRARY_ROOT

HEADERS += $$TEST_ROOT/*.h
SOURCES += $$TEST_ROOT/*.cpp

SOURCES += \
    $$LIBRARY_ROOT/LanternCommands.cpp \
    $$LIBRARY_ROOT/LanternProtocolParser.cpp
