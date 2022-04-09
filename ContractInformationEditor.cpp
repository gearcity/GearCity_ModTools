#include "ContractInformationEditor.h"
#include "ui_ContractInformationEditor.h"
#include "QtWidgets/QFileDialog"
#include "QtWidgets/QMessageBox"

ContractInformationEditor::ContractInformationEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContractInformationEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;

    lm = 0;
    cm = 0;
}

ContractInformationEditor::~ContractInformationEditor()
{
    delete ui;
    if(lm != 0)
        delete lm;

    if(cm != 0)
        delete cm;
}

/* Load Components.XML files. */
void ContractInformationEditor::on_button_SelectComponentsFile_clicked()
{

    QString componentsFileName =  QFileDialog::getOpenFileName(this, "Open Components File", "",
                                                         "XML Files (*.xml)");


    if (componentsFileName != "")
    {
        //if we have a file, create a components object, then send the combos over to be filled.
        cm = new ComponentsManager(componentsFileName,cp_wsc.ContractsCW);
        ui->label_selectedComponentsFile->setText(componentsFileName);

        refillCombos();
    }
    else
    {
        return;
    }

}

void ContractInformationEditor::refillCombos()
{
    ui->comboBox_v_fuelTypes->clear();
    ui->comboBox_v_vehicleTypeLimit->clear();
    ui->comboBox_e_CylinderLimit->clear();
    ui->comboBox_e_fuelLimit->clear();
    ui->comboBox_e_LayoutLimits->clear();
    ui->comboBox_g_GearLimits->clear();
    ui->comboBox_g_typeLimit->clear();

    //If cm was not processed fine, then we shouldn't touch the combos
    if(cm != 0 && cm->isGood())
    {

        ComponentsManager::ComponentDataLists cdl = cm->returnDataList();

       for(QList<ComponentsManager::CarTypes>::iterator it = cdl.cartypesList.begin(); it != cdl.cartypesList.end(); ++it)
       {
           if((*it).localName == 1 && lm != 0)
           {
            ui->comboBox_v_vehicleTypeLimit->addItem(lm->getWord((*it).type.toInt()), QVariant((*it).type));
           }
           else
           {
             ui->comboBox_v_vehicleTypeLimit->addItem((*it).type, QVariant((*it).type));
           }
       }


       for(QList<ComponentsManager::EngineFuelComps>::iterator it = cdl.engineFuelList.begin(); it != cdl.engineFuelList.end(); ++it)
       {
           if((*it).localName == 1 && lm != 0)
           {
             ui->comboBox_e_fuelLimit->addItem(lm->getWord((*it).name.toInt()), QVariant((*it).name));
             ui->comboBox_v_fuelTypes->addItem(lm->getWord((*it).name.toInt()), QVariant((*it).name));
           }
           else
           {
             ui->comboBox_e_fuelLimit->addItem((*it).name, QVariant((*it).name));
             ui->comboBox_v_fuelTypes->addItem((*it).name, QVariant((*it).name));
           }
       }

       for(QList<ComponentsManager::EngineLayoutComps>::iterator it = cdl.engineLayoutList.begin(); it != cdl.engineLayoutList.end(); ++it)
       {
           if((*it).localName == 1 && lm != 0)
           {
             ui->comboBox_e_LayoutLimits->addItem(lm->getWord((*it).name.toInt()), QVariant((*it).name));
           }
           else
           {
             ui->comboBox_e_LayoutLimits->addItem((*it).name, QVariant((*it).name));
           }
       }


       for(QList<ComponentsManager::EngineCylinderComps>::iterator it = cdl.engineCylinderList.begin(); it != cdl.engineCylinderList.end(); ++it)
       {
           if((*it).localName == 1 && lm != 0)
           {
             ui->comboBox_e_CylinderLimit->addItem(lm->getWord((*it).name.toInt()), QVariant((*it).name));
           }
           else
           {
             ui->comboBox_e_CylinderLimit->addItem((*it).name, QVariant((*it).name));
           }
       }

        ui->comboBox_e_contractType->addItem("Traditional Engine", QVariant("2"));
        ui->comboBox_e_contractType->addItem("Aircraft Engine", QVariant("3"));
        ui->comboBox_e_contractType->addItem("Ship Engine", QVariant("4"));


       for(QList<ComponentsManager::GearBoxComps>::iterator it = cdl.gearboxTypeList.begin(); it != cdl.gearboxTypeList.end(); ++it)
       {
           if((*it).localName == 1 && lm != 0)
           {
             ui->comboBox_g_typeLimit->addItem(lm->getWord((*it).name.toInt()), QVariant((*it).name));
           }
           else
           {
             ui->comboBox_g_typeLimit->addItem((*it).name, QVariant((*it).name));
           }
       }


       for(QList<ComponentsManager::GearGearsComps>::iterator it = cdl.gearboxGearsList.begin(); it != cdl.gearboxGearsList.end(); ++it)
       {
           if((*it).localName == 1 && lm != 0)
           {
             ui->comboBox_g_GearLimits->addItem(lm->getWord((*it).name.toInt()), QVariant((*it).name));
           }
           else
           {
             ui->comboBox_g_GearLimits->addItem((*it).name, QVariant((*it).name));
           }
       }





    }


}

