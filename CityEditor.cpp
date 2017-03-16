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


#include "CityEditor.h"
#include "ui_CityEditor.h"
#include "QLongLongValidator.h"
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <qmath.h>
#include "TurnEventsEditor.h"
#include "AIEditor.h"

//Create City Editor Tool
CityEditor::CityEditor(widgetContainerStorage wsc, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CityEditor)
{
    ui->setupUi(this);

    //Copy widget storage container
    cp_wsc = wsc;

    //Make sure some line editors only get numbers
    ui->LineEdit_CE_Population->setValidator(new qdesigner_internal::QLongLongValidator(this));
    ui->LineEdit_CE_PerCapita->setValidator(new QIntValidator());
    ui->LineEdit_CE_AvgWages->setValidator(new QIntValidator());

    //Allow Table to be sortable
    connect(ui->Table_CE_CitiesInList->horizontalHeader(),SIGNAL(sectionClicked(int)),
            this,SLOT(sectionDoubleClickedSlot(int)));
    ui->Table_CE_CitiesInList->verticalHeader()->setVisible(false);


    //Start a new list
    ui->Button_CE_NewCityList->click();

    //used for saving adjusted turn event.
    turnEventFileName = "";

}

CityEditor::~CityEditor()
{
    delete ui;
}

//Lower City Editor tool and allow the previous tool to be on top
void CityEditor::on_Button_CE_ReturnToPrevious_clicked()
{
    cp_wsc.CityEditorCW->lower();
}

//Open the Map Editor Tool
void CityEditor::on_Button_CE_MapEditor_clicked()
{
    cp_wsc.MapToolCW->raise();
}

//Create a new city list
void CityEditor::on_Button_CE_NewCityList_clicked()
{
    //Clear out all maps and tables, blank out and default the save names and info widgets
    saveFileName = "";

    cityMap.clear();
    interpolateMap.clear();

    ui->SpinBox_CE_CityID->setValue(1);
    ui->LineEdit_CE_CityName->setText("");
    ui->LineEdit_CE_CityCountry->setText("");
    ui->DropDown_CE_ExistingCountries->clear();
    ui->LineEdit_CE_Population->setText("");
    ui->SpinBox_CE_PopulationGrowth->setValue(1);
    ui->LineEdit_CE_PerCapita->setText("");
    ui->SpinBox_CE_EcoGrowth->setValue(1);
    ui->SpinBox_CE_ResourceRates->setValue(1);
    ui->SpinBox_CE_FuelCosts->setValue(1);
    ui->SpinBox_CE_InfRates->setValue(1);
    ui->SpinBox_CE_SkillRates->setValue(1);
    ui->LineEdit_CE_AvgWages->setText("");
    ui->SpinBox_CE_ManufacturingSkills->setValue(1);
    ui->SpinBox_CE_ManuGrowth->setValue(1);
    ui->SpinBox_CE_GovernmentConflict->setValue(1);
    ui->SpinBox_CE_TaxRate->setValue(1);
    ui->SpinBox_CE_BuyerRating->setValue(1);
    ui->LineEdit_CE_Flag->setText("");
    ui->LineEdit_CE_Heading->setText("");
    ui->SpinBox_CE_CordsLong->setValue(0);
    ui->SpinBox_CE_CordsLat->setValue(0);
    ui->Combo_CE_CityRegion->setCurrentIndex(0);
    ui->Combobox_CE_FlagSelector->clear();
    ui->Combobox_CE_HeaderSelector->clear();

    ui->Label_CE_CurrentSaveName->setText("File has not been saved.");
    ui->SpinBox_CE_InterpolateYear->setValue(2020);
    ui->Label_CE_InterpolationSource->setText("Select a later year City Script "
                                              "to interpolate data with.");
    ui->SpinBox_CE_CityListYear->setValue(2020);

    ui->Table_CE_CitiesInList->clearContents();
    ui->Table_CE_CitiesInList->model()->removeRows(0, ui->Table_CE_CitiesInList->rowCount());
}


