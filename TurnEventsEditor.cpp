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


#include "TurnEventsEditor.h"
#include "ui_TurnEventsEditor.h"
#include <QFileDialog>
#include "TurnEventTreeGrower.h"
#include <QMessageBox>
#include <QXmlStreamWriter>


//Create TurnEvents Editor Tool
TurnEventsEditor::TurnEventsEditor(widgetContainerStorage wsc, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TurnEventsEditor)
{
    ui->setupUi(this);

    //Copy Widget Storage Container
    cp_wsc = wsc;

    localeManager = 0;
    aiMan = 0;
    componentsFileName = "";
    turnEventsSaveName = "";

    //Create new list
    on_Button_NewList_clicked();
}

TurnEventsEditor::~TurnEventsEditor()
{
    delete ui;
    delete localeManager;
    delete aiMan;
}

//Lowers TurnEvent Editor to allow previous editor to be top
void TurnEventsEditor::on_Button_TE_ReturnToPrevious_clicked()
{
    cp_wsc.TurnEventEditorCW->lower();
}

//Open Map Editor
void TurnEventsEditor::on_Button_TE_MapEditor_clicked()
{
    cp_wsc.MapToolCW->raise();
}

//Open City Editor
void TurnEventsEditor::on_Button_TE_CityEditor_clicked()
{
    cp_wsc.CityEditorCW->raise();
}

//Open AI Editor
void TurnEventsEditor::on_Button_TE_AIEditor_clicked()
{
    cp_wsc.AIEditorCW->raise();
}

//Change Turn Events starting year, alters minimum year for all year sliders
void TurnEventsEditor::on_Spin_StartingYear_valueChanged(int arg1)
{
    ui->Spin_Buy_Year->setMinimum(arg1);
    ui->Spin_Car_Year->setMinimum(arg1);
    ui->Spin_CarPrice_Year->setMinimum(arg1);
    ui->Spin_City_Year->setMinimum(arg1);
    ui->Spin_Components_Year->setMinimum(arg1);
    ui->Spin_FinishYear->setMinimum(arg1+1);
    ui->Spin_Gas_Year->setMinimum(arg1);
    ui->Spin_Interest_Year->setMinimum(arg1);
    ui->Spin_News_Year->setMinimum(arg1);
    ui->Spin_Office_Year->setMinimum(arg1);
    ui->Spin_Pension_Year->setMinimum(arg1);
    ui->Spin_Stock_Year->setMinimum(arg1);
    ui->Spin_Buy_Interpolate_Year->setMinimum(arg1);
    ui->Spin_CarPrice_Interpolate_Year->setMinimum(arg1);
    ui->Spin_Gas_Interpolate_Year->setMinimum(arg1);
    ui->Spin_Interest_Interpolate_Year->setMinimum(arg1);
    ui->Spin_Pension_Interpolate_Year->setMinimum(arg1);
    ui->Spin_Stock_Interpolate_Year->setMinimum(arg1);

    //Refresh all tables and trees
    refreshAllTablesAndTrees();
}

//Change Turn Events finish year, alters maximum year for all year sliders
void TurnEventsEditor::on_Spin_FinishYear_valueChanged(int arg1)
{
    ui->Spin_Buy_Year->setMaximum(arg1);
    ui->Spin_Car_Year->setMaximum(arg1);
    ui->Spin_CarPrice_Year->setMaximum(arg1);
    ui->Spin_City_Year->setMaximum(arg1);
    ui->Spin_Components_Year->setMaximum(arg1);
    ui->Spin_StartingYear->setMaximum(arg1-1);
    ui->Spin_Gas_Year->setMaximum(arg1);
    ui->Spin_Interest_Year->setMaximum(arg1);
    ui->Spin_News_Year->setMaximum(arg1);
    ui->Spin_Office_Year->setMaximum(arg1);
    ui->Spin_Pension_Year->setMaximum(arg1);
    ui->Spin_Stock_Year->setMaximum(arg1);
    ui->Spin_Buy_Interpolate_Year->setMaximum(arg1);
    ui->Spin_CarPrice_Interpolate_Year->setMaximum(arg1);
    ui->Spin_Gas_Interpolate_Year->setMaximum(arg1);
    ui->Spin_Interest_Interpolate_Year->setMaximum(arg1);
    ui->Spin_Pension_Interpolate_Year->setMaximum(arg1);
    ui->Spin_Stock_Interpolate_Year->setMaximum(arg1);

    //Refresh all tables and trees
    refreshAllTablesAndTrees();
}

//Open Localization File and create localization manager
void TurnEventsEditor::on_Button_SetLocalizationFile_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Localization File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        //Create localization mananger
        localeManager = new LocalizationManager(openFileName, cp_wsc.TurnEventEditorCW);

        //If localization has words
        if(localeManager->size()!=0)
        {
            ui->Label_LocalizationFile->setText(openFileName);
            on_Button_SetComponentsScript_clicked(true);

            //Refresh Tables and Trees with new data
            refreshAllTablesAndTrees();
        }
    }
    else
    {
        return;
    }
}


//Open Flag Folder and store the flag file list in a combobox
void TurnEventsEditor::on_Button_SetFlagFolder_clicked()
{
    //Open Flag Folder
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Flag Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.dds");
    QStringList fileList = dir.entryList();

    //Clear Combobox, add flag files, refresh table and trees with new data.
    ui->Combo_City_Flag_Editable->clear();
    ui->Combo_City_Flag_Editable->addItems(fileList);
    ui->Label_FlagFolder->setText(folderPath);
    refreshAllTablesAndTrees();
}

//Open City Script file and fill combo boxes
void TurnEventsEditor::on_Button_SetCityScriptFile_clicked()
{
    //Open City Script file
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open City File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        //create new cityManager
        CityData *cityManager = new CityData(openFileName, cp_wsc.TurnEventEditorCW);

        //If there are cities, we'll change the label
        if(cityManager->size()!=0)
        {
            ui->Label_CityScriptFile->setText(openFileName);
        }

        //Copy citymap from the city manager
        cityMap = cityManager->returnCopyOfDSMap();

        //Clear the combo boxes
        ui->Combo_City_Cities->clear();
        ui->Combo_City_Flag_Editable->clear();
        ui->Combo_City_Nation_Editable->clear();

        //Loop through cityMap, save the data to the comboboxes
        for(QMap<int, CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end();
            ++it)
        {
            int id = ui->Combo_City_Cities->findText((*it).cityName + ", "+(*it).cityCountry);
            if(id == -1)
            {
              ui->Combo_City_Cities->addItem((*it).cityName + ", "+(*it).cityCountry,
                                           QVariant((*it).id));
            }

           id = ui->Combo_City_Flag_Editable->findText((*it).flagFileName);
           if(id == -1)
           {
            ui->Combo_City_Flag_Editable->addItem((*it).flagFileName,
                                                  QVariant((*it).cityCountry));
           }

           id = ui->Combo_City_Nation_Editable->findText((*it).cityCountry);
           if(id == -1)
           {
            ui->Combo_City_Nation_Editable->addItem((*it).cityCountry,
                                                    QVariant((*it).cityCountry));
           }
        }

        delete cityManager;

        //Refresh all tables and trees
        refreshAllTablesAndTrees();
    }
    else
    {
        return;
    }
}

//Get Component Script File and Parse it into comboboxes
void TurnEventsEditor::on_Button_SetComponentsScript_clicked(bool checked)
{
    if(checked && componentsFileName == "")
    {
        return;
    }
    else if(!checked)
    {
        componentsFileName =  QFileDialog::getOpenFileName(this, "Open Components File", "",
                                                         "XML Files (*.xml)");
    }


    if (componentsFileName != "")
    {
        //if we have a file, create a components object, then send the combos over to be filled.
        ComponentsManager cm = ComponentsManager(componentsFileName,cp_wsc.TurnEventEditorCW);
        ui->Combo_Components_PredefinedSelector->clear();
        ui->Combo_Car_SelectionID->clear();

        //If cm was not processed fine, then we shouldn't touch the combos
        if(cm.isGood())
        {
            ui->Label_ComponentsFile->setText(componentsFileName);
            cm.createComponentSelectorCombo(ui->Combo_Components_PredefinedSelector,
                                            ui->Combo_Car_SelectionID,localeManager);
        }

        //If we're not checking, refresh tables and trees
        if(!checked)
            refreshAllTablesAndTrees();

    }
    else
    {
        return;
    }
}

//Open AI Scripts
void TurnEventsEditor::on_Button_SetAIScript_clicked()
{
    //Open AI Script
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open AI File", "",
                                                         "XML Files (*.xml)");
    if (openFileName != "")
    {
        //Create AI Manager
        aiMan = new AIManager(openFileName,cp_wsc.TurnEventEditorCW);

        if(aiMan->getMap().size() > 0)
        {
             ui->Label_AIScriptFile->setText(openFileName);

            //refresh tables and trees
            refreshAllTablesAndTrees();
        }
        else
        {
            delete aiMan;
            aiMan = 0;
        }

    }
    else
    {
        return;
    }
}

//Create new Turn Events Script
void TurnEventsEditor::on_Button_NewList_clicked()
{
    //Create a new blank turn data
    TurnData td = TurnData("",cp_wsc.TurnEventEditorCW,localeManager);

    //Save a blank TE_Data for when we insert new turns
    blankTE_Data = td.generateBlankTEData();

    //clear turn map
    turnMap.clear();

    ui->Label_CurrentSaveFiles->setText("No Save File Has Been Made");

    //Refresh tables and trees
    refreshAllTablesAndTrees();
}