void ContractInformationEditor::on_button_SelectLocalizeFile_clicked()
{
    QString openFileName =  QFileDialog::getOpenFileName(this, "Open Localization File", "",
                                                         "XML Files (*.xml)");

    if (openFileName != "")
    {
        //Create localization mananger
        lm = new LocalizationManager(openFileName, cp_wsc.ContractsCW);

        //If localization has words
        if(lm->size()!=0)
        {
            ui->label_selectedLocalizedFile->setText(openFileName);
            refillCombos();
        }
    }
    else
    {
        return;
    }
}

void ContractInformationEditor::on_button_v_addFuelLimit_clicked()
{

    QList<QListWidgetItem*> find = ui->listWidget_v_fuelLimit->findItems(
                ui->lineEdit_v_fuelName->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_v_fuelLimit->addItem(ui->lineEdit_v_fuelName->text());
    }
    else
    {
        QMessageBox::critical(this,"This fuel type restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }

}

void ContractInformationEditor::on_button_v_vehicleTypeAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_v_vehicleTypeLimit->findItems(
                ui->lineEdit_v_vehicleTypeName->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_v_vehicleTypeLimit->addItem(ui->lineEdit_v_vehicleTypeName->text());
    }
    else
    {
        QMessageBox::critical(this,"This vehicle type restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }
}

void ContractInformationEditor::on_button_v_removeFuelLimit_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_v_fuelLimit->findItems(
                ui->lineEdit_v_fuelName->text(),Qt::MatchExactly);
    
    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_v_fuelLimit->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_button_v_vehicleTypeRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_v_vehicleTypeLimit->findItems(
                ui->lineEdit_v_vehicleTypeName->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_v_vehicleTypeLimit->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_comboBox_v_fuelTypes_currentIndexChanged(int index)
{
    ui->lineEdit_v_fuelName->setText(ui->comboBox_v_fuelTypes->itemData(index).toString());
}

void ContractInformationEditor::on_comboBox_v_vehicleTypeLimit_currentIndexChanged(int index)
{
     ui->lineEdit_v_vehicleTypeName->setText(ui->comboBox_v_vehicleTypeLimit->itemData(index).toString());
}



void ContractInformationEditor::on_listWidget_v_fuelLimit_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_v_fuelName->setText(item->text());
    int itemID = ui->comboBox_v_fuelTypes->findData(item->text());
    ui->comboBox_v_fuelTypes->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_listWidget_v_vehicleTypeLimit_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_v_vehicleTypeName->setText(item->text());
    int itemID = ui->comboBox_v_vehicleTypeLimit->findData(item->text());
    ui->comboBox_v_vehicleTypeLimit->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_button_v_addContract_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_v_vehicleContracts->findItems(
                ui->lineEdit_v_ContractFor->text(),Qt::MatchExactly);

    if(find.size() != 0)
    {
        QMessageBox::critical(this,"This vehicle contract already exists!",
                              "You can not duplicate vehicle contract names");

        return;
    }

    ContractManager::vehicleContract vc;


    
    vc.name = ui->lineEdit_v_ContractFor->text();
    vc.type = 1;
    vc.military = ui->checkBox_v_Military->isChecked();
    vc.start = ui->spinBox_v_StartYear->value();
    vc.end = ui->spinBox_v_EndYear->value();
    vc.ccMod = ui->spinBox_v_ccmod->value();
    vc.weightMod = ui->spinBox_v_weightMod->value();
    vc.fuelMod = ui->spinBox_v_fuelMod->value();
    vc.hpMod = ui->spinBox_v_hpmod->value();
    vc.cargoMod = ui->spinBox_v_cargoMod->value();
    vc.latgMod = ui->spinBox_v_latGMod->value();
    vc.torqueMod = ui->spinBox_v_torqueMod->value();
    vc.topSpeedMod = ui->spinBox_v_topSpeedMod->value();
    vc.contractSize = ui->spinBox_v_contractSizeMod->value();
    vc.contractPrice = ui->spinBox_v_contractSizeMod->value();

    for(int i = 0; i <  ui->listWidget_v_fuelLimit->count(); i++)
    {
        vc.fuelLimit.push_back( ui->listWidget_v_fuelLimit->item(i)->text());
    }

    for(int i = 0; i <  ui->listWidget_v_vehicleTypeLimit->count(); i++)
    {
        vc.vehicleTypeLimit.push_back( ui->listWidget_v_vehicleTypeLimit->item(i)->text());
    }


    vc.localizeName = ui->checkBox_v_LocalizeName->isChecked();
    vc.localizeFuel = ui->checkBox_v_fuelLimitLocalization->isChecked();
    vc.localizeType = ui->checkBox_v_vehicleTypeLocalize->isChecked();

    vcEditorList.push_back(vc);
     ui->listWidget_v_vehicleContracts->addItem(ui->lineEdit_v_ContractFor->text());
}

void ContractInformationEditor::on_pushButton_v_removeContract_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_v_vehicleContracts->findItems(
                ui->lineEdit_v_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_v_vehicleContracts->removeItemWidget((*it));
        delete (*it);

        for(QList<ContractManager::vehicleContract>::iterator itB = vcEditorList.begin();
            itB != vcEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_v_ContractFor->text())
            {
                vcEditorList.erase(itB);
                break;
            }
        }
    }
}



void ContractInformationEditor::on_button_v_editContract_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_v_vehicleContracts->findItems(
                ui->lineEdit_v_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {

        for(QList<ContractManager::vehicleContract>::iterator itB = vcEditorList.begin();
            itB != vcEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_v_ContractFor->text())
            {
                (*itB).type = 1;
                (*itB).military = ui->checkBox_v_Military->isChecked();
                (*itB).start = ui->spinBox_v_StartYear->value();
                (*itB).end = ui->spinBox_v_EndYear->value();
                (*itB).ccMod = ui->spinBox_v_ccmod->value();
                (*itB).weightMod = ui->spinBox_v_weightMod->value();
                (*itB).fuelMod = ui->spinBox_v_fuelMod->value();
                (*itB).hpMod = ui->spinBox_v_hpmod->value();
                (*itB).cargoMod = ui->spinBox_v_cargoMod->value();
                (*itB).latgMod = ui->spinBox_v_latGMod->value();
                (*itB).torqueMod = ui->spinBox_v_torqueMod->value();
                (*itB).topSpeedMod = ui->spinBox_v_topSpeedMod->value();
                (*itB).contractSize = ui->spinBox_v_contractSizeMod->value();
                (*itB).contractPrice = ui->spinBox_v_contractSizeMod->value();

                for(int i = 0; i <  ui->listWidget_v_fuelLimit->count(); i++)
                {
                    (*itB).fuelLimit.push_back( ui->listWidget_v_fuelLimit->item(i)->text());
                }

                for(int i = 0; i <  ui->listWidget_v_vehicleTypeLimit->count(); i++)
                {
                    (*itB).vehicleTypeLimit.push_back( ui->listWidget_v_vehicleTypeLimit->item(i)->text());
                }


                (*itB).localizeName = ui->checkBox_v_LocalizeName->isChecked();
                (*itB).localizeFuel = ui->checkBox_v_fuelLimitLocalization->isChecked();
                (*itB).localizeType = ui->checkBox_v_vehicleTypeLocalize->isChecked();

            }
        }
    }
}

