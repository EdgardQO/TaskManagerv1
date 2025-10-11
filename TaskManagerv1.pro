QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    detalleswindow.cpp \
    historialwindow.cpp \
    historymanager.cpp \
    historymodel.cpp \
    main.cpp \
    mainwindow.cpp \
    procesoswindow.cpp \
    processtablemodel.cpp \
    servicioswindow.cpp \
    startupmodel.cpp \
    startupwindow.cpp \
    usuarioswindow.cpp

HEADERS += \
    detalleswindow.h \
    historialwindow.h \
    historymanager.h \
    historymodel.h \
    mainwindow.h \
    procesoswindow.h \
    processtablemodel.h \
    servicioswindow.h \
    startupmodel.h \
    startupwindow.h \
    usuarioswindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    TaskManagerv1_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