//Open existing Turn Events Script
void TurnEventsEditor::on_Button_OpenList_clicked()
{
    turnEventsSaveName =  QFileDialog::getOpenFileName(this, "Open Turn Event File", "",
                                                         "XML Files (*.xml)");
    if (turnEventsSaveName != "")
    {
        //Create TurnData with save file
        TurnData td = TurnData(turnEventsSaveName,cp_wsc.TurnEventEditorCW,localeManager);

        //Get turn map, and make a blank TE_Data for new entries
        turnMap = td.getTurnMap();
        blankTE_Data = td.generateBlankTEData();
        ui->Label_CurrentSaveFiles->setText(turnEventsSaveName);

        //Refresh tables and Trees
        refreshAllTablesAndTrees();

    }
    else
    {
        return;
    }

}

//Refreshes all tables and trees with new information (or no information)
void TurnEventsEditor::refreshAllTablesAndTrees()
{
    ui->Tree_All_Everything->clear();
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Fill the main tree
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);

    //Individual Tools Tables
    refreshOfficeTable();
    refreshGlobalInterestTable();
    refreshCarPriceInterestTable();
    refreshFuelTable();
    refreshBuyerTable();
    refreshPensionTable();
    refreshStockTable();
    refreshComponentsTable();
    refreshVehiclePopTable();
    refreshCityEventsTable();
    refreshNewsPaperTable();
}

//Refresh Office Table and Combos
void TurnEventsEditor::refreshOfficeTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1899;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear table and combos
    ui->Table_Office->clearContents();
    ui->Combo_Office_File_Editable->clear();
    ui->Combo_Office_File_Editable->clearEditText();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit-12 || it.key()>finishYearLimit)
            continue;

        //if there is a change in the office file add to the table and combo boxes
        if((*it).officeFile != "")
        {
            ui->Table_Office->setRowCount(rowNumber+1);
            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_Office->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_Office->setItem(rowNumber,1,new QTableWidgetItem((*it).officeFile));

            if(ui->Combo_Office_File_Editable->findText((*it).officeFile) == -1)
            {
                ui->Combo_Office_File_Editable->addItem((*it).officeFile);
            }

            rowNumber++;
        }
    }

}

//Refresh Global Interest Table
void TurnEventsEditor::refreshGlobalInterestTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_Interest->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

         //if there is a change add to the table
        if((*it).globalInterest > -1)
        {
            ui->Table_Interest->setRowCount(rowNumber+1);

            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_Interest->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_Interest->setItem(rowNumber,1,new QTableWidgetItem(QString::number(
                                        (*it).globalInterest)));

            rowNumber++;
        }
    }
}

//Refresh Car Price Rate Table
void TurnEventsEditor::refreshCarPriceInterestTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_CarPrice->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if there is a change add to the table
        if((*it).carInflation > -1)
        {
            ui->Table_CarPrice->setRowCount(rowNumber+1);

            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_CarPrice->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_CarPrice->setItem(rowNumber,1,new QTableWidgetItem(
                                            QString::number((*it).carInflation)));

            rowNumber++;
        }
    }
}

//Refresh Fuel Rate Table
void TurnEventsEditor::refreshFuelTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
   ui->Table_Gas->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
         //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if there is a change add to the table
        if((*it).globalFuelRate > -1)
        {
            ui->Table_Gas->setRowCount(rowNumber+1);

            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_Gas->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_Gas->setItem(rowNumber,1,new QTableWidgetItem(
                                       QString::number((*it).globalFuelRate)));

            rowNumber++;
        }
    }
}

//Refresh Buyer Rate Table
void TurnEventsEditor::refreshBuyerTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_Buy->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if there is a change add to the table
        if((*it).buyerRate > -1)
        {
            ui->Table_Buy->setRowCount(rowNumber+1);

            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_Buy->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_Buy->setItem(rowNumber,1,new QTableWidgetItem(
                                       QString::number((*it).buyerRate)));

            rowNumber++;
        }
    }
}

//Refresh Pension Rate Table
void TurnEventsEditor::refreshPensionTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_Pension->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if there is a change add to the table
        if((*it).pensionRate > -1)
        {
            ui->Table_Pension->setRowCount(rowNumber+1);

            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_Pension->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_Pension->setItem(rowNumber,1,new QTableWidgetItem(
                                           QString::number((*it).pensionRate)));

            rowNumber++;
        }
    }
}

//Refresh Stock Rate Table
void TurnEventsEditor::refreshStockTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_Stock->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if there is a change add to the table
        if((*it).stockRate > -1)
        {
            ui->Table_Stock->setRowCount(rowNumber+1);

            month = it.key() % 12;
            year = it.key()/12;

            if(month == 0)
            {
                year--;
                month = 12;
            }

            ui->Table_Stock->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                      "/"+QString::number(year) ) );
            ui->Table_Stock->setItem(rowNumber,1,new QTableWidgetItem(
                                         QString::number((*it).stockRate)));

            rowNumber++;
        }
    }
}

//Refresh Component Tables
void TurnEventsEditor::refreshComponentsTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;
    QString componentName ="";


    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_Components->clearContents();


    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if the componentEventList is not empty, we have data!
        if(!(*it).CompEvtList.empty())
        {
            //Loop through Component Event List and stick it in the table
            for(QList<TurnData::TE_ComponentEvts>::iterator itCE = (*it).CompEvtList.begin();
                itCE != (*it).CompEvtList.end(); ++itCE)
            {
                ui->Table_Components->setRowCount(rowNumber+1);

                month = it.key() % 12;
                year = it.key()/12;

                if(month == 0)
                {
                    year--;
                    month = 12;
                }

                componentName = "";
                int comboIndex = ui->Combo_Components_PredefinedSelector->findData(
                            QVariant((*itCE).selectedID));
                if(comboIndex != -1)
                {
                    componentName = "("+ui->Combo_Components_PredefinedSelector->itemText(
                                comboIndex)+")";
                }

                ui->Table_Components->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                          "/"+QString::number(year) ) );


                ui->Table_Components->setItem(rowNumber,1,new QTableWidgetItem(
                                             QString::number((*itCE).selectedID)));

                ui->Table_Components->setItem(rowNumber,2,new QTableWidgetItem(componentName));
                ui->Table_Components->setItem(rowNumber,3,new QTableWidgetItem(
                                                  QString::number((*itCE).popChange)));

                rowNumber++;
            }
        }
    }
}


//Refresh Vehicle Popularity Table
void TurnEventsEditor::refreshVehiclePopTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;
    QString vehicleTypeName ="";

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_Car->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;


        //if the vehicle popularity list is not empty, we have data!
        if(!(*it).vehiclePopList.empty())
        {
            //Loop through vehicle pop List and stick it in the table
            for(QList<TurnData::TE_VehiclePop>::iterator itVP = (*it).vehiclePopList.begin();
                itVP != (*it).vehiclePopList.end(); ++itVP)
            {
                ui->Table_Car->setRowCount(rowNumber+1);

                month = it.key() % 12;
                year = it.key()/12;

                if(month == 0)
                {
                    year--;
                    month = 12;
                }


                vehicleTypeName = "";
                int comboIndex = ui->Combo_Car_SelectionID->findData(
                            QVariant((*itVP).selectorID));
                if(comboIndex != -1)
                {
                    vehicleTypeName = "("+ui->Combo_Car_SelectionID->itemText(
                                comboIndex)+")";
                }

                ui->Table_Car->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                          "/"+QString::number(year) ) );


                ui->Table_Car->setItem(rowNumber,1,new QTableWidgetItem(
                                             QString::number((*itVP).selectorID)));

                ui->Table_Car->setItem(rowNumber,2,new QTableWidgetItem(vehicleTypeName));
                ui->Table_Car->setItem(rowNumber,3,new QTableWidgetItem(
                                                  QString::number((*itVP).pop)));
                ui->Table_Car->setItem(rowNumber,4,new QTableWidgetItem(
                                                  QString::number((*itVP).popR1)));
                ui->Table_Car->setItem(rowNumber,5,new QTableWidgetItem(
                                                  QString::number((*itVP).popR2)));
                ui->Table_Car->setItem(rowNumber,6,new QTableWidgetItem(
                                                  QString::number((*itVP).popR3)));
                ui->Table_Car->setItem(rowNumber,7,new QTableWidgetItem(
                                                  QString::number((*itVP).popR4)));
                ui->Table_Car->setItem(rowNumber,8,new QTableWidgetItem(
                                                  QString::number((*itVP).popR5)));
                ui->Table_Car->setItem(rowNumber,9,new QTableWidgetItem(
                                                  QString::number((*itVP).popR6)));

                rowNumber++;
            }
        }
    }
}