void ContractInformationEditor::on_listWidget_v_vehicleContracts_itemClicked(QListWidgetItem *item)
{
    for(QList<ContractManager::vehicleContract>::iterator it = vcEditorList.begin();
        it != vcEditorList.end(); ++it)
    {
      if((*it).name == item->text())
      {
        ui->lineEdit_v_ContractFor->setText((*it).name);

        ui->checkBox_v_Military->setChecked((*it).military);
        ui->spinBox_v_StartYear->setValue((*it).start);
        ui->spinBox_v_EndYear->setValue((*it).end);
        ui->spinBox_v_ccmod->setValue((*it).ccMod);
        ui->spinBox_v_weightMod->setValue((*it).weightMod);
        ui->spinBox_v_fuelMod->setValue((*it).fuelMod);
        ui->spinBox_v_hpmod->setValue((*it).hpMod);
        ui->spinBox_v_cargoMod->setValue((*it).cargoMod);
        ui->spinBox_v_latGMod->setValue((*it).latgMod);
        ui->spinBox_v_torqueMod->setValue((*it).torqueMod);
        ui->spinBox_v_topSpeedMod->setValue((*it).topSpeedMod);
        ui->spinBox_v_contractSizeMod->setValue((*it).contractSize);
        ui->spinBox_v_contractSizeMod->setValue((*it).contractPrice);

        ui->listWidget_v_fuelLimit->clear();
        ui->listWidget_v_fuelLimit->addItems((*it).fuelLimit);

        ui->listWidget_v_vehicleTypeLimit->clear();
        ui->listWidget_v_vehicleTypeLimit->addItems((*it).vehicleTypeLimit);

        ui->checkBox_v_LocalizeName->setChecked((*it).localizeName);
        ui->checkBox_v_fuelLimitLocalization->setChecked((*it).localizeFuel);
        ui->checkBox_v_vehicleTypeLocalize->setChecked((*it).localizeType);

        break;
      }
    }
}

