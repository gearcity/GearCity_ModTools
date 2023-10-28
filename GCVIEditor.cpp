#include "GCVIEditor.h"
#include "ui_GCVIEditor.h"
#include <QtWidgets/QMessageBox>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtWidgets/QFileDialog>
#include <QStandardItemModel>
#include <QListView>
#include <QProcess>

GCVIEditor::GCVIEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCVIEditor)
{
    ui->setupUi(this);
    cp_wsc = wsc;

    gData = new GCVIData();

}

GCVIEditor::~GCVIEditor()
{
    delete ui;
    delete gData;

    gData = 0;
}


void GCVIEditor::on_Button_new_clicked()
{
    if(gData != NULL)
    {
        delete gData;
        gData = new GCVIData();
    }

    ui->checkBox_ModelDefaultActive->setChecked(false);
    ui->doubleSpinBox_Animation_HighRange->setValue(1);
    ui->doubleSpinBox_Animation_LowRange->setValue(0);
    ui->doubleSpinBox_Preset_AnimationValue->setValue(1);
    ui->label_CategoryImageFileName->setText("None Selected");
    ui->label_ModelImageFileName->setText("None Selected");
    ui->label_Preset_ImageFileName->setText("None Selected");
    ui->label_SelectedSceneFileName->setText("None Selected");
    ui->label_SubCategory_ImageSelected->setText("None Selected");
    ui->lineEdit_Animation_Name->setText("");
    ui->lineEdit_CategoryName->setText("");
    ui->lineEdit_GCVIName->setText("");
    ui->lineEdit_GCVI_Subtype->setText("");
    ui->lineEdit_GCVI_Type->setText("");
    ui->lineEdit_ModelName->setText("");
    ui->lineEdit_Preset_Name->setText("");
    ui->lineEdit_SubCategory_Name->setText("");
    ui->listWidget_Animations_AssignedAnimations->clear();
    ui->listWidget_Animations_AvaliableAnimations->clear();
    ui->listWidget_Models_Assigned->clear();
    ui->listWidget_Models_Avaliable->clear();
    ui->listWidget_Presets_PresetList->clear();
    ui->listWidget_Preset_AssignedAnimations->clear();
    ui->listWidget_Preset_AvaliableAnimations->clear();
    ui->ListWidget_Categories->clear();

    ui->spinBox_Animations_LinkedID->setValue(0);
    ui->spinBox_Animations_Localization->setValue(0);
    ui->spinBox_Category_Localization->setValue(0);
    ui->spinBox_Model_Localization->setValue(0);
    ui->spinBox_Model_Type->setValue(0);
    ui->spinBox_Presets_LinkedID->setValue(0);
    ui->spinBox_Presets_Localization->setValue(0);
    ui->spinBox_SubCategory_Localization->setValue(0);

}


