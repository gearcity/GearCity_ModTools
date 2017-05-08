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


#include "MapDataTool.h"
#include "ui_MapDataTool.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>


//Create map data tool
MapDataTool::MapDataTool(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapDataTool)
{
    ui->setupUi(this);

    //copy widget storage container
    cp_wsc = wsc;

    //align widgets on the same grid!
    ui->gridLayout->addWidget(ui->GroupBox_MapTools_AIStartingOptions,1,1);
    ui->gridLayout->addWidget(ui->GroupBox_MapTools_CityScriptLoadingYear,1,1);
    ui->gridLayout->addWidget(ui->Group_MapTools_MapArtwork_Change,1,1);

    ui->Table_MapTools_CityScriptLoadingYears->horizontalHeader()->setStretchLastSection(true);
    ui->Table_MapTools_MapArtLoadingYears->horizontalHeader()->setStretchLastSection(true);
    ui->Table_MapTools_StartingAINumbersAndFiles->horizontalHeader()->setStretchLastSection(true);
}

MapDataTool::~MapDataTool()
{
    delete ui;
}

//Open Main Menu
void MapDataTool::on_Button_MapTools_ReturnToMain_clicked()
{
    cp_wsc.MainMenuCW->raise();
}

//Open City Editor
void MapDataTool::on_Button_MapTool_CityEditor_clicked()
{
    cp_wsc.CityEditorCW->raise();
}

//Open AI Editor
void MapDataTool::on_Button_MapTool_AIEditor_clicked()
{
    cp_wsc.AIEditorCW->raise();
}

//Open Turn Events Editor
void MapDataTool::on_Button_MapTool_TurnEventsEditor_clicked()
{
    cp_wsc.TurnEventEditorCW->raise();
}

//Toggle Between the Dot City Format and the Dot Scene
void MapDataTool::on_Radio_MapTools_DotCity_toggled(bool checked)
{
    if(checked) //Dot City
    {
        ui->CheckBox_MapArtTools_ChangeMapEveryTurn->setVisible(true);
        ui->CheckBox_Regional_CityMap->setVisible(true);
        ui->Label_Regional_CityCoords->setVisible(true);
        ui->label_regional_topleft->setVisible(true);
        ui->label_regional_topright->setVisible(true);
        ui->label_regional_BottomLeft->setVisible(true);
        ui->label_regional_BottomRight->setVisible(true);
        ui->spinBox_RegionalCoords_TL_Lat->setVisible(true);
        ui->spinBox_RegionalCoords_TL_Long->setVisible(true);
        ui->spinBox_RegionalCoords_TR_Lat->setVisible(true);
        ui->spinBox_RegionalCoords_TR_Long->setVisible(true);
        ui->spinBox_RegionalCoords_BL_Long->setVisible(true);
        ui->spinBox_RegionalCoords_BL_Lat->setVisible(true);
        ui->spinBox_RegionalCoords_BR_Lat->setVisible(true);
        ui->spinBox_RegionalCoords_BR_Long->setVisible(true);
        ui->checkbox_Maptools_IndividualTextures->setVisible(false);
        ui->checkbox_Maptools_IndividualTextures->setChecked(false);

        ui->label_MapChanger_MapFileName->setText("Material Name:");


    }
    else //Dot Scene
    {
        //Hide all the dot city stuff and clear out their text boxes
        ui->CheckBox_MapArtTools_ChangeMapEveryTurn->setVisible(false);
        ui->CheckBox_Regional_CityMap->setVisible(false);
        ui->CheckBox_MapArtTools_ChangeMapEveryTurn->setChecked(false);
        ui->CheckBox_Regional_CityMap->setChecked(false);

        ui->Label_Regional_CityCoords->setVisible(false);
        ui->label_regional_topleft->setVisible(false);
        ui->label_regional_topright->setVisible(false);
        ui->label_regional_BottomLeft->setVisible(false);
        ui->label_regional_BottomRight->setVisible(false);
        ui->spinBox_RegionalCoords_TL_Lat->setVisible(false);
        ui->spinBox_RegionalCoords_TL_Long->setVisible(false);
        ui->spinBox_RegionalCoords_TR_Lat->setVisible(false);
        ui->spinBox_RegionalCoords_TR_Long->setVisible(false);
        ui->spinBox_RegionalCoords_BL_Long->setVisible(false);
        ui->spinBox_RegionalCoords_BL_Lat->setVisible(false);
        ui->spinBox_RegionalCoords_BR_Lat->setVisible(false);
        ui->spinBox_RegionalCoords_BR_Long->setVisible(false);
        ui->checkbox_Maptools_IndividualTextures->setVisible(true);


        ui->label_MapChanger_MapFileName->setText("Map File Name:");
    }
}

//Get path for Map Art Zip File. Put in Line Edit Box
void MapDataTool::on_ToolButton_MapTools_MapArtZipLocationFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Map Art Zip File", "",
                                                         "Zip File (*.zip)");

    if (openFileName != "")
    {
        ui->LineEdit_MapTools_MapArtZipLocation->setText(openFileName);
    }
}

//Get City Script Folder Path. Put in Line Edit Box
void MapDataTool::on_ToolButton_MapTools_CityScriptFolderFinder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select City Script Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(folderPath != "")
    {
        ui->LineEdit_MapTools_CityScriptFolder->setText(folderPath);
    }
}

//Get path for City Art Zip File. Put in Line Edit Box
void MapDataTool::on_Button_MapTools_CityArtZipLocationFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open City Art Zip File", "",
                                                         "Zip File (*.zip)");

    if (openFileName != "")
    {
        ui->LineEdit_MapTools_CityArtZipLocation->setText(openFileName);
    }
}

//Get path for flag art zip file. Put it in the line box.
void MapDataTool::on_Button_MapTools_FlagZipFileFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Flag Art Zip File", "",
                                                         "Zip File (*.zip)");

    if (openFileName != "")
    {
        ui->lineEdit_MapTools_FlagZipFile->setText(openFileName);
    }
}


//Get Turn Events Script Location. Put in Line Edit Box
void MapDataTool::on_Button_MapTools_TurnEventsXMLFileLocationFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Turn Events File", "",
                                                         "XML File (*.xml)");

    if (openFileName != "")
    {
        ui->LineEdit_MapTools_TurnEventsXMLFileLocation->setText(openFileName);
    }
}

//Get path for Newspaper Art Zip File. Put in Line Edit Box
void MapDataTool::on_Button_MapTools_NewspaperZipFileLocationFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Newspaper Art Zip File", "",
                                                         "Zip File (*.zip)");

    if (openFileName != "")
    {
        ui->LineEdit_MapTools_NewspaperZipFileLocation->setText(openFileName);
    }
}

