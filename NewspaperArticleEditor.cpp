#include "NewspaperArticleEditor.h"
#include "ui_NewspaperArticleEditor.h"
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QFileDialog>

NewspaperArticleEditor::NewspaperArticleEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewspaperArticleEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
}

NewspaperArticleEditor::~NewspaperArticleEditor()
{
    delete ui;
}

void NewspaperArticleEditor::on_button_addArticle_clicked()
{
    articleData ad;
    ad.article = ui->textbox_NewspaperArticle->toPlainText();
    ad.endingYear = ui->spinner_EndingYear->value();
    ad.startingYear = ui->spinner_StartingYear->value();
    ad.localized = ui->checkbox_NewspaperLocalized->isChecked();

    articles.push_back(ad);
    fillArticleTable();
}



void NewspaperArticleEditor::on_button_removeArticle_clicked()
{
    for(int i = 0; i < articles.size(); i++)
    {
        if(articles[i].article == ui->textbox_NewspaperArticle->toPlainText() &&
                articles[i].endingYear == ui->spinner_EndingYear->value() &&
                articles[i].startingYear == ui->spinner_StartingYear->value())
        {
            articles.remove(i);
            break;
        }
    }

    fillArticleTable();
}


void NewspaperArticleEditor::fillArticleTable()
{
    ui->table_Newspaper->clearContents();
    ui->table_Newspaper->setRowCount(articles.size());

    for(int i = 0; i < articles.size(); i++)
    {


        ui->table_Newspaper->setItem(i,0,new QTableWidgetItem(
                                         QString::number(articles[i].startingYear)));
        ui->table_Newspaper->setItem(i,1,new QTableWidgetItem(
                                         QString::number(articles[i].endingYear)));
        ui->table_Newspaper->setItem(i,2,new QTableWidgetItem(articles[i].article));
        ui->table_Newspaper->setItem(i,3,new QTableWidgetItem(
                                         QString::number(articles[i].localized)));

    }

}

void NewspaperArticleEditor::on_table_Newspaper_cellClicked(int row, int column)
{
    ui->spinner_StartingYear->setValue(ui->table_Newspaper->item(row,0)->text().toInt());
    ui->spinner_EndingYear->setValue(ui->table_Newspaper->item(row,1)->text().toInt());
    ui->textbox_NewspaperArticle->setPlainText(ui->table_Newspaper->item(row,2)->text());
    if(ui->table_Newspaper->item(row,3)->text().toInt())
    {
        ui->checkbox_NewspaperLocalized->setChecked(true);
    }
    else
    {
        ui->checkbox_NewspaperLocalized->setChecked(false);
    }

}


void NewspaperArticleEditor::on_button_newNewspaper_clicked()
{
    ui->spinner_StartingYear->setValue(1900);
    ui->spinner_EndingYear->setValue(2020);
    ui->textbox_NewspaperArticle->clear();
    ui->checkbox_NewspaperLocalized->setChecked(false);
    articles.clear();
    fillArticleTable();

}

void NewspaperArticleEditor::on_button_OpenNewspaper_clicked()
{

   QString fileName =  QFileDialog::getOpenFileName(this, "Open Newspaper Article File", "",
                                                         "XML Files (*.xml)");

   if(fileName == "")
       return;



   QFile file(fileName);
   if(!file.open(QFile::ReadOnly | QFile::Text))
   {
       QMessageBox::critical(this,"File Error",
                             QString("Whopse, opening %1 failed.").arg(fileName));
            return;
   }

   QString errorMsg;
   int errorLine, errorColumn;
   QDomDocument doc;
   QDomElement element;


   //Parse News.xml file.
   if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
   {
       QMessageBox::critical(this,"Error Processing Newspaper File",
                            QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                            errorMsg,errorLine,errorColumn));
      return;
   }

   //Begin Reading and putting the data into the map
   QDomElement rootNode = doc.firstChildElement("articles");
   if(rootNode.isNull())
   {
        QMessageBox::critical(this,"Error","Newspaper file is malformed.");
       return;
   }

   on_button_newNewspaper_clicked();

   //Get first year, and lets go
   rootNode = doc.documentElement();
   element = rootNode.firstChildElement("news");

   while(!element.isNull())
   {
       articleData ad;
       ad.article = element.text();
       if(element.hasAttribute("EndYear"))
        ad.endingYear = element.attributeNode("EndYear").value().toInt();
       else
        ad.endingYear = 3000;

       if(element.hasAttribute("StartYear"))
         ad.startingYear = element.attributeNode("StartYear").value().toInt();
       else
         ad.startingYear = 1890;


       ad.localized = element.attributeNode("localization").value().toInt();

       articles.push_back(ad);

       element = element.nextSiblingElement("news");
   }

   fillArticleTable();

   file.close();

}

void NewspaperArticleEditor::on_button_SaveNewspaper_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","XML Files (*.xml)");
    if (saveFileName == "")
        return;

    if (!saveFileName.endsWith(".xml"))
        saveFileName += ".xml";

    QFile saveFile(saveFileName);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(saveFileName,
                             saveFile.errorString()));

       return;
    }

    QXmlStreamWriter xmlWriter(&saveFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("articles");

    for(int i = 0; i < articles.size(); i++)
    {
       xmlWriter.writeStartElement("news");
          xmlWriter.writeAttribute("localization",QString::number(articles[i].localized));
          xmlWriter.writeAttribute("StartYear",QString::number(articles[i].startingYear));
          xmlWriter.writeAttribute("EndYear",QString::number(articles[i].endingYear));
          xmlWriter.writeCharacters(articles[i].article);
       xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndDocument();


    saveFile.close();


}

void NewspaperArticleEditor::on_button_ReturnToPrevious_clicked()
{
    cp_wsc.NewsPaperCW->lower();
}