void GCVIEditor::on_button_open_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","GCVI Files (*.gcvi)");

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
           QMessageBox::critical(this,"Error GCVI Script",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return;
        }

        //Begin Reading and putting the data into the map
        QDomElement rootNode = doc.firstChildElement("gcvi");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","GCVI Script is malformed.");
            return;
        }

        //Get first city, and lets go
        //rootNode = doc.documentElement();
        Element = rootNode.firstChildElement("name");

        if(Element.isNull())
            return;

        ui->lineEdit_GCVIName->setText(Element.text());
        gData->name = Element.text();

        Element = rootNode.firstChildElement("type");
        if(Element.isNull())
            return;

        ui->lineEdit_GCVI_Type->setText(Element.text());
        gData->type = Element.text();



        Element = rootNode.firstChildElement("subtype");
        if(Element.isNull())
            return;

        ui->lineEdit_GCVI_Subtype->setText(Element.text());
         gData->subtype = Element.text();

        Element = rootNode.firstChildElement("scenefile");
        if(Element.isNull())
            return;

        ui->label_SelectedSceneFileName->setText(Element.text());
         gData->sceneFile = Element.text();

        Element = rootNode.firstChildElement("category");

        QDomElement moElement;
        QDomElement scElement;
        QDomElement nodeElement;
        QDomElement animeMElement;
        QDomElement presetElement;
        QDomElement dmElement;
        do
        {

            GCVIData::GCVICategories cat;
            cat.name = Element.attribute("name");
            cat.image = Element.attribute("image");
            cat.localize = Element.attribute("localize");

            moElement = Element.firstChildElement("modelOptions");

            ui->ListWidget_Categories->addItem("cat.name");

            do
            {
                GCVIData::GCVICategories::GCVIModelOptions modO;
                modO.name = moElement.attribute("name");
                modO.image = moElement.attribute("image");
                modO.localize = moElement.attribute("localize");
                modO.type = moElement.attribute("type");
                modO.nodeName = moElement.attribute("nodeName");
                modO.defaultActive = moElement.attribute("defaultActive");
                modO.EntityName = moElement.attribute("EntityName");

                scElement = moElement.firstChildElement("subcategory");

                do
                {
                    modO.subName = scElement.attribute("name");
                    modO.subImage = scElement.attribute("image");
                    modO.subLocalize = scElement.attribute("localize");

                    nodeElement = scElement.firstChildElement("node");
                    do
                    {

                        dmElement = nodeElement.firstChildElement("displayMorph");
                        do
                        {
                            GCVIData::GCVICategories::GCVIModelOptions::GCVIAnimation animations;
                            animations.name = dmElement.attribute("name");
                            animations.image = dmElement.attribute("image");
                            animations.localize = dmElement.attribute("localize");
                            animations.lowRange = dmElement.attribute("lowRange");
                            animations.highRange = dmElement.attribute("highRange");
                            animations.LinkID = dmElement.attribute("id");
                            animations.animationName = dmElement.text();


                            modO.animations.push_back(animations);
                          dmElement = dmElement.nextSiblingElement("displayMorph");
                        }
                        while(!dmElement.isNull());

                        presetElement = nodeElement.firstChildElement("preset");
                        do
                        {
                            GCVIData::GCVICategories::GCVIModelOptions::GCVIPreset presetInfo;

                            presetInfo.name = presetElement.attribute("name");
                            presetInfo.image = presetElement.attribute("image");
                            presetInfo.localize = presetElement.attribute("localize");




                            animeMElement = presetElement.firstChildElement("animate");
                            do
                            {

                              GCVIData::GCVICategories::GCVIModelOptions::GCVIPreset::GCVIPresetAnimations panime;
                              panime.morphName = animeMElement.attribute("morphName");
                              panime.LinkID = animeMElement.attribute("id");
                              panime.animationValue = animeMElement.text();

                              presetInfo.presetAnimations.push_back(panime);

                              animeMElement = presetElement.nextSiblingElement("animate");
                            }
                            while(!animeMElement.isNull());

                            modO.presets.push_back(presetInfo);

                          presetElement = presetElement.nextSiblingElement("preset");
                        }
                        while(!presetElement.isNull());


                      nodeElement = nodeElement.nextSiblingElement("node");
                    }
                    while(!nodeElement.isNull());

                  scElement = scElement.nextSiblingElement("subcategory");
                }
                while(!scElement.isNull());

                cat.models.push_back(modO);

              moElement = moElement.nextSiblingElement("modelOptions");
            }
            while(!moElement.isNull());

          gData->guiData.push_back(cat);

          Element = Element.nextSiblingElement("category");
        }
        while(!Element.isNull());

        file.close();

        if(!loadSceneFile(QFileInfo(file).path(), gData->sceneFile))
        {
            QMessageBox::critical(this,"Error","Scene file can't be found or is malformed.");
        }

    }
    else
    {
        return;
    }
}


void GCVIEditor::on_button_save_clicked()
{
   QString saveFileName = QFileDialog::getSaveFileName(this, "Save File", "","GCVI Files (*.gcvi)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".gcvi"))
            saveFileName += ".gcvi";

        //Save the city script xml
        saveXML(saveFileName);
    }
}

