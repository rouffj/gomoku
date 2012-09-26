#ifndef STONE_H
#define STONE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>

#define STATIC_CENTER_OFFSET    4

class ErrorStone : public QObject, public QGraphicsEllipseItem
{
   Q_OBJECT
   Q_PROPERTY(QRectF rect READ rect WRITE setRect)
   Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
};

class Stone : public QObject, public QGraphicsPixmapItem
 {
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScaleAndOffset)
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity)

    QPointF     _initialOffset;
    qreal       _coef;
    qreal       _relativeCenterOffset;
public:

    void        setScaleAndOffset(qreal factor)
    {
        qreal   offset = this->_relativeCenterOffset - (this->_coef * factor);

        QGraphicsPixmapItem::setScale(factor);
        QGraphicsItem::setPos(this->_initialOffset.x() + offset,
                              this->_initialOffset.y() + offset);
    }

    void        initScaleAndOffset(qreal x, qreal y, qreal endScale, qreal relativeCenterOffset)
    {
        this->_relativeCenterOffset = relativeCenterOffset;
        this->_initialOffset = QPointF(x, y);
        this->_coef = this->_relativeCenterOffset / endScale;
    }
 };

#endif // STONE_H
