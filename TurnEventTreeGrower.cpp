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


#include "TurnEventTreeGrower.h"

//TurnEventsTreeGrower creates the main tree in the all tab.
TurnEventTreeGrower::TurnEventTreeGrower(QTreeWidget *tree, QMap<int,TurnData::TE_Data> map,
                                         QMap<int, CityData::dataStore> cityMap,
                                         int  startYearLimit, int finishYearLimit,
                                         QComboBox *compCombo,
                                         QComboBox *carCombo,
                                         LocalizationManager *lm)
{

    //Copy Localization Pointer
    localManager = lm;


    //loop through turn map and turn it into a tree
    for(QMap<int,TurnData::TE_Data>::iterator it = map.begin(); it != map.end(); ++it)
    {
        if(it.key()< startYearLimit || it.key() > finishYearLimit)
            continue;


        QTreeWidgetItem *treeItem = new QTreeWidgetItem(tree);
        if(it.key()%12 == 0)
            treeItem->setText(0,"12/"+QString::number(it.key()/12-1));
        else
            treeItem->setText(0,QString::number(it.key()%12)+"/"+QString::number(it.key()/12));

        if((*it).officeFile != "")
            addSingleValueLeafToTree(treeItem,"office","file=",(*it).officeFile);

        if((*it).globalInterest > -1)
            addSingleValueLeafToTree(treeItem,"interest","global=",
                                     QString::number((*it).globalInterest));

        if((*it).globalFuelRate > -1)
            addSingleValueLeafToTree(treeItem,"gas","rate=",
                                     QString::number((*it).globalFuelRate));

        if((*it).carInflation > -1)
            addSingleValueLeafToTree(treeItem,"carprice","rate=",
                                     QString::number((*it).carInflation));

        if((*it).buyerRate > -1)
            addSingleValueLeafToTree(treeItem,"buyrate","rate=",QString::number((*it).buyerRate));

        if((*it).pensionRate > -1)
            addSingleValueLeafToTree(treeItem,"pensionGrowth","rate=",
                                     QString::number((*it).pensionRate));


        if((*it).stockRate > -1)
            addSingleValueLeafToTree(treeItem,"stockrate","rate=",QString::number((*it).stockRate));


        if(!(*it).CompEvtList.empty())
        {
            for(QList<TurnData::TE_ComponentEvts>::iterator itCompEL = (*it).CompEvtList.begin();
                itCompEL != (*it).CompEvtList.end(); ++itCompEL)
            {
                addComponentLeafToTree(treeItem,(*itCompEL), compCombo);
            }
        }

        if(!(*it).CityEvtList.empty())
        {
            for(QList<TurnData::TE_CityEvts>::iterator itCityEL = (*it).CityEvtList.begin();
                itCityEL != (*it).CityEvtList.end(); ++itCityEL)
            {
                addCityLeafToTree(treeItem,(*itCityEL),cityMap);
            }
        }

        if(!(*it).NewsEvtList.empty())
        {
            for(QList<TurnData::TE_NewsEvts>::iterator itNewsEL = (*it).NewsEvtList.begin();
                itNewsEL != (*it).NewsEvtList.end(); ++itNewsEL)
            {
               addNewsLeafToTree(treeItem,(*itNewsEL));
            }
        }

        if(!(*it).vehiclePopList.empty())
        {
            for(QList<TurnData::TE_VehiclePop>::iterator itCarEL = (*it).vehiclePopList.begin();
                itCarEL != (*it).vehiclePopList.end(); ++itCarEL)
            {
                addVehicleLeafToTree(treeItem,(*itCarEL), carCombo);
            }
        }


    }
}


//Generic leaf adder for elements that only have 1 attribute
void TurnEventTreeGrower::addSingleValueLeafToTree(QTreeWidgetItem *parent, QString event,
                                              QString valueName, QString valueData)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(1,event);
    treeItem->setText(2,valueName);
    treeItem->setText(3,valueData);
    parent->addChild(treeItem);
}