void GCVIEditor::saveXML(QString saveFileName)
{
    //Open/Create the save file
    QFile saveGCVIFile(saveFileName);
    if(!saveGCVIFile.open(QFile::WriteOnly | QFile::Text))
    {
       QMessageBox::critical(this,"Error",
                             QString("Can not write to %1\n%2").arg(saveFileName,
                             saveGCVIFile.errorString()));

       return;
    }


    //Lets Start Writing!
    QXmlStreamWriter xmlWriter(&saveGCVIFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("gcvi");

    xmlWriter.writeStartElement("name");
        xmlWriter.writeCharacters(gData->name);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("type");
        xmlWriter.writeCharacters(gData->type);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("subtype");
        xmlWriter.writeCharacters(gData->subtype);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("scenefile");
        xmlWriter.writeCharacters(gData->sceneFile);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("gui");
        for(const GCVIData::GCVICategories& c : gData->guiData)
        {
            xmlWriter.writeStartElement("category");
                xmlWriter.writeAttribute("name",c.name);
                xmlWriter.writeAttribute("image",c.image);
                xmlWriter.writeAttribute("localize",c.localize);

                for(const GCVIData::GCVICategories::GCVIModelOptions& mo : c.models)
                {
                    xmlWriter.writeStartElement("modelOptions");
                        xmlWriter.writeAttribute("name",mo.name);
                        xmlWriter.writeAttribute("image",mo.image);
                        xmlWriter.writeAttribute("localize",mo.localize);
                        xmlWriter.writeAttribute("type",mo.type);
                        xmlWriter.writeAttribute("nodeName",mo.nodeName);
                        xmlWriter.writeAttribute("EntityName",mo.EntityName);
                        xmlWriter.writeAttribute("defaultActive",mo.defaultActive);

                        xmlWriter.writeStartElement("subcategory");
                            xmlWriter.writeAttribute("name",mo.subName);
                            xmlWriter.writeAttribute("image",mo.subImage);
                            xmlWriter.writeAttribute("localize",mo.subLocalize);

                            xmlWriter.writeStartElement("node");
                                xmlWriter.writeAttribute("name",mo.nodeName);
                                for(const GCVIData::GCVICategories::GCVIModelOptions::GCVIAnimation& am : mo.animations)
                                {
                                    xmlWriter.writeStartElement("displayMorph");
                                        xmlWriter.writeAttribute("name",am.name);
                                        xmlWriter.writeAttribute("image",am.image);
                                        xmlWriter.writeAttribute("localize",am.localize);
                                        xmlWriter.writeAttribute("lowRange",am.lowRange);
                                        xmlWriter.writeAttribute("highRange",am.highRange);
                                        xmlWriter.writeAttribute("id",am.LinkID);
                                        xmlWriter.writeCharacters(am.animationName);
                                    xmlWriter.writeEndElement();
                                }

                                for(const GCVIData::GCVICategories::GCVIModelOptions::GCVIPreset& presets : mo.presets)
                                {
                                    xmlWriter.writeStartElement("preset");
                                        xmlWriter.writeAttribute("name",presets.name);
                                        xmlWriter.writeAttribute("image",presets.image);
                                        xmlWriter.writeAttribute("localize",presets.localize);
                                        for(const GCVIData::GCVICategories::GCVIModelOptions::GCVIPreset::GCVIPresetAnimations& preAnime : presets.presetAnimations)
                                        {
                                            xmlWriter.writeStartElement("animate");
                                                xmlWriter.writeAttribute("morphName",preAnime.morphName);
                                                xmlWriter.writeAttribute("id",preAnime.LinkID);
                                                xmlWriter.writeCharacters(preAnime.animationValue);
                                            xmlWriter.writeEndElement();
                                        }

                                    xmlWriter.writeEndElement();
                                }

                            xmlWriter.writeEndElement();
                        xmlWriter.writeEndElement();

                    xmlWriter.writeEndElement();
                }
            xmlWriter.writeEndElement();
        }
    xmlWriter.writeEndElement();

     xmlWriter.writeEndDocument();

     saveGCVIFile.close();

     //We Did it! Yay!
     QMessageBox::information(this,"Save Complete",
                              "Your GCVI file has been saved!", QMessageBox::Ok);
}

void GCVIEditor::on_button_ReturnToMain_clicked()
{
    cp_wsc.GCVIEditorCW->lower();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}


void GCVIEditor::on_toolButton_loadSceneFile_clicked()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open File", "","Scene Files (*.scene)");

    ui->listWidget_Animations_AssignedAnimations->clear();
    ui->listWidget_Animations_AvaliableAnimations->clear();
    ui->listWidget_Models_Assigned->clear();
    ui->listWidget_Models_Avaliable->clear();
    ui->listWidget_Presets_PresetList->clear();
    ui->listWidget_Preset_AssignedAnimations->clear();
    ui->listWidget_Preset_AvaliableAnimations->clear();
    ui->ListWidget_Categories->clear();

    QFileInfo fileInfo(openFileName);
    loadSceneFile(fileInfo.path(), fileInfo.completeBaseName());

    ui->label_SelectedSceneFileName->setText(fileInfo.baseName());
    gData->sceneFile = fileInfo.completeBaseName();

}


