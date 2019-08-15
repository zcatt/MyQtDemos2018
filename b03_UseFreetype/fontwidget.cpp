#include <QtWidgets>

#include "fontwidget.h"

CFontWidget::CFontWidget(QWidget *parent) : QWidget(parent)
{
    FT_Error error = FT_Err_Ok;

    m_face = 0;
    m_library = 0;
    m_pointSize = 32;

    m_text = "hello";

    // For simplicity, the error handling is very rudimentary.
    error = FT_Init_FreeType(&m_library);
    Q_ASSERT(!error);

#if 0
    //slotLoadFontFile("../b03_UseFreetype/cour.ttf");
    slotLoadFontFile("../b03_UseFreetype/HanSerifSC.otf");
#endif
}

CFontWidget::~CFontWidget()
{
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}


void CFontWidget::slotLoadFontFile(QString fontFileName)
{
    FT_Done_Face(m_face);

    if(!fontFileName.isEmpty())
    {
        FT_Error error = FT_Err_Ok;

        error = FT_New_Face(m_library,
                            fontFileName.toLatin1().constData(),
                            0,
                            &m_face);

        qDebug()<<"errorCode="<<error;
        Q_ASSERT(!error);

        error = FT_Set_Char_Size(m_face,
                                 0,
                                 m_pointSize <<6 ,    // * 64
                                 physicalDpiX(),
                                 physicalDpiY());

        Q_ASSERT(!error);

        error = FT_Select_Charmap(m_face,FT_ENCODING_UNICODE);
        Q_ASSERT(!error);

    }
    update();
}

static void graySpans(int y,
                      int count,
                      const FT_Span_ *spans,
                      void *user)
{
  QPainter *painter = (QPainter *)user;
  y = -y;

  for (int i = 0; i < count; i++)
  {
    const FT_Span span = spans[i];
    qreal opacity = qreal(span.coverage) / 255.0;

    painter->setOpacity(opacity);

    if (span.len > 1)
      painter->drawLine(span.x, y, span.x + span.len - 1, y);
    else
      painter->drawPoint(span.x, y);
  }
}

void CFontWidget::drawText(QPainter &painter, QString &text, int x, int y)
{
    painter.translate(x, y);

    const QVector<uint> s = text.toUcs4();

    for(uint charCode : s)
    {
        FT_UInt gindex = FT_Get_Char_Index(m_face, charCode);

        FT_Error error = FT_Err_Ok;
        error = FT_Load_Char(m_face, charCode, FT_LOAD_RENDER );
        if ( error )
          continue;                 /* ignore errors */

        error = FT_Load_Glyph(m_face,
                              gindex,
                              FT_LOAD_DEFAULT);

        if (error)
            continue;

        FT_Pos left = m_face->glyph->metrics.horiBearingX;
        FT_Pos right = left + m_face->glyph->metrics.width;
        FT_Pos top = m_face->glyph->metrics.horiBearingY;
        FT_Pos bottom = top - m_face->glyph->metrics.height;

        FT_Raster_Params params;

        params.target = 0;
        params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA | FT_RASTER_FLAG_CLIP;
        params.user = &painter;
        params.gray_spans = &graySpans;
        params.black_spans = 0;
        params.bit_set = 0;
        params.bit_test = 0;
        params.clip_box.xMin = -32768L;
        params.clip_box.yMin = -32768L;
        params.clip_box.xMax =  32767L;
        params.clip_box.yMax =  32767L;


        FT_Outline* outline = &m_face->glyph->outline;

        FT_Outline_Render(m_library,
                          outline,
                          &params);

#define TRUNC(x) ((x) >> 6)

        m_glyphRect = QRect(QPoint(TRUNC(left),
                                 -TRUNC(top) + 1),
                          QSize(TRUNC(right - left) + 1,
                                TRUNC(top - bottom) + 1));

        FT_Vector &advance = m_face->glyph->advance;

        qDebug()<<"dx="<<(advance.x>>6)<<",dy="<<(advance.y>>6);
        painter.translate(advance.x>>6, 0);

    }
}

void CFontWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if(m_face && !m_text.isEmpty())
    {
        QPainter painter(this);

        painter.setPen(Qt::black);

        drawText(painter, m_text, 100,100);
    }
}

