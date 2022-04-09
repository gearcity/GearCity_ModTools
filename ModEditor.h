#ifndef MODEDITOR_H
#define MODEDITOR_H

#include <QtWidgets/QWidget>
#include "BaseContainerStorage.h"
#include <QString>
#include <QMap>

namespace Ui {
class ModEditor;
}

class ModEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ModEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~ModEditor();
    
private slots:
    void on_button_SelectNameList_clicked();
    void on_button_SelectNewspaper_clicked();
    void on_button_SelectReviews_clicked();
    void on_button_SelectComponents_clicked();
    void on_button_SelectComponentPop_clicked();
    void on_button_AddMap_clicked();
    void on_button_RemoveMap_clicked();
    void on_list_MapsToMod_currentRowChanged(int currentRow);
    void on_button_SelectAIFile_clicked();
    void on_button_AddAiMap_clicked();
    void on_button_RemoveAIMap_clicked();
    void on_table_AIModMaps_cellClicked(int row, int column);
    void on_table_CitiesModMap_cellClicked(int row, int column);
    void on_button_SelectCityFile_clicked();
    void on_button_addCityFileMap_clicked();
    void on_button_RemoveCityFileMap_clicked();
    void on_table_TurnEventsMap_cellClicked(int row, int column);
    void on_button_SelectTurnEventsFile_clicked();
    void on_button_AddTurnEventsMap_clicked();
    void on_button_RemoveTurnEventsMap_clicked();
    void on_button_NewMod_clicked();
    void on_button_SaveMod_clicked();
    void on_button_OpenMod_clicked();
    void on_button_ExportMod_clicked();

    void on_button_SelectRacingSeries_clicked();

    void on_button_SelectContractInfo_clicked();

    void on_button_ReturnToMain_clicked();

    void on_Button_NounListEditor_clicked();

    void on_Button_NewsPaperEditor_clicked();

    void on_button_ReviewsEditor_clicked();

    void on_button_ComponentsEditor_clicked();

    void on_button_AIEditor_clicked();

    void on_button_CitiesEditor_clicked();

    void on_button_turnEventsEditor_clicked();


    void on_checkbox_Components_stateChanged(int arg1);

    void on_button_SelectComponentImages_clicked();

    void on_button_SelectVehicleArtwork_clicked();

    void on_button_SelectMiscArtwork_clicked();


    void on_checkBox_Music_stateChanged(int arg1);

    void on_button_MusicFileSelector_clicked();

    void on_button_MusicFolderSelector_clicked();

    void on_button_PlayerLogoFileSelector_clicked();


    void on_button_PremadeFolderSelector_clicked();

    void on_button_PremadeFileSelector_clicked();

private:
    class aiModMaps
    {
        public:
        QString fileName;
        QString filePath;
        QString map;
        int numberOfAI;
        QString description;

    };

    class citiesModMaps
    {
        public:
        QString fileName;
        QString filePath;
        QString map;
        int cityYear;

    };

    class turnEventsModMaps
    {
        public:
        QString fileName;
        QString filePath;
        QString map;
    };

    Ui::ModEditor *ui;
    widgetContainerStorage cp_wsc;

    //Fill Mod Tables
    void fillAITableInfo();
    void fillCityTableInfo();
    void fillTurnTableInfo();
    void openFile(QString openFileName);
    bool checkSaveErrors();
    void exportMod(QString parentFolder);

    //save functions
    void saveForEditor(QString saveFileName);

    //Copy Files in Folder
    void copyFolderFiles(QString sourcePath, QString targetPath);

    //File Names
    QString nameListFile;
    QString newpaperArticlesFile;
    QString reviewsFile;
    QString componentsFile;
    QString componentsPopFile;
    QString racingFile;
    QString contractFile;
    QString aiFileName;
    QString aiFilePath;
    QString cityFileName;
    QString cityFilePath;
    QString turnFileName;
    QString turnFilePath;

    QString musicFilePath;
    QString musicFolderPath;
    QString plPath;

    QString premadeCarFolderPath;
    QString premadePath;


    QString componentImagesFile;
    QString vehicleArtworkFile;
    QString MiscArtworkFile;


    //Map Keys
    QString selectedTableAIMapKey;
    QString selectedTableCityMapKey;
    QString selectedTableTurnMapKey;

    //List of Maps this Mod Works On
    QStringList modMapList;


    QMap<QString,aiModMaps> aiMapMap;
    QMap<QString,citiesModMaps> cityMapMap;
    QMap<QString,turnEventsModMaps> turnMapMap;

};

#endif // MODEDITOR_H