void GCVIEditor::on_toolButton_Category_SelectImageFile_clicked()
{
    QString imgFileName = QFileDialog::getOpenFileName(this, "Open File", "","DDS Files (*.dds)");

    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a category from the list before changing its image.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            catDat.image = imgFileName;
            ui->label_CategoryImageFileName;

            return;
        }
    }
}


void GCVIEditor::on_pushButton_AssignModel_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }


    QListWidgetItem* item = ui->listWidget_Models_Avaliable->selectedItems().front();
    QString itemText = item->text();

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            GCVIData::GCVICategories::GCVIModelOptions moDat;
            moDat.name = itemText;

            catDat.models.push_back(moDat);

            break;
        }
    }

    ui->listWidget_Models_Assigned->addItem(itemText);
    ui->listWidget_Models_Avaliable->removeItemWidget(item);




}


void GCVIEditor::on_listView_Models_Assigned_pressed(const QModelIndex &index)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        return;
    }

    ui->listWidget_Models_Assigned->clear();
    ui->listWidget_Models_Avaliable->clear();
    ui->listWidget_Presets_PresetList->clear();

    ui->spinBox_Animations_LinkedID->setValue(0);
    ui->spinBox_Animations_Localization->setValue(0);
    ui->spinBox_Presets_LinkedID->setValue(0);
    ui->spinBox_Presets_Localization->setValue(0);

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (auto moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    ui->lineEdit_ModelName->setText(moDat.name);
                    ui->label_ModelImageFileName->setText(moDat.image);
                    ui->spinBox_Model_Localization->setValue(moDat.localize.toInt());
                    ui->spinBox_Model_Type->setValue(moDat.type.toInt());

                    if(moDat.defaultActive == "1")
                        ui->checkBox_ModelDefaultActive->setChecked(true);
                    else
                        ui->checkBox_ModelDefaultActive->setChecked(false);

                    ui->lineEdit_SubCategory_Name->setText(moDat.subName);
                    ui->label_SubCategory_ImageSelected->setText(moDat.subImage);
                    ui->spinBox_SubCategory_Localization->setValue(moDat.subLocalize.toInt());

                    foreach (auto animeDat, moDat.animations)
                    {
                        ui->listWidget_Models_Assigned->addItem(animeDat.name);
                    }

                    auto AllAnimations = ModelAnimationMap.equal_range(moDat.name);

                    for(auto it = AllAnimations.first; it != AllAnimations.second; ++it)
                    {
                        auto allDat = *it;

                        bool found = false;
                        foreach (auto animeDat, moDat.animations)
                        {
                            if(animeDat.name == moDat.name)
                            {
                                found = true;
                                break;
                            }
                        }

                        if(!found)
                            ui->listWidget_Models_Avaliable->addItem(allDat);
                    }

                    foreach (auto presetDat, moDat.presets)
                    {
                        ui->listWidget_Presets_PresetList->addItem(presetDat.name);
                    }

                    break;
                }
            }
        }
    }
}


void GCVIEditor::on_pushButton_RemoveModel_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to remove it to the category.");
        return;
    }

    QListWidgetItem* item = ui->listWidget_Models_Assigned->selectedItems().front();
    QString itemText = item->text();

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            int removeID = -1;
            int counter = 0;
            //catDat.models.push_back(moDat);

            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    removeID = counter;
                    break;
                }
                counter++;
            }

            if(removeID != -1)
            {
                catDat.models.remove(removeID);
                break;
            }
        }
    }


    ui->listWidget_Models_Avaliable->addItem(itemText);
    ui->listWidget_Models_Assigned->removeItemWidget(item);


}


