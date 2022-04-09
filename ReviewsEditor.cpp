#include "ReviewsEditor.h"
#include "ui_ReviewsEditor.h"
#include <QtWidgets/QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtWidgets/QFileDialog>

ReviewsEditor::ReviewsEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReviewsEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;
    lm = 0;
}

ReviewsEditor::~ReviewsEditor()
{
    delete ui;
}

void ReviewsEditor::on_button_Intro_Add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_Intro_minVal->value();
    rd.max = ui->spinBox_Intro_MaxVal->value();
    rd.localized = ui->checkBox_Intro_localized->isChecked();
    rd.review = ui->textbox_Intro->toPlainText();


    rd.carType = ui->comboBox_VehicleType_intro->itemData(ui->comboBox_VehicleType_intro->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_intro->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{2}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {2} Vehicle Type text. "
                                  "Your review must contain {2} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{3}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {3} Leading Competition text. "
                                  "Your review must contain {3} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{4}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {4} Worse Competition text. "
                                  "Your review must contain {4} somewhere in the text.");
           return;
        }
    }

    reviewsIntro.push_back(rd);

    fillIntroTable();
}

void ReviewsEditor::on_button_Intro_Remove_clicked()
{
    for(int i = 0; i < reviewsIntro.size(); i++)
    {

             if(reviewsIntro[i].review == ui->textbox_Intro->toPlainText())
             {
                 reviewsIntro.remove(i);
                 break;
             }

    }

    fillIntroTable();
}


void ReviewsEditor::fillIntroTable()
{
    ui->table_intro->clearContents();
     ui->table_intro->setRowCount(reviewsIntro.size());

    for(int i = 0; i < reviewsIntro.size(); i++)
    {
            ui->table_intro->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsIntro[i].min)));
            ui->table_intro->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsIntro[i].max)));
            ui->table_intro->setItem(i,2,new QTableWidgetItem(reviewsIntro[i].review));

    }



}

void ReviewsEditor::on_button_Intro_0Brand_clicked()
{
    ui->textbox_Intro->insertPlainText("{0}");
}

void ReviewsEditor::on_button_Intro_1Model_clicked()
{
    ui->textbox_Intro->insertPlainText("{1}");
}

void ReviewsEditor::on_button_Intro_2Type_clicked()
{
    ui->textbox_Intro->insertPlainText("{2}");
}

void ReviewsEditor::on_button_Intro_3BestCompetition_clicked()
{
    ui->textbox_Intro->insertPlainText("{3}");
}

void ReviewsEditor::on_button_Intro_4WorstCompetition_clicked()
{
    ui->textbox_Intro->insertPlainText("{4}");
}


void ReviewsEditor::on_table_intro_cellClicked(int row, int column)
{
    ui->spinBox_Intro_minVal->setValue(ui->table_intro->item(row,0)->text().toInt());
    ui->spinBox_Intro_MaxVal->setValue(ui->table_intro->item(row,1)->text().toInt());
    QString reviewText = ui->table_intro->item(row,2)->text();

    for(int i = 0; i < reviewsIntro.size(); i++)
    {

         if(reviewsIntro[i].review == reviewText)
         {
              ui->textbox_Intro->setPlainText(reviewsIntro[i].review);
              ui->checkBox_Intro_localized->setChecked(reviewsIntro[i].localized);

              if((reviewsIntro[i].carLocalize == 0 || lm == 0 ) && reviewsIntro[i].carType != "-1" )
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsIntro[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsIntro[i].carType));
              }
             break;
         }

    }

}

void ReviewsEditor::on_button_perf_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_Perf_MinVal->value();
    rd.max = ui->spinBox_Perf_MaxVal->value();
    rd.localized = ui->checkBox_perf_Localized->isChecked();
    rd.review = ui->textbox_perf->toPlainText();

    rd.carType = ui->comboBox_VehicleType_performance->itemData(ui->comboBox_VehicleType_performance->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_performance->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{2}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {2} HP text. "
                                  "Your review must contain {2} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{3}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {3} Top Speed text. "
                                  "Your review must contain {3} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{4}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {4} 0-60 text. "
                                  "Your review must contain {4} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{5}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {5} Engine text. "
                                  "Your review must contain {5} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{6}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {6} Vehicle Type text. "
                                  "Your review must contain {6} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{7}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {7} Power Review text. "
                                  "Your review must contain {7} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{8}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {8} Driveability Review text. "
                                  "Your review must contain {8} somewhere in the text.");
           return;
        }
    }

    reviewsPerformance.push_back(rd);

    fillPerformanceTable();
}

void ReviewsEditor::on_button_Perf_Remove_clicked()
{
    for(int i = 0; i < reviewsPerformance.size(); i++)
    {

         if(reviewsPerformance[i].review == ui->textbox_perf->toPlainText())
         {
             reviewsPerformance.remove(i);
             break;
         }

    }

    fillPerformanceTable();
}

void ReviewsEditor::on_button_perf_0Brand_clicked()
{
    ui->textbox_perf->insertPlainText("{0}");
}

void ReviewsEditor::on_button_perf_1Model_clicked()
{
    ui->textbox_perf->insertPlainText("{1}");
}

void ReviewsEditor::on_button_perf_2HP_clicked()
{
    ui->textbox_perf->insertPlainText("{2}");
}

void ReviewsEditor::on_button_perf_3TopSpeed_clicked()
{
    ui->textbox_perf->insertPlainText("{3}");
}

void ReviewsEditor::on_button_perf_4Acell_clicked()
{
    ui->textbox_perf->insertPlainText("{4}");
}

