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

#include "ModEditor.h"
#include "ui_ModEditor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>

ModEditor::ModEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModEditor)
{
    ui->setupUi(this);
    //copy widget storage container
    cp_wsc = wsc;

    nameListFile = "";


     //Get currently Installed Maps
  #if defined(Q_WS_WIN)
   QString folderPath = "../media/Maps/";
  #elif defined(Q_WS_X11)
   QString folderPath = "../media/Maps/";
  #elif defined(Q_WS_MACX)
   QString folderPath = OSXHelper::getMacPath(0) + "/media/Maps/";
  #endif


    QDir dir(folderPath);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList fileList = dir.entryList();

    ui->combo_AddNewMap->clear();
    ui->combo_AddNewMap->addItems(fileList);

    nameListFile = "";
    newpaperArticlesFile = "";
    reviewsFile = "";
    componentsFile = "";
    componentsPopFile = "";
    racingFile  = "";
    contractFile  = "";
    aiFileName = "";
    aiFilePath = "";
    cityFileName = "";
    cityFilePath = "";
    turnFileName = "";
    turnFilePath = "";
    componentImagesFile = "";
    vehicleArtworkFile = "";
    MiscArtworkFile = "";
    selectedTableAIMapKey = "";
    selectedTableCityMapKey = "";
    selectedTableTurnMapKey = "";

    musicFilePath = "";
    musicFolderPath = "";

    plPath = "";



    ui->checkBox_Artwork_ComponentImages->hide();
    ui->checkBox_Artwork_Vehicles->hide();
    ui->label_SelectedComponentImages->hide();
    ui->label_SelectedVehicleArtwork->hide();
    ui->button_SelectComponentImages->hide();
    ui->button_SelectVehicleArtwork->hide();

    ui->label_MusicFolderHead->hide();
    ui->label_selectedMusicFile->hide();
    ui->button_MusicFolderSelector->hide();


}

ModEditor::~ModEditor()
{
    delete ui;
}

//Get file name for noun/name list
void ModEditor::on_button_SelectNameList_clicked()
{
    nameListFile =  QFileDialog::getOpenFileName(this, "Open Name List Text File", "",
                                                         "Text File (*.txt)");
    //set label with filename.
    if (nameListFile != "")
    {
        ui->label_SelectedNames->setText(nameListFile);
    }
}

//Get file name for newspaper articles
void ModEditor::on_button_SelectNewspaper_clicked()
{
     newpaperArticlesFile =  QFileDialog::getOpenFileName(this, "Open Newpaper Articles File", "",
                                                         "XML File (*.xml)");
    //set label with filename.
    if (newpaperArticlesFile != "")
    {
        ui->label_SelectedNewsPaper->setText(newpaperArticlesFile);
    }
}

//Get file name for reviews
void ModEditor::on_button_SelectReviews_clicked()
{
     reviewsFile =  QFileDialog::getOpenFileName(this, "Open Reviews File", "",
                                                         "XML File (*.xml)");
    //set label with filename.
    if (reviewsFile != "")
    {
        ui->label_SelectedReviews->setText(reviewsFile);
    }
}

//Get file name for components
void ModEditor::on_button_SelectComponents_clicked()
{
     componentsFile =  QFileDialog::getOpenFileName(this, "Open Components File", "",
                                                         "XML File (*.xml)");
    //set label with filename.
    if (componentsFile != "")
    {
        ui->label_SelectedComponents->setText(componentsFile);
    }
}

//Get file name for component popularity
void ModEditor::on_button_SelectComponentPop_clicked()
{
    componentsPopFile =  QFileDialog::getOpenFileName(this, "Open Components Popularity File", "",
                                                         "XML File (*.xml)");

    //set label with filename.
    if (componentsPopFile != "")
    {
        ui->label_SelectedComponentPop->setText(componentsPopFile);
    }
}


//Add map to mod
void ModEditor::on_button_AddMap_clicked()
{
    if(!modMapList.contains(ui->combo_AddNewMap->currentText()))
    {
        modMapList.push_back(ui->combo_AddNewMap->currentText());
        ui->list_MapsToMod->clear();
        ui->list_MapsToMod->addItems(modMapList);

        ui->comboBox_CityFileMap->clear();
        ui->comboBox_CityFileMap->addItems(modMapList);

        ui->combo_SelectAIMap->clear();
        ui->combo_SelectAIMap->addItems(modMapList);

        ui->combo_TurnEventModMap->clear();
        ui->combo_TurnEventModMap->addItems(modMapList);
    }
}

//remove map from mod
void ModEditor::on_button_RemoveMap_clicked()
{
    if(modMapList.contains(ui->combo_AddNewMap->currentText()))
    {
      modMapList.removeAll(ui->combo_AddNewMap->currentText());
      ui->list_MapsToMod->clear();
      ui->list_MapsToMod->addItems(modMapList);

      ui->comboBox_CityFileMap->clear();
      ui->comboBox_CityFileMap->addItems(modMapList);

      ui->combo_SelectAIMap->clear();
      ui->combo_SelectAIMap->addItems(modMapList);

      ui->combo_TurnEventModMap->clear();
      ui->combo_TurnEventModMap->addItems(modMapList);
     }
}

//Selecting a map from the list will automatically fill the combo box.
void ModEditor::on_list_MapsToMod_currentRowChanged(int currentRow)
{
    if(ui->list_MapsToMod->item(currentRow) != 0)//make sure the row exists
       ui->combo_AddNewMap->setEditText(ui->list_MapsToMod->item(currentRow)->text());
}

void ModEditor::on_button_SelectAIFile_clicked()
{
    aiFilePath =  QFileDialog::getOpenFileName(this, "Open AI File", "",
                                                         "XML File (*.xml)");

    //set label with filename.
    if (aiFilePath != "")
    {
        QFileInfo fileInfo(aiFilePath);
        aiFileName = fileInfo.fileName();
        ui->label_AIModFileName->setText(aiFileName);
    }
}

