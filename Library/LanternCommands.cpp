#include "LanternCommands.h"
#include <QDebug>

std::shared_ptr<LanternCommand> LanternCommand::createCommand(LanternCommand::Type commandType) {
    std::shared_ptr<LanternCommand> result;
    if (typeOn == commandType) {
        result.reset(new LanternCommandOn());
    } else if (typeOff == commandType) {
        result.reset(new LanternCommandOff());
    } else if (typeColor == commandType) {
        result.reset(new LanternCommandColor());
    } else {
        result.reset(new LanternUnknownCommand());
    }
    return result;
}

LanternCommand::Type LanternCommand::type() const {
    return _type;
}

LanternCommandOn::LanternCommandOn() {
    _type = LanternCommand::typeOn;
}

void LanternCommandOn::setData(const QByteArray &data) {
    Q_UNUSED(data)
    qWarning() << "An attempt to set data to LanternCommandOn which doesn't accept any data";
}

LanternCommandOff::LanternCommandOff() {
    _type = LanternCommand::typeOff;
}

void LanternCommandOff::setData(const QByteArray &data) {
    Q_UNUSED(data)
    qWarning() << "An attempt to set data to LanternCommandOff which doesn't accept any data";
}

LanternCommandColor::LanternCommandColor() {
    _type = LanternCommand::typeColor;
}

void LanternCommandColor::setData(const QByteArray &data) {
    assert(data.size() == 3);
    if (data.size() != 3) {
        qWarning() << "Attempt to set invalid data to LanternCommandColor" << data;
        return;
    }
    // TODO
    const auto red = (unsigned char)data[0];
    const auto green = (unsigned char)data[1];
    const auto blue = (unsigned char)data[2];
    _color.setRgb(red, green, blue);
}

QColor LanternCommandColor::color() const {
    return _color;
}

LanternUnknownCommand::LanternUnknownCommand() {
}

void LanternUnknownCommand::setData(const QByteArray &data) {
    _data = data;
}
