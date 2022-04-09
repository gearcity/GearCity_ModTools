#include "PreMadeVehicleEditor.h"
#include "ui_PreMadeVehicleEditor.h"
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <QtWidgets/QFileDialog>
#include <QTextStream>
#include <QDir>

PreMadeVehicleEditor::PreMadeVehicleEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreMadeVehicleEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
}

PreMadeVehicleEditor::~PreMadeVehicleEditor()
{
    delete ui;
}

void PreMadeVehicleEditor::on_Button_Premade_AddNew_clicked()
{
    premadeData pd;
    pd.carFile = ui->TextBox_Premade_CarFileName->text();
    pd.designer = ui->TextBox_Premade_Designer->text();
    pd.name = ui->TextBox_Premade_Name->text();
    pd.startYear = ui->spinBox_premade_StartYear->value();
    pd.endYear = ui->spinBox_premade_endYear->value();

    pd.meshFile = ui->TextBox_Premade_MeshFile->text();

    if(pd.carFile.endsWith(".car") && pd.meshFile.endsWith(".mesh") && pd.name != "")
        premadeList.push_back(pd);
    else
    {
        QMessageBox::critical(this,"Error Adding Design",
                             QString("Vehicle design must have a valid .car file, valid .mesh file"
                                     " and a valid name."));
         return;
    }

    fillTable();
}

void PreMadeVehicleEditor::fillTable()
{
    ui->List_Premade_IncludeVehicles->clearContents();
    ui->List_Premade_IncludeVehicles->setRowCount(premadeList.size());
    int i = 0;
    for(QList<premadeData>::Iterator it = premadeList.begin(); it != premadeList.end(); ++it)
    {

        QTableWidgetItem *itemStartYear = new QTableWidgetItem;
        itemStartYear->setData(Qt::EditRole,(*it).startYear);
        QTableWidgetItem *itemEndYear = new QTableWidgetItem;
        itemEndYear->setData(Qt::EditRole,(*it).endYear);


            ui->List_Premade_IncludeVehicles->setItem(i,0,new QTableWidgetItem((*it).name));
            ui->List_Premade_IncludeVehicles->setItem(i,1,new QTableWidgetItem((*it).designer));
            ui->List_Premade_IncludeVehicles->setItem(i,2,new QTableWidgetItem((*it).carFile));
            ui->List_Premade_IncludeVehicles->setItem(i,3,new QTableWidgetItem((*it).meshFile));
            ui->List_Premade_IncludeVehicles->setItem(i,4,itemStartYear);
            ui->List_Premade_IncludeVehicles->setItem(i,5,itemEndYear);


            i++;
    }


}

void PreMadeVehicleEditor::on_List_Premade_IncludeVehicles_cellClicked(int row, int column)
{
    QString carFile = ui->List_Premade_IncludeVehicles->item(row,2)->text();

    for(QList<premadeData>::Iterator it = premadeList.begin(); it != premadeList.end(); ++it)
    {
        if((*it).carFile == carFile)
        {
            selectedPremade = (*it);

            ui->TextBox_Premade_CarFileName->setText(selectedPremade.carFile);
            ui->TextBox_Premade_Designer->setText(selectedPremade.designer);
            ui->TextBox_Premade_Name->setText(selectedPremade.name);
            ui->spinBox_premade_StartYear->setValue(selectedPremade.startYear);
            ui->spinBox_premade_endYear->setValue(selectedPremade.endYear);

            ui->TextBox_Premade_MeshFile->setText(selectedPremade.meshFile);


            break;
        }
        else
        {
            selectedPremade.carFile = "";
        }
    }

}

void PreMadeVehicleEditor::on_Button_Premade_EditSelected_clicked()
{

    for(QList<premadeData>::Iterator it = premadeList.begin(); it != premadeList.end(); ++it)
    {
        if((*it).carFile == selectedPremade.carFile)
        {
            (*it).carFile = ui->TextBox_Premade_CarFileName->text();
            (*it).designer = ui->TextBox_Premade_Designer->text();
            (*it).name = ui->TextBox_Premade_Name->text();
            (*it).startYear = ui->spinBox_premade_StartYear->value();
            (*it).endYear = ui->spinBox_premade_endYear->value();
            (*it).meshFile = ui->TextBox_Premade_MeshFile->text();

            break;
        }
    }

    fillTable();
}

void PreMadeVehicleEditor::on_Button_Premade_RemoveSelected_clicked()
{
    for(QList<premadeData>::Iterator it = premadeList.begin(); it != premadeList.end(); ++it)
    {
        if((*it).carFile == selectedPremade.carFile)
        {
            premadeList.erase(it);
            break;
        }
    }

    fillTable();
}

void PreMadeVehicleEditor::on_Button_Premade_NewList_clicked()
{

   QMessageBox::StandardButton reply = QMessageBox::question(this, "Start a New List",
         "Are you sure you want to start a new list? "
         "This will clear the data from the current list.",
         QMessageBox::Yes|QMessageBox::No);

     if (reply == QMessageBox::Yes)
     {
      premadeList.clear();
      fillTable();
      ui->TextBox_Premade_CarFileName->setText("");
      ui->TextBox_Premade_Designer->setText("");
      ui->TextBox_Premade_Name->setText("");
      ui->spinBox_premade_StartYear->setValue(1900);
      ui->spinBox_premade_endYear->setValue(1900);

      ui->TextBox_Premade_MeshFile->setText("");

     }
     else
     {
       return;
     }
}