//Refresh City Events Tables
void TurnEventsEditor::refreshCityEventsTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_City->clearContents();


    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if the City Event List is not empty, we have data!
        if(!(*it).CityEvtList.empty())
        {
            //Loop through City Event List and stick it in the table
            for(QList<TurnData::TE_CityEvts>::iterator itCE = (*it).CityEvtList.begin();
                itCE != (*it).CityEvtList.end(); ++itCE)
            {
                ui->Table_City->setRowCount(rowNumber+1);

                month = it.key() % 12;
                year = it.key()/12;

                if(month == 0)
                {
                    year--;
                    month = 12;
                }

                ui->Table_City->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                          "/"+QString::number(year) ) );

                ui->Table_City->setItem(rowNumber,1,new QTableWidgetItem(
                                             QString::number((*itCE).id)));
                if(cityMap.contains((*itCE).id))
                {
                    ui->Table_City->setItem(rowNumber,2,
                                   new QTableWidgetItem(cityMap.find((*itCE).id).value().cityName +
                                   ","+cityMap.find((*itCE).id).value().cityCountry));
                }

                if((*itCE).bgovChange)
                {
                    ui->Table_City->setItem(rowNumber,3,new QTableWidgetItem(
                                                      QString::number((*itCE).gov)));
                }
                if((*itCE).bNationOrFlagChange)
                {
                    ui->Table_City->setItem(rowNumber,4,new QTableWidgetItem((*itCE).Flag));
                    ui->Table_City->setItem(rowNumber,5,new QTableWidgetItem((*itCE).Nation));
                }
                if((*itCE).bInfManuChange)
                {
                    ui->Table_City->setItem(rowNumber,6,new QTableWidgetItem(
                                                  QString::number((*itCE).infChange)));
                    ui->Table_City->setItem(rowNumber,7,new QTableWidgetItem(
                                                  QString::number((*itCE).manuChange)));
                }

                rowNumber++;
            }
        }
    }
}



void TurnEventsEditor::refreshNewsPaperTable()
{
    int rowNumber = 0;
    int month = 1;
    int year = 1900;

    //Limit data within the bounds of the game
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    //Clear Table
    ui->Table_News->clearContents();

    //Loop through turn map
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //if the turn is not in the current game years, go to the next entry in the map
        if(it.key()<startYearLimit || it.key()>finishYearLimit)
            continue;

        //if the News Event List is not empty, we have data!
        if(!(*it).NewsEvtList.empty())
        {
            //Loop through News Event List and stick it in the table
            for(QList<TurnData::TE_NewsEvts>::iterator itNE = (*it).NewsEvtList.begin();
                itNE != (*it).NewsEvtList.end(); ++itNE)
            {
                ui->Table_News->setRowCount(rowNumber+1);

                month = it.key() % 12;
                year = it.key()/12;

                if(month == 0)
                {
                    year--;
                    month = 12;
                }

                ui->Table_News->setItem(rowNumber,0,new QTableWidgetItem(QString::number(month)+
                                                          "/"+QString::number(year) ) );

                if((*itNE).localization)
                {
                   ui->Table_News->setItem(rowNumber,1,new QTableWidgetItem("1"));
                }
                else
                {
                   ui->Table_News->setItem(rowNumber,1,new QTableWidgetItem("0"));                   
                }

                ui->Table_News->setItem(rowNumber,2,new QTableWidgetItem((*itNE).headline));
                ui->Table_News->setItem(rowNumber,3,new QTableWidgetItem((*itNE).body));
                ui->Table_News->setItem(rowNumber,4,new QTableWidgetItem((*itNE).image));
                if((*itNE).glow)
                {
                    ui->Table_News->setItem(rowNumber,5,new QTableWidgetItem("1"));
                }
                else
                {
                    ui->Table_News->setItem(rowNumber,5,new QTableWidgetItem("0"));
                }
                rowNumber++;
            }
        }
    }
}

//Office Table clicked
void TurnEventsEditor::on_Table_Office_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners and then set file
    dateStrSplitList = ui->Table_Office->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Office_Month->setValue(month);
    ui->Spin_Office_Year->setValue(year);
    ui->Combo_Office_File_Editable->setEditText(ui->Table_Office->item(row,1)->text());
}


//Office Add or Edit Clicked
void TurnEventsEditor::on_Button_Office_AddEdit_clicked()
{
    //Get turn
    int year = ui->Spin_Office_Year->value();
    int stopYear = year;
    int turn = 0;

    if(ui->checkbox_TE_OfficeYearlyAdd->isChecked())
    {
     stopYear = ui->Spin_Office_YearAutoStop->value();
    }

    do
    {
        turn = ui->Spin_Office_Month->value() + (year*12);

        //If turn exists, we edit. If not, we insert
        if(turnMap.contains(turn))
        {
            turnMap.find(turn).value().officeFile = ui->Combo_Office_File_Editable->currentText();
        }
        else
        {
            TurnData::TE_Data teda = blankTE_Data;
            teda.officeFile = ui->Combo_Office_File_Editable->currentText();
            turnMap.insert(turn,teda);
        }

        year++;
    }
    while(year < stopYear);

    //Refresh Office Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshOfficeTable();
}

//Office Remove Clicked
void TurnEventsEditor::on_Button_Office_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Office_Month->value() + (ui->Spin_Office_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().officeFile = "";
    }

    //Refresh Office Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshOfficeTable();

}

//Interest Table clicked
void TurnEventsEditor::on_Table_Interest_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_Interest->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Interest_Month->setValue(month);
    ui->Spin_Interest_Year->setValue(year);
    ui->Spin_Interest_Rate->setValue(ui->Table_Interest->item(row,1)->text().toDouble());

}

//Interest Add or Edit Clicked
void TurnEventsEditor::on_Button_Interest_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Interest_Month->value() + (ui->Spin_Interest_Year->value()*12);

    //If turn exists, we edit. If not, we insert
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().globalInterest = ui->Spin_Interest_Rate->value();
    }
    else
    {
        TurnData::TE_Data teda = blankTE_Data;
        teda.globalInterest = ui->Spin_Interest_Rate->value();
        turnMap.insert(turn,teda);
    }

    //If interpolation is checked, add or edit values until interpolation date is reached
    if(ui->Check_Interest_Interpolate->isChecked())
    {
        int interpolateTurn = ui->Spin_Interest_Interpolate_Month->value() +
                (ui->Spin_Interest_Interpolate_Year->value()*12);

        //If interpolate date is less than the current date, we can't interpolate
        if(interpolateTurn>turn)
        {
            int entryTurn = turn;
            double entryRate = ui->Spin_Interest_Rate->value();

            //This is the amount we change the base per turn until we reach the inerpolation turn
            double changeAmountPerTurn = (ui->Spin_Interest_Interpolate_Rate->value() -
                   ui->Spin_Interest_Rate->value()) / (interpolateTurn-turn);

            while(entryTurn < interpolateTurn)
            {
                //Already inserted at base turn, so add the change amount and up the base turn 1
                entryRate += changeAmountPerTurn;
                entryTurn++;

                if(turnMap.contains(entryTurn))
                {
                    turnMap.find(entryTurn).value().globalInterest = entryRate;
                }
                else
                {
                    TurnData::TE_Data teda = blankTE_Data;
                    teda.globalInterest = entryRate;
                    turnMap.insert(entryTurn,teda);
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Could Not Interpolate!",
                                  "We could not interpolate the data for you.\n"
                                  "Interpolation date must be higher than inital date!");
            return;
        }
    }

    //Refresh Interest Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshGlobalInterestTable();
}

//Interest Remove Clicked
void TurnEventsEditor::on_Button_Interest_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Interest_Month->value() + (ui->Spin_Interest_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().globalInterest = -1;
    }

    //Refresh Interest Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshGlobalInterestTable();

}

//CarPrice Table clicked
void TurnEventsEditor::on_Table_CarPrice_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_CarPrice->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_CarPrice_Month->setValue(month);
    ui->Spin_CarPrice_Year->setValue(year);
    ui->Spin_CarPrice_Rate->setValue(ui->Table_CarPrice->item(row,1)->text().toDouble());

}

//Car Price Add or Edit Clicked
void TurnEventsEditor::on_Button_CarPrice_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_CarPrice_Month->value() + (ui->Spin_CarPrice_Year->value()*12);

    //If turn exists, we edit. If not, we insert
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().carInflation = ui->Spin_CarPrice_Rate->value();
    }
    else
    {
        TurnData::TE_Data teda = blankTE_Data;
        teda.carInflation = ui->Spin_CarPrice_Rate->value();
        turnMap.insert(turn,teda);
    }

    //If interpolation is checked, add or edit values until interpolation date is reached
    if(ui->Check_CarPrice_Interpolate->isChecked())
    {
        int interpolateTurn = ui->Spin_CarPrice_Interpolate_Month->value() +
                (ui->Spin_CarPrice_Interpolate_Year->value()*12);

        //If interpolate date is less than the current date, we can't interpolate
        if(interpolateTurn>turn)
        {
            int entryTurn = turn;
            double entryRate = ui->Spin_CarPrice_Rate->value();

            //This is the amount we change the base per turn until we reach the inerpolation turn
            double changeAmountPerTurn = (ui->Spin_CarPrice_Interpolate_Rate->value() -
                   ui->Spin_CarPrice_Rate->value()) / (interpolateTurn-turn);

            while(entryTurn < interpolateTurn)
            {
                //Already inserted at base turn, so add the change amount and up the base turn 1
                entryRate += changeAmountPerTurn;
                entryTurn++;

                if(turnMap.contains(entryTurn))
                {
                    turnMap.find(entryTurn).value().carInflation = entryRate;
                }
                else
                {
                    TurnData::TE_Data teda = blankTE_Data;
                    teda.carInflation = entryRate;
                    turnMap.insert(entryTurn,teda);
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Could Not Interpolate!",
                                  "We could not interpolate the data for you.\n"
                                  "Interpolation date must be higher than inital date!");
            return;
        }
    }

    //Refresh Car Price Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshCarPriceInterestTable();
}

