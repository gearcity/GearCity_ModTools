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


#include "AIEditor.h"
#include "ui_AIEditor.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include "QLongLongValidator.h"
#include <qmath.h>

//Create AIEditor Tool
AIEditor::AIEditor(widgetContainerStorage wsc, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AIEditor)
{
    ui->setupUi(this);

    //Copy the widget storage container
    cp_wsc = wsc;

    //Disable table list refill until we need to fill the tables.
    refillList = false;

    //Make sure some lines only get numbers.
    ui->line_StartingFunds->setValidator(new qdesigner_internal::QLongLongValidator(this));
    ui->line_ExistingLoans->setValidator(new qdesigner_internal::QLongLongValidator(this));

    //Makes Table Headers sortable.
    connect(ui->table_AICompanyList->horizontalHeader(),SIGNAL(sectionClicked(int)),
            this,SLOT(sectionDoubleClickedSlot(int)));

    //Remove's List Number Column.
    ui->table_AICompanyList->verticalHeader()->setVisible(false);


    //Automatically Create "New" List of AI companies.
    on_Button_makeNewAIList_clicked();
    saveFileName= "";

}

AIEditor::~AIEditor()
{
    delete ui;
}

void AIEditor::on_Button_AI_ReturnToPrevious_clicked()
{
    //Lowers the AIEditor, making the previous container the top container.
    cp_wsc.AIEditorCW->lower();
}


void AIEditor::on_Button_AI_MapEditor_clicked()
{
    //Raise the MapTool
    cp_wsc.MapToolCW->raise();
}


void AIEditor::on_Button_AI_CityEditor_clicked()
{
    //Raise the CityEditor
    cp_wsc.CityEditorCW->raise();
}

//Assigns all values needed for the new AI node into the AIInfo class.
//Then sends it to be saved or edited
void AIEditor::on_button_SaveAINode_clicked()
{
    //Make a new AI company.
    AIManager::AIInfo aiNode;

    //Gather up all it's information
    aiNode.id = ui->Label_AIID->text().toInt();
    aiNode.name = ui->lineEdit_AIName->text();
    aiNode.ownerID = ui->spin_OwnerID->value();
    aiNode.hq = ui->spin_HQID->value();
    aiNode.startYear = ui->spin_FoundingYear->value();
    aiNode.deathYear = ui->spin_DeathYear->value();
    aiNode.logo = ui->line_AILogo->text();
    aiNode.funds = ui->line_StartingFunds->text().toLongLong();
    aiNode.loan = ui->line_ExistingLoans->text().toLongLong();
    aiNode.credit = ui->combo_CreditRating->currentIndex();
    aiNode.manuSkills = ui->spin_ManuSkills->value();
    aiNode.rndSkills = ui->spin_RnDSkills->value();
    aiNode.adminSkills = ui->spin_AdminSkills->value();
    aiNode.marketingSkills = ui->spin_MarketingSkills->value();
    aiNode.dealerSkills = ui->spin_DealershipSkills->value();
    aiNode.engineDesign  = ui->spin_EngineDesignSkills->value();
    aiNode.chassisDesign  = ui->spin_ChassisDesignSkills->value();
    aiNode.gearboxDesign  = ui->spin_GearboxDesignSkills->value();
    aiNode.bodyDesign  = ui->spin_BodyDesignSkills->value();
    aiNode.luxDesign  = ui->spin_LuxuryDesignSkills->value();
    aiNode.safeDesign = ui->spin_SafetyDesignSkills->value();
    aiNode.globalImage  = ui->spin_GlobalImage->value();
    aiNode.qualityImage  = ui->spin_QualityImage->value();
    aiNode.racingImage  = ui->spin_RacingImage->value();
    aiNode.workImage = ui->spin_WorkmanImage->value();
    aiNode.genericDesigner = ui->check_GenericDesign->isChecked();
    aiNode.performanceBehavoir  = ui->spin_PerformanceBehave->value();
    aiNode.drivingBehavoir  = ui->spin_DriveBehave->value();
    aiNode.luxuryBehavoir  = ui->spin_LuxuryBehave->value();
    aiNode.safetyBehavoir  = ui->spin_SafetyBehave->value();
    aiNode.fuelBehavoir = ui->spin_FuelBehave->value();
    aiNode.powerBehavoir  = ui->spin_PowereBehave->value();
    aiNode.cargoBehavoir  = ui->spin_CargoBehave->value();
    aiNode.dependabilityBehavoir = ui->spin_DependBehave->value();
    aiNode.designAgression = ui->spin_DesignAgression->value();
    aiNode.sellAgression = ui->spin_SalesAggression->value();
    aiNode.buildAgression  = ui->spin_BuildAggression->value();
    aiNode.marketingAgression  = ui->spin_MarketingAggression->value();
    aiNode.costAgression = ui->spin_CostsAggression->value();
    aiNode.qualityAgression  = ui->spin_QualityAggression->value();
    aiNode.priceAgression = ui->spin_PriceAggression->value();
    aiNode.expansionAgression = ui->spin_ExpansionAggression->value();
    aiNode.clusting  = ui->spin_BuildingClustering->value();
    aiNode.exportDesigns  = ui->spin_DesignExport->value();
    aiNode.importDesigns = ui->spiner_ImportDesign->value();

    //Lets save or edit an existing company.
    saveOrEditAINode(aiNode);
}