void ModEditor::on_button_AddAiMap_clicked()
{
    if(aiFilePath == "")
        return;

    aiModMaps amm;
    amm.filePath = aiFilePath;
    amm.fileName = aiFileName;
    amm.description = ui->lineedit_AIModDescription->text();
    amm.map = ui->combo_SelectAIMap->currentText();
    amm.numberOfAI = ui->spinBox_numberOfAI->value();

    aiMapMap.insert(QString::number(amm.numberOfAI)+"@@"+amm.fileName+"@@"+ amm.map, amm);

    fillAITableInfo();

}

void ModEditor::on_button_RemoveAIMap_clicked()
{
    if(aiMapMap.contains(selectedTableAIMapKey))
    {
        aiMapMap.remove(selectedTableAIMapKey);
        fillAITableInfo();
    }
}

void ModEditor::on_table_AIModMaps_cellClicked(int row, int column)
{
    ui->label_AIModFileName->setText(ui->table_AIModMaps->item(row,1)->text());
    selectedTableAIMapKey = ui->table_AIModMaps->item(row,0)->text()+"@@"+
             ui->table_AIModMaps->item(row,1)->text()+"@@"+
             ui->table_AIModMaps->item(row,2)->text();
    if(aiMapMap.contains(selectedTableAIMapKey))
    {
      aiModMaps amm = aiMapMap.find(selectedTableAIMapKey).value();
        ui->lineedit_AIModDescription->setText(amm.description);
        ui->combo_SelectAIMap->setCurrentIndex(ui->combo_SelectAIMap->findText(amm.map));

        ui->spinBox_numberOfAI->setValue(amm.numberOfAI);
    }
}


void ModEditor::fillAITableInfo()
{
    ui->table_AIModMaps->clearContents();
    ui->table_AIModMaps->setRowCount(aiMapMap.size());
    int tableID = 0;
    for(QMap<QString,aiModMaps>::Iterator it= aiMapMap.begin();
        it != aiMapMap.end(); ++it)
    {
        QTableWidgetItem *aiNumbers = new QTableWidgetItem;
        QTableWidgetItem *fileName = new QTableWidgetItem;
        QTableWidgetItem *mapName = new QTableWidgetItem;

        aiNumbers->setData(Qt::EditRole,(*it).numberOfAI);
        fileName->setData(Qt::EditRole,(*it).fileName);
        mapName->setData(Qt::EditRole,(*it).map);

        ui->table_AIModMaps->setItem(tableID,0,aiNumbers);
        ui->table_AIModMaps->setItem(tableID,1,fileName);
        ui->table_AIModMaps->setItem(tableID,2,mapName);

        tableID++;

    }
}

void ModEditor::on_table_CitiesModMap_cellClicked(int row, int column)
{
    ui->label_SelectedCityFile->setText(ui->table_CitiesModMap->item(row,1)->text());
    selectedTableCityMapKey = ui->table_CitiesModMap->item(row,0)->text()+"@@"+
             ui->table_CitiesModMap->item(row,1)->text()+"@@"+
             ui->table_CitiesModMap->item(row,2)->text();

    if(cityMapMap.contains(selectedTableCityMapKey))
    {
      citiesModMaps cmm = cityMapMap.find(selectedTableCityMapKey).value();
        ui->comboBox_CityFileMap->setCurrentIndex(ui->comboBox_CityFileMap->findText(cmm.map));

        ui->spinBox_CityMapYear->setValue(cmm.cityYear);
    }
}

void ModEditor::on_button_SelectCityFile_clicked()
{
    cityFilePath =  QFileDialog::getOpenFileName(this, "Open City File", "",
                                                         "XML File (*.xml)");

    //set label with filename.
    if (cityFilePath != "")
    {
        QFileInfo fileInfo(cityFilePath);
        cityFileName = fileInfo.fileName();
        ui->label_SelectedCityFile->setText(cityFileName);
    }
}

void ModEditor::on_button_addCityFileMap_clicked()
{
    if(cityFileName == "")
        return;

    citiesModMaps cmm;
    cmm.filePath = cityFilePath;
    cmm.fileName = cityFileName;
    cmm.map = ui->comboBox_CityFileMap->currentText();
    cmm.cityYear = ui->spinBox_CityMapYear->value();

    cityMapMap.insert(QString::number(cmm.cityYear)+"@@"+cmm.fileName+"@@"+ cmm.map, cmm);

    fillCityTableInfo();
}

void ModEditor::on_button_RemoveCityFileMap_clicked()
{
    if(cityMapMap.contains(selectedTableCityMapKey))
    {
        cityMapMap.remove(selectedTableCityMapKey);
        fillCityTableInfo();
    }
}

void ModEditor::fillCityTableInfo()
{
    ui->table_CitiesModMap->clearContents();
    ui->table_CitiesModMap->setRowCount(cityMapMap.size());
    int tableID = 0;
    for(QMap<QString,citiesModMaps>::Iterator it= cityMapMap.begin();
        it != cityMapMap.end(); ++it)
    {
        QTableWidgetItem *yearItem = new QTableWidgetItem;
        QTableWidgetItem *fileName = new QTableWidgetItem;
        QTableWidgetItem *mapName = new QTableWidgetItem;

        yearItem->setData(Qt::EditRole,(*it).cityYear);
        fileName->setData(Qt::EditRole,(*it).fileName);
        mapName->setData(Qt::EditRole,(*it).map);

        ui->table_CitiesModMap->setItem(tableID,0,yearItem);
        ui->table_CitiesModMap->setItem(tableID,1,fileName);
        ui->table_CitiesModMap->setItem(tableID,2,mapName);

        tableID++;

    }
}

void ModEditor::on_table_TurnEventsMap_cellClicked(int row, int column)
{
    ui->label_TurnEventsModFile->setText(ui->table_TurnEventsMap->item(row,0)->text());
    selectedTableTurnMapKey = ui->table_TurnEventsMap->item(row,1)->text();

    if(turnMapMap.contains(selectedTableTurnMapKey))
    {
      turnEventsModMaps tmm = turnMapMap.find(selectedTableTurnMapKey).value();
        ui->combo_TurnEventModMap->setCurrentIndex(ui->combo_TurnEventModMap->findText(tmm.map));
    }

}

