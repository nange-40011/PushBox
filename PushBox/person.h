#ifndef PERSON_H
#define PERSON_H

#include <QObject>

class person : public QObject
{
    Q_OBJECT
public:
    explicit person(QObject *parent = nullptr);

    //人物坐标
    int posX;
    int posY;

signals:

public slots:
};

#endif // PERSON_H