//Get AI Script Folder Path. Put in Line Edit Box
void MapDataTool::on_Button_MapTools_AIScriptFolderFinder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select AI Script Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(folderPath != "")
    {
        ui->LineEdit_MapTools_AIScriptFolder->setText(folderPath);
    }
}

//Get path for AI Art Logo Zip File. Put in Line Edit Box
void MapDataTool::on_Button_MapTools_AiLogoZipLocationFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open AI Logo Art Zip File", "",
                                                         "Zip File (*.zip)");

    if (openFileName != "")
    {
        ui->LineEdit_MapTools_AiLogoZipLocation->setText(openFileName);
    }
}

//Get picture to display in game for the map!
void MapDataTool::on_Button_MapTools_MapPictureFinder_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Map Picture DDS File", "",
                                                         "DDS File (*.dds)");

    if (openFileName != "")
    {
        ui->LineEdit_MapTools_MapPicture->setText(openFileName);
    }
}

//Add Map Change Information.
void MapDataTool::on_Button_MapTools_AddMapArtworkToLoader_clicked()
{
    MapChangeData mcd;

    mcd.mapFileName = ui->LineEdit_MapTools_MapArtworkLoaderFileName->text();
    mcd.year = ui->SpinBox_MapTools_MapArtworkLoadingYear->value();

    //If it's a dot city we need to check if things are material files.
    if(ui->Radio_MapTools_DotCity->isChecked())
    {
        /*if(!mcd.mapFileName.contains(".material",Qt::CaseSensitive))
        {
            QMessageBox::critical(this,"Error!","Dot City maps must be a .material file!");
            return;
        }*/

        mcd.cityTexture = ui->LineEdit_MapTools_DotCityCityTexture->text();

        /*if(!mcd.cityTexture.contains(".material",Qt::CaseSensitive))
        {
            QMessageBox::critical(this,
                                  "Error!","Dot City Maps City Texture must be a .material file!");
            return;
        }*/

        mcd.branchTexture = ui->LineEdit_MapTools_DotCityBranchTexture->text();

        /*if(!mcd.branchTexture.contains(".material",Qt::CaseSensitive))
        {
            QMessageBox::critical(this,"Error!",
                                  "Dot City Maps Branch Texture must be a .material file!");
            return;
        }*/

        mcd.factoryTexture = ui->LineEdit_MapTools_DotCityFactoryTexture->text();
        mcd.blendTexture = ui->LineEdit_MapTools_DotCityBlendTexture->text();

        if(ui->CheckBox_MapArtTools_BlendTextures->isChecked())
        {
            mcd.blendBFTextures = "True";
        }
        else
        {
            mcd.blendBFTextures = "False";
        }

       /* if(!mcd.factoryTexture.contains(".material",Qt::CaseSensitive))
        {
            QMessageBox::critical(this,"Error!",
                                  "Dot City Maps Factory Texture must be a .material file!");
            return;
        }*/

        if(ui->CheckBox_MapArtTools_ChangeMapEveryTurn->isChecked())
        {
            mcd.turnBasedChange = "True";
        }
        else
        {
            mcd.turnBasedChange = "False";
        }

        if(ui->CheckBox_Regional_CityMap->isChecked())
        {
            mcd.regionalMap = "True";
            mcd.tlLat = ui->spinBox_RegionalCoords_TL_Lat->value();
            mcd.tlLong = ui->spinBox_RegionalCoords_TL_Long->value();
            mcd.trLat = ui->spinBox_RegionalCoords_TR_Lat->value();
            mcd.trLong = ui->spinBox_RegionalCoords_TR_Long->value();
            mcd.blLat = ui->spinBox_RegionalCoords_BL_Lat->value();
            mcd.blLong = ui->spinBox_RegionalCoords_BL_Long->value();
            mcd.brLat = ui->spinBox_RegionalCoords_BR_Lat->value();
            mcd.brLong = ui->spinBox_RegionalCoords_BR_Long->value();
        }
        else
        {
            mcd.regionalMap = "False";
            mcd.tlLat = 0;
            mcd.tlLong = 0;
            mcd.trLat = 0;
            mcd.trLong = 0;
            mcd.blLat = 0;
            mcd.blLong = 0;
            mcd.brLat = 0;
            mcd.brLong = 0;
        }

        mcd.individualTextures = "False";
    }
    else //Well, it's a dot scene, make sure the file is a .scene file.
    {
        if(!mcd.mapFileName.contains(".scene",Qt::CaseSensitive))
        {
            QMessageBox::critical(this,"Error!","Dot Scene maps must be a .scene file!");
            return;
        }

        mcd.cityTexture = ui->LineEdit_MapTools_DotCityCityTexture->text();
        mcd.branchTexture = ui->LineEdit_MapTools_DotCityBranchTexture->text();
        mcd.factoryTexture = ui->LineEdit_MapTools_DotCityFactoryTexture->text();
        mcd.turnBasedChange = "False";

        mcd.blendTexture = ui->LineEdit_MapTools_DotCityBlendTexture->text();

        if(ui->CheckBox_MapArtTools_BlendTextures->isChecked())
        {
            mcd.blendBFTextures = "True";
        }
        else
        {
            mcd.blendBFTextures = "False";
        }

        mcd.regionalMap = "False";
        mcd.tlLat = 0;
        mcd.tlLong = 0;
        mcd.trLat = 0;
        mcd.trLong = 0;
        mcd.blLat = 0;
        mcd.blLong = 0;
        mcd.brLat = 0;
        mcd.brLong = 0;

        if(ui->checkbox_Maptools_IndividualTextures->isChecked())
        {
             mcd.individualTextures = "True";
        }
        else
        {
             mcd.individualTextures = "False";
        }

    }

    //Insert into change map and refill the table.
    mapChangeMap.insert(mcd.year, mcd);
    fillMapArtworkTable();

}


//Clear Map Change Table, then fill it up with what's in the map!
void MapDataTool::fillMapArtworkTable()
{
    ui->Table_MapTools_MapArtLoadingYears->clearContents();
    ui->Table_MapTools_MapArtLoadingYears->setRowCount(mapChangeMap.size());

    int rowNumber = 0;

    for(QMap<int,MapChangeData>::iterator it = mapChangeMap.begin(); it != mapChangeMap.end(); ++it)
    {
        ui->Table_MapTools_MapArtLoadingYears->setItem(
                    rowNumber,0,new QTableWidgetItem(QString::number((*it).year)));

        ui->Table_MapTools_MapArtLoadingYears->setItem(
                    rowNumber,1,new QTableWidgetItem((*it).mapFileName));

        rowNumber++;
    }

}

