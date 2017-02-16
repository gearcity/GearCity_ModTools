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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "MapDataTool.h"
#include "CityEditor.h"
#include "AIEditor.h"
#include "TurnEventsEditor.h"
#include "BaseContainerStorage.h"
#include "MaterialMakerWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    //Resizes child widget containers and forms
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_Button_MapDataTools_clicked();
    void on_Button_CityEditor_clicked();
    void on_Button_AIEditor_clicked();
    void on_Button_TurnEventsEditor_clicked();
    void on_Button_ModDataTools_clicked();
    void on_Button_ScenarioDataTools_clicked();
    void on_Button_LocalizationEditor_clicked();
    void on_Button_ComponentEditor_clicked();
    void on_Button_NewsPaperEditor_clicked();
    void on_Button_MagazineEditor_clicked();
    void on_Button_DYKEditor_clicked();
    void on_Button_LogoEditor_clicked();
    void on_Button_MusicEditor_clicked();
    void on_Button_PreMadeVehicleLoader_clicked();
    void on_Button_Racing_clicked();

private:
     Ui::MainWindow *ui;

     //Editor Objects
     MapDataTool *mapTool;
     CityEditor *cityEditorTool;
     AIEditor *aiEditorTool;
     TurnEventsEditor *turnEventEditorTool;

     //Widget Storage Container for all Editor containers
     //This is to be used to raise and lower various editors in the program.
     widgetContainerStorage wsc;
};

#endif // MAINWINDOW_H
