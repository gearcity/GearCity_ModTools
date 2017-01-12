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


#ifndef CITYEDITOR_H
#define CITYEDITOR_H

#include <QWidget>
#include <QFrame>
#include "BaseContainerStorage.h"
#include "CityData.h"
#include <QMap>
#include <QModelIndex>
#include <QFile>


namespace Ui {
class CityEditor;
}

class CityEditor : public QFrame
{
    Q_OBJECT

public:
    explicit CityEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~CityEditor();

private slots:
    void on_Button_CE_ReturnToPrevious_clicked();
    void on_Button_CE_MapEditor_clicked();
    void on_Button_CE_NewCityList_clicked();
    void on_Button_CE_SaveCityChange_clicked();
    void sectionDoubleClickedSlot(int index);
    void on_Table_CE_CitiesInList_clicked(const QModelIndex &index);
    void on_DropDown_CE_ExistingCountries_currentIndexChanged(const QString &arg1);
    void on_Combobox_CE_FlagSelector_currentIndexChanged(const QString &arg1);
    void on_Combobox_CE_HeaderSelector_currentIndexChanged(const QString &arg1);
    void on_Button_CE_MakeNewCity_clicked();
    void on_Button_CE_NewCloneCity_clicked();
    void on_Button_CE_RemoveSelectedCity_clicked();
    void on_Button_CE_SaveCityList_clicked();
    void on_Button_CE_LoadCityList_clicked();
    void on_Button_CE_InterpolationSourceFinder_clicked();
    void on_LineEdit_CE_Population_textEdited(const QString &arg1);
    void on_LineEdit_CE_PerCapita_textEdited(const QString &arg1);
    void on_SpinBox_CE_ManufacturingSkills_editingFinished();
    void on_SpinBox_CE_CityListYear_editingFinished();
    void on_SpinBox_CE_InterpolateYear_editingFinished();
    void on_Button_CityEditor_CancelInterpolater_clicked();
    void on_Button_CE_AIEditor_clicked();



    void on_Button_CE_TurnEventsEditor_clicked();

private:
    Ui::CityEditor *ui;
    widgetContainerStorage cp_wsc;
    QMap<int,CityData::dataStore> cityMap;
    QMap<int,CityData::dataStore> interpolateMap;
    QString saveFileName;

    void fillTableList();
    bool checkNewCityData(CityData::dataStore saveDS);
    void saveXML();
    void openXML(QString openFileName, bool interpolater);
    void interpolateValues(int key);
};

#endif // CITYEDITOR_H