void ModEditor::on_button_SelectTurnEventsFile_clicked()
{
    turnFilePath =  QFileDialog::getOpenFileName(this, "Open Turn Events File", "",
                                                         "XML File (*.xml)");

    //set label with filename.
    if (turnFilePath != "")
    {
        QFileInfo fileInfo(turnFilePath);
        turnFileName = fileInfo.fileName();
        ui->label_TurnEventsModFile->setText(turnFileName);
    }

}

void ModEditor::on_button_AddTurnEventsMap_clicked()
{
    if(turnFileName == "")
        return;

    turnEventsModMaps tmm;
    tmm.filePath = turnFilePath;
    tmm.fileName = turnFileName;
    tmm.map = ui->combo_TurnEventModMap->currentText();

    turnMapMap.insert(tmm.map, tmm);

    fillTurnTableInfo();
}

void ModEditor::on_button_RemoveTurnEventsMap_clicked()
{
    if(turnMapMap.contains(selectedTableTurnMapKey))
    {
        turnMapMap.remove(selectedTableTurnMapKey);
        fillTurnTableInfo();
    }
}



void ModEditor::fillTurnTableInfo()
{
    ui->table_TurnEventsMap->clearContents();
    ui->table_TurnEventsMap->setRowCount(turnMapMap.size());
    int tableID = 0;
    for(QMap<QString,turnEventsModMaps>::Iterator it= turnMapMap.begin();
        it != turnMapMap.end(); ++it)
    {
        QTableWidgetItem *fileName = new QTableWidgetItem;
        QTableWidgetItem *mapName = new QTableWidgetItem;

        fileName->setData(Qt::EditRole,(*it).fileName);
        mapName->setData(Qt::EditRole,(*it).map);

        ui->table_TurnEventsMap->setItem(tableID,0,fileName);
        ui->table_TurnEventsMap->setItem(tableID,1,mapName);

        tableID++;

    }
}

void ModEditor::on_button_NewMod_clicked()
{
    ui->checkbox_ComponentPopularity->setChecked(false);
    ui->checkbox_Components->setChecked(false);
    ui->checkbox_NameList->setChecked(false);
    ui->checkbox_Newspapers->setChecked(false);
    ui->checkbox_Reviews->setChecked(false);

    ui->comboBox_CityFileMap->clear();
    ui->combo_SelectAIMap->clear();
    ui->combo_TurnEventModMap->clear();

    ui->label_AIModFileName->setText("No File Selected");
    ui->label_SelectedCityFile->setText("No File Selected");
    ui->label_SelectedComponentPop->setText("No File Selected");
    ui->label_SelectedComponents->setText("No File Selected");
    ui->label_SelectedNames->setText("No File Selected");
    ui->label_SelectedNewsPaper->setText("No File Selected");
    ui->label_SelectedReviews->setText("No File Selected");
    ui->label_TurnEventsModFile->setText("No File Selected");
    ui->label_selectedRacingSeries->setText("No File Selected");
    ui->label_selectedContractInfo->setText("No File Selected");
    ui->Label_SelectedMiscArtwork->setText("No File Selected");
    ui->label_SelectedComponentImages->setText("No File Selected");
    ui->label_SelectedVehicleArtwork->setText("No File Selected");

    ui->label_selectedMusicFile->setText("No File Selected");
    ui->label_selectedMusicFolder->setText("No File Selected");
    ui->label_selectedPlayerLogos->setText("No File Selected");



    ui->lineedit_AIModDescription->setText("");
    ui->lineEdit_Author->setText("");
    ui->lineEdit_ModName->setText("");

    ui->list_MapsToMod->clear();

    ui->spinBox_CityMapYear->setValue(1900);
    ui->spinBox_numberOfAI->setValue(-1);

    ui->table_AIModMaps->clearContents();
    ui->table_AIModMaps->setRowCount(0);
    ui->table_CitiesModMap->clearContents();
    ui->table_CitiesModMap->setRowCount(0);
    ui->table_TurnEventsMap->clearContents();
    ui->table_TurnEventsMap->setRowCount(0);

    ui->textBox_Comments->clear();

    ui->checkBox_Artwork_ComponentImages->hide();
    ui->checkBox_Artwork_Vehicles->hide();
    ui->label_SelectedComponentImages->hide();
    ui->label_SelectedVehicleArtwork->hide();
    ui->button_SelectComponentImages->hide();
    ui->button_SelectVehicleArtwork->hide();


    nameListFile = "";
    newpaperArticlesFile = "";
    reviewsFile = "";
    componentsFile = "";
    componentsPopFile = "";
    racingFile = "";
    contractFile = "";
    aiFileName = "";
    aiFilePath = "";
    cityFileName = "";
    cityFilePath = "";
    turnFileName = "";
    turnFilePath = "";
    componentImagesFile = "";
    vehicleArtworkFile = "";
    MiscArtworkFile = "";
    selectedTableAIMapKey = "";
    selectedTableCityMapKey = "";
    selectedTableTurnMapKey = "";
    musicFilePath = "";
    musicFolderPath = "";

    plPath = "";
    modMapList.clear();

    aiMapMap.clear();
    cityMapMap.clear();
    turnMapMap.clear();


}

void ModEditor::on_button_SaveMod_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".xml"))
            saveFileName += ".xml";

        //Call function to actually save.
        saveForEditor(saveFileName);
    }

}