//Car Price Remove Clicked
void TurnEventsEditor::on_Button_CarPrice_Remove_clicked()
{
    //Get Turn

    int turn = ui->Spin_CarPrice_Month->value() + (ui->Spin_CarPrice_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().carInflation = -1;
    }

    //Refresh Car Price Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshCarPriceInterestTable();
}


//Fuel Table clicked
void TurnEventsEditor::on_Table_Gas_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_Gas->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Gas_Month->setValue(month);
    ui->Spin_Gas_Year->setValue(year);
    ui->Spin_Gas_FuelRate->setValue(ui->Table_Gas->item(row,1)->text().toDouble());

}

//Fuel Add or Edit Clicked
void TurnEventsEditor::on_Spin_Gas_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Gas_Month->value() + (ui->Spin_Gas_Year->value()*12);

    //If turn exists, we edit. If not, we insert
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().globalFuelRate = ui->Spin_Gas_FuelRate->value();
    }
    else
    {
        TurnData::TE_Data teda = blankTE_Data;
        teda.globalFuelRate = ui->Spin_Gas_FuelRate->value();
        turnMap.insert(turn,teda);
    }

    //If interpolation is checked, add or edit values until interpolation date is reached
    if(ui->Check_Gas_Interpolate->isChecked())
    {
        int interpolateTurn = ui->Spin_Gas_Interpolate_Month->value() +
                (ui->Spin_Gas_Interpolate_Year->value()*12);

        //If interpolate date is less than the current date, we can't interpolate
        if(interpolateTurn>turn)
        {
            int entryTurn = turn;
            double entryRate = ui->Spin_Gas_FuelRate->value();

            //This is the amount we change the base per turn until we reach the inerpolation turn
            double changeAmountPerTurn = (ui->Spin_Gas_Interpolate_Rate->value() -
                   ui->Spin_Gas_FuelRate->value()) / (interpolateTurn-turn);

            while(entryTurn < interpolateTurn)
            {
                //Already inserted at base turn, so add the change amount and up the base turn 1
                entryRate += changeAmountPerTurn;
                entryTurn++;

                if(turnMap.contains(entryTurn))
                {
                    turnMap.find(entryTurn).value().globalFuelRate = entryRate;
                }
                else
                {
                    TurnData::TE_Data teda = blankTE_Data;
                    teda.globalFuelRate = entryRate;
                    turnMap.insert(entryTurn,teda);
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Could Not Interpolate!",
                                  "We could not interpolate the data for you.\n"
                                  "Interpolation date must be higher than inital date!");
            return;
        }
    }

    //Fuel Interest Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshFuelTable();
}

//Fuel Remove Clicked
void TurnEventsEditor::on_Spin_Gas_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Gas_Month->value() + (ui->Spin_Gas_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().globalFuelRate = -1;
    }

    //Refresh Fuel Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshFuelTable();
}

//Buyer Table clicked
void TurnEventsEditor::on_Table_Buy_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_Buy->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Buy_Month->setValue(month);
    ui->Spin_Buy_Year->setValue(year);
    ui->Spin_Buy_Rate->setValue(ui->Table_Buy->item(row,1)->text().toDouble());

}

//Buyer Rate Add or Edit Clicked
void TurnEventsEditor::on_Button_Buy_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Buy_Month->value() + (ui->Spin_Buy_Year->value()*12);

    //If turn exists, we edit. If not, we insert
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().buyerRate = ui->Spin_Buy_Rate->value();
    }
    else
    {
        TurnData::TE_Data teda = blankTE_Data;
        teda.buyerRate = ui->Spin_Buy_Rate->value();
        turnMap.insert(turn,teda);
    }

    //If interpolation is checked, add or edit values until interpolation date is reached
    if(ui->Check_Buy_Interpolate->isChecked())
    {
        int interpolateTurn = ui->Spin_Buy_Interpolate_Month->value() +
                (ui->Spin_Buy_Interpolate_Year->value()*12);

        //If interpolate date is less than the current date, we can't interpolate
        if(interpolateTurn>turn)
        {
            int entryTurn = turn;
            double entryRate = ui->Spin_Buy_Rate->value();

            //This is the amount we change the base per turn until we reach the inerpolation turn
            double changeAmountPerTurn = (ui->Spin_Buy_Interpolate_Rate->value() -
                   ui->Spin_Buy_Rate->value()) / (interpolateTurn-turn);

            while(entryTurn < interpolateTurn)
            {
                //Already inserted at base turn, so add the change amount and up the base turn 1
                entryRate += changeAmountPerTurn;
                entryTurn++;

                if(turnMap.contains(entryTurn))
                {
                    turnMap.find(entryTurn).value().buyerRate = entryRate;
                }
                else
                {
                    TurnData::TE_Data teda = blankTE_Data;
                    teda.buyerRate = entryRate;
                    turnMap.insert(entryTurn,teda);
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Could Not Interpolate!",
                                  "We could not interpolate the data for you.\n"
                                  "Interpolation date must be higher than inital date!");
            return;
        }
    }

    //Buy Rate Interest Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshBuyerTable();
}

//Buy Rate Remove Clicked
void TurnEventsEditor::on_Button_Buy_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Buy_Month->value() + (ui->Spin_Buy_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().buyerRate = -1;
    }

    //Refresh Buy Rate Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshBuyerTable();
}

//Pension Table clicked
void TurnEventsEditor::on_Table_Pension_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_Pension->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Pension_Month->setValue(month);
    ui->Spin_Pension_Year->setValue(year);
    ui->Spin_Pension_Rate->setValue(ui->Table_Pension->item(row,1)->text().toDouble());

}

//Pension Add or Edit Clicked
void TurnEventsEditor::on_Button_Pension_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Pension_Month->value() + (ui->Spin_Pension_Year->value()*12);

    //If turn exists, we edit. If not, we insert
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().pensionRate = ui->Spin_Pension_Rate->value();
    }
    else
    {
        TurnData::TE_Data teda = blankTE_Data;
        teda.pensionRate = ui->Spin_Pension_Rate->value();
        turnMap.insert(turn,teda);
    }


    //If interpolation is checked, add or edit values until interpolation date is reached
    if(ui->Check_Pension_Interpolate->isChecked())
    {
        int interpolateTurn = ui->Spin_Pension_Interpolate_Month->value() +
                (ui->Spin_Pension_Interpolate_Year->value()*12);

        //If interpolate date is less than the current date, we can't interpolate
        if(interpolateTurn>turn)
        {
            int entryTurn = turn;
            double entryRate = ui->Spin_Pension_Rate->value();

            //This is the amount we change the base per turn until we reach the inerpolation turn
            double changeAmountPerTurn = (ui->Spin_Pension_Interpolate_Rate->value() -
                   ui->Spin_Pension_Rate->value()) / (interpolateTurn-turn);

            while(entryTurn < interpolateTurn)
            {
                //Already inserted at base turn, so add the change amount and up the base turn 1
                entryRate += changeAmountPerTurn;
                entryTurn++;

                if(turnMap.contains(entryTurn))
                {
                    turnMap.find(entryTurn).value().pensionRate = entryRate;
                }
                else
                {
                    TurnData::TE_Data teda = blankTE_Data;
                    teda.pensionRate = entryRate;
                    turnMap.insert(entryTurn,teda);
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Could Not Interpolate!",
                                  "We could not interpolate the data for you.\n"
                                  "Interpolation date must be higher than inital date!");
            return;
        }
    }

    //Refresh Pension Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshPensionTable();
}

//Pension Remove Clicked
void TurnEventsEditor::on_Button_Pension_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Pension_Month->value() + (ui->Spin_Pension_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().pensionRate = -1;
    }

    //Refresh Pension Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshPensionTable();
}


//Stock Table clicked
void TurnEventsEditor::on_Table_Stock_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_Stock->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Stock_Month->setValue(month);
    ui->Spin_Stock_Year->setValue(year);
    ui->Spin_Stock_Rate->setValue(ui->Table_Stock->item(row,1)->text().toDouble());
}

//Stock Add or Edit Clicked
void TurnEventsEditor::on_Button_Stock_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Stock_Month->value() + (ui->Spin_Stock_Year->value()*12);

    //If turn exists, we edit. If not, we insert
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().stockRate = ui->Spin_Stock_Rate->value();
    }
    else
    {
        TurnData::TE_Data teda = blankTE_Data;
        teda.stockRate = ui->Spin_Stock_Rate->value();
        turnMap.insert(turn,teda);
    }

    //If interpolation is checked, add or edit values until interpolation date is reached
    if(ui->Check_Stock_Interpolate->isChecked())
    {
        int interpolateTurn = ui->Spin_Stock_Interpolate_Month->value() +
                (ui->Spin_Stock_Interpolate_Year->value()*12);

        //If interpolate date is less than the current date, we can't interpolate
        if(interpolateTurn>turn)
        {
            int entryTurn = turn;
            double entryRate = ui->Spin_Stock_Rate->value();

            //This is the amount we change the base per turn until we reach the inerpolation turn
            double changeAmountPerTurn = (ui->Spin_Stock_Interpolate_Rate->value() -
                   ui->Spin_Stock_Rate->value()) / (interpolateTurn-turn);

            while(entryTurn < interpolateTurn)
            {
                //Already inserted at base turn, so add the change amount and up the base turn 1
                entryRate += changeAmountPerTurn;
                entryTurn++;

                if(turnMap.contains(entryTurn))
                {
                    turnMap.find(entryTurn).value().stockRate = entryRate;
                }
                else
                {
                    TurnData::TE_Data teda = blankTE_Data;
                    teda.stockRate = entryRate;
                    turnMap.insert(entryTurn,teda);
                }
            }
        }
        else
        {
            QMessageBox::critical(this,"Could Not Interpolate!",
                                  "We could not interpolate the data for you.\n"
                                  "Interpolation date must be higher than inital date!");
            return;
        }
    }

    //Refresh Stock Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshStockTable();
}

