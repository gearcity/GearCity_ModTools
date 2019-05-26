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


#ifndef COMPONENTSMANAGER_H
#define COMPONENTSMANAGER_H
#include <QString>
#include <QList>
#include <QWidget>
#include <QComboBox>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include "LocalizationManager.h"

class ComponentsManager
{
public:
    class ChassisFrameComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death, skill, selectionIndex;
        double str,performance, safety, durability, weight, design, manu, costs, popindex;
        int localName, localDescription, localPara;
    };
    class SuspensionComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death, skill, selectionIndex;
        double steering,braking, comfort, performance, durability, design, manu, costs, popindex;
        int localName, localDescription, localPara;

    };
    class DriveTrainComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death, skill, selectionIndex;
        double ridePerformance, rideSteering, duriblity, weight, carPerformance, engineWidth;
        double engineLength, manu, design, costs, popindex;
        int localName, localDescription, localPara;

    };
    class EngineLayoutComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death, valve;
        double cost, designCosts, design, manu, reliability, weight, finish, width, length, smooth;
        double popIndex, layoutFuel, layoutpower ;
        int skill, selectionIndex, useDisplacement, cylinderLengthArrangment;
        QString possibleCyl, possibleFuel, possibleInduction;
        int localName, localDescription, localPara;
        int turbine;

        QStringList supportCyl;
        QStringList supportFuel;
        QStringList supportInduct;

    };
    class EngineCylinderComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death;
        double cylCosts, designCosts, design, manu, power, fuel, reliability, weight, finish, smooth;
        int numberCyl, skill, selectionIndex;
        double popIndex;
        int localName, localDescription, localPara;
    };
    class EngineFuelComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death;
        double costs, designCost, design, manu, power, fuel, reliability, weight, finish, rpm, smooth;
        double popIndex;
        int skill, selectionIndex;
        int localName, localDescription, localPara;
        QString popLink;
    };
    class EngineValveComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death, group;
        double cost, designCost, design, manu, fuel, reliability, weight, finish, rpm, smooth, size;
        double popIndex, power;
        int skill, selectionIndex;
        int localName, localDescription, localPara;

    };
    class EngineInductionComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death;
        double costs, designCosts, design, manu, power, fuel, reliability, weight, finish, popIndex;
        int skill, selectionIndex;
        int localName, localDescription, localPara;

    };
    class GearBoxComps
    {
        public:
        QString name, typeName, about, picture, shift;
        int year, death;
        double costs, designCosts, weight, complex, smooth, comfort, fuel, performance, popIndex;
        int skill, selectionIndex;
        QString possibleGears, possibleAddons;
        int localName, localDescription, localPara;

        QStringList possiblegearsList;
        int trans, reverse, overdrive, limited;
    };

    class GearGearsComps
    {
        public:
        QString name, typeName, about, picture;
        int year, death, gears, skill, selectionIndex;
        double popIndex;
        int localName, localDescription, localPara;
    };

    class GearAddonsComps
    {
        public:
        QString name, typeName, about, picture, shift;
        int year, death, selectionIndex;
        double popIndex;
        int localName, localDescription, localPara;

    };
    class CarModels
    {
        public:
         QString name, typeName, model;
         int year, death, designReq, pass;
         int localName, localDescription, localPara;

    };
    class AccessoriesModels
    {
        public:
        QString name, typeName, model, picture;
        int year, death,designreq, autoMirror, autoPaint;
        int decal, height, width;
        int localName, localDescription, localPara;
    };
    class CarTypes
    {
        public:
        QString type, about, picture;
        int year, death;
        double rPerformance, rDrivability, rLuxury, rSafety, rFuel, rPower, rCargo, rDependability;
        double popNA, popEU, popSA, popAF, popAS, popAU, pop;
        double cargo;
        int wealth, age, pass, milFleet, civFleet, civ;
        bool aval;
        int localName, localDescription, localPara;
        int selectionIndex;
        double weight, size;

    };

    class aiPopData
    {
        public:
            int year;
            float fuel, performance, luxury, costs, ovrpop;
            int id;
            QString name;
    };

    /*//Will move this out of class and out of xml file
    class RacingSeries
    {
        public:
        QString name, seriesType, carType, country, image, trophy;
        int id, supply, ccRestrict,start, end, year, death;
        int tracks, teams, manuInvolve;
        bool singleMake, singleConstructor;
        double funding, popularity;
        int localName, localDescription, localPara;

    };

     //Will move this out of class and out of xml file
    class ContractEngines
    {
        public:
        QString nameFor;
        int type, military, birth, death;
        double ccMod, weightMod, fuelMod, hpMod, widthMod, lengthMod;
        int localeName;

    };*/

    class ComponentDataLists
    {
        public:
            QList<ChassisFrameComps> frameList;
            QList<SuspensionComps> suspensionList;
            QList<DriveTrainComps> drivetrainList;
            QList<EngineLayoutComps> engineLayoutList;
            QList<EngineCylinderComps> engineCylinderList;
            QList<EngineFuelComps> engineFuelList;
            QList<EngineValveComps> engineValveList;
            QList<EngineInductionComps> engineInductionList;
            QList<GearBoxComps> gearboxTypeList;
            QList<GearGearsComps> gearboxGearsList;
            QList<GearAddonsComps> gearboxAddonsList;
            QList<CarModels> carModelsList;
            QList<AccessoriesModels> accessoriesList;
            QList<CarTypes> cartypesList;
          /*  QList<RacingSeries> racingSeriesList;
            QList<ContractEngines> contractEnginesList;*/
    };


    ComponentsManager();
    ComponentsManager(QString openFileName, QWidget *widget);
    void createComponentSelectorCombo(QComboBox *combo, QComboBox *carCombo,                                      
                                      LocalizationManager *lm = 0);

    ComponentDataLists returnDataList();
    QMap<QString, ComponentsManager::aiPopData> returnPopsMap();

    bool parseAIComponentsPopularity(QString openFileName, QWidget *widget);
    bool isGood();

    static bool saveComponentsXMLFile(QString componentsFileName,
                                                    ComponentsManager::ComponentDataLists saveData);
    static bool saveComponentsPopXMLFile(QString popFileName,
                            QMap<QString, ComponentsManager::aiPopData> savePop);

private:
    ComponentDataLists dataList;
    QMap<QString, ComponentsManager::aiPopData> aiPops;

    void chassisFrameRead(QDomElement element);
    void chassisSuspensionRead(QDomElement element);
    void chassisDrivetrainRead(QDomElement element);
    void engineLayotRead(QDomElement element);
    void engineCylinderRead(QDomElement element);
    void engineFuelRead(QDomElement element);
    void engineValveRead(QDomElement element);
    void engineInductionRead(QDomElement element);
    void gearboxTypeRead(QDomElement element);
    void gearboxGearsRead(QDomElement element);
    void gearboxAddonsRead(QDomElement element);
    void carModelRead(QDomElement element);
    void accessoryModelRead(QDomElement element);
    void carTypeRead(QDomElement element);


    //Strip these from this class and put in their own xml
    void racingSeriesRead(QDomElement element);
    void contractEngineRead(QDomElement element);

    bool processedFile;




};

#endif // COMPONENTSMANAGER_H
