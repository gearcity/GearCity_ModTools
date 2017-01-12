#-------------------------------------------------
#
# Project created by QtCreator 2016-12-26T10:39:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += xml

TARGET = GearCity_ModToolSuite
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    MapDataTool.cpp \
    CityEditor.cpp \
    CityData.cpp \
    QLongLongValidator.cpp \
    AIEditor.cpp \
    TurnEventsEditor.cpp \
    LocalizationManager.cpp \
    ComponentsManager.cpp \
    AIManager.cpp \
    TurnEventsData.cpp \
    TurnEventTreeGrower.cpp

HEADERS  += MainWindow.h \
    MapDataTool.h \
    BaseContainerStorage.h \
    CityEditor.h \
    CityData.h \
    QLongLongValidator.h \
    AIEditor.h \
    TurnEventsEditor.h \
    TurnEventsData.h \
    LocalizationManager.h \
    ComponentsManager.h \
    AIManager.h \
    TurnEventTreeGrower.h

FORMS    += MainWindow.ui \
    MapDataTool.ui \
    CityEditor.ui \
    AIEditor.ui \
    TurnEventsEditor.ui