//Save Add/Edit Change to city
void CityEditor::on_Button_CE_SaveCityChange_clicked()
{

    //Put all the information into the city data storage class
   CityData::dataStore saveDS;
   //saveDS.id = ui->Label_CE_CityID->text().toInt();
   saveDS.id = ui->SpinBox_CE_CityID->value();

   saveDS.cityName = ui->LineEdit_CE_CityName->text();
   saveDS.cityCountry = ui->LineEdit_CE_CityCountry->text();
   saveDS.population = ui->LineEdit_CE_Population->text().toLongLong();
   saveDS.popGrowth = ui->SpinBox_CE_PopulationGrowth->value();
   saveDS.perCapita = ui->LineEdit_CE_PerCapita->text().toInt();
   saveDS.ecoGrowth = ui->SpinBox_CE_EcoGrowth->value();
   saveDS.resourceRates = ui->SpinBox_CE_ResourceRates->value();
   saveDS.fuelCosts = ui->SpinBox_CE_FuelCosts->value();
   saveDS.infRates = ui->SpinBox_CE_InfRates->value();
   saveDS.skillRates = ui->SpinBox_CE_SkillRates->value();
   saveDS.avgWages = ui->LineEdit_CE_AvgWages->text().toInt();
   saveDS.manuSkill = ui->SpinBox_CE_ManufacturingSkills->value();
   saveDS.manuGrowth = ui->SpinBox_CE_ManuGrowth->value();
   saveDS.gov = ui->SpinBox_CE_GovernmentConflict->value();
   saveDS.taxRate = ui->SpinBox_CE_TaxRate->value();
   saveDS.buyerRate = ui->SpinBox_CE_BuyerRating->value();
   saveDS.flagFileName = ui->LineEdit_CE_Flag->text();
   saveDS.headingFileName = ui->LineEdit_CE_Heading->text();
   saveDS.cordLong = ui->SpinBox_CE_CordsLong->value();
   saveDS.cordLat  = ui->SpinBox_CE_CordsLat->value();
   saveDS.region = ui->Combo_CE_CityRegion->currentText().toInt();

   //Check if there are any game breaking errors
   if(!checkNewCityData(saveDS))
   {
       return;
   }

   //Check if city map contains the city, if not add it as new.
     if(cityMap.contains(saveDS.id))
     {
        if(cityMap.find(saveDS.id).value().cityName == saveDS.cityName)
        {
            (*cityMap.find(saveDS.id)) = saveDS;
        }
        else
        {
            QMessageBox::StandardButton answer;
              answer = QMessageBox::question(this, "Overwrite Existing City", "Do you wish to "
              "overwrite the existing city with this ID?",
                                            QMessageBox::Yes|QMessageBox::No);

            if(answer == QMessageBox::Yes)
            {
               (*cityMap.find(saveDS.id)) = saveDS;
            }
            else
            {
                QMap<int,CityData::dataStore> newCityMap;
                for(QMap<int,CityData::dataStore>::Iterator itcm = cityMap.begin();
                    itcm != cityMap.end(); ++itcm)
                {
                    if(itcm.key() >= saveDS.id)
                    {
                        (*itcm).id++;
                        newCityMap.insert(itcm.key()+1,itcm.value());
                    }
                    else
                    {
                        newCityMap.insert(itcm.key(),itcm.value());
                    }
                }

                //Autoadjust turn events
                if(ui->checkBox_CE_AutoAdjustTurnEvents->isChecked())
                {
                    if(!turnEventMap.empty())
                    {
                        for(QMap<int,TurnData::TE_Data>::Iterator it = turnEventMap.begin();
                            it != turnEventMap.end(); ++it)
                        {
                            for(QList<TurnData::TE_CityEvts>::Iterator celIT =
                                (*it).CityEvtList.begin(); celIT != (*it).CityEvtList.end();)
                            {
                                if((*celIT).id >= saveDS.id)
                                {
                                    (*celIT).id++;
                                }

                            }

                        }
                    }
                }

                //Autoadjust AI
                if(ui->checkBox_CE_AutoAdjustAIFiles->isChecked())
                {
                    if(!mapOfAIMaps.empty())
                    {
                        for(QMap<QString,QMap<int, AIManager::AIInfo> >::Iterator it = mapOfAIMaps.begin();
                            it != mapOfAIMaps.end(); ++it)
                        {
                            for(QMap<int, AIManager::AIInfo>::Iterator aiIT = (*it).begin();
                                aiIT != (*it).end(); ++aiIT)
                            {

                                if((*aiIT).hq >= saveDS.id)
                                {
                                    (*aiIT).hq++;
                                }
                            }
                        }
                    }
                }

                //Add to other city files...
                if(ui->checkBox_CE_AutoAdjustCityFiles->isChecked())
                {
                    if(!cityAdjustMap.empty())
                    {
                        for(QMap<QString,QMap<int, CityData::dataStore> >::Iterator it = cityAdjustMap.begin();
                            it != cityAdjustMap.end(); ++it)
                        {
                            QMap<int, CityData::dataStore> rekeyedMap;
                            for(QMap<int, CityData::dataStore>::Iterator cdIT = (*it).begin();
                                cdIT != (*it).end(); ++cdIT)
                            {
                                if((*cdIT).id < saveDS.id)
                                {
                                    rekeyedMap.insert(cdIT.key(),cdIT.value());
                                }
                                else if((*cdIT).id > saveDS.id)
                                {
                                    (*cdIT).id++;
                                    rekeyedMap.insert((*cdIT).id,cdIT.value());
                                }

                            }

                            rekeyedMap.insert(saveDS.id,saveDS);
                            (*it) = rekeyedMap;
                        }
                    }
                }

                cityMap = newCityMap;
                cityMap.insert(saveDS.id,saveDS);
            }
        }
     }
     else
     {
        cityMap.insert(saveDS.id,saveDS);
        //ui->Label_CE_CityID->setText(QString::number(cityMap.size()+1));
        ui->SpinBox_CE_CityID->setValue(cityMap.size()+1);
     }

   //Refresh table list
   fillTableList();

ui->SpinBox_CE_CityID->setMaximum(cityMap.size()+1);

}