//Remove Map Change from the Map based on year spinner, then refill the table.
void MapDataTool::on_Button_MapTools_RemoveMapArtworkToLoader_clicked()
{
    //Check if the year is in the map, if so, remove it and refill the table.
    if(mapChangeMap.contains(ui->SpinBox_MapTools_MapArtworkLoadingYear->value()))
    {
        mapChangeMap.remove(ui->SpinBox_MapTools_MapArtworkLoadingYear->value());
        fillMapArtworkTable();
    }
}



void MapDataTool::on_Table_MapTools_MapArtLoadingYears_cellClicked(int row, int column)
{
    int selectedYear = ui->Table_MapTools_MapArtLoadingYears->item(row,0)->text().toInt();

    if(mapChangeMap.contains(selectedYear))
    {
        QMap<int,MapChangeData>::Iterator mcmIT = mapChangeMap.find(selectedYear);
        ui->SpinBox_MapTools_MapArtworkLoadingYear->setValue(selectedYear);
        ui->LineEdit_MapTools_DotCityBranchTexture->setText((*mcmIT).branchTexture);
        ui->LineEdit_MapTools_DotCityFactoryTexture->setText((*mcmIT).factoryTexture);
        ui->LineEdit_MapTools_DotCityCityTexture->setText((*mcmIT).cityTexture);
        ui->LineEdit_MapTools_MapArtworkLoaderFileName->setText((*mcmIT).mapFileName);

        if((*mcmIT).turnBasedChange == "True")
        {
            ui->CheckBox_MapArtTools_ChangeMapEveryTurn->setChecked(true);
        }
        else
        {
            ui->CheckBox_MapArtTools_ChangeMapEveryTurn->setChecked(false);
        }

        if((*mcmIT).regionalMap == "True")
        {
            ui->CheckBox_Regional_CityMap->setChecked(true);
        }
        else
        {
            ui->CheckBox_Regional_CityMap->setChecked(false);
        }

       ui->spinBox_RegionalCoords_TL_Lat->setValue((*mcmIT).tlLat);
       ui->spinBox_RegionalCoords_TL_Long->setValue((*mcmIT).tlLong);
       ui->spinBox_RegionalCoords_TR_Lat->setValue((*mcmIT).trLat);
       ui->spinBox_RegionalCoords_TR_Long->setValue((*mcmIT).trLong);
       ui->spinBox_RegionalCoords_BL_Long->setValue((*mcmIT).blLat);
       ui->spinBox_RegionalCoords_BL_Long->setValue((*mcmIT).blLong);
       ui->spinBox_RegionalCoords_BR_Long->setValue((*mcmIT).brLat);
       ui->spinBox_RegionalCoords_BR_Long->setValue((*mcmIT).brLong);

       if((*mcmIT).individualTextures == "True")
       {
           ui->checkbox_Maptools_IndividualTextures->setChecked(true);
       }
       else
       {
           ui->checkbox_Maptools_IndividualTextures->setChecked(false);
       }
    }
}

void MapDataTool::on_Button_MapTools_AddCityScriptYearLoader_clicked()
{
    QString cityScript = ui->LineEdit_MapTools_CityScriptFileName->text();

    if(!cityScript.contains(".xml",Qt::CaseSensitive))
    {
        QMessageBox::critical(this,"Error!","City scripts must be XML files!");
        return;
    }

    //Insert into change map and refill the table.
    cityScriptMap.insert(ui->SpinBox_MapTools_CityLoadingYear->value(), cityScript);
    fillCityScriptTable();

}


void MapDataTool::fillCityScriptTable()
{
    ui->Table_MapTools_CityScriptLoadingYears->clearContents();
    ui->Table_MapTools_CityScriptLoadingYears->setRowCount(cityScriptMap.size());

    int rowNumber = 0;

    for(QMap<int,QString>::iterator it = cityScriptMap.begin();
        it != cityScriptMap.end(); ++it)
    {
        ui->Table_MapTools_CityScriptLoadingYears->setItem(
                    rowNumber,0,new QTableWidgetItem(QString::number(it.key())));

        ui->Table_MapTools_CityScriptLoadingYears->setItem(
                    rowNumber,1,new QTableWidgetItem((*it)));

        rowNumber++;
    }

}


void MapDataTool::on_Button_MapTools_RemoveCityScriptYearLoader_clicked()
{
    //Check if the year is in the map, if so, remove it and refill the table.
    if(cityScriptMap.contains(ui->SpinBox_MapTools_CityLoadingYear->value()))
    {
        cityScriptMap.remove(ui->SpinBox_MapTools_CityLoadingYear->value());
        fillCityScriptTable();
    }
}


void MapDataTool::on_Table_MapTools_CityScriptLoadingYears_cellClicked(int row, int column)
{
    int selectedYear = ui->Table_MapTools_CityScriptLoadingYears->item(row,0)->text().toInt();

    if(cityScriptMap.contains(selectedYear))
    {
        ui->LineEdit_MapTools_CityScriptFileName->setText((*cityScriptMap.find(selectedYear)));
        ui->SpinBox_MapTools_CityLoadingYear->setValue(selectedYear);
    }
}

void MapDataTool::on_Button_MapTools_AddStartingAINumbersAndFile_clicked()
{
    QString aiScript = ui->LineEdit_MapTools_StartingAIFileAdder->text();
    QString aiDesc = ui->LineEdit_MapTools_StartingAIFileDescribe->text();
    int numberAI = ui->SpinBox_MapTools_StartingAINumberSetter->value();

    mapAIData mad;

    mad.description = aiDesc;
    mad.fileName = aiScript;
    mad.number = numberAI;

    if(!aiScript.contains(".xml",Qt::CaseSensitive))
    {
        QMessageBox::critical(this,"Error!","AI scripts must be XML files!");
        return;
    }

    //Insert into change map and refill the table.
    AIScriptMap.insert(QString::number(numberAI)+" "+
                       aiScript, mad);
    fillAIScriptTable();
}

void MapDataTool::fillAIScriptTable()
{
    ui->Table_MapTools_StartingAINumbersAndFiles->clearContents();
    ui->Table_MapTools_StartingAINumbersAndFiles->setRowCount(AIScriptMap.size());

    int rowNumber = 0;

    for(QMap<QString,mapAIData>::iterator it = AIScriptMap.begin();
        it != AIScriptMap.end(); ++it)
    {
        ui->Table_MapTools_StartingAINumbersAndFiles->setItem(
                    rowNumber,0,new QTableWidgetItem(QString::number((*it).number)));

        ui->Table_MapTools_StartingAINumbersAndFiles->setItem(
                    rowNumber,1,new QTableWidgetItem((*it).fileName));

        ui->Table_MapTools_StartingAINumbersAndFiles->setItem(
                    rowNumber,2,new QTableWidgetItem((*it).description));

        rowNumber++;
    }

}
void MapDataTool::on_Button_MapTools_RemoveStartingAINumberAndFile_clicked()
{
    int number = ui->SpinBox_MapTools_StartingAINumberSetter->value();
    QString aiFile = ui->LineEdit_MapTools_StartingAIFileAdder->text();

    //Check if the year is in the map, if so, remove it and refill the table.
    if(AIScriptMap.contains(QString::number(number)+" "+aiFile))
    {
        AIScriptMap.remove(QString::number(number)+" "+aiFile);
        fillAIScriptTable();
    }
}



