#include "RacingSeriesEditor.h"
#include "ui_RacingSeriesEditor.h"
#include "QMessageBox"
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <QFileDialog>

RacingSeriesEditor::RacingSeriesEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RacingSeriesEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
}

RacingSeriesEditor::~RacingSeriesEditor()
{
    delete ui;
}

void RacingSeriesEditor::on_button_add_clicked()
{
    seriesData sd;
    sd.name = ui->lineEdit_name->text();
    sd.type = ui->comboBox_seriesType->currentText();
    sd.cartype = ui->comboBox_vehicleTypeLimit->currentText();
    sd.country = ui->comboBox_nation->currentText();
    sd.id = ui->spin_id->value();
    sd.supply = ui->comboBox_supply->currentIndex()+1;
    sd.ccrestrict = ui->spinBox_ccLimit->value();
    sd.start = ui->spinBox_startmonth->value();
    sd.end = ui->spinBox_endmonth->value();
    sd.birth = ui->spinBox_startyear->value();
    sd.death = ui->spinBox_endyear->value();
    sd.tracks = ui->spinBox_tracks->value();
    sd.teams = ui->spinBox_teams->value();
    sd.manuInvolve = ui->comboBox_manuInvolvement->currentIndex()+1;
    sd.localName = ui->checkBox_name_localize->isChecked();
    sd.localDesc = 0;
    sd.localParam = ui->checkBox_vehicleType_Localize->isChecked();
    sd.funding = ui->spin_funding->value();
    sd.popularity = ui->spin_pop->value();

    for(QList<seriesData>::Iterator it = seriesList.begin(); it != seriesList.end(); ++it)
    {
        if((*it).id == sd.id)
        {
            QMessageBox::critical(this,"Error!","This ID already Exists!");
            return;
        }
        else if((*it).name == sd.name)
        {
            QMessageBox::critical(this,"Error!","There is already a series with this name!");
            return;
        }
    }

    if(ui->comboBox_nation->findText(sd.country) == -1)
        ui->comboBox_nation->addItem(sd.country);
    if(ui->comboBox_seriesType->findText(sd.type) == -1)
        ui->comboBox_seriesType->addItem(sd.type);
    if(ui->comboBox_vehicleTypeLimit->findText(sd.cartype) == -1)
        ui->comboBox_vehicleTypeLimit->addItem(sd.cartype);


   seriesList.push_back(sd);

   fillTable();
}


void RacingSeriesEditor::on_button_remove_clicked()
{
    int id = ui->spin_id->value();
    for(QList<seriesData>::Iterator it = seriesList.begin(); it != seriesList.end(); ++it)
    {
        if((*it).id == id)
        {
            seriesList.erase(it);
            break;
        }
    }

    fillTable();
}

void RacingSeriesEditor::fillTable()
{
    ui->table_series->clearContents();
    ui->table_series->setRowCount(seriesList.size());
    int i = 0;
    for(QList<seriesData>::Iterator it = seriesList.begin(); it != seriesList.end(); ++it)
    {
            ui->table_series->setItem(i,0,new QTableWidgetItem(
                                         QString::number((*it).id)));
            ui->table_series->setItem(i,1,new QTableWidgetItem((*it).name));
            ui->table_series->setItem(i,2,new QTableWidgetItem(
                                             QString::number((*it).supply)));
            ui->table_series->setItem(i,3,new QTableWidgetItem(QString::number((*it).funding)));
            ui->table_series->setItem(i,4,new QTableWidgetItem(
                                         QString::number((*it).popularity)));

            ui->table_series->setItem(i,5,new QTableWidgetItem(
                                         QString::number((*it).start)+"/"+
                                         QString::number((*it).birth) ));
            ui->table_series->setItem(i,6,new QTableWidgetItem(
                                         QString::number((*it).end)+"/"+
                                         QString::number((*it).death) ));
            i++;
    }

}
void RacingSeriesEditor::on_table_series_cellClicked(int row, int column)
{

    int selectID = ui->table_series->item(row,0)->text().toInt();

    for(QList<seriesData>::Iterator it = seriesList.begin(); it != seriesList.end(); ++it)
    {

         if((*it).id == selectID)
         {
             seriesData sd = (*it);
                ui->lineEdit_name->setText(sd.name);
                ui->comboBox_seriesType->setEditText(sd.type);
                ui->comboBox_vehicleTypeLimit->setEditText(sd.cartype);
                ui->comboBox_nation->setEditText(sd.country);
                ui->spin_id->setValue(sd.id);
                ui->comboBox_supply->setCurrentIndex(sd.supply-1);
                ui->spinBox_ccLimit->setValue(sd.ccrestrict);
                ui->spinBox_startmonth->setValue(sd.start);
                ui->spinBox_endmonth->setValue(sd.end);
                ui->spinBox_startyear->setValue(sd.birth);
                ui->spinBox_endyear->setValue(sd.death);
                ui->spinBox_tracks->setValue(sd.tracks);
                ui->spinBox_teams->setValue(sd.teams);
                ui->comboBox_manuInvolvement->setCurrentIndex(sd.manuInvolve-1);
                ui->checkBox_name_localize->setChecked(sd.localName);
                ui->checkBox_vehicleType_Localize->setChecked(sd.localParam);
                ui->spin_funding->setValue(sd.funding);
                ui->spin_pop->setValue(sd.popularity);

             break;
         }

    }
}

