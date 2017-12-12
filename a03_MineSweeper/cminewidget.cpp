#include<QtWidgets>
#include<QSound>

#include "cminewidget.h"


//#define _MinePaint          //show mine
//#define _AnswerPaint      //show mine and digit

const int CMineWidget::MineCountMin = 10;
const int CMineWidget::BoardWidthMin = 9;
const int CMineWidget::BoardHeightMin = 9;
const int CMineWidget::BoardWidthMax = 30;
const int CMineWidget::BoardHeightMax = 24;
const float CMineWidget::MineRatio = 0.6f;

CMineWidget::CMine::CMine(): m_nValue(0), m_nMineValue(MINE_Unknown)
{}

void CMineWidget::CMine::clear(void)
{
    m_nValue = 0;
    m_nMineValue = MINE_Unknown;
}

bool CMineWidget::CMine::isMine(void)
{
    return (m_nValue <0 );
}


CMineWidget::CMineWidget(QWidget *parent) : QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setLineWidth(3);
    setMidLineWidth(0);

    QPalette p;
    p= palette();

    QColor c;
    CResource::getColor(CResource::Light, c);
    p.setColor(QPalette::Light, c);
    CResource::getColor(CResource::Normal, c);
    p.setColor(QPalette::Window, c);
    CResource::getColor(CResource::Dark, c);
    p.setColor(QPalette::Dark, c);
    setPalette(p);


    m_pMines = new CMine[BoardHeightMax * BoardWidthMax];

    setSound(false);


    setMark(false);
    setLevel(Primary);

    m_bSweepMultiple = false;

}

CMineWidget::~CMineWidget()
{
    delete m_pMines;
}

bool CMineWidget::getMark(void)
{
    return m_bMark;
}

void CMineWidget::setMark(bool bMark)
{
    m_bMark = bMark;
}


bool CMineWidget::getSound(void)
{
    return m_bSound;
}

void CMineWidget::setSound(bool bSound)
{
    m_bSound = bSound;
}

CMineWidget::State CMineWidget::getState(void)
{
    return m_nState;
}

bool CMineWidget::setState(State nState)
{
    Q_ASSERT(nState >= Ready && nState<=Ok);
    if(m_nState == nState)
        return true;

    //check validate
    switch(m_nState)
    {
    case Ready:
        Q_ASSERT(nState == Sweeping);
        break;
    case Sweeping:
        m_timer.stop();
        break;
    case Bombing:
    case Ok:
        Q_ASSERT(nState == Ready);
        break;
    }

    m_nState = nState;

    switch(nState)
    {
    case Sweeping:
        m_timer.start(1000, this);
        break;
    case Ready:
    case Bombing:
    case Ok:
        emit stateChanged(nState);
        break;
    }

    return true;
}


CMineWidget::Level CMineWidget::getLevel()
{
    return m_nLevel;
}
void  CMineWidget::setLevel(Level l)
{
    m_nLevel = l;
    if(m_nLevel != Custom)
    {
        int w, h;

        CResource::getBoardSize(m_nLevel, w, h);
        setBoardSize(w,h);

        int nCount;
        nCount = CResource::getMineCount(m_nLevel);
        setMineCount(nCount);

        newGame();
    }
}

void CMineWidget::getBoardSize(int& width, int& height)
{
    width =  m_nBoardWidth;
    height = m_nBoardHeight;
}

void CMineWidget::setBoardSize(int w, int h)
{
    m_nBoardWidth = qBound(BoardWidthMin, w, BoardWidthMax);
    m_nBoardHeight = qBound(BoardHeightMin, h, BoardHeightMax);

    setFixedSize(m_nBoardWidth*CResource::MineIconWidth +lineWidth()*2
            , m_nBoardHeight * CResource::MineIconHeight + lineWidth()*2);

    updateGeometry();
    update();
}

int CMineWidget::getMineCount(void)
{
    return  m_nMineCount;
}