void GCVIEditor::on_toolButton_Model_SelectImageFile_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to remove it to the category.");
        return;
    }

    QString openFileName = QFileDialog::getOpenFileName(this, "Open Model Image File", "","DDS Files (*.dds)");


    ui->label_ModelImageFileName->setText(openFileName);

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {


            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.image = openFileName;
                }

            }

        }
    }


}


void GCVIEditor::on_toolButton_SubCategory_SelectImageFile_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to remove it to the category.");
        return;
    }

    QString openFileName = QFileDialog::getOpenFileName(this, "Open Sub Image File", "","DDS Files (*.dds)");


    ui->label_SubCategory_ImageSelected->setText(openFileName);

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {


            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.subImage = openFileName;
                }

            }

        }
    }

}


void GCVIEditor::on_pushButton_Animation_AssignAnimation_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Animations_AvaliableAnimations->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select an animation.");
        return;
    }

    QListWidgetItem* item = ui->listWidget_Animations_AssignedAnimations->selectedItems().front();
    QString itemText = item->text();


    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Avaliable->selectedItems().front()->text())
                {
                    GCVIData::GCVICategories::GCVIModelOptions::GCVIAnimation animation;
                    animation.name = itemText;
                    moDat.animations.push_back(animation);

                    break;
                }
            }

            break;
        }
    }

    ui->listWidget_Animations_AssignedAnimations->addItem(itemText);
    ui->listWidget_Animations_AvaliableAnimations->removeItemWidget(item);



}


void GCVIEditor::on_listView_Animations_AssignedAnimations_clicked(const QModelIndex &index)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        return;
    }



    ui->spinBox_Animations_LinkedID->setValue(0);
    ui->spinBox_Animations_Localization->setValue(0);
    ui->spinBox_Presets_LinkedID->setValue(0);
    ui->spinBox_Presets_Localization->setValue(0);

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (auto moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            ui->doubleSpinBox_Animation_HighRange->setValue(aDat.highRange.toInt());
                            ui->doubleSpinBox_Animation_LowRange->setValue(aDat.lowRange.toInt());
                            ui->lineEdit_Animation_Name->setText(aDat.name);
                            ui->spinBox_Animations_LinkedID->setValue(aDat.LinkID.toInt());
                            ui->spinBox_Animations_Localization->setValue(aDat.localize.toInt());
                        }
                        return;
                    }

                }
            }
        }
    }
}


void GCVIEditor::on_pushButton_Animation_RemoveAnimation_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    QListWidgetItem* item = ui->listWidget_Animations_AssignedAnimations->selectedItems().front();
    QString itemText = item->text();



    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    int removeID = -1;
                    int counter = 0;

                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            removeID = counter;
                            break;
                        }
                        counter++;
                    }

                    if(removeID != -1)
                    {
                        moDat.animations.remove(removeID);
                        break;
                    }

                    break;
                }

            }
            break;

        }
    }


    ui->listWidget_Animations_AvaliableAnimations->addItem(itemText);
    ui->listWidget_Animations_AssignedAnimations->removeItemWidget(item);


}


void GCVIEditor::on_listView_Presets_PresetList_clicked(const QModelIndex &index)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            ui->listWidget_Preset_AssignedAnimations->clear();
                            ui->listWidget_Preset_AvaliableAnimations->clear();

                            ui->label_Preset_ImageFileName->setText(pDat.image);
                            ui->lineEdit_Preset_Name->setText(pDat.name);
                            ui->spinBox_Presets_Localization->setValue(pDat.localize.toInt());

                            foreach(auto paDat, pDat.presetAnimations)
                            {
                                ui->listWidget_Preset_AssignedAnimations->addItem(paDat.morphName);
                            }

                            auto AllAnimations = ModelAnimationMap.equal_range(moDat.name);

                            for(auto it = AllAnimations.first; it != AllAnimations.second; ++it)
                            {
                                auto allDat = *it;

                                bool found = false;

                                foreach(auto paDat, pDat.presetAnimations)
                                {
                                    if(paDat.morphName == allDat)
                                    {
                                        found = true;
                                        break;
                                    }
                                }

                                if(!found)
                                {
                                    ui->listWidget_Preset_AvaliableAnimations->addItem(allDat);
                                }

                            }
                        }
                    }
                }
            }
        }
    }

}