void RacingSeriesEditor::on_button_new_clicked()
{
    ui->lineEdit_name->setText("");
    ui->comboBox_seriesType->setEditText("");
    ui->comboBox_vehicleTypeLimit->setEditText("");
    ui->comboBox_nation->setEditText("");
    ui->spin_id->setValue(0);
    ui->comboBox_supply->setCurrentIndex(0);
    ui->spinBox_ccLimit->setValue(0);
    ui->spinBox_startmonth->setValue(1);
    ui->spinBox_endmonth->setValue(1);
    ui->spinBox_startyear->setValue(1900);
    ui->spinBox_endyear->setValue(3000);
    ui->spinBox_tracks->setValue(1);
    ui->spinBox_teams->setValue(1);
    ui->comboBox_manuInvolvement->setCurrentIndex(0);
    ui->checkBox_name_localize->setChecked(0);
    ui->checkBox_vehicleType_Localize->setChecked(0);
    ui->spin_funding->setValue(5);
    ui->spin_pop->setValue(5);

    seriesList.clear();

    fillTable();

}

void RacingSeriesEditor::on_button_open_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open Racing Series File",
                                                        "","XML Files (*.xml)");

    if (openFileName != "")
    {
        seriesData data;

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
        QDomElement rootNode = doc.firstChildElement("RacingSeries");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","Racing Script is malformed.");
            return;
        }

        //Get first city, and lets go
        rootNode = doc.documentElement();
        Element = rootNode.firstChildElement("series");

        if(Element.isNull())
            return;

        do
        {
          //Company Attributes
            data.name = Element.attributeNode("name").value();
            data.id = Element.attributeNode("id").value().toInt();
            data.type = Element.attributeNode("type").value();
            data.supply = Element.attributeNode("supply").value().toInt();
            data.cartype = Element.attributeNode("cartype").value();
            data.ccrestrict = Element.attributeNode("ccrestrict").value().toInt();
            data.funding = Element.attributeNode("funding").value().toDouble();
            data.popularity = Element.attributeNode("popularity").value().toDouble();
            data.country = Element.attributeNode("country").value();
            data.start = Element.attributeNode("start").value().toInt();
            data.end = Element.attributeNode("end").value().toInt();
            data.birth = Element.attributeNode("birth").value().toInt();
            data.death = Element.attributeNode("death").value().toInt();
            data.tracks = Element.attributeNode("tracks").value().toInt();
            data.teams = Element.attributeNode("teams").value().toInt();
            //data.singleMake = Element.attributeNode("singleMake").value();
            //data.singleConstructor = Element.attributeNode("singleConstructor").value();
            //data.image = Element.attributeNode("image").value();
            //data.trophy = Element.attributeNode("trophy").value();
            data.manuInvolve = Element.attributeNode("manuInvolve").value().toInt();
            data.localName = Element.attributeNode("localName").value().toInt();
            data.localDesc = Element.attributeNode("localDesc").value().toInt();
            data.localParam = Element.attributeNode("localParam").value().toInt();


          seriesList.push_back(data);

          Element = Element.nextSiblingElement("series");
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

void RacingSeriesEditor::on_button_save_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save Racing Series File",
                                                "","XML Files (*.xml)");

    if (saveFileName != "")
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

            xmlWriter.writeStartElement("RacingSeries");

            //Loop through and write the data to file xml.
            for(QList<seriesData>::Iterator it = seriesList.begin(); it != seriesList.end(); ++it)
            {
                xmlWriter.writeStartElement("series");
                    xmlWriter.writeAttribute("name",(*it).name);
                    xmlWriter.writeAttribute("id",QString::number((*it).id));
                    xmlWriter.writeAttribute("type",(*it).type);
                    xmlWriter.writeAttribute("supply",QString::number((*it).supply));

                    if((*it).supply != 1)
                        xmlWriter.writeAttribute("cartype","0");
                    else
                    {
                        if((*it).cartype == "No Limit")
                        xmlWriter.writeAttribute("cartype","-1");
                        else
                        xmlWriter.writeAttribute("cartype",(*it).cartype);
                    }

                    xmlWriter.writeAttribute("ccrestrict",QString::number((*it).ccrestrict));
                    xmlWriter.writeAttribute("funding",QString::number((*it).funding));
                    xmlWriter.writeAttribute("popularity",QString::number((*it).popularity));
                    xmlWriter.writeAttribute("country",(*it).country);
                    xmlWriter.writeAttribute("start",QString::number((*it).start));

                    xmlWriter.writeAttribute("end",QString::number((*it).end));
                    xmlWriter.writeAttribute("birth",QString::number((*it).birth));
                    xmlWriter.writeAttribute("death",QString::number((*it).death));
                    xmlWriter.writeAttribute("tracks",QString::number((*it).tracks));
                    xmlWriter.writeAttribute("teams",QString::number((*it).teams));
                    xmlWriter.writeAttribute("singleMake","false");
                    xmlWriter.writeAttribute("singleConstructor","");
                    xmlWriter.writeAttribute("image","");
                    xmlWriter.writeAttribute("trophy","");
                    xmlWriter.writeAttribute("manuInvolve",QString::number((*it).manuInvolve));
                    xmlWriter.writeAttribute("localName",QString::number((*it).localName));
                    xmlWriter.writeAttribute("localDesc",QString::number((*it).localDesc));
                    xmlWriter.writeAttribute("localParam",QString::number((*it).localParam));

                 xmlWriter.writeEndElement(); //series

            }

            //close the final tag and close the file.
             xmlWriter.writeEndDocument();

             saveAIFile.close();

             //We Did it! Yay!
             QMessageBox::information(this,"Save Complete",
                                      "The AI File has been saved!", QMessageBox::Ok);
    }

}

void RacingSeriesEditor::on_button_return_clicked()
{
    cp_wsc.RacingCW->lower();
}