//should be called after setting boardSize
void CMineWidget::setMineCount(int nCount)
{
    m_nMineCount = qBound(MineCountMin, nCount, static_cast<int>(m_nBoardWidth * m_nBoardHeight * MineRatio));
}

int CMineWidget::getLeftMineCount(void)
{
    return m_nMineLeft;
}


//x and y are 0-based.
CMineWidget::CMine* CMineWidget::getMine(int x, int y)
{
    if(x<0||y<0||x>=m_nBoardWidth||y>=m_nBoardHeight)
        return NULL;

    return &m_pMines[y*m_nBoardWidth + x];
}

bool CMineWidget::isMine(int x, int y)
{
    return getMine(x,y)->isMine();
}

bool CMineWidget::isMarkedOrSweeped(int x, int y)
{
    return (getMine(x,y)->m_nMineValue != MINE_Unknown);
}

void CMineWidget::newGame(void)
{
    int i,j;

    m_nMineLeft = m_nMineCount;
    emit mineLeftChanged(m_nMineLeft);

    m_nElapsedTime = 0;
    emit timeChanged(m_nElapsedTime);

    m_nSweepCount = 0;
    setState(Ready);

    //clear mines
    for(j = 0; j<m_nBoardHeight;j++)
        for(i = 0; i< m_nBoardWidth; i++)
            getMine(i,j)->clear();

    //random mines
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    int count = m_nMineCount;
    CMine* pMine;
    while(count>0)
    {
        i = qrand()%m_nBoardWidth;
        j = qrand()%m_nBoardHeight;
        pMine = getMine(i,j);

        //collision
        if(pMine->isMine())
            continue;

        pMine->m_nValue = -1; //set mine
        count--;
    }

    //update value of mines
    for(j = 0; j<m_nBoardHeight;j++)
        for(i = 0; i< m_nBoardWidth; i++)
        {
            if(!getMine(i,j)->isMine())
                continue;

            for(int m = -1; m<=1; m++)
                for(int n = -1; n<=1; n++)
                {
                    pMine = getMine(i+m,j+n);
                    if(pMine && !pMine->isMine())
                        pMine->m_nValue++;
                }
        }

    update();

#if 0
    qDebug()<<"board:\n";

    QString s;
    QTextStream o(&s);

    for(j = 0; j<m_nBoardHeight;j++)
    {
        for(i = 0; i< m_nBoardWidth; i++)
        {
            o<< getMine(i,j)->m_nValue << " ";
        }
        qDebug()<<s;
        s.clear();
        //o.seek(0);
    }
#endif
}

void CMineWidget::checkGameOver(void)
{

    if(m_nState == Sweeping && m_nMineLeft == 0
            && m_nSweepCount + m_nMineCount == m_nBoardHeight * m_nBoardWidth)
    {   //game over

        //check
        CMine* pMine;
        for(int j = 0; j< m_nBoardHeight; j++)
            for(int i = 0; i< m_nBoardWidth; i++)
            {
                pMine = getMine(i,j);
                if(pMine->isMine())
                    Q_ASSERT(pMine->m_nMineValue = MINE_Mark);
            }

        if(m_bSound)
            CResource::getSound(CResource::SoundOk).play();
        setState(Ok);
    }
}

void CMineWidget::Bomb(void)
{
    CMine* pMine;
    for(int j = 0; j< m_nBoardHeight; j++)
        for(int i = 0; i< m_nBoardWidth; i++)
        {
            pMine = getMine(i,j);
            if(pMine->isMine())
                pMine->m_nMineValue = MINE_Mine;
            else if(pMine->m_nMineValue == MINE_Mark)
                pMine->m_nMineValue = MINE_False;
        }

    if(m_bSound)
        CResource::getSound(CResource::SoundBomb).play();
    setState(Bombing);
}

