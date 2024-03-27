QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#QT += openglwidgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../lib_c/s21_backend.c \
    main.cpp \
    opengl.cpp \
    viewer.cpp

HEADERS += \
    ../lib_c/s21_backend.h \
    opengl.h \
    viewer.h

FORMS += \
    viewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(Qgif/QtGifImage/src/gifimage/qtgifimage.pri)

DISTFILES += \
    ../Configs/crypt_custom.txt \
    ../Configs/crypt_default.txt \
    ../Configs/custom.conf \
    ../Configs/default.conf


