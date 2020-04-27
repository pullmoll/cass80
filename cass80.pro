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
    $$PWD/basictoken.cpp \
    $$PWD/bdfdata.cpp \
    $$PWD/cass80handler.cpp \
    $$PWD/cass80infodlg.cpp \
    $$PWD/cass80main.cpp \
    $$PWD/cass80xml.cpp \
    $$PWD/main.cpp \
    $$PWD/z80/z80dasm.cpp \
    $$PWD/z80/z80def.cpp \
    $$PWD/z80/z80defs.cpp \
    $$PWD/z80/z80token.cpp \
    bdfglyph.cpp

HEADERS += \
    $$PWD/basictoken.h \
    $$PWD/bdfdata.h \
    $$PWD/cass80handler.h \
    $$PWD/cass80infodlg.h \
    $$PWD/cass80main.h \
    $$PWD/cass80xml.h \
    $$PWD/constants.h \
    $$PWD/z80/z80dasm.h \
    $$PWD/z80/z80def.h \
    $$PWD/z80/z80defs.h \
    $$PWD/z80/z80token.h \
    bdfglyph.h

FORMS += \
    cass80infodlg.ui \
    cass80main.ui

TRANSLATIONS += \
    cas_de_DE.ts

INCLUDEPATH += $$PWD/z80

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    cass80.qrc

DISTFILES += \
    cgenie.rom
