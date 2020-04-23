#include "LanternProtocolParserTests.h"
#include "LanternCommands.h"
#include "LanternProtocolParser.h"
#include <QTest>
#include <vector>

// const char g_commandFullOn[]{0x12, 0x00, 0x00};
std::vector<LanternCommand::Type> g_commandFullOn{0x12, 0x00, 0x00};
std::vector<LanternCommand::Type> g_commandFullOff{0x13, 0x00, 0x00};
std::vector<LanternCommand::Type> g_commandFullColorRed{0x20, 0x00, 0x03, 0xff, 0x00, 0x00};

static QByteArray toByteArray(const std::vector<LanternCommand::Type> &v) {
    return QByteArray(reinterpret_cast<const char *>(v.data()), v.size());
}

void LanternProtocolParserTests::trivialTest() {
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}

void LanternProtocolParserTests::testEmptyParser() {
    LanternProtocolParser parser;
    QVERIFY(!parser.fetch());
}

void LanternProtocolParserTests::testSingleOnCommand() {
    LanternProtocolParser parser;
    parser.parse(toByteArray(g_commandFullOn));
    auto command = parser.fetch();
    QVERIFY(command);
    QVERIFY(command->type() == LanternCommand::typeOn);
}

void LanternProtocolParserTests::testSingleOffCommand() {
    LanternProtocolParser parser;
    parser.parse(toByteArray(g_commandFullOff));
    auto command = parser.fetch();
    QVERIFY(command);
    QVERIFY(command->type() == LanternCommand::typeOff);
}

void LanternProtocolParserTests::testSingleColorCommand() {
    LanternProtocolParser parser;
    parser.parse(toByteArray(g_commandFullColorRed));
    auto command = parser.fetch();
    QVERIFY(command);
    QVERIFY(command->type() == LanternCommand::typeColor);
}
