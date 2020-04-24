#include "LanternWidget.h"
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QRegion>
#include <QVariantAnimation>

std::unique_ptr<QPixmap> LanternWidget::_imageFullSize;
std::unique_ptr<QImage> LanternWidget::_imageMaskFullSize;

LanternWidget::LanternWidget(QWidget *parent)
        : QWidget(parent) {
    if (!_imageFullSize) {
        _imageFullSize.reset(new QPixmap(":/lantern.png"));
        assert(!_imageFullSize->isNull());
    }
    if (!_imageMaskFullSize) {
        _imageMaskFullSize.reset(new QImage(":/lanternLightMask.png"));
        assert(!_imageMaskFullSize->isNull());
    }
    _color = QColor(Qt::transparent);
}

void LanternWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    _updatePaintCacheIfNeeded(size());

    QPainter painter(this);
    painter.setClipRegion(_paintCache.lightRegion);
    if (_isOn) {
        painter.fillRect(_paintCache.rect, QBrush(_color));
    } else {
        painter.fillRect(_paintCache.rect, QBrush(_color, Qt::BrushStyle::Dense7Pattern));
    }
    painter.setClipRect(_paintCache.rect);
    painter.drawPixmap(_paintCache.rect.toRect(), _paintCache.lantern);
}

void LanternWidget::setColor(const QColor &color) {
    if (_isOn) {
        auto animation = new QVariantAnimation(this);
        animation->setStartValue(_color);
        animation->setEndValue(color);
        animation->setDuration(500);
        connect(animation, &QVariantAnimation::valueChanged, [this](const QVariant &v) {
            this->_color = v.value<QColor>();
            this->update();
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    _color = color;
}

void LanternWidget::turnOn() {
    setColor(_color);
    _isOn = true;
    setEnabled(true);
    update();
}

void LanternWidget::turnOff() {
    _isOn = false;
    setEnabled(false);
    update();
}

void LanternWidget::_updatePaintCacheIfNeeded(const QSize newSize) {
    if (newSize == _paintCache.widgetSize) {
        return;
    }
    auto &lantern = _paintCache.lantern;

    lantern = _imageFullSize->scaled(size(), Qt::AspectRatioMode::KeepAspectRatio);
    const auto shift = 0.5 * QPointF(width() - lantern.width(), height() - lantern.height());
    _paintCache.rect = QRectF(shift, lantern.size());
    auto lightMask = QBitmap(QPixmap::fromImage(
        _imageMaskFullSize->scaled(lantern.size(), Qt::AspectRatioMode::KeepAspectRatio)
            .createAlphaMask()));
    _paintCache.lightRegion = QRegion(lightMask);
    _paintCache.lightRegion.translate(shift.toPoint());
}
