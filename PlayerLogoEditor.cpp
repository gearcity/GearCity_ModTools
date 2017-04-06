#include "PlayerLogoEditor.h"
#include "ui_PlayerLogoEditor.h"
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QFileDialog>

PlayerLogoEditor::PlayerLogoEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerLogoEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
}

PlayerLogoEditor::~PlayerLogoEditor()
{
    delete ui;
}

void PlayerLogoEditor::on_button_selectLogoFolder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Logo Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.dds");
    QStringList fileList = dir.entryList();

    ui->comboBox_logoFileSelector->clear();
    ui->comboBox_logoFileSelector->addItems(fileList);
    ui->label_logoFolder->setText(folderPath);
}


void PlayerLogoEditor::on_button_addLogo_clicked()
{
    QString name = ui->lineEdit_logoEditor->text();
    QString file = ui->comboBox_logoFileSelector->currentText();

    if(!file.endsWith(".dds"))
    {
        QMessageBox::critical(this,"Error",
                              QString("Picture file must be a .dds and end with the"
                                      " .dds extension!"));
        return;
    }

    logoMap.insert(name,file);
}

void PlayerLogoEditor::on_button_removeLogo_clicked()
{
    QString name = ui->lineEdit_logoEditor->text();
    logoMap.remove(name);
}


void PlayerLogoEditor::fillTable()
{
    ui->table_logo->clearContents();
    ui->table_logo->setRowCount(logoMap.size());

    int i = 0;
    for(QMap<QString,QString>::Iterator it = logoMap.begin(); it != logoMap.end(); ++it)
    {
        ui->table_logo->setItem(i,0,new QTableWidgetItem(it.key()));
        ui->table_logo->setItem(i,1,new QTableWidgetItem(it.value()));
        i++;
    }

}


void PlayerLogoEditor::on_table_logo_cellClicked(int row, int column)
{
    QString name = ui->table_logo->item(row,0)->text();
    QMap<QString,QString>::Iterator mapIT;
    mapIT = logoMap.find(name);

    ui->lineEdit_logoEditor->setText(mapIT.key());
    ui->comboBox_logoFileSelector->setCurrentIndex(
                ui->comboBox_logoFileSelector->findText(mapIT.value()));
}


void PlayerLogoEditor::on_button_newLogo_clicked()
{
    logoMap.clear();
    fillTable();
}

void PlayerLogoEditor::on_button_openLogoFile_clicked()
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
           QMessageBox::critical(this,"Error Logo Script",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return;
        }

        //Begin Reading and putting the data into the map
        QDomElement rootNode = doc.firstChildElement("LogoFiles");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","Logo Script is malformed.");
            return;
        }

        //Get first city, and lets go
        rootNode = doc.documentElement();
        Element = rootNode.firstChildElement("Logo");
        if(Element.isNull())
            return;

        logoMap.clear();


        do
        {
          logoMap.insert(Element.attributeNode("Name").value(),
                         Element.text());

          Element = Element.nextSiblingElement("Logo");
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

void PlayerLogoEditor::on_button_saveLogoFile_clicked()
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

    xmlWriter.writeStartElement("LogoFiles");

    //Loop through and write the data to file xml.
    for(QMap<QString,QString>::iterator it = logoMap.begin(); it != logoMap.end(); ++it)
    {
        xmlWriter.writeStartElement("Logo");
               xmlWriter.writeAttribute("Name",it.key());
               xmlWriter.writeCharacters(it.value());
        xmlWriter.writeEndElement();//Logo
    }

    //close the final tag and close the file.
     xmlWriter.writeEndDocument();

     saveFile.close();

}

void PlayerLogoEditor::on_pushButton_5_clicked()
{
    cp_wsc.LogoEditorCW->lower();
}
