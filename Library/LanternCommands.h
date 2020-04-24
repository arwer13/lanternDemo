#pragma once
#include <QByteArray>
#include <QColor>
#include <memory>

//! Interface and a factory for lantern commands
class LanternCommand {
public:
    typedef unsigned char Type;

    static const constexpr Type typeInvalid = 0x00;
    static const constexpr Type typeOn = 0x12;
    static const constexpr Type typeOff = 0x13;
    static const constexpr Type typeColor = 0x20;

    //! Create a specific command or return nullptr if command type is unknown
    static std::shared_ptr<LanternCommand> createCommand(Type commandType);

    Type type() const;

    //! To debug string representation
    virtual QString toString() const = 0;

    //! Set command data (if it's already set then override)
    virtual void setData(const QByteArray &data) = 0;

protected:
    Type _type = typeInvalid;
};

//! Command to turn lantern on
class LanternCommandOn : public LanternCommand {
public:
    LanternCommandOn();
    virtual void setData(const QByteArray &data) override;
    virtual QString toString() const override {
        return "ON";
    }
};

//! Command to turn lantern off
class LanternCommandOff : public LanternCommand {
public:
    LanternCommandOff();
    virtual void setData(const QByteArray &data) override;
    virtual QString toString() const override {
        return "OFF";
    }
};

//! Command to set lantern's color
class LanternCommandColor : public LanternCommand {
public:
    LanternCommandColor();
    virtual void setData(const QByteArray &data) override;
    virtual QString toString() const override {
        return "COLOR " + _color.name();
    }
    QColor color() const;

protected:
    QColor _color;
};


/**
 * @brief A class for commands of type unknown to the factory
 *
 * Keeps command type and command payload, but doesn't parse it.
 */
class LanternUnknownCommand : public LanternCommand {
public:
    LanternUnknownCommand();
    virtual void setData(const QByteArray &data) override;
    virtual QString toString() const override {
        return "<UNKNOWN> with data size " + QString::number(_data.size());
    }

protected:
    QByteArray _data;
};