//Check if there are any game breaking errors, if so pop a message and return a false
bool CityEditor::checkNewCityData(CityData::dataStore saveDS)
{
    if(saveDS.cityName == "")
    {
        QMessageBox::critical(this,"Error!","City Name must not be blank!");
        return false;
    }
    else if(saveDS.cityCountry == "")
    {
        QMessageBox::critical(this,"Error!","Country Name must not be blank!");
        return false;
    }
    else if(saveDS.population <= 0)
    {
        QMessageBox::critical(this,"Error!","Population must be greater than 0!");
        return false;
    }
    else if(saveDS.perCapita <= 0)
    {
        QMessageBox::critical(this,"Error!","Per Capita must be greater than 0!");
        return false;
    }
    else if(saveDS.avgWages <= 0)
    {
        QMessageBox::critical(this,"Error!","Average Wages must be greater than 0!");
        return false;
    }
    else if(saveDS.flagFileName == "" || !saveDS.flagFileName.contains(".dds"))
    {
        if(saveDS.flagFileName == "")
        {
            QMessageBox::critical(this,"Error!","Flag file must not be blank!");
            return false;
        }
        else
        {
            QMessageBox::critical(this,"Error!","Flag file must be in dds format,"
                                  "and the file name must have .dds extension!");
            return false;
        }
    }
    else if(saveDS.headingFileName == "" || !saveDS.headingFileName.contains(".dds"))
    {
        if(saveDS.headingFileName == "")
        {
            QMessageBox::critical(this,"Error!","Heading file must not be blank!");
            return false;
        }
        else
        {
            QMessageBox::critical(this,"Error!","heading file must be in dds format,"
                                  "and the file name must have .dds extension!");
            return false;
        }
    }

    if(!cityMap.contains(saveDS.id))
    {
        for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
        {
         if(saveDS.cityName == (*it).cityName)
         {
            QMessageBox::critical(this,"Error!","City Name must be unique!"
                                  "This City is currently being used!");
            return false;
         }
        }
    }

    return true;
}


//Clear and Refill table and combo boxes
void CityEditor::fillTableList()
{
    ui->Table_CE_CitiesInList->clearContents();
    ui->Table_CE_CitiesInList->setRowCount(cityMap.size());

    //Existing Country Combobox
    QStringList comboCountryList;
    ui->DropDown_CE_ExistingCountries->clear();
    ui->DropDown_CE_ExistingCountries->addItem("");


    //Existing Flag Combobox
    QStringList comboFlagList;
    ui->Combobox_CE_FlagSelector->clear();
    ui->Combobox_CE_FlagSelector->addItem("");

    //Existing Head Combobox
    QStringList comboHeadList;
    ui->Combobox_CE_HeaderSelector->clear();
    ui->Combobox_CE_HeaderSelector->addItem("");

    for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
    {
        //Only do this for numbered items so sorting works properly...
        QTableWidgetItem *itemID = new QTableWidgetItem;
        QTableWidgetItem *itemPopulation = new QTableWidgetItem;
        QTableWidgetItem *itemPerCapita = new QTableWidgetItem;
        QTableWidgetItem *itemGov = new QTableWidgetItem;

        itemID->setData(Qt::EditRole,(*it).id);
        itemPopulation->setData(Qt::EditRole,(*it).population);
        itemPerCapita->setData(Qt::EditRole,(*it).perCapita);
        itemGov->setData(Qt::EditRole,(*it).gov);


        //ID, Name, Country, Population, PerCapita, Gov
        ui->Table_CE_CitiesInList->setItem((*it).id-1,0,itemID);
        ui->Table_CE_CitiesInList->setItem((*it).id-1,1,new QTableWidgetItem((*it).cityName));
        ui->Table_CE_CitiesInList->setItem((*it).id-1,2,new QTableWidgetItem((*it).cityCountry));
        ui->Table_CE_CitiesInList->setItem((*it).id-1,3,itemPopulation);
        ui->Table_CE_CitiesInList->setItem((*it).id-1,4,itemPerCapita);
        ui->Table_CE_CitiesInList->setItem((*it).id-1,5,itemGov);

        //Existing Country Combobox Checker and adder
        if(!comboCountryList.contains((*it).cityCountry))
        {
          ui->DropDown_CE_ExistingCountries->addItem((*it).cityCountry);
          comboCountryList.push_back((*it).cityCountry);
        }

        //Existing Flag Combobox Checker and adder
        if(!comboFlagList.contains((*it).flagFileName))
        {
          ui->Combobox_CE_FlagSelector->addItem((*it).flagFileName);
          comboFlagList.push_back((*it).flagFileName);
        }

        //Existing Header Combobox Checker and adder
        if(!comboHeadList.contains((*it).headingFileName))
        {
          ui->Combobox_CE_HeaderSelector->addItem((*it).headingFileName);
          comboHeadList.push_back((*it).headingFileName);
        }
    }
}


//Allows table to be sortable
void CityEditor::sectionDoubleClickedSlot(int index)
{
     ui->Table_CE_CitiesInList->sortByColumn(index,Qt::AscendingOrder);
}