void ReviewsEditor::on_button_perf_5Engine_clicked()
{
    ui->textbox_perf->insertPlainText("{5}");
}

void ReviewsEditor::on_button_perf_6Type_clicked()
{
    ui->textbox_perf->insertPlainText("{6}");
}

void ReviewsEditor::on_button_perf_7Power_clicked()
{
    ui->textbox_perf->insertPlainText("{7}");
}

void ReviewsEditor::on_button_perf_8Drive_clicked()
{
    ui->textbox_perf->insertPlainText("{8}");
}

void ReviewsEditor::on_table_perf_cellClicked(int row, int column)
{
    ui->spinBox_Perf_MinVal->setValue(ui->table_perf->item(row,0)->text().toInt());
    ui->spinBox_Perf_MaxVal->setValue(ui->table_perf->item(row,1)->text().toInt());
    QString reviewText = ui->table_perf->item(row,2)->text();

    for(int i = 0; i < reviewsPerformance.size(); i++)
    {

         if(reviewsPerformance[i].review == reviewText)
         {
              ui->textbox_perf->setPlainText(reviewsPerformance[i].review);
              ui->checkBox_perf_Localized->setChecked(reviewsPerformance[i].localized);

              if(reviewsPerformance[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsPerformance[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsPerformance[i].carType));
              }
             break;
         }
    }

}

void ReviewsEditor::fillPerformanceTable()
{
    ui->table_perf->clearContents();
    ui->table_perf->setRowCount(reviewsPerformance.size());

    for(int i = 0; i < reviewsPerformance.size(); i++)
    {
            ui->table_perf->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsPerformance[i].min)));
            ui->table_perf->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsPerformance[i].max)));
            ui->table_perf->setItem(i,2,new QTableWidgetItem(reviewsPerformance[i].review));
    }

}



void ReviewsEditor::on_button_drive_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_drive_minval->value();
    rd.max = ui->spinBox_drive_maxval->value();
    rd.localized = ui->checkBox_drive_localized->isChecked();
    rd.review = ui->textbox_drive->toPlainText();

    rd.carType = ui->comboBox_VehicleType_driveability->itemData(ui->comboBox_VehicleType_driveability->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_driveability->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{2}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {2} Vehicle Type text. "
                                  "Your review must contain {2} somewhere in the text.");
           return;
        }

    }

    reviewsDrive.push_back(rd);

    fillDriveTable();
}

void ReviewsEditor::on_button_drive_remove_clicked()
{
    for(int i = 0; i < reviewsDrive.size(); i++)
    {
             if(reviewsDrive[i].review == ui->textbox_drive->toPlainText())
             {
                 reviewsDrive.remove(i);
                 break;
             }
    }

    fillDriveTable();
}

void ReviewsEditor::on_button_drive_0Brand_clicked()
{
    ui->textbox_drive->insertPlainText("{0}");
}

void ReviewsEditor::on_button_drive_1Model_clicked()
{
    ui->textbox_drive->insertPlainText("{1}");
}

void ReviewsEditor::on_button_drive_2Type_clicked()
{
    ui->textbox_drive->insertPlainText("{2}");
}

void ReviewsEditor::on_button_drive_3Rating_clicked()
{
    ui->textbox_drive->insertPlainText("{3}");
}

void ReviewsEditor::on_table_drive_cellClicked(int row, int column)
{
    ui->spinBox_drive_minval->setValue(ui->table_drive->item(row,0)->text().toInt());
    ui->spinBox_drive_maxval->setValue(ui->table_drive->item(row,1)->text().toInt());
    QString reviewText = ui->table_drive->item(row,2)->text();

    for(int i = 0; i < reviewsDrive.size(); i++)
    {

         if(reviewsDrive[i].review == reviewText)
         {
              ui->textbox_drive->setPlainText(reviewsDrive[i].review);
              ui->checkBox_drive_localized->setChecked(reviewsDrive[i].localized);

              if(reviewsDrive[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsDrive[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsDrive[i].carType));
              }
             break;
         }

    }

}

void ReviewsEditor::fillDriveTable()
{
    ui->table_drive->clearContents();
    ui->table_drive->setRowCount(reviewsDrive.size());

    for(int i = 0; i < reviewsDrive.size(); i++)
    {
            ui->table_drive->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsDrive[i].min)));
            ui->table_drive->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsDrive[i].max)));
            ui->table_drive->setItem(i,2,new QTableWidgetItem(reviewsDrive[i].review));
    }

}

void ReviewsEditor::on_button_lux_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_lux_minval->value();
    rd.max = ui->spinBox_lux_maxVal->value();
    rd.localized = ui->checkBox_lux_Localized->isChecked();
    rd.review = ui->textbox_lux->toPlainText();

    rd.carType = ui->comboBox_VehicleType_luxury->itemData(ui->comboBox_VehicleType_luxury->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_luxury->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }

    }

    reviewsLux.push_back(rd);

    fillLuxTable();
}

void ReviewsEditor::on_button_lux_remove_clicked()
{
    for(int i = 0; i < reviewsLux.size(); i++)
    {

             if(reviewsLux[i].review == ui->textbox_lux->toPlainText())
             {
                 reviewsLux.remove(i);
                 break;
             }

    }

    fillLuxTable();
}

void ReviewsEditor::on_button_lux_0Brand_clicked()
{
    ui->textbox_lux->insertPlainText("{0}");
}

void ReviewsEditor::on_button_lux_1Model_clicked()
{
    ui->textbox_lux->insertPlainText("{1}");
}

void ReviewsEditor::on_button_lux_2Lux_clicked()
{
    ui->textbox_lux->insertPlainText("{2}");
}

