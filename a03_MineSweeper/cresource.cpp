#include <QtDebug>
#include <QPixmap>
#include <QSound>

#include "cresource.h"
#include "cminewidget.h"

//images
static const char* MineIconsRsc=":/images/mines.bmp";
static const char* EmotionsRsc=":/images/emotions.bmp";
static const char* DigitsRsc=":/images/digits.bmp";
//sounds
static const char* TickRsc = ":/sounds/tick.wav";
static const char* OkRsc = ":/sounds/ok.wav";
static const char* BombRsc = ":/sounds/bomb.wav";



bool CResource::m_bLoaded = false;
#if 0
QPixmap CResource::m_MineIcons[CResource::MineIconNum];
QPixmap CResource::m_Emotions[CResource::EmotionNum];
QPixmap CResource::m_Digits[CResource::DigitNum];
#else
QPixmap *CResource::m_MineIcons[CResource::MineIconNum];
QPixmap *CResource::m_Emotions[CResource::EmotionNum];
QPixmap *CResource::m_Digits[CResource::DigitNum];
#endif

QSound* CResource::m_tickSound;
QSound* CResource::m_okSound;
QSound* CResource::m_bombSound;

//QColor CResource::m_Color[ColorNum];

void CResource::LoadResource(void)
{
    if(m_bLoaded)
        return;

    bool bOK;
    QPixmap pm;

    bOK = pm.load(QLatin1String(MineIconsRsc));
    qDebug()<< (bOK?"Mine icons loaded ok.":"Mine icons loaded fail.");


    int i;

    for(i = 0; i<MineIconNum; i++)
        m_MineIcons[i] = new QPixmap(pm.copy(0, i*MineIconHeight, MineIconWidth, MineIconHeight));



    bOK = pm.load(QLatin1String(EmotionsRsc));
    qDebug()<< (bOK?"Emotions loaded ok.":"Emotions loaded fail.");

    for(i = 0; i<EmotionNum; i++)
        m_Emotions[i] = new QPixmap(pm.copy(0, i*EmotionHeight, EmotionWidth, EmotionHeight));



    bOK = pm.load(QLatin1String(DigitsRsc));
    qDebug()<< (bOK?"Digits loaded ok.":"Digits loaded fail.");

    for(i = 0; i<DigitNum; i++)
        m_Digits[i] = new QPixmap(pm.copy(0, i*DigitHeight, DigitWidth, DigitHeight));


    m_tickSound = new QSound(TickRsc);
    m_okSound = new QSound(OkRsc);
    m_bombSound = new QSound(BombRsc);

    m_bLoaded = true;
}

void CResource:: UnloadResource(void)
{
    if(!m_bLoaded)
        return;

    int i;
    for (i = 0 ; i < MineIconNum; i++)
        if(m_MineIcons[i]!= NULL)
            delete m_MineIcons[i];

    for (i = 0 ; i < EmotionNum; i++)
        if(m_Emotions[i]!= NULL)
            delete m_Emotions[i];

    for (i = 0 ; i < DigitNum; i++)
        if(m_Digits[i]!= NULL)
            delete m_Digits[i];

    delete m_tickSound;
    delete m_okSound;
    delete m_bombSound;

    m_bLoaded = false;
}

QPixmap* CResource::MineImage(int nId)
{
    Q_ASSERT(nId>=MineIconUnknown && nId<=MineIcon0);
    return m_MineIcons[nId];
}

QPixmap* CResource::EmotionImage(int nId)
{
    Q_ASSERT(nId>=EmotionPush && nId<=EmotionSmile);
    return m_Emotions[nId];
}

QPixmap* CResource::DigitImage(int nId)
{
    Q_ASSERT(nId>=DigitSub && nId<=Digit0);
    return m_Digits[nId];
}

void CResource::getColor(int nColorId, QColor& color)
{
    switch(nColorId)
    {
    case Light:
        color.setRgb(255,255,255);
        break;
    case Dark:
        color.setRgb(128,128,128);
        break;
    case Normal:
    default:
        color.setRgb(192,192,192);
        break;
    }
}

void CResource::getBoardSize(int nLevel, int& width, int& height)
{
    Q_ASSERT(nLevel >= CMineWidget::Primary && nLevel <= CMineWidget::Advance);

    switch(nLevel)
    {
    case CMineWidget::Primary:
        width = PrimaryWidth;
        height = PrimaryHeight;
        break;
    case CMineWidget::Intermediate:
        width = IntermediateWidth;
        height = IntermediateHeight;
        break;
    case CMineWidget::Advance:
        width = AdvanceWidth;
        height = AdvanceHeight;
        break;
    }
}

int CResource::getMineCount(int nLevel)
{
    Q_ASSERT(nLevel >= CMineWidget::Primary && nLevel <= CMineWidget::Advance);

    int res;
    switch(nLevel)
    {
    case CMineWidget::Primary:
        res = PrimaryMineCount;
        break;
    case CMineWidget::Intermediate:
        res = IntermediateMineCount;
        break;
    case CMineWidget::Advance:
        res = AdvanceMineCount;
        break;
    }
    return res;
}


QSound& CResource::getSound(int nSoundId)
{
    if(nSoundId == SoundTick)
        return *m_tickSound;
    else if (nSoundId == SoundOk)
        return *m_okSound;
    else //if (nSoundId == SoundBomb)
        return *m_bombSound;
}


