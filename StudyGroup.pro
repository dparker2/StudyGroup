#-------------------------------------------------
#
# Project created by QtCreator 2017-09-17T19:38:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StudyGroup
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    user_account_check.cpp \
    server.cpp \
    groupwidget.cpp \
    whiteboard.cpp \
    whiteboard_p.cpp \
    flashcard.cpp \
    startpage.cpp \
    joingrouppage.cpp \
    creategrouppage.cpp \
    settingspage.cpp \
    sgtcpsocket.cpp \
    sgwidget.cpp \
    groupmainpage.cpp \
    deck.cpp \
    fcfront.cpp \
    fcback.cpp

HEADERS += \
        mainwindow.h \
    user_account_check.h \
    server.h \
    groupwidget.h \
    whiteboard.h \
    whiteboard_p.h\
    flashcard.h \
    startpage.h \
    joingrouppage.h \
    creategrouppage.h \
    settingspage.h \
    sgtcpsocket.h \
    sgwidget.h \
    groupmainpage.h \
    deck.h \
    fcfront.h \
    fcback.h

FORMS += \
        mainwindow.ui \
    groupwidget.ui\
    flashcard.ui \
    startpage.ui \
    joingrouppage.ui \
    creategrouppage.ui \
    settingspage.ui \
    deck.ui \
    fcfront.ui \
    fcback.ui

RESOURCES += \
    resource.qrc