void ReviewsEditor::on_table_lux_cellClicked(int row, int column)
{
    ui->spinBox_lux_minval->setValue(ui->table_lux->item(row,0)->text().toInt());
    ui->spinBox_lux_maxVal->setValue(ui->table_lux->item(row,1)->text().toInt());
    QString reviewText = ui->table_lux->item(row,2)->text();

    for(int i = 0; i < reviewsLux.size(); i++)
    {

         if(reviewsLux[i].review == reviewText)
         {
              ui->textbox_lux->setPlainText(reviewsLux[i].review);
              ui->checkBox_lux_Localized->setChecked(reviewsLux[i].localized);

              if(reviewsLux[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsLux[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsLux[i].carType));
              }
             break;
         }

    }
}

void ReviewsEditor::fillLuxTable()
{
    ui->table_lux->clearContents();
    ui->table_lux->setRowCount(reviewsLux.size());

    for(int i = 0; i < reviewsLux.size(); i++)
    {
            ui->table_lux->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsLux[i].min)));
            ui->table_lux->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsLux[i].max)));
            ui->table_lux->setItem(i,2,new QTableWidgetItem(reviewsLux[i].review));
    }

}
void ReviewsEditor::on_button_safe_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_safe_minVal->value();
    rd.max = ui->spinBox_safe_maxval->value();
    rd.localized = ui->checkBox_safe_localized->isChecked();
    rd.review = ui->textbox_safe->toPlainText();

    rd.carType = ui->comboBox_VehicleType_safety->itemData(ui->comboBox_VehicleType_safety->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_safety->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }


    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
    }


    reviewsSafe.push_back(rd);

    fillSafeTable();
}

void ReviewsEditor::on_button_safe_remove_clicked()
{
    for(int i = 0; i < reviewsSafe.size(); i++)
    {

             if(reviewsSafe[i].review == ui->textbox_safe->toPlainText())
             {
                 reviewsSafe.remove(i);
                 break;
             }

    }

    fillSafeTable();
}

void ReviewsEditor::on_button_safe_0Brand_clicked()
{
    ui->textbox_safe->insertPlainText("{0}");
}

void ReviewsEditor::on_button_safe_1Model_clicked()
{
    ui->textbox_safe->insertPlainText("{1}");
}

void ReviewsEditor::on_table_safe_cellClicked(int row, int column)
{
    ui->spinBox_safe_minVal->setValue(ui->table_safe->item(row,0)->text().toInt());
    ui->spinBox_safe_maxval->setValue(ui->table_safe->item(row,1)->text().toInt());
    QString reviewText = ui->table_safe->item(row,2)->text();

    for(int i = 0; i < reviewsSafe.size(); i++)
    {

         if(reviewsSafe[i].review == reviewText)
         {
              ui->textbox_safe->setPlainText(reviewsSafe[i].review);
              ui->checkBox_safe_localized->setChecked(reviewsSafe[i].localized);

              if(reviewsSafe[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsSafe[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsSafe[i].carType));
              }
             break;
         }

    }
}

void ReviewsEditor::fillSafeTable()
{
    ui->table_safe->clearContents();
    ui->table_safe->setRowCount(reviewsSafe.size());

    for(int i = 0; i < reviewsSafe.size(); i++)
    {
            ui->table_safe->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsSafe[i].min)));
            ui->table_safe->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsSafe[i].max)));
            ui->table_safe->setItem(i,2,new QTableWidgetItem(reviewsSafe[i].review));
    }

}


void ReviewsEditor::on_button_fuel_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_fuel_minVal->value();
    rd.max = ui->spinBox_fuel_MaxVal->value();
    rd.localized = ui->checkBox_fuel_localize->isChecked();
    rd.review = ui->textbox_fuel->toPlainText();

    rd.carType = ui->comboBox_VehicleType_fuel->itemData(ui->comboBox_VehicleType_fuel->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_fuel->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{2}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {2} Fuel Consumption City text. "
                                  "Your review must contain {2} somewhere in the text.");
           return;
        }

    }

    reviewsFuel.push_back(rd);

    fillFuelTable();
}

void ReviewsEditor::on_button_fuel_remove_clicked()
{
    for(int i = 0; i < reviewsFuel.size(); i++)
    {
         if(reviewsFuel[i].review == ui->textbox_fuel->toPlainText())
         {
             reviewsFuel.remove(i);
             break;
         }
    }

    fillFuelTable();
}

void ReviewsEditor::on_button_fuel_0Brand_clicked()
{
    ui->textbox_fuel->insertPlainText("{0}");
}

void ReviewsEditor::on_button_fuel_1Model_clicked()
{
    ui->textbox_fuel->insertPlainText("{1}");
}

void ReviewsEditor::on_button_fuel_2FuelCity_clicked()
{
    ui->textbox_fuel->insertPlainText("{2}");
}

void ReviewsEditor::on_button_fuel_3FuelHwy_clicked()
{
    ui->textbox_fuel->insertPlainText("{3}");
}

void ReviewsEditor::on_table_fuel_cellClicked(int row, int column)
{
    ui->spinBox_fuel_minVal->setValue(ui->table_fuel->item(row,0)->text().toInt());
    ui->spinBox_fuel_MaxVal->setValue(ui->table_fuel->item(row,1)->text().toInt());
    QString reviewText = ui->table_fuel->item(row,2)->text();

    for(int i = 0; i < reviewsFuel.size(); i++)
    {

         if(reviewsFuel[i].review == reviewText)
         {
              ui->textbox_fuel->setPlainText(reviewsFuel[i].review);
              ui->checkBox_fuel_localize->setChecked(reviewsFuel[i].localized);

              if(reviewsFuel[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsFuel[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsFuel[i].carType));
              }
             break;
         }

    }
}

