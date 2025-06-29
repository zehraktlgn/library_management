QT       += core gui sql
#ZEHRA KUTLUGÜN
#22100011002
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kitapislemleri.cpp \
    main.cpp \
    mainwindow.cpp \
    oduncalmaislemleri.cpp \
    oduncteslimetmeislemleri.cpp \
    uyeislemleri.cpp

HEADERS += \
    kitapislemleri.h \
    mainwindow.h \
    oduncalmaislemleri.h \
    oduncteslimetmeislemleri.h \
    uyeislemleri.h

FORMS += \
    kitapislemleri.ui \
    mainwindow.ui \
    oduncalmaislemleri.ui \
    oduncteslimetmeislemleri.ui \
    uyeislemleri.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
