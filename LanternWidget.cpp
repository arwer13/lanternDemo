#include "LanternWidget.h"
#include <QPainter>
#include <QPixmap>
#include <QVariantAnimation>

std::unique_ptr<QPixmap> LanternWidget::_imageFullSize;

LanternWidget::LanternWidget(QWidget *parent)
        : QWidget(parent) {
    if (!_imageFullSize) {
        _imageFullSize.reset(new QPixmap(":/lantern2.png"));
        assert(!_imageFullSize->isNull());
    }
    _color = QColor(Qt::transparent);
}

void LanternWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    // TODO: Don't scale the pixmap on every paintEvent
    _pixmapCache = _imageFullSize->scaled(width(), height(), Qt::AspectRatioMode::KeepAspectRatio);
    const auto shift
        = 0.5 * QPointF(width() - _pixmapCache.width(), height() - _pixmapCache.height());
    const auto pixmapRect = QRectF(shift, _pixmapCache.size());
    if (_isOn) {
        painter.fillRect(pixmapRect, QBrush(_color));
    } else {
        painter.fillRect(pixmapRect, QBrush(_color, Qt::BrushStyle::Dense7Pattern));
    }
    painter.drawPixmap(pixmapRect.toRect(), _pixmapCache);
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