//Check to see if we need to save a new company or edit an existing one.
//Then do it.
void AIEditor::saveOrEditAINode(AIManager::AIInfo aiNode)
{
    //Check to make sure that there is nothing that will break the game in this AI company.
    if(aiNode.name == "" || aiNode.name == " ")
    {
        QMessageBox::critical(this,"Error!","Company Name must not be blank!");
        return;
    }
    else if(aiNode.deathYear <= aiNode.startYear)
    {
        QMessageBox::critical(this,"Error!","Error starting year must be lower than death year!");
        return;
    }
    else if(aiNode.funds <= 0)
    {
        QMessageBox::critical(this,"Error!","Starting funds must be greater than 0!");
        return;
    }
    else if(aiNode.logo == "" || !aiNode.logo.contains(".dds"))
    {
        if(aiNode.logo == "")
        {
            QMessageBox::critical(this,"Error!","Logo File Must Not Be Blank!");
            return;
        }
        else
        {
            QMessageBox::critical(this,"Error!","Logo file must be in dds format,"
                                  "and the file name must have .dds extension!");
            return;
        }
    }

    //Check to make sure the company name doesn't already exist!
    if(!aiMap.contains(aiNode.id))
    {
        for(QMap<int,AIManager::AIInfo>::iterator it = aiMap.begin(); it != aiMap.end(); ++it)
        {
            if((*it).name == aiNode.name)
            {
                QMessageBox::critical(this,"Error!","This company name already exists!"
                                      "\nCompany names must be unique...");
                return;
            }
        }
    }


    //Warning messages. Not really errors, but something the user should be concerned about.
    //Due to poor GUI system, the game doesn't really support more than 20char lengths in listboxes.
    if(aiNode.name.size() > 20)
    {
        QMessageBox::critical(this,"Warning!","Company Names over 20 characters long may"
                              "have formatting issues!\n You may want to edit this name.");
    }


    //Logo isn't in the logo folder the user assigned.
    int index = ui->combo_LogoList->findText(aiNode.logo);
    if(index == -1 && ui->combo_LogoList->count() !=0)
        QMessageBox::critical(this,"Warning!","This Logo Is Not In The Folder!");

    //Selected city isn't in the city list the user has assigned.
    index = ui->combo_CityList->findText(getCityNameByID(aiNode.hq));
        if(index == -1 && ui->combo_CityList->count() !=0)
            QMessageBox::critical(this,"Warning!","This City is not in the city script!");

    //If this ID already exists in our company map, then just change the information, otherwise,
    //insert the new company into the map.
    if(aiMap.contains(aiNode.id))
    {
       aiMap.find(aiNode.id).value() = aiNode;
    }
    else
    {
       aiMap.insert(aiNode.id,aiNode);
       ui->Label_AIID->setText(QString::number(aiMap.size()+1));
       ui->spin_OwnerID->setValue(aiMap.size()+1);
       ui->lineEdit_AIName->setText("");
    }

    //clear and refill our table
    fillTableList();

}

void AIEditor::fillTableList()
{
    //allow the table list to be refilled
    refillList = true;

    //clear out the table
    ui->table_AICompanyList->clearContents();
    ui->table_AICompanyList->setRowCount(aiMap.size());


    //loop through our company map and fill up the table data.
    for(QMap<int,AIManager::AIInfo>::iterator it = aiMap.begin(); it != aiMap.end(); ++it)
    {
        //Only do this for numbered items so sorting works properly...
        QTableWidgetItem *itemID = new QTableWidgetItem;
        QTableWidgetItem *itemFound = new QTableWidgetItem;
        QTableWidgetItem *itemDeath = new QTableWidgetItem;
        QTableWidgetItem *itemFunds = new QTableWidgetItem;

        itemID->setData(Qt::EditRole,(*it).id);
        itemFound->setData(Qt::EditRole,(*it).startYear);
        itemDeath->setData(Qt::EditRole,(*it).deathYear);
        itemFunds->setData(Qt::EditRole,(*it).funds);


        //set the table item for this row
        ui->table_AICompanyList->setItem((*it).id-1,0,itemID);
        ui->table_AICompanyList->setItem((*it).id-1,1,new QTableWidgetItem((*it).name));
        ui->table_AICompanyList->setItem((*it).id-1,2,new QTableWidgetItem(
                                             getCityNameByID((*it).hq)));
        ui->table_AICompanyList->setItem((*it).id-1,3,new QTableWidgetItem(
                                             getExistingNameByID((*it).ownerID)));
        ui->table_AICompanyList->setItem((*it).id-1,4,itemFound);
        ui->table_AICompanyList->setItem((*it).id-1,5,itemDeath);
        ui->table_AICompanyList->setItem((*it).id-1,6,itemFunds);


      //add possible owner to the owner combo box.
          if(ui->combo_OwnerList->findText(getExistingNameByID((*it).ownerID)) == -1)
             ui->combo_OwnerList->addItem(getExistingNameByID((*it).ownerID));

    }

    //now that the table list is refilled, disable it again!
    refillList = false;

}

//Reset AI Information Widgets to Default settings.
void AIEditor::resetAINode()
{
        ui->Label_AIID->setText(QString::number(aiMap.size()+1));
        ui->lineEdit_AIName->setText("");
        ui->spin_OwnerID->setValue(aiMap.size()+1);
        ui->spin_HQID->setValue(1);
        ui->spin_FoundingYear->setValue(1900);
        ui->spin_DeathYear->setValue(1950);
        ui->line_AILogo->setText("");
        ui->line_StartingFunds->setText("1000000");
        ui->line_ExistingLoans->setText(0);
        ui->combo_CreditRating->setCurrentIndex(7);
        ui->spin_ManuSkills->setValue(20);
        ui->spin_RnDSkills->setValue(20);
        ui->spin_AdminSkills->setValue(20);
        ui->spin_MarketingSkills->setValue(20);
        ui->spin_DealershipSkills->setValue(20);
        ui->spin_EngineDesignSkills->setValue(20);
        ui->spin_ChassisDesignSkills->setValue(20);
        ui->spin_GearboxDesignSkills->setValue(20);
        ui->spin_BodyDesignSkills->setValue(20);
        ui->spin_LuxuryDesignSkills->setValue(20);
        ui->spin_SafetyDesignSkills->setValue(20);
        ui->spin_GlobalImage->setValue(20);
        ui->spin_QualityImage->setValue(20);
        ui->spin_RacingImage->setValue(20);
        ui->spin_WorkmanImage->setValue(20);
        ui->check_GenericDesign->setChecked(false);
        ui->spin_PerformanceBehave->setValue(0.5);
        ui->spin_DriveBehave->setValue(0.5);
        ui->spin_LuxuryBehave->setValue(0.5);
        ui->spin_SafetyBehave->setValue(0.5);
        ui->spin_FuelBehave->setValue(0.5);
        ui->spin_PowereBehave->setValue(0.5);
        ui->spin_CargoBehave->setValue(0.5);
        ui->spin_DependBehave->setValue(0.5);
        ui->spin_DesignAgression->setValue(0.5);
        ui->spin_SalesAggression->setValue(0.5);
        ui->spin_BuildAggression->setValue(0.5);
        ui->spin_MarketingAggression->setValue(0.5);
        ui->spin_CostsAggression->setValue(0.5);
        ui->spin_QualityAggression->setValue(0.5);
        ui->spin_PriceAggression->setValue(0.5);
        ui->spin_ExpansionAggression->setValue(0.5);
        ui->spin_BuildingClustering->setValue(0.5);
        ui->spin_DesignExport->setValue(0.5);
        ui->spiner_ImportDesign->setValue(0.5);
       ui->combo_CityList->setCurrentIndex(1);
       ui->combo_QuickAgression->setCurrentIndex(0);
       ui->combo_QuickDesignBehavior->setCurrentIndex(0);
       ui->combo_QuickDesignSkills->setCurrentIndex(3);
       ui->combo_QuickFinances->setCurrentIndex(3);
       ui->combo_QuickImage->setCurrentIndex(4);
       ui->combo_QuickSkills->setCurrentIndex(3);

       if(ui->combo_OwnerList->findText("Self Owned") == -1)
            ui->combo_OwnerList->addItem("Self Owned");
}


