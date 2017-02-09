/*Copyright 2016 Visual Entertainment And Technologies, LLC. All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

- Neither the name of Visual Entertainment And Technologies, LLC
nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.")*/


/* *
 * CODING GUIDELINES...
 * Allman Indent Style!
 * 100 +/- Column Width
 * Avoid Namespaces
 * Lower Camel Case for variables and functions.
 * Upper Camel Case for everything else. (UI Widgets, Class Names)
 * Don't shoot me if I forget these things as well...
 * */


#ifndef MAPDATATOOL_H
#define MAPDATATOOL_H

#include <QWidget>
#include "BaseContainerStorage.h"
#include <QString>
#include <QMap>


namespace Ui {
class MapDataTool;
}

class MapDataTool : public QWidget
{
    Q_OBJECT

    class MapChangeData
    {
        public:
            int year;
            QString cityTexture;
            QString branchTexture;
            QString factoryTexture;
            QString mapFileName;
            QString turnBasedChange;
            QString regionalMap;
            double  tlLat,tlLong,trLat,trLong;
            double  blLat,blLong,brLat,brLong;
            QString individualTextures;
    };

    class mapAIData
    {
        public:
        QString fileName;
        QString description;
        int number;
    };

public:
    explicit MapDataTool(widgetContainerStorage wsc, QWidget *parent = 0);
    ~MapDataTool();


private slots:
    void on_Button_MapTools_ReturnToMain_clicked();
    void on_Button_MapTool_CityEditor_clicked();
    void on_Button_MapTool_AIEditor_clicked();
    void on_Button_MapTool_TurnEventsEditor_clicked();
    void on_Radio_MapTools_DotCity_toggled(bool checked);
    void on_ToolButton_MapTools_MapArtZipLocationFinder_clicked();
    void on_ToolButton_MapTools_CityScriptFolderFinder_clicked();
    void on_Button_MapTools_CityArtZipLocationFinder_clicked();
    void on_Button_MapTools_TurnEventsXMLFileLocationFinder_clicked();
    void on_Button_MapTools_NewspaperZipFileLocationFinder_clicked();
    void on_Button_MapTools_AIScriptFolderFinder_clicked();
    void on_Button_MapTools_AiLogoZipLocationFinder_clicked();
    void on_Button_MapTools_AddMapArtworkToLoader_clicked();
    void on_Button_MapTools_RemoveMapArtworkToLoader_clicked();
    void on_Table_MapTools_MapArtLoadingYears_cellClicked(int row, int column);
    void on_Button_MapTools_AddCityScriptYearLoader_clicked();
    void on_Button_MapTools_RemoveCityScriptYearLoader_clicked();
    void on_Table_MapTools_CityScriptLoadingYears_cellClicked(int row, int column);
    void on_Button_MapTools_AddStartingAINumbersAndFile_clicked();
    void on_Button_MapTools_RemoveStartingAINumberAndFile_clicked();
    void on_Table_MapTools_StartingAINumbersAndFiles_cellClicked(int row,
                                                                 int column);
    void on_Button_MapData_Save_clicked();
    void on_Button_MapData_Open_clicked();
    void on_Button_MapTools_MapPictureFinder_clicked();
    void on_Button_MapData_Export_clicked();
    void on_Button_MapTools_FlagZipFileFinder_clicked();

private:
    Ui::MapDataTool *ui;
    widgetContainerStorage cp_wsc;
    QMap<int,MapChangeData> mapChangeMap;
    QMap<int, QString> cityScriptMap;
    QMap<QString, mapAIData> AIScriptMap;

    void fillMapArtworkTable();
    void fillCityScriptTable();
    void fillAIScriptTable();

    void saveMapLoaderData(QString fileName);
    void openMapFile(QString openFileName);
    void exportMap(QString parentFolder);
    void saveMapInfoBaseFile(QString mapFolder);
    void saveLoaderForModToolsExport(QString folder);
    bool checkExportErrors();

};

#endif // MAPDATATOOL_H
