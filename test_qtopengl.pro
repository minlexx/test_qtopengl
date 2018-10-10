QT += core gui widgets
TARGET = test_qtopengl
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
        main.cpp \
        widget.cpp

HEADERS += \
        widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
