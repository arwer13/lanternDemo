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
    static std::unique_ptr<QImage> _imageMaskFullSize;

    //! Cache of
    struct PaintCache {
        //! Image of the lantern
        QPixmap lantern;

        //! Region of light
        QRegion lightRegion;

        //! Size of widget for which the cache is calculated
        QSize widgetSize;

        //! Pixmap rect
        QRectF rect;

    } _paintCache;

    //! Update the cache if sizes differ
    void _updatePaintCacheIfNeeded(const QSize newSize);

    //! Color of the lantern
    QColor _color;
    bool _isOn = false;
};