void ReviewsEditor::fillFuelTable()
{
    ui->table_fuel->clearContents();
    ui->table_fuel->setRowCount(reviewsFuel.size());

    for(int i = 0; i < reviewsFuel.size(); i++)
    {
            ui->table_fuel->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsFuel[i].min)));
            ui->table_fuel->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsFuel[i].max)));
            ui->table_fuel->setItem(i,2,new QTableWidgetItem(reviewsFuel[i].review));
    }

}

void ReviewsEditor::on_button_power_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_power_MinVal->value();
    rd.max = ui->spinBox_power_MaxVal->value();
    rd.localized = ui->checkBox_power_localize->isChecked();
    rd.review = ui->textbox_power->toPlainText();


    rd.carType = ui->comboBox_VehicleType_power->itemData(ui->comboBox_VehicleType_power->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_power->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }


    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Torque text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Towing Weight. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
    }

    reviewsPower.push_back(rd);

    fillPowerTable();
}

void ReviewsEditor::on_button_power_remove_clicked()
{
    for(int i = 0; i < reviewsPower.size(); i++)
    {

             if(reviewsPower[i].review == ui->textbox_power->toPlainText())
             {
                 reviewsPower.remove(i);
                 break;
             }

    }

    fillPowerTable();
}

void ReviewsEditor::on_button_power_0Torque_clicked()
{
    ui->textbox_power->insertPlainText("{0}");
}

void ReviewsEditor::on_button_power_1Towing_clicked()
{
    ui->textbox_power->insertPlainText("{1}");
}

void ReviewsEditor::on_table_power_cellClicked(int row, int column)
{
    ui->spinBox_power_MinVal->setValue(ui->table_power->item(row,0)->text().toInt());
    ui->spinBox_power_MaxVal->setValue(ui->table_power->item(row,1)->text().toInt());
    QString reviewText = ui->table_power->item(row,2)->text();

    for(int i = 0; i < reviewsPower.size(); i++)
    {

         if(reviewsPower[i].review == reviewText)
         {
              ui->textbox_power->setPlainText(reviewsPower[i].review);
              ui->checkBox_power_localize->setChecked(reviewsPower[i].localized);

              if(reviewsPower[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsPower[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsPower[i].carType));
              }
             break;
         }

    }

}

void ReviewsEditor::fillPowerTable()
{
    ui->table_power->clearContents();
    ui->table_power->setRowCount(reviewsPower.size());

    for(int i = 0; i < reviewsPower.size(); i++)
    {
            ui->table_power->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsPower[i].min)));
            ui->table_power->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsPower[i].max)));
            ui->table_power->setItem(i,2,new QTableWidgetItem(reviewsPower[i].review));
    }

}

void ReviewsEditor::on_button_cargo_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_cargo_minVal->value();
    rd.max = ui->spinBox_cargo_maxval->value();
    rd.localized = ui->checkBox_cargo_localize->isChecked();
    rd.review = ui->textbox_cargo->toPlainText();

    rd.carType = ui->comboBox_VehicleType_cargo->itemData(ui->comboBox_VehicleType_cargo->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_cargo->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }


    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{2}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {2} Cargo Space text. "
                                  "Your review must contain {2} somewhere in the text.");
           return;
        }
    }

    reviewsCargo.push_back(rd);

    fillCargoTable();
}

void ReviewsEditor::on_button_cargo_remove_clicked()
{
    for(int i = 0; i < reviewsCargo.size(); i++)
    {
         if(reviewsCargo[i].review == ui->textbox_cargo->toPlainText())
         {
             reviewsCargo.remove(i);
             break;
         }
    }

    fillCargoTable();
}

void ReviewsEditor::on_button_cargo_0Brand_clicked()
{
    ui->textbox_cargo->insertPlainText("{0}");
}

void ReviewsEditor::on_button_cargo_1Model_clicked()
{
    ui->textbox_cargo->insertPlainText("{1}");
}

void ReviewsEditor::on_button_cargo_2CargoSpace_clicked()
{
    ui->textbox_cargo->insertPlainText("{2}");
}

void ReviewsEditor::on_table_cargo_cellClicked(int row, int column)
{
    ui->spinBox_cargo_minVal->setValue(ui->table_cargo->item(row,0)->text().toInt());
    ui->spinBox_cargo_maxval->setValue(ui->table_cargo->item(row,1)->text().toInt());
    QString reviewText = ui->table_cargo->item(row,2)->text();

    for(int i = 0; i < reviewsCargo.size(); i++)
    {

         if(reviewsCargo[i].review == reviewText)
         {
              ui->textbox_cargo->setPlainText(reviewsCargo[i].review);
              ui->checkBox_cargo_localize->setChecked(reviewsCargo[i].localized);

              if(reviewsCargo[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsCargo[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsCargo[i].carType));
              }
             break;
         }

    }

}

void ReviewsEditor::fillCargoTable()
{
    ui->table_cargo->clearContents();
    ui->table_cargo->setRowCount(reviewsCargo.size());

    for(int i = 0; i < reviewsCargo.size(); i++)
    {
            ui->table_cargo->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsCargo[i].min)));
            ui->table_cargo->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsCargo[i].max)));
            ui->table_cargo->setItem(i,2,new QTableWidgetItem(reviewsCargo[i].review));
    }

}