//internal, only called by SweepOne
void CMineWidget::SweepAdjacencies(int x, int y)
{
    CMine *pMine;
    pMine = getMine(x,y);

    if(pMine && (pMine->m_nMineValue == MINE_Unknown || pMine->m_nMineValue == MINE_Question))
    {
        Q_ASSERT(!pMine->isMine());

        pMine->m_nMineValue = static_cast<Mine>(MINE_0 - pMine->m_nValue);
        m_nSweepCount++;

        if(pMine->m_nValue == 0)
        {
            for(int j = -1; j<=1; j++)
                for(int i = -1; i<=1; i++)
                    SweepAdjacencies(x+i,y+j);
        }

    }
}

void CMineWidget::SweepOne(int x, int y)
{
    if(m_nState == Ready)
        setState(Sweeping);

    if(m_nState != Sweeping)
    {
        qDebug()<<"fail to sweep in a wrong state.";
        return;
    }

    CMine *pMine;
    pMine = getMine(x,y);

    if(pMine && (pMine->m_nMineValue == MINE_Unknown || pMine->m_nMineValue == MINE_Question))
    {

        if(!getMine(x,y)->isMine())
        {        //正确开辟
            SweepAdjacencies(x,y);

            //game over判定
            checkGameOver();
        }
        else
        {        //踩到地雷
            Bomb();
            pMine->m_nMineValue = MINE_Bomb;
        }
    }
}

void CMineWidget::SweepMultiple(int x, int y)
{
    if(m_nState == Ready)
        setState(Sweeping);

    Q_ASSERT(m_nState == Sweeping);

    CMine *pMine;
    pMine = getMine(x,y);


    if(pMine != NULL && pMine->m_nMineValue >= MINE_8 && pMine->m_nMineValue < MINE_0)
    {
        CMine *p;
        int nMarkCount = 0;
        int nUnknown = 0;
        for(int j = -1; j<=1; j++)
            for(int i = -1; i<=1; i++)
            {
                p = getMine(x+i,y+j);
                if(p==NULL)
                    continue;

                if(p->m_nMineValue == MINE_Mark)
                    nMarkCount++;
                else if(p->m_nMineValue == MINE_Unknown
                        || p->m_nMineValue == MINE_Question)
                    nUnknown++;

            }


        if(nUnknown > 0 && nMarkCount == pMine->m_nValue)
        {
            for(int j = -1; j<=1; j++)
                for(int i = -1; i<=1; i++)
                {
                    SweepOne(x+i, y+j);
                }


            checkGameOver();
        }
    }
}

void CMineWidget::Mark(int x, int y)
{
    if(m_nState == Ready)
        setState(Sweeping);

    Q_ASSERT(m_nState == Sweeping);

    CMine *pMine;
    pMine = getMine(x,y);

    if(pMine->m_nMineValue == MINE_Unknown)
    {
        pMine->m_nMineValue = MINE_Mark;
        m_nMineLeft--;
        qDebug()<<"mineLeft="<<m_nMineLeft;
        emit mineLeftChanged(m_nMineLeft);

    }
    else if(pMine->m_nMineValue == MINE_Mark)
    {
        pMine->m_nMineValue = m_bMark? MINE_Question :MINE_Unknown;
        m_nMineLeft++;
        emit mineLeftChanged(m_nMineLeft);
    }
    else if (pMine->m_nMineValue == MINE_Question)
    {
        pMine->m_nMineValue = MINE_Unknown;
    }
}


//return
//  true, pick one mine
//  false, not pick one mine
bool CMineWidget::getRowColAt(QPoint pt, int& x, int& y)
{
    int nLineWidth;
    int i,j;

    nLineWidth = lineWidth();

    if(pt.x() < nLineWidth || pt.x() >= m_nBoardWidth * CResource::MineIconWidth + nLineWidth
            || pt.y() < nLineWidth || pt.y() >= m_nBoardHeight * CResource::MineIconHeight + nLineWidth)
        return false;

    for(i = 1; i<= m_nBoardWidth; i++)
    {
        if(pt.x()< i* CResource::MineIconWidth + nLineWidth)
        {
            break;
        }
    }

    for(j = 1; j<= m_nBoardHeight; j++)
    {
        if(pt.y()< j* CResource::MineIconHeight + nLineWidth)
        {
            break;
        }
    }

    x = i-1;
    y = j-1;

    qDebug()<<x<<y<<m_nBoardWidth<<m_nBoardHeight;
    Q_ASSERT(x >=0 && y>=0 && x < m_nBoardWidth && y < m_nBoardHeight);

    return true;
}

