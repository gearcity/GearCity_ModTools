#include "SteamWorksUploader.h"
#include "ui_SteamWorksUploader.h"
#include "QMessageBox"
#include "QDesktopServices"
#include "QUrl"
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <QFileDialog>



SteamWorksUploader::SteamWorksUploader(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SteamWorksUploader)
{
    ui->setupUi(this);

    //copy all widget containers
    cp_wsc = wsc;


    //Hide the update panel
    ui->frame_UpdateWorkshopItems->hide();
    ui->progressBar_WorkshopUpload->hide();
    ui->label_Steamworkshop_UploadStatus->hide();


#if STEAMSUPPORT
    QRegExp rex("[^,]+");

    /*
     *Unneeded Because We use the commas for spitting since tags
     *requires an array.
     *
     *QValidator *val = new QRegExpValidator(rex,this);
     *ui->line_WorkshopItemTags->setValidator(val);
     */

    connect(&timer,SIGNAL(timeout()),this,SLOT(updateSteam()));


#endif

}

SteamWorksUploader::~SteamWorksUploader()
{
    delete ui;
}

void SteamWorksUploader::on_button_SteamWorkshop_ReturnToMain_clicked()
{
    //return to the main menu
     cp_wsc.SteamWorkshopCW->lower();
}


//Making a new workshop item
void SteamWorksUploader::on_button_CreateANewWorkShopItem_clicked()
{


    //Make sure we're logged into steam
   if(!SteamAPI_Init())
   {
       QMessageBox::critical(this,"You Did Not Initalize!","SteamAPI_Init failed!"
                             "\nAre you logged into Steam?");

       return;
   }

   //Tell Steam we want to make a new item
    SteamAPICall_t steamAPICall = SteamUGC()->CreateItem( SteamUtils()->GetAppID(), k_EWorkshopFileTypeCommunity );
    cb_swCreateItem.Set(steamAPICall, this, &SteamWorksUploader::createItemResults);

    if(!timer.isActive())
        timer.start(1000);




}


//We told steam we wanted to make a new item, they gave us information back
void SteamWorksUploader::createItemResults(CreateItemResult_t* callBack, bool failure)
{

    //Check for failures
    if (failure)
    {
        QMessageBox::warning(this, "Error", "Something broke.");
        return;
    }
    else if(callBack->m_eResult == k_EResultInsufficientPrivilege)
    {
        QMessageBox::warning(this, "Error", "Insufficient privileges. (Have you been banned?)");
        return;
    }
    else if(callBack->m_eResult == k_EResultTimeout)
    {
        QMessageBox::warning(this, "Error", "Connection timed out.");
        return;
    }
    else if(callBack->m_eResult == k_EResultNotLoggedOn)
    {
        QMessageBox::warning(this, "Error", "You're not logged into Steam...");
        return;
    }


    //Store the new item's ID number. This needs to be stored with the mods as well.
    itemID = callBack->m_nPublishedFileId;

    //If the user hasn't accepted Steam's workshop terms, we need to force them to.
    if(callBack->m_bUserNeedsToAcceptWorkshopLegalAgreement)
    {
        if(!AcceptsSteamsTerms())
        {
            QMessageBox::warning(this, "Error", "Can't Upload if you don't agree.");
            return;
        }
    }

    /*Since we have an ID now, we can load up the update tool so the modder can fill
    information out */
    ui->frame_UpdateWorkshopItems->show();
    clearUpdateInfos();


}

void SteamWorksUploader::on_button_UpdateWorkShopItem_clicked()
{
    clearUpdateInfos();

    if(!SteamAPI_Init())
    {
        QMessageBox::critical(this,"You Did Not Initalize!","SteamAPI_Init failed!"
                              "\nAre you logged into Steam?");

        return;
    }


    if(!SteamUGC())
    {
        QMessageBox::critical(this,"You Did Not Initalize!","SteamAPI_Init failed!"
                              "\nAre you logged into Steam?");

        return;
    }

    if(!loadItemManifest())
    {
        ui->frame_UpdateWorkshopItems->hide();
        return;
    }

    ui->frame_UpdateWorkshopItems->show();

}