//Leaf adder for components
void TurnEventTreeGrower::addComponentLeafToTree(QTreeWidgetItem *parent,
                                                 TurnData::TE_ComponentEvts tecoe,
                                                 QComboBox *compCombo)
{
    QString componentName = "";
    int comboIndex = compCombo->findData(QVariant(tecoe.selectedID));
    if(comboIndex != -1)
    {
        componentName = "("+compCombo->itemText(comboIndex)+")";
    }
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(1,"component");
    treeItem->setText(2,"selectedIndex=");
    treeItem->setText(3,QString::number(tecoe.selectedID)+componentName);
    treeItem->setText(4,"popChange=");
    treeItem->setText(5,QString::number(tecoe.popChange));
    parent->addChild(treeItem);
}

//Leaf adder for cities
void TurnEventTreeGrower::addCityLeafToTree(QTreeWidgetItem *parent, TurnData::TE_CityEvts tecie,
                                            QMap<int, CityData::dataStore> cityMap)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(1,"cityChange");
    treeItem->setText(2,"id=");
    if(cityMap.contains(tecie.id))
    {
        treeItem->setText(3,QString::number(tecie.id)+
                      " ("+cityMap.find(tecie.id).value().cityName+")");
    }
    else
    {
        treeItem->setText(3,QString::number(tecie.id));
    }


    if(tecie.bNationOrFlagChange)
    {
        treeItem->setText(4,"Flag=");
        treeItem->setText(5,tecie.Flag);
        treeItem->setText(6,"Nation=");
        treeItem->setText(7,tecie.Nation);
    }
    if(tecie.bgovChange)
    {
        treeItem->setText(8,"gov=");
        treeItem->setText(9,QString::number(tecie.gov));
    }

    if(tecie.bInfManuChange)
    {
        treeItem->setText(10,"infChange=");
        treeItem->setText(11,QString::number(tecie.infChange));
        treeItem->setText(12,"manuChange=");
        treeItem->setText(13,QString::number(tecie.manuChange));
    }




    parent->addChild(treeItem);
}


//Leaf adder for news
void TurnEventTreeGrower::addNewsLeafToTree(QTreeWidgetItem *parent, TurnData::TE_NewsEvts tene)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(1,"news comment");
    treeItem->setText(2,"type=");
    treeItem->setText(3,QString::number(tene.type));
    treeItem->setText(4,"localization=");
    treeItem->setText(5,QString::number(tene.localization));
    treeItem->setText(6,"headline=");
    treeItem->setText(8,"body=");
    if(localManager != 0 && tene.localization)
    {
        treeItem->setText(7,localManager->getWord(tene.headline.toInt()));
        treeItem->setText(9,localManager->getWord(tene.body.toInt()));
    }
    else
    {
        treeItem->setText(7,tene.headline);
        treeItem->setText(9,tene.body);
    }

    treeItem->setText(10,"image=");
    treeItem->setText(11,tene.image);

     parent->addChild(treeItem);
}


//Leaf adder for vehicle
void TurnEventTreeGrower::addVehicleLeafToTree(QTreeWidgetItem *parent,
                                                 TurnData::TE_VehiclePop tevp,
                                                 QComboBox *carCombo)
{
    QString typeName = "";
    int comboIndex = carCombo->findData(QVariant(tevp.selectorID));
    if(comboIndex != -1)
    {
        typeName = "("+carCombo->itemText(comboIndex)+")";
    }

    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(1,"vehicle");
    treeItem->setText(2,"selectedIndex=");
    treeItem->setText(3,QString::number(tevp.selectorID)+typeName);
    treeItem->setText(4,"pop=");
    treeItem->setText(5,QString::number(tevp.pop));
    treeItem->setText(6,"popR1=");
    treeItem->setText(7,QString::number(tevp.popR1));
    treeItem->setText(8,"popR2=");
    treeItem->setText(9,QString::number(tevp.popR2));
    treeItem->setText(10,"popR3=");
    treeItem->setText(11,QString::number(tevp.popR3));
    treeItem->setText(12,"popR4=");
    treeItem->setText(13,QString::number(tevp.popR4));
    treeItem->setText(14,"popR5=");
    treeItem->setText(15,QString::number(tevp.popR5));
    treeItem->setText(16,"popR6=");
    treeItem->setText(17,QString::number(tevp.popR6));
    parent->addChild(treeItem);
}
