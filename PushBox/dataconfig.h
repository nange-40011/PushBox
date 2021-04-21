#ifndef DATACONFIG_H
#define DATACONFIG_H
#define minLevel 1 //最小关卡数
#define maxLevel 15 //最大关卡数
#define DIR_UP 1
#define DIR_LEFT 2
#define DIR_DOWN 3
#define DIR_RIGHT 4

#include <QObject>
#include <QMap>
#include <QVector>

class DataConfig : public QObject
{
    Q_OBJECT
public:
    explicit DataConfig(QObject *parent = nullptr);

    // 用来存放关卡地图的数据
    QMap<int, QVector< QVector<int> > >mData;

signals:

public slots:
};

#endif // DATACONFIG_H