/* If the user hasn't accepted Steam's Terms, we force them to with this popup box*/
bool SteamWorksUploader::AcceptsSteamsTerms()
{
    QMessageBox workshopTermsBox;
    workshopTermsBox.setText("By submitting this item, you agree to the workshop terms of service");
    QAbstractButton* wtbYes = workshopTermsBox.addButton("Agree",QMessageBox::YesRole);
    QAbstractButton* wtbNo = workshopTermsBox.addButton("Disagree",QMessageBox::NoRole);
    QAbstractButton* wtbRead = workshopTermsBox.addButton("Read Terms", QMessageBox::HelpRole);

    workshopTermsBox.exec();

    if(workshopTermsBox.clickedButton() == wtbNo)
        return false;
    else if (workshopTermsBox.clickedButton() == wtbYes)
        return true;
    else if(workshopTermsBox.clickedButton() == wtbRead)
    {
        /*They wanted to read the terms (yeah right), we open the default web browser then
        recurse the function until they hit yes or no...*/

        QDesktopServices::openUrl(QUrl("http://steamcommunity.com/sharedfiles/workshoplegalagreement"));
        return AcceptsSteamsTerms();
    }

    return true;

}

void SteamWorksUploader::clearUpdateInfos()
{
    ui->label_WorkshopItem_SelectedFolder->setText("No Folder Selected.");
    ui->label_WorkshopItem_SelectedImage->setText("No Images Selected.");

    ui->textbox_WorkshopItemDescription->clear();
    ui->textbox_WorkshopItemDescription->clear();
    ui->line_WorkshopItemTags->clear();
    ui->line_WorkshopItemTitle->clear();
    ui->progressBar_WorkshopUpload->hide();
    ui->label_Steamworkshop_UploadStatus->hide();

    ui->combo_WorkshopItem_Language->setCurrentIndex(7);
    ui->radio_WorkshopItem_Type_Mod->setChecked(true);
    ui->radio_WorkshopItem_Visibility_You->setChecked(true);

    workshopItemFolderPath.clear();
    workshopItemImage.clear();
}


void SteamWorksUploader::updateSteam()
{
     SteamAPI_RunCallbacks();
}