void CMineWidget::updateRect(int x, int y)
{
    int nLineWidth;
    nLineWidth = lineWidth();
    QRect rect(x* CResource::MineIconWidth + nLineWidth
               , y* CResource::MineIconHeight + nLineWidth
               , CResource::MineIconWidth, CResource::MineIconHeight);
    update(rect);

}

void CMineWidget::clearPush(void)
{
    for(int i = 0; i< 9; i++)
    {
        if(m_nPush[i] <0)
            break;

        updateRect(m_nPush[i] & 0xff, m_nPush[i]>>8);
        m_nPush[i] = -1;
    }
}

void CMineWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    CMine *pMine;
    int nLineWidth;
    int i,j;

    nLineWidth = lineWidth();

    for(j = 0;j<m_nBoardHeight; j++)
        for(i = 0; i< m_nBoardWidth; i++)
        {
            pMine = getMine(i,j);

#ifdef _MinePaint
            if(pMine->isMine() && pMine->m_nMineValue != MINE_Bomb && pMine->m_nMineValue != MINE_False)
            {
                painter.drawPixmap(i* CResource::MineIconWidth + nLineWidth
                    , j* CResource::MineIconHeight + nLineWidth
                    , *CResource::MineImage(CResource::MineIconMine));

            }
            else
            {
                painter.drawPixmap(i* CResource::MineIconWidth + nLineWidth
                    , j* CResource::MineIconHeight + nLineWidth
                    , *CResource::MineImage(pMine->m_nMineValue));
            }

#elif defined(_AnswerPaint)       //show mine
            if(pMine->isMine())
            {
                painter.drawPixmap(i* CResource::MineIconWidth + nLineWidth
                    , j* CResource::MineIconHeight + nLineWidth
                    , *CResource::MineImage(CResource::MineIconMine));

            }
            else
            {
                painter.drawPixmap(i* CResource::MineIconWidth + nLineWidth
                    , j* CResource::MineIconHeight + nLineWidth
                    , *CResource::MineImage(CResource::MineIcon0 - pMine->m_nValue));
            }

#else   //normal paint
            painter.drawPixmap(i* CResource::MineIconWidth + nLineWidth
                , j* CResource::MineIconHeight + nLineWidth
                , *CResource::MineImage(pMine->m_nMineValue));
#endif
        }

    //draw pushes
    for(i=0;i<9;i++)
    {
        if(m_nPush[i]<0)
            break;

        int x,y;
        x = m_nPush[i] & 0xff;
        y = m_nPush[i] >> 8;
        if(pMine->m_nMineValue == MINE_Unknown)
        {
            painter.drawPixmap(x* CResource::MineIconWidth + nLineWidth
                , y* CResource::MineIconHeight + nLineWidth
                , *CResource::MineImage(CResource::MineIcon0));
        }
        else if(pMine->m_nMineValue == MINE_Question)
        {
            painter.drawPixmap(x* CResource::MineIconWidth + nLineWidth
                , y* CResource::MineIconHeight + nLineWidth
                , *CResource::MineImage(CResource::MineIconQuestionPush));
        }
    }


    QFrame::paintEvent(e);
}