//Stock Remove Clicked
void TurnEventsEditor::on_Button_Stock_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Stock_Month->value() + (ui->Spin_Stock_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        turnMap.find(turn).value().stockRate = -1;
    }

    //Refresh Stock Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshStockTable();
}

//Components Table clicked
void TurnEventsEditor::on_Table_Components_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners
    dateStrSplitList = ui->Table_Components->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Components_Month->setValue(month);
    ui->Spin_Components_Year->setValue(year);

    //Set Components Index and if possible the combobox
    ui->Spin_Components_SelectorIndex->setValue(ui->Table_Components->item(row,1)->text().toInt());
    int comboIndex = ui->Combo_Components_PredefinedSelector->findData(QVariant(
                                                    ui->Spin_Components_SelectorIndex->value()));
    if(comboIndex != -1)
    {
        ui->Combo_Components_PredefinedSelector->setCurrentIndex(comboIndex);
    }

    //Set spinner componenets pop
    ui->Spin_Components_PopChange->setValue(ui->Table_Stock->item(row,3)->text().toDouble());
}


//Components Add or Edit Clicked
void TurnEventsEditor::on_Button_Components_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Components_Month->value() + (ui->Spin_Components_Year->value()*12);

    //If turn exists, we look at the list and see if we need to update or insert
    if(turnMap.contains(turn))
    {
        int selectionID = ui->Spin_Components_SelectorIndex->value();
        bool found = false;

        //Get iterator for TurnMap based on the turn
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        //loop through list, if component is there, update
        for(QList<TurnData::TE_ComponentEvts>::iterator itCE = (*itTM).CompEvtList.begin();
            itCE != (*itTM).CompEvtList.end(); ++itCE)
        {
            if(selectionID == (*itCE).selectedID)
            {
                (*itCE).popChange = ui->Spin_Components_PopChange->value();
                found = true;
                break;
            }
        }

        //Componenet was not in list, we insert into list
        if(!found)
        {
            TurnData::TE_ComponentEvts tece;
            tece.selectedID = selectionID;
            tece.popChange = ui->Spin_Components_PopChange->value();
            (*itTM).CompEvtList.push_back(tece);
        }

    }
    else //No turn in TurnMap, make a new TE_Data, insert into turn, then insert into list.
    {
        TurnData::TE_Data teda = blankTE_Data;
        TurnData::TE_ComponentEvts tece;
        tece.selectedID = ui->Spin_Components_SelectorIndex->value();
        tece.popChange = ui->Spin_Components_PopChange->value();

        teda.CompEvtList.push_back(tece);
        turnMap.insert(turn,teda);
    }

    //Refresh Components Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshComponentsTable();

}

//Components Remove Clicked
void TurnEventsEditor::on_Button_Components_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Components_Month->value() + (ui->Spin_Components_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        int selectionID = ui->Spin_Components_SelectorIndex->value();
        QList<TurnData::TE_ComponentEvts>::iterator itToDelete;
        bool found = false;
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        for(QList<TurnData::TE_ComponentEvts>::iterator itCE = (*itTM).CompEvtList.begin();
            itCE != (*itTM).CompEvtList.end(); ++itCE)
        {
            if(selectionID == (*itCE).selectedID)
            {
                itToDelete = itCE;
                found = true;
            }
        }

        if(found)
        {
            (*itTM).CompEvtList.erase(itToDelete);
        }

    }

    //Refresh Components Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshComponentsTable();
}

//Components Combo changed, change the selectorindex spinner
void TurnEventsEditor::on_Combo_Components_PredefinedSelector_currentIndexChanged(int index)
{
    int selectorID = ui->Combo_Components_PredefinedSelector->itemData(index).toInt();
    ui->Spin_Components_SelectorIndex->setValue(selectorID);
}

//Componenets selection index changed, see if we can find the combo for it.
void TurnEventsEditor::on_Spin_Car_SelectionID_editingFinished()
{
    int comboIndex = ui->Combo_Car_SelectionID->findData(QVariant(
                                                    ui->Spin_Car_SelectionID->value()));
    if(comboIndex != -1)
    {
        ui->Combo_Car_SelectionID->setCurrentIndex(comboIndex);
    }

}

//Vehicle Combo Changed, set vehicle pop spinner
void TurnEventsEditor::on_Combo_Car_SelectionID_currentIndexChanged(int index)
{
    int selectorID = ui->Combo_Car_SelectionID->itemData(index).toInt();
    ui->Spin_Car_SelectionID->setValue(selectorID);
}

//Vehicle Pop Table Clicked
void TurnEventsEditor::on_Table_Car_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners
    dateStrSplitList = ui->Table_Car->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_Car_Month->setValue(month);
    ui->Spin_Car_Year->setValue(year);

    //Set selection ID and if possible assign combo box
    ui->Spin_Car_SelectionID->setValue(ui->Table_Car->item(row,1)->text().toInt());
    int comboIndex = ui->Combo_Car_SelectionID->findData(QVariant(
                                                    ui->Spin_Car_SelectionID->value()));
    if(comboIndex != -1)
    {
        ui->Combo_Car_SelectionID->setCurrentIndex(comboIndex);
    }

    //set ratings
    ui->Spin_Car_Pop->setValue(ui->Table_Car->item(row,3)->text().toDouble());
    ui->Spin_Car_PopR1->setValue(ui->Table_Car->item(row,4)->text().toDouble());
    ui->Spin_Car_PopR2->setValue(ui->Table_Car->item(row,5)->text().toDouble());
    ui->Spin_Car_PopR3->setValue(ui->Table_Car->item(row,6)->text().toDouble());
    ui->Spin_Car_PopR4->setValue(ui->Table_Car->item(row,7)->text().toDouble());
    ui->Spin_Car_PopR5->setValue(ui->Table_Car->item(row,8)->text().toDouble());
    ui->Spin_Car_PopR6->setValue(ui->Table_Car->item(row,9)->text().toDouble());
}

//Vehicle Pop Add or Edit Clicked
void TurnEventsEditor::on_Button_Car_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_Car_Month->value() + (ui->Spin_Car_Year->value()*12);

    //If turn exists, we look at the list and see if we need to update or insert
    if(turnMap.contains(turn))
    {
        int selectionID = ui->Spin_Car_SelectionID->value();
        bool found = false;

        //Get iterator for TurnMap based on the turn
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        //loop through list, if vehicle pop is there, update
        for(QList<TurnData::TE_VehiclePop>::iterator itVP = (*itTM).vehiclePopList.begin();
            itVP != (*itTM).vehiclePopList.end(); ++itVP)
        {
            //loop through list, if vehicle is there, update
            if(selectionID == (*itVP).selectorID)
            {
                (*itVP).pop = ui->Spin_Car_Pop->value();
                (*itVP).popR1 = ui->Spin_Car_PopR1->value();
                (*itVP).popR2 = ui->Spin_Car_PopR2->value();
                (*itVP).popR3 = ui->Spin_Car_PopR3->value();
                (*itVP).popR4 = ui->Spin_Car_PopR4->value();
                (*itVP).popR5 = ui->Spin_Car_PopR5->value();
                (*itVP).popR6 = ui->Spin_Car_PopR6->value();
                found = true;
                break;
            }
        }

        //vehicle was not in list, we insert into list
        if(!found)
        {
            TurnData::TE_VehiclePop tevp;
            tevp.selectorID = selectionID;
            tevp.pop = ui->Spin_Car_Pop->value();
            tevp.popR1 = ui->Spin_Car_PopR1->value();
            tevp.popR2 = ui->Spin_Car_PopR2->value();
            tevp.popR3 = ui->Spin_Car_PopR3->value();
            tevp.popR4 = ui->Spin_Car_PopR4->value();
            tevp.popR5 = ui->Spin_Car_PopR5->value();
            tevp.popR6 = ui->Spin_Car_PopR6->value();
            (*itTM).vehiclePopList.push_back(tevp);
        }

    }
    else //No turn in TurnMap, make a new TE_Data, insert into turn, then insert into list.
    {
        TurnData::TE_Data teda = blankTE_Data;
        TurnData::TE_VehiclePop tevp;
        tevp.selectorID = ui->Spin_Car_SelectionID->value();
        tevp.pop = ui->Spin_Car_Pop->value();
        tevp.popR1 = ui->Spin_Car_PopR1->value();
        tevp.popR2 = ui->Spin_Car_PopR2->value();
        tevp.popR3 = ui->Spin_Car_PopR3->value();
        tevp.popR4 = ui->Spin_Car_PopR4->value();
        tevp.popR5 = ui->Spin_Car_PopR5->value();
        tevp.popR6 = ui->Spin_Car_PopR6->value();
        teda.vehiclePopList.push_back(tevp);
        turnMap.insert(turn,teda);
    }

    //Refresh vehicle pop Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshVehiclePopTable();
}