void ReviewsEditor::on_button_qual_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_qual_minVal->value();
    rd.max = ui->spinBox_qual_MaxVal->value();
    rd.localized = ui->checkBox_Qual_Localize->isChecked();
    rd.review = ui->textbox_qual->toPlainText();

    rd.carType = ui->comboBox_VehicleType_quality->itemData(ui->comboBox_VehicleType_quality->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_quality->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }


    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
    }


    reviewsQuality.push_back(rd);

    fillQualTable();
}

void ReviewsEditor::on_button_qual_remove_clicked()
{
    for(int i = 0; i < reviewsQuality.size(); i++)
    {

             if(reviewsQuality[i].review == ui->textbox_qual->toPlainText())
             {
                 reviewsQuality.remove(i);
                 break;
             }

    }

    fillQualTable();
}

void ReviewsEditor::on_button_qual_0Brand_clicked()
{
      ui->textbox_qual->insertPlainText("{0}");
}

void ReviewsEditor::on_button_qual_1Model_clicked()
{
      ui->textbox_qual->insertPlainText("{1}");
}

void ReviewsEditor::on_table_qual_cellClicked(int row, int column)
{
    ui->spinBox_qual_minVal->setValue(ui->table_qual->item(row,0)->text().toInt());
    ui->spinBox_qual_MaxVal->setValue(ui->table_qual->item(row,1)->text().toInt());
    QString reviewText = ui->table_qual->item(row,2)->text();

    for(int i = 0; i < reviewsQuality.size(); i++)
    {

         if(reviewsQuality[i].review == reviewText)
         {
              ui->textbox_qual->setPlainText(reviewsQuality[i].review);
              ui->checkBox_Qual_Localize->setChecked(reviewsQuality[i].localized);

              if(reviewsQuality[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsQuality[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsQuality[i].carType));
              }
             break;
         }

    }
}

void ReviewsEditor::fillQualTable()
{
    ui->table_qual->clearContents();
    ui->table_qual->setRowCount(reviewsQuality.size());

    for(int i = 0; i < reviewsQuality.size(); i++)
    {
            ui->table_qual->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsQuality[i].min)));
            ui->table_qual->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsQuality[i].max)));
            ui->table_qual->setItem(i,2,new QTableWidgetItem(reviewsQuality[i].review));
    }

}


void ReviewsEditor::on_button_depend_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_depend_minval->value();
    rd.max = ui->spinBox_depend_maxVal->value();
    rd.localized = ui->checkBox_depend_localize->isChecked();
    rd.review = ui->textbox_depend->toPlainText();

    rd.carType = ui->comboBox_VehicleType_dependability->itemData(ui->comboBox_VehicleType_dependability->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_dependability->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
    }


    reviewsDepend.push_back(rd);

    fillDependTable();
}

void ReviewsEditor::on_button_depend_remove_clicked()
{
    for(int i = 0; i < reviewsDepend.size(); i++)
    {

             if(reviewsDepend[i].review == ui->textbox_depend->toPlainText())
             {
                 reviewsDepend.remove(i);
                 break;
             }

    }

    fillDependTable();
}

void ReviewsEditor::on_button_depend_0Brand_clicked()
{
     ui->textbox_depend->insertPlainText("{0}");
}

void ReviewsEditor::on_button_depend_1Model_clicked()
{
     ui->textbox_depend->insertPlainText("{1}");
}

void ReviewsEditor::on_table_depend_cellClicked(int row, int column)
{
    ui->spinBox_depend_minval->setValue(ui->table_depend->item(row,0)->text().toInt());
    ui->spinBox_depend_maxVal->setValue(ui->table_depend->item(row,1)->text().toInt());
    QString reviewText = ui->table_depend->item(row,2)->text();

    for(int i = 0; i < reviewsDepend.size(); i++)
    {

         if(reviewsDepend[i].review == reviewText)
         {
              ui->textbox_depend->setPlainText(reviewsDepend[i].review);
              ui->checkBox_depend_localize->setChecked(reviewsDepend[i].localized);

              if(reviewsDepend[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsDepend[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsDepend[i].carType));
              }
             break;
         }

    }
}

void ReviewsEditor::fillDependTable()
{
    ui->table_depend->clearContents();
    ui->table_depend->setRowCount(reviewsDepend.size());

    for(int i = 0; i < reviewsDepend.size(); i++)
    {
            ui->table_depend->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsDepend[i].min)));
            ui->table_depend->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsDepend[i].max)));
            ui->table_depend->setItem(i,2,new QTableWidgetItem(reviewsDepend[i].review));
    }

}


void ReviewsEditor::on_button_overall_add_clicked()
{
    reviewData rd;
    rd.min = ui->spinBox_overall_minval->value();
    rd.max = ui->spinBox_overall_maxval->value();
    rd.localized = ui->checkBox_overall_localize->isChecked();
    rd.review = ui->textbox_overall->toPlainText();

    rd.carType = ui->comboBox_VehicleType_overall->itemData(ui->comboBox_VehicleType_overall->currentIndex()).toString();

    if(rd.carType == ui->comboBox_VehicleType_overall->currentText() || rd.carType == "-1" )
    {
        rd.carLocalize = 0;
    }
    else
    {
        if(rd.carType != "-1")
           rd.carLocalize = 1;
    }

    if(!rd.localized)
    {
        if(!rd.review.contains("{0}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {0} Brand text. Your review must "
                                  "contain {0} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{1}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {1} Model text. Your review must "
                                  "contain {1} somewhere in the text.");
           return;
        }
        else if(!rd.review.contains("{2}"))
        {
            QMessageBox::critical(this,"Error","Review is missing {2} Vehicle Type text. "
                                  "Your review must contain {2} somewhere in the text.");
           return;
        }
    }


    reviewsOverall.push_back(rd);

    fillOverallTable();
}

