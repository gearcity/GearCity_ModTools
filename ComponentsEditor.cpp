#include "ComponentsEditor.h"
#include "ui_ComponentsEditor.h"
#include <QtWidgets/QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtWidgets/QFileDialog>

ComponentsEditor::ComponentsEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentsEditor)
{
    ui->setupUi(this);

    cp_wsc = wsc;   

    ui->label_accessory_height->hide();
    ui->label_accessory_width->hide();
    ui->spinBox_accessories_width->hide();
    ui->spinBox_accessory_height->hide();

    connect(ui->table_aipop->horizontalHeader(),SIGNAL(sectionClicked(int)),
            this,SLOT(sectionDoubleClickedSlot(int)));

    localeManager = 0;
}

ComponentsEditor::~ComponentsEditor()
{
    delete ui;
}



void ComponentsEditor::on_button_frame_add_clicked()
{

    ComponentsManager::ChassisFrameComps cf;
        cf.selectionIndex = ui->spinner_frame_ID->value();
        cf.name = ui->lineEdit_frame_name->text();
        cf.picture = ui->combo_frame_picture->currentText();
        cf.about = ui->textbox_frame_about->toPlainText();
        cf.year = ui->spinner_frame_year->value();
        cf.death = ui->spinner_frame_stopYear->value();
        cf.skill  = ui->spinner_frame_skillreq->value();
        cf.str  = ui->spinner_frame_strength->value();
        cf.performance = ui->spinner_frame_performance->value();
        cf.safety = ui->spinner_frame_safety->value();
        cf.durability = ui->spinner_frame_durability->value();
        cf.weight = ui->spinner_frame_weight->value();
        cf.design = ui->spinner_frame_design->value();
        cf.manu = ui->spinner_frame_manufacturer->value();
        cf.costs = ui->spinner_frame_costs->value();
        cf.popindex = ui->spinner_frames_AIPop->value();
        cf.localName = ui->check_frame_name_localized->isChecked();
        cf.localDescription = ui->checkbox_frame_about_localize->isChecked();
        cf.localPara = 0;

        if(!cf.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < frames.size(); i++)
        {
            if(frames[i].name == cf.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(cf.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(cf.selectionIndex,"Frames-"+cf.name);
        frames.push_back(cf);

        fillFrameTable();
}

void ComponentsEditor::fillFrameTable()
{
    ui->table_frame->clearContents();
    ui->table_frame->setRowCount(frames.size());

    ui->table_frame->setSortingEnabled(false);


    for(int i = 0; i < frames.size(); i++)
    {
            ui->table_frame->setItem(i,0,new QTableWidgetItem(
                                         QString::number(frames[i].selectionIndex)));
            ui->table_frame->setItem(i,1,new QTableWidgetItem(
                                             QString::number(frames[i].year)));
            ui->table_frame->setItem(i,2,new QTableWidgetItem(
                                             QString::number(frames[i].death)));
            if(frames[i].localName && localeManager != 0)
                 ui->table_frame->setItem(i,3,new QTableWidgetItem(localeManager->getWord(frames[i].name.toInt())));
            else
                ui->table_frame->setItem(i,3,new QTableWidgetItem(frames[i].name));

            ui->table_frame->setItem(i,4,new QTableWidgetItem(
                                         QString::number(frames[i].skill)));
    }

    ui->table_frame->setSortingEnabled(true);



    fillAIPopComboData();

}

void ComponentsEditor::on_table_frame_cellClicked(int row, int column)
{
    QString componenetName = ui->table_frame->item(row,3)->text();
    int selectID = ui->table_frame->item(row,0)->text().toInt();

    for(int i = 0; i < frames.size(); i++)
    {

        QString frameName =frames[i].name;

        if(frames[i].localName && localeManager != 0)
            frameName = localeManager->getWord(frames[i].name.toInt());

         if(frameName == componenetName && frames[i].selectionIndex == selectID)
         {
             ComponentsManager::ChassisFrameComps cf = frames[i];
             ui->spinner_frame_ID->setValue(cf.selectionIndex);
             ui->lineEdit_frame_name->setText(cf.name);
             ui->combo_frame_picture->setEditText(cf.picture);
             ui->textbox_frame_about->setPlainText(cf.about);
             ui->spinner_frame_year->setValue(cf.year);
             ui->spinner_frame_stopYear->setValue(cf.death);
             ui->spinner_frame_skillreq->setValue(cf.skill);
             ui->spinner_frame_strength->setValue(cf.str);
             ui->spinner_frame_performance->setValue(cf.performance);
             ui->spinner_frame_safety->setValue(cf.safety);
             ui->spinner_frame_durability->setValue(cf.durability);
             ui->spinner_frame_weight->setValue(cf.weight);
             ui->spinner_frame_design->setValue(cf.design);
             ui->spinner_frame_manufacturer->setValue(cf.manu);
             ui->spinner_frame_costs->setValue(cf.costs);
             ui->spinner_frames_AIPop->setValue(cf.popindex);
             ui->check_frame_name_localized->setChecked(cf.localName);
             ui->checkbox_frame_about_localize->setChecked(cf.localDescription);
             break;
         }

    }
}

void ComponentsEditor::on_button_frame_remove_clicked()
{
    for(int i = 0; i < frames.size(); i++)
    {

         if(frames[i].name == ui->lineEdit_frame_name->text() &&
                 frames[i].selectionIndex == ui->spinner_frame_ID->value())
         {
             selectorIDMap.remove(frames[i].selectionIndex);
             frames.remove(i);
             break;

         }
    }

    fillFrameTable();
}


void ComponentsEditor::on_button_suspension_add_clicked()
{
    ComponentsManager::SuspensionComps  cs;
        cs.selectionIndex = ui->spinner_suspension_ID->value();
        cs.name = ui->lineEdit_suspension_name->text();
        cs.picture = ui->comboBox_suspension_picture->currentText();
        cs.about = ui->textEdit_suspension_about->toPlainText();
        cs.year = ui->spinBox_suspension_year->value();
        cs.death = ui->spinBox_suspension_endyear->value();
        cs.skill  = ui->spinner_suspension_skillreq->value();
        cs.steering  = ui->spinner_suspension_steering->value();
        cs.performance = ui->spinner_suspension_performance->value();
        cs.braking = ui->spinner_suspension_braking->value();
        cs.comfort = ui->spinner_suspension_comfort->value();
        cs.durability = ui->spinner_suspension_durability->value();
        cs.design = ui->spinner_suspension_design->value();
        cs.manu = ui->spinner_suspension_manufacturer->value();
        cs.costs = ui->spinner_suspension_costs->value();
        cs.popindex = ui->spinner_suspension_AIPop->value();
        cs.localName = ui->checkBox_suspension_name_localize->isChecked();
        cs.localDescription = ui->checkBox_suspension_about_localized->isChecked();
        cs.localPara = 0;

        if(!cs.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < suspensions.size(); i++)
        {
            if(suspensions[i].name == cs.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(cs.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(cs.selectionIndex,"Suspension-"+cs.name);
        suspensions.push_back(cs);

        fillSuspensionTable();
}

void ComponentsEditor::fillSuspensionTable()
{
    ui->table_suspension->clearContents();
    ui->table_suspension->setRowCount(suspensions.size());
    ui->table_suspension->setSortingEnabled(false);


    for(int i = 0; i < suspensions.size(); i++)
    {
            ui->table_suspension->setItem(i,0,new QTableWidgetItem(
                                         QString::number(suspensions[i].selectionIndex)));
            ui->table_suspension->setItem(i,1,new QTableWidgetItem(
                                             QString::number(suspensions[i].year)));
            ui->table_suspension->setItem(i,2,new QTableWidgetItem(
                                             QString::number(suspensions[i].death)));
            if(suspensions[i].localName && localeManager != 0)
                ui->table_suspension->setItem(i,3,new QTableWidgetItem(localeManager->getWord(suspensions[i].name.toInt())));
            else
                ui->table_suspension->setItem(i,3,new QTableWidgetItem(suspensions[i].name));

            ui->table_suspension->setItem(i,4,new QTableWidgetItem(
                                         QString::number(suspensions[i].skill)));
    }

    ui->table_suspension->setSortingEnabled(true);


    fillAIPopComboData();
}

void ComponentsEditor::on_button_suspension_remove_clicked()
{
    for(int i = 0; i < suspensions.size(); i++)
    {

         if(suspensions[i].name == ui->lineEdit_suspension_name->text() &&
                 suspensions[i].selectionIndex == ui->spinner_suspension_ID->value())
         {
             selectorIDMap.remove(suspensions[i].selectionIndex);
             suspensions.remove(i);
             break;
         }
    }

    fillSuspensionTable();
}

void ComponentsEditor::on_table_suspension_cellClicked(int row, int column)
{

    QString componenetName = ui->table_suspension->item(row,3)->text();
    int selectID = ui->table_suspension->item(row,0)->text().toInt();

    for(int i = 0; i < suspensions.size(); i++)
    {
         QString suspensionsName =suspensions[i].name;
         if(suspensions[i].localName && localeManager != 0)
             suspensionsName = localeManager->getWord(suspensions[i].name.toInt());


         if(suspensionsName == componenetName && suspensions[i].selectionIndex == selectID)
         {
             ComponentsManager::SuspensionComps  cs = suspensions[i];
             ui->spinner_suspension_ID->setValue(cs.selectionIndex);
             ui->lineEdit_suspension_name->setText(cs.name);
             ui->comboBox_suspension_picture->setEditText(cs.picture);
             ui->textEdit_suspension_about->setPlainText(cs.about);
             ui->spinBox_suspension_year->setValue(cs.year);
             ui->spinBox_suspension_endyear->setValue(cs.death);
             ui->spinner_suspension_skillreq->setValue(cs.skill);
             ui->spinner_suspension_steering->setValue(cs.steering);
             ui->spinner_suspension_performance->setValue(cs.performance);
             ui->spinner_suspension_braking->setValue(cs.braking);
             ui->spinner_suspension_comfort->setValue(cs.comfort);
             ui->spinner_suspension_durability->setValue(cs.durability);
             ui->spinner_suspension_design->setValue(cs.design);
             ui->spinner_suspension_manufacturer->setValue(cs.manu);
             ui->spinner_suspension_costs->setValue(cs.costs);
             ui->spinner_suspension_AIPop->setValue(cs.popindex);
             ui->checkBox_suspension_name_localize->setChecked(cs.localName);
             ui->checkBox_suspension_about_localized->setChecked(cs.localDescription);
             break;
         }

    }
}

void ComponentsEditor::on_button_drivertrain_add_clicked()
{
    ComponentsManager::DriveTrainComps   cd;
        cd.selectionIndex = ui->spinBox_drivetrain_id->value();
        cd.name = ui->lineEdit_drivetrain_name->text();
        cd.picture = ui->comboBox_drivertrain_picture->currentText();
        cd.about = ui->textEdit_drivetrain_about->toPlainText();
        cd.year = ui->spinner_drivetrain_year->value();
        cd.death = ui->spinner_drivetrain_endyear->value();
        cd.skill  = ui->spinner_drivetrain_skillreq->value();
        cd.ridePerformance  = ui->spinner_drivetrain_rideperformance->value();
        cd.rideSteering = ui->spinner_drivetrain_steering->value();
        cd.carPerformance = ui->spinner_drivetrain_carperformance->value();
        cd.duriblity = ui->spinner_drivetrain_durability->value();
        cd.weight = ui->spinner_drivetrain_weight->value();
        cd.engineLength = ui->spinner_drivetrain_enginelength->value();
        cd.engineWidth = ui->spinner_drivetrain_enginewidth->value();
        cd.design = ui->spinner_drivetrain_design->value();
        cd.manu = ui->spinner_drivetrain_manufacturing->value();
        cd.costs = ui->spinner_drivetrain_costs->value();
        cd.popindex = ui->spinner_drivetrain_aipop->value();
        cd.localName = ui->checkBox_drivetrain_name_localized->isChecked();
        cd.localDescription = ui->checkBox_drivetrain_about_localized->isChecked();
        cd.localPara = 0;

        if(!cd.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < drivetrains.size(); i++)
        {
            if(drivetrains[i].name == cd.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(cd.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(cd.selectionIndex,"Drivetrain-"+cd.name);
        drivetrains.push_back(cd);

        fillDrivetrainTable();
}


void ComponentsEditor::fillDrivetrainTable()
{
    ui->table_drivetrain->clearContents();
    ui->table_drivetrain->setRowCount(drivetrains.size());

    ui->table_drivetrain->setSortingEnabled(false);


    for(int i = 0; i < drivetrains.size(); i++)
    {
            ui->table_drivetrain->setItem(i,0,new QTableWidgetItem(
                                         QString::number(drivetrains[i].selectionIndex)));
            ui->table_drivetrain->setItem(i,1,new QTableWidgetItem(
                                             QString::number(drivetrains[i].year)));
            ui->table_drivetrain->setItem(i,2,new QTableWidgetItem(
                                             QString::number(drivetrains[i].death)));

            if(drivetrains[i].localName && localeManager != 0)
                ui->table_drivetrain->setItem(i,3,new QTableWidgetItem(localeManager->getWord(drivetrains[i].name.toInt())));
            else
                ui->table_drivetrain->setItem(i,3,new QTableWidgetItem(drivetrains[i].name));

            ui->table_drivetrain->setItem(i,4,new QTableWidgetItem(
                                         QString::number(drivetrains[i].skill)));
    }

    ui->table_drivetrain->setSortingEnabled(true);


    fillAIPopComboData();
}



void ComponentsEditor::on_button_drivetrain_remove_clicked()
{
    for(int i = 0; i < drivetrains.size(); i++)
    {

         if(drivetrains[i].name == ui->lineEdit_drivetrain_name->text() &&
                 drivetrains[i].selectionIndex == ui->spinBox_drivetrain_id->value())
         {
             selectorIDMap.remove(drivetrains[i].selectionIndex);
             drivetrains.remove(i);
             break;
         }
    }

    fillDrivetrainTable();
}

void ComponentsEditor::on_table_drivetrain_cellClicked(int row, int column)
{

    QString componenetName = ui->table_drivetrain->item(row,3)->text();
    int selectID = ui->table_drivetrain->item(row,0)->text().toInt();

    for(int i = 0; i < drivetrains.size(); i++)
    {
        QString drivetrainsName =drivetrains[i].name;

        if(drivetrains[i].localName && localeManager != 0)
            drivetrainsName = localeManager->getWord(drivetrains[i].name.toInt());

         if(drivetrainsName == componenetName && drivetrains[i].selectionIndex == selectID)
         {
             ComponentsManager::DriveTrainComps   cd = drivetrains[i];
             ui->spinBox_drivetrain_id->setValue(cd.selectionIndex);
             ui->lineEdit_drivetrain_name->setText(cd.name);
             ui->comboBox_drivertrain_picture->setEditText(cd.picture);
             ui->textEdit_drivetrain_about->setPlainText(cd.about);
             ui->spinner_drivetrain_year->setValue(cd.year);
             ui->spinner_drivetrain_endyear->setValue(cd.death);
             ui->spinner_drivetrain_skillreq->setValue(cd.skill);
             ui->spinner_drivetrain_rideperformance->setValue(cd.ridePerformance);
             ui->spinner_drivetrain_steering->setValue(cd.rideSteering);
             ui->spinner_drivetrain_carperformance->setValue(cd.carPerformance);
             ui->spinner_drivetrain_durability->setValue(cd.duriblity);
             ui->spinner_drivetrain_weight->setValue(cd.weight);
             ui->spinner_drivetrain_enginelength->setValue(cd.engineLength);
             ui->spinner_drivetrain_enginewidth->setValue(cd.engineWidth);
             ui->spinner_drivetrain_design->setValue(cd.design);
             ui->spinner_drivetrain_manufacturing->setValue(cd.manu);
             ui->spinner_drivetrain_costs->setValue(cd.costs);
             ui->spinner_drivetrain_aipop->setValue(cd.popindex);
             ui->checkBox_drivetrain_name_localized->setChecked(cd.localName);
             ui->checkBox_drivetrain_about_localized->setChecked(cd.localDescription);
             break;
         }

    }
}


void ComponentsEditor::on_button_englayout_cylrestrict_add_clicked()
{
    if(currentCylRestrictions.contains(ui->comboBox_englayout_cylrestrict->currentText()))
    {
        QMessageBox::critical(this,"Error",
                              QString("This Cylinder has allready been added."));
    }
    else
    {
        currentCylRestrictions.push_back(ui->comboBox_englayout_cylrestrict->currentText());
        ui->list_englayout_cylrestrict->clear();
        ui->list_englayout_cylrestrict->addItems(currentCylRestrictions);

        if(ui->comboBox_englayout_cylrestrict->findText(
                    ui->comboBox_englayout_cylrestrict->currentText()) == -1)
        {
            ui->comboBox_englayout_cylrestrict->addItem(
                        ui->comboBox_englayout_cylrestrict->currentText());
        }
    }
}



void ComponentsEditor::on_button_englayout_cylrestrict_remove_clicked()
{
    if(currentCylRestrictions.contains(ui->comboBox_englayout_cylrestrict->currentText()))
    {
        currentCylRestrictions.removeAll(ui->comboBox_englayout_cylrestrict->currentText());
        ui->list_englayout_cylrestrict->clear();
        ui->list_englayout_cylrestrict->addItems(currentCylRestrictions);
    }

}

void ComponentsEditor::on_list_englayout_cylrestrict_currentTextChanged(const QString &currentText)
{
    ui->comboBox_englayout_cylrestrict->setEditText(currentText);
}

void ComponentsEditor::on_button_englayout_fuelrestrict_add_clicked()
{
    if(currentFuelRestrictions.contains(ui->comboBox_englayout_fuelrestrict->currentText()))
    {
        QMessageBox::critical(this,"Error",
                              QString("This Fuel has allready been added."));
    }
    else
    {
        currentFuelRestrictions.push_back(ui->comboBox_englayout_fuelrestrict->currentText());
        ui->list_englayout_fuelrestrict->clear();
        ui->list_englayout_fuelrestrict->addItems(currentFuelRestrictions);

        if(ui->comboBox_englayout_fuelrestrict->findText(
                    ui->comboBox_englayout_fuelrestrict->currentText()) == -1)
        {
            ui->comboBox_englayout_fuelrestrict->addItem(
                        ui->comboBox_englayout_fuelrestrict->currentText());
        }
    }
}

void ComponentsEditor::on_button_englayout_fuelrestrict_remove_clicked()
{
    if(currentFuelRestrictions.contains(ui->comboBox_englayout_fuelrestrict->currentText()))
    {
        currentFuelRestrictions.removeAll(ui->comboBox_englayout_fuelrestrict->currentText());
        ui->list_englayout_fuelrestrict->clear();
        ui->list_englayout_fuelrestrict->addItems(currentFuelRestrictions);
    }
}

void ComponentsEditor::on_list_englayout_fuelrestrict_currentTextChanged(const QString &currentText)
{
    ui->comboBox_englayout_fuelrestrict->setEditText(currentText);
}

void ComponentsEditor::on_button_englayout_inductrestrict_add_clicked()
{
    if(currentInductionRestrictions.contains(ui->combo_englayout_inductrestrict->currentText()))
    {
        QMessageBox::critical(this,"Error",
                              QString("This Induction has allready been added."));
    }
    else
    {
        currentInductionRestrictions.push_back(ui->combo_englayout_inductrestrict->currentText());
        ui->list_englayout_inducrestrict->clear();
        ui->list_englayout_inducrestrict->addItems(currentInductionRestrictions);

        if(ui->combo_englayout_inductrestrict->findText(
                    ui->combo_englayout_inductrestrict->currentText()) == -1)
        {
            ui->combo_englayout_inductrestrict->addItem(
                        ui->combo_englayout_inductrestrict->currentText());
        }
    }
}

void ComponentsEditor::on_button_englayout_inductrestrict_remove_clicked()
{
    if(currentInductionRestrictions.contains(ui->combo_englayout_inductrestrict->currentText()))
    {
        currentInductionRestrictions.removeAll(ui->combo_englayout_inductrestrict->currentText());
        ui->list_englayout_inducrestrict->clear();
        ui->list_englayout_inducrestrict->addItems(currentInductionRestrictions);
    }
}

void ComponentsEditor::on_list_englayout_inducrestrict_currentTextChanged(const QString &currentText)
{
    ui->combo_englayout_inductrestrict->setEditText(currentText);
}

void ComponentsEditor::on_button_englayout_add_clicked()
{
    ComponentsManager::EngineLayoutComps el;
        el.selectionIndex = ui->spinner_englayout_ID->value();
        el.name = ui->lineEdit_englayout_name->text();
        el.picture = ui->comboBox_englayout_picture->currentText();
        el.about = ui->textEdit_englayout_about->toPlainText();
        el.year = ui->spinBox_englayout_year->value();
        el.death = ui->spinBox_englayout_EndYear->value();
        el.skill  = ui->spinBox_englayout_skillreqs->value();

        el.valve  = ui->spinBox_englayout_valveGroup->value();
        el.useDisplacement = ui->checkBox_englayout_usedisplayment->isChecked();
        el.cylinderLengthArrangment = ui->spinBox_englayout_layoutArangment->value();

        el.designCosts = ui->spinBox_englayout_designCosts->value();
        el.cost = ui->spinBox_englayout_unitCost->value();
        el.design = ui->spinBox_englayout_design->value();
        el.manu = ui->spinBox_englayout_manu->value();
        el.reliability = ui->spinBox_englayout_reliability->value();
        el.weight = ui->spinBox_englayout_weight->value();
        el.finish = ui->spinBox_englayout_finish->value();
        el.width = ui->spinBox_englayout_width->value();
        el.length = ui->spinBox_englayout_length->value();
        el.smooth = ui->spinBox_englayout_smooth->value();
        el.popIndex   = ui->spinBox_englayout_aipop->value();
        el.layoutpower = ui->spinBox_englayout_power->value();
        el.layoutFuel = ui->spinBox_englayout_fuel->value();

        el.localName = ui->checkBox_englayout_name_localized->isChecked();
        el.localDescription = ui->checkBox_englayout_about_localized->isChecked();
        el.localPara = ui->checkBox_enginelayout_restriction_Localization->isChecked();

        el.turbine = ui->checkBox_enginelayout_turbine->isChecked();

        el.supportCyl = currentCylRestrictions;
        el.supportFuel = currentFuelRestrictions;
        el.supportInduct = currentInductionRestrictions;

        if(!el.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < layouts.size(); i++)
        {
            if(layouts[i].name == el.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(el.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }

        if(el.supportCyl.empty())
        {
            QMessageBox::critical(this,"Error",
                                  QString("Layout must support a cylinder sub component!"));
            return;
        }

        if(el.supportFuel.empty())
        {
            QMessageBox::critical(this,"Error",
                                  QString("Layout must support a fuel sub component!"));
            return;
        }

        if(el.supportInduct.empty())
        {
            QMessageBox::critical(this,"Error",
                                  QString("Layout must support a induction sub component!"));
            return;
        }


        selectorIDMap.insert(el.selectionIndex,"EngineLayout-"+el.name);
        layouts.push_back(el);

        fillEngineLayoutTable();
}


void ComponentsEditor::fillEngineLayoutTable()
{
    ui->table_englayout->clearContents();
    ui->table_englayout->setRowCount(layouts.size());

    ui->table_englayout->setSortingEnabled(false);


    for(int i = 0; i < layouts.size(); i++)
    {
            ui->table_englayout->setItem(i,0,new QTableWidgetItem(
                                         QString::number(layouts[i].selectionIndex)));
            ui->table_englayout->setItem(i,1,new QTableWidgetItem(
                                             QString::number(layouts[i].year)));
            ui->table_englayout->setItem(i,2,new QTableWidgetItem(
                                             QString::number(layouts[i].death)));
            if(layouts[i].localName && localeManager != 0)
                ui->table_englayout->setItem(i,3,new QTableWidgetItem(localeManager->getWord(layouts[i].name.toInt())));
            else
                ui->table_englayout->setItem(i,3,new QTableWidgetItem(layouts[i].name));

            ui->table_englayout->setItem(i,4,new QTableWidgetItem(
                                         QString::number(layouts[i].skill)));
            ui->table_englayout->setItem(i,5,new QTableWidgetItem(
                                         QString::number(layouts[i].valve)));
    }

    ui->table_englayout->setSortingEnabled(true);


    fillAIPopComboData();
}

void ComponentsEditor::on_button_englayout_remove_clicked()
{
    for(int i = 0; i < layouts.size(); i++)
    {

         if(layouts[i].name == ui->lineEdit_englayout_name->text() &&
                 layouts[i].selectionIndex == ui->spinner_englayout_ID->value())
         {
             selectorIDMap.remove(layouts[i].selectionIndex);
             layouts.remove(i);
             break;
         }
    }

    fillEngineLayoutTable();
}

void ComponentsEditor::on_table_englayout_cellClicked(int row, int column)
{
    QString componenetName = ui->table_englayout->item(row,3)->text();
    int selectID = ui->table_englayout->item(row,0)->text().toInt();

    for(int i = 0; i < layouts.size(); i++)
    {
        QString layoutName =layouts[i].name;

        if(layouts[i].localName && localeManager != 0)
            layoutName = localeManager->getWord(layouts[i].name.toInt());



         if(layoutName == componenetName && layouts[i].selectionIndex == selectID)
         {
             ComponentsManager::EngineLayoutComps el = layouts[i];
             ui->spinner_englayout_ID->setValue(el.selectionIndex);
             ui->lineEdit_englayout_name->setText(el.name);
             ui->comboBox_englayout_picture->setEditText(el.picture);
             ui->textEdit_englayout_about->setPlainText(el.about);
             ui->spinBox_englayout_year->setValue(el.year);
             ui->spinBox_englayout_EndYear->setValue(el.death);
             ui->spinBox_englayout_skillreqs->setValue(el.skill);

             ui->spinBox_englayout_valveGroup->setValue(el.valve);
             ui->checkBox_englayout_usedisplayment->setChecked(el.useDisplacement);
             ui->spinBox_englayout_layoutArangment->setValue(el.cylinderLengthArrangment);

             ui->spinBox_englayout_designCosts->setValue(el.designCosts);
             ui->spinBox_englayout_unitCost->setValue(el.cost);
             ui->spinBox_englayout_design->setValue(el.design);
             ui->spinBox_englayout_manu->setValue(el.manu);
             ui->spinBox_englayout_reliability->setValue(el.reliability);
             ui->spinBox_englayout_weight->setValue(el.weight);
             ui->spinBox_englayout_finish->setValue(el.finish);
             ui->spinBox_englayout_width->setValue(el.width);
             ui->spinBox_englayout_length->setValue(el.length);
             ui->spinBox_englayout_smooth->setValue(el.smooth);
             ui->spinBox_englayout_aipop->setValue(el.popIndex);
             ui->spinBox_englayout_power->setValue(el.layoutpower);
             ui->spinBox_englayout_fuel->setValue(el.layoutFuel);

             ui->checkBox_englayout_name_localized->setChecked(el.localName);
             ui->checkBox_englayout_about_localized->setChecked(el.localDescription);
             ui->checkBox_enginelayout_restriction_Localization->setChecked(el.localPara);

             ui->checkBox_enginelayout_turbine->setChecked(el.turbine);

             currentCylRestrictions = el.supportCyl;
             currentFuelRestrictions = el.supportFuel;
             currentInductionRestrictions = el.supportInduct;

             ui->list_englayout_cylrestrict->clear();
             ui->list_englayout_fuelrestrict->clear();
             ui->list_englayout_inducrestrict->clear();

             ui->list_englayout_cylrestrict->addItems(currentCylRestrictions);
             ui->list_englayout_fuelrestrict->addItems(currentFuelRestrictions);
             ui->list_englayout_inducrestrict->addItems(currentInductionRestrictions);

             break;
         }

    }
}

void ComponentsEditor::on_button_cyl_add_clicked()
{
    ComponentsManager::EngineCylinderComps  ec;
        ec.selectionIndex = ui->spinBox_cyl_id->value();
        ec.name = ui->lineEdit_cyl_name->text();
        ec.picture = ui->comboBox_cyl_picture->currentText();
        ec.about = ui->textEdit_cyl_about->toPlainText();
        ec.year = ui->spinBox_cyl_year->value();
        ec.death = ui->spinBox_cyl_endyear->value();
        ec.skill  = ui->spinner_cyl_skill->value();
        ec.cylCosts  = ui->spinner_cyl_costs->value();
        ec.designCosts = ui->spinner_cyl_designcost->value();
        ec.design = ui->spinner_cyl_design->value();
        ec.manu = ui->spinner_cyl_manu->value();
        ec.power = ui->spinner_cyl_power->value();
        ec.fuel = ui->spinner_cyl_fuel->value();
        ec.reliability = ui->spinner_cyl_reliability->value();
        ec.weight = ui->spinner_cyl_weight->value();
        ec.finish = ui->spinner_cyl_finish->value();
        ec.smooth = ui->spinner_cyl_smoothness->value();
        ec.popIndex = ui->spinner_cyl_aipop->value();
        ec.numberCyl = ui->spinner_cyl_cylindercount->value();
        ec.localName = ui->checkBox_cyl_name_localized->isChecked();
        ec.localDescription = ui->checkBox_cyl_about_localized->isChecked();
        ec.localPara = 0;

        if(!ec.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < cylinders.size(); i++)
        {
            if(cylinders[i].name == ec.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(ec.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(ec.selectionIndex,"EngineCylinder-"+ec.name);
        cylinders.push_back(ec);

        fillCylinderTable();

        if(ui->comboBox_englayout_cylrestrict->findText(ec.name) == -1)
        {
            ui->comboBox_englayout_cylrestrict->addItem(ec.name);
        }
}


void ComponentsEditor::fillCylinderTable()
{
    ui->table_cyl->clearContents();
    ui->table_cyl->setRowCount(cylinders.size());

    ui->table_cyl->setSortingEnabled(false);


    for(int i = 0; i < cylinders.size(); i++)
    {
            ui->table_cyl->setItem(i,0,new QTableWidgetItem(
                                         QString::number(cylinders[i].selectionIndex)));
            ui->table_cyl->setItem(i,1,new QTableWidgetItem(
                                             QString::number(cylinders[i].year)));
            ui->table_cyl->setItem(i,2,new QTableWidgetItem(
                                             QString::number(cylinders[i].death)));

            if(cylinders[i].localName && localeManager != 0)
                ui->table_cyl->setItem(i,3,new QTableWidgetItem(localeManager->getWord(cylinders[i].name.toInt())));
            else
                ui->table_cyl->setItem(i,3,new QTableWidgetItem(cylinders[i].name));

            ui->table_cyl->setItem(i,4,new QTableWidgetItem(
                                         QString::number(cylinders[i].skill)));

    }

    ui->table_cyl->setSortingEnabled(true);


    fillAIPopComboData();
}



void ComponentsEditor::on_button_cyl_remove_clicked()
{
    for(int i = 0; i < cylinders.size(); i++)
    {

         if(cylinders[i].name == ui->lineEdit_cyl_name->text() &&
                 cylinders[i].selectionIndex == ui->spinBox_cyl_id->value())
         {
             selectorIDMap.remove(cylinders[i].selectionIndex);
             cylinders.remove(i);
             break;
         }
    }

    fillCylinderTable();
}



void ComponentsEditor::on_table_cyl_cellClicked(int row, int column)
{
    QString componenetName = ui->table_cyl->item(row,3)->text();
    int selectID = ui->table_cyl->item(row,0)->text().toInt();

    for(int i = 0; i < cylinders.size(); i++)
    {
        QString cylindersName =cylinders[i].name;

        if(cylinders[i].localName && localeManager != 0)
            cylindersName = localeManager->getWord(cylinders[i].name.toInt());


         if(cylindersName == componenetName && cylinders[i].selectionIndex == selectID)
         {
             ComponentsManager::EngineCylinderComps  ec = cylinders[i];
             ui->spinBox_cyl_id->setValue(ec.selectionIndex);
             ui->lineEdit_cyl_name->setText(ec.name);
             ui->comboBox_cyl_picture->setEditText(ec.picture);
             ui->textEdit_cyl_about->setPlainText(ec.about);
             ui->spinBox_cyl_year->setValue(ec.year);
             ui->spinBox_cyl_endyear->setValue(ec.death);
             ui->spinner_cyl_skill->setValue(ec.skill);
             ui->spinner_cyl_costs->setValue(ec.cylCosts);
             ui->spinner_cyl_designcost->setValue(ec.designCosts);
             ui->spinner_cyl_design->setValue(ec.design);
             ui->spinner_cyl_manu->setValue(ec.manu);
             ui->spinner_cyl_power->setValue(ec.power);
             ui->spinner_cyl_fuel->setValue(ec.fuel);
             ui->spinner_cyl_reliability->setValue(ec.reliability);
             ui->spinner_cyl_weight->setValue(ec.weight);
             ui->spinner_cyl_finish->setValue(ec.finish);
             ui->spinner_cyl_smoothness->setValue(ec.smooth);
             ui->spinner_cyl_aipop->setValue(ec.popIndex);
             ui->spinner_cyl_cylindercount->setValue(ec.numberCyl);
             ui->checkBox_cyl_name_localized->setChecked(ec.localName);
             ui->checkBox_cyl_about_localized->setChecked(ec.localDescription);

             break;
         }

    }
}



void ComponentsEditor::on_button_fuel_add_clicked()
{

    ComponentsManager::EngineFuelComps   ef;
        ef.selectionIndex = ui->spinner_fuel_ID->value();
        ef.name = ui->lineEdit_fuel_name->text();
        ef.picture = ui->comboBox_fuel_picture->currentText();
        ef.about = ui->textEdit_fuel_about->toPlainText();
        ef.year = ui->spinner_fuel_year->value();
        ef.death = ui->spinner_fuel_endyear->value();
        ef.skill  = ui->spinner_fuel_skill->value();
        ef.costs  = ui->spinner_fuel_cost->value();
        ef.designCost = ui->spinner_fuel_designcost->value();
        ef.design = ui->spinner_fuel_design->value();
        ef.manu = ui->spinner_fuel_manu->value();
        ef.power = ui->spinner_fuel_power->value();
        ef.fuel = ui->spinner_fuel_fuel->value();
        ef.reliability = ui->spinner_fuel_reliability->value();
        ef.weight = ui->spinner_fuel_weight->value();
        ef.finish = ui->spinner_fuel_finish->value();
        ef.smooth = ui->spinner_fuel_smooth->value();
        ef.rpm = ui->spinner_fuel_rpm->value();
        ef.popIndex = ui->spinner_fuel_aipop->value();
        ef.localName = ui->checkBox_fuel_name_localized->isChecked();
        ef.localDescription = ui->checkBox_fuel_about_localized->isChecked();
        ef.popLink = ui->lineEdit_fuel_popLink->text();
        ef.localPara = 0;


        if(!ef.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < fuels.size(); i++)
        {
            if(fuels[i].name == ef.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(ef.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(ef.selectionIndex,"EngineFuel-"+ef.name);
        fuels.push_back(ef);

        fillFuelTable();

        if(ui->comboBox_englayout_fuelrestrict->findText(ef.name) == -1)
        {
            ui->comboBox_englayout_fuelrestrict->addItem(ef.name);
        }
}


void ComponentsEditor::on_button_fuel_remove_clicked()
{
    for(int i = 0; i < fuels.size(); i++)
    {

         if(fuels[i].name == ui->lineEdit_fuel_name->text() &&
                 fuels[i].selectionIndex == ui->spinner_fuel_ID->value())
         {
             selectorIDMap.remove(fuels[i].selectionIndex);
             fuels.remove(i);
             break;
         }
    }

    fillFuelTable();
}


void ComponentsEditor::fillFuelTable()
{
    ui->table_fuel->clearContents();
    ui->table_fuel->setRowCount(fuels.size());
    ui->table_fuel->setSortingEnabled(false);


    for(int i = 0; i < fuels.size(); i++)
    {
            ui->table_fuel->setItem(i,0,new QTableWidgetItem(
                                         QString::number(fuels[i].selectionIndex)));
            ui->table_fuel->setItem(i,1,new QTableWidgetItem(
                                             QString::number(fuels[i].year)));
            ui->table_fuel->setItem(i,2,new QTableWidgetItem(
                                             QString::number(fuels[i].death)));
            if(fuels[i].localName && localeManager != 0)
              ui->table_fuel->setItem(i,3,new QTableWidgetItem(localeManager->getWord(fuels[i].name.toInt())));
            else
              ui->table_fuel->setItem(i,3,new QTableWidgetItem(fuels[i].name));

            ui->table_fuel->setItem(i,4,new QTableWidgetItem(
                                         QString::number(fuels[i].skill)));

    }

    ui->table_fuel->setSortingEnabled(true);


    fillAIPopComboData();
}


void ComponentsEditor::on_table_fuel_cellClicked(int row, int column)
{
    QString componenetName = ui->table_fuel->item(row,3)->text();
    int selectID = ui->table_fuel->item(row,0)->text().toInt();

    for(int i = 0; i < fuels.size(); i++)
    {
        QString fuelsName =fuels[i].name;

        if(fuels[i].localName && localeManager != 0)
            fuelsName = localeManager->getWord(fuels[i].name.toInt());

         if(fuelsName == componenetName && fuels[i].selectionIndex == selectID)
         {
             ComponentsManager::EngineFuelComps   ef = fuels[i];
             ui->spinner_fuel_ID->setValue(ef.selectionIndex);
             ui->lineEdit_fuel_name->setText(ef.name);
             ui->comboBox_fuel_picture->setEditText(ef.picture);
             ui->textEdit_fuel_about->setPlainText(ef.about);
             ui->spinner_fuel_year->setValue(ef.year);
             ui->spinner_fuel_endyear->setValue(ef.death);
             ui->spinner_fuel_skill->setValue(ef.skill);
             ui->spinner_fuel_cost->setValue(ef.costs);
             ui->spinner_fuel_designcost->setValue(ef.designCost);
             ui->spinner_fuel_design->setValue(ef.design);
             ui->spinner_fuel_manu->setValue(ef.manu);
             ui->spinner_fuel_power->setValue(ef.power);
             ui->spinner_fuel_fuel->setValue(ef.fuel);
             ui->spinner_fuel_reliability->setValue(ef.reliability);
             ui->spinner_fuel_weight->setValue(ef.weight);
             ui->spinner_fuel_finish->setValue(ef.finish);
             ui->spinner_fuel_smooth->setValue(ef.smooth);
             ui->spinner_fuel_rpm->setValue(ef.rpm);
             ui->spinner_fuel_aipop->setValue(ef.popIndex);
             ui->checkBox_fuel_name_localized->setChecked(ef.localName);
             ui->checkBox_fuel_about_localized->setChecked(ef.localDescription);
             ui->lineEdit_fuel_popLink->setText(ef.popLink);

             break;
         }

    }
}

void ComponentsEditor::on_button_valve_add_clicked()
{
    ComponentsManager::EngineValveComps    ev;
        ev.group = ui->spinBox_valve_group->value();
        ev.selectionIndex = ui->spinBox_valve_id->value();
        ev.name = ui->lineEdit_valve_name->text();
        ev.picture = ui->comboBox_valve_picture->currentText();
        ev.about = ui->textEdit_valve_about->toPlainText();
        ev.year = ui->spinBox_valve_year->value();
        ev.death = ui->spinBox_valve_endyear->value();
        ev.skill  = ui->spinBox_valve_skill->value();
        ev.cost  = ui->spinBox_valve_costs->value();
        ev.designCost = ui->spinBox_valve_designcost->value();
        ev.design = ui->spinBox_valve_design->value();
        ev.manu = ui->spinBox_valve_manu->value();
        ev.size = ui->spinBox_valve_size->value();
        ev.power = ui->spinBox_Valve_Power->value();
        ev.fuel = ui->spinBox_valve_fuel->value();
        ev.reliability = ui->spinBox_valve_reliability->value();
        ev.weight = ui->spinBox_valve_weight->value();
        ev.finish = ui->spinBox_valve_finish->value();
        ev.smooth = ui->spinBox_valve_smooth->value();
        ev.rpm = ui->spinBox_valve_rpm->value();
        ev.popIndex = ui->spinBox_valve_aipop->value();
        ev.localName = ui->checkBox_valve_name_localize->isChecked();
        ev.localDescription = ui->checkBox_valve_about_localized->isChecked();
        ev.localPara = 0;


        if(!ev.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < valves.size(); i++)
        {
            if(valves[i].name == ev.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(ev.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(ev.selectionIndex,"EngineValve-"+ev.name);
        valves.push_back(ev);

        fillValveTable();
}

void ComponentsEditor::on_button_valve_remove_clicked()
{
    for(int i = 0; i < valves.size(); i++)
    {

         if(valves[i].name == ui->lineEdit_valve_name->text() &&
                 valves[i].selectionIndex == ui->spinBox_valve_id->value())
         {
             selectorIDMap.remove(valves[i].selectionIndex);
             valves.remove(i);
             break;
         }
    }

    fillValveTable();
}


void ComponentsEditor::fillValveTable()
{
    ui->table_valve->clearContents();
    ui->table_valve->setRowCount(valves.size());
    ui->table_valve->setSortingEnabled(false);

    for(int i = 0; i < valves.size(); i++)
    {
            ui->table_valve->setItem(i,0,new QTableWidgetItem(
                                         QString::number(valves[i].selectionIndex)));
            ui->table_valve->setItem(i,1,new QTableWidgetItem(
                                             QString::number(valves[i].year)));
            ui->table_valve->setItem(i,2,new QTableWidgetItem(
                                             QString::number(valves[i].death)));

            if(valves[i].localName && localeManager != 0)
                ui->table_valve->setItem(i,3,new QTableWidgetItem(localeManager->getWord(valves[i].name.toInt())));
            else
                ui->table_valve->setItem(i,3,new QTableWidgetItem(valves[i].name));

            ui->table_valve->setItem(i,4,new QTableWidgetItem(
                                         QString::number(valves[i].skill)));

    }

    ui->table_valve->setSortingEnabled(true);


    fillAIPopComboData();
}


void ComponentsEditor::on_table_valve_cellClicked(int row, int column)
{
    QString componenetName = ui->table_valve->item(row,3)->text();
    int selectID = ui->table_valve->item(row,0)->text().toInt();

    for(int i = 0; i < valves.size(); i++)
    {
        QString valvesName = valves[i].name;
        if(valves[i].localName && localeManager != 0)
            valvesName = localeManager->getWord(valves[i].name.toInt());

         if(valvesName == componenetName && valves[i].selectionIndex == selectID)
         {
             ComponentsManager::EngineValveComps    ev = valves[i];
             ui->spinBox_valve_group->setValue(ev.group);
             ui->spinBox_valve_id->setValue(ev.selectionIndex);
             ui->lineEdit_valve_name->setText(ev.name);
             ui->comboBox_valve_picture->setEditText(ev.picture);
             ui->textEdit_valve_about->setPlainText(ev.about);
             ui->spinBox_valve_year->setValue(ev.year);
             ui->spinBox_valve_endyear->setValue(ev.death);
             ui->spinBox_valve_skill->setValue(ev.skill);
             ui->spinBox_valve_costs->setValue(ev.cost);
             ui->spinBox_valve_designcost->setValue(ev.designCost);
             ui->spinBox_valve_design->setValue(ev.design);
             ui->spinBox_valve_manu->setValue(ev.manu);
             ui->spinBox_valve_size->setValue(ev.size);
             ui->spinBox_Valve_Power->setValue(ev.power);
             ui->spinBox_valve_fuel->setValue(ev.fuel);
             ui->spinBox_valve_reliability->setValue(ev.reliability);
             ui->spinBox_valve_weight->setValue(ev.weight);
             ui->spinBox_valve_finish->setValue(ev.finish);
             ui->spinBox_valve_smooth->setValue(ev.smooth);
             ui->spinBox_valve_rpm->setValue(ev.rpm);
             ui->spinBox_valve_aipop->setValue(ev.popIndex);
             ui->checkBox_valve_name_localize->setChecked(ev.localName);
             ui->checkBox_valve_about_localized->setChecked(ev.localDescription);

             break;
         }

    }
}

void ComponentsEditor::on_button_induct_add_clicked()
{
    ComponentsManager::EngineInductionComps ei;
        ei.selectionIndex = ui->spinBox_induct_id->value();
        ei.name = ui->lineEdit_induct_name->text();
        ei.picture = ui->comboBox_induct_picture->currentText();
        ei.about = ui->textEdit_induct_about->toPlainText();
        ei.year = ui->spinBox_induct_year->value();
        ei.death = ui->spinBox_induct_endyear->value();
        ei.skill  = ui->spinBox_induct_skill->value();
        ei.costs  = ui->spinBox_induct_cost->value();
        ei.designCosts = ui->spinBox_induct_designcosts->value();
        ei.design = ui->spinBox_induct_design->value();
        ei.manu = ui->spinBox_induct_manu->value();
        ei.fuel = ui->spinBox_induct_fuel->value();
        ei.power = ui->spinBox_induct_power->value();
        ei.reliability = ui->spinBox_induct_realiability->value();
        ei.weight = ui->spinBox_induct_weight->value();
        ei.finish = ui->spinBox_induct_finish->value();
        ei.popIndex = ui->spinBox_induct_aipop->value();
        ei.localName = ui->checkBox_induct_name_localize->isChecked();
        ei.localDescription = ui->checkBox_induct_about_localize->isChecked();
        ei.localPara = 0;


        if(!ei.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < inductions.size(); i++)
        {
            if(inductions[i].name == ei.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(ei.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(ei.selectionIndex,"EngineInduction-"+ei.name);
        inductions.push_back(ei);

        fillInductionTable();

        if(ui->combo_englayout_inductrestrict->findText(ei.name) == -1)
        {
            ui->combo_englayout_inductrestrict->addItem(ei.name);
        }
}

void ComponentsEditor::on_button_induct_remove_clicked()
{
    for(int i = 0; i < inductions.size(); i++)
    {

         if(inductions[i].name == ui->lineEdit_induct_name->text() &&
                 inductions[i].selectionIndex == ui->spinBox_induct_id->value())
         {
             selectorIDMap.remove(inductions[i].selectionIndex);
             inductions.remove(i);
             break;
         }
    }

    fillInductionTable();
}


void ComponentsEditor::fillInductionTable()
{
    ui->table_induction->clearContents();
    ui->table_induction->setRowCount(inductions.size());
    ui->table_induction->setSortingEnabled(false);


    for(int i = 0; i < inductions.size(); i++)
    {
            ui->table_induction->setItem(i,0,new QTableWidgetItem(
                                         QString::number(inductions[i].selectionIndex)));
            ui->table_induction->setItem(i,1,new QTableWidgetItem(
                                             QString::number(inductions[i].year)));
            ui->table_induction->setItem(i,2,new QTableWidgetItem(
                                             QString::number(inductions[i].death)));

            if(inductions[i].localName && localeManager != 0)
                ui->table_induction->setItem(i,3,new QTableWidgetItem(localeManager->getWord(inductions[i].name.toInt())));
            else
                ui->table_induction->setItem(i,3,new QTableWidgetItem(inductions[i].name));

            ui->table_induction->setItem(i,4,new QTableWidgetItem(
                                         QString::number(inductions[i].skill)));

    }
    ui->table_induction->setSortingEnabled(true);


    fillAIPopComboData();
}

void ComponentsEditor::on_table_induction_cellClicked(int row, int column)
{
    QString componenetName = ui->table_induction->item(row,3)->text();
    int selectID = ui->table_induction->item(row,0)->text().toInt();

    for(int i = 0; i < inductions.size(); i++)
    {
        QString inductionsName =inductions[i].name;

        if(inductions[i].localName && localeManager != 0)
            inductionsName = localeManager->getWord(inductions[i].name.toInt());

         if(inductionsName == componenetName && inductions[i].selectionIndex == selectID)
         {
             ComponentsManager::EngineInductionComps ei = inductions[i];
             ui->spinBox_induct_id->setValue(ei.selectionIndex);
             ui->lineEdit_induct_name->setText(ei.name);
             ui->comboBox_induct_picture->setEditText(ei.picture);
             ui->textEdit_induct_about->setPlainText(ei.about);
             ui->spinBox_induct_year->setValue(ei.year);
             ui->spinBox_induct_endyear->setValue(ei.death);
             ui->spinBox_induct_skill->setValue(ei.skill);
             ui->spinBox_induct_cost->setValue(ei.costs);
             ui->spinBox_induct_designcosts->setValue(ei.designCosts);
             ui->spinBox_induct_design->setValue(ei.design);
             ui->spinBox_induct_manu->setValue(ei.manu);
             ui->spinBox_induct_fuel->setValue(ei.fuel);
             ui->spinBox_induct_power->setValue(ei.power);
             ui->spinBox_induct_realiability->setValue(ei.reliability);
             ui->spinBox_induct_weight->setValue(ei.weight);
             ui->spinBox_induct_finish->setValue(ei.finish);
             ui->spinBox_induct_aipop->setValue(ei.popIndex);
             ui->checkBox_induct_name_localize->setChecked(ei.localName);
             ui->checkBox_induct_about_localize->setChecked(ei.localDescription);

             break;
         }

    }
}


void ComponentsEditor::on_button_gearbox_gears_add_clicked()
{
    if(currentGearRestrictions.contains(ui->comboBox_gearbox_gears->currentText()))
    {
        QMessageBox::critical(this,"Error",
                              QString("This Gear system has allready been added."));
    }
    else
    {
        currentGearRestrictions.push_back(ui->comboBox_gearbox_gears->currentText());
        ui->list_gearbox_gears->clear();
        ui->list_gearbox_gears->addItems(currentGearRestrictions);

        if(ui->comboBox_gearbox_gears->findText(ui->comboBox_gearbox_gears->currentText()) == -1)
        {
            ui->comboBox_gearbox_gears->addItem(ui->comboBox_gearbox_gears->currentText());
        }
    }
}



void ComponentsEditor::on_button_gearbox_gear_remove_clicked()
{
    if(currentGearRestrictions.contains(ui->comboBox_gearbox_gears->currentText()))
    {
        currentGearRestrictions.removeAll(ui->comboBox_gearbox_gears->currentText());
        ui->list_gearbox_gears->clear();
        ui->list_gearbox_gears->addItems(currentGearRestrictions);
    }
}

void ComponentsEditor::on_list_gearbox_gears_currentTextChanged(const QString &currentText)
{
    ui->comboBox_gearbox_gears->setEditText(currentText);
}


void ComponentsEditor::on_button_gearbox_add_clicked()
{
    ComponentsManager::GearBoxComps gb;
        gb.selectionIndex = ui->spin_gearbox_id->value();
        gb.name = ui->lineEdit_gearbox_name->text();
        gb.picture = ui->comboBox_gearbox_picture->currentText();
        gb.about = ui->textEdit_gearbox_about->toPlainText();
        gb.shift = ui->comboBox_gearbox_shift->currentText();
        gb.year = ui->spin_gearbox_year->value();
        gb.death = ui->spin_gearbox_endyear->value();
        gb.skill  = ui->spin_gearbox_skill->value();


        gb.designCosts = ui->spin_gearbox_designcost->value();
        gb.costs = ui->spin_gearbox_costs->value();
        gb.smooth = ui->spin_gearbox_smooth->value();
        gb.comfort = ui->spin_gearbox_comfort->value();
        gb.fuel = ui->spin_gearbox_fuel->value();
        gb.performance = ui->spin_gearbox_performance->value();
        gb.complex = ui->spin_gearbox_complexity->value();
        gb.weight = ui->spin_gearbox_weight->value();
        gb.popIndex   = ui->spin_gearbox_aipop->value();

        gb.localName = ui->checkBox_gearboxname_localized->isChecked();
        gb.localDescription = ui->checkBox_gearbox_about_localized->isChecked();
        gb.localPara = ui->checkBox_gearbox_restriction_localize->isChecked();

        gb.possiblegearsList = currentGearRestrictions;

        gb.trans = ui->checkBox_gearbox_addons_trans->isChecked();
        gb.reverse = ui->checkBox_gearbox_addon_reverse->isChecked();
        gb.overdrive = ui->checkBox_gearbox_addon_overdrive->isChecked();
        gb.limited = ui->checkBox_gearbox_addon_limited->isChecked();



        if(!gb.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < gearboxes.size(); i++)
        {
            if(gearboxes[i].name == gb.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(gb.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }

        if(gb.possiblegearsList.empty())
        {
            QMessageBox::critical(this,"Error",
                                  QString("Gearbox must have a selection of Gears!"));
            return;
        }

        selectorIDMap.insert(gb.selectionIndex,"Gearbox-"+gb.name);
        gearboxes.push_back(gb);

        fillGearboxTable();
}

void ComponentsEditor::fillGearboxTable()
{
    ui->table_gearbox->clearContents();
    ui->table_gearbox->setRowCount(gearboxes.size());

    for(int i = 0; i < gearboxes.size(); i++)
    {
            ui->table_gearbox->setItem(i,0,new QTableWidgetItem(
                                         QString::number(gearboxes[i].selectionIndex)));
            ui->table_gearbox->setItem(i,1,new QTableWidgetItem(
                                             QString::number(gearboxes[i].year)));
            ui->table_gearbox->setItem(i,2,new QTableWidgetItem(
                                             QString::number(gearboxes[i].death)));
            if(gearboxes[i].localName && localeManager != 0)
             ui->table_gearbox->setItem(i,3,new QTableWidgetItem(localeManager->getWord(gearboxes[i].name.toInt())));
            else
             ui->table_gearbox->setItem(i,3,new QTableWidgetItem(gearboxes[i].name));

            ui->table_gearbox->setItem(i,4,new QTableWidgetItem(
                                         QString::number(gearboxes[i].skill)));

    }

    fillAIPopComboData();
}

void ComponentsEditor::on_button_gearbox_remove_clicked()
{
    for(int i = 0; i < gearboxes.size(); i++)
    {

         if(gearboxes[i].name == ui->lineEdit_gearbox_name->text() &&
                 gearboxes[i].selectionIndex == ui->spin_gearbox_id->value())
         {
             selectorIDMap.remove(gearboxes[i].selectionIndex);
             gearboxes.remove(i);
             break;
         }
    }

    fillGearboxTable();
}



void ComponentsEditor::on_table_gearbox_cellClicked(int row, int column)
{
    QString componenetName = ui->table_gearbox->item(row,3)->text();
    int selectID = ui->table_gearbox->item(row,0)->text().toInt();

    for(int i = 0; i < gearboxes.size(); i++)
    {
        QString gearboxName =gearboxes[i].name;

        if(gearboxes[i].localName && localeManager != 0)
            gearboxName = localeManager->getWord(gearboxes[i].name.toInt());

         if(gearboxName == componenetName && gearboxes[i].selectionIndex == selectID)
         {
             ComponentsManager::GearBoxComps gb = gearboxes[i];
             ui->spin_gearbox_id->setValue(gb.selectionIndex);
             ui->lineEdit_gearbox_name->setText(gb.name);
             ui->comboBox_gearbox_picture->setEditText(gb.picture);
             ui->textEdit_gearbox_about->setPlainText(gb.about);
             ui->comboBox_gearbox_shift->setCurrentIndex(
                ui->comboBox_gearbox_shift->findText(gb.shift));

             ui->spin_gearbox_year->setValue(gb.year);
             ui->spin_gearbox_endyear->setValue(gb.death);
             ui->spin_gearbox_skill->setValue(gb.skill);

             ui->spin_gearbox_designcost->setValue(gb.designCosts);
             ui->spin_gearbox_costs->setValue(gb.costs);
             ui->spin_gearbox_smooth->setValue(gb.smooth);
             ui->spin_gearbox_comfort->setValue(gb.comfort);
             ui->spin_gearbox_fuel->setValue(gb.fuel);
             ui->spin_gearbox_performance->setValue(gb.performance);
             ui->spin_gearbox_complexity->setValue(gb.complex);
             ui->spin_gearbox_weight->setValue(gb.weight);
             ui->spin_gearbox_aipop->setValue(gb.popIndex);

             ui->checkBox_gearboxname_localized->setChecked(gb.localName);
             ui->checkBox_gearbox_about_localized->setChecked(gb.localDescription);
             ui->checkBox_gearbox_restriction_localize->setChecked(gb.localPara);


             currentGearRestrictions = gb.possiblegearsList;

             ui->checkBox_gearbox_addons_trans->setChecked(gb.trans);
             ui->checkBox_gearbox_addon_reverse->setChecked(gb.reverse);
             ui->checkBox_gearbox_addon_overdrive->setChecked(gb.overdrive);
             ui->checkBox_gearbox_addon_limited->setChecked(gb.limited);


             ui->list_gearbox_gears->clear();


             ui->list_gearbox_gears->addItems(currentGearRestrictions);

             break;
         }
    }

}

void ComponentsEditor::on_button_gears_add_clicked()
{
    ComponentsManager::GearGearsComps g;
        g.selectionIndex = ui->spinBox_gears_ID->value();
        g.name = ui->lineEdit_gears_name->text();
        g.picture = ui->comboBox_gears_picture->currentText();
        g.about = ui->textEdit_gears_about->toPlainText();
        g.year = ui->spinBox_gears_year->value();
        g.death = ui->spinBox_gears_endyear->value();
        g.skill  = ui->spinBox_gears_skillReq->value();
        g.gears  = ui->spinBox_gears_gears->value();
        g.popIndex = ui->spinbox_gears_aipop->value();
        g.localName = ui->checkBox_gears_name_localized->isChecked();
        g.localDescription = ui->checkBox_gears_about_localized->isChecked();
        g.localPara = 0;


        if(!g.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < gearVector.size(); i++)
        {
            if(gearVector[i].name == g.name)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(g.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(g.selectionIndex,"Gears-"+g.name);
        gearVector.push_back(g);

        fillGearsTable();

        if(ui->comboBox_gearbox_gears->findText(g.name) == -1)
        {
            ui->comboBox_gearbox_gears->addItem(g.name);
        }
}



void ComponentsEditor::on_button_gears_remove_clicked()
{
    for(int i = 0; i < gearVector.size(); i++)
    {

         if(gearVector[i].name == ui->lineEdit_gears_name->text() &&
                 gearVector[i].selectionIndex == ui->spinBox_gears_ID->value())
         {
             selectorIDMap.remove(gearVector[i].selectionIndex);
             gearVector.remove(i);
             break;
         }
    }

    fillGearsTable();
}


void ComponentsEditor::fillGearsTable()
{
    ui->table_gears->clearContents();
    ui->table_gears->setRowCount(gearVector.size());
    ui->table_gears->setSortingEnabled(false);


    for(int i = 0; i < gearVector.size(); i++)
    {
            ui->table_gears->setItem(i,0,new QTableWidgetItem(
                                         QString::number(gearVector[i].selectionIndex)));
            ui->table_gears->setItem(i,1,new QTableWidgetItem(
                                             QString::number(gearVector[i].year)));
            ui->table_gears->setItem(i,2,new QTableWidgetItem(
                                             QString::number(gearVector[i].death)));

            if(gearVector[i].localName && localeManager != 0)
                ui->table_gears->setItem(i,3,new QTableWidgetItem(localeManager->getWord(gearVector[i].name.toInt())));
            else
                ui->table_gears->setItem(i,3,new QTableWidgetItem(gearVector[i].name));

            ui->table_gears->setItem(i,4,new QTableWidgetItem(
                                         QString::number(gearVector[i].gears)));
            ui->table_gears->setItem(i,5,new QTableWidgetItem(
                                         QString::number(gearVector[i].skill)));

    }
    ui->table_gears->setSortingEnabled(true);


    fillAIPopComboData();
}



void ComponentsEditor::on_table_gears_cellClicked(int row, int column)
{
    QString componenetName = ui->table_gears->item(row,3)->text();
    int selectID = ui->table_gears->item(row,0)->text().toInt();

    for(int i = 0; i < gearVector.size(); i++)
    {

        QString gearsName =gearVector[i].name;

        if(gearVector[i].localName && localeManager != 0)
            gearsName = localeManager->getWord(gearVector[i].name.toInt());

         if(gearsName == componenetName && gearVector[i].selectionIndex == selectID)
         {
             ComponentsManager::GearGearsComps g = gearVector[i];
              ui->spinBox_gears_ID->setValue(g.selectionIndex);
              ui->lineEdit_gears_name->setText(g.name);
              ui->comboBox_gears_picture->setEditText(g.picture);
              ui->textEdit_gears_about->setPlainText(g.about);
              ui->spinBox_gears_year->setValue(g.year);
              ui->spinBox_gears_endyear->setValue(g.death);
              ui->spinBox_gears_skillReq->setValue(g.skill);
              ui->spinBox_gears_gears->setValue(g.gears);
              ui->spinbox_gears_aipop->setValue(g.popIndex);
              ui->checkBox_gears_name_localized->setChecked(g.localName);
              ui->checkBox_gears_about_localized->setChecked(g.localDescription);

             break;
         }

    }
}

void ComponentsEditor::on_button_cartype_add_clicked()
{
    ComponentsManager::CarTypes ct;
        ct.selectionIndex = ui->spinBox_carType_ID->value();
        ct.type = ui->lineEdit_carType_Name->text();
        ct.picture = ui->comboBox_carType_pictures->currentText();
        ct.about = ui->textEdit_carType_about->toPlainText();
        ct.year = ui->spinBox_carType_year->value();
        ct.death = ui->spinBox_carType_endyear->value();

        ct.rPerformance = ui->spinBox_carType_performance->value();
        ct.rDrivability = ui->spinBox_carType_driveability->value();
        ct.rLuxury = ui->spinBox_carType_luxury->value();
        ct.rSafety = ui->spinBox_carType_safety->value();
        ct.rFuel = ui->spinBox_carType_fuel->value();
        ct.rPower = ui->spinBox_carType_power->value();
        ct.rCargo = ui->spinBox_carType_cargo->value();
        ct.rDependability = ui->spinBox_carType_depend->value();
        ct.popNA = ui->spinBox_carType_na->value();
        ct.popEU = ui->spinBox_carType_eu->value();
        ct.popSA = ui->spinBox_carType_sa->value();
        ct.popAF = ui->spinBox_carType_af->value();
        ct.popAS = ui->spinBox_carType_as->value();
        ct.popAU = ui->spinBox_carType_au->value();
        ct.pop = ui->spinBox_carType_ovrpop->value();
        ct.cargo = ui->spinBox_carType_cargoSpace->value();
        ct.wealth = ui->comboBox_cartype_wealth->currentText().toInt();
        ct.age = ui->comboBox_carType_age->currentText().toInt();
        ct.pass = ui->spinBox_carType_pass->value();


        ct.milFleet = ui->checkBox_cartype_military->isChecked();
        ct.civFleet = ui->checkBox_carType_civFleet->isChecked();
        ct.civ = ui->checkBox_cartype_civ->isChecked();

        ct.localName = ui->checkBox_carType_Name_Localized->isChecked();
        ct.localDescription = ui->checkBox_carType_about_localized->isChecked();
        ct.localPara = 0;

        ct.weight = ui->CarType_Weight_SpinBox->value();
        ct.size = ui->CarType_Size_Spinner->value();

        if(!ct.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture file must be a .dds and end with the"
                                          " .dds extension!"));
            return;
        }

        for(int i = 0; i < typeVector.size(); i++)
        {
            if(typeVector[i].type == ct.type)
            {
                QMessageBox::critical(this,"Error",
                                      QString("You can not have duplicate names"
                                              " in the same subcomponent type!"));
                return;
            }
        }

        if(selectorIDMap.contains(ct.selectionIndex))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Selector ID is currently being used by a different"
                                          "component!"));
            return;
        }


        selectorIDMap.insert(ct.selectionIndex,"CarTypes-"+ct.type);
        typeVector.push_back(ct);

        fillCarTypeTable();


}

void ComponentsEditor::on_button_cartype_remove_clicked()
{
    for(int i = 0; i < typeVector.size(); i++)
    {

         if(typeVector[i].type == ui->lineEdit_carType_Name->text() &&
                 typeVector[i].selectionIndex == ui->spinBox_carType_ID->value())
         {
             selectorIDMap.remove(typeVector[i].selectionIndex);
             typeVector.remove(i);
             break;
         }
    }

    fillCarTypeTable();
}


void ComponentsEditor::fillCarTypeTable()
{
    ui->table_cartype->clearContents();
    ui->table_cartype->setRowCount(typeVector.size());
    ui->table_cartype->setSortingEnabled(false);


    for(int i = 0; i < typeVector.size(); i++)
    {
            ui->table_cartype->setItem(i,0,new QTableWidgetItem(
                                         QString::number(typeVector[i].selectionIndex)));
            ui->table_cartype->setItem(i,1,new QTableWidgetItem(
                                             QString::number(typeVector[i].year)));
            ui->table_cartype->setItem(i,2,new QTableWidgetItem(
                                             QString::number(typeVector[i].death)));


            if(typeVector[i].localName && localeManager != 0)
                 ui->table_cartype->setItem(i,3,new QTableWidgetItem(localeManager->getWord(typeVector[i].type.toInt())));
            else
                ui->table_cartype->setItem(i,3,new QTableWidgetItem(typeVector[i].type));

            if(ui->comboBox_carmodels_type->findText(typeVector[i].type) == -1)
            {
                ui->comboBox_carmodels_type->addItem(typeVector[i].type);
            }

    }
    ui->table_cartype->setSortingEnabled(true);


    fillAIPopComboData();
}


void ComponentsEditor::on_table_cartype_cellClicked(int row, int column)
{
    QString componenetName = ui->table_cartype->item(row,3)->text();
    int selectID = ui->table_cartype->item(row,0)->text().toInt();

    for(int i = 0; i < typeVector.size(); i++)
    {

        QString typeName =typeVector[i].type;

        if(typeVector[i].localName && localeManager != 0)
            typeName = localeManager->getWord(typeVector[i].type.toInt());


         if(typeName == componenetName && typeVector[i].selectionIndex == selectID)
         {
             ComponentsManager::CarTypes ct = typeVector[i];
              ui->spinBox_carType_ID->setValue(ct.selectionIndex);
              ui->lineEdit_carType_Name->setText(ct.type);
              ui->comboBox_carType_pictures->setEditText(ct.picture);
              ui->textEdit_carType_about->setPlainText(ct.about);
              ui->spinBox_carType_year->setValue(ct.year);
              ui->spinBox_carType_endyear->setValue(ct.death);

              ui->spinBox_carType_performance->setValue(ct.rPerformance);
             ui->spinBox_carType_driveability->setValue(ct.rDrivability);
              ui->spinBox_carType_luxury->setValue(ct.rLuxury);
              ui->spinBox_carType_safety->setValue(ct.rSafety);
              ui->spinBox_carType_fuel->setValue(ct.rFuel);
              ui->spinBox_carType_power->setValue(ct.rPower);
              ui->spinBox_carType_cargo->setValue(ct.rCargo);
              ui->spinBox_carType_depend->setValue(ct.rDependability);
              ui->spinBox_carType_na->setValue(ct.popNA);
              ui->spinBox_carType_eu->setValue(ct.popEU);
              ui->spinBox_carType_sa->setValue(ct.popSA);
              ui->spinBox_carType_af->setValue(ct.popAF);
              ui->spinBox_carType_as->setValue(ct.popAS);
              ui->spinBox_carType_au->setValue(ct.popAU);
              ui->spinBox_carType_ovrpop->setValue(ct.pop);
              ui->spinBox_carType_cargoSpace->setValue(ct.cargo);
              ui->comboBox_cartype_wealth->setCurrentIndex(
                 ui->comboBox_cartype_wealth->findText(QString::number(ct.wealth)));
              ui->comboBox_carType_age->setCurrentIndex(
                 ui->comboBox_carType_age->findText(QString::number(ct.age)));

              ui->spinBox_carType_pass->setValue(ct.pass);

              ui->checkBox_cartype_military->setChecked(ct.milFleet);
              ui->checkBox_carType_civFleet->setChecked(ct.civFleet);
              ui->checkBox_cartype_civ->setChecked(ct.civ);

              ui->checkBox_carType_Name_Localized->setChecked(ct.localName);
              ui->checkBox_carType_about_localized->setChecked(ct.localDescription);

              ui->CarType_Weight_SpinBox->setValue(ct.weight);
              ui->CarType_Size_Spinner->setValue(ct.size);

             break;
         }

    }
}

void ComponentsEditor::on_button_carmodel_add_clicked()
{
    ComponentsManager::CarModels cm;
        cm.name = ui->lineEdit_carmodels_Name->text();
        cm.typeName = ui->comboBox_carmodels_type->currentText();
        cm.model = ui->comboBox_carmodel_model->currentText();
        cm.year = ui->spinBox_carmodel_year->value();
        cm.death = ui->spinBox_carmodel_endyear->value();

        cm.designReq = ui->spinBox_carmodel_skillreq->value();
        cm.pass = ui->spinBox_carmodel_passengers->value();

        cm.localName = 0;
        cm.localDescription = ui->checkBox_carModel_type_localize->isChecked();
        cm.localPara = 0;


        if(!cm.model.endsWith(".mesh"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Model must be a .mesh file! It must end in .mesh."));
            return;
        }




       carModelVector.push_back(cm);

        fillCarModelTable();
}

void ComponentsEditor::on_button_carmodel_remove_clicked()
{
    for(int i = 0; i < carModelVector.size(); i++)
    {

         if(carModelVector[i].name == ui->lineEdit_carmodels_Name->text() &&
                 carModelVector[i].typeName == ui->comboBox_carmodels_type->currentText())
         {
             carModelVector.remove(i);
             break;
         }
    }

    fillCarModelTable();
}


void ComponentsEditor::fillCarModelTable()
{
    ui->Table_CarModels->clearContents();
    ui->Table_CarModels->setRowCount(carModelVector.size());
    ui->Table_CarModels->setSortingEnabled(false);

    for(int i = 0; i < carModelVector.size(); i++)
    {
            ui->Table_CarModels->setItem(i,0,new QTableWidgetItem(
                                         QString::number(carModelVector[i].year)));
            ui->Table_CarModels->setItem(i,1,new QTableWidgetItem(
                                             QString::number(carModelVector[i].death)));
            ui->Table_CarModels->setItem(i,2,new QTableWidgetItem(carModelVector[i].name));

            if(carModelVector[i].localDescription && localeManager != 0)
                 ui->Table_CarModels->setItem(i,3,new QTableWidgetItem(localeManager->getWord(carModelVector[i].typeName.toInt())));
            else
                 ui->Table_CarModels->setItem(i,3,new QTableWidgetItem(carModelVector[i].typeName));

            ui->Table_CarModels->setItem(i,4,new QTableWidgetItem(carModelVector[i].model));
    }
    ui->Table_CarModels->setSortingEnabled(true);
    fillAIPopComboData();
}



void ComponentsEditor::on_Table_CarModels_cellClicked(int row, int column)
{
    QString Name = ui->Table_CarModels->item(row,2)->text();
    QString carType = ui->Table_CarModels->item(row,3)->text();

    for(int i = 0; i < carModelVector.size(); i++)
    {
        QString typeName =carModelVector[i].typeName;

        if(carModelVector[i].localDescription && localeManager != 0)
            typeName = localeManager->getWord(carModelVector[i].typeName.toInt());

         if(carModelVector[i].name == Name && typeName == carType)
         {
              ComponentsManager::CarModels cm = carModelVector[i];
               ui->lineEdit_carmodels_Name->setText(cm.name);
               ui->comboBox_carmodels_type->setEditText(cm.typeName);
               ui->comboBox_carmodel_model->setEditText(cm.model);
               ui->spinBox_carmodel_year->setValue(cm.year);
               ui->spinBox_carmodel_endyear->setValue(cm.death);

               ui->spinBox_carmodel_skillreq->setValue(cm.designReq);
               ui->spinBox_carmodel_passengers->setValue(cm.pass);


               ui->checkBox_carModel_type_localize->setChecked(cm.localDescription);


             break;
         }

    }
}

void ComponentsEditor::on_button_Accessory_add_clicked()
{
    ComponentsManager::AccessoriesModels am;
        am.name = ui->lineEdit_Accessory_Name->text();
        am.typeName = ui->comboBox_Accessory_type->currentText();
        am.model = ui->comboBox_Accessory_model->currentText();
        am.picture = ui->comboBox_Accessory_picture->currentText();
        am.year = ui->spinBox_Accessory_year->value();
        am.death = ui->spinBox_Accessory_endyear->value();
        am.designreq = ui->spinBox_Accessory_skillreq->value();

        am.autoMirror = ui->checkBox_Accessory_autoMirror->isChecked();
        am.autoPaint = ui->checkBox_Accessory_autoPaint->isChecked();
        am.decal = ui->checkBox_accessory_decal->isChecked();

        if(am.decal)
        {
            am.height = ui->spinBox_accessory_height->value();
            am.width = ui->spinBox_accessories_width->value();
        }

        am.localName = 0;
        am.localDescription = ui->checkBox_Accessory_TypeLocalize->isChecked();
        am.localPara = 0;


        if(!am.model.endsWith(".mesh") && !am.decal)
        {
            QMessageBox::critical(this,"Error",
                                  QString("Model must be a .mesh file! It must end in .mesh."));
            return;
        }


        if(!am.picture.endsWith(".dds"))
        {
            QMessageBox::critical(this,"Error",
                                  QString("Picture files must be a .dds file!"
                                          " It must end in .dds."));
            return;
        }


       accessories.push_back(am);

        fillAccessoriesTable();

         if(ui->comboBox_Accessory_type->findText(am.typeName) == -1)
         {
             ui->comboBox_Accessory_type->addItem(am.typeName);
         }
}

void ComponentsEditor::on_button_Accessory_remove_clicked()
{
    for(int i = 0; i < accessories.size(); i++)
    {

         if(accessories[i].name == ui->lineEdit_Accessory_Name->text() &&
                 accessories[i].typeName == ui->comboBox_Accessory_type->currentText())
         {
             accessories.remove(i);
             break;
         }
    }

    fillAccessoriesTable();
}


void ComponentsEditor::fillAccessoriesTable()
{
    ui->table_Accessory->clearContents();
    ui->table_Accessory->setRowCount(accessories.size());
    ui->table_Accessory->setSortingEnabled(false);


    for(int i = 0; i < accessories.size(); i++)
    {
            ui->table_Accessory->setItem(i,0,new QTableWidgetItem(
                                         QString::number(accessories[i].year)));
            ui->table_Accessory->setItem(i,1,new QTableWidgetItem(
                                             QString::number(accessories[i].death)));
            ui->table_Accessory->setItem(i,2,new QTableWidgetItem(accessories[i].name));

            if(accessories[i].localDescription && localeManager != 0)
               ui->table_Accessory->setItem(i,3,new QTableWidgetItem(localeManager->getWord(accessories[i].typeName.toInt())));
            else
               ui->table_Accessory->setItem(i,3,new QTableWidgetItem(accessories[i].typeName));

            ui->table_Accessory->setItem(i,4,new QTableWidgetItem(accessories[i].model));
    }

    ui->table_Accessory->setSortingEnabled(true);

    fillAIPopComboData();
}


void ComponentsEditor::on_table_Accessory_cellClicked(int row, int column)
{
    QString Name = ui->table_Accessory->item(row,2)->text();
    QString carType = ui->table_Accessory->item(row,3)->text();

    for(int i = 0; i < accessories.size(); i++)
    {
        QString typeName =accessories[i].typeName;

        if(accessories[i].localDescription && localeManager != 0)
            typeName = localeManager->getWord(accessories[i].typeName.toInt());

         if(accessories[i].name == Name && typeName == carType)
         {
              ComponentsManager::AccessoriesModels am = accessories[i];
               ui->lineEdit_Accessory_Name->setText(am.name);
               ui->comboBox_Accessory_type->setEditText(am.typeName);
               ui->comboBox_Accessory_model->setEditText(am.model);
               ui->comboBox_Accessory_picture->setEditText(am.picture);

               ui->spinBox_Accessory_year->setValue(am.year);
               ui->spinBox_Accessory_endyear->setValue(am.death);

               ui->checkBox_Accessory_TypeLocalize->setChecked(am.localDescription);
               ui->checkBox_Accessory_autoMirror->setChecked(am.autoMirror);
               ui->checkBox_Accessory_autoPaint->setChecked(am.autoPaint);

               ui->checkBox_accessory_decal->setChecked(am.decal);

               if(am.decal)
               {
                   ui->spinBox_accessories_width->setValue(am.width);
                   ui->spinBox_accessory_height->setValue(am.height);
               }

             break;
         }

    }
}

void ComponentsEditor::on_button_aipop_add_clicked()
{

    QString keyName = QString::number(ui->spinBox_aipop_year->value()) + "-"+
            ui->comboBox_aipop_id->itemData(ui->comboBox_aipop_id->currentIndex()).toString();

    ComponentsManager::aiPopData ap;
        ap.year = ui->spinBox_aipop_year->value();
        ap.fuel = ui->spinBox_aipop_fuel->value();
        ap.performance = ui->spinBox_aipop_performance->value();
        ap.luxury = ui->spinBox_aipop_luxury->value();
        ap.costs = ui->spinBox_aipop_costs->value();
        ap.ovrpop = ui->spinBox_aipop_ovrpop->value();
        ap.id = ui->comboBox_aipop_id->itemData(ui->comboBox_aipop_id->currentIndex()).toInt();
        ap.name = ui->comboBox_aipop_id->currentText();

        aiPops.insert(keyName,ap);

         fillAIPopTable();


}

void ComponentsEditor::on_button_aipop_remove_clicked()
{
    QString keyName = QString::number(ui->spinBox_aipop_year->value()) + "-"+
            ui->comboBox_aipop_id->itemData(ui->comboBox_aipop_id->currentIndex()).toString();

    if(aiPops.contains(keyName))
    {
        aiPops.remove(keyName);
    }
    else
    {
            QModelIndexList selection = ui->table_aipop->selectionModel()->selectedIndexes();

            foreach(QModelIndex index, selection)
            {
                QString keyNameAttemptTwo = ui->table_aipop->item(index.row(),0)->text()+"-"+
                        ui->table_aipop->item(index.row(),2)->text();

                if(aiPops.contains(keyNameAttemptTwo))
                {
                    aiPops.remove(keyNameAttemptTwo);
                }
            }


    }


    fillAIPopTable();
}


void ComponentsEditor::fillAIPopTable()
{
    ui->table_aipop->clearContents();
    ui->table_aipop->setRowCount(aiPops.size());

    ui->table_aipop->setSortingEnabled(false);

    int i = 0;
    for(QMap<QString, ComponentsManager::aiPopData>::Iterator it = aiPops.begin(); it != aiPops.end(); ++it)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(Qt::EditRole, (*it).id);

            ui->table_aipop->setItem(i,0,new QTableWidgetItem(
                                         QString::number((*it).year)));

            QStringList sl = (*it).name.split("-");
            bool ok = false;
            QString partOne = "";
            foreach(QString compName, sl)
            {

                int local = compName.toInt(&ok, 10);

                if(ok && local > 10000 && localeManager != 0 )
                {
                     ui->table_aipop->setItem(i,1,new QTableWidgetItem(partOne + "-"+localeManager->getWord(local)));
                     break;
                }
                else
                {
                    partOne = compName;
                    ok = false;
                }
            }

            if(!ok)
              ui->table_aipop->setItem(i,1,new QTableWidgetItem((*it).name));


            ui->table_aipop->setItem(i,2,item);
            ui->table_aipop->setItem(i,3,new QTableWidgetItem(
                                         QString::number((*it).fuel)));
            ui->table_aipop->setItem(i,4,new QTableWidgetItem(
                                         QString::number((*it).performance)));
            ui->table_aipop->setItem(i,5,new QTableWidgetItem(
                                         QString::number((*it).luxury)));
            ui->table_aipop->setItem(i,6,new QTableWidgetItem(
                                         QString::number((*it).costs)));
            ui->table_aipop->setItem(i,7,new QTableWidgetItem(
                                         QString::number((*it).ovrpop)));

            i++;
    }

    ui->table_aipop->setSortingEnabled(true);


    fillAIPopComboData();
}


void ComponentsEditor::on_table_aipop_cellClicked(int row, int column)
{
    QString keyName = ui->table_aipop->item(row,0)->text()+"-"+
            ui->table_aipop->item(row,2)->text();


         if(aiPops.contains(keyName))
         {
              ComponentsManager::aiPopData ap = aiPops.find(keyName).value();
               ui->spinBox_aipop_year->setValue(ap.year);
               ui->spinBox_aipop_fuel->setValue(ap.fuel);
               ui->spinBox_aipop_performance->setValue(ap.performance);
               ui->spinBox_aipop_luxury->setValue(ap.luxury);
               ui->spinBox_aipop_costs->setValue(ap.costs);
               ui->spinBox_aipop_ovrpop->setValue(ap.ovrpop);

               QStringList sl = ap.name.split("-");
               bool ok = false;
               QString partOne = "";
               QString textName = "";
               foreach(QString compName, sl)
               {

                   int local = compName.toInt(&ok, 10);

                   if(ok && local > 10000 && localeManager != 0 )
                   {
                        textName = partOne + "-"+localeManager->getWord(local);
                        break;
                   }
                   else
                   {
                       partOne = compName;
                       ok = false;
                   }
               }

               if(!ok)
                textName = ap.name;



               ui->comboBox_aipop_id->setCurrentIndex(ui->comboBox_aipop_id->findText(textName));

         }


}

 void ComponentsEditor::fillAIPopComboData()
 {
     ui->comboBox_aipop_id->clear();
     for(QMap<int, QString>::Iterator it = selectorIDMap.begin(); it != selectorIDMap.end(); ++it)
     {
         QString textName = "";
         QStringList sl = (*it).split("-");
         bool ok = false;
         QString partOne = "";
         foreach(QString compName, sl)
         {

             int local = compName.toInt(&ok, 10);

             if(ok && local > 10000 && localeManager != 0 )
             {
                  textName = partOne + "-"+localeManager->getWord(local);
                  break;
             }
             else
             {
                 partOne = compName;
                 ok = false;
             }
         }

         if(!ok)
           textName = (*it);


         ui->comboBox_aipop_id->addItem(textName, QVariant(it.key()));
     }
 }

void ComponentsEditor::on_button_select_folder_picture_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Picture Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.dds");
    QStringList fileList = dir.entryList();

    ui->comboBox_Accessory_picture->clear();
    ui->comboBox_carType_pictures->clear();
    ui->comboBox_cyl_picture->clear();
    ui->comboBox_drivertrain_picture->clear();
    ui->comboBox_englayout_picture->clear();
    ui->comboBox_fuel_picture->clear();
    ui->comboBox_gearbox_gears->clear();
    ui->comboBox_gears_picture->clear();
    ui->comboBox_induct_picture->clear();
    ui->comboBox_suspension_picture->clear();
    ui->comboBox_valve_picture->clear();
    ui->combo_frame_picture->clear();


    ui->comboBox_Accessory_picture->addItems(fileList);
    ui->comboBox_carType_pictures->addItems(fileList);
    ui->comboBox_cyl_picture->addItems(fileList);
    ui->comboBox_drivertrain_picture->addItems(fileList);
    ui->comboBox_englayout_picture->addItems(fileList);
    ui->comboBox_fuel_picture->addItems(fileList);
   // ui->comboBox_gearbox_gears->addItems(fileList);
    ui->comboBox_gears_picture->addItems(fileList);
    ui->comboBox_induct_picture->addItems(fileList);
    ui->comboBox_suspension_picture->addItems(fileList);
    ui->comboBox_valve_picture->addItems(fileList);
    ui->combo_frame_picture->addItems(fileList);

    ui->label_folder_picture->setText(folderPath);
}



void ComponentsEditor::on_button_select_carmodel_folder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Car Model Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.mesh");
    QStringList fileList = dir.entryList();

    ui->comboBox_carmodel_model->clear();
    ui->comboBox_carmodel_model->addItems(fileList);
}

void ComponentsEditor::on_button_accessories_selectfolder_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Accessories Model Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    QDir dir(folderPath);
    dir.setNameFilters(QStringList()<<"*.mesh");
    QStringList fileList = dir.entryList();

    ui->comboBox_Accessory_model->clear();
    ui->comboBox_Accessory_model->addItems(fileList);
}

void ComponentsEditor::on_button_newComponentsFile_clicked()
{
    frames.clear();
    suspensions.clear();
    drivetrains.clear();
    layouts.clear();
    cylinders.clear();
    fuels.clear();
    valves.clear();
    inductions.clear();
    gearboxes.clear();
    gearVector.clear();
    typeVector.clear();
    carModelVector.clear();
    accessories.clear();

    aiPops.clear();

    currentCylRestrictions.clear();
    currentFuelRestrictions.clear();
    currentInductionRestrictions.clear();
    currentGearRestrictions.clear();

    selectorIDMap.clear();

    fillFrameTable();
    fillSuspensionTable();
    fillDrivetrainTable();
    fillEngineLayoutTable();
    fillCylinderTable();
    fillFuelTable();
    fillValveTable();
    fillInductionTable();
    fillGearboxTable();
    fillGearsTable();
    fillCarTypeTable();
    fillCarModelTable();
    fillAccessoriesTable();
    fillAIPopTable();
    fillAIPopComboData();

    ui->list_englayout_cylrestrict->clear();
    ui->list_englayout_fuelrestrict->clear();
    ui->list_englayout_inducrestrict->clear();

    ui->list_gearbox_gears->clear();

}

void ComponentsEditor::on_button_openComponentsFiles_clicked()
{
    QString ComponentsFileName =  QFileDialog::getOpenFileName(
                this, "Open Components File", "","XML Files (*.xml)");

    if (ComponentsFileName != "")
    {
       ComponentsManager *cm = new ComponentsManager(ComponentsFileName, cp_wsc.ComponentsCW);
       ComponentsManager::ComponentDataLists dataList = cm->returnDataList();

       frames = dataList.frameList.toVector();
       suspensions = dataList.suspensionList.toVector();
       drivetrains = dataList.drivetrainList.toVector();
       layouts = dataList.engineLayoutList.toVector();
       cylinders = dataList.engineCylinderList.toVector();
       fuels = dataList.engineFuelList.toVector();
       valves = dataList.engineValveList.toVector();
       inductions = dataList.engineInductionList.toVector();
       gearboxes = dataList.gearboxTypeList.toVector();
       gearVector = dataList.gearboxGearsList.toVector();
       typeVector = dataList.cartypesList.toVector();
       carModelVector = dataList.carModelsList.toVector();
       accessories = dataList.accessoriesList.toVector();

       for(int i = 0; i < frames.size(); i++)
       {
         selectorIDMap.insert(frames[i].selectionIndex,"Frames-"+frames[i].name);
       }

       for(int i = 0; i < suspensions.size(); i++)
       {
         selectorIDMap.insert(suspensions[i].selectionIndex,"Suspension-"+suspensions[i].name);
       }

       for(int i = 0; i < drivetrains.size(); i++)
       {
         selectorIDMap.insert(drivetrains[i].selectionIndex,"Drivetrain-"+drivetrains[i].name);
       }

       for(int i = 0; i < layouts.size(); i++)
       {
         selectorIDMap.insert(layouts[i].selectionIndex,"EngineLayout-"+layouts[i].name);
       }

       for(int i = 0; i < cylinders.size(); i++)
       {
         selectorIDMap.insert(cylinders[i].selectionIndex,"EngineCylinder-"+cylinders[i].name);
         ui->comboBox_englayout_cylrestrict->addItem(cylinders[i].name);
       }

       for(int i = 0; i < fuels.size(); i++)
       {
         selectorIDMap.insert(fuels[i].selectionIndex,"EngineFuel-"+fuels[i].name);
          ui->comboBox_englayout_fuelrestrict->addItem(fuels[i].name);
       }

       for(int i = 0; i < valves.size(); i++)
       {
         selectorIDMap.insert(valves[i].selectionIndex,"EngineValve-"+valves[i].name);         
       }

       for(int i = 0; i < inductions.size(); i++)
       {
         selectorIDMap.insert(inductions[i].selectionIndex,"EngineInduction-"+inductions[i].name);
          ui->combo_englayout_inductrestrict->addItem(inductions[i].name);
       }

       for(int i = 0; i < gearboxes.size(); i++)
       {
         selectorIDMap.insert(gearboxes[i].selectionIndex,"Gearbox-"+gearboxes[i].name);
       }

       for(int i = 0; i < gearVector.size(); i++)
       {
         selectorIDMap.insert(gearVector[i].selectionIndex,"Gears-"+gearVector[i].name);
         ui->comboBox_gearbox_gears->addItem(gearVector[i].name);
       }

       for(int i = 0; i < typeVector.size(); i++)
       {
         selectorIDMap.insert(typeVector[i].selectionIndex,"CarTypes-"+typeVector[i].type);
       }


       fillFrameTable();
       fillSuspensionTable();
       fillDrivetrainTable();
       fillEngineLayoutTable();
       fillCylinderTable();
       fillFuelTable();
       fillValveTable();
       fillInductionTable();
       fillGearboxTable();
       fillGearsTable();
       fillCarTypeTable();
       fillCarModelTable();
       fillAccessoriesTable();
       fillAIPopTable();
       fillAIPopComboData();

       QString popFileName =  QFileDialog::getOpenFileName(
                   this, "Open Components Popularity File", "","XML Files (*.xml)");

       if (popFileName != "")
       {
           if(cm->parseAIComponentsPopularity(popFileName, cp_wsc.ComponentsCW))
           {
               aiPops = cm->returnPopsMap();
               fillAIPopTable();
           }

       }



       delete cm;
    }
    else
    {
        return;

    }


}

void ComponentsEditor::on_button_saveComponentsFiles_clicked()
{
    QString saveComponentsFileName = QFileDialog::getSaveFileName(this, "Save Components File",
                                                          "","XML Files (*.xml)");

    if (saveComponentsFileName != "")
    {
        if (!saveComponentsFileName.endsWith(".xml"))
            saveComponentsFileName += ".xml";

        ui->label_savename_components->setText(saveComponentsFileName);

        //Call function to actually save.
        ComponentsManager::ComponentDataLists dataList;

        dataList.frameList = frames.toList();
        dataList.suspensionList = suspensions.toList();
        dataList.drivetrainList = drivetrains.toList();
        dataList.engineLayoutList = layouts.toList();
        dataList.engineCylinderList = cylinders.toList();
        dataList.engineFuelList = fuels.toList();
        dataList.engineValveList = valves.toList();
        dataList.engineInductionList = inductions.toList();
        dataList.gearboxTypeList = gearboxes.toList();
        dataList.gearboxGearsList = gearVector.toList();
        dataList.cartypesList = typeVector.toList();
        dataList.carModelsList = carModelVector.toList();
        dataList.accessoriesList = accessories.toList();

        ComponentsManager::saveComponentsXMLFile(saveComponentsFileName,dataList);
    }


    if(!aiPops.isEmpty())
    {
       QString savePopFileName = QFileDialog::getSaveFileName(this, "Save AI Components Pop File",
                                                          "","XML Files (*.xml)");

        if (savePopFileName != "")
        {
            if (!savePopFileName.endsWith(".xml"))
                savePopFileName += ".xml";

            ui->label_savename_aipop->setText(savePopFileName);

            //Call function to actually save.
            ComponentsManager::saveComponentsPopXMLFile(savePopFileName,aiPops);
        }
    }
}



void ComponentsEditor::on_checkBox_accessory_decal_clicked()
{
    if(!ui->checkBox_accessory_decal->isChecked())
    {
        ui->label_accessory_height->hide();
        ui->label_accessory_width->hide();
        ui->spinBox_accessories_width->hide();
        ui->spinBox_accessory_height->hide();
    }
    else if(ui->checkBox_accessory_decal->isChecked())
    {
        ui->label_accessory_height->show();
        ui->label_accessory_width->show();
        ui->spinBox_accessories_width->show();
        ui->spinBox_accessory_height->show();
    }
}

void ComponentsEditor::on_button_returnToMain_clicked()
{
    cp_wsc.ComponentsCW->lower();
}

//Allows table to be sortable
void ComponentsEditor::sectionDoubleClickedSlot(int index)
{
     ui->table_aipop->sortByColumn(index,Qt::AscendingOrder);
}

void ComponentsEditor::on_button_select_localizationFile_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Localization File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        //Create localization mananger
        localeManager = new LocalizationManager(openFileName, cp_wsc.ComponentsCW);

        //If localization has words
        if(localeManager->size()!=0)
        {
            ui->label_localization_filename->setText(openFileName);

        }
    }
    else
    {
        return;
    }
}