//Get Company ID, return a name if it exists, the ID if not.
QString AIEditor::getExistingNameByID(int id)
{
    QString name = QString::number(id);
    QMap<int,AIManager::AIInfo>::iterator it = aiMap.find(id);
    if(it != aiMap.end())
       name = (*it).name;

    return name;
}

//Get Company Name, return company ID or -1 if it does not exist
int AIEditor::getExistingIDByName(QString name)
{
    int id = -1;
    for(QMap<int,AIManager::AIInfo>::iterator it = aiMap.begin(); it != aiMap.end(); ++it)
    {
        if(name == (*it).name)
        {
            id = (*it).id;
            break;
        }
    }
    return id;
}

//Create New AI file
void AIEditor::on_Button_makeNewAIList_clicked()
{
    //Clear out maps and lists, clear combos, reset the tables.
    aiMap.clear();
    cityMap.clear();

    ui->combo_CityList->clear();
    ui->combo_LogoList->clear();
    ui->combo_OwnerList->clear();
    ui->label_LogoFolderHelper->setText("No Logo Folder Selected.");
    ui->label_CityHelper->setText("No City Script Selected.");

    resetAINode();
    fillTableList();
    saveFileName = "";
    ui->label_FileSaveName->setText("File has not been saved yet.");

}

//Clicked AI Table, fill the information widgets.
void AIEditor::on_table_AICompanyList_clicked(const QModelIndex &index)
{
    //Get the selected row, then use that to get the selected ID, get the iterator
    //from the map and then fill the AI Info widgets on the wide with the data.
    int selectedRow = index.row();
    int selectedID = ui->table_AICompanyList->item(selectedRow,0)->text().toInt();
    QMap<int,AIManager::AIInfo>::iterator mapIT = aiMap.find(selectedID);

    ui->Label_AIID->setText(QString::number((*mapIT).id));
    ui->lineEdit_AIName->setText((*mapIT).name);
    ui->spin_OwnerID->setValue((*mapIT).ownerID);
    ui->spin_HQID->setValue((*mapIT).hq);
    ui->spin_FoundingYear->setValue((*mapIT).startYear);
    ui->spin_DeathYear->setValue((*mapIT).deathYear);
    ui->line_AILogo->setText((*mapIT).logo);
    ui->line_StartingFunds->setText(QString::number((*mapIT).funds));
    ui->line_ExistingLoans->setText(QString::number((*mapIT).loan));
    ui->combo_CreditRating->setCurrentIndex((*mapIT).credit);
    ui->spin_ManuSkills->setValue((*mapIT).manuSkills);
    ui->spin_RnDSkills->setValue((*mapIT).rndSkills);
    ui->spin_AdminSkills->setValue((*mapIT).adminSkills);
    ui->spin_MarketingSkills->setValue((*mapIT).marketingSkills);
    ui->spin_DealershipSkills->setValue((*mapIT).dealerSkills);
    ui->spin_EngineDesignSkills->setValue((*mapIT).engineDesign);
    ui->spin_ChassisDesignSkills->setValue((*mapIT).chassisDesign);
    ui->spin_GearboxDesignSkills->setValue((*mapIT).gearboxDesign);
    ui->spin_BodyDesignSkills->setValue((*mapIT).bodyDesign);
    ui->spin_LuxuryDesignSkills->setValue((*mapIT).luxDesign);
    ui->spin_SafetyDesignSkills->setValue((*mapIT).safeDesign);
    ui->spin_GlobalImage->setValue((*mapIT).globalImage);
    ui->spin_QualityImage->setValue((*mapIT).qualityImage);
    ui->spin_RacingImage->setValue((*mapIT).racingImage);
    ui->spin_WorkmanImage->setValue((*mapIT).workImage);
    ui->check_GenericDesign->setChecked((*mapIT).genericDesigner);
    ui->spin_PerformanceBehave->setValue((*mapIT).performanceBehavoir);
    ui->spin_DriveBehave->setValue((*mapIT).drivingBehavoir);
    ui->spin_LuxuryBehave->setValue((*mapIT).luxuryBehavoir);
    ui->spin_SafetyBehave->setValue((*mapIT).safetyBehavoir);
    ui->spin_FuelBehave->setValue((*mapIT).fuelBehavoir);
    ui->spin_PowereBehave->setValue((*mapIT).powerBehavoir);
    ui->spin_CargoBehave->setValue((*mapIT).cargoBehavoir);
    ui->spin_DependBehave->setValue((*mapIT).dependabilityBehavoir);
    ui->spin_DesignAgression->setValue((*mapIT).designAgression);
    ui->spin_SalesAggression->setValue((*mapIT).sellAgression);
    ui->spin_BuildAggression->setValue((*mapIT).buildAgression);
    ui->spin_MarketingAggression->setValue((*mapIT).marketingAgression);
    ui->spin_CostsAggression->setValue((*mapIT).costAgression);
    ui->spin_QualityAggression->setValue((*mapIT).qualityAgression);
    ui->spin_PriceAggression->setValue((*mapIT).priceAgression);
    ui->spin_ExpansionAggression->setValue((*mapIT).expansionAgression);
    ui->spin_BuildingClustering->setValue((*mapIT).clusting);
    ui->spin_DesignExport->setValue((*mapIT).exportDesigns);
    ui->spiner_ImportDesign->setValue((*mapIT).importDesigns);
}


//Save button clicked, get file name.
void AIEditor::on_button_SaveAIList_clicked()
{   
    saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");

    ui->label_FileSaveName->setText(saveFileName);

    //Call function to actually save.
    saveXML();
}

