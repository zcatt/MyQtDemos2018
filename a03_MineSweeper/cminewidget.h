#ifndef CMINEWIDGET_H
#define CMINEWIDGET_H

#include <QFrame>
#include <QBasicTimer>

#include "cresource.h"

class QSound;

class CMineWidget : public QFrame
{
    Q_OBJECT
public:
    enum State
    {
        Ready,
        Sweeping,
        Bombing,
        Ok
    };

    enum Level
    {
        Primary = 0,
        Intermediate = 1,
        Advance = 2,
        Custom = 3
    };

    enum Mine
    {
        MINE_Unknown            = CResource::MineIconUnknown,//0
        MINE_Mark               = CResource::MineIconMark,//1,
        MINE_Question           = CResource::MineIconQuestion,//2,
        MINE_Bomb               = CResource::MineIconBomb,//3,
        MINE_False              = CResource::MineIconFalse,//4,
        MINE_Mine               = CResource::MineIconMine,//5,
        //MINE_QuestionPush               = CResource::MineIconQuestionPush,//6,
        MINE_8                          = CResource::MineIcon8,//7,
        MINE_7                          = CResource::MineIcon7,//8,
        MINE_6                          = CResource::MineIcon6,//9,
        MINE_5                          = CResource::MineIcon5,//10,
        MINE_4                          = CResource::MineIcon4,//11,
        MINE_3                          = CResource::MineIcon3,//12,
        MINE_2                          = CResource::MineIcon2,//13,
        MINE_1                          = CResource::MineIcon1,//14,
        MINE_0                          = CResource::MineIcon0,//15,
    };

    class CMine
    {
    public:
        CMine();
        void clear();
        bool isMine(void);
    public:
        int m_nValue;       //actual value
                            //0~8, number of adjacent mines; -1, mine
        Mine m_nMineValue;  //visual value
    };

public:
    CMineWidget(QWidget *parent = nullptr);
    ~CMineWidget();

    bool getSound(void);
    void setSound(bool bSound);

    bool getMark(void);
    void setMark(bool bMark);

    State getState(void);
    bool setState(State nState);


    Level getLevel();

    void getBoardSize(int& width, int& height);

    int getMineCount(void);
    void setMineCount(int nCount);
    int getLeftMineCount(void);


    CMine* getMine(int x, int y);
    bool isMine(int x, int y);
    bool isMarkedOrSweeped(int x, int y);

    void SweepAdjacencies(int x, int y);
    void SweepOne(int x, int y);
    void SweepMultiple(int x, int y);
    void Mark(int x, int y);

    void checkGameOver(void);
    void Bomb(void);

    bool getRowColAt(QPoint pt, int& x, int& y);
    void updateRect(int x, int y);
    void clearPush(void);


protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void timerEvent(QTimerEvent *) override;



signals:
    void mineLeftChanged(int nLeft);
    void timeChanged(int nElapsedTime);
    void stateChanged(int nState);

public slots:
    void newGame(void);
    void setLevel(Level l);
    void setBoardSize(int w, int h);

public:
    static const int MineCountMin;
    static const int BoardWidthMin;
    static const int BoardHeightMin;
    static const int BoardWidthMax;
    static const int BoardHeightMax;
    static const float MineRatio;


    CMine *m_pMines;
    int m_nPush[9];

    State m_nState;
    Level m_nLevel;
    int m_nBoardWidth;
    int m_nBoardHeight;

    int m_nMineCount;
    int m_nMineLeft;
    int m_nSweepCount;
    int m_nElapsedTime;

    bool m_bSound;
    bool m_bMark;
    bool m_bSweepMultiple;

    QBasicTimer m_timer;
};

#endif // CMINEWIDGET_H
