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

    //Create the forms and send the objects the container widgets to attach to.
    mapTool = new MapDataTool(wsc, ui->MapDataToolWidgetContainer);
    cityEditorTool = new CityEditor(wsc, ui->CityEditorWidgetContainer);
    aiEditorTool = new AIEditor(wsc, ui->AIEditorWidgetContainer);
    turnEventEditorTool = new TurnEventsEditor(wsc, ui->TurnEventEditorWidgetContainer);

    //Raise up the main menu container widget.
    wsc.MainMenuCW->raise();
}

MainWindow::~MainWindow()
{
    //Delete all those pointers.
    delete ui;
    delete mapTool;
    delete cityEditorTool;
    delete aiEditorTool;
    delete turnEventEditorTool;

}

//MapTools Button
void MainWindow::on_Button_MapDataTools_clicked()
{
   //Raise the MapTool
    wsc.MapToolCW->raise();
}

//CityEditor Button
void MainWindow::on_Button_CityEditor_clicked()
{
    //Raise the CityEditor
    wsc.CityEditorCW->raise();
}

//AIEditor Button
void MainWindow::on_Button_AIEditor_clicked()
{
    //Raise the AIEditor
    wsc.AIEditorCW->raise();
}

//TurnEvents Editor
void MainWindow::on_Button_TurnEventsEditor_clicked()
{
    //Raise the TurnEvents Editor
    wsc.TurnEventEditorCW->raise();
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

    //Resize the object forms based on the Main Window size.
    mapTool->resize(this->width(), this->height());
    cityEditorTool->resize(this->width(), this->height());
    aiEditorTool->resize(this->width(), this->height());
    turnEventEditorTool->resize(this->width(), this->height());
}