void ContractInformationEditor::on_pushButton_e_fuelLimitAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_FuelLimit->findItems(
                ui->lineEdit_e_fuelLimit->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_e_FuelLimit->addItem(ui->lineEdit_e_fuelLimit->text());
    }
    else
    {
        QMessageBox::critical(this,"This engine fuel restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }
}


void ContractInformationEditor::on_button_e_layoutLimitAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_layoutLimit->findItems(
                ui->lineEdit_e_layoutLimitsName->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_e_layoutLimit->addItem(ui->lineEdit_e_layoutLimitsName->text());
    }
    else
    {
        QMessageBox::critical(this,"This engine layout restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }
}

void ContractInformationEditor::on_button_e_cylinderLimitAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_cylinderLimit->findItems(
                ui->lineEdit_e_cylinderLimitName->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_e_cylinderLimit->addItem(ui->lineEdit_e_cylinderLimitName->text());
    }
    else
    {
        QMessageBox::critical(this,"This engine layout restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }
}

void ContractInformationEditor::on_pushButton_e_fuelLimitRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_FuelLimit->findItems(
                ui->lineEdit_e_fuelLimit->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_e_FuelLimit->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_button_e_layoutLimitsRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_layoutLimit->findItems(
                ui->lineEdit_e_layoutLimitsName->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_e_layoutLimit->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_button_e_cylinderLimitRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_cylinderLimit->findItems(
                ui->lineEdit_e_cylinderLimitName->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_e_cylinderLimit->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_comboBox_e_fuelLimit_currentIndexChanged(int index)
{
     ui->lineEdit_e_fuelLimit->setText(ui->comboBox_e_fuelLimit->itemData(index).toString());
}

void ContractInformationEditor::on_comboBox_e_LayoutLimits_currentIndexChanged(int index)
{
     ui->lineEdit_e_layoutLimitsName->setText(ui->comboBox_e_LayoutLimits->itemData(index).toString());
}

void ContractInformationEditor::on_comboBox_e_CylinderLimit_currentIndexChanged(int index)
{
     ui->lineEdit_e_cylinderLimitName->setText(ui->comboBox_e_CylinderLimit->itemData(index).toString());
}

void ContractInformationEditor::on_listWidget_e_FuelLimit_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_e_fuelLimit->setText(item->text());
    int itemID = ui->comboBox_e_fuelLimit->findData(item->text());
    ui->comboBox_e_fuelLimit->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_listWidget_e_layoutLimit_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_e_layoutLimitsName->setText(item->text());
    int itemID = ui->comboBox_e_LayoutLimits->findData(item->text());
    ui->comboBox_e_LayoutLimits->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_listWidget_e_cylinderLimit_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_e_cylinderLimitName->setText(item->text());
    int itemID = ui->comboBox_e_CylinderLimit->findData(item->text());
    ui->comboBox_e_CylinderLimit->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_pushButton_e_contractAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_contracts->findItems(
                ui->lineEdit_e_ContractFor->text(),Qt::MatchExactly);

    if(find.size() != 0)
    {
        QMessageBox::critical(this,"This engine contract already exists!",
                              "You can not duplicate vehicle contract names");

        return;
    }

    ContractManager::engineContract ec;

    ec.name = ui->lineEdit_e_ContractFor->text();


    ec.type = ui->comboBox_e_contractType->itemData(ui->comboBox_e_contractType->currentIndex()).toInt();
    ec.military = ui->checkBox_e_Military->isChecked();
    ec.start = ui->spinBox_e_StartYear->value();
    ec.end = ui->spinBox_e_EndYear->value();
    ec.ccMod = ui->spinBox_e_engineSizeMod->value();
    ec.weightMod = ui->spinBox_e_weightMod->value();
    ec.fuelMod = ui->spinBox_e_fuelMod->value();
    ec.hpMod = ui->spinBox_e_hpMod->value();
    ec.widthMod = ui->spinBox_e_widthMod->value();
    ec.lengthMod = ui->spinBox_e_lengthMod->value();
    ec.torqueMod = ui->spinBox_e_torqueMod->value();
    ec.contractSize = ui->spinBox_e_contractSizeMod->value();
    ec.contractPrice = ui->spinBox_e_contractPaymentMod->value();

    for(int i = 0; i <  ui->listWidget_e_FuelLimit->count(); i++)
    {
     ec.fuelLimit.push_back( ui->listWidget_e_FuelLimit->item(i)->text());
    }

    for(int i = 0; i <  ui->listWidget_e_layoutLimit->count(); i++)
    {
     ec.layoutLimit.push_back( ui->listWidget_e_layoutLimit->item(i)->text());
    }

    for(int i = 0; i <  ui->listWidget_e_cylinderLimit->count(); i++)
    {
     ec.cylinderLimit.push_back( ui->listWidget_e_cylinderLimit->item(i)->text());
    }


    ec.localizeName = ui->checkBox_e_LocalizeName->isChecked();
    ec.localizeFuel = ui->checkBox_e_fuelLimitLocalize->isChecked();
    ec.localizeLayout = ui->checkBox_e_layoutLimitLocalize->isChecked();
    ec.localizeCylinder = ui->checkBox_e_cylinderLimitLocalize->isChecked();

    ecEditorList.push_back(ec);
     ui->listWidget_e_contracts->addItem(ui->lineEdit_e_ContractFor->text());
}

void ContractInformationEditor::on_pushButton_e_contractEdit_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_contracts->findItems(
                ui->lineEdit_e_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {

        for(QList<ContractManager::engineContract>::iterator itB = ecEditorList.begin();
            itB != ecEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_e_ContractFor->text())
            {
                (*itB).type = ui->comboBox_e_contractType->itemData(ui->comboBox_e_contractType->currentIndex()).toInt();
                (*itB).military = ui->checkBox_e_Military->isChecked();
                (*itB).start = ui->spinBox_e_StartYear->value();
                (*itB).end = ui->spinBox_e_EndYear->value();
                (*itB).ccMod = ui->spinBox_e_engineSizeMod->value();
                (*itB).weightMod = ui->spinBox_e_weightMod->value();
                (*itB).fuelMod = ui->spinBox_e_fuelMod->value();
                (*itB).hpMod = ui->spinBox_e_hpMod->value();
                (*itB).widthMod = ui->spinBox_e_widthMod->value();
                (*itB).lengthMod = ui->spinBox_e_lengthMod->value();
                (*itB).torqueMod = ui->spinBox_e_torqueMod->value();
                (*itB).contractSize = ui->spinBox_e_contractSizeMod->value();
                (*itB).contractPrice = ui->spinBox_e_contractPaymentMod->value();

                for(int i = 0; i <  ui->listWidget_e_FuelLimit->count(); i++)
                {
                 (*itB).fuelLimit.push_back( ui->listWidget_e_FuelLimit->item(i)->text());
                }

                for(int i = 0; i <  ui->listWidget_e_layoutLimit->count(); i++)
                {
                 (*itB).layoutLimit.push_back( ui->listWidget_e_layoutLimit->item(i)->text());
                }

                for(int i = 0; i <  ui->listWidget_e_cylinderLimit->count(); i++)
                {
                 (*itB).cylinderLimit.push_back( ui->listWidget_e_cylinderLimit->item(i)->text());
                }


                (*itB).localizeName = ui->checkBox_e_LocalizeName->isChecked();
                (*itB).localizeFuel = ui->checkBox_e_fuelLimitLocalize->isChecked();
                (*itB).localizeLayout = ui->checkBox_e_layoutLimitLocalize->isChecked();
                (*itB).localizeCylinder = ui->checkBox_e_cylinderLimitLocalize->isChecked();

            }
        }
    }
}

void ContractInformationEditor::on_pushButton_e_contractRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_e_contracts->findItems(
                ui->lineEdit_e_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_e_contracts->removeItemWidget((*it));
        delete (*it);

        for(QList<ContractManager::engineContract>::iterator itB = ecEditorList.begin();
            itB != ecEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_e_ContractFor->text())
            {
                ecEditorList.erase(itB);
                break;
            }
        }
    }
}

void ContractInformationEditor::on_listWidget_e_contracts_itemClicked(QListWidgetItem *item)
{
    for(QList<ContractManager::engineContract>::iterator it = ecEditorList.begin();
        it != ecEditorList.end(); ++it)
    {
      if((*it).name == item->text())
      {
          ui->lineEdit_e_ContractFor->setText((*it).name);

          int itemTypeID = ui->comboBox_e_contractType->findData((*it).type);
          ui->comboBox_e_contractType->setCurrentIndex(itemTypeID);

           ui->checkBox_e_Military->setChecked((*it).military);
           ui->spinBox_e_StartYear->setValue((*it).start);
           ui->spinBox_e_EndYear->setValue((*it).end);
           ui->spinBox_e_engineSizeMod->setValue((*it).ccMod);
           ui->spinBox_e_weightMod->setValue((*it).weightMod);
           ui->spinBox_e_fuelMod->setValue((*it).fuelMod);
           ui->spinBox_e_hpMod->setValue((*it).hpMod);
           ui->spinBox_e_widthMod->setValue((*it).widthMod);
           ui->spinBox_e_lengthMod->setValue((*it).lengthMod);
           ui->spinBox_e_torqueMod->setValue((*it).torqueMod);
           ui->spinBox_e_contractSizeMod->setValue((*it).contractSize);
           ui->spinBox_e_contractPaymentMod->setValue((*it).contractPrice);

            ui->listWidget_e_FuelLimit->clear();
            ui->listWidget_e_FuelLimit->addItems((*it).fuelLimit);

            ui->listWidget_e_layoutLimit->clear();
            ui->listWidget_e_layoutLimit->addItems((*it).layoutLimit);

            ui->listWidget_e_cylinderLimit->clear();
            ui->listWidget_e_cylinderLimit->addItems((*it).cylinderLimit);


            ui->checkBox_e_LocalizeName->setChecked((*it).localizeName );
            ui->checkBox_e_fuelLimitLocalize->setChecked((*it).localizeFuel);
            ui->checkBox_e_layoutLimitLocalize->setChecked( (*it).localizeLayout);
            ui->checkBox_e_cylinderLimitLocalize->setChecked((*it).localizeCylinder);


          break;
      }
    }
}

void ContractInformationEditor::on_pushButton_c_contractsAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_c_contracts->findItems(
                ui->lineEdit_c_ContractFor->text(),Qt::MatchExactly);

    if(find.size() != 0)
    {
        QMessageBox::critical(this,"This chassis contract already exists!",
                              "You can not duplicate vehicle contract names");

        return;
    }

    ContractManager::chassisContract cc;

    cc.name = ui->lineEdit_c_ContractFor->text();

    cc.type = 1;
    cc.military = ui->checkBox_c_Military->isChecked();
    cc.start = ui->spinBox_c_StartYear->value();
    cc.end = ui->spinBox_c_EndYear->value();
    cc.weightMod = ui->spinnerBox_c_WeightMod->value();
    cc.minWidth = ui->spinnerBox_c_minWidth->value();
    cc.maxWidth = ui->spinnerBox_c_maxWidth->value();
    cc.minLength = ui->spinnerBox_c_minLength->value();
    cc.maxLength = ui->spinnerBox_c_maxLength->value();
    cc.contractSize = ui->spinnerBox_c_contractSize->value();
    cc.contractPrice = ui->spinnerBox_c_contractPayment->value();

    cc.localizeName = ui->checkBox_c_LocalizeName->isChecked();

    ccEditorList.push_back(cc);
    ui->listWidget_c_contracts->addItem(ui->lineEdit_c_ContractFor->text());
}

void ContractInformationEditor::on_pushButton_c_contractsEdit_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_c_contracts->findItems(
                ui->lineEdit_c_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {

        for(QList<ContractManager::chassisContract>::iterator itB = ccEditorList.begin();
            itB != ccEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_c_ContractFor->text())
            {
                (*itB).name = ui->lineEdit_c_ContractFor->text();

                (*itB).type = 1;
                (*itB).military = ui->checkBox_c_Military->isChecked();
                (*itB).start = ui->spinBox_c_StartYear->value();
                (*itB).end = ui->spinBox_c_EndYear->value();
                (*itB).weightMod = ui->spinnerBox_c_WeightMod->value();
                (*itB).minWidth = ui->spinnerBox_c_minWidth->value();
                (*itB).maxWidth = ui->spinnerBox_c_maxWidth->value();
                (*itB).minLength = ui->spinnerBox_c_minLength->value();
                (*itB).maxLength = ui->spinnerBox_c_maxLength->value();
                (*itB).contractSize = ui->spinnerBox_c_contractSize->value();
                (*itB).contractPrice = ui->spinnerBox_c_contractPayment->value();

                (*itB).localizeName = ui->checkBox_c_LocalizeName->isChecked();

            }
        }
    }
}