void ReviewsEditor::on_button_overall_remove_clicked()
{
    for(int i = 0; i < reviewsOverall.size(); i++)
    {

             if(reviewsOverall[i].review == ui->textbox_overall->toPlainText())
             {
                 reviewsOverall.remove(i);
                 break;
             }

    }

    fillOverallTable();
}

void ReviewsEditor::on_button_overall_0Brand_clicked()
{
    ui->textbox_overall->insertPlainText("{0}");
}

void ReviewsEditor::on_button_overall_1Model_clicked()
{
    ui->textbox_overall->insertPlainText("{1}");
}

void ReviewsEditor::on_button_overall_2Type_clicked()
{
    ui->textbox_overall->insertPlainText("{2}");
}

void ReviewsEditor::on_table_overall_cellClicked(int row, int column)
{
    ui->spinBox_overall_minval->setValue(ui->table_overall->item(row,0)->text().toInt());
    ui->spinBox_overall_maxval->setValue(ui->table_overall->item(row,1)->text().toInt());
    QString reviewText = ui->table_overall->item(row,2)->text();

    for(int i = 0; i < reviewsOverall.size(); i++)
    {

         if(reviewsOverall[i].review == reviewText)
         {
              ui->textbox_overall->setPlainText(reviewsOverall[i].review);
              ui->checkBox_overall_localize->setChecked(reviewsOverall[i].localized);

              if(reviewsOverall[i].carLocalize == 0 || lm == 0)
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findText(reviewsOverall[i].carType));
              else
              {
                ui->comboBox_VehicleType_intro->setCurrentIndex(ui->comboBox_VehicleType_intro->findData(reviewsOverall[i].carType));
              }
             break;
         }

    }
}


void ReviewsEditor::fillOverallTable()
{
    ui->table_overall->clearContents();
    ui->table_overall->setRowCount(reviewsOverall.size());

    for(int i = 0; i < reviewsOverall.size(); i++)
    {
            ui->table_overall->setItem(i,0,new QTableWidgetItem(
                                             QString::number(reviewsOverall[i].min)));
            ui->table_overall->setItem(i,1,new QTableWidgetItem(
                                             QString::number(reviewsOverall[i].max)));
            ui->table_overall->setItem(i,2,new QTableWidgetItem(reviewsOverall[i].review));
    }

}



void ReviewsEditor::on_button_NewReview_clicked()
{

    ui->table_intro->clearContents();
    ui->table_overall->clearContents();
    ui->table_perf->clearContents();
    ui->table_drive->clearContents();
    ui->table_lux->clearContents();
    ui->table_safe->clearContents();
    ui->table_fuel->clearContents();
    ui->table_power->clearContents();
    ui->table_cargo->clearContents();
    ui->table_qual->clearContents();
    ui->table_depend->clearContents();


    ui->table_intro->setRowCount(0);
    ui->table_overall->setRowCount(0);
    ui->table_perf->setRowCount(0);
    ui->table_drive->setRowCount(0);
    ui->table_lux->setRowCount(0);
    ui->table_safe->setRowCount(0);
    ui->table_fuel->setRowCount(0);
    ui->table_power->setRowCount(0);
    ui->table_cargo->setRowCount(0);
    ui->table_qual->setRowCount(0);
    ui->table_depend->setRowCount(0);


    ui->spinBox_Intro_minVal->setValue(0);
    ui->spinBox_Intro_MaxVal->setValue(150);
    ui->spinBox_Perf_MinVal->setValue(0);
    ui->spinBox_Perf_MaxVal->setValue(150);
    ui->spinBox_drive_minval->setValue(0);
    ui->spinBox_drive_maxval->setValue(150);
    ui->spinBox_lux_minval->setValue(0);
    ui->spinBox_lux_maxVal->setValue(150);
    ui->spinBox_safe_minVal->setValue(0);
    ui->spinBox_safe_maxval->setValue(150);
    ui->spinBox_fuel_minVal->setValue(0);
    ui->spinBox_fuel_MaxVal->setValue(150);
    ui->spinBox_power_MinVal->setValue(0);
    ui->spinBox_power_MaxVal->setValue(150);
    ui->spinBox_cargo_minVal->setValue(0);
    ui->spinBox_cargo_maxval->setValue(150);
    ui->spinBox_qual_minVal->setValue(0);
    ui->spinBox_qual_MaxVal->setValue(150);
    ui->spinBox_depend_minval->setValue(0);
    ui->spinBox_depend_maxVal->setValue(150);
    ui->spinBox_overall_minval->setValue(0);
    ui->spinBox_overall_maxval->setValue(150);


    ui->checkBox_Intro_localized->setChecked(false);
    ui->checkBox_perf_Localized->setChecked(false);
    ui->checkBox_drive_localized->setChecked(false);
    ui->checkBox_lux_Localized->setChecked(false);
    ui->checkBox_fuel_localize->setChecked(false);
    ui->checkBox_power_localize->setChecked(false);
    ui->checkBox_safe_localized->setChecked(false);
    ui->checkBox_cargo_localize->setChecked(false);
    ui->checkBox_Qual_Localize->setChecked(false);
    ui->checkBox_depend_localize->setChecked(false);
    ui->checkBox_overall_localize->setChecked(false);


    ui->textbox_Intro->clear();
    ui->textbox_drive->clear();
    ui->textbox_safe->clear();
    ui->textbox_fuel->clear();
    ui->textbox_power->clear();
    ui->textbox_cargo->clear();
    ui->textbox_qual->clear();
    ui->textbox_depend->clear();
    ui->textbox_overall->clear();

    reviewsIntro.clear();
    reviewsPerformance.clear();
    reviewsDrive.clear();
    reviewsLux.clear();
    reviewsSafe.clear();
    reviewsFuel.clear();
    reviewsPower.clear();
    reviewsCargo.clear();
    reviewsQuality.clear();
    reviewsDepend.clear();
    reviewsOverall.clear();


}

