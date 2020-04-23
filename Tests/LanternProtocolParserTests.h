#pragma once
#include <QByteArray>
#include <QObject>

class LanternProtocolParserTests : public QObject {
    Q_OBJECT
private slots:
    void trivialTest();
    void testEmptyParser();
    void testSingleOnCommand();
    void testSingleOffCommand();
    void testSingleColorCommand();
};