//Save the AI map to the save name file.
void AIEditor::saveXML()
{
    //Open/create AI file
    QFile saveAIFile(saveFileName);
    if(!saveAIFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(saveFileName,
                             saveAIFile.errorString()));
       return;
    }


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveAIFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("AINode");

    //Loop through and write the data to file xml.
    for(QMap<int,AIManager::AIInfo>::iterator it = aiMap.begin(); it != aiMap.end(); ++it)
    {
        xmlWriter.writeStartElement("Company");
               xmlWriter.writeAttribute("ID",QString::number((*it).id));
               xmlWriter.writeAttribute("Name",(*it).name);
               if((*it).id == (*it).ownerID)
                 xmlWriter.writeAttribute("Active","1");
               else
                 xmlWriter.writeAttribute("Active","0");

               xmlWriter.writeAttribute("OwnerID",QString::number((*it).ownerID));
               xmlWriter.writeAttribute("HQ",QString::number((*it).hq));
               xmlWriter.writeAttribute("Founded",QString::number((*it).startYear));
               xmlWriter.writeAttribute("Death",QString::number((*it).deathYear));
               xmlWriter.writeAttribute("Logo",(*it).logo);


            xmlWriter.writeStartElement("Funds");
               xmlWriter.writeAttribute("OnHand",QString::number((*it).funds));
               xmlWriter.writeAttribute("Credit",QString::number((*it).credit));
               xmlWriter.writeAttribute("Loans",QString::number((*it).loan));
            xmlWriter.writeEndElement(); //Funds

            xmlWriter.writeStartElement("Skills");
               xmlWriter.writeAttribute("Manufactoring",QString::number((*it).manuSkills));
               xmlWriter.writeAttribute("RnD",QString::number((*it).rndSkills));
               xmlWriter.writeAttribute("Admin",QString::number((*it).adminSkills));
               xmlWriter.writeAttribute("Marketing",QString::number((*it).marketingSkills));
               xmlWriter.writeAttribute("Dealers",QString::number((*it).dealerSkills));
            xmlWriter.writeEndElement(); //Skills

            xmlWriter.writeStartElement("Design");
                xmlWriter.writeAttribute("Engine",QString::number((*it).engineDesign));
                xmlWriter.writeAttribute("Chassis",QString::number((*it).chassisDesign));
                xmlWriter.writeAttribute("Transmission",QString::number((*it).gearboxDesign));
                xmlWriter.writeAttribute("Body",QString::number((*it).bodyDesign));
                xmlWriter.writeAttribute("Lux",QString::number((*it).luxDesign));
                xmlWriter.writeAttribute("safety",QString::number((*it).safeDesign));
            xmlWriter.writeEndElement(); //Design

            xmlWriter.writeStartElement("Image");
                xmlWriter.writeAttribute("GeneralGlobal",QString::number((*it).globalImage));
                xmlWriter.writeAttribute("Quality",QString::number((*it).qualityImage));
                xmlWriter.writeAttribute("Racing",QString::number((*it).racingImage));
                xmlWriter.writeAttribute("Work",QString::number((*it).workImage));
            xmlWriter.writeEndElement(); //Image

            xmlWriter.writeStartElement("Behavior");
                xmlWriter.writeAttribute("GenericDesigner",QString::number((*it).genericDesigner));
                xmlWriter.writeAttribute("Rating_Performance",QString::number((*it).performanceBehavoir));
                xmlWriter.writeAttribute("Rating_Drivability",QString::number((*it).drivingBehavoir));
                xmlWriter.writeAttribute("Rating_Luxury",QString::number((*it).luxuryBehavoir));
                xmlWriter.writeAttribute("Rating_Safety",QString::number((*it).safetyBehavoir));
                xmlWriter.writeAttribute("Rating_Fuel",QString::number((*it).fuelBehavoir));
                xmlWriter.writeAttribute("Rating_Power",QString::number((*it).powerBehavoir));
                xmlWriter.writeAttribute("Rating_Cargo",QString::number((*it).cargoBehavoir));
                xmlWriter.writeAttribute("Rating_Dependability",QString::number((*it).dependabilityBehavoir));
                xmlWriter.writeAttribute("DesignAggression",QString::number((*it).designAgression));
                xmlWriter.writeAttribute("SellAggression",QString::number((*it).sellAgression));
                xmlWriter.writeAttribute("BuildAggression",QString::number((*it).buildAgression));
                xmlWriter.writeAttribute("MarketingAggression",QString::number((*it).marketingAgression));
                xmlWriter.writeAttribute("CostAggression",QString::number((*it).costAgression));
                xmlWriter.writeAttribute("QualityAggression",QString::number((*it).qualityAgression));
                xmlWriter.writeAttribute("PriceAggression",QString::number((*it).priceAgression));
                xmlWriter.writeAttribute("ExpansionAggression",QString::number((*it).expansionAgression));
                xmlWriter.writeAttribute("ClusterSpace",QString::number((*it).clusting));
                xmlWriter.writeAttribute("ExportDesigns",QString::number((*it).exportDesigns));
                xmlWriter.writeAttribute("ImportDesigns",QString::number((*it).importDesigns));
            xmlWriter.writeEndElement(); //Behavior

         xmlWriter.writeEndElement(); //Company

    }

    //close the final tag and close the file.
     xmlWriter.writeEndDocument();

     saveAIFile.close();

     //We Did it! Yay!
     QMessageBox::information(this,"Save Complete",
                              "The AI File has been saved!", QMessageBox::Ok);


}

//Open AI file button, select AI File
void AIEditor::on_button_OpenAIList_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","XML Files (*.xml)");

    if (openFileName != "")
    {
        openAIScript(openFileName);
        fillTableList();
        ui->label_FileSaveName->setText(openFileName);
        saveFileName = openFileName;
    }
    else
    {
        return;
    }
}

//Actual opener for AI script, uses AI Manager object
void AIEditor::openAIScript(QString fileName)
{
    //Clear AIMap, Generate object for AIManager then use that
    //Manager to get the aiMap.
    aiMap.clear();
    AIManager aim = AIManager(fileName,cp_wsc.AIEditorCW);
    aiMap = aim.getMap();
}

//Open City Script to fill Combo boxes
void AIEditor::on_button_LoadCityScript_clicked()
{
    //Open City Script File
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","XML Files (*.xml)");

    if (openFileName != "")
    {
        openCityXML(openFileName);

        //Clear city combo, loop through the citymap and fill up the combo box.
        ui->combo_CityList->clear();
        for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
        {
            ui->combo_CityList->addItem((*it).cityName +", "+(*it).cityCountry);
        }

        ui->label_CityHelper->setText(openFileName);

        //Refresh the table list.
        fillTableList();
    }
    else
    {
        return;
    }
}

//Create CityData object and fill up the citymap
void AIEditor::openCityXML(QString openFileName)
{
    cityMap.clear();
    CityData cd = CityData(openFileName, cp_wsc.AIEditorCW);
    cityMap = cd.returnCopyOfDSMap();
}