//Vehicle Remove Clicked
void TurnEventsEditor::on_Button_Car_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_Car_Month->value() + (ui->Spin_Car_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        int selectionID = ui->Spin_Car_SelectionID->value();
        QList<TurnData::TE_VehiclePop>::iterator itToDelete;
        bool found = false;
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        for(QList<TurnData::TE_VehiclePop>::iterator itVP = (*itTM).vehiclePopList.begin();
            itVP != (*itTM).vehiclePopList.end(); ++itVP)
        {
            if(selectionID == (*itVP).selectorID)
            {
                itToDelete = itVP;
                found = true;
            }
        }

        if(found)
        {
            (*itTM).vehiclePopList.erase(itToDelete);
        }

    }

    //Refresh Vehicle Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshVehiclePopTable();
}

//City Spinners Changed, set various Combos if info is there
void TurnEventsEditor::on_Spin_City_CityID_editingFinished()
{
    QString countryName = "";

    for(QMap<int, CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end();
        ++it)
    {
        if(it.key() == ui->Spin_City_CityID->value())
        {
            countryName = (*it).cityCountry;
            break;
        }
    }

    int comboIndex = ui->Combo_City_Cities->findData(QVariant(ui->Spin_City_CityID->value()));
    if(comboIndex != -1)
    {
      ui->Combo_City_Cities->setCurrentIndex(comboIndex);
    }

    comboIndex = ui->Combo_City_Flag_Editable->findData(QVariant(countryName));
    if(comboIndex != -1)
    {
      ui->Combo_City_Flag_Editable->setCurrentIndex(comboIndex);
    }

    comboIndex = ui->Combo_City_Nation_Editable->findData(QVariant(countryName));
    if(comboIndex != -1)
    {
      ui->Combo_City_Nation_Editable->setCurrentIndex(comboIndex);
    }

}

//City Combos Changed, set spinners and combos as needed
void TurnEventsEditor::on_Combo_City_Cities_currentIndexChanged(int index)
{
    int selectorID = ui->Combo_City_Cities->itemData(index).toInt();
    ui->Spin_City_CityID->setValue(selectorID);

    QString countryName = "";

    for(QMap<int, CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end();
        ++it)
    {
        if(it.key() == selectorID)
        {
            countryName = (*it).cityCountry;
            break;
        }
    }

    int comboIndex = ui->Combo_City_Flag_Editable->findData(QVariant(countryName));
    if(comboIndex != -1)
    {
      ui->Combo_City_Flag_Editable->setCurrentIndex(comboIndex);
    }

    comboIndex = ui->Combo_City_Nation_Editable->findData(QVariant(countryName));
    if(comboIndex != -1)
    {
      ui->Combo_City_Nation_Editable->setCurrentIndex(comboIndex);
    }
}

//Clear flags and nation info
void TurnEventsEditor::on_Button_City_Clear_Nations_clicked()
{
    ui->Combo_City_Flag_Editable->setEditText("");
    ui->Combo_City_Nation_Editable->setEditText("");
    ui->Check_City_NationChange->setChecked(false);
}

//Clear city ratings flags and details
void TurnEventsEditor::on_Button_City_Clear_Ratings_clicked()
{
    ui->Spin_City_Infra->setValue(1.0000);
    ui->Spin_City_Manu->setValue(1.0000);
    ui->Check_City_CityRating->setChecked(false);
}

//Clear all city information
void TurnEventsEditor::on_Button_City_ClearAll_clicked()
{
    ui->Check_City_Gov->setChecked(false);
    ui->Combo_City_Government->setCurrentIndex(3);
    ui->Spin_City_Infra->setValue(1.0000);
    ui->Spin_City_Manu->setValue(1.0000);
    ui->Check_City_CityRating->setChecked(false);
    ui->Combo_City_Flag_Editable->setEditText("");
    ui->Combo_City_Nation_Editable->setEditText("");
    ui->Check_City_NationChange->setChecked(false);
}

//City Table clicked
void TurnEventsEditor::on_Table_City_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners then set rate
    dateStrSplitList = ui->Table_City->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_City_Month->setValue(month);
    ui->Spin_City_Year->setValue(year);

    //Set City ID Spinner, and If possible select the city combo box
    ui->Spin_City_CityID->setValue(ui->Table_City->item(row,1)->text().toInt());
    int comboIndex = ui->Combo_City_Cities->findData(QVariant(
                                                    ui->Spin_City_CityID->value()));
    if(comboIndex != -1)
    {
        ui->Combo_City_Cities->setCurrentIndex(comboIndex);
    }

    //If the data is there, set flag and nation combos and check the flag
    if(ui->Table_City->item(row,4) != 0 )
    {
     ui->Combo_City_Flag_Editable->setEditText(ui->Table_City->item(row,4)->text());
     ui->Combo_City_Nation_Editable->setEditText(ui->Table_City->item(row,5)->text());
     ui->Check_City_NationChange->setChecked(true);
    }
    else
    {
        ui->Check_City_NationChange->setChecked(false);
    }

    //If the data is there, set infrastructor and manufacturing data and check the flag
     if(ui->Table_City->item(row,6) != 0 )
     {
         ui->Spin_City_Infra->setValue(ui->Table_City->item(row,6)->text().toDouble());
         ui->Spin_City_Manu->setValue(ui->Table_City->item(row,7)->text().toDouble());
         ui->Check_City_CityRating->setChecked(true);
     }
     else
     {
          ui->Check_City_CityRating->setChecked(false);
     }

     //If the data is there, set gov combos and check the flag
     if(ui->Table_City->item(row,3) != 0 )
     {
         ui->Combo_City_Government->setCurrentIndex(
                     ui->Table_City->item(row,3)->text().toInt()+2);
         ui->Check_City_Gov->setChecked(true);
     }
     else
     {
         ui->Check_City_Gov->setChecked(false);
     }

}

//City Add or Edit Clicked
void TurnEventsEditor::on_Button_City_AddEdit_clicked()
{
    //Get turn
    int turn = ui->Spin_City_Month->value() + (ui->Spin_City_Year->value()*12);

    //If turn exists, we look at the list and see if we need to update or insert
    if(turnMap.contains(turn))
    {
        int cityID = ui->Spin_City_CityID->value();
        bool found = false;

        //Get iterator for TurnMap based on the turn
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        //loop through list, if city is there, update
        for(QList<TurnData::TE_CityEvts>::iterator itCE = (*itTM).CityEvtList.begin();
            itCE != (*itTM).CityEvtList.end(); ++itCE)
        {
            if(cityID == (*itCE).id)
            {
                (*itCE).id = ui->Spin_City_CityID->value();
                (*itCE).bInfManuChange = ui->Check_City_CityRating->isChecked();
                (*itCE).bgovChange = ui->Check_City_Gov->isChecked();
                (*itCE).bNationOrFlagChange = ui->Check_City_NationChange->isChecked();

                if((*itCE).bNationOrFlagChange)
                {
                    (*itCE).Flag = ui->Combo_City_Flag_Editable->currentText();
                    (*itCE).Nation = ui->Combo_City_Nation_Editable->currentText();
                }
                if((*itCE).bInfManuChange)
                {
                    (*itCE).infChange = ui->Spin_City_Infra->value();
                    (*itCE).manuChange = ui->Spin_City_Manu->value();
                }

                if((*itCE).bgovChange)
                {
                    //Index 0 is -2 in the game, Index 3 is 1 in game... so we subtract 2
                    (*itCE).gov = ui->Combo_City_Government->currentIndex()-2;
                }


                found = true;
                break;
            }
        }

        //City was not in list, we insert into list
        if(!found)
        {
            TurnData::TE_CityEvts tece;
            tece.id = ui->Spin_City_CityID->value();
            tece.bInfManuChange = ui->Check_City_CityRating->isChecked();
            tece.bgovChange = ui->Check_City_Gov->isChecked();
            tece.bNationOrFlagChange = ui->Check_City_NationChange->isChecked();

            if(tece.bNationOrFlagChange)
            {
                tece.Flag = ui->Combo_City_Flag_Editable->currentText();
                tece.Nation = ui->Combo_City_Nation_Editable->currentText();
            }
            if(tece.bInfManuChange)
            {
                tece.infChange = ui->Spin_City_Infra->value();
                tece.manuChange = ui->Spin_City_Manu->value();
            }

            if(tece.bgovChange)
            {
                //Index 0 is -2 in the game, Index 3 is 1 in game... so we subtract 2
                tece.gov = ui->Combo_City_Government->currentIndex()-2;
            }

            (*itTM).CityEvtList.push_back(tece);
        }

    }
    else //No turn in TurnMap, make a new TE_Data, insert into turn, then insert into list.
    {
        TurnData::TE_Data teda = blankTE_Data;
        TurnData::TE_CityEvts tece;
        tece.id = ui->Spin_City_CityID->value();
        tece.bInfManuChange = ui->Check_City_CityRating->isChecked();
        tece.bgovChange = ui->Check_City_Gov->isChecked();
        tece.bNationOrFlagChange = ui->Check_City_NationChange->isChecked();

        if(tece.bNationOrFlagChange)
        {
            tece.Flag = ui->Combo_City_Flag_Editable->currentText();
            tece.Nation = ui->Combo_City_Nation_Editable->currentText();
        }
        if(tece.bInfManuChange)
        {
            tece.infChange = ui->Spin_City_Infra->value();
            tece.manuChange = ui->Spin_City_Manu->value();
        }

        if(tece.bgovChange)
        {
            //Index 0 is -2 in the game, Index 3 is 1 in game... so we subtract 2
            tece.gov = ui->Combo_City_Government->currentIndex()-2;
        }

        teda.CityEvtList.push_back(tece);
        turnMap.insert(turn,teda);
    }

    //Refresh City Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshCityEventsTable();

}

