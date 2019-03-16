#ifndef STEAMWORKSUPLOADER_H
#define STEAMWORKSUPLOADER_H

#include <QWidget>
#include "BaseContainerStorage.h"
#include "steam/steam_api.h"
#include "steam/isteamugc.h"
#include <QTimer>

namespace Ui {
class SteamWorksUploader;
}

class SteamWorksUploader : public QWidget
{
    Q_OBJECT
    
public:
    explicit SteamWorksUploader(widgetContainerStorage wsc, QWidget *parent = 0);
    ~SteamWorksUploader();
    
private slots:
    void on_button_SteamWorkshop_ReturnToMain_clicked();
    void on_button_CreateANewWorkShopItem_clicked();
    void on_button_UpdateWorkShopItem_clicked();
    void updateSteam();

    void on_button_WorkshopItem_ContentFolder_clicked();

    void on_button_WorkshopItem_PreviewImage_clicked();

    void on_textbox_WorkshopItemDescription_textChanged();

    void on_button_SubmitWorkshopItem_clicked();

    void updateProgressBar();

private:
    Ui::SteamWorksUploader *ui;
    widgetContainerStorage cp_wsc;
    unsigned long long itemID;
    QTimer timer;
    QTimer progressTimer;
    QString workshopItemFolderPath;
    QString workshopItemImage;
    UGCUpdateHandle_t UpdateHandle;

    CCallResult<SteamWorksUploader, CreateItemResult_t> cb_swCreateItem;
    CCallResult<SteamWorksUploader, SubmitItemUpdateResult_t> cb_swUpdateItem;


    void createItemResults(CreateItemResult_t* callBack, bool failure);
    void itemUpdateResults(SubmitItemUpdateResult_t* callBack, bool failure);
    bool AcceptsSteamsTerms();
    void clearUpdateInfos();
    bool saveItemManifest();
    bool loadItemManifest();
};

#endif // STEAMWORKSUPLOADER_H
