#include "MaterialMakerWindow.h"
#include "ui_MaterialMakerWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

MaterialMakerWindow::MaterialMakerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MaterialMakerWindow)
{
    ui->setupUi(this);
}

MaterialMakerWindow::~MaterialMakerWindow()
{
    delete ui;
}

void MaterialMakerWindow::on_Button_MatEditor_OpenMatFile_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Material File", "",
                                                         "Material File (*.material)");

    if (openFileName != "")
    {
        materialMap.clear();

        QFile openFile(openFileName);
        bool haveMaterial = false;
        QString materialName = "";

        if (openFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&openFile);
           while (!in.atEnd())
           {
              QString line = in.readLine();
              line = line.simplified();
              if(line.startsWith("material "))
              {
                  haveMaterial = true;
                  materialName = line.remove("material ");
              }
              else if(line.startsWith("texture "))
              {
                   if(haveMaterial)
                   {
                       materialMap.insert(materialName,line.remove("texture "));
                       haveMaterial = false;
                   }
              }

           }
           openFile.close();
         }

        ui->Label_MatEditor_CurrentMatFile->setText(openFileName);
        fillTable();
    }
    else
    {
        QMessageBox::critical(this,"Error!","We could not read your material file!");
    }
}


void MaterialMakerWindow::fillTable()
{
    int rowNumber = 0;
    ui->Table_MatEditor_Mats->clearContents();

    for(QMap<QString,QString>::iterator it = materialMap.begin(); it != materialMap.end(); ++it)
    {
        ui->Table_MatEditor_Mats->setRowCount(rowNumber+1);

        ui->Table_MatEditor_Mats->setItem(rowNumber,0,new QTableWidgetItem(it.key()));
        ui->Table_MatEditor_Mats->setItem(rowNumber,1,new QTableWidgetItem(it.value()));

        rowNumber++;
    }
}

void MaterialMakerWindow::on_Button_MatEditor_SaveMatFile_clicked()
{
    //Get File Name
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "",
                                                        "Material Files (*.material)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".material"))
            saveFileName += ".material";



        QFile saveFile(saveFileName);

           if (saveFile.open(QIODevice::ReadWrite))
           {
               QTextStream out(&saveFile);

               for(QMap<QString,QString>::iterator it = materialMap.begin(); it != materialMap.end(); ++it)
               {
                  out << "material " << it.key() << endl;
                  out << "{" << endl;
                  out << "\ttechnique" << endl;
                  out << "\t{" << endl;
                  out << "\t\tpass" << endl;
                  out << "\t\t{" << endl;

                  out << "\t\t\tambient 1.3 1.3 1.3 1" << endl;
                  out << "\t\t\tdiffuse 1.3 1.3 1.3 1" << endl;
                  out << "\t\t\tspecular 0 0 0 10 0" << endl;
                  out << "\n" << endl;
                  out << "\t\t\ttexture_unit" << endl;
                  out << "\t\t\t{" << endl;
                  out << "\t\t\t\ttexture "+it.value() << endl;
                  out << "\t\t\t}" << endl;
                  out << "\t\t}" << endl;
                  out << "\t}" << endl;
                  out << "}" << endl;
                  out << "\n" << endl;

               }
           }


           ui->Label_MatEditor_CurrentMatFile->setText(saveFileName);

           saveFile.close();

    }

}

void MaterialMakerWindow::on_Button_MatEditor_AddEditMat_clicked()
{
    if(ui->Line_MatEditor_TextureFile->text().endsWith(".dds"))
    {
        materialMap.insert(ui->Line_MatEditor_MatName->text(),
                           ui->Line_MatEditor_TextureFile->text());

        fillTable();
    }
    else
    {
        QMessageBox::critical(this,"Error!","Texture files must be in .dds format!");
    }
}

void MaterialMakerWindow::on_Button_MatEditor_ClearMatInfo_clicked()
{
    ui->Line_MatEditor_MatName->clear();
    ui->Line_MatEditor_TextureFile->clear();
}



void MaterialMakerWindow::on_Button_MatEditor_DeleteMat_clicked()
{
    materialMap.remove(ui->Line_MatEditor_MatName->text());
    fillTable();
}

void MaterialMakerWindow::on_Table_MatEditor_Mats_cellClicked(int row, int column)
{
    ui->Line_MatEditor_MatName->setText(ui->Table_MatEditor_Mats->item(row,0)->text());
   ui->Line_MatEditor_TextureFile->setText(ui->Table_MatEditor_Mats->item(row,1)->text());
}

