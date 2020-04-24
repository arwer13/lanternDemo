#pragma once
#include <QWidget>
#include <memory>

class LanternWidget : public QWidget {
public:
    LanternWidget(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Set lantern color
     *
     * If lantern is on, new color would be set through color animation.
     */
    void setColor(const QColor &color);

    void turnOn();

    void turnOff();

protected:
    static std::unique_ptr<QPixmap> _imageFullSize;
    QPixmap _pixmap;

    //! Color of the lantern
    QColor _color;
    bool _isOn = false;
};