void SteamWorksUploader::on_button_WorkshopItem_ContentFolder_clicked()
{
    //Get Item
    workshopItemFolderPath = QFileDialog::getExistingDirectory(this, "Select Workshop Item Folder", "",
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    ui->label_WorkshopItem_SelectedFolder->setText(workshopItemFolderPath);


}

void SteamWorksUploader::on_button_WorkshopItem_PreviewImage_clicked()
{
    workshopItemImage =  QFileDialog::getOpenFileName(this, "Select Workshop Item Preview Image", "",
                                                         "Pictures (*.jpg *.jpeg *.png *.gif);;"
                                                      "JPEG (*.jpg *.jpeg);;PNG (*.png);;GIF (*.gif)");

    ui->label_WorkshopItem_SelectedImage->setText(workshopItemImage);
}

void SteamWorksUploader::on_textbox_WorkshopItemDescription_textChanged()
{
    if(ui->textbox_WorkshopItemDescription->toPlainText().length() > 8000)
        ui->textbox_WorkshopItemDescription->textCursor().deletePreviousChar();
}

void SteamWorksUploader::on_button_SubmitWorkshopItem_clicked()
{


    if(ui->line_WorkshopItemTitle->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Item Title is Empty");
        return;
    }
    else if(ui->textbox_WorkshopItemDescription->toPlainText().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Item Description is Empty");
        return;
    }
    else if(workshopItemFolderPath.isEmpty())
    {
        QMessageBox::critical(this, "Error", "No Items Folder Selected");
        return;
    }
    else if(workshopItemImage.isEmpty())
    {
        QMessageBox::critical(this, "Error", "No Item Preview Image Selected");
        return;
    }

    QMessageBox::information(this, "Workshop Manifest!", "You must save the Workshop Item Manifest."
                             "\n\nDo not save this in the same folder as your map or mods."
                             "\n\nKeep this file in a safe location as it contains your publisher ID number,"
                             " and it is what you will use to update your workshop item.");


    if(!saveItemManifest())
    {
        QMessageBox::warning(this, "Error", "You must save the Manifest to upload. "
                             "Otherwise you will lose the publisher ID.");
        return;
    }

    UpdateHandle = SteamUGC()->StartItemUpdate(285110,itemID);

    SteamUGC()->SetItemTitle(UpdateHandle, ui->line_WorkshopItemTitle->text().toUtf8());
    SteamUGC()->SetItemDescription(UpdateHandle, ui->textbox_WorkshopItemDescription->toPlainText().toUtf8());

    if(ui->radio_WorkshopItem_Type_Both->isChecked())
        SteamUGC()->SetItemMetadata(UpdateHandle,"both");
    else if(ui->radio_WorkshopItem_Type_Map->isChecked())
        SteamUGC()->SetItemMetadata(UpdateHandle,"map");
    else if(ui->radio_WorkshopItem_Type_Mod->isChecked())
        SteamUGC()->SetItemMetadata(UpdateHandle,"mod");



    SteamUGC()->SetItemContent(UpdateHandle,workshopItemFolderPath.toUtf8());
    SteamUGC()->SetItemPreview(UpdateHandle,workshopItemImage.toUtf8());

    QVector<const char *> tagVector;
    QStringList tagList = ui->line_WorkshopItemTags->text().split(",");

    if(!tagVector.isEmpty())
    {

        foreach(QString tag, tagList)
        {
            tagVector.push_back(tag.toUtf8().data());
        }

        SteamParamStringArray_t steamTags;
        steamTags.m_ppStrings = tagVector.data();
        steamTags.m_nNumStrings = tagVector.size()+1;

        SteamUGC()->SetItemTags(UpdateHandle,&steamTags);
    }

    SteamUGC()->SetItemUpdateLanguage(UpdateHandle,ui->combo_WorkshopItem_Language->currentText().toUtf8());

    if(ui->radio_WorkshopItem_Visibility_Everyone->isChecked())
        SteamUGC()->SetItemVisibility(UpdateHandle,k_ERemoteStoragePublishedFileVisibilityPublic);
    else if(ui->radio_WorkshopItem_Visibility_Friends->isChecked())
        SteamUGC()->SetItemVisibility(UpdateHandle,k_ERemoteStoragePublishedFileVisibilityFriendsOnly);
    else if(ui->radio_WorkshopItem_Visibility_You->isChecked())
        SteamUGC()->SetItemVisibility(UpdateHandle,k_ERemoteStoragePublishedFileVisibilityPrivate);



    SteamAPICall_t SteamAPICall = SteamUGC()->SubmitItemUpdate(UpdateHandle, ui->textbox_SteamWorkshop_ChangeLog->toPlainText().toUtf8());
    cb_swUpdateItem.Set(SteamAPICall, this, &SteamWorksUploader::itemUpdateResults);

    ui->progressBar_WorkshopUpload->show();
    ui->label_Steamworkshop_UploadStatus->show();

    connect(&progressTimer,SIGNAL(timeout()),this,SLOT(updateProgressBar()));

    if(!timer.isActive())
        timer.start(1000);
    if(!progressTimer.isActive())
        progressTimer.start(1000);


}

void SteamWorksUploader::itemUpdateResults(SubmitItemUpdateResult_t* callBack, bool failure)
{

    progressTimer.stop();
    ui->progressBar_WorkshopUpload->hide();
    ui->label_Steamworkshop_UploadStatus->hide();

    if(failure || callBack->m_eResult == k_EResultFail)
    {
        QMessageBox::critical(this, "Error", "Upload Failed");        
        return;
    }
    else if(callBack->m_eResult == k_EResultInvalidParam)
    {
        QMessageBox::warning(this, "Error", "Developer Broke Something (Invalid Param)...");
        return;
    }
    else if(callBack->m_eResult == k_EResultAccessDenied )
    {
        QMessageBox::warning(this, "Error", "k_EResultAccessDenied\n"
                             " The user doesn't own a license for the provided app ID.");
        return;
    }
    else if(callBack->m_eResult == k_EResultFileNotFound  )
    {
        QMessageBox::warning(this, "Error", "k_EResultFileNotFound \n"
                             " Failed to get the workshop info for the item or failed to read the preview file.");
        return;
    }
    else if(callBack->m_eResult == k_EResultLockingFailed  )
    {
        QMessageBox::warning(this, "Error", "k_EResultLockingFailed \n"
                             "  Failed to aquire UGC Lock.");
        return;
    }
    else if(callBack->m_eResult == k_EResultFileNotFound  )
    {
        QMessageBox::warning(this, "Error", "k_EResultFileNotFound \n"
                             " The provided content folder is not valid.");
        return;
    }
    else if(callBack->m_eResult == k_EResultFileNotFound  )
    {
        QMessageBox::warning(this, "Error", "k_EResultLimitExceeded \n"
                             " The preview image is too large, it must be less than 1 Megabyte;"
                             " or there is not enough space available on the users Steam Cloud.");
        return;
    }


    if(callBack->m_eResult == k_EResultOK )
    {
        QMessageBox::information(this, "Success!", "You have successfully uploaded the files.");
    }

}


bool SteamWorksUploader::saveItemManifest()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save  Workshop Item Manifest",
                                                "","XML Files (*.xml)");

    if (saveFileName != "")
    {
        if (!saveFileName.endsWith(".xml"))
            saveFileName += ".xml";

            //Create/overwrite workshop manifest
            QFile saveFile(saveFileName);

            if(!saveFile.open(QFile::WriteOnly | QFile::Text))
            {
               QMessageBox::critical(this,"Error",
                                     QString("Can not write to %1\n%2").arg(saveFileName,
                                     saveFile.errorString()));
               return false;
            }

            //Lets Start Writing!
            QXmlStreamWriter xmlWriter(&saveFile);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement("WorkshopManifest");

                xmlWriter.writeStartElement("title");
                    xmlWriter.writeCharacters(ui->line_WorkshopItemTitle->text());
                xmlWriter.writeEndElement(); //title


                xmlWriter.writeStartElement("description");
                    xmlWriter.writeCharacters(ui->textbox_WorkshopItemDescription->toPlainText());
                xmlWriter.writeEndElement(); //description


                xmlWriter.writeStartElement("type");
                if(ui->radio_WorkshopItem_Type_Both->isChecked())
                    xmlWriter.writeCharacters("0");
                else if(ui->radio_WorkshopItem_Type_Map->isChecked())
                    xmlWriter.writeCharacters("1");
                else if(ui->radio_WorkshopItem_Type_Mod->isChecked())
                    xmlWriter.writeCharacters("2");
                xmlWriter.writeEndElement(); //type


                xmlWriter.writeStartElement("itemPath");
                    xmlWriter.writeCharacters(workshopItemFolderPath);
                xmlWriter.writeEndElement(); //itemPath

                xmlWriter.writeStartElement("itemImage");
                    xmlWriter.writeCharacters(workshopItemImage);
                xmlWriter.writeEndElement(); //itemImage

                xmlWriter.writeStartElement("tags");
                    xmlWriter.writeCharacters(ui->line_WorkshopItemTags->text());
                xmlWriter.writeEndElement(); //tags

                xmlWriter.writeStartElement("language");
                    xmlWriter.writeCharacters(QString::number(ui->combo_WorkshopItem_Language->currentIndex()));
                xmlWriter.writeEndElement(); //language


                xmlWriter.writeStartElement("visibility");
                if(ui->radio_WorkshopItem_Visibility_Everyone->isChecked())
                    xmlWriter.writeCharacters("0");
                else if(ui->radio_WorkshopItem_Visibility_Friends->isChecked())
                    xmlWriter.writeCharacters("1");
                else if(ui->radio_WorkshopItem_Visibility_You->isChecked())
                    xmlWriter.writeCharacters("2");
                xmlWriter.writeEndElement(); //visibility


                xmlWriter.writeStartElement("PublishID");
                    xmlWriter.writeCharacters(QString::number(itemID));
                xmlWriter.writeEndElement(); //PublishID



              xmlWriter.writeEndElement(); //WorkshopManifest



            //close the final tag and close the file.
             xmlWriter.writeEndDocument();

             saveFile.close();

             //We Did it! Yay!
             QMessageBox::information(this,"Save Complete",
                                      "The Manifest has been saved! Starting upload process!", QMessageBox::Ok);
    }
    else
    {
        return false;
    }

    return true;
}