void GCVIEditor::on_pushButton_Preset_CreatePreset_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    GCVIData::GCVICategories::GCVIModelOptions::GCVIPreset preset;
                    preset.name = "New Preset";
                    moDat.presets.push_back(preset);

                    ui->listWidget_Presets_PresetList->addItem(preset.name);

                    break;
                }
            }

            break;
        }
    }

}


void GCVIEditor::on_pushButton_Preset_DeletePreset_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    QListWidgetItem* item = ui->listWidget_Presets_PresetList->selectedItems().front();

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    int removeID = -1;
                    int counter = 0;

                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            removeID = counter;
                            break;
                        }
                        counter++;
                    }

                    if(removeID != -1)
                    {
                        moDat.presets.remove(removeID);
                        break;
                    }
                }
            }
        }
    }


    ui->listWidget_Presets_PresetList->removeItemWidget(item);


}


void GCVIEditor::on_toolButton_Preset_SelectImageFile_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to remove it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    QString openFileName = QFileDialog::getOpenFileName(this, "Open Model Image File", "","DDS Files (*.dds)");


    ui->label_Preset_ImageFileName->setText(openFileName);

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {

            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            pDat.image = openFileName;
                            break;
                        }
                    }

                    break;
                }

            }
            break;
        }
    }

}


void GCVIEditor::on_pushButton_Preset_AssignAnimation_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    if(ui->listWidget_Preset_AvaliableAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }



    QListWidgetItem* item = ui->listWidget_Preset_AvaliableAnimations->selectedItems().front();
    QString itemText = item->text();

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (auto moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            GCVIData::GCVICategories::GCVIModelOptions::GCVIPreset::GCVIPresetAnimations panimeDat;
                            panimeDat.morphName = itemText;

                            pDat.presetAnimations.push_back(panimeDat);
                            break;
                        }
                    }
                    break;
                }

            }
            break;
        }
    }

    ui->listWidget_Preset_AssignedAnimations->addItem(itemText);
    ui->listWidget_Preset_AvaliableAnimations->removeItemWidget(item);


}


void GCVIEditor::on_listView_Preset_AssignedAnimations_clicked(const QModelIndex &index)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    if(ui->listWidget_Preset_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (auto moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            foreach (auto paDat, pDat.presetAnimations)
                            {
                                if(paDat.morphName == ui->listWidget_Preset_AssignedAnimations->selectedItems().front()->text())
                                {
                                    ui->doubleSpinBox_Preset_AnimationValue->setValue(paDat.animationValue.toFloat());
                                    ui->spinBox_Presets_LinkedID->setValue(paDat.LinkID.toInt());
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }

            }
            break;
        }
    }
}


void GCVIEditor::on_pushButton_Preset_RemoveAnimation_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    if(ui->listWidget_Preset_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }


    QListWidgetItem* item = ui->listWidget_Preset_AssignedAnimations->selectedItems().front();
    QString itemText = item->text();



    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            int removeID = -1;
                            int counter = 0;

                            foreach (auto paDat, pDat.presetAnimations)
                            {
                                if(paDat.morphName == ui->listWidget_Preset_AssignedAnimations->selectedItems().front()->text())
                                {
                                    removeID = counter;
                                    break;
                                }
                                counter++;
                            }

                            if(removeID != -1)
                            {
                                pDat.presetAnimations.remove(removeID);
                                break;
                            }
                        }
                    }

                    break;
                }

            }
            break;

        }
    }


    ui->listWidget_Preset_AvaliableAnimations->addItem(itemText);
    ui->listWidget_Preset_AssignedAnimations->removeItemWidget(item);


}


