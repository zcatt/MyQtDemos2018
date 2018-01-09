#include <QtWidgets>

#include "widget.h"

Widget::Widget()
    : QWidget(0), addInterface(0), m_lineEditA(0), m_lineEditB(0), m_resultLabel(0), m_calcBtn(0)
{
    createGUI();
    setWindowTitle(tr("a0b_AddPlugin application"));

    if(!loadPlugin())
    {
        QMessageBox::information(this, "Error", "Could not load the ADD plugin.");
        m_calcBtn->setEnabled(false);
    }
}

Widget::~Widget()
{
}

void Widget::createGUI(void)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *hlayout;

    hlayout = new QHBoxLayout;

    QLabel *label;

    //first row:   x + x = ?

    m_lineEditA = new QLineEdit;
    m_lineEditA->setValidator(new QIntValidator);
    hlayout->addWidget(m_lineEditA);

    label = new QLabel("+");
    hlayout->addWidget(label);

    m_lineEditB = new QLineEdit;
    m_lineEditB->setValidator(new QIntValidator);
    hlayout->addWidget(m_lineEditB);

    label = new QLabel("=");
    hlayout->addWidget(label);

    m_resultLabel = new QLabel;
    m_resultLabel->setFrameStyle(QFrame::Box | QFrame::Plain);
    m_resultLabel->setMinimumWidth(100);
    hlayout->addWidget(m_resultLabel);

    layout->addLayout(hlayout);


    //second row:   calc btn
    hlayout = new QHBoxLayout;

    m_calcBtn = new QPushButton(tr("Calc"));
    hlayout->addWidget(m_calcBtn,0, Qt::AlignRight);
    layout->addLayout(hlayout);

    //layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    connect(m_calcBtn, &QPushButton::clicked, this, &Widget::add);
}

bool Widget::loadPlugin(void)
{
    QDir pluginsDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cdUp();              //cd up from app target dir
    pluginsDir.cd("plugins");       // see AddPlugin.pro/DESTDIR

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            addInterface = qobject_cast<CAddInterface *>(plugin);
            if (addInterface)
                return true;
        }
    }

    return false;

}

void Widget::add(void)
{
    if(addInterface)
    {
        int sum = addInterface->add(m_lineEditA->text().toInt(), m_lineEditB->text().toInt());
        m_resultLabel->setNum(sum);
    }
}