//Table was clicked, fill the info in the info widgets
void CityEditor::on_Table_CE_CitiesInList_clicked(const QModelIndex &index)
{
    //Get table row
    int selectedRow = index.row();
    int selectedID = ui->Table_CE_CitiesInList->item(selectedRow,0)->text().toInt();

    //Get map iterator
    QMap<int,CityData::dataStore>::iterator mapIT = cityMap.find(selectedID);

    //Fill widget infos.
    //ui->Label_CE_CityID->setText(QString::number((*mapIT).id));
    ui->SpinBox_CE_CityID->setValue((*mapIT).id);
    ui->LineEdit_CE_CityName->setText((*mapIT).cityName);
    ui->DropDown_CE_ExistingCountries->setCurrentIndex(
                ui->DropDown_CE_ExistingCountries->findText((*mapIT).cityCountry));
    ui->LineEdit_CE_CityCountry->setText((*mapIT).cityCountry);
    ui->LineEdit_CE_Population->setText(QString::number((*mapIT).population));
    ui->SpinBox_CE_PopulationGrowth->setValue((*mapIT).popGrowth);
    ui->LineEdit_CE_PerCapita->setText(QString::number((*mapIT).perCapita));
    ui->SpinBox_CE_EcoGrowth->setValue((*mapIT).ecoGrowth);
    ui->SpinBox_CE_ResourceRates->setValue((*mapIT).resourceRates);
    ui->SpinBox_CE_FuelCosts->setValue((*mapIT).fuelCosts);
    ui->SpinBox_CE_InfRates->setValue((*mapIT).infRates);
    ui->SpinBox_CE_SkillRates->setValue((*mapIT).skillRates);
    ui->LineEdit_CE_AvgWages->setText(QString::number((*mapIT).avgWages));
    ui->SpinBox_CE_ManufacturingSkills->setValue((*mapIT).manuSkill);
    ui->SpinBox_CE_ManuGrowth->setValue((*mapIT).manuGrowth);
    ui->SpinBox_CE_GovernmentConflict->setValue((*mapIT).gov);
    ui->SpinBox_CE_TaxRate->setValue((*mapIT).taxRate);
    ui->SpinBox_CE_BuyerRating->setValue((*mapIT).buyerRate);
    ui->LineEdit_CE_Flag->setText((*mapIT).flagFileName);
    ui->LineEdit_CE_Heading->setText((*mapIT).headingFileName);
    ui->SpinBox_CE_CordsLong->setValue((*mapIT).cordLong);
    ui->SpinBox_CE_CordsLat->setValue((*mapIT).cordLat);
    ui->Combo_CE_CityRegion->setCurrentIndex((*mapIT).region-1);


}

//Set Country name by selecting an existing country from drop down box
void CityEditor::on_DropDown_CE_ExistingCountries_currentIndexChanged(const QString &arg1)
{
    if(arg1=="")
        return;

  ui->LineEdit_CE_CityCountry->setText(arg1);
}

//Set flag name by selecting an existing flag from drop down box
void CityEditor::on_Combobox_CE_FlagSelector_currentIndexChanged(const QString &arg1)
{
    if(arg1=="")
        return;
    ui->LineEdit_CE_Flag->setText(arg1);
}

//Set header image name by selecting an existing header from drop down box
void CityEditor::on_Combobox_CE_HeaderSelector_currentIndexChanged(const QString &arg1)
{
    if(arg1=="")
        return;
    ui->LineEdit_CE_Heading->setText(arg1);
}

//Make a new city, reset city widget information
void CityEditor::on_Button_CE_MakeNewCity_clicked()
{
    //ui->Label_CE_CityID->setText(QString::number(cityMap.size()+1));
    ui->SpinBox_CE_CityID->setValue(cityMap.size()+1);
    ui->LineEdit_CE_CityName->setText("");
    ui->LineEdit_CE_CityCountry->setText("");
    ui->DropDown_CE_ExistingCountries->clear();
    ui->LineEdit_CE_Population->setText("");
    ui->SpinBox_CE_PopulationGrowth->setValue(1);
    ui->LineEdit_CE_PerCapita->setText("");
    ui->SpinBox_CE_EcoGrowth->setValue(1);
    ui->SpinBox_CE_ResourceRates->setValue(1);
    ui->SpinBox_CE_FuelCosts->setValue(1);
    ui->SpinBox_CE_InfRates->setValue(1);
    ui->SpinBox_CE_SkillRates->setValue(1);
    ui->LineEdit_CE_AvgWages->setText("");
    ui->SpinBox_CE_ManufacturingSkills->setValue(1);
    ui->SpinBox_CE_ManuGrowth->setValue(1);
    ui->SpinBox_CE_GovernmentConflict->setValue(1);
    ui->SpinBox_CE_TaxRate->setValue(1);
    ui->SpinBox_CE_BuyerRating->setValue(1);
    ui->LineEdit_CE_Flag->setText("");
    ui->LineEdit_CE_Heading->setText("");
    ui->SpinBox_CE_CordsLong->setValue(0);
    ui->SpinBox_CE_CordsLat->setValue(0);
    ui->Combo_CE_CityRegion->setCurrentIndex(0);
}

//Clone selected city from table
void CityEditor::on_Button_CE_NewCloneCity_clicked()
{
    //ui->Label_CE_CityID->setText(QString::number(cityMap.size()+1));
    ui->SpinBox_CE_CityID->setValue(cityMap.size()+1);
    ui->LineEdit_CE_CityName->setText("");
}

