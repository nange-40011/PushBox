#include "qpushbox.h"
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <dataconfig.h>
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLabel>
#include <QPoint>
#include <QPen>

QPushBox::QPushBox(QWidget *parent) :
    QWidget(parent)
{
    //推箱子界面设计
    setFixedSize(760, 560); //设置固定大小
    setWindowTitle("推箱子"); //设置标题
    setWindowIcon(QIcon(":/images/down.png")); //设置图标

    //关卡地图数据初始化
    initLevel(minLevel);

    //切换关卡按钮及其他按钮设计：
    //上一关卡按钮
    QPushButton * lastBtn = new QPushButton("上一关卡");
    lastBtn->setParent(this);
    lastBtn->resize(100,60);
    lastBtn->setStyleSheet("background-color:orange");
    lastBtn->setFont(QFont("华文新魏", 12));
    lastBtn->move(560 +50, 40);
    //监听点击信号
    connect(lastBtn, &QPushButton::clicked,[=](){
        if(levelindex!=minLevel)
        {
            initLevel(levelindex-1);
            update();
        }
    });

    //下一关卡按钮
    QPushButton * nextBtn = new QPushButton("下一关卡");
    nextBtn->setParent(this);
    nextBtn->resize(100,60);
    nextBtn->setStyleSheet("background-color:orange");
    nextBtn->setFont(QFont("华文新魏", 12));
    nextBtn->move(560 +50, 40+140);
    //监听点击信号
    connect(nextBtn, &QPushButton::clicked,[=](){
        if(levelindex!=maxLevel)
        {
            initLevel(levelindex+1);
            update();
        }
    });

    //重新开始本关按钮
    QPushButton * restartBtn = new QPushButton("重玩本关");
    restartBtn->setParent(this);
    restartBtn->resize(100,60);
    restartBtn->setStyleSheet("background-color:orange");
    restartBtn->setFont(QFont("华文新魏", 12));
    restartBtn->move(560 +50, 40+140*2);
    //监听点击信号
    connect(restartBtn, &QPushButton::clicked,[=](){
        initLevel(levelindex);
        update();
    });

    //游戏说明按钮
    QPushButton * noteBtn = new QPushButton("游戏说明");
    noteBtn->setParent(this);
    noteBtn->resize(100,60);
    noteBtn->setStyleSheet("background-color:orange");
    noteBtn->setFont(QFont("华文新魏", 12));
    noteBtn->move(560 +50, 40+140*3);
    //监听点击信号
    connect(noteBtn, &QPushButton::clicked,[=](){
        QString str = "用键盘上的上、下、左、右键移动小人，把箱子全部推到小球的位置即可过关。箱子只可向前推，不能往后拉，并且小人一次只能推动一个箱子";
        QMessageBox::information(this,"游戏说明",str);
    });

}

QPushBox::~QPushBox()
{
    qDebug() << "调用了析构函数";
}

void QPushBox::initLevel(int level) // 使用level参数初始化对应关卡数据
{
    //判断是否越界
    if(level < minLevel) level = minLevel;
    if(level > maxLevel) level = maxLevel;
    levelindex = level;
    this->person_position_x = -1;
    this->person_position_y = -1;
    //获胜标志
    isWin = false;
    //人物朝向为下
    hero_stat = DIR_DOWN;
    //移动次数置0
    moveTimes = 0;
    //读取关卡数据
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {            
            gameArray[i][j] = config.mData[level][j][i];
            posArray[i][j] = config.mData[level][j][i];
        }
    }
}

bool QPushBox::tryGo(QPoint p1, QPoint p2)
{
    if(posArray[p1.x()][p1.y()]==1) //前面是墙
    {
        return false;
    }
    if(posArray[p1.x()][p1.y()]==3||posArray[p1.x()][p1.y()]==5)
    {   //前面是箱子 还需判断箱子前面是否有障碍物，箱子或墙
        if(posArray[p2.x()][p2.y()]==1||posArray[p2.x()][p2.y()]==3||posArray[p2.x()][p2.y()]==5)
        {
            return false;
        }
        //若没有 人物前面的箱子前进一步
        posArray[p2.x()][p2.y()]=3;
    }
    // 如果都没判断成功 人物前进一步
    posArray[p1.x()][p1.y()]=4;
    // 如果人物移动了 原来的位置如何显示
    int cur = posArray[this->person_position_x][this->person_position_y];
    if(cur!=2) //不是陷阱
    {
        if(cur==5) //可能是5 既有箱子也有陷阱
        {
            cur=2; //移开还是陷阱
        }
        else
        {
            cur=0; //移开是地板
        }
    }
    //重置小人位置的地图参数
    posArray[this->person_position_x][this->person_position_y]=cur;
    //人物移动了 更新坐标值
    this->person_position_x = p1.x();
    this->person_position_y = p1.y();
//    qDebug() << this->person_position_x << " , " <<this->person_position_y;
    //移动了 返回true
    return true;
}