void MapDataTool::on_Table_MapTools_StartingAINumbersAndFiles_cellClicked(int row, int column)
{
    QString selectedValue = ui->Table_MapTools_StartingAINumbersAndFiles->item(row,0)->text();
    QString aiFileName = ui->Table_MapTools_StartingAINumbersAndFiles->item(row,1)->text();

    if(AIScriptMap.contains(selectedValue+" "+aiFileName))
    {
        ui->LineEdit_MapTools_StartingAIFileAdder->setText(
                    (*AIScriptMap.find(selectedValue+" "+aiFileName)).fileName);
        ui->LineEdit_MapTools_StartingAIFileDescribe->setText(
                    (*AIScriptMap.find(selectedValue+" "+aiFileName)).description);
        ui->SpinBox_MapTools_StartingAINumberSetter->setValue(selectedValue.toInt());
    }
}

void MapDataTool::on_Button_MapData_Save_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");

    if (!saveFileName.endsWith(".xml"))
        saveFileName += ".xml";

    saveMapLoaderData(saveFileName);
}


void MapDataTool::saveMapLoaderData(QString fileName)
{
    //Open Save File
    QFile saveMapFile(fileName);
    if(!saveMapFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(fileName,
                                                                    saveMapFile.errorString()));

       return;
    }


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveMapFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("MapInformationLoader");
      xmlWriter.writeTextElement("MapName",ui->LineEdit_MapTools_MapName->text());
      xmlWriter.writeTextElement("Version",
                                 QString::number(ui->SpinBox_MapTools_MapVersionNumber->value()));
      xmlWriter.writeTextElement("AuthorName",ui->LineEdit_MapTools_Author->text());
      xmlWriter.writeTextElement("AuthorLink",ui->LineEdit_MapTools_AuthorLink->text());

      xmlWriter.writeTextElement("MapDesc", ui->TextEdit_MapTools_MapDescription->toPlainText());
      xmlWriter.writeTextElement("MapPicture", ui->LineEdit_MapTools_MapPicture->text());


        bool dotCity = ui->Radio_MapTools_DotCity->isChecked();
        if(dotCity)
            xmlWriter.writeTextElement("MapType","0");
        else
            xmlWriter.writeTextElement("MapType","1");


      xmlWriter.writeTextElement("MapArtLocation",ui->LineEdit_MapTools_MapArtZipLocation->text());



    //Loop through Map and write the data
       xmlWriter.writeStartElement("MapChanges");
    for(QMap<int,MapChangeData>::iterator it = mapChangeMap.begin(); it != mapChangeMap.end(); ++it)
    {
        xmlWriter.writeStartElement("ChangeMap");
            xmlWriter.writeAttribute("year",QString::number((*it).year));
            xmlWriter.writeAttribute("cityTexture",(*it).cityTexture);
            xmlWriter.writeAttribute("branchTexture",(*it).branchTexture);
            xmlWriter.writeAttribute("factoryTexture",(*it).factoryTexture);
            xmlWriter.writeAttribute("turnBasedChange",(*it).turnBasedChange);
            xmlWriter.writeAttribute("individualTextures",(*it).individualTextures);
            xmlWriter.writeAttribute("regionalMap",(*it).regionalMap);
                xmlWriter.writeAttribute("tlLat",QString::number((*it).tlLat));
                xmlWriter.writeAttribute("tlLong",QString::number((*it).tlLong));
                xmlWriter.writeAttribute("trLat",QString::number((*it).trLat));
                xmlWriter.writeAttribute("trLong",QString::number((*it).trLong));
                xmlWriter.writeAttribute("blLat",QString::number((*it).blLat));
                xmlWriter.writeAttribute("blLong",QString::number((*it).blLong));
                xmlWriter.writeAttribute("brLat",QString::number((*it).brLat));
                xmlWriter.writeAttribute("brLong",QString::number((*it).brLong));
            xmlWriter.writeCharacters((*it).mapFileName);
        xmlWriter.writeEndElement(); //ChangeMap
    }
    xmlWriter.writeEndElement(); //MapChange

    xmlWriter.writeTextElement("CityScriptFolder",ui->LineEdit_MapTools_CityScriptFolder->text());
    xmlWriter.writeTextElement("CityArtLocation",ui->LineEdit_MapTools_CityArtZipLocation->text());


    //Loop through Map and write the data
    xmlWriter.writeStartElement("CityScriptChanges");
    for(QMap<int,QString>::iterator it = cityScriptMap.begin(); it != cityScriptMap.end(); ++it)
    {
        xmlWriter.writeStartElement("ChangeCityScripts");
            xmlWriter.writeAttribute("year",QString::number(it.key()));
            xmlWriter.writeCharacters((*it));
        xmlWriter.writeEndElement(); //ChangeCityScripts
    }
    xmlWriter.writeEndElement(); //CityScriptChanges

    xmlWriter.writeTextElement("FlagFile",ui->lineEdit_MapTools_FlagZipFile->text());


    xmlWriter.writeTextElement("TurnEventsFile",
                               ui->LineEdit_MapTools_TurnEventsXMLFileLocation->text());
    xmlWriter.writeTextElement("NewspaperArtLocation",
                               ui->LineEdit_MapTools_NewspaperZipFileLocation->text());

    xmlWriter.writeTextElement("AIScriptFolder",ui->LineEdit_MapTools_AIScriptFolder->text());
    xmlWriter.writeTextElement("AILogoFile",ui->LineEdit_MapTools_AiLogoZipLocation->text());

    //Loop through Map and write the data
    xmlWriter.writeStartElement("AIStartingOptions");
    for(QMap<QString,mapAIData>::iterator it = AIScriptMap.begin(); it != AIScriptMap.end(); ++it)
    {
        xmlWriter.writeStartElement("AIStart");
            xmlWriter.writeAttribute("number",QString::number((*it).number));
            xmlWriter.writeAttribute("file",(*it).fileName);
            xmlWriter.writeCharacters((*it).description);
        xmlWriter.writeEndElement(); //AIStart
    }
    xmlWriter.writeEndElement(); //AIStartingOptions

    xmlWriter.writeTextElement("FactorySizeAdjust",ui->SpinBox_MapTools_FactorySizeAdjust->text());
    xmlWriter.writeTextElement("BranchDealerAdjust",
                               ui->SpinBox_MapTools_BranchDealerAdjust->text());

    if(ui->CheckBox_SupportsLowDefinitionTextures->isChecked())
    {
        xmlWriter.writeTextElement("SupportLDTextures","True");
    }
    else
    {
        xmlWriter.writeTextElement("SupportLDTextures","False");
    }

    xmlWriter.writeEndDocument();

    saveMapFile.close();

    //We Did it! Yay!
    QMessageBox::information(this,"Save Complete",
                             "The Map File has been saved!", QMessageBox::Ok);



}

