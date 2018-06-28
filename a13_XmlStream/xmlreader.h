#ifndef XMLREADER_H
#define XMLREADER_H

#include <QXmlStreamReader>

class XmlReader
{
public:
    XmlReader();

    bool read(QIODevice *device);
    QString errorString() const;

private:
    void readXBEL();
    void readTitle();
    void readSeparator();
    void readFolder();
    void readBookmark();

    QXmlStreamReader xml;

};

#endif // XMLREADER_H
