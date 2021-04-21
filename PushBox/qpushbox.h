#ifndef QPUSHBOX_H
#define QPUSHBOX_H

#include <QWidget>
#include <QLabel>
#include <QPoint>
#include <dataconfig.h>

class QPushBox : public QWidget
{
    Q_OBJECT

public:
    explicit QPushBox(QWidget *parent = 0);
    ~QPushBox();

    //重写paintEvent事件 画背景图
    void paintEvent(QPaintEvent *);
    //按键事件 响应键盘 人物移动
    void keyReleaseEvent(QKeyEvent *event);
    //初始化关卡数据
    void initLevel(int level);
    //判断人物是否能走 p1 p2分别是往前一格 两格
    bool tryGo(QPoint p1, QPoint p2);
    //判断游戏是否获胜
    bool GameIsWin();

    int levelindex; //成员属性 记录当前所在的关卡
    int person_position_x ; //初始时人物的x坐标
    int person_position_y ; //初始时人物的y坐标
    int hero_stat; //人物的朝向
    bool isWin; // 游戏是否胜利标志
    int moveTimes; //游戏移动次数
    DataConfig config; //关卡数据


    //二维数组 维护每个关卡的数据
    int gameArray[16][16]; //用来存储最初的地图数据
    int posArray[16][16];  //用来存储当前的地图数据

};

#endif // QPUSHBOX_H