bool SteamWorksUploader::loadItemManifest()
{
    QString openFileName = QFileDialog::getOpenFileName(this, "Open Workshop Item Manifest",
                                                        "","XML Files (*.xml)");

    if (openFileName != "")
    {

        //Open File
        QFile file(openFileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
           QMessageBox::critical(this,"File Error",
                                 QString("Whopse, opening %1 failed.").arg(openFileName));
           return false;
        }

        //Process XML File.
        QString errorMsg;
        int errorLine, errorColumn;
        QDomDocument doc;
        QDomElement Element;


        if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
        {
           QMessageBox::critical(this,"Error Base Video",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return false;
        }

        //Begin reading the manifest and putting the data in the ui and what have you.
        QDomElement rootNode = doc.firstChildElement("WorkshopManifest");
        if(rootNode.isNull())
        {
            QMessageBox::critical(this,"Error","Workshop Manifest is malformed.");
            return false;
        }

        //Get Workshop Manifest
        rootNode = doc.documentElement();

        Element = rootNode.firstChildElement("title");
        if(!Element.isNull())
            ui->line_WorkshopItemTitle->setText(Element.text());

        Element = rootNode.firstChildElement("description");
        if(!Element.isNull())
            ui->textbox_WorkshopItemDescription->setPlainText(Element.text());

        Element = rootNode.firstChildElement("type");
        if(!Element.isNull())
        {
            if(Element.text() == "0")
                ui->radio_WorkshopItem_Type_Both->setChecked(true);
            else if(Element.text() == "1")
                ui->radio_WorkshopItem_Type_Map->setChecked(true);
            else if(Element.text() == "2")
                ui->radio_WorkshopItem_Type_Mod->setChecked(true);
        }

        Element = rootNode.firstChildElement("itemPath");
        if(!Element.isNull())
        {
            workshopItemFolderPath = Element.text();
            ui->label_WorkshopItem_SelectedFolder->setText(Element.text());
        }

        Element = rootNode.firstChildElement("itemImage");
        if(!Element.isNull())
        {
            workshopItemImage = Element.text();
            ui->label_WorkshopItem_SelectedImage->setText(Element.text());
        }

        Element = rootNode.firstChildElement("tags");
        if(!Element.isNull())
            ui->line_WorkshopItemTags->setText(Element.text());

        Element = rootNode.firstChildElement("language");
        if(!Element.isNull())
            ui->combo_WorkshopItem_Language->setCurrentIndex(Element.text().toInt());


        Element = rootNode.firstChildElement("visibility");
        if(!Element.isNull())
        {
            if(Element.text() == "0")
                ui->radio_WorkshopItem_Visibility_Everyone->setChecked(true);
            else if(Element.text() == "1")
                ui->radio_WorkshopItem_Visibility_Friends->setChecked(true);
            else if(Element.text() == "2")
                ui->radio_WorkshopItem_Visibility_You->setChecked(true);
        }

        Element = rootNode.firstChildElement("PublishID");
        if(Element.isNull())
        {
            QMessageBox::critical(this,"Error","Missing Publish ID!");
            return false;
        }
        else
            itemID = Element.text().toULongLong();


        file.close();
    }
    else
    {
        return false;
    }




    return true;
}


void SteamWorksUploader::updateProgressBar()
{
    unsigned long long uploadedBytes = 0, totalBytes = 0;
    EItemUpdateStatus status = SteamUGC()->GetItemUpdateProgress(UpdateHandle,&uploadedBytes,&totalBytes);

    ui->progressBar_WorkshopUpload->setMaximum(totalBytes/1024.0);
    ui->progressBar_WorkshopUpload->setValue(uploadedBytes/1024.0);

    if(status == k_EItemUpdateStatusInvalid)
        ui->label_Steamworkshop_UploadStatus->setText("Something Probably Broke...");
    else if(status == k_EItemUpdateStatusPreparingConfig)
        ui->label_Steamworkshop_UploadStatus->setText("The item update is processing configuration data.");
    else if(status == k_EItemUpdateStatusPreparingContent)
        ui->label_Steamworkshop_UploadStatus->setText("The item update is reading and processing content files.");
    else if(status == k_EItemUpdateStatusUploadingContent)
        ui->label_Steamworkshop_UploadStatus->setText("The item update is uploading content changes to Steam.");
    else if(status == k_EItemUpdateStatusUploadingPreviewFile)
        ui->label_Steamworkshop_UploadStatus->setText("The item update is uploading new preview file image.");
    else if(status == k_EItemUpdateStatusCommittingChanges)
        ui->label_Steamworkshop_UploadStatus->setText("The item update is committing all changes.");


}