void CMineWidget::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"press Event\n";

    if(m_nState == Ok || m_nState== Bombing)
        return;

    int x, y;
    if(!getRowColAt(e->pos(), x, y))
        return;

    qDebug()<< "x,y="<<x<<y<<endl;

    CMine *pMine;

    if((e->buttons() & Qt::LeftButton && (e->buttons() & Qt::RightButton))
            || (e->buttons() & Qt::MidButton))
    {
        qDebug()<<"midButton or LR Button\n";

        m_bSweepMultiple = true;


        int i,j, k;
        k = 0;
        for(j = -1; j<=1; j++)
            for(i = -1; i<=1; i++)
            {
                pMine = getMine(x+i, y+j);
                if (!pMine || (pMine->m_nMineValue != MINE_Unknown && pMine->m_nMineValue != MINE_Question))
                    continue;

                m_nPush[k++] = (y+j)<<8 | (x+i);

                updateRect(x+i, y+j);
            }
    }
    else if (e->button() == Qt::LeftButton)
    {
        qDebug()<<"left Button\n";

        pMine = getMine(x, y);

        if(pMine && (pMine->m_nMineValue == MINE_Unknown || pMine->m_nMineValue == MINE_Question))
        {

            m_nPush[0] = y<<8 | x;

            updateRect(x, y);
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        qDebug()<<"right Button\n";
        Mark(x,y);
        updateRect(x,y);
    }

    //QWidget::mousePressEvent(e);
}

void CMineWidget::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug()<<"release Event\n";

    if(m_nState == Ok || m_nState== Bombing)
        return;

    int x, y;
    if(!getRowColAt(e->pos(), x, y))
        return;

    clearPush();

    CMine *pMine;

    if((e->button() & Qt::AllButtons) && m_bSweepMultiple)
    {
        qDebug()<<"sweep mutliple\n";

        m_bSweepMultiple = false;
        SweepMultiple(x, y);
    }

    if (e->button() == Qt::LeftButton)
    {
        qDebug()<<"left Button\n";

        pMine = getMine(x, y);

        if(pMine && (pMine->m_nMineValue == MINE_Unknown || pMine->m_nMineValue == MINE_Question))
        {

            SweepOne(x, y);
        }
    }

    if (e->button() == Qt::RightButton)
    {
        qDebug()<<"right Button\n";
    }

    //if(m_nState == Ok || m_nState== Bombing)
        update();

    //QWidget::mouseReleaseEvent(e);
}

void CMineWidget::mouseMoveEvent(QMouseEvent *e)
{
    qDebug()<<"move Event\n";

    if(m_nState == Ok || m_nState== Bombing)
        return;

    int x, y;
    if(!getRowColAt(e->pos(), x, y))
        return;

    qDebug()<< "x,y="<<x<<y << m_bSweepMultiple;

    clearPush();

    CMine *pMine;

    if((e->buttons() & Qt::LeftButton && (e->buttons() & Qt::RightButton))
            || (e->buttons() & Qt::MidButton))
    {
        qDebug()<<"midButton or LR Button\n";

        //m_bSweepMultiple = true;


        int i,j, k;
        k = 0;
        for(j = -1; j<=1; j++)
            for(i = -1; i<=1; i++)
            {
                pMine = getMine(x+i, y+j);
                if (!pMine || (pMine->m_nMineValue != MINE_Unknown && pMine->m_nMineValue != MINE_Question))
                    continue;

                m_nPush[k++] = (y+j)<<8 | (x+i);

                updateRect(x+i, y+j);
            }
    }
    else if (!m_bSweepMultiple && e->buttons() == Qt::LeftButton)
    {
        qDebug()<<"move";

        pMine = getMine(x, y);

        if(pMine && (pMine->m_nMineValue == MINE_Unknown || pMine->m_nMineValue == MINE_Question))
        {

            m_nPush[0] = y<<8 | x;

            updateRect(x, y);
        }
    }
    qDebug()<<"---";

#if 0
    else if (e->button() == Qt::RightButton)
    {
        qDebug()<<"right Button\n";
        Mark(x,y);
        updateRect(x,y);
    }
#endif

    //QWidget::mouseMoveEvent(e);
}

void CMineWidget::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_timer.timerId())
    {
        m_nElapsedTime++;
        if(m_bSound)
            CResource::getSound(CResource::SoundTick).play();

        emit timeChanged(m_nElapsedTime);

    }
    else
        QWidget::timerEvent(e);
}