//Remove selected city from city map
void CityEditor::on_Button_CE_RemoveSelectedCity_clicked()
{
    /*if(ui->Table_CE_CitiesInList->selectedItems().empty())
        return;*/

    //int selectedRow = ui->Table_CE_CitiesInList->selectionModel()->currentIndex().row();
    int selectedID = ui->SpinBox_CE_CityID->value();
    //ui->Table_CE_CitiesInList->item(selectedRow,0)->text().toInt();

    cityMap.remove(selectedID);

    //now we must rekey. Since this isn't performance impared we'll do
    //the easiest way to rekey many keys in a map...

    QMap<int,CityData::dataStore> tmpMap;

    for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
    {

      if(it.key() > selectedID)
      {
          (*it).id = it.key()-1;
          tmpMap.insert(it.key()-1,(*it));
      }
      else
      {
          tmpMap.insert(it.key(),(*it));
      }
    }

    cityMap.clear();
    cityMap = tmpMap;

    //ui->Label_CE_CityID->setText(QString::number(cityMap.size()+1));
    ui->SpinBox_CE_CityID->setValue(cityMap.size()+1);

    //If turnevents adjustment has been selected, and there is turn events data loaded. Lets re-id
    //city events.
    bool deleteIT = false;


    if(ui->checkBox_CE_AutoAdjustTurnEvents->isChecked())
    {
        if(!turnEventMap.empty())
        {
            for(QMap<int,TurnData::TE_Data>::Iterator it = turnEventMap.begin();
                it != turnEventMap.end(); ++it)
            {
                for(QList<TurnData::TE_CityEvts>::Iterator celIT = (*it).CityEvtList.begin();
                    celIT != (*it).CityEvtList.end();)
                {
                    if((*celIT).id == selectedID)
                    {
                        deleteIT = true;
                    }
                    else if((*celIT).id > selectedID)
                    {
                        (*celIT).id--;
                    }



                    if(deleteIT)
                    {
                        deleteIT = false;
                        celIT = (*it).CityEvtList.erase(celIT);
                    }
                    else
                    {
                        ++celIT;
                    }
                }

            }
        }
    }

    //Now we do the same for AI, if the adjustment is selected, we go through each AI file and
    //Change the hq locations.
    if(ui->checkBox_CE_AutoAdjustAIFiles->isChecked())
    {
        if(!mapOfAIMaps.empty())
        {
            for(QMap<QString,QMap<int, AIManager::AIInfo> >::Iterator it = mapOfAIMaps.begin();
                it != mapOfAIMaps.end(); ++it)
            {
                for(QMap<int, AIManager::AIInfo>::Iterator aiIT = (*it).begin();
                    aiIT != (*it).end(); ++aiIT)
                {

                    if((*aiIT).hq > selectedID)
                    {
                        (*aiIT).hq--;
                    }
                }
            }
        }
    }

    //Finally Clean out other City Files
    deleteIT = false;

    if(ui->checkBox_CE_AutoAdjustCityFiles->isChecked())
    {
        if(!cityAdjustMap.empty())
        {
            for(QMap<QString,QMap<int, CityData::dataStore> >::Iterator it = cityAdjustMap.begin();
                it != cityAdjustMap.end(); ++it)
            {
                QMap<int, CityData::dataStore> rekeyedMap;
                for(QMap<int, CityData::dataStore>::Iterator cdIT = (*it).begin();
                    cdIT != (*it).end();  ++cdIT)
                {
                    if((*cdIT).id < selectedID)
                    {
                        rekeyedMap.insert(cdIT.key(),cdIT.value());
                    }
                    else if((*cdIT).id > selectedID)
                    {
                        (*cdIT).id--;
                        rekeyedMap.insert((*cdIT).id,cdIT.value());
                    }

                }

                (*it) = rekeyedMap;
            }
        }
    }

    fillTableList();

    ui->SpinBox_CE_CityID->setMaximum(cityMap.size()+1);
    if(selectedID <= cityMap.size())
    {
        QModelIndex index = ui->Table_CE_CitiesInList->model()->index(selectedID-1,0,QModelIndex());
        on_Table_CE_CitiesInList_clicked(index);
     }
}

//Set save file name
void CityEditor::on_Button_CE_SaveCityList_clicked()
{    
    saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".xml"))
            saveFileName += ".xml";

        ui->Label_CE_CurrentSaveName->setText(saveFileName);

        //Save the city script xml
        saveXML();
    }

    if(ui->checkBox_CE_AutoAdjustTurnEvents->isChecked() && turnEventFileName != "")
    {
       TurnEventsEditor* tee = new TurnEventsEditor(cp_wsc,cp_wsc.CityEditorCW);
           tee->setTurnEventMap(turnEventMap);
           tee->saveXML(turnEventFileName);
       delete tee;
    }

    if(ui->checkBox_CE_AutoAdjustAIFiles->isChecked())
    {
        for(QMap<QString,QMap<int, AIManager::AIInfo> >::Iterator it = mapOfAIMaps.begin();
            it != mapOfAIMaps.end(); ++it)
        {
            AIEditor* aie = new AIEditor(cp_wsc,cp_wsc.CityEditorCW);
                aie->setSaveFileNameExternally(it.key());
                aie->setAIMapExternally(it.value());
                aie->saveXML();
             delete aie;
        }
    }

    if(ui->checkBox_CE_AutoAdjustCityFiles->isChecked())
    {
        for(QMap<QString,QMap<int, CityData::dataStore> >::Iterator it = cityAdjustMap.begin();
            it != cityAdjustMap.end(); ++it)
        {
            CityEditor* ce = new CityEditor(cp_wsc,cp_wsc.CityEditorCW);
                ce->setSaveFileNameExternally(it.key());
                ce->setCityMapExternally((*it));
                ce->saveXML();
            delete ce;
        }
    }
}

