#include "NameListEditor.h"
#include "ui_NameListEditor.h"
#include <QtWidgets/QMessageBox>
#include <QFile>
#include <QtWidgets/QFileDialog>
#include <QTextStream>

NameListEditor::NameListEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NameListEditor)
{
    ui->setupUi(this);

    cp_wsc = wsc;
}

NameListEditor::~NameListEditor()
{
    delete ui;
}

void NameListEditor::on_button_addName_clicked()
{
    if(ui->line_Name->text() == "")
        return;


    if(nameList.contains(ui->line_Name->text()))
    {
        QMessageBox::critical(this,"Error!","This name already exists in the list!");
    }
    else
    {
        nameList.push_back(ui->line_Name->text());
    }

    fillList();
}


void NameListEditor::on_button_removeName_clicked()
{
    if(nameList.contains(ui->line_Name->text()))
    {
        nameList.removeAll(ui->line_Name->text());
    }

    fillList();
}


void NameListEditor::fillList()
{
    ui->list_Names->clear();
    ui->list_Names->addItems(nameList);
}


void NameListEditor::on_list_Names_currentTextChanged(const QString &currentText)
{
    ui->line_Name->setText(currentText);
}



void NameListEditor::on_button_NewNameList_clicked()
{
    nameList.clear();
    fillList();
    ui->line_Name->clear();
}



void NameListEditor::on_button_OpenNameList_clicked()
{
   QString openFile =  QFileDialog::getOpenFileName(this, "Open Name List", "",
                                                        "Text File (*.txt)");
   //set label with filename.
   if (openFile != "")
   {
       QFile file(openFile);
       if(!file.open(QFile::ReadOnly | QFile::Text))
       {
          QMessageBox::critical(this,"Error",
                                QString("Can not write to %1\n%2").arg(openFile,
                                file.errorString()));
          return;
       }

       QTextStream in(&file);

       while(!in.atEnd())
       {
           nameList.push_back(in.readLine());
       }

       file.close();
   }

   fillList();
   ui->line_Name->clear();
}



void NameListEditor::on_button_SaveNameList_clicked()
{

    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","Text Files (*.txt)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".txt"))
            saveFileName += ".txt";

        QFile fOut(saveFileName);
          if (fOut.open(QFile::WriteOnly | QFile::Text))
          {
            QTextStream s(&fOut);
            for (int i = 0; i < nameList.size(); ++i)
              s << nameList.at(i) << '\n';
          }
          else
          {
            return;
          }

          fOut.close();
    }
}

void NameListEditor::on_button_ReturnToPrevious_clicked()
{
    cp_wsc.NameListCW->lower();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}
