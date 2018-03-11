#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T16:03:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = b01_GraphicLine
TEMPLATE = app

CONFIG+=precompile_header
PRECOMPILED_HEADER=preheader.h

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

//QT_MESSAGE_PATTERN = "%{file}_%{line} %{function}:   %{message}"

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    draftscene.cpp \
    draftview.cpp \
    shapeitem.cpp \
    lineitem.cpp \
    shapeselection.cpp \
    propeditor.cpp

HEADERS += \
    preheader.h \
    mainwindow.h \
    draftscene.h \
    draftview.h \
    shapeitem.h \
    lineitem.h \
    shapeselection.h \
    propeditor.h

RESOURCES += \
    graphicline.qrc

!include($$[QT_INSTALL_PREFIX]/../Src/qttools/src/shared/qtpropertybrowser/qtpropertybrowser.pri) {
    error("Can't find qtpropertybrowser.pri.")
}
