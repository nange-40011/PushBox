#ifndef BOX_H
#define BOX_H

#include <QObject>

class box : public QObject
{
    Q_OBJECT
public:
    explicit box(QObject *parent = nullptr);

    int posX;
    int posY;

signals:

public slots:
};

#endif // BOX_H
