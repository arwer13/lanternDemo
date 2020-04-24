#pragma once
#include <QByteArray>
#include <QColor>
#include <memory>

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

class LanternCommandOn : public LanternCommand {
public:
    LanternCommandOn();
    virtual void setData(const QByteArray &data) override;
    virtual QString toString() const override {
        return "ON";
    }
};

class LanternCommandOff : public LanternCommand {
public:
    LanternCommandOff();
    virtual void setData(const QByteArray &data) override;
    virtual QString toString() const override {
        return "OFF";
    }
};

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
