#ifndef CRESOURCE_H
#define CRESOURCE_H

#include <QColor>

class QPixmap;
class QSound;

class CResource
{
public:
    enum {
        DigitNum  = 12,
        DigitHeight = 23,
        DigitWidth = 13,

        DigitSub = 0,
        DigitNull = 1,
        Digit9 = 2,
        Digit8 = 3,
        Digit7 = 4,
        Digit6 = 5,
        Digit5 = 6,
        Digit4 = 7,
        Digit3 = 8,
        Digit2 = 9,
        Digit1 = 10,
        Digit0 = 11,

        EmotionNum  = 5,
        EmotionHeight = 24,
        EmotionWidth = 24,

        EmotionPush = 0,
        EmotionGlass = 1,
        EmotionSad = 2,
        EmotionOh = 3,
        EmotionSmile = 4,

        MineIconNum = 16,
        MineIconHeight = 16,
        MineIconWidth = 16,

        MineIconUnknown         = 0,
        MineIconMark            = 1,
        MineIconQuestion        = 2,
        MineIconBomb            = 3,
        MineIconFalse           = 4,
        MineIconMine            = 5,
        MineIconQuestionPush    = 6,
        MineIcon8               = 7,
        MineIcon7               = 8,
        MineIcon6               = 9,
        MineIcon5               = 10,
        MineIcon4               = 11,
        MineIcon3               = 12,
        MineIcon2               = 13,
        MineIcon1               = 14,
        MineIcon0               = 15,
        //MineIconPush            = MineIcon0,

        ColorNum = 3,
        Light = 0,
        Normal = 1,
        Dark = 2,

        PrimaryWidth = 9,
        PrimaryHeight = 9,
        PrimaryMineCount = 10,

        IntermediateWidth = 16,
        IntermediateHeight = 16,
        IntermediateMineCount = 40,

        AdvanceWidth = 30,
        AdvanceHeight = 16,
        AdvanceMineCount =99,

        SoundTick = 0,
        SoundOk = 1,
        SoundBomb = 2,

    };

public:
    static void LoadResource(void);
    static void UnloadResource(void);
    static QPixmap* MineImage(int nId);
    static QPixmap* EmotionImage(int nId);
    static QPixmap* DigitImage(int nId);
    static void getColor(int nColorId, QColor& color);
    static void getBoardSize(int nLevel, int& width, int& height);
    static int getMineCount(int nLevel);
    static QSound& getSound(int nSoundId);

private:
    CResource();    //i
    ~CResource();

public:

    static bool m_bLoaded;
    static QPixmap *m_MineIcons[MineIconNum];
    static QPixmap *m_Emotions[EmotionNum];
    static QPixmap *m_Digits[DigitNum];

    static QSound* m_tickSound;
    static QSound* m_okSound;
    static QSound* m_bombSound;

};

#endif // CRESOURCE_H
