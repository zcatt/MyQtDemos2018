#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QLCDNumber>
#include <QTime>
#include <QScrollArea>

class QTabWidget;
class QDialogButtonBox;
class QLabel;
class QShortcut;
class QCheckBox;
class QSpinBox;

class CStopWatch;

class QSettings;
class CAdditionTableWidget;
class CMultiplicationTableWidget;
class CAdditionWidget;
class CSubtractionWidget;
class CMultiplicationWidget;
class CDivisionWidget;
class CConfigWidget;

#define CFG_EnablePreLabel     0

#define VALUE(k,v)  mainwin->m_settings->value(k,v)
#define SETVALUE(k,v)  mainwin->m_settings->setValue(k,v)

enum
{
    MaxExerciseCount = 50,
    MaxAdd = 999
};

class CMainDialog : public QDialog
{
    Q_OBJECT

public:
    enum
    {
        TabIndex_AdditionTable,
        TabIndex_MultiplicationTable,
        TabIndex_Addition,
        TabIndex_Subtraction,
        TabIndex_Multiplication,
        TabIndex_Division,
        TabIndex_Config,
    };
public:
    CMainDialog(QWidget *parent = 0);
    ~CMainDialog();

    void makeTabWidget(void);
public slots:
    void slotNextAction(void);

public:
    QShortcut *m_nextShortcut;
    QSettings *m_settings;
    
    QTabWidget *m_tabWidget;
    QDialogButtonBox *m_buttonBox;

    CAdditionTableWidget* m_addTableWidget;
    CMultiplicationTableWidget* m_mulTableWidget;
    CAdditionWidget* m_addWidget;
    CSubtractionWidget* m_subWidget;
    CMultiplicationWidget* m_mulWidget;
    CDivisionWidget* m_divWidget;
    CConfigWidget* m_cfgWidget;
};

class CExItem
{
public:
    enum
    {
        Op_Add,
        Op_Sub,
        Op_Multiple,
        Op_Division
    };

    bool equal(CExItem& ei);
    QString repr(bool bUnordered, bool bResult);

public:
    int op;
    int add1;
    int add2;
};

class CAdditionTableWidget : public QWidget
{
    Q_OBJECT
public:
    CAdditionTableWidget(QWidget *parent = 0);
    ~CAdditionTableWidget();

    void readSettings(void);
    void writeSettings(void);

    bool generateExercise(void);
    void slotStartNext();
    void slotStop();

public:
    bool m_bUnordered;
    bool m_addend1[10];      //true，表示该值入选
    bool m_addend2[10];
    int m_exCount;            //总题数 1~50
    int m_cur;                //当前题号
    CExItem m_exItems[MaxExerciseCount];


    CStopWatch *m_stopWatch;
    QDialogButtonBox *m_buttonBox;
    QLabel *m_preLabel;
    QLabel *m_nextLabel;
    QLabel *m_curLabel;
    QLabel *m_progressingLabel;
};

class CMultiplicationTableWidget : public QWidget
{
    Q_OBJECT
public:
    CMultiplicationTableWidget(QWidget *parent = 0);
    ~CMultiplicationTableWidget();

    void readSettings(void);
    void writeSettings(void);

    bool generateExercise(void);
    void slotStartNext();
    void slotStop();

public:
    bool m_bUnordered;
    bool m_addend1[10];      //true，表示该值入选
    bool m_addend2[10];
    int m_exCount;            //总题数 1~50
    int m_cur;                //当前题号
    CExItem m_exItems[MaxExerciseCount];

    CStopWatch *m_stopWatch;
    QDialogButtonBox *m_buttonBox;
    QLabel *m_preLabel;
    QLabel *m_nextLabel;
    QLabel *m_curLabel;
    QLabel *m_progressingLabel;

};
class CAdditionWidget : public QWidget
{
    Q_OBJECT
public:
    CAdditionWidget(QWidget *parent = 0);
    ~CAdditionWidget();

    void readSettings(void);
    void writeSettings(void);

    bool generateExercise(void);
    void slotStartNext();
    void slotStop();

public:
    int m_min1;
    int m_max1;
    int m_min2;
    int m_max2;
    int m_exCount;            //总题数 1~50
    int m_cur;                //当前题号
    CExItem m_exItems[MaxExerciseCount];

