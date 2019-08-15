#ifndef FONTWIDGET_H
#define FONTWIDGET_H

#include <QWidget>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

class CFontWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CFontWidget(QWidget *parent = nullptr);
    ~CFontWidget();

signals:

public slots:
    void slotLoadFontFile(QString fontFileName);
    void drawText(QPainter &painter, QString &text, int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    FT_Library m_library;
    FT_Face m_face;

    int m_pointSize;
    QRect m_glyphRect;

    QString m_text;
};

#endif // FONTWIDGET_H
