QT      += core gui xml widgets
CONFIG  += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/src/basictoken.cpp \
    $$PWD/bdf/bdfdata.cpp \
    $$PWD/bdf/bdfglyph.cpp \
    $$PWD/dialogs/aboutdlg.cpp \
    $$PWD/dialogs/casinfodlg.cpp \
    $$PWD/dialogs/preferencesdlg.cpp \
    $$PWD/src/cass80handler.cpp \
    $$PWD/src/cass80main.cpp \
    $$PWD/src/cass80xml.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/util.cpp \
    $$PWD/z80/listing2xml.cpp \
    $$PWD/z80/z80dasm.cpp \
    $$PWD/z80/z80def.cpp \
    $$PWD/z80/z80defs.cpp \
    $$PWD/z80/z80token.cpp

HEADERS += \
    $$PWD/include/basictoken.h \
    $$PWD/bdf/bdfdata.h \
    $$PWD/bdf/bdfglyph.h \
    $$PWD/dialogs/aboutdlg.h \
    $$PWD/dialogs/casinfodlg.h \
    $$PWD/dialogs/preferencesdlg.h \
    $$PWD/include/cass80handler.h \
    $$PWD/include/cass80main.h \
    $$PWD/include/cass80xml.h \
    $$PWD/include/constants.h \
    $$PWD/include/util.h \
    $$PWD/z80/listing2xml.h \
    $$PWD/z80/z80dasm.h \
    $$PWD/z80/z80def.h \
    $$PWD/z80/z80defs.h \
    $$PWD/z80/z80token.h

FORMS += \
    $$PWD/dialogs/aboutdlg.ui \
    $$PWD/dialogs/casinfodlg.ui \
    $$PWD/dialogs/preferencesdlg.ui \
    cass80main.ui
    cass80main.ui \

win32: LIBS += -lzlib
unix: LIBS += -lz

TRANSLATIONS += \
    cass80_de_DE.ts

INCLUDEPATH += $$PWD/bdf
INCLUDEPATH += $$PWD/dialogs
INCLUDEPATH += $$PWD/z80
INCLUDEPATH += $$PWD/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cass80.qrc

DISTFILES += \
    cass80_de_DE.qm \
    image/application-exit.png \
    image/document-open.png \
    image/document-save-as.png \
    image/document-save.png \
    image/preferences.png \
    resources/cgenie.rom \
    resources/cgenie1.fnt \
    resources/cgenie-2008-08-10.txt
