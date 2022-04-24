/*Copyright 2016 Visual Entertainment And Technologies, LLC. All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

- Neither the name of Visual Entertainment And Technologies, LLC
nor the names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.")*/


/* *
 * CODING GUIDELINES...
 * Allman Indent Style!
 * 100 +/- Column Width
 * Avoid Namespaces
 * Lower Camel Case for variables and functions.
 * Upper Camel Case for everything else. (UI Widgets, Class Names)
 * Don't shoot me if I forget these things as well...
 * */


#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtWidgets/QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create container widgets to hold our layout forms.
    wsc.MapToolCW = ui->MapDataToolWidgetContainer;
    wsc.MainMenuCW = ui->MainMenuGroupBoxContainer;
    wsc.CityEditorCW = ui->CityEditorWidgetContainer;
    wsc.AIEditorCW = ui->AIEditorWidgetContainer;
    wsc.TurnEventEditorCW = ui->TurnEventEditorWidgetContainer;
    wsc.ModToolCW = ui->ModDataToolWidgetContainer;
    wsc.NameListCW = ui->NameListToolWidgetContainer;
    wsc.NewsPaperCW = ui->NewsArticlesWidgetContainer;
    wsc.ReviewsCW = ui->ReviewsWidgetContainer;
    wsc.ComponentsCW = ui->ComponentsEditorWidgetContainer;
    wsc.RacingCW = ui->RacingSeriesEditorWidgetContainer;
    wsc.ContractsCW = ui->ContractsInfoWidgetContainer;
    wsc.LogoEditorCW = ui->PlayerLogoEditorWidgetContainer;
    wsc.MusicEditorCW = ui->MusicEditorWidgetContainer;
    wsc.DYKEditorCW = ui->DidYouKnowWidgetContainer;
    wsc.PreMadeEditorCW = ui->PreMadeVehicleWidgetContainer;
    wsc.ScenarioEditorCW = ui->ScenarioWidgetContainer;
    wsc.SteamWorkshopCW = ui->SteamworksWidgetContainer;

    //Create child windows/edits before creating editors
    wsc.MaterialEditorWindow = new MaterialMakerWindow(this);

    //Create the forms and send the objects the container widgets to attach to.
    mapTool = new MapDataTool(wsc, ui->MapDataToolWidgetContainer);
    cityEditorTool = new CityEditor(wsc, ui->CityEditorWidgetContainer);
    aiEditorTool = new AIEditor(wsc, ui->AIEditorWidgetContainer);
    turnEventEditorTool = new TurnEventsEditor(wsc, ui->TurnEventEditorWidgetContainer);
    modEditorTool = new ModEditor(wsc,ui->ModDataToolWidgetContainer);

    contractEditorTool = new ContractInformationEditor(wsc, ui->ContractsInfoWidgetContainer);
    componentsEditorTool = new ComponentsEditor(wsc, ui->ComponentsEditorWidgetContainer);
    dykTool = new DYKEditor(wsc, ui->DidYouKnowWidgetContainer);
    musicTool = new MusicEditor(wsc, ui->MusicEditorWidgetContainer);
    nameListTool = new NameListEditor(wsc, ui->NameListToolWidgetContainer);
    newsArticleTool = new NewspaperArticleEditor(wsc, ui->NewsArticlesWidgetContainer);
    playerLogoTool = new PlayerLogoEditor(wsc, ui->PlayerLogoEditorWidgetContainer);
    premadeCarsTool = new PreMadeVehicleEditor(wsc, ui->PreMadeVehicleWidgetContainer);
    racingTool = new RacingSeriesEditor(wsc, ui->RacingSeriesEditorWidgetContainer);
    reviewTool = new ReviewsEditor(wsc, ui->ReviewsWidgetContainer);
    scenarioTool = new ScenarioEditor(wsc, ui->ScenarioWidgetContainer);
    steamworksTool = new SteamWorksUploader(wsc,ui->SteamworksWidgetContainer);




    //Raise up the main menu container widget.
    wsc.MainMenuCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

MainWindow::~MainWindow()
{
    //Delete all those pointers.
    delete ui;
    delete mapTool;
    delete cityEditorTool;
    delete aiEditorTool;
    delete turnEventEditorTool;
    delete modEditorTool;
    delete wsc.MaterialEditorWindow;

    delete componentsEditorTool;
    delete dykTool;
    delete musicTool;
    delete nameListTool;
    delete newsArticleTool;
    delete playerLogoTool;
    delete premadeCarsTool;
    delete racingTool;
    delete reviewTool;
    delete scenarioTool;

}