//City Remove Clicked
void TurnEventsEditor::on_Button_City_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_City_Month->value() + (ui->Spin_City_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        int selectionID = ui->Spin_City_CityID->value();
        QList<TurnData::TE_CityEvts>::iterator itToDelete;
        bool found = false;
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        for(QList<TurnData::TE_CityEvts>::iterator itCE = (*itTM).CityEvtList.begin();
            itCE != (*itTM).CityEvtList.end(); ++itCE)
        {
            if(selectionID == (*itCE).id)
            {
                itToDelete = itCE;
                found = true;
            }
        }

        if(found)
        {
            (*itTM).CityEvtList.erase(itToDelete);
        }

    }

    //Refresh City Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshCityEventsTable();
}

//News Table clicked
void TurnEventsEditor::on_Table_News_cellClicked(int row, int column)
{
    QStringList dateStrSplitList;
    int month = 1;
    int year = 1900;

    //Split Date in Cell and assign it to the spinners
    dateStrSplitList = ui->Table_News->item(row,0)->text().split("/");
    for(QStringList::iterator it = dateStrSplitList.begin(); it != dateStrSplitList.end(); ++it)
    {
        if(it == dateStrSplitList.begin())
        {
            month = (*it).toInt();
        }
        else
        {
            year = (*it).toInt();
        }
    }

    ui->Spin_News_Month->setValue(month);
    ui->Spin_News_Year->setValue(year);


    /*IF the news has been localized, set the various data in the widgets then check if localization
    * manager has been setup. If so, put the translation words into the text boxes,
    * but disable the text boxes. If Localization Manager is not set, we just use the numbers.
    */
    if(ui->Table_News->item(row,1) != 0 && ui->Table_News->item(row,1)->text().toInt() == 1 )
    {
     ui->Check_News_Locale->setChecked(ui->Table_News->item(row,1)->text().toInt());
     ui->Spin_News_HeadlineWord->setValue(ui->Table_News->item(row,2)->text().toInt());    
     ui->Spin_News_BodyWord->setValue(ui->Table_News->item(row,3)->text().toInt());  
     ui->Line_News_Image->setText(ui->Table_News->item(row,4)->text());
     ui->Check_News_Glow->setChecked(ui->Table_News->item(row,5)->text().toInt());

     if(localeManager != 0)
     {
       ui->Line_News_Headline->setText(localeManager->getWord(ui->Spin_News_HeadlineWord->value()));
       ui->TextEdit_News_Body->setText(localeManager->getWord(ui->Spin_News_BodyWord->value()));
     }
     else
     {
         ui->Line_News_Headline->setText("No Localization File Loaded");
         ui->TextEdit_News_Body->setText("No Localization File Loaded");
     }

     //Make header line and body text editable.
     ui->Line_News_Headline->setEnabled(false);
     ui->TextEdit_News_Body->setEnabled(false);
    }
    else //News is not localized, allow body and header to be editable.
    {
        ui->Check_News_Locale->setChecked(0);
        ui->Line_News_Headline->setEnabled(true);
        ui->TextEdit_News_Body->setEnabled(true);
        ui->Spin_News_HeadlineWord->setValue(0);
        ui->Spin_News_BodyWord->setValue(0);
        ui->Line_News_Headline->setText(ui->Table_News->item(row,2)->text());
        ui->TextEdit_News_Body->setText(ui->Table_News->item(row,3)->text());
        ui->Line_News_Image->setText(ui->Table_News->item(row,4)->text());
        ui->Check_News_Glow->setChecked(ui->Table_News->item(row,5)->text().toInt());
    }



    
}

//News Add or Edit Clicked
void TurnEventsEditor::on_Button_News_Add_clicked()
{
    //Get turn
    int turn = ui->Spin_News_Month->value() + (ui->Spin_News_Year->value()*12);

    //If turn exists, we look at the list and see if we need to update or insert
    if(turnMap.contains(turn))
    {
        QString heading = "", body = "", image = "";

        if(ui->Check_News_Locale->isChecked())
        {
          heading = QString::number(ui->Spin_News_HeadlineWord->value());
          body = QString::number(ui->Spin_News_BodyWord->value());
        }
        else
        {
          heading = ui->Line_News_Headline->text();
          body = ui->TextEdit_News_Body->toPlainText();
        }

        image = ui->Line_News_Image->text();

        bool found = false;

        //Get iterator for TurnMap based on the turn
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        //loop through list, if news is there, update
        for(QList<TurnData::TE_NewsEvts>::iterator itNE = (*itTM).NewsEvtList.begin();
            itNE != (*itTM).NewsEvtList.end(); ++itNE)
        {
            //Since there is no "ID" we check against headline and body text at this turn...
            if(heading == (*itNE).headline && body == (*itNE).body)
            {
                //If localization is checked.
                if(ui->Check_News_Locale->isChecked())
                {
                    (*itNE).body = QString::number(ui->Spin_News_BodyWord->value());
                    (*itNE).headline = QString::number(ui->Spin_News_HeadlineWord->value());
                    (*itNE).localization = true;
                }
                else
                {
                    (*itNE).body = ui->TextEdit_News_Body->toPlainText();
                    (*itNE).headline = ui->Line_News_Headline->text();
                    (*itNE).localization = false;
                }
                (*itNE).glow = ui->Check_News_Glow->isChecked();
                (*itNE).image = ui->Line_News_Image->text();

                (*itNE).type = 1;
                found = true;
                break;
            }
        }

        //News was not in list, we insert into list
        if(!found)
        {
            TurnData::TE_NewsEvts tene;

            //If localization is checked.
            if(ui->Check_News_Locale->isChecked())
            {
                tene.body = QString::number(ui->Spin_News_BodyWord->value());
                tene.headline = QString::number(ui->Spin_News_HeadlineWord->value());
                tene.localization = true;
            }
            else
            {
                tene.body = ui->TextEdit_News_Body->toPlainText();
                tene.headline = ui->Line_News_Headline->text();
                tene.localization = false;
            }
            tene.glow = ui->Check_News_Glow->isChecked();
            tene.image = ui->Line_News_Image->text();

            tene.type = 1;
            (*itTM).NewsEvtList.push_back(tene);
        }

    }
    else //No turn in TurnMap, make a new TE_Data, insert into turn, then insert into list.
    {
        TurnData::TE_Data teda = blankTE_Data;
        TurnData::TE_NewsEvts tene;

         //If localization is checked.
        if(ui->Check_News_Locale->isChecked())
        {
            tene.body = QString::number(ui->Spin_News_BodyWord->value());
            tene.headline = QString::number(ui->Spin_News_HeadlineWord->value());
            tene.localization = true;
        }
        else
        {
            tene.body = ui->TextEdit_News_Body->toPlainText();
            tene.headline = ui->Line_News_Headline->text();
            tene.localization = false;
        }
        tene.glow = ui->Check_News_Glow->isChecked();
        tene.image = ui->Line_News_Image->text();

        tene.type = 1;
        teda.NewsEvtList.push_back(tene);
        turnMap.insert(turn,teda);
    }

    //Refresh News Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshNewsPaperTable();
}

//Edit selected news entry
void TurnEventsEditor::on_Button_News_Edit_clicked()
{
    //Get Turn
    int turn = ui->Spin_News_Month->value() + (ui->Spin_News_Year->value()*12);

    //Find turn in the map
    if(turnMap.contains(turn))
    {
        int row = ui->Table_News->currentRow();
        if(row == -1)
            return;

        //This is the old heading/body/image we will use this to find the value in the news list
        QString heading = ui->Table_News->item(row,2)->text();
        QString body = ui->Table_News->item(row,3)->text();
        QString image = ui->Table_News->item(row,4)->text();


        bool found = false;
        //Make iterator for TurnMap for selected turn
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        //loop through news list
        for(QList<TurnData::TE_NewsEvts>::iterator itNE = (*itTM).NewsEvtList.begin();
            itNE != (*itTM).NewsEvtList.end(); ++itNE)
        {
            //if we find an entry with the old headline/body/image, update that entry.
            if(heading == (*itNE).headline && body == (*itNE).body && (*itNE).image == image)
            {
                if(ui->Check_News_Locale->isChecked())
                {
                    (*itNE).body = QString::number(ui->Spin_News_BodyWord->value());
                    (*itNE).headline = QString::number(ui->Spin_News_HeadlineWord->value());
                    (*itNE).localization = true;
                }
                else
                {
                    (*itNE).body = ui->TextEdit_News_Body->toPlainText();
                    (*itNE).headline = ui->Line_News_Headline->text();
                    (*itNE).localization = false;
                }
                (*itNE).glow = ui->Check_News_Glow->isChecked();
                (*itNE).image = ui->Line_News_Image->text();

                (*itNE).type = 1;
                found = true;
                break;
            }
        }


    }

    //Refresh News Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshNewsPaperTable();
}

