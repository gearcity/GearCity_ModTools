#include "DYKEditor.h"
#include "ui_DYKEditor.h"
#include <QtWidgets/QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtWidgets/QFileDialog>

DYKEditor::DYKEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DYKEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
}

DYKEditor::~DYKEditor()
{
    delete ui;
}


void DYKEditor::on_button_add_clicked()
{
    QModelIndexList indexList = ui->listWidget_dyk->selectionModel()->selectedIndexes();

    if(!indexList.isEmpty())
    {
        for( QModelIndexList::Iterator it = indexList.begin(); it!= indexList.end(); ++it)
        {
             dykList.replace((*it).row(), ui->textEdit_dyk->toPlainText());
        }
    }
    else
    {
        dykList.push_back(ui->textEdit_dyk->toPlainText());
    }

    fillListWidget();
}

void DYKEditor::on_button_remove_clicked()
{
    QModelIndexList indexList = ui->listWidget_dyk->selectionModel()->selectedIndexes();
    for( QModelIndexList::Iterator it = indexList.begin(); it!= indexList.end(); ++it)
    {
        dykList.removeAt((*it).row());
    }

    fillListWidget();
}

void DYKEditor::fillListWidget()
{
    ui->listWidget_dyk->clear();
    ui->listWidget_dyk->addItems(dykList);
}



void DYKEditor::on_Button_new_clicked()
{
    dykList.clear();
    ui->listWidget_dyk->clear();
    ui->textEdit_dyk->clear();
}

void DYKEditor::on_button_open_clicked()
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
           QMessageBox::critical(this,"Error DYK Script",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return;
        }

        //Begin Reading and putting the data into the map
        QDomElement rootNode = doc.firstChildElement("DYKHeader");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","DYK Script is malformed.");
            return;
        }

        //Get first city, and lets go
        rootNode = doc.documentElement();
        Element = rootNode.firstChildElement("dyk");
        if(Element.isNull())
            return;

        dykList.clear();


        do
        {
          dykList.append(Element.text());

          Element = Element.nextSiblingElement("dyk");
        }
        while(!Element.isNull());

        file.close();

        fillListWidget();

    }
    else
    {
        return;
    }
}

void DYKEditor::on_button_save_clicked()
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

    xmlWriter.writeStartElement("DYKHeader");

    //Loop through and write the data to file xml.
    for(QStringList::Iterator it = dykList.begin(); it != dykList.end(); ++it)
    {
        xmlWriter.writeTextElement("dyk",(*it));
    }

    //close the final tag and close the file.
     xmlWriter.writeEndDocument();

     saveFile.close();
}

void DYKEditor::on_listWidget_dyk_itemClicked(QListWidgetItem *item)
{
    ui->textEdit_dyk->clear();
    ui->textEdit_dyk->insertPlainText(item->text());
}

void DYKEditor::on_button_ReturnToMain_clicked()
{
    cp_wsc.DYKEditorCW->lower();
}

void DYKEditor::on_Button_NewDYK_clicked()
{
    ui->textEdit_dyk->clear();
    ui->listWidget_dyk->clearSelection();
}