//This is the function that actually saves the city script to file.
void CityEditor::saveXML()
{
    //Open/Create the save file
    QFile saveCityFile(saveFileName);
    if(!saveCityFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(saveFileName,
                             saveCityFile.errorString()));

       return;
    }


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveCityFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("CreateCityDB");

    for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
    {
        xmlWriter.writeStartElement("City");
            xmlWriter.writeStartElement("ID");
               xmlWriter.writeAttribute("id",QString::number((*it).id));
            xmlWriter.writeEndElement(); //ID

            xmlWriter.writeStartElement("NAME");
               xmlWriter.writeAttribute("name",(*it).cityName);
            xmlWriter.writeEndElement(); //NAME

            xmlWriter.writeStartElement("COUNTRY");
               xmlWriter.writeAttribute("nation",(*it).cityCountry);
            xmlWriter.writeEndElement(); //COUNTRY

            xmlWriter.writeStartElement("POPULATION");
               xmlWriter.writeAttribute("pop",QString::number((*it).population));
            xmlWriter.writeEndElement(); //POPULATION

            xmlWriter.writeStartElement("POPGROWTH");
               xmlWriter.writeAttribute("pg",QString::number((*it).popGrowth));
            xmlWriter.writeEndElement(); //POPGROWTH

            xmlWriter.writeStartElement("CAPITA");
               xmlWriter.writeAttribute("cap",QString::number((*it).perCapita));
            xmlWriter.writeEndElement(); //CAPITA

            xmlWriter.writeStartElement("ECOGROWTH");
               xmlWriter.writeAttribute("ecog",QString::number((*it).ecoGrowth));
            xmlWriter.writeEndElement(); //ECOGROWTH

            xmlWriter.writeStartElement("RESOURCES");
               xmlWriter.writeAttribute("res",QString::number((*it).resourceRates));
            xmlWriter.writeEndElement(); //RESOURCES

            xmlWriter.writeStartElement("GAS");
               xmlWriter.writeAttribute("gastax",QString::number((*it).fuelCosts));
            xmlWriter.writeEndElement(); //GAS

            xmlWriter.writeStartElement("INF");
               xmlWriter.writeAttribute("infit",QString::number((*it).infRates));
            xmlWriter.writeEndElement(); //INF

            xmlWriter.writeStartElement("SKILL");
               xmlWriter.writeAttribute("ski",QString::number((*it).skillRates));
            xmlWriter.writeEndElement(); //SKILL

            xmlWriter.writeStartElement("WAGE");
               xmlWriter.writeAttribute("empwage",QString::number((*it).avgWages));
            xmlWriter.writeEndElement(); //WAGE

            xmlWriter.writeStartElement("MNFBASE");
               xmlWriter.writeAttribute("manub",QString::number((*it).manuSkill));
            xmlWriter.writeEndElement(); //MNFBASE

            xmlWriter.writeStartElement("MANUGROWTH");
               xmlWriter.writeAttribute("manug",QString::number((*it).manuGrowth));
            xmlWriter.writeEndElement(); //MANUGROWTH

            xmlWriter.writeStartElement("ALIGNMENT");
               xmlWriter.writeAttribute("align","");
            xmlWriter.writeEndElement(); //ALIGNMENT

            xmlWriter.writeStartElement("GOVERN");
               xmlWriter.writeAttribute("gov",QString::number((*it).gov));
            xmlWriter.writeEndElement(); //GOVERN

            xmlWriter.writeStartElement("TAX");
               xmlWriter.writeAttribute("taxrate",QString::number((*it).taxRate));
            xmlWriter.writeEndElement(); //TAX

            xmlWriter.writeStartElement("FLAG");
               xmlWriter.writeAttribute("flagfile",(*it).flagFileName);
            xmlWriter.writeEndElement(); //FLAG

            xmlWriter.writeStartElement("PIC");
               xmlWriter.writeAttribute("picture",(*it).headingFileName);
            xmlWriter.writeEndElement(); //PIC

            xmlWriter.writeStartElement("CORDS");
               xmlWriter.writeAttribute("long",QString::number((*it).cordLong));
               xmlWriter.writeAttribute("lat",QString::number((*it).cordLat));
            xmlWriter.writeEndElement(); //CORDS

            xmlWriter.writeTextElement("REGION",QString::number((*it).region));
            xmlWriter.writeTextElement("BUYRATE",QString::number((*it).buyerRate));

        xmlWriter.writeEndElement(); // City
    }

     xmlWriter.writeEndDocument();

     saveCityFile.close();

     //We Did it! Yay!
     QMessageBox::information(this,"Save Complete",
                              "Your Settings Have Been Saved!", QMessageBox::Ok);


}

//Select City Script to Open
void CityEditor::on_Button_CE_LoadCityList_clicked()
{
   QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","XML Files (*.xml)");

   if (openFileName != "")
   {
       openXML(openFileName,false);
       fillTableList();
       ui->Label_CE_CurrentSaveName->setText(openFileName);
       QModelIndex index = ui->Table_CE_CitiesInList->model()->index(0,0,QModelIndex());
       on_Table_CE_CitiesInList_clicked(index);
   }
   else
   {
       return;
   }
}