//News Remove Clicked
void TurnEventsEditor::on_Button_News_Remove_clicked()
{
    //Get Turn
    int turn = ui->Spin_News_Month->value() + (ui->Spin_News_Year->value()*12);

    //Find it in Map and Remove
    if(turnMap.contains(turn))
    {
        int row = ui->Table_News->currentRow();
        if(row == -1)
            return;

        QString heading = ui->Table_News->item(row,2)->text();
        QString body = ui->Table_News->item(row,3)->text();
        QString image = ui->Table_News->item(row,4)->text();

        QList<TurnData::TE_NewsEvts>::iterator itToDelete;
        bool found = false;
        QMap<int,TurnData::TE_Data>::iterator itTM = turnMap.find(turn);

        for(QList<TurnData::TE_NewsEvts>::iterator itNE = (*itTM).NewsEvtList.begin();
            itNE != (*itTM).NewsEvtList.end(); ++itNE)
        {

            if(heading == (*itNE).headline &&  body == (*itNE).body && (*itNE).image == image)
            {
                    itToDelete = itNE;
                    found = true;
            }
        }

        if(found)
        {
            (*itTM).NewsEvtList.erase(itToDelete);
        }

    }

    //Refresh News Table and Tree
    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;

    ui->Tree_All_Everything->clear();
    TurnEventTreeGrower(ui->Tree_All_Everything,turnMap, cityMap, startYearLimit, finishYearLimit,
                        ui->Combo_Components_PredefinedSelector, ui->Combo_Car_SelectionID,
                        localeManager);
    refreshNewsPaperTable();
}

//Save File Button
void TurnEventsEditor::on_Button_SaveList_clicked()
{
    //Get File Name
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".xml"))
            saveFileName += ".xml";

        ui->Label_CurrentSaveFiles->setText(saveFileName);

        //Actually Save File
        saveXML(saveFileName);
    }
}


void TurnEventsEditor::saveXML(QString saveFileName)
{
    //Open Save File
    QFile saveTurnFile(saveFileName);
    if(!saveTurnFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(saveFileName,
                             saveTurnFile.errorString()));

       return;
    }

    int startYearLimit = ui->Spin_StartingYear->value()*12;
    int finishYearLimit = ui->Spin_FinishYear->value()*12;


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveTurnFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("Evts");
    int month=0, year=0;
    bool yearOpen = false;

    //Loop through Map and write the data
    for(QMap<int,TurnData::TE_Data>::iterator it = turnMap.begin(); it != turnMap.end(); ++it)
    {
        //Check to make sure we're only saving the data between the starting year and finish year
        if(it.key()<startYearLimit-12 || it.key()>finishYearLimit)
            continue;

        if(year != it.key()/12)
        {
            if(it.key()%12 != 0)
            {

                if(yearOpen)
                    xmlWriter.writeEndElement(); //Year


                year = it.key()/12;
                xmlWriter.writeStartElement("year");
                       xmlWriter.writeAttribute("y",QString::number(year));
                yearOpen = true;
            }
        }



        month = it.key()%12;
        if(month == 0)
        {
            month = 12;
        }


          xmlWriter.writeStartElement("turn");
               xmlWriter.writeAttribute("t",QString::number(month));

            xmlWriter.writeStartElement("WorldEvts");
              for(QList<TurnData::TE_CityEvts>::iterator itCE = (*it).CityEvtList.begin();
                  itCE != (*it).CityEvtList.end(); ++itCE)
              {
                xmlWriter.writeStartElement("cityChange");
                   xmlWriter.writeAttribute("id",QString::number((*itCE).id));

                   if((*itCE).bgovChange)
                   {
                     xmlWriter.writeAttribute("gov",QString::number((*itCE).gov));
                   }
                   if((*itCE).bInfManuChange)
                   {
                     xmlWriter.writeAttribute("infChange",QString::number((*itCE).infChange));
                     xmlWriter.writeAttribute("manuChange",QString::number((*itCE).manuChange));
                   }
                   if((*itCE).bNationOrFlagChange)
                   {
                       xmlWriter.writeAttribute("Nation",(*itCE).Nation);
                       xmlWriter.writeAttribute("Flag",(*itCE).Flag);
                   }

                   xmlWriter.writeEndElement(); //cityChange
              }

            xmlWriter.writeEndElement(); //WorldEvts


            xmlWriter.writeStartElement("NewsEvts");
              for(QList<TurnData::TE_NewsEvts>::iterator itNE = (*it).NewsEvtList.begin();
                  itNE != (*it).NewsEvtList.end(); ++itNE)
              {
                  xmlWriter.writeStartElement("comment");
                    if((*itNE).glow)
                        xmlWriter.writeAttribute("glow","1");
                    if((*itNE).localization)
                        xmlWriter.writeAttribute("localization","1");
                    else
                        xmlWriter.writeAttribute("localization","0");

                    xmlWriter.writeAttribute("type","1");
                    xmlWriter.writeAttribute("headline",(*itNE).headline);
                    xmlWriter.writeAttribute("body",(*itNE).body);
                    xmlWriter.writeAttribute("image",(*itNE).image);

                  xmlWriter.writeEndElement(); //comment
              }
            xmlWriter.writeEndElement(); //NewsEvts


            xmlWriter.writeStartElement("GameEvts");
                if((*it).officeFile != "")
                {
                    xmlWriter.writeStartElement("office");
                      xmlWriter.writeAttribute("file",(*it).officeFile);
                    xmlWriter.writeEndElement(); //office
                }

                if((*it).buyerRate > -1)
                {
                    xmlWriter.writeStartElement("buyrate");
                      xmlWriter.writeAttribute("rate",QString::number((*it).buyerRate));
                    xmlWriter.writeEndElement(); //buyrate
                }

                if((*it).pensionRate > -1)
                {
                    xmlWriter.writeStartElement("pensionGrowth");
                      xmlWriter.writeAttribute("rate",QString::number((*it).pensionRate));
                    xmlWriter.writeEndElement(); //pensionGrowth
                }

                if((*it).stockRate > -1)
                {
                    xmlWriter.writeStartElement("stockrate");
                      xmlWriter.writeAttribute("rate",QString::number((*it).stockRate));
                    xmlWriter.writeEndElement(); //stockrate
                }

                if((*it).globalInterest > -1)
                {
                    xmlWriter.writeStartElement("interest");
                      xmlWriter.writeAttribute("global",QString::number((*it).globalInterest));
                    xmlWriter.writeEndElement(); //interest
                }

                if((*it).globalFuelRate > -1)
                {
                    xmlWriter.writeStartElement("gas");
                      xmlWriter.writeAttribute("rate",QString::number((*it).globalFuelRate));
                    xmlWriter.writeEndElement(); //gas
                }

                if((*it).carInflation > -1)
                {
                    xmlWriter.writeStartElement("carprice");
                      xmlWriter.writeAttribute("rate",QString::number((*it).carInflation));
                    xmlWriter.writeEndElement(); //carprice
                }

                for(QList<TurnData::TE_ComponentEvts>::iterator itCE = (*it).CompEvtList.begin();
                    itCE != (*it).CompEvtList.end(); ++itCE)
                {
                    xmlWriter.writeStartElement("component");
                      xmlWriter.writeAttribute("selectedIndex",QString::number((*itCE).selectedID));
                      xmlWriter.writeAttribute("popChange",QString::number((*itCE).popChange));
                    xmlWriter.writeEndElement(); //component
                }

                for(QList<TurnData::TE_VehiclePop>::iterator itVP = (*it).vehiclePopList.begin();
                    itVP != (*it).vehiclePopList.end(); ++itVP)
                {
                    xmlWriter.writeStartElement("vehiclepop");
                      xmlWriter.writeAttribute("selectedIndex",QString::number((*itVP).selectorID));
                      xmlWriter.writeAttribute("pop",QString::number((*itVP).pop));
                      xmlWriter.writeAttribute("popR1",QString::number((*itVP).popR1));
                      xmlWriter.writeAttribute("popR2",QString::number((*itVP).popR2));
                      xmlWriter.writeAttribute("popR3",QString::number((*itVP).popR3));
                      xmlWriter.writeAttribute("popR4",QString::number((*itVP).popR4));
                      xmlWriter.writeAttribute("popR5",QString::number((*itVP).popR5));
                      xmlWriter.writeAttribute("popR6",QString::number((*itVP).popR6));
                    xmlWriter.writeEndElement(); //vehiclepop
                }


            xmlWriter.writeEndElement(); //GameEvts

         xmlWriter.writeEndElement(); //Turn



    }

    if(yearOpen)
        xmlWriter.writeEndElement(); //Year

     xmlWriter.writeEndDocument();

     saveTurnFile.close();

     //We Did it! Yay!
     QMessageBox::information(this,"Save Complete",
                              "The Turn File has been saved!", QMessageBox::Ok);


}

void TurnEventsEditor::setTurnEventMap(QMap<int,TurnData::TE_Data> tmpMap)
{
    turnMap = tmpMap;
}