void MapDataTool::on_Button_MapData_Open_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Map File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        openMapFile(openFileName);
    }
}


void MapDataTool::openMapFile(QString openFileName)
{
    mapChangeMap.clear();
    cityScriptMap.clear();
    AIScriptMap.clear();

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
        QMessageBox::critical(this,"Error Processing TurnEvents File",
                             QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                             errorMsg,errorLine,errorColumn));
       return;
    }

    QDomElement rootNode = doc.firstChildElement("MapInformationLoader");
    if(rootNode.isNull())
    {
         QMessageBox::critical(this,"Error","Map Loader Data is malformed.");
        return;
    }

    rootNode = doc.documentElement();
    baseElement = rootNode.firstChildElement("MapName");
    ui->LineEdit_MapTools_MapName->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("Version");
    ui->SpinBox_MapTools_MapVersionNumber->setValue(baseElement.text().toDouble());

    baseElement = rootNode.firstChildElement("AuthorName");
    ui->LineEdit_MapTools_Author->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("AuthorLink");
    ui->LineEdit_MapTools_AuthorLink->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("MapDesc");
    ui->TextEdit_MapTools_MapDescription->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("MapPicture");
    ui->LineEdit_MapTools_MapPicture->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("MapType");
    if(baseElement.text().toInt() == 0)
        ui->Radio_MapTools_DotCity->setChecked(true);
    else
        ui->Radio_MapTools_DotScene->setChecked(true);

    baseElement = rootNode.firstChildElement("MapArtLocation");
    ui->LineEdit_MapTools_MapArtZipLocation->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("MapChanges");
    subElement = baseElement.firstChildElement("ChangeMap");

    while(!subElement.isNull())
    {
        MapChangeData mcd;
        mcd.year = subElement.attributeNode("year").value().toInt();
        mcd.cityTexture = subElement.attributeNode("cityTexture").value();
        mcd.branchTexture = subElement.attributeNode("branchTexture").value();
        mcd.factoryTexture = subElement.attributeNode("factoryTexture").value();
        if(!subElement.attributeNode("turnBasedChange").isNull())
        {
            mcd.turnBasedChange = subElement.attributeNode("turnBasedChange").value();
        }
        else
        {
            mcd.turnBasedChange = "False";
        }

        if(!subElement.attributeNode("individualTextures").isNull())
        {
            mcd.individualTextures = subElement.attributeNode("individualTextures").value();
        }
        else
        {
            mcd.individualTextures = "False";
        }

        if(!subElement.attributeNode("blendBranchAndFactory").isNull())
        {
            mcd.blendBFTextures = subElement.attributeNode("blendBranchAndFactory").value();
        }
        else
        {
            mcd.blendBFTextures = "False";
        }

        if(!subElement.attributeNode("blendTexture").isNull())
        {
            mcd.blendTexture = subElement.attributeNode("blendTexture").value();
        }
        else
        {
            mcd.blendTexture = "";
        }



        if(!subElement.attributeNode("regionalMap").isNull())
        {
            mcd.regionalMap = subElement.attributeNode("regionalMap").value();
            mcd.tlLat =  subElement.attributeNode("tlLat").value().toDouble();
            mcd.tlLong = subElement.attributeNode("tlLong").value().toDouble();
            mcd.trLat = subElement.attributeNode("trLat").value().toDouble();
            mcd.trLong = subElement.attributeNode("trLong").value().toDouble();
            mcd.blLat = subElement.attributeNode("blLat").value().toDouble();
            mcd.blLong = subElement.attributeNode("blLong").value().toDouble();
            mcd.brLat = subElement.attributeNode("brLat").value().toDouble();
            mcd.brLong = subElement.attributeNode("brLong").value().toDouble();

        }
        else
        {
            mcd.regionalMap = "False";
            mcd.tlLat = 0;
            mcd.tlLong = 0;
            mcd.trLat = 0;
            mcd.trLong = 0;
            mcd.blLat = 0;
            mcd.blLong = 0;
            mcd.brLat = 0;
            mcd.brLong = 0;
        }


        mcd.mapFileName = subElement.text();

        mapChangeMap.insert(mcd.year,mcd);

        subElement = subElement.nextSiblingElement("ChangeMap");
    }

    baseElement = rootNode.firstChildElement("CityScriptFolder");
    ui->LineEdit_MapTools_CityScriptFolder->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("CityArtLocation");
    ui->LineEdit_MapTools_CityArtZipLocation->setText(baseElement.text());


    baseElement = rootNode.firstChildElement("CityScriptChanges");
    subElement = baseElement.firstChildElement("ChangeCityScripts");

    while(!subElement.isNull())
    {
        int year = subElement.attributeNode("year").value().toInt();
        QString scriptName = subElement.text();

        cityScriptMap.insert(year,scriptName);

        subElement = subElement.nextSiblingElement("ChangeCityScripts");
    }

    baseElement = rootNode.firstChildElement("FlagFile");
    ui->lineEdit_MapTools_FlagZipFile->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("TurnEventsFile");
    ui->LineEdit_MapTools_TurnEventsXMLFileLocation->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("NewspaperArtLocation");
    ui->LineEdit_MapTools_NewspaperZipFileLocation->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("AIScriptFolder");
    ui->LineEdit_MapTools_AIScriptFolder->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("AILogoFile");
    ui->LineEdit_MapTools_AiLogoZipLocation->setText(baseElement.text());

    baseElement = rootNode.firstChildElement("AIStartingOptions");
    subElement = baseElement.firstChildElement("AIStart");

    while(!subElement.isNull())
    {
        int number = subElement.attributeNode("number").value().toInt();
        QString aiFile = subElement.attributeNode("file").value();
        QString descriptionName = subElement.text();

        mapAIData mad;

        mad.number = number;
        mad.fileName = aiFile;
        mad.description = descriptionName;

        AIScriptMap.insert(QString::number(mad.number)+" "+mad.fileName,mad);

        subElement = subElement.nextSiblingElement("AIStart");
    }

    baseElement = rootNode.firstChildElement("FactorySizeAdjust");
    ui->SpinBox_MapTools_FactorySizeAdjust->setValue(baseElement.text().toDouble());

    baseElement = rootNode.firstChildElement("BranchDealerAdjust");
    ui->SpinBox_MapTools_BranchDealerAdjust->setValue(baseElement.text().toDouble());

    ui->CheckBox_SupportsLowDefinitionTextures->setChecked(false);
     baseElement = rootNode.firstChildElement("SupportLDTextures");
     if(!baseElement.isNull())
     {
         if(baseElement.text() == "True")
             ui->CheckBox_SupportsLowDefinitionTextures->setChecked(true);
     }

    fillMapArtworkTable();
    fillCityScriptTable();
    fillAIScriptTable();
}