void ModEditor::saveForEditor(QString saveFileName)
{
    QFile saveFile(saveFileName);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(saveFileName,
                             saveFile.errorString()));
       return;
    }

    checkSaveErrors();


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("ModFile");
    xmlWriter.writeTextElement("ModName", ui->lineEdit_ModName->text());
    xmlWriter.writeTextElement("GCModVersion","12200"); //Used to compare to GC's Mod Version Var.
    xmlWriter.writeTextElement("Author",ui->lineEdit_Author->text());
    xmlWriter.writeTextElement("Comments",ui->textBox_Comments->toPlainText());

    xmlWriter.writeStartElement("NameList");
        xmlWriter.writeAttribute("Override",QString::number(ui->checkbox_NameList->isChecked()));
        if(ui->checkbox_NameList->isChecked())
        {
            QFileInfo fi(nameListFile);
             xmlWriter.writeAttribute("FileName", fi.fileName());
            xmlWriter.writeCharacters(nameListFile);

        }
    xmlWriter.writeEndElement(); //NameList

    xmlWriter.writeStartElement("Newspaper");
        xmlWriter.writeAttribute("Override",QString::number(ui->checkbox_Newspapers->isChecked()));
        if(ui->checkbox_Newspapers->isChecked())
        {
            QFileInfo fi(newpaperArticlesFile);
             xmlWriter.writeAttribute("FileName", fi.fileName());
            xmlWriter.writeCharacters(newpaperArticlesFile);
        }
    xmlWriter.writeEndElement(); //Newspaper

    xmlWriter.writeStartElement("Reviews");
        xmlWriter.writeAttribute("Override",QString::number(ui->checkbox_Reviews->isChecked()));
        if(ui->checkbox_Reviews->isChecked())
        {
            QFileInfo fi(reviewsFile);
             xmlWriter.writeAttribute("FileName", fi.fileName());
            xmlWriter.writeCharacters(reviewsFile);
        }
    xmlWriter.writeEndElement(); //Reviews

    xmlWriter.writeStartElement("Components");
        xmlWriter.writeAttribute("Override",QString::number(ui->checkbox_Components->isChecked()));
            if(ui->checkbox_Components->isChecked())
            {
                QFileInfo fi(componentsFile);
                 xmlWriter.writeAttribute("FileName", fi.fileName());
                xmlWriter.writeCharacters(componentsFile);
            }
    xmlWriter.writeEndElement(); //Components


        xmlWriter.writeStartElement("ArtworkComponentsImages");
           xmlWriter.writeAttribute("Override",QString::number(
                                      ui->checkBox_Artwork_ComponentImages->isChecked()));
                if(ui->checkBox_Artwork_ComponentImages->isChecked())
                {
                    QFileInfo fi(componentImagesFile);
                     xmlWriter.writeAttribute("FileName", fi.fileName());
                    xmlWriter.writeCharacters(componentImagesFile);
                }
        xmlWriter.writeEndElement(); //ArtworkComponentsImages



        xmlWriter.writeStartElement("ArtworkVehicles");
           xmlWriter.writeAttribute("Override",QString::number(
                                      ui->checkBox_Artwork_Vehicles->isChecked()));
                if(ui->checkBox_Artwork_Vehicles->isChecked())
                {
                    QFileInfo fi(vehicleArtworkFile);
                     xmlWriter.writeAttribute("FileName", fi.fileName());
                    xmlWriter.writeCharacters(vehicleArtworkFile);
                }
        xmlWriter.writeEndElement(); //ArtworkVehicles


        xmlWriter.writeStartElement("ArtworkMisc");
           xmlWriter.writeAttribute("Override",QString::number(
                                      ui->checkBox_Artwork_Misc->isChecked()));
                if(ui->checkBox_Artwork_Misc->isChecked())
                {
                    QFileInfo fi(MiscArtworkFile);
                     xmlWriter.writeAttribute("FileName", fi.fileName());
                    xmlWriter.writeCharacters(MiscArtworkFile);
                }
        xmlWriter.writeEndElement(); //ArtworkMisc


        xmlWriter.writeStartElement("MusicFile");
           xmlWriter.writeAttribute("Override",QString::number(
                                      ui->checkBox_Music->isChecked()));
                if(ui->checkBox_Music->isChecked())
                {
                    QFileInfo fi(musicFilePath);
                     xmlWriter.writeAttribute("FileName", fi.fileName());
                    xmlWriter.writeCharacters(musicFilePath);
                }
        xmlWriter.writeEndElement(); //MusicFile

        xmlWriter.writeStartElement("PlayerLogos");
           xmlWriter.writeAttribute("Override",QString::number(
                                      ui->checkBox_playerLogo->isChecked()));
                if(ui->checkBox_playerLogo->isChecked())
                {
                    QFileInfo fi(plPath);
                     xmlWriter.writeAttribute("FileName", fi.fileName());
                    xmlWriter.writeCharacters(plPath);
                }
        xmlWriter.writeEndElement(); //PlayerLogos


        xmlWriter.writeTextElement("MusicFolder",musicFolderPath);




    xmlWriter.writeStartElement("ComponentsPop");
    xmlWriter.writeAttribute("Override",QString::number(
                                 ui->checkbox_ComponentPopularity->isChecked()));
        if(ui->checkbox_ComponentPopularity->isChecked())
        {
            QFileInfo fi(componentsPopFile);
             xmlWriter.writeAttribute("FileName", fi.fileName());
            xmlWriter.writeCharacters(componentsPopFile);
        }
    xmlWriter.writeEndElement(); //ComponentsPop


    xmlWriter.writeStartElement("RacingSeries");
    xmlWriter.writeAttribute("Override",QString::number(
                                 ui->checkBox_RacingSeries->isChecked()));
        if(ui->checkBox_RacingSeries->isChecked())
        {
            QFileInfo fi(racingFile);
             xmlWriter.writeAttribute("FileName", fi.fileName());
            xmlWriter.writeCharacters(racingFile);
        }
    xmlWriter.writeEndElement(); //RacingSeries

    xmlWriter.writeStartElement("ContractInfo");
    xmlWriter.writeAttribute("Override",QString::number(
                                 ui->checkBox_ContractInfo->isChecked()));
        if(ui->checkBox_ContractInfo->isChecked())
        {
            QFileInfo fi(contractFile);
             xmlWriter.writeAttribute("FileName", fi.fileName());
            xmlWriter.writeCharacters(contractFile);
        }
    xmlWriter.writeEndElement(); //ContractInfo


    xmlWriter.writeStartElement("SupportedMaps");
        for(QStringList::Iterator it = modMapList.begin(); it != modMapList.end(); ++it)
        {
            xmlWriter.writeTextElement("Map",(*it));
        }
    xmlWriter.writeEndElement(); //SupportedMaps

    if(!aiMapMap.empty())
    {
        xmlWriter.writeStartElement("AIModFiles");
        for(QMap<QString,aiModMaps>::Iterator it = aiMapMap.begin(); it != aiMapMap.end(); ++it)
        {
            xmlWriter.writeStartElement("AI");
             xmlWriter.writeAttribute("FileName",(*it).fileName);
             xmlWriter.writeAttribute("FilePath",(*it).filePath);
             xmlWriter.writeAttribute("Map",(*it).map);
             xmlWriter.writeAttribute("AINumber",QString::number((*it).numberOfAI));
             xmlWriter.writeCharacters((*it).description);
            xmlWriter.writeEndElement(); //AI
        }
        xmlWriter.writeEndElement(); //AIModFiles
    }

    if(!cityMapMap.empty())
    {
        xmlWriter.writeStartElement("CityModFiles");
        for(QMap<QString,citiesModMaps>::Iterator it = cityMapMap.begin();
            it != cityMapMap.end(); ++it)
        {
            xmlWriter.writeStartElement("CityFile");
             xmlWriter.writeAttribute("FileName",(*it).fileName);
             xmlWriter.writeAttribute("FilePath",(*it).filePath);
             xmlWriter.writeAttribute("Map",(*it).map);
             xmlWriter.writeAttribute("Year",QString::number((*it).cityYear));
            xmlWriter.writeEndElement(); //CityFile
        }
        xmlWriter.writeEndElement(); //CityModFiles
    }

    if(!turnMapMap.empty())
    {
        xmlWriter.writeStartElement("TurnEventsModFiles");
        for(QMap<QString,turnEventsModMaps>::Iterator it = turnMapMap.begin();
            it != turnMapMap.end(); ++it)
        {
            xmlWriter.writeStartElement("TurnFile");
             xmlWriter.writeAttribute("FileName",(*it).fileName);
             xmlWriter.writeAttribute("FilePath",(*it).filePath);
             xmlWriter.writeAttribute("Map",(*it).map);
            xmlWriter.writeEndElement(); //TurnFile
        }
        xmlWriter.writeEndElement(); //TurnEventsModFiles
    }

    xmlWriter.writeEndDocument();

    saveFile.close();


}


