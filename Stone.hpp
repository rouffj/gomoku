#ifndef STONE_H
#define STONE_H

#include <QObject>
#include <QGraphicsEllipseItem>

class Stone : public QObject, public QGraphicsEllipseItem
 {
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity)
 };

#endif // STONE_H