void ContractInformationEditor::on_pushButton_c_contractsRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_c_contracts->findItems(
                ui->lineEdit_c_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_c_contracts->removeItemWidget((*it));
        delete (*it);

        for(QList<ContractManager::chassisContract>::iterator itB = ccEditorList.begin();
            itB != ccEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_c_ContractFor->text())
            {
                ccEditorList.erase(itB);
                break;
            }
        }
    }
}

void ContractInformationEditor::on_listWidget_c_contracts_itemClicked(QListWidgetItem *item)
{
    for(QList<ContractManager::chassisContract>::iterator it = ccEditorList.begin();
        it != ccEditorList.end(); ++it)
    {
      if((*it).name == item->text())
      {
           ui->lineEdit_c_ContractFor->setText((*it).name);


           ui->checkBox_c_Military->setChecked((*it).military);
           ui->spinBox_c_StartYear->setValue((*it).start);
           ui->spinBox_c_EndYear->setValue((*it).end);
           ui->spinnerBox_c_WeightMod->setValue((*it).weightMod);
           ui->spinnerBox_c_minWidth->setValue((*it).minWidth);
           ui->spinnerBox_c_maxWidth->setValue((*it).maxWidth);
           ui->spinnerBox_c_minLength->setValue((*it).minLength);
           ui->spinnerBox_c_maxLength->setValue((*it).maxLength);
           ui->spinnerBox_c_contractSize->setValue((*it).contractSize);
           ui->spinnerBox_c_contractPayment->setValue((*it).contractPrice);

           ui->checkBox_c_LocalizeName->setChecked( (*it).localizeName);

      }
    }
}