    CStopWatch *m_stopWatch;
    QDialogButtonBox *m_buttonBox;
    QLabel *m_preLabel;
    QLabel *m_nextLabel;
    QLabel *m_curLabel;
    QLabel *m_progressingLabel;


};
class CSubtractionWidget : public QWidget
{
    Q_OBJECT
public:
    CSubtractionWidget(QWidget *parent = 0);
    ~CSubtractionWidget();

    void readSettings(void);
    void writeSettings(void);

    bool generateExercise(void);
    void slotStartNext();
    void slotStop();

public:
    int m_min1;     //减数
    int m_max1;
    int m_min2;     //差
    int m_max2;
    int m_exCount;            //总题数 1~50
    int m_cur;                //当前题号
    CExItem m_exItems[MaxExerciseCount];

    CStopWatch *m_stopWatch;
    QDialogButtonBox *m_buttonBox;
    QLabel *m_preLabel;
    QLabel *m_nextLabel;
    QLabel *m_curLabel;
    QLabel *m_progressingLabel;


};
class CMultiplicationWidget : public QWidget
{
    Q_OBJECT
public:
    CMultiplicationWidget(QWidget *parent = 0);
    ~CMultiplicationWidget();

    void readSettings(void);
    void writeSettings(void);


    bool generateExercise(void);
    void slotStartNext();
    void slotStop();


public:
    int m_min1;
    int m_max1;
    int m_min2;
    int m_max2;
    int m_exCount;            //总题数 1~50
    int m_cur;                //当前题号
    CExItem m_exItems[MaxExerciseCount];

    CStopWatch *m_stopWatch;
    QDialogButtonBox *m_buttonBox;
    QLabel *m_preLabel;
    QLabel *m_nextLabel;
    QLabel *m_curLabel;
    QLabel *m_progressingLabel;
};



class CDivisionWidget : public QWidget
{
    Q_OBJECT
public:
    CDivisionWidget(QWidget *parent = 0);
    ~CDivisionWidget();

    void readSettings(void);
    void writeSettings(void);

    bool generateExercise(void);
    void slotStartNext();
    void slotStop();

public:
    int m_min1;     //减数
    int m_max1;
    int m_min2;     //差
    int m_max2;
    int m_exCount;            //总题数 1~50
    int m_cur;                //当前题号
    CExItem m_exItems[MaxExerciseCount];

    CStopWatch *m_stopWatch;
    QDialogButtonBox *m_buttonBox;
    QLabel *m_preLabel;
    QLabel *m_nextLabel;
    QLabel *m_curLabel;
    QLabel *m_progressingLabel;
};


class CConfigWidget : public QWidget
{
    Q_OBJECT
public:
    CConfigWidget(QWidget *parent = 0);
    ~CConfigWidget();

public slots:
    void refreshAddTbl(void);
    void refreshMulTbl(void);
    void refreshAdd(void);
    void refreshSub(void);
    void refreshMul(void);
    void refreshDiv(void);

public:
    //part of addition table
    QCheckBox *m_addtblUnordered;
    QSpinBox *m_addtblExCount;
    QCheckBox *m_addtblAdd1[9];
    QCheckBox *m_addtblAdd2[9];

    //part of multi table
    QCheckBox *m_multblUnordered;
    QSpinBox *m_multblExCount;
    QCheckBox *m_multblAdd1[9];
    QCheckBox *m_multblAdd2[9];

    //part of addition
    QSpinBox *m_addMin1;
    QSpinBox *m_addMax1;
    QSpinBox *m_addMin2;
    QSpinBox *m_addMax2;
    QSpinBox *m_addExCount;

    //part of subtract
    QSpinBox *m_subMin1;
    QSpinBox *m_subMax1;
    QSpinBox *m_remMin2;
    QSpinBox *m_remMax2;
    QSpinBox *m_subExCount;

    //part of multiplication
    QSpinBox *m_mulMin1;
    QSpinBox *m_mulMax1;
    QSpinBox *m_mulMin2;
    QSpinBox *m_mulMax2;
    QSpinBox *m_mulExCount;

    //part of division
    QSpinBox *m_divMin1;
    QSpinBox *m_divMax1;
    QSpinBox *m_divMin2;
    QSpinBox *m_divMax2;
    QSpinBox *m_divExCount;
};


class CStopWatch : public QLCDNumber
{
    Q_OBJECT

public:
    CStopWatch(QWidget *parent = 0);
    void clear();
    bool start(void);
    bool stop(void);

    void slotShowTime();

public:
    bool m_bStarted;
    QTimer *m_timer;
    QTime m_baseTime;
    int m_ms;
};



#endif // MAINDIALOG_H