void MapDataTool::on_Button_MapData_Export_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder To Export To", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(folderPath != "")
    {
        exportMap(folderPath);
    }
}


void MapDataTool::exportMap(QString parentFolder)
{
    checkExportErrors();

    #if defined(Q_WS_WIN)
        QString mapFolder = parentFolder +"\\"+ ui->LineEdit_MapTools_MapName->text()+"\\";
    #else
        QString mapFolder = parentFolder +"/"+ ui->LineEdit_MapTools_MapName->text()+"/";
    #endif


    if(!QDir(mapFolder).exists())
    {
        QDir().mkdir(mapFolder);
    }

    saveMapInfoBaseFile(mapFolder);

    //MapPicture
    QFile::copy(ui->LineEdit_MapTools_MapPicture->text(), mapFolder+"mapPicture.dds");

    //MapArt
    #if defined(Q_WS_WIN)
        QString mapArtFolder = mapFolder+"MapArt\\";
    #else
        QString mapArtFolder = mapFolder+"MapArt/";
    #endif

    if(!QDir(mapArtFolder).exists())
    {
        QDir().mkdir(mapArtFolder);
    }

    QFile::copy(ui->LineEdit_MapTools_MapArtZipLocation->text(), mapArtFolder+"MapArt.zip");

    //City Script Folder
    #if defined(Q_WS_WIN)
        QString scriptFolder = mapFolder+"scripts\\";
        QString baseCityScriptFolder = ui->LineEdit_MapTools_CityScriptFolder->text() +"\\";
    #else
        QString scriptFolder = mapFolder+"scripts/";
        QString baseCityScriptFolder = ui->LineEdit_MapTools_CityScriptFolder->text() +"/";
    #endif

    if(!QDir(scriptFolder).exists())
    {
        QDir().mkdir(scriptFolder);
    }

    for(QMap<int,QString>::Iterator it = cityScriptMap.begin();
        it != cityScriptMap.end(); ++it)
    {
        QFile::copy(baseCityScriptFolder+(*it),scriptFolder+(*it));
    }

    //CityArt
    #if defined(Q_WS_WIN)
        QString cityArtFolder = mapFolder+"CityArt\\";
    #else
        QString cityArtFolder = mapFolder+"CityArt/";
    #endif

    if(!QDir(cityArtFolder).exists())
    {
        QDir().mkdir(cityArtFolder);
    }

    QFile::copy(ui->LineEdit_MapTools_CityArtZipLocation->text(), cityArtFolder+"CityArt.zip");


    //FlagArt
    #if defined(Q_WS_WIN)
        QString flagArtFolder = mapFolder+"FlagArt\\";
    #else
        QString flagArtFolder = mapFolder+"FlagArt/";
    #endif

    if(!QDir(flagArtFolder).exists())
    {
        QDir().mkdir(flagArtFolder);
    }

    QFile::copy(ui->lineEdit_MapTools_FlagZipFile->text(), flagArtFolder+"Flags.zip");


    //Turn Events File
    if(!QDir(scriptFolder).exists())
    {
        QDir().mkdir(scriptFolder);
    }

    QFile::copy(ui->LineEdit_MapTools_TurnEventsXMLFileLocation->text(),
                scriptFolder+"TurnEvents.xml");


    //NewspaperArt
    #if defined(Q_WS_WIN)
        QString newsArtFolder = mapFolder+"NewsArt\\";
    #else
        QString newsArtFolder = mapFolder+"NewsArt/";
    #endif

    if(!QDir(newsArtFolder).exists())
    {
        QDir().mkdir(newsArtFolder);
    }

    QFile::copy(ui->LineEdit_MapTools_NewspaperZipFileLocation->text(),
                newsArtFolder+"News.zip");


    //AI Script Folder
    #if defined(Q_WS_WIN)
        QString baseAIScriptFolder = ui->LineEdit_MapTools_AIScriptFolder->text() +"\\";
    #else
        QString baseAIScriptFolder = ui->LineEdit_MapTools_AIScriptFolder->text() +"/";
    #endif

    if(!QDir(scriptFolder).exists())
    {
        QDir().mkdir(scriptFolder);
    }

    for(QMap<QString,mapAIData>::Iterator it = AIScriptMap.begin();
        it != AIScriptMap.end(); ++it)
    {
        QFile::copy(baseAIScriptFolder+(*it).fileName,scriptFolder+(*it).fileName);
    }


    //AILogoArt
    #if defined(Q_WS_WIN)
        QString logoArtFolder = mapFolder+"AILogos\\";
    #else
        QString logoArtFolder = mapFolder+"AILogos/";
    #endif

    if(!QDir(logoArtFolder).exists())
    {
        QDir().mkdir(logoArtFolder);
    }

    QFile::copy(ui->LineEdit_MapTools_AiLogoZipLocation->text(),
                logoArtFolder+"AILogos.zip");

    saveLoaderForModToolsExport(mapFolder);

    //We Did it! Yay!
    QMessageBox::information(this,"Export Complete",
                             "The Map has been successfully exported!", QMessageBox::Ok);

}