//Get a city name, return a city ID
QString AIEditor::getCityNameByID(int ID)
{
    QString name = QString::number(ID);
    if(cityMap.contains(ID))
    {
        QMap<int, CityData::dataStore>::iterator it = cityMap.find(ID);
        name = (*it).cityName + ", "+ (*it).cityCountry;
    }

    return name;
}

//Get city ID, return its name.
int AIEditor::getCityIDByName(QString name)
{
    int id = -1;

    for(QMap<int,CityData::dataStore>::iterator it = cityMap.begin(); it != cityMap.end(); ++it)
    {
        if(((*it).cityName+", "+(*it).cityCountry) == name)
        {
           id = (*it).id;
           break;
        }
    }
    return id;
}

//Select and parse a folder for .dds files to be used in logos.
//Then fill the logo combobox with these files for the user to select
void AIEditor::on_button_LoadLogoFolder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Logo Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.dds");
    QStringList fileList = dir.entryList();

    ui->combo_LogoList->clear();
    ui->combo_LogoList->addItems(fileList);
    ui->label_LogoFolderHelper->setText(folderPath);
}

//Select an owner from combo box and set the spinner owner ID
void AIEditor::on_combo_OwnerList_currentIndexChanged(const QString &arg1)
{
    if(refillList)
        return;

    ui->spin_OwnerID->setValue(getExistingIDByName(arg1));
}


//Change in Spinner ID changes the selected combo if the company exists in the list.
void AIEditor::on_spin_OwnerID_valueChanged(int arg1)
{
    if(refillList)
        return;

    QString name = getExistingNameByID(arg1);
    int index = ui->combo_OwnerList->findText(name);

    if(index != -1)
        ui->combo_OwnerList->setCurrentIndex(index);
}

//HQ Spinner changes HQ City combo box if the spinner is spun.
void AIEditor::on_spin_HQID_valueChanged(int arg1)
{
    if(refillList)
        return;

    QString name = getCityNameByID(arg1);
    int index = ui->combo_CityList->findText(name);

    if(index != -1)
        ui->combo_CityList->setCurrentIndex(index);
}

//Selected a city from combo box, adjust the spinner.
void AIEditor::on_combo_CityList_currentIndexChanged(const QString &arg1)
{
    if(refillList)
        return;

   ui->spin_HQID->setValue(getCityIDByName(arg1));
}

//Selected a logo from the combo box, set the logo line box text.
void AIEditor::on_combo_LogoList_currentIndexChanged(const QString &arg1)
{
    if(refillList)
        return;

    ui->line_AILogo->setText(arg1);
}

//Used to sort the columns in the table.
void AIEditor::sectionDoubleClickedSlot(int index)
{
     ui->table_AICompanyList->sortByColumn(index,Qt::AscendingOrder);
}

//Quickly change financial values from drop down box
void AIEditor::on_combo_QuickFinances_currentIndexChanged(int index)
{
    int startYear = ui->spin_FoundingYear->value()-1850;
    long long startingFunds = 0;
    long long loan = 0;
    int credit = 9;

    switch(index)
    {
        case 0: //Giant
            startingFunds = 500000 * pow(1.05,startYear);
            credit = 9;
            break;
        case 1: //Huge
            startingFunds = 400000 * pow(1.05,startYear);
            credit = 9;
            break;
        case 2: //Large
            startingFunds = 300000 * pow(1.05,startYear);
            credit = 9;
            break;
        case 3: //Medium
            startingFunds = 200000 * pow(1.045,startYear);
            credit = 8;
            break;
        case 4: //Small
            startingFunds = 100000 * pow(1.045,startYear);
            credit = 7;
            break;
        case 5: //Tiny
            startingFunds = 100000 * pow(1.04,startYear);
            credit = 7;
            break;
        case 6://Huge In Debt
            startingFunds = 400000 * pow(1.05,startYear);
            credit = 7;
            loan = startingFunds * 0.8;
            break;
        case 7://Medium In Debt
            startingFunds = 200000 * pow(1.045,startYear);
            credit = 6;
            loan = startingFunds * 0.7;
            break;
        case 8://Tiny In Debt
            startingFunds = 100000 * pow(1.04,startYear);
            credit = 5;
            loan = startingFunds * 0.6;
            break;

    };


    ui->line_StartingFunds->setText(QString::number(startingFunds));
    ui->line_ExistingLoans->setText(QString::number(loan));
    ui->combo_CreditRating->setCurrentIndex(credit);


}

//Quickly change skill values from drop down box
void AIEditor::on_combo_QuickSkills_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0: //Superior
            ui->spin_ManuSkills->setValue(90);
            ui->spin_RnDSkills->setValue(90);
            ui->spin_AdminSkills->setValue(90);
            ui->spin_MarketingSkills->setValue(90);
            ui->spin_DealershipSkills->setValue(90);
            break;
        case 1: //Extremely Advanced
            ui->spin_ManuSkills->setValue(75);
            ui->spin_RnDSkills->setValue(75);
            ui->spin_AdminSkills->setValue(75);
            ui->spin_MarketingSkills->setValue(75);
            ui->spin_DealershipSkills->setValue(75);
            break;
        case 2: //Advanced
            ui->spin_ManuSkills->setValue(60);
            ui->spin_RnDSkills->setValue(60);
            ui->spin_AdminSkills->setValue(60);
            ui->spin_MarketingSkills->setValue(60);
            ui->spin_DealershipSkills->setValue(60);
            break;
        case 3: //Average
            ui->spin_ManuSkills->setValue(45);
            ui->spin_RnDSkills->setValue(45);
            ui->spin_AdminSkills->setValue(45);
            ui->spin_MarketingSkills->setValue(45);
            ui->spin_DealershipSkills->setValue(45);
            break;
        case 4: //Slightly Behind
            ui->spin_ManuSkills->setValue(35);
            ui->spin_RnDSkills->setValue(35);
            ui->spin_AdminSkills->setValue(35);
            ui->spin_MarketingSkills->setValue(35);
            ui->spin_DealershipSkills->setValue(35);
            break;
        case 5: //Talentless
            ui->spin_ManuSkills->setValue(20);
            ui->spin_RnDSkills->setValue(20);
            ui->spin_AdminSkills->setValue(20);
            ui->spin_MarketingSkills->setValue(20);
            ui->spin_DealershipSkills->setValue(20);
            break;
        case 6: //Startup
            ui->spin_ManuSkills->setValue(10);
            ui->spin_RnDSkills->setValue(10);
            ui->spin_AdminSkills->setValue(10);
            ui->spin_MarketingSkills->setValue(10);
            ui->spin_DealershipSkills->setValue(10);
            break;
        case 7: //Bored 12yr kids
            ui->spin_ManuSkills->setValue(1);
            ui->spin_RnDSkills->setValue(1);
            ui->spin_AdminSkills->setValue(1);
            ui->spin_MarketingSkills->setValue(1);
            ui->spin_DealershipSkills->setValue(1);
            break;
    };
}

