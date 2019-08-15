DEFINES += FT2_BUILD_LIBRARY
#DEFINES += FT_CONFIG_OPTION_SYSTEM_ZLIB
#include(../zlib_dependency.pri)

#DEFINES += FT_CONFIG_OPTION_USE_PNG
#include($$OUT_PWD/../../gui/qtgui-config.pri)
#QMAKE_USE_PRIVATE += libpng

#DEFINES += TT_CONFIG_OPTION_SUBPIXEL_HINTING

LIBS += -lz

INCLUDEPATH += $$PWD/include

HEADERS += \
            $$INCLUDEPATH/freetype/config/ftconfig.h \
            $$INCLUDEPATH/freetype/config/ftheader.h \
            $$INCLUDEPATH/freetype/config/ftmodule.h \
            $$INCLUDEPATH/freetype/config/ftoption.h \
            $$INCLUDEPATH/freetype/config/ftstdlib.h

win32 {
    SOURCES += $$PWD/src/base/ftsystem.c
} else {
    SOURCES += $$PWD/builds/unix/ftsystem.c
    INCLUDEPATH += $$PWD/builds/unix
}

#base components
SOURCES += \
            $$PWD/src/base/ftinit.c \
            $$PWD/src/base/ftdebug.c    \
            $$PWD/src/base/ftbase.c  \
            $$PWD/src/base/ftbbox.c  \
            $$PWD/src/base/ftglyph.c  \
            $$PWD/src/base/ftbdf.c  \
            $$PWD/src/base/ftbitmap.c  \
            $$PWD/src/base/ftmm.c  \
            $$PWD/src/base/ftotval.c   \
            $$PWD/src/base/ftlcdfil.c   \
            $$PWD/src/base/ftsynth.c  \
            $$PWD/src/base/fttype1.c  \
            $$PWD/src/base/ftfntfmt.c  \
            $$PWD/src/base/ftwinfnt.c  \
            $$PWD/src/base/ftcid.c

#font drivers
SOURCES += \
            $$PWD/src/bdf/bdf.c \
            $$PWD/src/cff/cff.c \
            $$PWD/src/sfnt/sfnt.c \
            $$PWD/src/truetype/truetype.c \
            $$PWD/src/type1/type1.c \
            $$PWD/src/type42/type42.c \
            $$PWD/src/winfonts/winfnt.c

#rasterizers
SOURCES += \
            $$PWD/src/raster/raster.c \
            $$PWD/src/smooth/smooth.c

#auxiliary modules
SOURCES += \
            $$PWD/src/autofit/autofit.c \
            $$PWD/src/cache/ftcache.c \
            $$PWD/src/gzip/ftgzip.c \
            $$PWD/src/lzw/ftlzw.c \
            $$PWD/src/otvalid/otvalid.c \
            $$PWD/src/otvalid/otvbase.c \
            $$PWD/src/otvalid/otvcommn.c \
            $$PWD/src/otvalid/otvgdef.c \
            $$PWD/src/otvalid/otvgpos.c \
            $$PWD/src/otvalid/otvgsub.c \
            $$PWD/src/otvalid/otvjstf.c \
            $$PWD/src/otvalid/otvmod.c \
            $$PWD/src/psaux/psaux.c \
            $$PWD/src/pshinter/pshinter.c \
            $$PWD/src/psnames/psnames.c

