#include "MusicEditor.h"
#include "ui_MusicEditor.h"
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QFileDialog>


MusicEditor::MusicEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
}

MusicEditor::~MusicEditor()
{
    delete ui;
}

void MusicEditor::on_button_selectMusicFolder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Logo Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.ogg");
    QStringList fileList = dir.entryList();

    ui->comboBox_songFile->clear();
    ui->comboBox_songFile->addItems(fileList);
    ui->label_selectedMusicFolder->setText(folderPath);
}

void MusicEditor::on_button_add_clicked()
{
    MusicData md;
    md.endYear = ui->spinBox_endYear->value();
    md.startYear = ui->spinBox_startYear->value();
    md.File = ui->comboBox_songFile->currentText();
    md.Name = ui->lineEdit_songName->text();
    md.use = ui->comboBox_playLocation->currentIndex();

    musicMap.insert(md.Name+"-"+ui->comboBox_playLocation->currentText(), md);

    fillTable();
}

void MusicEditor::on_button_remove_clicked()
{
    QString keyFinder = ui->lineEdit_songName->text() + "-" +
            ui->comboBox_playLocation->currentText();

    if(musicMap.contains(keyFinder))
            musicMap.remove(keyFinder);

    fillTable();
}


void MusicEditor::fillTable()
{
    ui->table_music->clearContents();
    ui->table_music->setRowCount(musicMap.size());

    int i = 0;
    for(QMap<QString,MusicData>::Iterator it = musicMap.begin(); it != musicMap.end(); ++it)
    {
        ui->table_music->setItem(i,0,new QTableWidgetItem((*it).Name));
        ui->table_music->setItem(i,1,new QTableWidgetItem((*it).File));
        if((*it).use == 0)
            ui->table_music->setItem(i,1,new QTableWidgetItem("All States"));
        else if((*it).use == 1)
            ui->table_music->setItem(i,1,new QTableWidgetItem("RnD"));
        else
            ui->table_music->setItem(i,1,new QTableWidgetItem("Intro"));

        ui->spinBox_endYear->setValue((*it).endYear);
        ui->spinBox_startYear->setValue((*it).startYear);

        i++;
    }

}


void MusicEditor::on_table_music_cellClicked(int row, int column)
{
    QString keyFinder = ui->table_music->item(row,0)->text()+"-"+ui->table_music->item(row,2)->text();
    QMap<QString,MusicData>::Iterator mapIT;
    mapIT = musicMap.find(keyFinder);

    ui->spinBox_endYear->setValue((*mapIT).endYear);
    ui->spinBox_startYear->setValue((*mapIT).startYear);
    ui->lineEdit_songName->setText((*mapIT).Name);

    ui->comboBox_playLocation->setCurrentIndex((*mapIT).use);
    ui->comboBox_songFile->setCurrentIndex(
                ui->comboBox_songFile->findText((*mapIT).File));
}


void MusicEditor::on_button_newList_clicked()
{
    musicMap.clear();
    fillTable();
}

void MusicEditor::on_button_openList_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","XML Files (*.xml)");

    if (openFileName != "")
    {

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
           QMessageBox::critical(this,"Error Music Script",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return;
        }

        //Begin Reading and putting the data into the map
        QDomElement rootNode = doc.firstChildElement("MusicFiles");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","Music Script is malformed.");
            return;
        }

        //Get first city, and lets go
        rootNode = doc.documentElement();
        Element = rootNode.firstChildElement("Music");
        if(Element.isNull())
            return;

        musicMap.clear();


        do
        {
          QString KeyName = Element.attributeNode("Name").value()+"-";
          int use = Element.attributeNode("Use").value().toInt();
          if(use == 0)
              KeyName += "All States";
          else if(use == 1)
              KeyName += "RnD";
          else
              KeyName += "Intro";

          MusicData md;
          md.Name = Element.attributeNode("Name").value();
          md.File = Element.text();
          md.startYear = Element.attributeNode("StartYear").value().toInt();
          md.endYear = Element.attributeNode("EndYear").value().toInt();
          md.use = Element.attributeNode("Use").value().toInt();

          musicMap.insert(KeyName,md);

          Element = Element.nextSiblingElement("Music");
        }
        while(!Element.isNull());

        file.close();

        fillTable();

    }
    else
    {
        return;
    }
}

void MusicEditor::on_button_saveList_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".xml"))
            saveFileName += ".xml";
    }


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

    xmlWriter.writeStartElement("MusicFiles");

    //Loop through and write the data to file xml.
    for(QMap<QString,MusicData>::iterator it = musicMap.begin(); it != musicMap.end(); ++it)
    {
        xmlWriter.writeStartElement("Music");
               xmlWriter.writeAttribute("Name",(*it).Name);
               xmlWriter.writeAttribute("StartYear",QString::number((*it).startYear));
               xmlWriter.writeAttribute("EndYear",QString::number((*it).endYear));
               xmlWriter.writeAttribute("Use",QString::number((*it).use));
               xmlWriter.writeCharacters((*it).File);
        xmlWriter.writeEndElement();//Logo
    }

    //close the final tag and close the file.
     xmlWriter.writeEndDocument();

     saveFile.close();
}

void MusicEditor::on_button_ReturnToMain_clicked()
{
    cp_wsc.MusicEditorCW->lower();
}