//Quickly change design skills values from drop down box
void AIEditor::on_combo_QuickDesignSkills_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0://Superior
          ui->spin_EngineDesignSkills->setValue(90);
          ui->spin_ChassisDesignSkills->setValue(90);
          ui->spin_GearboxDesignSkills->setValue(90);
          ui->spin_BodyDesignSkills->setValue(90);
          ui->spin_LuxuryDesignSkills->setValue(90);
          ui->spin_SafetyDesignSkills->setValue(90);
          break;
        case 1://Extremely Advanced
            ui->spin_EngineDesignSkills->setValue(75);
            ui->spin_ChassisDesignSkills->setValue(75);
            ui->spin_GearboxDesignSkills->setValue(75);
            ui->spin_BodyDesignSkills->setValue(75);
            ui->spin_LuxuryDesignSkills->setValue(75);
            ui->spin_SafetyDesignSkills->setValue(75);
          break;
        case 2://Advanced
            ui->spin_EngineDesignSkills->setValue(60);
            ui->spin_ChassisDesignSkills->setValue(60);
            ui->spin_GearboxDesignSkills->setValue(60);
            ui->spin_BodyDesignSkills->setValue(60);
            ui->spin_LuxuryDesignSkills->setValue(60);
            ui->spin_SafetyDesignSkills->setValue(60);
          break;
        case 3://Average
            ui->spin_EngineDesignSkills->setValue(45);
            ui->spin_ChassisDesignSkills->setValue(45);
            ui->spin_GearboxDesignSkills->setValue(45);
            ui->spin_BodyDesignSkills->setValue(45);
            ui->spin_LuxuryDesignSkills->setValue(45);
            ui->spin_SafetyDesignSkills->setValue(45);
          break;
        case 4://Below Average
            ui->spin_EngineDesignSkills->setValue(30);
            ui->spin_ChassisDesignSkills->setValue(30);
            ui->spin_GearboxDesignSkills->setValue(30);
            ui->spin_BodyDesignSkills->setValue(30);
            ui->spin_LuxuryDesignSkills->setValue(30);
            ui->spin_SafetyDesignSkills->setValue(30);
          break;
        case 5://Slightly Behind
            ui->spin_EngineDesignSkills->setValue(20);
            ui->spin_ChassisDesignSkills->setValue(20);
            ui->spin_GearboxDesignSkills->setValue(20);
            ui->spin_BodyDesignSkills->setValue(20);
            ui->spin_LuxuryDesignSkills->setValue(20);
            ui->spin_SafetyDesignSkills->setValue(20);
          break;
        case 6://Behind
            ui->spin_EngineDesignSkills->setValue(10);
            ui->spin_ChassisDesignSkills->setValue(10);
            ui->spin_GearboxDesignSkills->setValue(10);
            ui->spin_BodyDesignSkills->setValue(10);
            ui->spin_LuxuryDesignSkills->setValue(10);
            ui->spin_SafetyDesignSkills->setValue(10);
          break;
        case 7://VENT
            ui->spin_EngineDesignSkills->setValue(5);
            ui->spin_ChassisDesignSkills->setValue(5);
            ui->spin_GearboxDesignSkills->setValue(5);
            ui->spin_BodyDesignSkills->setValue(5);
            ui->spin_LuxuryDesignSkills->setValue(5);
            ui->spin_SafetyDesignSkills->setValue(5);
          break;
        case 8://Digital Homicide Skills
            ui->spin_EngineDesignSkills->setValue(1);
            ui->spin_ChassisDesignSkills->setValue(1);
            ui->spin_GearboxDesignSkills->setValue(1);
            ui->spin_BodyDesignSkills->setValue(1);
            ui->spin_LuxuryDesignSkills->setValue(1);
            ui->spin_SafetyDesignSkills->setValue(1);
          break;
    };
}


//Quickly change image values from drop down box
void AIEditor::on_combo_QuickImage_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0://Adored
            ui->spin_GlobalImage->setValue(90);
            ui->spin_QualityImage->setValue(90);
            ui->spin_RacingImage->setValue(90);
            ui->spin_WorkmanImage->setValue(90);
             break;
        case 1://Loved
            ui->spin_GlobalImage->setValue(75);
            ui->spin_QualityImage->setValue(75);
            ui->spin_RacingImage->setValue(75);
            ui->spin_WorkmanImage->setValue(75);
            break;
        case 2://Pleased
            ui->spin_GlobalImage->setValue(50);
            ui->spin_QualityImage->setValue(50);
            ui->spin_RacingImage->setValue(50);
            ui->spin_WorkmanImage->setValue(50);
            break;
        case 3://Neutral
            ui->spin_GlobalImage->setValue(40);
            ui->spin_QualityImage->setValue(40);
            ui->spin_RacingImage->setValue(40);
            ui->spin_WorkmanImage->setValue(40);
            break;
        case 4://Unknown
            ui->spin_GlobalImage->setValue(25);
            ui->spin_QualityImage->setValue(25);
            ui->spin_RacingImage->setValue(25);
            ui->spin_WorkmanImage->setValue(25);
            break;
        case 5://Hated
            ui->spin_GlobalImage->setValue(10);
            ui->spin_QualityImage->setValue(10);
            ui->spin_RacingImage->setValue(10);
            ui->spin_WorkmanImage->setValue(10);
            break;
        case 6://Scum
            ui->spin_GlobalImage->setValue(1);
            ui->spin_QualityImage->setValue(1);
            ui->spin_RacingImage->setValue(1);
            ui->spin_WorkmanImage->setValue(1);
            break;

    };
}

