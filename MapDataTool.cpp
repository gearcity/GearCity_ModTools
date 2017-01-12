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


#include "MapDataTool.h"
#include "ui_MapDataTool.h"

//Create map data tool
MapDataTool::MapDataTool(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapDataTool)
{
    ui->setupUi(this);

    //copy widget storage container
    cp_wsc = wsc;

    //align widgets on the same grid!
    ui->gridLayout->addWidget(ui->GroupBox_MapTools_AIStartingOptions,1,1);
    ui->gridLayout->addWidget(ui->GroupBox_MapTools_CityScriptLoadingYear,1,1);
    ui->gridLayout->addWidget(ui->Group_MapTools_MapArtwork_Change,1,1);
}

MapDataTool::~MapDataTool()
{
    delete ui;
}

//Open Main Menu
void MapDataTool::on_Button_MapTools_ReturnToMain_clicked()
{
    cp_wsc.MainMenuCW->raise();
}

//Open City Editor
void MapDataTool::on_Button_MapTool_CityEditor_clicked()
{
    cp_wsc.CityEditorCW->raise();
}

//Open AI Editor
void MapDataTool::on_Button_MapTool_AIEditor_clicked()
{
    cp_wsc.AIEditorCW->raise();
}

//Open Turn Events Editor
void MapDataTool::on_Button_MapTool_TurnEventsEditor_clicked()
{
    cp_wsc.TurnEventEditorCW->raise();
}