void ModEditor::on_button_OpenMod_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Mod File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        openFile(openFileName);
    }
}



void ModEditor::openFile(QString openFileName)
{
    on_button_NewMod_clicked();

    QFile file(openFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this,"File Error",
                              QString("Whopse, opening %1 failed.").arg(openFileName));
             return;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;

    QDomElement baseElement;
    QDomElement subElement;


    //Parse TurnEvents.xml file.
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        QMessageBox::critical(this,"Error Processing Mod File",
                             QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                             errorMsg,errorLine,errorColumn));
       return;
    }

    QDomElement rootNode = doc.firstChildElement("ModFile");
    if(rootNode.isNull())
    {
         QMessageBox::critical(this,"Error","Map Loader Data is malformed.");
        return;
    }



    rootNode = doc.documentElement();
    baseElement = rootNode.firstChildElement("ModName");
    ui->lineEdit_ModName->setText(baseElement.text());


    baseElement = rootNode.firstChildElement("Author");
    ui->lineEdit_Author->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("Comments");
    ui->textBox_Comments->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("NameList");
    ui->label_SelectedNames->setText(baseElement.text());
    ui->checkbox_NameList->setChecked(baseElement.attributeNode("Override").value().toInt());
    nameListFile = baseElement.text();

    baseElement = rootNode.firstChildElement("Newspaper");
    ui->label_SelectedNewsPaper->setText(baseElement.text());
    ui->checkbox_Newspapers->setChecked(baseElement.attributeNode("Override").value().toInt());
    newpaperArticlesFile = baseElement.text();

    baseElement = rootNode.firstChildElement("Reviews");
    ui->label_SelectedReviews->setText(baseElement.text());
    ui->checkbox_Reviews->setChecked(baseElement.attributeNode("Override").value().toInt());
    reviewsFile = baseElement.text();

    baseElement = rootNode.firstChildElement("Components");
    ui->checkbox_Components->setChecked(baseElement.attributeNode("Override").value().toInt());
    ui->label_SelectedComponents->setText(baseElement.text());
    componentsFile = baseElement.text();

    baseElement = rootNode.firstChildElement("ArtworkComponentsImages");
    ui->checkBox_Artwork_ComponentImages->setChecked(
                baseElement.attributeNode("Override").value().toInt());
    ui->label_SelectedComponentImages->setText(baseElement.text());
    componentImagesFile = baseElement.text();

    baseElement = rootNode.firstChildElement("ArtworkVehicles");
    ui->checkBox_Artwork_Vehicles->setChecked(
                baseElement.attributeNode("Override").value().toInt());
    ui->label_SelectedVehicleArtwork->setText(baseElement.text());
    vehicleArtworkFile = baseElement.text();

    baseElement = rootNode.firstChildElement("ArtworkMisc");
    ui->checkBox_Artwork_Misc->setChecked(
                baseElement.attributeNode("Override").value().toInt());
    ui->Label_SelectedMiscArtwork->setText(baseElement.text());
    MiscArtworkFile = baseElement.text();

    baseElement = rootNode.firstChildElement("MusicFile");
    ui->checkBox_Music->setChecked(
                baseElement.attributeNode("Override").value().toInt());
    ui->label_selectedMusicFile->setText(baseElement.text());
    musicFilePath = baseElement.text();

    baseElement = rootNode.firstChildElement("PlayerLogos");
    ui->checkBox_playerLogo->setChecked(
                baseElement.attributeNode("Override").value().toInt());
    ui->label_selectedPlayerLogos->setText(baseElement.text());
    plPath = baseElement.text();

    baseElement = rootNode.firstChildElement("MusicFolder");
    musicFolderPath = baseElement.text();
    if(musicFolderPath == "")
        ui->label_selectedMusicFolder->setText("No File Selected");
    else
        ui->label_selectedMusicFolder->setText(baseElement.text());


    baseElement = rootNode.firstChildElement("ComponentsPop");
    ui->label_SelectedComponentPop->setText(baseElement.text());
    ui->checkbox_ComponentPopularity->setChecked(baseElement.attributeNode("Override").value().toInt());
    componentsPopFile = baseElement.text();

    baseElement = rootNode.firstChildElement("RacingSeries");
    ui->checkBox_RacingSeries->setChecked(baseElement.attributeNode("Override").value().toInt());
    ui->label_selectedRacingSeries->setText(baseElement.text());
    racingFile = baseElement.text();

    baseElement = rootNode.firstChildElement("ContractInfo");
    ui->checkBox_ContractInfo->setChecked(baseElement.attributeNode("Override").value().toInt());
    ui->label_selectedContractInfo->setText(baseElement.text());
    contractFile = baseElement.text();

    baseElement = rootNode.firstChildElement("SupportedMaps");
    subElement = baseElement.firstChildElement("Map");
    while(!subElement.isNull())
    {
        modMapList.push_back(subElement.text());
        subElement = subElement.nextSiblingElement("Map");
    }

    baseElement = rootNode.firstChildElement("AIModFiles");
    subElement = baseElement.firstChildElement("AI");
    while(!subElement.isNull())
    {
        aiModMaps amm;
        amm.fileName = subElement.attributeNode("FileName").value();
        amm.filePath = subElement.attributeNode("FilePath").value();
        amm.map = subElement.attributeNode("Map").value();
        amm.numberOfAI = subElement.attributeNode("AINumber").value().toInt();
        amm.description = subElement.text();

        aiMapMap.insert(QString::number(amm.numberOfAI)+"@@"+amm.fileName+"@@"+ amm.map,amm);

        subElement = subElement.nextSiblingElement("AI");
    }

    baseElement = rootNode.firstChildElement("CityModFiles");
    subElement = baseElement.firstChildElement("CityFile");
    while(!subElement.isNull())
    {
        citiesModMaps cmm;
        cmm.fileName = subElement.attributeNode("FileName").value();
        cmm.filePath = subElement.attributeNode("FilePath").value();
        cmm.map = subElement.attributeNode("Map").value();
        cmm.cityYear = subElement.attributeNode("Year").value().toInt();

        cityMapMap.insert(QString::number(cmm.cityYear)+"@@"+cmm.fileName+"@@"+ cmm.map, cmm);

        subElement = subElement.nextSiblingElement("CityFile");
    }

    baseElement = rootNode.firstChildElement("TurnEventsModFiles");
    subElement = baseElement.firstChildElement("TurnFile");
    while(!subElement.isNull())
    {
        turnEventsModMaps tmm;
        tmm.fileName = subElement.attributeNode("FileName").value();
        tmm.filePath = subElement.attributeNode("FilePath").value();
        tmm.map = subElement.attributeNode("Map").value();

        turnMapMap.insert(tmm.map, tmm);

        subElement = subElement.nextSiblingElement("TurnFile");
    }


    fillAITableInfo();
    fillCityTableInfo();
    fillTurnTableInfo();

    file.close();
}