//Quickly change design behavior values from drop down box
void AIEditor::on_combo_QuickDesignBehavior_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0: //Generic Manufacturer
            ui->check_GenericDesign->setChecked(true);
            ui->spin_PerformanceBehave->setValue(0.35);
            ui->spin_DriveBehave->setValue(0.35);
            ui->spin_LuxuryBehave->setValue(0.35);
            ui->spin_SafetyBehave->setValue(0.35);
            ui->spin_FuelBehave->setValue(0.35);
            ui->spin_PowereBehave->setValue(0.35);
            ui->spin_CargoBehave->setValue(0.35);
            ui->spin_DependBehave->setValue(0.35);
            break;
        case 1: //Upper Class Generic
            ui->check_GenericDesign->setChecked(true);
            ui->spin_PerformanceBehave->setValue(0.5);
            ui->spin_DriveBehave->setValue(0.5);
            ui->spin_LuxuryBehave->setValue(0.8);
            ui->spin_SafetyBehave->setValue(0.8);
            ui->spin_FuelBehave->setValue(0.2);
            ui->spin_PowereBehave->setValue(0.7);
            ui->spin_CargoBehave->setValue(0.6);
            ui->spin_DependBehave->setValue(0.7);
            break;
        case 2: //Lower Class Generic
            ui->check_GenericDesign->setChecked(true);
            ui->spin_PerformanceBehave->setValue(0.1);
            ui->spin_DriveBehave->setValue(0.1);
            ui->spin_LuxuryBehave->setValue(0.1);
            ui->spin_SafetyBehave->setValue(0.3);
            ui->spin_FuelBehave->setValue(0.7);
            ui->spin_PowereBehave->setValue(0.2);
            ui->spin_CargoBehave->setValue(0.3);
            ui->spin_DependBehave->setValue(0.4);
            break;
        case 3: //Performance Generic
            ui->check_GenericDesign->setChecked(true);
            ui->spin_PerformanceBehave->setValue(0.8);
            ui->spin_DriveBehave->setValue(0.6);
            ui->spin_LuxuryBehave->setValue(0.3);
            ui->spin_SafetyBehave->setValue(0.3);
            ui->spin_FuelBehave->setValue(0.2);
            ui->spin_PowereBehave->setValue(0.8);
            ui->spin_CargoBehave->setValue(0.3);
            ui->spin_DependBehave->setValue(0.3);
            break;

        case 4: //Truck Company
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.5);
            ui->spin_DriveBehave->setValue(0.1);
            ui->spin_LuxuryBehave->setValue(0.1);
            ui->spin_SafetyBehave->setValue(0.2);
            ui->spin_FuelBehave->setValue(0.2);
            ui->spin_PowereBehave->setValue(1.0);
            ui->spin_CargoBehave->setValue(1.0);
            ui->spin_DependBehave->setValue(0.7);
            break;

        case 5: //Performance Company
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.7);
            ui->spin_DriveBehave->setValue(0.7);
            ui->spin_LuxuryBehave->setValue(0.3);
            ui->spin_SafetyBehave->setValue(0.3);
            ui->spin_FuelBehave->setValue(0.2);
            ui->spin_PowereBehave->setValue(0.6);
            ui->spin_CargoBehave->setValue(0.3);
            ui->spin_DependBehave->setValue(0.5);
            break;

        case 6: //Ultra Rich High Performance
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(1.0);
            ui->spin_DriveBehave->setValue(0.8);
            ui->spin_LuxuryBehave->setValue(0.3);
            ui->spin_SafetyBehave->setValue(0.1);
            ui->spin_FuelBehave->setValue(0.1);
            ui->spin_PowereBehave->setValue(1.0);
            ui->spin_CargoBehave->setValue(0.1);
            ui->spin_DependBehave->setValue(0.1);
            break;

        case 7: //Fun Handling
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.5);
            ui->spin_DriveBehave->setValue(1.0);
            ui->spin_LuxuryBehave->setValue(0.1);
            ui->spin_SafetyBehave->setValue(0.1);
            ui->spin_FuelBehave->setValue(0.3);
            ui->spin_PowereBehave->setValue(0.3);
            ui->spin_CargoBehave->setValue(0.1);
            ui->spin_DependBehave->setValue(0.2);
            break;

        case 8: //Luxury Company
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.4);
            ui->spin_DriveBehave->setValue(0.4);
            ui->spin_LuxuryBehave->setValue(1.0);
            ui->spin_SafetyBehave->setValue(0.8);
            ui->spin_FuelBehave->setValue(0.3);
            ui->spin_PowereBehave->setValue(0.6);
            ui->spin_CargoBehave->setValue(0.7);
            ui->spin_DependBehave->setValue(0.7);
            break;

        case 9: //Economic Company
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.3);
            ui->spin_DriveBehave->setValue(0.3);
            ui->spin_LuxuryBehave->setValue(0.1);
            ui->spin_SafetyBehave->setValue(0.4);
            ui->spin_FuelBehave->setValue(0.8);
            ui->spin_PowereBehave->setValue(0.3);
            ui->spin_CargoBehave->setValue(0.4);
            ui->spin_DependBehave->setValue(0.3);
            break;

        case 10: //Micro Car Company
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.05);
            ui->spin_DriveBehave->setValue(0.1);
            ui->spin_LuxuryBehave->setValue(0.05);
            ui->spin_SafetyBehave->setValue(0.05);
            ui->spin_FuelBehave->setValue(0.8);
            ui->spin_PowereBehave->setValue(0.05);
            ui->spin_CargoBehave->setValue(0.05);
            ui->spin_DependBehave->setValue(0.5);
            break;

        case 11: //Safety First
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.2);
            ui->spin_DriveBehave->setValue(0.6);
            ui->spin_LuxuryBehave->setValue(0.4);
            ui->spin_SafetyBehave->setValue(1.0);
            ui->spin_FuelBehave->setValue(0.3);
            ui->spin_PowereBehave->setValue(0.3);
            ui->spin_CargoBehave->setValue(0.6);
            ui->spin_DependBehave->setValue(0.8);
            break;

        case 12: //Green
            ui->check_GenericDesign->setChecked(false);
            ui->spin_PerformanceBehave->setValue(0.1);
            ui->spin_DriveBehave->setValue(0.4);
            ui->spin_LuxuryBehave->setValue(0.4);
            ui->spin_SafetyBehave->setValue(0.6);
            ui->spin_FuelBehave->setValue(1.0);
            ui->spin_PowereBehave->setValue(0.1);
            ui->spin_CargoBehave->setValue(0.5);
            ui->spin_DependBehave->setValue(0.5);
            break;
    };




}