void ReviewsEditor::on_button_OpenReviews_clicked()
{


    QString fileName =  QFileDialog::getOpenFileName(this, "Open Reviews File", "",
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
    QDomElement subElement;

    //Parse Reviews.xml file.
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        QMessageBox::critical(this,"Error Processing Reviews File",
                             QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                             errorMsg,errorLine,errorColumn));
       return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("ReviewText");
    if(rootNode.isNull())
    {
         QMessageBox::critical(this,"Error","Reviews file is malformed.");
        return;
    }

    on_button_NewReview_clicked();

    //Get first value and lets go!
    rootNode = doc.documentElement();


    element = rootNode.firstChildElement("Performance");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsPerformance.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }

    //Driveability
    element = rootNode.firstChildElement("Driveability");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";



        reviewsDrive.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Luxury
    element = rootNode.firstChildElement("Luxury");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";



        reviewsLux.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Safety
    element = rootNode.firstChildElement("Safety");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsSafe.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Fuel
    element = rootNode.firstChildElement("Fuel");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsFuel.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Power
    element = rootNode.firstChildElement("Power");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsPower.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Cargo
    element = rootNode.firstChildElement("Cargo");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsCargo.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Quality
    element = rootNode.firstChildElement("Quality");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsQuality.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Dependability
    element = rootNode.firstChildElement("Dependability");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsDepend.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Overall
    element = rootNode.firstChildElement("Overall");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsOverall.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }


    //Introductions
    element = rootNode.firstChildElement("Introductions");
    subElement = element.firstChildElement("review");

    while(!subElement.isNull())
    {
        reviewData rd;
        rd.min = subElement.attributeNode("minVal").value().toInt();
        rd.max = subElement.attributeNode("maxVal").value().toInt();
        rd.localized = subElement.attributeNode("localization").value().toInt();
        rd.review = subElement.text();

        if(!subElement.attributeNode("localizeType").isNull())
         rd.carLocalize =  subElement.attributeNode("localizeType").value().toInt();
        else
         rd.carLocalize = 0;

        if(!subElement.attributeNode("vehicleType").isNull())
         rd.carType =  subElement.attributeNode("vehicleType").value();
        else
         rd.carType = "-1";


        reviewsIntro.push_back(rd);

       subElement = subElement.nextSiblingElement("review");
    }



    fillIntroTable();
    fillPerformanceTable();
    fillDriveTable();
    fillLuxTable();
    fillSafeTable();
    fillFuelTable();
    fillPowerTable();
    fillCargoTable();
    fillQualTable();
    fillDependTable();
    fillOverallTable();

    file.close();
}