//Actually open the selected file and fill the maps
void CityEditor::openXML(QString openFileName, bool interpolater)
{
    //If we're interpolating clear the interpolation map, otherwise city map
    if(interpolater)
    {
        interpolateMap.clear();
    }
    else
    {
        cityMap.clear();
    }

    CityData::dataStore dsData;

    //Open the file
    QFile file(openFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::critical(this,"File Error",
                             QString("Whopse, opening %1 failed.").arg(openFileName));
       return;
    }

    //Process XML File.
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement cityElement;


    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
       QMessageBox::critical(this,"Error City Script",
                            QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                            errorMsg,errorLine,errorColumn));
        return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("CreateCityDB");
    if(rootNode.isNull())
    {
        QMessageBox::critical(this,"Error","City Script is malformed.");
        return;
    }

    //Get first city, and lets go
    rootNode = doc.documentElement();
    cityElement = rootNode.firstChildElement("City");

  do
  {
   dsData.id = cityElement.firstChildElement("ID").attributeNode("id").value().toInt();
   dsData.cityName = cityElement.firstChildElement("NAME").attributeNode("name").value();
   dsData.cityCountry = cityElement.firstChildElement("COUNTRY").attributeNode("nation").value();
   dsData.population = cityElement.firstChildElement("POPULATION").attributeNode("pop").value().toLongLong();
   dsData.popGrowth = cityElement.firstChildElement("POPGROWTH").attributeNode("pg").value().toDouble();
   dsData.perCapita = cityElement.firstChildElement("CAPITA").attributeNode("cap").value().toInt();
   dsData.ecoGrowth = cityElement.firstChildElement("ECOGROWTH").attributeNode("ecog").value().toDouble();
   dsData.resourceRates = cityElement.firstChildElement("RESOURCES").attributeNode("res").value().toDouble();
   dsData.fuelCosts = cityElement.firstChildElement("GAS").attributeNode("gastax").value().toDouble();
   dsData.infRates = cityElement.firstChildElement("INF").attributeNode("infit").value().toInt();
   dsData.skillRates = cityElement.firstChildElement("SKILL").attributeNode("ski").value().toInt();
   dsData.avgWages = cityElement.firstChildElement("WAGE").attributeNode("empwage").value().toInt();
   dsData.manuSkill = cityElement.firstChildElement("MNFBASE").attributeNode("manub").value().toInt();
   dsData.manuGrowth = cityElement.firstChildElement("MANUGROWTH").attributeNode("manug").value().toDouble();
   dsData.gov = cityElement.firstChildElement("GOVERN").attributeNode("gov").value().toInt();
   dsData.taxRate = cityElement.firstChildElement("TAX").attributeNode("taxrate").value().toDouble();
   dsData.flagFileName = cityElement.firstChildElement("FLAG").attributeNode("flagfile").value();
   dsData.headingFileName = cityElement.firstChildElement("PIC").attributeNode("picture").value();
   dsData.cordLat = cityElement.firstChildElement("CORDS").attributeNode("lat").value().toDouble();
   dsData.cordLong = cityElement.firstChildElement("CORDS").attributeNode("long").value().toDouble();
   dsData.region = cityElement.firstChildElement("REGION").text().toInt();
   dsData.buyerRate = cityElement.firstChildElement("BUYRATE").text().toDouble();

   //if we're opening interpolating map, we need to do it here
   if(interpolater)
   {
     //insert into interpolation map
     interpolateMap.insert(dsData.id,dsData);

     //if the city map has the city, we interpolate
     if(cityMap.contains(dsData.id))
     {
        //Get the city from the map
        QMap<int, CityData::dataStore>::iterator it = cityMap.find(dsData.id);

        //double check to make sure that the city has the same lat and long, if not we'll
        // flag the population with -1 so you can fix it
        if(static_cast<int>(dsData.cordLat * 1000) != static_cast<int>((*it).cordLat * 1000) ||
           static_cast<int>(dsData.cordLong * 1000) != static_cast<int>((*it).cordLong * 1000))
        {
            dsData.population = -1;
            it.value() = dsData;
        }

        //interpolate the data
        interpolateValues(dsData.id);
     }
     else //It's not in the map, lets insert.
     {
         cityMap.insert(dsData.id,dsData);
     }
   }
   else// We're not opening an interpolating map, we can directly insert and then interpolate
   {
    cityMap.insert(dsData.id,dsData);
    interpolateValues(dsData.id);
   }

   cityElement = cityElement.nextSiblingElement("City");
  }
  while(!cityElement.isNull());

   file.close();


}

//Open city script to interpole to.
void CityEditor::on_Button_CE_InterpolationSourceFinder_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","XML Files (*.xml)");

    if (openFileName != "")
    {
        openXML(openFileName,true);
        fillTableList();
        ui->Label_CE_InterpolationSource->setText(openFileName);
        QModelIndex index = ui->Table_CE_CitiesInList->model()->index(1,0,QModelIndex());
        on_Table_CE_CitiesInList_clicked(index);
    }
    else
    {
        return;
    }
}


//This is the interpolator. It will adjust the growth values of the city map or widgets
//based on the values in the interpolation map.
void CityEditor::interpolateValues(int key)
{
    //if the city isnt' in the interpolation map, return
    if(!interpolateMap.contains(key))
        return;

    //get interpolation iterator
    QMap<int, CityData::dataStore>::iterator it = interpolateMap.find(key);

    //Are we changing the cityMap? If false, we're changing widgets
    bool editMap = false;
    if(cityMap.contains(key))
        editMap = true;

    double yearDif = ui->SpinBox_CE_InterpolateYear->value() - ui->SpinBox_CE_CityListYear->value();


    //Widgets is the same as city id so we're changing growth values.
    if(ui->SpinBox_CE_CityID->value() == key)
    {
     double populationGrowth = pow(((*it).population / ui->LineEdit_CE_Population->text().toDouble()),
            (1.0/(yearDif)));

     ui->SpinBox_CE_PopulationGrowth->setValue(populationGrowth);


     double capitaGrowth = pow(((*it).perCapita / ui->LineEdit_CE_PerCapita->text().toDouble()),
            (1.0/(yearDif)));


     ui->SpinBox_CE_EcoGrowth->setValue(capitaGrowth);

     double manuGrowth = pow(((*it).manuSkill / static_cast<double>(ui->SpinBox_CE_ManufacturingSkills->value())),
            (1.0/(yearDif)));

     ui->SpinBox_CE_ManuGrowth->setValue(manuGrowth);
    }

    // We're interpolating data in the city maps as well
    if(editMap)
    {
        QMap<int, CityData::dataStore>::iterator cmit = cityMap.find(key);
        double populationGrowth = pow(((*it).population / static_cast<double>((*cmit).population)),
               (1.0/(yearDif)));

        (*cmit).popGrowth = populationGrowth;

        double capitaGrowth = pow(((*it).perCapita / static_cast<double>((*cmit).perCapita)),
               (1.0/(yearDif)));

        (*cmit).ecoGrowth = capitaGrowth;

        double manuGrowth = pow(((*it).manuSkill /static_cast<double>((*cmit).manuSkill)),
               (1.0/(yearDif)));

        (*cmit).manuGrowth = manuGrowth;
    }

}