void MapDataTool::saveMapInfoBaseFile(QString mapFolder)
{
    QFile saveMapFile(mapFolder+"MapFileForGame.xml");
    if(!saveMapFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(mapFolder+"MapInfo.xml",
                                                              saveMapFile.errorString()));
       return;
    }


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveMapFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("MapInfo");
      xmlWriter.writeTextElement("MapName",ui->LineEdit_MapTools_MapName->text());
      xmlWriter.writeTextElement("Version",
                 QString::number(ui->SpinBox_MapTools_MapVersionNumber->value()));
      xmlWriter.writeTextElement("AuthorName",ui->LineEdit_MapTools_Author->text());
      xmlWriter.writeTextElement("AuthorLink",ui->LineEdit_MapTools_AuthorLink->text());

      xmlWriter.writeTextElement("MapDesc", ui->TextEdit_MapTools_MapDescription->toPlainText());

      xmlWriter.writeTextElement("MapPicture", "mapPicture.dds");

      if(ui->Radio_MapTools_DotCity->isChecked())
          xmlWriter.writeTextElement("MapType", "0");
      else
          xmlWriter.writeTextElement("MapType", "1");

      xmlWriter.writeTextElement("MapArt", "MapArt/MapArt.zip");
      xmlWriter.writeTextElement("Scripts", "scripts/");
      xmlWriter.writeTextElement("CityArt", "CityArt/CityArt.zip");
      xmlWriter.writeTextElement("FlagArt", "FlagArt/Flags.zip");
      xmlWriter.writeTextElement("NewsArt", "NewsArt/News.zip");
      xmlWriter.writeTextElement("AILogos", "AILogos/AILogos.zip");

      xmlWriter.writeStartElement("MapChanges");
          for(QMap<int,MapChangeData>::iterator it = mapChangeMap.begin();
              it != mapChangeMap.end(); ++it)
          {
              xmlWriter.writeStartElement("ChangeMap");
                  xmlWriter.writeAttribute("year",QString::number((*it).year));
                  xmlWriter.writeAttribute("cityTexture",(*it).cityTexture);
                  xmlWriter.writeAttribute("branchTexture",(*it).branchTexture);
                  xmlWriter.writeAttribute("factoryTexture",(*it).factoryTexture);
                  xmlWriter.writeAttribute("blendTexture",(*it).blendTexture);
                  xmlWriter.writeAttribute("turnBasedChange",(*it).turnBasedChange);
                  xmlWriter.writeAttribute("individualTextures",(*it).individualTextures);
                  xmlWriter.writeAttribute("blendBranchAndFactory",(*it).blendBFTextures);
                  xmlWriter.writeAttribute("regionalMap",(*it).regionalMap);
                      xmlWriter.writeAttribute("tlLat",QString::number((*it).tlLat));
                      xmlWriter.writeAttribute("tlLong",QString::number((*it).tlLong));
                      xmlWriter.writeAttribute("trLat",QString::number((*it).trLat));
                      xmlWriter.writeAttribute("trLong",QString::number((*it).trLong));
                      xmlWriter.writeAttribute("blLat",QString::number((*it).blLat));
                      xmlWriter.writeAttribute("blLong",QString::number((*it).blLong));
                      xmlWriter.writeAttribute("brLat",QString::number((*it).brLat));
                      xmlWriter.writeAttribute("brLong",QString::number((*it).brLong));
                  xmlWriter.writeCharacters((*it).mapFileName);
              xmlWriter.writeEndElement(); //ChangeMap
          }
      xmlWriter.writeEndElement();//MapChanges

      xmlWriter.writeStartElement("CityScriptChanges");
      for(QMap<int,QString>::iterator it = cityScriptMap.begin(); it != cityScriptMap.end(); ++it)
      {
          xmlWriter.writeStartElement("ChangeCityScripts");
              xmlWriter.writeAttribute("year",QString::number(it.key()));
              xmlWriter.writeCharacters((*it));
          xmlWriter.writeEndElement(); //ChangeCityScripts
      }
      xmlWriter.writeEndElement(); //CityScriptChanges

      xmlWriter.writeStartElement("AIStartingOptions");
      for(QMap<QString,mapAIData>::iterator it = AIScriptMap.begin(); it != AIScriptMap.end(); ++it)
      {
          xmlWriter.writeStartElement("AIStart");
              xmlWriter.writeAttribute("number",QString::number((*it).number));
              xmlWriter.writeAttribute("file",(*it).fileName);
              xmlWriter.writeCharacters((*it).description);
          xmlWriter.writeEndElement(); //AIStart
      }
      xmlWriter.writeEndElement(); //AIStartingOptions

      xmlWriter.writeTextElement("EnableEdgeWarp","False");

      xmlWriter.writeTextElement("FactorySizeAdjust",ui->SpinBox_MapTools_FactorySizeAdjust->text());
      xmlWriter.writeTextElement("BranchDealerAdjust",
                                 ui->SpinBox_MapTools_BranchDealerAdjust->text());

      if(ui->CheckBox_SupportsLowDefinitionTextures->isChecked())
      {
          xmlWriter.writeTextElement("SupportLDTextures","True");
      }
      else
      {
          xmlWriter.writeTextElement("SupportLDTextures","False");
      }


    xmlWriter.writeEndDocument();

    saveMapFile.close();


}