//Quickly change company aggressional values from drop down box
void AIEditor::on_combo_QuickAgression_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0://Passive
            ui->spin_DesignAgression->setValue(0.2);
            ui->spin_SalesAggression->setValue(0.25);
            ui->spin_BuildAggression->setValue(0.2);
            ui->spin_MarketingAggression->setValue(0.2);
            ui->spin_CostsAggression->setValue(0.4);
            ui->spin_QualityAggression->setValue(0.4);
            ui->spin_PriceAggression->setValue(0.2);
            ui->spin_ExpansionAggression->setValue(0.2);
            ui->spin_BuildingClustering->setValue(0.5);
            ui->spin_DesignExport->setValue(0.5);
            ui->spiner_ImportDesign->setValue(0.5);
            break;
        case 1://Average
            ui->spin_DesignAgression->setValue(0.5);
            ui->spin_SalesAggression->setValue(0.5);
            ui->spin_BuildAggression->setValue(0.5);
            ui->spin_MarketingAggression->setValue(0.5);
            ui->spin_CostsAggression->setValue(0.5);
            ui->spin_QualityAggression->setValue(0.5);
            ui->spin_PriceAggression->setValue(0.5);
            ui->spin_ExpansionAggression->setValue(0.5);
            ui->spin_BuildingClustering->setValue(0.4);
            ui->spin_DesignExport->setValue(0.5);
            ui->spiner_ImportDesign->setValue(0.5);
            break;
        case 2://Aggressive
            ui->spin_DesignAgression->setValue(0.8);
            ui->spin_SalesAggression->setValue(0.8);
            ui->spin_BuildAggression->setValue(0.8);
            ui->spin_MarketingAggression->setValue(0.8);
            ui->spin_CostsAggression->setValue(0.3);
            ui->spin_QualityAggression->setValue(0.3);
            ui->spin_PriceAggression->setValue(0.8);
            ui->spin_ExpansionAggression->setValue(0.8);
            ui->spin_BuildingClustering->setValue(0.3);
            ui->spin_DesignExport->setValue(0.2);
            ui->spiner_ImportDesign->setValue(0.8);
        case 3://Aggressive Expansion
            ui->spin_DesignAgression->setValue(0.4);
            ui->spin_SalesAggression->setValue(0.6);
            ui->spin_BuildAggression->setValue(0.8);
            ui->spin_MarketingAggression->setValue(0.4);
            ui->spin_CostsAggression->setValue(0.3);
            ui->spin_QualityAggression->setValue(0.3);
            ui->spin_PriceAggression->setValue(0.3);
            ui->spin_ExpansionAggression->setValue(1.0);
            ui->spin_BuildingClustering->setValue(0.1);
            ui->spin_DesignExport->setValue(0.3);
            ui->spiner_ImportDesign->setValue(0.3);
            break;
        case 4://Aggressive Costs
            ui->spin_DesignAgression->setValue(0.2);
            ui->spin_SalesAggression->setValue(0.4);
            ui->spin_BuildAggression->setValue(0.3);
            ui->spin_MarketingAggression->setValue(0.4);
            ui->spin_CostsAggression->setValue(0.9);
            ui->spin_QualityAggression->setValue(0.3);
            ui->spin_PriceAggression->setValue(0.6);
            ui->spin_ExpansionAggression->setValue(0.3);
            ui->spin_BuildingClustering->setValue(0.5);
            ui->spin_DesignExport->setValue(0.7);
            ui->spiner_ImportDesign->setValue(0.1);
            break;
        case 5://Best Quality
            ui->spin_DesignAgression->setValue(0.6);
            ui->spin_SalesAggression->setValue(0.3);
            ui->spin_BuildAggression->setValue(0.3);
            ui->spin_MarketingAggression->setValue(0.3);
            ui->spin_CostsAggression->setValue(0.1);
            ui->spin_QualityAggression->setValue(0.9);
            ui->spin_PriceAggression->setValue(0.6);
            ui->spin_ExpansionAggression->setValue(0.3);
            ui->spin_BuildingClustering->setValue(0.3);
            ui->spin_DesignExport->setValue(0.4);
            ui->spiner_ImportDesign->setValue(0.05);
            break;
        case 6://Look for Profits
            ui->spin_DesignAgression->setValue(0.4);
            ui->spin_SalesAggression->setValue(0.7);
            ui->spin_BuildAggression->setValue(0.3);
            ui->spin_MarketingAggression->setValue(0.6);
            ui->spin_CostsAggression->setValue(0.5);
            ui->spin_QualityAggression->setValue(0.4);
            ui->spin_PriceAggression->setValue(0.8);
            ui->spin_ExpansionAggression->setValue(0.4);
            ui->spin_BuildingClustering->setValue(0.05);
            ui->spin_DesignExport->setValue(0.8);
            ui->spiner_ImportDesign->setValue(0.3);
            break;
        case 7://Close to Home
            ui->spin_DesignAgression->setValue(0.4);
            ui->spin_SalesAggression->setValue(0.4);
            ui->spin_BuildAggression->setValue(0.6);
            ui->spin_MarketingAggression->setValue(0.3);
            ui->spin_CostsAggression->setValue(0.4);
            ui->spin_QualityAggression->setValue(0.3);
            ui->spin_PriceAggression->setValue(0.4);
            ui->spin_ExpansionAggression->setValue(0.7);
            ui->spin_BuildingClustering->setValue(0.9);
            ui->spin_DesignExport->setValue(0.4);
            ui->spiner_ImportDesign->setValue(0.4);
            break;
    };


}

//Create a new AI company
void AIEditor::on_button_NewAICompany_clicked()
{
   resetAINode();
   int index = ui->combo_OwnerList->findText("Self Owned");

   if(index != -1)
     ui->combo_OwnerList->setCurrentIndex(index);
}


//Remove selected AI from the table.
void AIEditor::on_button_RemoveAICompany_clicked()
{
    if(ui->table_AICompanyList->selectedItems().empty())
        return;

    int selectedRow = ui->table_AICompanyList->selectionModel()->currentIndex().row();
    int selectedID = ui->table_AICompanyList->item(selectedRow,0)->text().toInt();

    aiMap.remove(selectedID);

    QMap<int, AIManager::AIInfo> tmpMap;

    //ReID and ReOwner companies.
    for(QMap<int,AIManager::AIInfo>::iterator it = aiMap.begin(); it != aiMap.end(); ++it)
    {
       if((*it).id > selectedID )
       {
           (*it).id = (*it).id - 1;
       }

       if((*it).ownerID > selectedID)
       {
           (*it).ownerID = (*it).ownerID - 1;
       }
       else if((*it).ownerID == selectedID)
       {
           (*it).ownerID = (*it).id;
       }

       tmpMap.insert((*it).id, (*it));


    }

    //Rekey Map
    aiMap.clear();
    aiMap = tmpMap;

    fillTableList();

}

//Switch to the turn events tool.
void AIEditor::on_Button_AI_TurnEventsEditor_clicked()
{
    cp_wsc.TurnEventEditorCW->raise();
}