void ReviewsEditor::on_button_SaveReviews_clicked()
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

    xmlWriter.writeStartElement("ReviewText");

        xmlWriter.writeStartElement("Performance");
        for(int i = 0; i < reviewsPerformance.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsPerformance[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsPerformance[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsPerformance[i].localized));

                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsPerformance[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsPerformance[i].carType);


                xmlWriter.writeCharacters(reviewsPerformance[i].review);



            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Performance


        xmlWriter.writeStartElement("Driveability");
        for(int i = 0; i < reviewsDrive.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsDrive[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsDrive[i].max));
                xmlWriter.writeAttribute("localization", QString::number(reviewsDrive[i].localized));
                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsDrive[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsDrive[i].carType);

                xmlWriter.writeCharacters(reviewsDrive[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Driveability


        xmlWriter.writeStartElement("Luxury");
        for(int i = 0; i < reviewsLux.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsLux[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsLux[i].max));
                xmlWriter.writeAttribute("localization", QString::number(reviewsLux[i].localized));
                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsLux[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsLux[i].carType);

                xmlWriter.writeCharacters(reviewsLux[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Luxury

        xmlWriter.writeStartElement("Safety");
        for(int i = 0; i < reviewsSafe.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsSafe[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsSafe[i].max));
                xmlWriter.writeAttribute("localization", QString::number(reviewsSafe[i].localized));
                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsSafe[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsSafe[i].carType);

                xmlWriter.writeCharacters(reviewsSafe[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Safety


        xmlWriter.writeStartElement("Fuel");
        for(int i = 0; i < reviewsFuel.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsFuel[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsFuel[i].max));
                xmlWriter.writeAttribute("localization", QString::number(reviewsFuel[i].localized));
                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsFuel[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsFuel[i].carType);

                xmlWriter.writeCharacters(reviewsFuel[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Fuel

        xmlWriter.writeStartElement("Power");
        for(int i = 0; i < reviewsPower.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsPower[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsPower[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsPower[i].localized));
                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsPower[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsPower[i].carType);

                xmlWriter.writeCharacters(reviewsPower[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Power


        xmlWriter.writeStartElement("Cargo");
        for(int i = 0; i < reviewsCargo.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsCargo[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsCargo[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsCargo[i].localized));
                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsCargo[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsCargo[i].carType);

                xmlWriter.writeCharacters(reviewsCargo[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Cargo


        xmlWriter.writeStartElement("Quality");
        for(int i = 0; i < reviewsQuality.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsQuality[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsQuality[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsQuality[i].localized));

                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsQuality[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsQuality[i].carType);

                xmlWriter.writeCharacters(reviewsQuality[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Quality

        xmlWriter.writeStartElement("Dependability");
        for(int i = 0; i < reviewsDepend.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsDepend[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsDepend[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsDepend[i].localized));

                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsDepend[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsDepend[i].carType);

                xmlWriter.writeCharacters(reviewsDepend[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Dependability

        xmlWriter.writeStartElement("Overall");
        for(int i = 0; i < reviewsOverall.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsOverall[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsOverall[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsOverall[i].localized));

                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsOverall[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsOverall[i].carType);

                xmlWriter.writeCharacters(reviewsOverall[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Overall

        xmlWriter.writeStartElement("Introductions");
        for(int i = 0; i < reviewsIntro.size(); i++)
        {
            xmlWriter.writeStartElement("review");
                xmlWriter.writeAttribute("minVal",QString::number(reviewsIntro[i].min));
                xmlWriter.writeAttribute("maxVal",QString::number(reviewsIntro[i].max));
                xmlWriter.writeAttribute("localization",
                                         QString::number(reviewsIntro[i].localized));

                xmlWriter.writeAttribute("localizeType",
                                         QString::number(reviewsIntro[i].carLocalize));

                xmlWriter.writeAttribute("vehicleType",
                                        reviewsIntro[i].carType);

                xmlWriter.writeCharacters(reviewsIntro[i].review);
            xmlWriter.writeEndElement(); //Reviews
        }
        xmlWriter.writeEndElement(); //Introductions


    xmlWriter.writeEndDocument();


    saveFile.close();
}

void ReviewsEditor::on_button_PreviousMenu_clicked()
{
 cp_wsc.ReviewsCW->lower();
}



void ReviewsEditor::on_toolButton_select_LocalizationFile_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Localization File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        //Create localization mananger
        lm = new LocalizationManager(openFileName, cp_wsc.ReviewsCW);

        //If localization has words
        if(lm->size()!=0)
        {
            ui->label_selectedLocalizationFile->setText(openFileName);
        }
    }
    else
    {
        return;
    }
}

void ReviewsEditor::on_toolButton_selectComponentsFile_clicked()
{
   QString componentsFileName =  QFileDialog::getOpenFileName(this, "Open Components File", "",
                                                     "XML Files (*.xml)");

   if (componentsFileName != "")
   {
       //if we have a file, create a components object, then send the combos over to be filled.
       cm = new ComponentsManager(componentsFileName,cp_wsc.ReviewsCW);


       //If cm was not processed fine, then we shouldn't touch the combos
       if(cm->isGood())
       {
           ui->label_selectedComponentsFile->setText(componentsFileName);
           fillCarTypeCombos();
       }
   }
   else
   {
       return;
   }

}


void ReviewsEditor::fillCarTypeCombos()
{
    ui->comboBox_VehicleType_cargo->clear();
    ui->comboBox_VehicleType_dependability->clear();
    ui->comboBox_VehicleType_driveability->clear();
    ui->comboBox_VehicleType_fuel->clear();
    ui->comboBox_VehicleType_intro->clear();
    ui->comboBox_VehicleType_luxury->clear();
    ui->comboBox_VehicleType_overall->clear();
    ui->comboBox_VehicleType_performance->clear();
    ui->comboBox_VehicleType_power->clear();
    ui->comboBox_VehicleType_quality->clear();
    ui->comboBox_VehicleType_safety->clear();

            ui->comboBox_VehicleType_cargo->addItem("Generic",-1);
            ui->comboBox_VehicleType_dependability->addItem("Generic",-1);
            ui->comboBox_VehicleType_driveability->addItem("Generic",-1);
            ui->comboBox_VehicleType_fuel->addItem("Generic",-1);
            ui->comboBox_VehicleType_intro->addItem("Generic",-1);
            ui->comboBox_VehicleType_luxury->addItem("Generic",-1);
            ui->comboBox_VehicleType_overall->addItem("Generic",-1);
            ui->comboBox_VehicleType_performance->addItem("Generic",-1);
            ui->comboBox_VehicleType_power->addItem("Generic",-1);
            ui->comboBox_VehicleType_quality->addItem("Generic",-1);
            ui->comboBox_VehicleType_safety->addItem("Generic",-1);
    if(cm != 0)
    {
      ComponentsManager::ComponentDataLists cdl = cm->returnDataList();
      QString ctName = "";
      QString ctData = "";

     foreach(ComponentsManager::CarTypes ct, cdl.cartypesList)
     {
         if(lm != 0 && ct.localName)
         {
             ctName = lm->getWord(ct.type.toInt());
             ctData = ct.type;
         }
         else
         {
             ctName = ct.type;
             ctData = ct.type;
         }

        ui->comboBox_VehicleType_cargo->addItem(ctName, ctData);
        ui->comboBox_VehicleType_dependability->addItem(ctName, ctData);
        ui->comboBox_VehicleType_driveability->addItem(ctName, ctData);
        ui->comboBox_VehicleType_fuel->addItem(ctName, ctData);
        ui->comboBox_VehicleType_intro->addItem(ctName, ctData);
        ui->comboBox_VehicleType_luxury->addItem(ctName, ctData);
        ui->comboBox_VehicleType_overall->addItem(ctName, ctData);
        ui->comboBox_VehicleType_performance->addItem(ctName, ctData);
        ui->comboBox_VehicleType_power->addItem(ctName, ctData);
        ui->comboBox_VehicleType_quality->addItem(ctName, ctData);
        ui->comboBox_VehicleType_safety->addItem(ctName, ctData);
     }
    }
}