void MapDataTool::saveLoaderForModToolsExport(QString folder)
{
    QString pathFromModTools = "../media/Maps/"+ui->LineEdit_MapTools_MapName->text()+"/";
    //Open Save File
    QFile saveMapFile(folder+"MapFileForModTools.xml");
    if(!saveMapFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(folder+"MapFileForModTools.xml",
                                                                    saveMapFile.errorString()));

       return;
    }


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveMapFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("MapInformationLoader");
      xmlWriter.writeTextElement("MapName",ui->LineEdit_MapTools_MapName->text());
      xmlWriter.writeTextElement("Version",
                                 QString::number(ui->SpinBox_MapTools_MapVersionNumber->value()));
      xmlWriter.writeTextElement("AuthorName",ui->LineEdit_MapTools_Author->text());
      xmlWriter.writeTextElement("AuthorLink",ui->LineEdit_MapTools_AuthorLink->text());

      xmlWriter.writeTextElement("MapDesc", ui->TextEdit_MapTools_MapDescription->toPlainText());
      xmlWriter.writeTextElement("MapPicture", pathFromModTools+"mapPicture.dds");


        bool dotCity = ui->Radio_MapTools_DotCity->isChecked();
        if(dotCity)
            xmlWriter.writeTextElement("MapType","0");
        else
            xmlWriter.writeTextElement("MapType","1");


      xmlWriter.writeTextElement("MapArtLocation",pathFromModTools+"MapArt/MapArt.zip");



    //Loop through Map and write the data
       xmlWriter.writeStartElement("MapChanges");
    for(QMap<int,MapChangeData>::iterator it = mapChangeMap.begin(); it != mapChangeMap.end(); ++it)
    {
        xmlWriter.writeStartElement("ChangeMap");
            xmlWriter.writeAttribute("year",QString::number((*it).year));
            xmlWriter.writeAttribute("cityTexture",(*it).cityTexture);
            xmlWriter.writeAttribute("branchTexture",(*it).branchTexture);
            xmlWriter.writeAttribute("factoryTexture",(*it).factoryTexture);
            xmlWriter.writeAttribute("blendTexture",(*it).blendTexture);
            xmlWriter.writeAttribute("turnBasedChange",(*it).turnBasedChange);
            xmlWriter.writeAttribute("individualTextures",(*it).individualTextures);
            xmlWriter.writeAttribute("blendBranchAndFactory",(*it).blendBFTextures);
            xmlWriter.writeAttribute("regionalMap",(*it).regionalMap);
                xmlWriter.writeAttribute("tlLat",QString::number((*it).tlLat));
                xmlWriter.writeAttribute("tlLong",QString::number((*it).tlLong));
                xmlWriter.writeAttribute("trLat",QString::number((*it).trLat));
                xmlWriter.writeAttribute("trLong",QString::number((*it).trLong));
                xmlWriter.writeAttribute("blLat",QString::number((*it).blLat));
                xmlWriter.writeAttribute("blLong",QString::number((*it).blLong));
                xmlWriter.writeAttribute("brLat",QString::number((*it).brLat));
                xmlWriter.writeAttribute("brLong",QString::number((*it).brLong));
            xmlWriter.writeCharacters((*it).mapFileName);
        xmlWriter.writeEndElement(); //ChangeMap
    }
    xmlWriter.writeEndElement(); //MapChange

    xmlWriter.writeTextElement("CityScriptFolder",pathFromModTools+"scripts/");
    xmlWriter.writeTextElement("CityArtLocation",pathFromModTools+"CityArt/CityArt.zip");


    //Loop through Map and write the data
    xmlWriter.writeStartElement("CityScriptChanges");
    for(QMap<int,QString>::iterator it = cityScriptMap.begin(); it != cityScriptMap.end(); ++it)
    {
        xmlWriter.writeStartElement("ChangeCityScripts");
            xmlWriter.writeAttribute("year",QString::number(it.key()));
            xmlWriter.writeCharacters((*it));
        xmlWriter.writeEndElement(); //ChangeCityScripts
    }
    xmlWriter.writeEndElement(); //CityScriptChanges

    xmlWriter.writeTextElement("FlagFile",pathFromModTools+"FlagArt/Flags.zip");


    xmlWriter.writeTextElement("TurnEventsFile",pathFromModTools+"scripts/TurnEvents.xml");
    xmlWriter.writeTextElement("NewspaperArtLocation",pathFromModTools+"NewsArt/News.zip");

    xmlWriter.writeTextElement("AIScriptFolder",pathFromModTools+"scripts/");
    xmlWriter.writeTextElement("AILogoFile",pathFromModTools+"AILogos/AILogos.zip");

    //Loop through Map and write the data
    xmlWriter.writeStartElement("AIStartingOptions");
    for(QMap<QString,mapAIData>::iterator it = AIScriptMap.begin(); it != AIScriptMap.end(); ++it)
    {
        xmlWriter.writeStartElement("AIStart");
            xmlWriter.writeAttribute("number",QString::number((*it).number));
            xmlWriter.writeAttribute("file",(*it).fileName);
            xmlWriter.writeCharacters((*it).description);
        xmlWriter.writeEndElement(); //AIStart
    }
    xmlWriter.writeEndElement(); //AIStartingOptions

    xmlWriter.writeTextElement("FactorySizeAdjust",ui->SpinBox_MapTools_FactorySizeAdjust->text());
    xmlWriter.writeTextElement("BranchDealerAdjust",
                               ui->SpinBox_MapTools_BranchDealerAdjust->text());

    if(ui->CheckBox_SupportsLowDefinitionTextures->isChecked())
    {
        xmlWriter.writeTextElement("SupportLDTextures","True");
    }
    else
    {
        xmlWriter.writeTextElement("SupportLDTextures","False");
    }

    xmlWriter.writeEndDocument();

    saveMapFile.close();

}


bool MapDataTool::checkExportErrors()
{
    bool haveError = false;
    QString errorMessage = "";

    if(ui->LineEdit_MapTools_MapName->text() == "")
    {
        haveError = true;
        errorMessage = "You must name the map.";
    }
    else if(ui->LineEdit_MapTools_MapPicture->text() == "")
    {
        haveError = true;
        errorMessage = "You must have a picture for the map.";
    }
    else if(!ui->LineEdit_MapTools_MapPicture->text().endsWith(".dds"))
    {
        haveError = true;
        errorMessage = "Map picture must be in DDS format.";
    }
    else if(ui->LineEdit_MapTools_MapArtZipLocation->text() == "")
    {
        haveError = true;
        errorMessage = "You must have Map Art Zip File.";
    }
    else if(!ui->LineEdit_MapTools_MapArtZipLocation->text().endsWith(".zip"))
    {
        haveError = true;
        errorMessage = "Map Art must be in a zip file. Remember, no folders can be in the zip"
                "file";
    }
    else if(ui->LineEdit_MapTools_CityScriptFolder->text() == "")
    {
        haveError = true;
        errorMessage = "You must have a folder with script files.";
    }
    else if(ui->LineEdit_MapTools_CityArtZipLocation->text() == "")
    {
        haveError = true;
        errorMessage = "You must have City Art Zip File.";
    }
    else if(!ui->LineEdit_MapTools_CityArtZipLocation->text().endsWith(".zip"))
    {
        haveError = true;
        errorMessage = "City Art must be in a zip file. Remember, no folders can be in the zip"
                "file";
    }
    else if(ui->lineEdit_MapTools_FlagZipFile->text() == "")
    {
        haveError = true;
        errorMessage = "You must have Flag Art Zip File.";
    }
    else if(!ui->lineEdit_MapTools_FlagZipFile->text().endsWith(".zip"))
    {
        haveError = true;
        errorMessage = "Flag Art must be in a zip file. Remember, no folders can be in the zip"
                "file";
    }
    else if(ui->LineEdit_MapTools_NewspaperZipFileLocation->text() == "")
    {
        haveError = true;
        errorMessage = "You must have Newspaper Art Zip File.";
    }
    else if(!ui->LineEdit_MapTools_NewspaperZipFileLocation->text().endsWith(".zip"))
    {
        haveError = true;
        errorMessage = "Newspaper Art must be in a zip file. Remember, no folders can be in the zip"
                "file";
    }
    else if(ui->LineEdit_MapTools_AiLogoZipLocation->text() == "")
    {
        haveError = true;
        errorMessage = "You must have AILogo Zip File.";
    }
    else if(!ui->LineEdit_MapTools_AiLogoZipLocation->text().endsWith(".zip"))
    {
        haveError = true;
        errorMessage = "AILogo File must be in a zip file. Remember, no folders can be in the zip"
                "file";
    }
    else if(ui->LineEdit_MapTools_AIScriptFolder->text() == "")
    {
        haveError = true;
        errorMessage = "You must have a folder with AI script files.";
    }
    else if(ui->LineEdit_MapTools_TurnEventsXMLFileLocation->text() == "")
    {
        haveError = true;
        errorMessage = "You must have a turn events file selected.";
    }
    else if(!ui->LineEdit_MapTools_TurnEventsXMLFileLocation->text().endsWith(".xml"))
    {
        haveError = true;
        errorMessage = "Turn events file must be an XML file.";
    }

    if(haveError)
    {
        QMessageBox::critical(this,"Error!",errorMessage);
        return false;
    }

    return true;
}

void MapDataTool::on_Button_MapTool_MaterialEditor_clicked()
{
    cp_wsc.MaterialEditorWindow->show();
}