bool QPushBox::GameIsWin()
{
    for(int i = 0 ; i < 16; i++)
    {
        for(int j = 0 ; j < 16 ; j++)
        {
            if((this->gameArray[i][j]==2&&this->posArray[i][j]!=3) || (this->gameArray[i][j]==5&&this->posArray[i][j]!=3)) //原来是陷阱的地方现在是箱子 原来是陷阱和箱子的地方现在有箱子
            {
                return false; //不满足则返回false
            }
        }
    }
    return true; //每个陷阱都有箱子对应 返回true
}

void QPushBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/images/block.gif");
    // 画背景图
    int d = 35;
    for(int i = 0; i < 16; i++) //画出背景地板
    {
        for(int j = 0; j < 16; j++)
        {
            painter.drawPixmap(d*i, d*j, pix);
        }
    }
    // 画其他的图案
    QString str;
    for(int i = 0 ; i < 16; i++)
    {
        for(int j = 0 ; j < 16 ; j++)
        {
            if(this->posArray[i][j] == 1) //显示墙
            {
                str = ":/images/wall.png";
                pix.load(str);
                painter.drawPixmap(d*i, d*j -11, pix);
            }
            else if(this->posArray[i][j] == 2) //显示陷阱
            {
                str = ":/images/ball.png";
                pix.load(str);
                painter.drawPixmap(d*i +3, d*j +5, pix);
            }
            else if(this->posArray[i][j] == 3) //显示箱子
            {
                str = ":/images/box.png";
                pix.load(str);
                painter.drawPixmap(d*i, d*j -10, pix);
            }
            else if(this->posArray[i][j] == 4) //显示人物
            {
                this->person_position_x = i;
                this->person_position_y = j;
                //画人物
                if(hero_stat == DIR_UP){
                    str = ":/images/up.png";
                }else if(hero_stat == DIR_DOWN){
                    str = ":/images/down.png";
                }else if(hero_stat == DIR_LEFT){
                    str = ":/images/left.png";
                }else if(hero_stat == DIR_RIGHT){
                    str = ":/images/right.png";
                }
                pix.load(str);
                painter.drawPixmap(d*i -8, d*j -27, pix);
            }
            else if(this->posArray[i][j] == 5) //判断特殊情况 箱子在陷阱中
            {
                str = ":/images/box.png";
                pix.load(str);
                painter.drawPixmap(d*i, d*j -10, pix);
            }
            else if(this->gameArray[i][j]==2||this->gameArray[i][j]==5)
            {
                str = ":/images/ball.png";
                pix.load(str);
                painter.drawPixmap(d*i +3, d*j +5, pix);
            }
        }
    }
    QString levelMsg = QString("第 %1 关").arg(levelindex);
    painter.drawText(QRect(35,35,105,35),levelMsg);
    QString moveMsg = QString("移动次数： %1").arg(moveTimes);
    painter.drawText(QRect(35,70,105,35),moveMsg);
}

void QPushBox::keyReleaseEvent(QKeyEvent *event)
{
    // 如果游戏胜利了 则不反应键盘事件
    if(isWin)
    {
        return QWidget::keyPressEvent(event);
    }
    // 如果是处理两个普通按键，得避免自动重复
    int i = this->person_position_x;
    int j = this->person_position_y;
    // 人物移动逻辑判断
    QPoint p1; //移动方向走一格处
    QPoint p2; //移动方向走两格处
    if(event->key() == Qt::Key_Up) //点击了向上
    {
        hero_stat = DIR_UP;
        p1 = QPoint(i,j-1);
        p2 = QPoint(i,j-2);
    }
    else if(event->key() == Qt::Key_Left) //点击了向左
    {
        hero_stat = DIR_LEFT;
        p1 = QPoint(i-1,j);
        p2 = QPoint(i-2,j);
    }
    else if(event->key() == Qt::Key_Down) //点击了向下
    {
        hero_stat = DIR_DOWN;
        p1 = QPoint(i,j+1);
        p2 = QPoint(i,j+2);
    }
    else if(event->key() == Qt::Key_Right) //点击了向右
    {
        hero_stat = DIR_RIGHT;
        p1 = QPoint(i+1,j);
        p2 = QPoint(i+2,j);
    }
    else return QWidget::keyPressEvent(event);
    if(tryGo(p1,p2))
    {
        moveTimes ++;
    }
    // 判断游戏当前状况是否获胜
    if(GameIsWin())
    {
        isWin = true;
        QMessageBox::information(this,"提示信息","恭喜过关！");
        initLevel(levelindex+1);
    }
    // 重绘地图
    update();
}

