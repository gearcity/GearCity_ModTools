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


#ifndef AIEDITOR_H
#define AIEDITOR_H

#include <QWidget>
#include <QFrame>
#include "BaseContainerStorage.h"
#include <QMap>
#include <QModelIndex>
#include "CityData.h"
#include "AIManager.h"

namespace Ui {
class AIEditor;
}

class AIEditor : public QFrame
{
    Q_OBJECT

public:
    explicit AIEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~AIEditor();

private slots:
    void on_Button_AI_ReturnToPrevious_clicked();
    void on_Button_AI_MapEditor_clicked();
    void on_Button_AI_CityEditor_clicked();
    void on_Button_AI_TurnEventsEditor_clicked();
    void on_button_SaveAINode_clicked();
    void on_Button_makeNewAIList_clicked();
    void on_table_AICompanyList_clicked(const QModelIndex &index);
    void on_button_SaveAIList_clicked();
    void on_button_OpenAIList_clicked();
    void on_button_LoadCityScript_clicked();
    void on_button_LoadLogoFolder_clicked();
    void on_combo_OwnerList_currentIndexChanged(const QString &arg1);
    void on_spin_OwnerID_valueChanged(int arg1);
    void on_spin_HQID_valueChanged(int arg1);
    void on_combo_CityList_currentIndexChanged(const QString &arg1);
    void on_combo_LogoList_currentIndexChanged(const QString &arg1);
    void sectionDoubleClickedSlot(int index);
    void on_combo_QuickFinances_currentIndexChanged(int index);
    void on_combo_QuickSkills_currentIndexChanged(int index);
    void on_combo_QuickDesignSkills_currentIndexChanged(int index);
    void on_combo_QuickImage_currentIndexChanged(int index);
    void on_combo_QuickDesignBehavior_currentIndexChanged(int index);
    void on_combo_QuickAgression_currentIndexChanged(int index);
    void on_button_NewAICompany_clicked();
    void on_button_RemoveAICompany_clicked();



private:
    Ui::AIEditor *ui;
    widgetContainerStorage cp_wsc;
    QMap<int, AIManager::AIInfo> aiMap;
    QMap<int, CityData::dataStore> cityMap;
    QString saveFileName;
    bool refillList;

    void saveOrEditAINode(AIManager::AIInfo aiNode);
    void fillTableList();
    void resetAINode();
    QString getExistingNameByID(int id);
    int getExistingIDByName(QString name);
    void saveXML();
    void openAIScript(QString fileName);
    void openCityXML(QString openFileName);
    int getCityIDByName(QString name);
    QString getCityNameByID(int ID);

};

#endif // AIEDITOR_H
