
QDOC = qdoc
DOC_TARGET = simpleQDoc
MAKE_DOCS = $$PWD/doc.qdocconf
OUTPUT_DIR = --outputdir $$shell_quote($$OUT_PWD/html)
# DOC_INDEXES += -indexdir $$shell_quote($$[QT_INSTALL_DOCS/get])

QHELPGENERATOR = qhelpgenerator


message($$PWD)
htmlDocs.commands = $$QDOC $$MAKE_DOCS $$OUTPUT_DIR
#htmlDocs.depends += $$MAKE_DOCS

qch_docs.commands = $$QHELPGENERATOR $$shell_quote($$OUT_PWD/html/$${DOC_TARGET}.qhp) -o $$shell_quote($$OUT_PWD/html/$${DOC_TARGET}.qch)
qch_docs.depends = htmlDocs

docs.depends = qch_docs

QMAKE_EXTRA_TARGETS += htmlDocs docs qch_docs
PRE_TARGETDEPS += htmlDocs docs

DISTFILES += \
    $$PWD/doc.qdocconf \
    $$PWD/doc.pri \
    $$PWD/doc.qdocconf \
    $$PWD/src/b02_SimpleQDoc.qdoc \
    $$PWD/src/gettingStart.qdoc \
    $$PWD/doc.pri \
    $$PWD/images/girl.jpeg \
    $$PWD/doc.qdocconf \
    $$PWD/src/b02_SimpleQDoc.qdoc \
    $$PWD/src/gettingStart.qdoc \
    $$PWD/doc.pri \
    $$PWD/images/girl.jpeg \
    $$PWD/config/style/simple.css \
    $$PWD/doc.qdocconf \
    $$PWD/src/b02_SimpleQDoc.qdoc \
    $$PWD/src/gettingStart.qdoc \
    $$PWD/doc.pri \
    $$PWD/images/girl.jpeg \
    $$PWD/config/style/offline.css \
    $$PWD/config/style/simple.css \
    $$PWD/doc.qdocconf \
    $$PWD/src/b02_SimpleQDoc.qdoc \
    $$PWD/src/gettingStart.qdoc

