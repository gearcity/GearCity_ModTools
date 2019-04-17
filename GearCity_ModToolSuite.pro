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

DEFINES += STEAMSUPPORT




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
    TurnEventTreeGrower.cpp \
    MaterialMakerWindow.cpp \
    ModEditor.cpp \
    ReviewsEditor.cpp \
    NameListEditor.cpp \
    NewspaperArticleEditor.cpp \
    ComponentsEditor.cpp \
    RacingSeriesEditor.cpp \
    ContractInformationEditor.cpp \
    PlayerLogoEditor.cpp \
    MusicEditor.cpp \
    DYKEditor.cpp \
    PreMadeVehicleEditor.cpp \
    ScenarioEditor.cpp \
    SteamWorksUploader.cpp \
    ContractManager.cpp


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
    TurnEventTreeGrower.h \
    MaterialMakerWindow.h \
    ModEditor.h \
    OSXHelper.h \
    ReviewsEditor.h \
    NameListEditor.h \
    NewspaperArticleEditor.h \
    ComponentsEditor.h \
    RacingSeriesEditor.h \
    ContractInformationEditor.h \
    PlayerLogoEditor.h \
    MusicEditor.h \
    DYKEditor.h \
    PreMadeVehicleEditor.h \
    ScenarioEditor.h \
    SteamWorksUploader.h \
    ContractManager.h

FORMS    += MainWindow.ui \
    MapDataTool.ui \
    CityEditor.ui \
    AIEditor.ui \
    TurnEventsEditor.ui \
    MaterialMakerWindow.ui \
    ModEditor.ui \
    ReviewsEditor.ui \
    NameListEditor.ui \
    NewspaperArticleEditor.ui \
    ComponentsEditor.ui \
    RacingSeriesEditor.ui \
    ContractInformationEditor.ui \
    PlayerLogoEditor.ui \
    MusicEditor.ui \
    DYKEditor.ui \
    PreMadeVehicleEditor.ui \
    ScenarioEditor.ui \
    SteamWorksUploader.ui



macx{
    LIBS += $$PWD/Steamworks_142/sdk/redistributable_bin/osx32/libsteam_api142.dylib
    LIBS += -framework CoreFoundation
    SOURCES +=  OSXHelper.cpp
    QMAKE_LFLAGS += -F/System/Library/Frameworks/

    INCLUDEPATH += $$PWD/Steamworks_142/sdk/public/

} else {

    unix{ #doesn't like Linux for some reason...

        LIBS += $$PWD/Steamworks_142/sdk/redistributable_bin/linux32/libsteam_api.so
        INCLUDEPATH += $$PWD/Steamworks_142/sdk/public/
    }

    win32{
        LIBS += $$PWD/Steamworks_142/sdk/redistributable_bin/steam_api.lib
        INCLUDEPATH += $$PWD/Steamworks_142/sdk/public/
    }

}