bool ModEditor::checkSaveErrors()
{
    bool haveError = false;
    QString errorMessage = "";

    if(ui->lineEdit_ModName->text() == "Default")
    {
        haveError = true;
        errorMessage = "You can not name your mod \"Default\"\n This name is reserved for GearCity"
                "'s default files.";
    }
    else if(modMapList.empty() && (!aiMapMap.empty() || !cityMapMap.empty() || !turnMapMap.empty()))
    {
        haveError = true;
        errorMessage = "This mod has no maps selected. But has map overridders for AI, cities, or "
                "turn events. These will not be useable in game.";
    }
    else if(ui->checkbox_ComponentPopularity->isChecked() &&
            componentsPopFile == "")
    {
        haveError = true;
        errorMessage = "Component Popularity Override has been enabled"
                "but no file has been selected.";
    }
    else if(ui->checkbox_Components->isChecked() && componentsFile == "")
    {
        haveError = true;
        errorMessage = "Component Override has been enabled but no file has been selected";
    }
    else if(ui->checkbox_NameList->isChecked() && nameListFile == "")
    {
        haveError = true;
        errorMessage = "Noun list Override has been enabled but no file has been selected";
    }
    else if(ui->checkbox_Newspapers->isChecked() && newpaperArticlesFile == "")
    {
        haveError = true;
        errorMessage = "Newspaper Override has been enabled but no file has been selected";
    }
    else if(ui->checkbox_Reviews->isChecked() && reviewsFile == "")
    {
        haveError = true;
        errorMessage = "Review Override has been enabled but no file has been selected";
    }
    else if(ui->checkBox_ContractInfo->isChecked() && contractFile == "")
    {
        haveError = true;
        errorMessage = "Contract Info Override has been enabled but no file has been selected";
    }
    else if(ui->checkBox_RacingSeries->isChecked() && racingFile == "")
    {
        haveError = true;
        errorMessage = "Racing Series Override has been enabled but no file has been selected";
    }
    else if(ui->checkBox_Artwork_Misc->isChecked() && MiscArtworkFile == "")
    {
        haveError = true;
        errorMessage = "Miscellaneous Artwork has been enabled but no file has been selected";
    }
    else if(ui->checkBox_Artwork_ComponentImages->isChecked() && componentImagesFile == "")
    {
        haveError = true;
        errorMessage = "Component Images has been enabled but no file has been selected";
    }
    else if(ui->checkBox_Artwork_Vehicles->isChecked() && vehicleArtworkFile == "")
    {
        haveError = true;
        errorMessage = "Vehicle Artwork has been enabled but no file has been selected";
    }

    if(haveError)
    {
        QMessageBox::critical(this,"Error!",errorMessage);
        return false;
    }

    return true;
}

void ModEditor::on_button_ExportMod_clicked()
{
    if(!checkSaveErrors())
        return;

    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder To Export To", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(folderPath != "")
    {
        exportMod(folderPath);
    }
}

