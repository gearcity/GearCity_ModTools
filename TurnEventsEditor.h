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


#ifndef TURNEVENTSEDITOR_H
#define TURNEVENTSEDITOR_H

#include <QWidget>
#include <QFrame>
#include "BaseContainerStorage.h"
#include "TurnEventsData.h"
#include <QMap>
#include "LocalizationManager.h"
#include "CityData.h"
#include "ComponentsManager.h"
#include "AIManager.h"
#include <QTreeWidgetItem>
#include "VehiclePopInterPolators.h"
#include "TurnEventTreeGrower.h"



namespace Ui {
class TurnEventsEditor;
}

class TurnEventsEditor : public QFrame
{
    Q_OBJECT

public:
    explicit TurnEventsEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~TurnEventsEditor();

    void saveXML(QString saveFileName);
    void setTurnEventMap(QMap<int,TurnData::TE_Data> tmpMap);

private slots:

    void on_Button_TE_ReturnToPrevious_clicked();
    void on_Button_TE_MapEditor_clicked();
    void on_Button_TE_CityEditor_clicked();
    void on_Button_TE_AIEditor_clicked();
    void on_Spin_StartingYear_valueChanged(int arg1);
    void on_Spin_FinishYear_valueChanged(int arg1);
    void on_Button_SetLocalizationFile_clicked();
    void on_Button_SetFlagFolder_clicked();
    void on_Button_SetCityScriptFile_clicked();
    void on_Button_SetComponentsScript_clicked(bool checked);
    void on_Button_SetAIScript_clicked();
    void on_Button_OpenList_clicked();
    void on_Table_Office_cellClicked(int row, int column);
    void on_Button_Office_AddEdit_clicked();
    void on_Button_Office_Remove_clicked();
    void on_Table_Interest_cellClicked(int row, int column);
    void on_Button_Interest_AddEdit_clicked();
    void on_Button_Interest_Remove_clicked();
    void on_Table_CarPrice_cellClicked(int row, int column);
    void on_Button_CarPrice_AddEdit_clicked();
    void on_Button_CarPrice_Remove_clicked();
    void on_Table_Gas_cellClicked(int row, int column);
    void on_Spin_Gas_AddEdit_clicked();
    void on_Spin_Gas_Remove_clicked();
    void on_Table_Buy_cellClicked(int row, int column);
    void on_Button_Buy_AddEdit_clicked();
    void on_Button_Buy_Remove_clicked();
    void on_Table_Pension_cellClicked(int row, int column);
    void on_Button_Pension_AddEdit_clicked();
    void on_Button_Pension_Remove_clicked();
    void on_Table_Stock_cellClicked(int row, int column);
    void on_Button_Stock_AddEdit_clicked();
    void on_Button_Stock_Remove_clicked();
    void on_Button_NewList_clicked();
    void on_Table_Components_cellClicked(int row, int column);
    void on_Button_Components_AddEdit_clicked();
    void on_Button_Components_Remove_clicked();
    void on_Combo_Components_PredefinedSelector_currentIndexChanged(int index);
    void on_Spin_Car_SelectionID_editingFinished();
    void on_Combo_Car_SelectionID_currentIndexChanged(int index);
    void on_Table_Car_cellClicked(int row, int column);
    void on_Button_Car_AddEdit_clicked();
    void on_Button_Car_Remove_clicked();
    void on_Spin_City_CityID_editingFinished();
    void on_Combo_City_Cities_currentIndexChanged(int index);
    void on_Button_City_Clear_Nations_clicked();
    void on_Button_City_Clear_Ratings_clicked();
    void on_Button_City_ClearAll_clicked();
    void on_Table_City_cellClicked(int row, int column);
    void on_Button_City_AddEdit_clicked();
    void on_Button_City_Remove_clicked();
    void on_Table_News_cellClicked(int row, int column);
    void on_Button_News_Add_clicked();
    void on_Button_News_Edit_clicked();
    void on_Button_News_Remove_clicked();
    void on_Button_SaveList_clicked();
    void sectionTableCitySlot(int index);
    void sectionTableCarSlot(int index);


    void on_Button_ImportVehiclePopChanges_clicked();

private:
    Ui::TurnEventsEditor *ui;
    widgetContainerStorage cp_wsc;
    QMap<int,TurnData::TE_Data> turnMap;
    QMap<int, CityData::dataStore> cityMap;
    QString componentsFileName; //needs to be a member because Localization file changes.
    QString turnEventsSaveName;
    TurnData::TE_Data blankTE_Data;
    ComponentsManager currentComponents;

    LocalizationManager *localeManager;
    AIManager           *aiMan;

    bool notImporting;

    void refreshAllTablesAndTrees();
    void refreshOfficeTable();
    void refreshGlobalInterestTable();
    void refreshCarPriceInterestTable();
    void refreshFuelTable();
    void refreshBuyerTable();
    void refreshPensionTable();
    void refreshStockTable();
    void refreshComponentsTable();
    void refreshVehiclePopTable();
    void refreshCityEventsTable();
    void refreshNewsPaperTable();



};

#endif // TURNEVENTSEDITOR_H
