#include "xmlreader.h"

XmlReader::XmlReader()
{
}

bool XmlReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == "xbel" && xml.attributes().value("version") == "1.0")
            readXBEL();
        else
            xml.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
    }

    return !xml.error();
}

QString XmlReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

void XmlReader::readXBEL()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "xbel");

    while (xml.readNextStartElement()) {
        if (xml.name() == "folder")
            readFolder();
        else if (xml.name() == "bookmark")
            readBookmark();
        else if (xml.name() == "separator")
            readSeparator();
        else
            xml.skipCurrentElement();
    }
}

void XmlReader::readTitle()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "title");

    QString title = xml.readElementText();
}

void XmlReader::readSeparator()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "separator");
}

void XmlReader::readFolder()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "folder");


    while (xml.readNextStartElement()) {
        if (xml.name() == "title")
            readTitle();
        else if (xml.name() == "folder")
            readFolder();
        else if (xml.name() == "bookmark")
            readBookmark();
        else if (xml.name() == "separator")
            readSeparator();
        else
            xml.skipCurrentElement();
    }
}

void XmlReader::readBookmark()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == "bookmark");

    while (xml.readNextStartElement()) {
        if (xml.name() == "title")
            readTitle();
        else
            xml.skipCurrentElement();
    }
}