void ModEditor::exportMod(QString parentFolder)
{
    if(!checkSaveErrors())
        return;

    #if defined(Q_WS_WIN)
        QString modFolder = parentFolder +"\\"+ ui->lineEdit_ModName->text()+"\\";
    #else
        QString modFolder = parentFolder +"/"+ ui->lineEdit_ModName->text()+"/";
    #endif

    QString modPrefix = "mod_"+ui->lineEdit_ModName->text()+"_";//.replace(" ","_")+"_";


    if(!QDir(modFolder).exists())
    {
        QDir().mkdir(modFolder);
    }


    QString src_name = nameListFile;
    QString src_news = newpaperArticlesFile;
    QString src_review = reviewsFile;
    QString src_comp = componentsFile;
    QString src_compPop = componentsPopFile;
    QString src_racing = racingFile;
    QString src_contract = contractFile;
    QString src_componentImage = componentImagesFile;
    QString src_vehicleArtwork = vehicleArtworkFile;
    QString src_MiscArtwork = MiscArtworkFile;
    QString src_Music = musicFilePath;
    QString src_PlayerLogo = plPath;

    QString src_MusicFolder = musicFolderPath;


    QMap<QString,aiModMaps> src_aiMapMap = aiMapMap;
    QMap<QString,citiesModMaps> src_cityMapMap = cityMapMap;
    QMap<QString,turnEventsModMaps> src_turnMapMap = turnMapMap;

    if(ui->checkbox_NameList->isChecked())
    {
        QFile::copy(nameListFile, modFolder+modPrefix+"NounList.txt");
        nameListFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+modPrefix+"NounList.txt";
    }

    if(ui->checkbox_Newspapers->isChecked())
    {
        QFile::copy(newpaperArticlesFile, modFolder+modPrefix+"NewsArticles.xml");
        newpaperArticlesFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"NewsArticles.xml";
    }

    if(ui->checkbox_Reviews->isChecked())
    {
        QFile::copy(reviewsFile, modFolder+modPrefix+"Reviews.xml");
        reviewsFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+modPrefix+"Reviews.xml";
    }

    if(ui->checkbox_Components->isChecked())
    {
        QFile::copy(componentsFile, modFolder+modPrefix+"Components.xml");
        componentsFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"Components.xml";
    }

    if(ui->checkbox_ComponentPopularity->isChecked())
    {
        QFile::copy(componentsPopFile, modFolder+modPrefix+"ComponentsStartingPop.xml");
        componentsPopFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"ComponentsStartingPop.xml";
    }

    if(ui->checkBox_RacingSeries->isChecked())
    {
        QFile::copy(racingFile, modFolder+modPrefix+"RacingSeries.xml");
        racingFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"RacingSeries.xml";
    }

    if(ui->checkBox_ContractInfo->isChecked())
    {
        QFile::copy(contractFile, modFolder+modPrefix+"ContractInfo.xml");
        contractFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"ContractInfo.xml";
    }

    if(ui->checkBox_Artwork_Misc->isChecked())
    {
        QFile::copy(MiscArtworkFile, modFolder+modPrefix+"MiscArtwork.zip");
        contractFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"MiscArtwork.zip";
    }

    if(ui->checkBox_Artwork_ComponentImages->isChecked())
    {
        QFile::copy(contractFile, modFolder+modPrefix+"ComponentImages.zip");
        componentImagesFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"ComponentImages.zip";
    }

    if(ui->checkBox_Artwork_Vehicles->isChecked())
    {
        QFile::copy(vehicleArtworkFile, modFolder+modPrefix+"VehicleArtwork.zip");
        vehicleArtworkFile = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"VehicleArtwork.zip";
    }

    if(ui->checkBox_Music->isChecked())
    {
        QFile::copy(musicFilePath, modFolder+modPrefix+"Music.xml");
        musicFilePath = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"Music.xml";

        if(musicFolderPath != "")
        {
            QFile::copy(musicFolderPath, modFolder+modPrefix+"Music/");
            musicFilePath = modFolder+modPrefix+"Music/";
        }
    }

    if(ui->checkBox_playerLogo->isChecked())
    {
        QFile::copy(plPath, modFolder+modPrefix+"PlayerLogos.xml");
        plPath = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+"PlayerLogos.xml";


    }

    for(QMap<QString,aiModMaps>::Iterator it = aiMapMap.begin(); it != aiMapMap.end(); ++it)
    {
        QFile::copy((*it).filePath, modFolder+modPrefix+(*it).fileName);
        (*it).filePath = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+(*it).map+"_"+(*it).fileName;
        (*it).fileName = modPrefix+"_"+(*it).map+"_"+(*it).fileName;
    }

    for(QMap<QString,citiesModMaps>::Iterator it = cityMapMap.begin();
        it != cityMapMap.end(); ++it)
    {
        QFile::copy((*it).filePath, modFolder+modPrefix+(*it).fileName);
        (*it).filePath = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+(*it).map+"_"+ (*it).fileName;
        (*it).fileName = modPrefix+"_"+(*it).map+"_"+(*it).fileName;
    }

    for(QMap<QString,turnEventsModMaps>::Iterator it = turnMapMap.begin();
        it != turnMapMap.end(); ++it)
    {
        QFile::copy((*it).filePath, modFolder+modPrefix+(*it).fileName);
        (*it).filePath = "../media/Mods/"+ ui->lineEdit_ModName->text()+"/"+
                modPrefix+(*it).map+"_"+ (*it).fileName;
        (*it).fileName = modPrefix+"_"+(*it).map+"_"+(*it).fileName;
    }





    saveForEditor(modFolder+"ModFile.xml");

    nameListFile = src_name;
    newpaperArticlesFile = src_news;
    reviewsFile = src_review;
    componentsFile = src_comp;
    componentsPopFile = src_compPop;
    racingFile = src_racing;
    contractFile = src_contract;
    componentImagesFile = src_componentImage;
    vehicleArtworkFile = src_vehicleArtwork;
    MiscArtworkFile = src_MiscArtwork;
    aiMapMap = src_aiMapMap;
    cityMapMap = src_cityMapMap;
    turnMapMap = src_turnMapMap;    
    musicFilePath = src_Music;
    plPath = src_PlayerLogo;
    musicFolderPath = src_MusicFolder;


    for(QMap<QString,aiModMaps>::Iterator it = aiMapMap.begin(); it != aiMapMap.end(); ++it)
    {
        (*it).filePath = (*it).filePath.replace(modPrefix+(*it).map+"_","");
    }

    for(QMap<QString,citiesModMaps>::Iterator it = cityMapMap.begin();
        it != cityMapMap.end(); ++it)
    {
        (*it).filePath = (*it).filePath.replace(modPrefix+(*it).map+"_","");
    }

    for(QMap<QString,turnEventsModMaps>::Iterator it = turnMapMap.begin();
        it != turnMapMap.end(); ++it)
    {
        (*it).filePath = (*it).filePath.replace(modPrefix+(*it).map+"_","");
    }



    //We Did it! Yay!
    QMessageBox::information(this,"Export Complete",
                             "The Mod has been successfully exported!", QMessageBox::Ok);

}