void PreMadeVehicleEditor::on_Button_Premade_OpenList_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open Premade Vehicle List",
                                                        "","XML Files (*.xml)");

    if (openFileName != "")
    {
        premadeList.clear();
        premadeData data;

        //Open File
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
        QDomElement Element;


        if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
        {
           QMessageBox::critical(this,"Error Base Video",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return;
        }

        //Begin Reading and putting the data into the map
        QDomElement rootNode = doc.firstChildElement("Predesigns");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","Premades file is malformed.");
            return;
        }

        //Get first city, and lets go
        rootNode = doc.documentElement();
        Element = rootNode.firstChildElement("prebuilt");

        if(Element.isNull())
            return;

        do
        {
          //Company Attributes
            data.startYear = Element.attributeNode("startYear").value().toInt();
            data.endYear = Element.attributeNode("deathYear").value().toInt();
            data.meshFile = Element.attributeNode("baseMesh").value();
            data.designer = Element.attributeNode("author").value();
            data.name = Element.attributeNode("designName").value();
            data.carFile = Element.text();

          premadeList.push_back(data);

          Element = Element.nextSiblingElement("prebuilt");
        }
        while(!Element.isNull());

        file.close();
    }
    else
    {
        return;
    }

    fillTable();

}

void PreMadeVehicleEditor::on_Button_Premade_SaveList_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save Premade File List",
                                                "","XML Files (*.xml)");

    if (!saveFileName.endsWith(".xml"))
        saveFileName += ".xml";

    if (saveFileName != "")
    {
            //Open/create AI file
            QFile saveFile(saveFileName);
            if(!saveFile.open(QFile::WriteOnly | QFile::Text))
            {
               QMessageBox::critical(this,"Error",
                                     QString("Can not write to %1\n%2").arg(saveFileName,
                                     saveFile.errorString()));
               return;
            }

            //Lets Start Writing!
            QXmlStreamWriter xmlWriter(&saveFile);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement("Predesigns");

            //Loop through and write the data to file xml.
            for(QList<premadeData>::Iterator it = premadeList.begin(); it != premadeList.end(); ++it)
            {

                xmlWriter.writeStartElement("prebuilt");
                    xmlWriter.writeAttribute("startYear",QString::number((*it).startYear));
                    xmlWriter.writeAttribute("deathYear",QString::number((*it).endYear));

                    xmlWriter.writeAttribute("baseMesh",(*it).meshFile);
                    xmlWriter.writeAttribute("author",(*it).designer);
                    xmlWriter.writeAttribute("designName",(*it).name);
                   xmlWriter.writeCharacters((*it).carFile);


                 xmlWriter.writeEndElement(); //prebuilt

            }

            //close the final tag and close the file.
             xmlWriter.writeEndDocument();

             saveFile.close();

             //We Did it! Yay!
             QMessageBox::information(this,"Save Complete",
                                      "The Premades File has been saved!", QMessageBox::Ok);
    }

}

void PreMadeVehicleEditor::on_Button_Premade_ReturnToMain_clicked()
{
    cp_wsc.PreMadeEditorCW->lower();
}



void PreMadeVehicleEditor::on_Button_Premade_GetCarFile_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open Saved Car File",
                                                        "","Car Files (*.car)");

    QFile file(openFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
       QMessageBox::critical(this,"File Error",
                             QString("Whopse, opening %1 failed.").arg(openFileName));
       return;
    }

    ui->TextBox_Premade_CarFileName->setText(QFileInfo(file.fileName()).fileName());


    QTextStream stream(&file);
    QString line = stream.readLine(); //Revision
        if(line != "REV 1")
        {

            QMessageBox::critical(this,"Out of Date File",
                                  QString("The %1 car file is for an older"
                                          " version of gearcity.").arg(openFileName));
            file.close();
            return;
        }
        line = stream.readLine(); //mesh info

    ui->TextBox_Premade_MeshFile->setText(line);

    file.close();


}



void PreMadeVehicleEditor::on_Button_Premade_BulkAdd_clicked()
{
    //Open Premade Folder
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Car Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    QStringList fileList = dir.entryList(QStringList("*.car"));


    for(QStringList::iterator it = fileList.begin(); it != fileList.end(); ++it)
    {
        premadeData pd;
        QFile file(folderPath+"/"+(*it));
        if(!file.open(QIODevice::ReadOnly))
        {
           QMessageBox::critical(this,"File Error",
                                 QString("Whopse, opening %1 failed.").arg((*it)));
           return;
        }

        pd.carFile = (QFileInfo(file.fileName()).fileName());


        QTextStream stream(&file);
        QString line = stream.readLine(); //rev info
        if(line != "REV 1")
        {
            file.close();
            continue;
        }

        line = stream.readLine(); //car info
            line = stream.readLine(); //mesh info

         pd.meshFile = line;


        pd.designer = ui->TextBox_Premade_BulkDesigner->text();
        pd.name = pd.carFile.left(pd.carFile.lastIndexOf(".car"));
        pd.startYear = 1900;
        pd.endYear = 1999;

        if(pd.carFile.endsWith(".car") && pd.meshFile.endsWith(".mesh") && pd.name != "")
            premadeList.push_back(pd);

        file.close();
    }

    fillTable();
}