void GCVIEditor::on_pushButton_Category_CreateEdit_clicked()
{
    GCVIData::GCVICategories cat;
    cat.name = "New Cat";

    gData->guiData.push_back(cat);

    QListWidgetItem *newItem = new QListWidgetItem(cat.name);

    ui->ListWidget_Categories->addItem(newItem);

    newItem->setSelected(true);

}


void GCVIEditor::on_pushButton_Category_Delete_clicked()
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    int removeID = -1;
    int counter = 0;

    QListWidgetItem* item = ui->ListWidget_Categories->selectedItems().front();


    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            removeID = counter;
            break;
        }
        counter++;
    }

    if(removeID != -1)
    {
        gData->guiData.remove(removeID);
    }

    ui->ListWidget_Categories->removeItemWidget(item);

}


void GCVIEditor::on_lineEdit_ModelName_textEdited(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.name = ui->lineEdit_ModelName->text();
                    ui->listWidget_Models_Assigned->selectedItems().front()->setText(moDat.name);
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_spinBox_Model_Localization_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.localize = QString(ui->spinBox_Model_Localization->value());
                    break;
                }
            }
            break;
        }
    }
}




void GCVIEditor::on_spinBox_Model_Type_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.localize = QString(ui->spinBox_Model_Type->value());
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_checkBox_ModelDefaultActive_stateChanged(int arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    if(ui->checkBox_ModelDefaultActive->isChecked() == true)
                        moDat.defaultActive = "1";
                    else
                        moDat.defaultActive = "0";

                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_lineEdit_SubCategory_Name_textEdited(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.subName = ui->lineEdit_SubCategory_Name->text();
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_spinBox_SubCategory_Localization_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    moDat.subLocalize = QString::number(ui->spinBox_SubCategory_Localization->value());
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_lineEdit_Animation_Name_textEdited(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            aDat.name = ui->lineEdit_Animation_Name->text();
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_spinBox_Animations_Localization_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            aDat.localize = QString::number(ui->spinBox_Animations_Localization->value());
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_doubleSpinBox_Animation_LowRange_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            aDat.lowRange = QString::number(ui->doubleSpinBox_Animation_LowRange->value());
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_doubleSpinBox_Animation_HighRange_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            aDat.highRange = QString::number(ui->doubleSpinBox_Animation_HighRange->value());
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_spinBox_Animations_LinkedID_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Assigned->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a model selected.");
        return;
    }

    if(ui->listWidget_Animations_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto aDat, moDat.animations)
                    {
                        if(aDat.animationName == ui->listWidget_Animations_AssignedAnimations->selectedItems().front()->text())
                        {
                            aDat.LinkID = QString::number(ui->spinBox_Animations_LinkedID->value());
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_lineEdit_Preset_Name_textEdited(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            pDat.name = ui->lineEdit_Preset_Name->text();
                            ui->listWidget_Presets_PresetList->selectedItems().front()->setText(pDat.name);
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

}


void GCVIEditor::on_spinBox_Presets_Localization_textChanged(const QString &arg1)
{

    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            pDat.localize = QString(ui->spinBox_Presets_Localization->value());
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }


}


void GCVIEditor::on_spinBox_Presets_LinkedID_textChanged(const QString &arg1)
{

    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    if(ui->listWidget_Preset_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            foreach (auto paDat, pDat.presetAnimations)
                            {
                                if(paDat.morphName == ui->listWidget_Preset_AssignedAnimations->selectedItems().front()->text())
                                {
                                     paDat.LinkID = QString(ui->spinBox_Presets_LinkedID->value());
                                     break;
                                }

                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_doubleSpinBox_Preset_AnimationValue_textChanged(const QString &arg1)
{
    if(ui->ListWidget_Categories->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","You must have a category selected.");
        return;
    }

    if(ui->listWidget_Models_Avaliable->selectedItems().empty())
    {
        QMessageBox::critical(this,"Error","Select a model before trying to assign it to the category.");
        return;
    }

    if(ui->listWidget_Presets_PresetList->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have a preset selected.");
        return;
    }

    if(ui->listWidget_Preset_AssignedAnimations->selectedItems().isEmpty())
    {
        QMessageBox::critical(this,"Error","You must have an animation selected.");
        return;
    }

    foreach (GCVIData::GCVICategories catDat, gData->guiData)
    {
        if(catDat.name == ui->ListWidget_Categories->selectedItems().front()->text())
        {
            foreach (GCVIData::GCVICategories::GCVIModelOptions moDat, catDat.models)
            {
                if(moDat.name == ui->listWidget_Models_Assigned->selectedItems().front()->text())
                {
                    foreach (auto pDat, moDat.presets)
                    {
                        if(pDat.name == ui->listWidget_Presets_PresetList->selectedItems().front()->text())
                        {
                            foreach (auto paDat, pDat.presetAnimations)
                            {
                                if(paDat.morphName == ui->listWidget_Preset_AssignedAnimations->selectedItems().front()->text())
                                {
                                     paDat.animationValue = QString::number(ui->doubleSpinBox_Preset_AnimationValue->value());
                                     break;
                                }

                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}


void GCVIEditor::on_lineEdit_ModelName_editingFinished()
{

}

bool GCVIEditor::ConvertMeshFiles(QString filePath, QString meshFilename)
{


        // Create a QProcess instance
        QProcess process;

        // Define the command to run and its arguments
        QString program = "./OgreTools/OgreXMLConverter"; // Replace with the actual command
        QStringList arguments;
        arguments << filePath+meshFilename << meshFilename+".xml"; // Replace with your command-line arguments

        // Set the program and arguments
        process.setProgram(program);
        process.setArguments(arguments);

        // Start the process
        process.start();

        // Optionally, wait for the process to finish
        if (!process.waitForFinished())
        {
            return false;
        }

        ReadMeshFiles(meshFilename+".xml");

        return true;
}

bool GCVIEditor::ReadMeshFiles(QString meshFilename)
{
    QFile file("OgreTools/"+meshFilename+".xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::critical(this,"File Error",
                             QString("Whopse, opening %1 failed.").arg("OgreTools/"+meshFilename+".xml"));
       return false;
    }

    //Process XML File.
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement Element;
    //QDomElement entityElement;


    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
       QMessageBox::critical(this,"Error .mesh.xml file",
                            QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                            errorMsg,errorLine,errorColumn));
        return false;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("mesh");

    if(rootNode.isNull())
    {
        QMessageBox::critical(this,"Error","Mesh xml is malformed.");
        return false;
    }

    QString modelName = meshFilename;

    if (modelName.endsWith(".xml"))
        modelName.chop(4);

    //Get first city, and lets go
    //rootNode = doc.documentElement();
    Element = rootNode.firstChildElement("poses");

    if(!Element.isNull())
    {
        Element = Element.firstChildElement("pose");

        do
        {
            if(!ModelAnimationMap.contains(modelName,Element.attribute("name")))
                ModelAnimationMap.insert(modelName,Element.attribute("name"));

            Element = Element.nextSiblingElement("pose");
        }
        while(!Element.isNull());

    }
}


bool GCVIEditor::loadSceneFile(QString filePath, QString filename)
{


    QFile file(filePath+filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::critical(this,"File Error",
                             QString("Whopse, opening %1 failed.").arg(filePath+filename));
       return false;
    }

    //Process XML File.
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement Element;
    QDomElement entityElement;


    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
       QMessageBox::critical(this,"Error scene file",
                            QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                            errorMsg,errorLine,errorColumn));
        return false;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("scene");
    if(rootNode.isNull())
    {
        QMessageBox::critical(this,"Error","Scene Script is malformed.");
        return false;
    }

    //Get first city, and lets go
    //rootNode = doc.documentElement();
    Element = rootNode.firstChildElement("nodes");

    if(!Element.isNull())
    {
        Element = Element.firstChildElement("node");

        do
        {
            entityElement = Element.firstChildElement("entity");
            if(!entityElement.isNull())
            {
                ConvertMeshFiles(filePath,entityElement.attribute("meshFile"));
            }

            ui->listWidget_Models_Avaliable->addItem(Element.attribute("name"));

            Element = Element.nextSiblingElement("node");
        }
        while(!Element.isNull());

    }




    return true;
}