void ContractInformationEditor::on_button_g_GearLimitsAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_GearLimits->findItems(
                ui->lineEdit_g_GearLimitsName->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_g_GearLimits->addItem(ui->lineEdit_g_GearLimitsName->text());
    }
    else
    {
        QMessageBox::critical(this,"This gears restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }
}


void ContractInformationEditor::on_button_g_typeLimitAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_TypeLimit->findItems(
                ui->lineEdit_g_typeLimitName->text(),Qt::MatchExactly);

    if(find.size() == 0)
    {
        ui->listWidget_g_TypeLimit->addItem(ui->lineEdit_g_typeLimitName->text());
    }
    else
    {
        QMessageBox::critical(this,"This gearbox type restriction already exists!",
                              "You can not duplicate names in the restriction lists");

        return;
    }
}

void ContractInformationEditor::on_button_g_GearLimitsRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_GearLimits->findItems(
                ui->lineEdit_g_GearLimitsName->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_g_GearLimits->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_button_g_typeLimitRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_TypeLimit->findItems(
                ui->lineEdit_g_typeLimitName->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_g_TypeLimit->removeItemWidget((*it));
        delete (*it);
    }
}

void ContractInformationEditor::on_comboBox_g_GearLimits_currentIndexChanged(int index)
{
     ui->lineEdit_g_GearLimitsName->setText(ui->comboBox_g_GearLimits->itemData(index).toString());
}