//Population edited, interpolate if we need to.
void CityEditor::on_LineEdit_CE_Population_textEdited(const QString &arg1)
{
    interpolateValues(ui->SpinBox_CE_CityID->value());
}

//Percapita edited, interpolate if we need to.
void CityEditor::on_LineEdit_CE_PerCapita_textEdited(const QString &arg1)
{
    interpolateValues(ui->SpinBox_CE_CityID->value());
}

//Manufacturing skill edited, interpolate if we need to.
void CityEditor::on_SpinBox_CE_ManufacturingSkills_editingFinished()
{
    interpolateValues(ui->SpinBox_CE_CityID->value());
}

//City edited, interpolate if we need to.
void CityEditor::on_SpinBox_CE_CityListYear_editingFinished()
{
    interpolateValues(ui->SpinBox_CE_CityID->value());
    for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
    {
        interpolateValues(it.key());
    }
}

//Base Year edited, interpolate if we need to.
void CityEditor::on_SpinBox_CE_InterpolateYear_editingFinished()
{
    interpolateValues(ui->SpinBox_CE_CityID->value());
    for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
    {
        interpolateValues(it.key());
    }
}

//Stop interpolating!
void CityEditor::on_Button_CityEditor_CancelInterpolater_clicked()
{
    interpolateMap.clear();
    ui->Label_CE_InterpolationSource->setText("Select a later year City Script"
                                              "to interpolate data with.");
}

//Open AI Editor
void CityEditor::on_Button_CE_AIEditor_clicked()
{
    cp_wsc.AIEditorCW->raise();
}


//Open Turn Events editor
void CityEditor::on_Button_CE_TurnEventsEditor_clicked()
{
    cp_wsc.TurnEventEditorCW->raise();
}

void CityEditor::on_LineEdit_CE_PerCapita_editingFinished()
{
   ui->LineEdit_CE_AvgWages->setText(QString::number(ui->LineEdit_CE_PerCapita->text().toInt()/12));
}


void CityEditor::on_button_CE_AA_SelectTurnEvents_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Turn Events File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        TurnData *td = new TurnData(openFileName, cp_wsc.CityEditorCW,0);
        turnEventMap = td->getTurnMap();
        delete td;
        turnEventFileName = openFileName;
    }


}

void CityEditor::on_button_CE_AA_AddAIFile_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open AI File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        AIManager *ai = new AIManager(openFileName,cp_wsc.CityEditorCW);
        mapOfAIMaps.insert(openFileName, ai->getMap());
        fillAIAdjustList();
        delete ai;

    }
}


void CityEditor::fillAIAdjustList()
{
    ui->list_CE_AA_AIFiles->clear();
    for(QMap<QString,QMap<int, AIManager::AIInfo> >::Iterator it = mapOfAIMaps.begin();
        it != mapOfAIMaps.end(); ++it)
    {
        ui->list_CE_AA_AIFiles->addItem(it.key());
    }

}



void CityEditor::on_button_CE_AA_RemoveAIFile_clicked()
{
    QModelIndex index = ui->list_CE_AA_AIFiles->currentIndex();
    QString text = index.data(Qt::DisplayRole).toString();

    mapOfAIMaps.remove(text);
    fillAIAdjustList();
}

void CityEditor::on_button_CE_AA_AddCity_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open City File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        CityEditor* ce = new CityEditor(cp_wsc,cp_wsc.CityEditorCW);
        ce->openXML(openFileName, false);
        cityAdjustMap.insert(openFileName, ce->getCityMapForExternalUse());
        delete ce;
    }

    fillCityAdjustList();

}

void CityEditor::on_button_CE_AA_RemoveCity_clicked()
{
    QModelIndex index = ui->list_CE_AA_CityList->currentIndex();
    QString text = index.data(Qt::DisplayRole).toString();

    cityAdjustMap.remove(text);
    fillCityAdjustList();
}

QMap<int,CityData::dataStore> CityEditor::getCityMapForExternalUse()
{
    return cityMap;
}

void CityEditor::fillCityAdjustList()
{
    ui->list_CE_AA_CityList->clear();
    for(QMap<QString,QMap<int, CityData::dataStore> >::Iterator it =
        cityAdjustMap.begin(); it != cityAdjustMap.end(); ++it)
    {
        ui->list_CE_AA_CityList->addItem(it.key());
    }

}

void CityEditor::setSaveFileNameExternally(QString fileName)
{
    saveFileName = fileName;
}

void CityEditor::setCityMapExternally(QMap<int,CityData::dataStore> newMap)
{
    cityMap = newMap;
}
