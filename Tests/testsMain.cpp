#include "LanternProtocolParserTests.h"
#include <pch.h>

int main(int argc, char *argv[]) {
    LanternProtocolParserTests tests;
    //    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&tests, argc, argv);
}