void ContractInformationEditor::on_comboBox_g_typeLimit_currentIndexChanged(int index)
{
     ui->lineEdit_g_typeLimitName->setText(ui->comboBox_g_typeLimit->itemData(index).toString());
}

void ContractInformationEditor::on_listWidget_g_GearLimits_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_g_GearLimitsName->setText(item->text());
    int itemID = ui->comboBox_g_GearLimits->findData(item->text());
    ui->comboBox_g_GearLimits->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_listWidget_g_TypeLimit_itemClicked(QListWidgetItem *item)
{
    ui->lineEdit_g_typeLimitName->setText(item->text());
    int itemID = ui->comboBox_g_typeLimit->findData(item->text());
    ui->comboBox_g_typeLimit->setCurrentIndex(itemID);
}

void ContractInformationEditor::on_button_g_contractsAdd_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_contracts->findItems(
                ui->lineEdit_g_ContractFor->text(),Qt::MatchExactly);

    if(find.size() != 0)
    {
        QMessageBox::critical(this,"This gearbox contract already exists!",
                              "You can not duplicate vehicle contract names");

        return;
    }

    ContractManager::gearboxContract gc;

    gc.name = ui->lineEdit_g_ContractFor->text();
    gc.type = 1;
    gc.military = ui->checkBox_g_Military->isChecked();
    gc.start = ui->spinBox_g_StartYear->value();
    gc.end = ui->spinBox_g_EndYear->value();
    gc.weightMod = ui->spinner_g_WeightMod->value();
    gc.contractSize = ui->spinner_g_ContractSize->value();
    gc.contractPrice = ui->spinner_g_ContractPayment->value();

    for(int i = 0; i <  ui->listWidget_g_GearLimits->count(); i++)
    {
     gc.gearLimit.push_back(ui->listWidget_g_GearLimits->item(i)->text());
    }

    for(int i = 0; i <  ui->listWidget_g_TypeLimit->count(); i++)
    {
     gc.typeLimit.push_back(ui->listWidget_g_TypeLimit->item(i)->text());;
    }

    gc.localizeName = ui->checkBox_g_LocalizeName->isChecked();
    gc.localizeGear = ui->checkBox_g_GearLimitLocalize->isChecked();
    gc.localizeType = ui->checkBox_g_typeLimitLocalize->isChecked();

    gcEditorList.push_back(gc);
     ui->listWidget_g_contracts->addItem(ui->lineEdit_g_ContractFor->text());
}


void ContractInformationEditor::on_button_g_contractsEdit_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_contracts->findItems(
                ui->lineEdit_g_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {

        for(QList<ContractManager::gearboxContract>::iterator itB = gcEditorList.begin();
            itB != gcEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_g_ContractFor->text())
            {
                (*itB).name = ui->lineEdit_g_ContractFor->text();
                (*itB).type = 1;
                (*itB).military = ui->checkBox_g_Military->isChecked();
                (*itB).start = ui->spinBox_g_StartYear->value();
                (*itB).end = ui->spinBox_g_EndYear->value();
                (*itB).weightMod = ui->spinner_g_WeightMod->value();
                (*itB).contractSize = ui->spinner_g_ContractSize->value();
                (*itB).contractPrice = ui->spinner_g_ContractPayment->value();

                for(int i = 0; i <  ui->listWidget_g_GearLimits->count(); i++)
                {
                 (*itB).gearLimit.push_back(ui->listWidget_g_GearLimits->item(i)->text());
                }

                for(int i = 0; i <  ui->listWidget_g_TypeLimit->count(); i++)
                {
                 (*itB).typeLimit.push_back(ui->listWidget_g_TypeLimit->item(i)->text());;
                }

                (*itB).localizeName = ui->checkBox_g_LocalizeName->isChecked();
                (*itB).localizeGear = ui->checkBox_g_GearLimitLocalize->isChecked();
                (*itB).localizeType = ui->checkBox_g_typeLimitLocalize->isChecked();

            }
        }
    }
}