//MapTools Button
void MainWindow::on_Button_MapDataTools_clicked()
{
   //Raise the MapTool
    wsc.MapToolCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

//CityEditor Button
void MainWindow::on_Button_CityEditor_clicked()
{
    //Raise the CityEditor
    wsc.CityEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

//AIEditor Button
void MainWindow::on_Button_AIEditor_clicked()
{
    //Raise the AIEditor
    wsc.AIEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

//TurnEvents Editor
void MainWindow::on_Button_TurnEventsEditor_clicked()
{
    //Raise the TurnEvents Editor
    wsc.TurnEventEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

//Do to some odd issue with how I attach forms to Widget Containers, auto resize is broke
//So this is a hackish work around to make window resizing work properly.
void MainWindow::resizeEvent(QResizeEvent* event)
{
    //Resize container widget's based on Main Window size.
    wsc.MapToolCW->resize(this->width(), this->height());
    wsc.MainMenuCW->resize(this->width(), this->height());
    wsc.CityEditorCW->resize(this->width(), this->height());
    wsc.AIEditorCW->resize(this->width(), this->height());
    wsc.TurnEventEditorCW->resize(this->width(), this->height());
    wsc.ModToolCW->resize(this->width(), this->height());
    wsc.NameListCW->resize(this->width(), this->height());
    wsc.NewsPaperCW->resize(this->width(), this->height());
    wsc.ReviewsCW->resize(this->width(), this->height());
    wsc.ComponentsCW->resize(this->width(), this->height());
    wsc.RacingCW->resize(this->width(), this->height());
    wsc.ContractsCW->resize(this->width(), this->height());
    wsc.LogoEditorCW->resize(this->width(), this->height());
    wsc.MusicEditorCW->resize(this->width(), this->height());
    wsc.DYKEditorCW->resize(this->width(), this->height());
    wsc.PreMadeEditorCW->resize(this->width(), this->height());
    wsc.ScenarioEditorCW->resize(this->width(), this->height());
    wsc.SteamWorkshopCW->resize(this->width(), this->height());

    //Resize the object forms based on the Main Window size.
    mapTool->resize(this->width(), this->height());
    cityEditorTool->resize(this->width(), this->height());
    aiEditorTool->resize(this->width(), this->height());
    turnEventEditorTool->resize(this->width(), this->height());
    modEditorTool->resize(this->width(), this->height());
    componentsEditorTool->resize(this->width(), this->height());
    dykTool->resize(this->width(), this->height());
    musicTool->resize(this->width(), this->height());
    nameListTool->resize(this->width(), this->height());
    newsArticleTool->resize(this->width(), this->height());
    playerLogoTool->resize(this->width(), this->height());
    premadeCarsTool->resize(this->width(), this->height());
    racingTool->resize(this->width(), this->height());
    reviewTool->resize(this->width(), this->height());
    scenarioTool->resize(this->width(), this->height());
    steamworksTool->resize(this->width(), this->height());


}

void MainWindow::on_Button_ModDataTools_clicked()
{
    //Raise the Mod Editor
    wsc.ModToolCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_ScenarioDataTools_clicked()
{
    QMessageBox::critical(this,"Sorry!","Sorry! The Scenario system is not implemented yet.");
     //wsc.ScenarioEditorCW->raise();
    return;
}

void MainWindow::on_Button_LocalizationEditor_clicked()
{
    if(!QDesktopServices::openUrl(QUrl("http://185.52.1.98")))
    {
        QMessageBox::critical(this,"Sorry!","Sorry! We were unable to open your web browser."
                              "Please visit http://185.52.1.98/ to use our translation system.");
    }
    return;
}

void MainWindow::on_Button_ComponentEditor_clicked()
{
    wsc.ComponentsCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_NewsPaperEditor_clicked()
{
    wsc.NewsPaperCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_MagazineEditor_clicked()
{
    wsc.ReviewsCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_DYKEditor_clicked()
{
    wsc.DYKEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_LogoEditor_clicked()
{
    wsc.LogoEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_MusicEditor_clicked()
{
    wsc.MusicEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_PreMadeVehicleLoader_clicked()
{
    wsc.PreMadeEditorCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_Button_Racing_clicked()
{
    wsc.RacingCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}

void MainWindow::on_button_ContractEditor_clicked()
{
    /*QMessageBox::critical(this,"Sorry!","Sorry! This will be implemented after the contract"
                          "redesign coming in v1.23.");*/
    wsc.ContractsCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

void MainWindow::on_button_NameListEditor_clicked()
{
    wsc.NameListCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
}

void MainWindow::on_button_ddsConverter_clicked()
{
#if defined(Q_OS_UNIX)
    QMessageBox::critical(this,"Sorry!","A DDS converter is not included with Linux/BSD/macOS."
                          "\n\nPlease look into the DDS conversion plugin for GIMP "
                          "or use ImageMagick's commandline converter."
                          "\n\nYou may also be able to find webbased converters online as well.");
#elif defined(Q_OS_WIN)
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + "/Aorta.exe";
    process->startDetached(file);
#else
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() +"/Aorta.app";
    process->startDetached(file);
#endif
}

void MainWindow::on_pushButton_clicked()
{
#if !STEAMSUPPORT
    QMessageBox::critical(this,"You're Not Using Steam!","This is a tool to upload to Steam's"
                          "modding system. You're not using Steam, so it doesn't apply to you."
                          " You can still make and distrbute mods though. You'll just have to use "
                          "a different service. Like zipping up the mod folders and uploading it to "
                          "the forums! www.ventdev.com/forums/");
#endif
    wsc.SteamWorkshopCW->raise();

#if defined(Q_OS_MACX)
    this->repaint();
#endif
    return;
}