void ModEditor::on_button_SelectRacingSeries_clicked()
{
    racingFile =  QFileDialog::getOpenFileName(this, "Open Racing Series File", "",
                                                        "XML File (*.xml)");
   //set label with filename.
   if (racingFile != "")
   {
       ui->label_selectedRacingSeries->setText(racingFile);
   }
}

void ModEditor::on_button_SelectContractInfo_clicked()
{
    contractFile =  QFileDialog::getOpenFileName(this, "Open Contract Info File", "",
                                                        "XML File (*.xml)");
   //set label with filename.
   if (contractFile != "")
   {
       ui->label_selectedContractInfo->setText(contractFile);
   }
}

void ModEditor::on_button_ReturnToMain_clicked()
{
   cp_wsc.MainMenuCW->raise();
}

void ModEditor::on_Button_NounListEditor_clicked()
{
    cp_wsc.NameListCW->raise();
}

void ModEditor::on_Button_NewsPaperEditor_clicked()
{
    cp_wsc.NewsPaperCW->raise();
}

void ModEditor::on_button_ReviewsEditor_clicked()
{
    cp_wsc.ReviewsCW->raise();
}

void ModEditor::on_button_ComponentsEditor_clicked()
{
    cp_wsc.ComponentsCW->raise();
}

void ModEditor::on_button_AIEditor_clicked()
{
    cp_wsc.AIEditorCW->raise();
}

void ModEditor::on_button_CitiesEditor_clicked()
{
    cp_wsc.CityEditorCW->raise();
}

void ModEditor::on_button_turnEventsEditor_clicked()
{
    cp_wsc.TurnEventEditorCW->raise();
}


void ModEditor::on_checkbox_Components_stateChanged(int arg1)
{
    if(arg1 == 0)
    {

        ui->checkBox_Artwork_ComponentImages->hide();
        ui->checkBox_Artwork_Vehicles->hide();
        ui->label_SelectedComponentImages->hide();
        ui->label_SelectedVehicleArtwork->hide();
        ui->button_SelectComponentImages->hide();
        ui->button_SelectVehicleArtwork->hide();

    }
    else
    {
        ui->checkBox_Artwork_ComponentImages->show();
        ui->checkBox_Artwork_Vehicles->show();
        ui->label_SelectedComponentImages->show();
        ui->label_SelectedVehicleArtwork->show();
        ui->button_SelectComponentImages->show();
        ui->button_SelectVehicleArtwork->show();

    }
}

void ModEditor::on_button_SelectComponentImages_clicked()
{
    componentImagesFile =  QFileDialog::getOpenFileName(this, "Open Component Images Zip", "",
                                                        "Zip File (*.zip)");
   //set label with filename.
   if (componentImagesFile != "")
   {
       ui->label_SelectedComponentImages->setText(componentImagesFile);
   }
}

void ModEditor::on_button_SelectVehicleArtwork_clicked()
{
    vehicleArtworkFile =  QFileDialog::getOpenFileName(this, "Open Vehicle Artwork Zip", "",
                                                        "Zip File (*.zip)");
   //set label with filename.
   if (vehicleArtworkFile != "")
   {
       ui->label_SelectedVehicleArtwork->setText(vehicleArtworkFile);
   }
}

void ModEditor::on_button_SelectMiscArtwork_clicked()
{
    MiscArtworkFile =  QFileDialog::getOpenFileName(this, "Open Miscellaneous Artwork Zip", "",
                                                        "Zip File (*.zip)");
   //set label with filename.
   if (MiscArtworkFile != "")
   {
       ui->Label_SelectedMiscArtwork->setText(MiscArtworkFile);
   }
}


void ModEditor::on_checkBox_Music_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->label_MusicFolderHead->show();
        ui->label_selectedMusicFile->show();
        ui->button_MusicFolderSelector->show();

    }
    else
    {
        ui->label_MusicFolderHead->hide();
        ui->label_selectedMusicFile->hide();
        ui->button_MusicFolderSelector->hide();
    }
}


void ModEditor::on_button_MusicFileSelector_clicked()
{
    musicFilePath =  QFileDialog::getOpenFileName(this, "Select Music File", "",
                                                        "xml File (*.xml)");
   //set label with filename.
   if (musicFilePath != "")
   {
       ui->label_selectedMusicFile->setText(musicFilePath);
   }
}

void ModEditor::on_button_MusicFolderSelector_clicked()
{
    musicFolderPath = QFileDialog::getExistingDirectory(this, "Select Folder With Music", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //set label with folder path.
    if (musicFolderPath != "")
    {
        ui->label_selectedMusicFolder->setText(musicFolderPath);
    }
}



void ModEditor::on_button_PlayerLogoFileSelector_clicked()
{
    plPath =  QFileDialog::getOpenFileName(this, "Select Player Logo File", "",
                                                        "xml File (*.xml)");
   //set label with filename.
   if (plPath != "")
   {
       ui->label_selectedPlayerLogos->setText(plPath);
   }
}