void ContractInformationEditor::on_button_g_contractsRemove_clicked()
{
    QList<QListWidgetItem*> find = ui->listWidget_g_contracts->findItems(
                ui->lineEdit_g_ContractFor->text(),Qt::MatchExactly);

    for(QList<QListWidgetItem*>::iterator it = find.begin(); it != find.end(); ++it )
    {
        ui->listWidget_g_contracts->removeItemWidget((*it));
        delete (*it);

        for(QList<ContractManager::gearboxContract>::iterator itB = gcEditorList.begin();
            itB != gcEditorList.end(); ++itB)
        {
            if((*itB).name ==  ui->lineEdit_g_ContractFor->text())
            {
                gcEditorList.erase(itB);
                break;
            }
        }
    }
}


void ContractInformationEditor::on_listWidget_g_contracts_itemClicked(QListWidgetItem *item)
{
    for(QList<ContractManager::gearboxContract>::iterator it = gcEditorList.begin();
        it != gcEditorList.end(); ++it)
    {
      if((*it).name == item->text())
      {
          ui->lineEdit_g_ContractFor->setText((*it).name);

          ui->lineEdit_g_ContractFor->setText((*it).name);

          ui->checkBox_g_Military->setChecked((*it).military);
          ui->spinBox_g_StartYear->setValue((*it).start);
          ui->spinBox_g_EndYear->setValue((*it).end);
          ui->spinner_g_WeightMod->setValue((*it).weightMod);
          ui->spinner_g_ContractSize->setValue((*it).contractSize);
          ui->spinner_g_ContractPayment->setValue((*it).contractPrice);


          ui->listWidget_g_GearLimits->addItems((*it).gearLimit);
          ui->listWidget_g_TypeLimit->addItems((*it).typeLimit);


          ui->checkBox_g_LocalizeName->setChecked((*it).localizeName);
          ui->checkBox_g_GearLimitLocalize->setChecked((*it).localizeGear);
          ui->checkBox_g_typeLimitLocalize->setChecked((*it).localizeType);
          break;
      }
    }

}

void ContractInformationEditor::on_button_NewContractsFile_clicked()
{
    vcEditorList.clear();
    ecEditorList.clear();
    ccEditorList.clear();
    gcEditorList.clear();

    ui->listWidget_c_contracts->clear();
    ui->listWidget_e_contracts->clear();
    ui->listWidget_g_contracts->clear();
    ui->listWidget_v_vehicleContracts->clear();
}



void ContractInformationEditor::on_button_SaveContractsFile_clicked()
{
    QString saveContractsFileName = QFileDialog::getSaveFileName(this, "Save Contracts File",
                                                          "","XML Files (*.xml)");

    if (saveContractsFileName != "")
    {
        if (!saveContractsFileName.endsWith(".xml"))
            saveContractsFileName += ".xml";

        ContractManager contMan;

        contMan.saveContractsXML(saveContractsFileName, vcEditorList,
        ecEditorList, ccEditorList,gcEditorList);
    }

}


void ContractInformationEditor::on_button_OpenContracts_clicked()
{
    QString fileName =  QFileDialog::getOpenFileName(this, "Open Existing Contract File",
                                                           "", "XML Files (*.xml)");
    if (fileName != "")
    {
        ContractManager contMan;
        QString errorBody = "", errorHead = "";
        if(contMan.openContractsXML(fileName,errorHead, errorBody))
        {
            vcEditorList = contMan.getVCList();
            ecEditorList = contMan.getECList();
            ccEditorList = contMan.getCCList();
            gcEditorList = contMan.getGCList();


            for( QList<ContractManager::vehicleContract>::iterator it = vcEditorList.begin();
                 it != vcEditorList.end(); ++it)
            {
                ui->listWidget_v_vehicleContracts->addItem((*it).name);
            }

            for( QList<ContractManager::engineContract>::iterator it = ecEditorList.begin();
                 it != ecEditorList.end(); ++it)
            {
                 ui->listWidget_e_contracts->addItem((*it).name);
            }

            for( QList<ContractManager::chassisContract>::iterator it = ccEditorList.begin();
                 it != ccEditorList.end(); ++it)
            {
                 ui->listWidget_c_contracts->addItem((*it).name);
            }

            for( QList<ContractManager::gearboxContract>::iterator it = gcEditorList.begin();
                 it != gcEditorList.end(); ++it)
            {
                 ui->listWidget_g_contracts->addItem((*it).name);
            }


        }
        else
        {
            QMessageBox::critical(this,errorHead,errorBody);

        }

    }
}
