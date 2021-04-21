#ifndef BALL_H
#define BALL_H

#include <QObject>

class ball : public QObject
{
    Q_OBJECT
public:
    explicit ball(QObject *parent = nullptr);

    int posX;
    int posY;

signals:

public slots:
};

#endif // BALL_H
