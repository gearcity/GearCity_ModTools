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


#include "ComponentsManager.h"
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamWriter>

//Open Components XML file.
ComponentsManager::ComponentsManager()
{

}

ComponentsManager::ComponentsManager(QString openFileName, QWidget *widget)
{
    processedFile = true;

    //Open the Components.xml file
    QFile file(openFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(widget,"Could Not open file!!!!",
                              "We had a problem reading this file!");

        processedFile = false;
             return;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement BaseElement;
    QDomElement subElement;



    //Error in prasing XML file
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        QMessageBox::critical(widget,"Error opening file!!!!",
                              "This file is not valid xml!");
        processedFile = false;
       return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("Components");
    if(rootNode.isNull())
    {
        QMessageBox::critical(widget,"Error opening file!!!!",
                              "This file is a components xml for the game!");
        processedFile = false;
        return;
    }

    //
    //Get chassis components and read data
    //
    rootNode = doc.documentElement();

    BaseElement = rootNode.firstChildElement("ChassisComponents");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("chassis");
        while(!subElement.isNull())
        {
            chassisFrameRead(subElement);
            subElement = subElement.nextSiblingElement("chassis");
        }
    }



    BaseElement = rootNode.firstChildElement("SuspensionComponents");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("suspension");
        while(!subElement.isNull())
        {
            chassisSuspensionRead(subElement);
            subElement = subElement.nextSiblingElement("suspension");
        }
    }


    BaseElement = rootNode.firstChildElement("DrivetrainComponents");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("drivetrain");
        while(!subElement.isNull())
        {
            chassisDrivetrainRead(subElement);
            subElement = subElement.nextSiblingElement("drivetrain");
        }
    }

    //
    //Get engine components and read data
    //
    BaseElement = rootNode.firstChildElement("EngineComponents");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("layout");
        while(!subElement.isNull())
        {
            engineLayotRead(subElement);
            subElement = subElement.nextSiblingElement("layout");
        }

        subElement = BaseElement.firstChildElement("cylinder");
        while(!subElement.isNull())
        {
            engineCylinderRead(subElement);
            subElement = subElement.nextSiblingElement("cylinder");
        }

        subElement = BaseElement.firstChildElement("fuel");
        while(!subElement.isNull())
        {
            engineFuelRead(subElement);
            subElement = subElement.nextSiblingElement("fuel");
        }

        subElement = BaseElement.firstChildElement("valve");
        while(!subElement.isNull())
        {
            engineValveRead(subElement);
            subElement = subElement.nextSiblingElement("valve");
        }

        subElement = BaseElement.firstChildElement("induction");
        while(!subElement.isNull())
        {
            engineInductionRead(subElement);
            subElement = subElement.nextSiblingElement("induction");
        }
    }

    //
    //Get GearBox components and read data
    //
    BaseElement = rootNode.firstChildElement("GearboxComponents");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("gearbox");
        while(!subElement.isNull())
        {
            gearboxTypeRead(subElement);
            subElement = subElement.nextSiblingElement("gearbox");
        }

        subElement = BaseElement.firstChildElement("gears");
        while(!subElement.isNull())
        {
            gearboxGearsRead(subElement);
            subElement = subElement.nextSiblingElement("gears");
        }

        subElement = BaseElement.firstChildElement("addons");
        while(!subElement.isNull())
        {
            gearboxAddonsRead(subElement);
            subElement = subElement.nextSiblingElement("addons");
        }
    }

    //
    //Get car models and read data
    //
    BaseElement = rootNode.firstChildElement("CarModels");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("car");
        while(!subElement.isNull())
        {
            carModelRead(subElement);
            subElement = subElement.nextSiblingElement("car");
        }
    }

    //
    //Get accessories models and read data
    //
    BaseElement = rootNode.firstChildElement("AccessoriesModels");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("access");
        while(!subElement.isNull())
        {
            accessoryModelRead(subElement);
            subElement = subElement.nextSiblingElement("access");
        }
    }

    //
    //Get car types and read data
    //
    BaseElement = rootNode.firstChildElement("NewCarType");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("car");
        while(!subElement.isNull())
        {
            carTypeRead(subElement);
            subElement = subElement.nextSiblingElement("car");
        }
    }


    //
    //Get racing series and read data, this should be moved to its own file
    //
 /*   BaseElement = rootNode.firstChildElement("RacingSeries");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("series");
        while(!subElement.isNull())
        {
            racingSeriesRead(subElement);
            subElement = subElement.nextSiblingElement("series");
        }
    }

    //
    //Get engine contract data and read data, this should be moved to its own file.
    //
    BaseElement = rootNode.firstChildElement("ContractingEngines");
    if(!BaseElement.isNull())
    {
        subElement = BaseElement.firstChildElement("engine");
        while(!subElement.isNull())
        {
            contractEngineRead(subElement);
            subElement = subElement.nextSiblingElement("engine");
        }
    }*/





    file.close();

}

//Read- ChassisComponents  chassis
void ComponentsManager::chassisFrameRead(QDomElement element)
{
    ChassisFrameComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.str = element.attributeNode("str").value().toDouble();
        data.performance = element.attributeNode("performance").value().toDouble();
        data.safety = element.attributeNode("safety").value().toDouble();
        data.durability = element.attributeNode("durability").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.costs = element.attributeNode("cost").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popindex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();

    dataList.frameList.push_back(data);
}
//Read- SuspensionComponents  suspension
void ComponentsManager::chassisSuspensionRead(QDomElement element)
{
    SuspensionComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.steering = element.attributeNode("steering").value().toDouble();
        data.braking = element.attributeNode("braking").value().toDouble();
        data.comfort = element.attributeNode("comfort").value().toDouble();
        data.performance = element.attributeNode("performance").value().toDouble();
        data.durability = element.attributeNode("durability").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.costs = element.attributeNode("cost").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popindex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();

    dataList.suspensionList.push_back(data);
}
//Read- DrivetrainComponents  drivetrain
void ComponentsManager::chassisDrivetrainRead(QDomElement element)
{
    DriveTrainComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.ridePerformance = element.attributeNode("ridePerformance").value().toDouble();
        data.rideSteering = element.attributeNode("rideSteering").value().toDouble();
        data.duriblity = element.attributeNode("duriblity").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.carPerformance = element.attributeNode("carPerformance").value().toDouble();
        data.engineWidth = element.attributeNode("engineWidth").value().toDouble();
        data.engineLength = element.attributeNode("engineLength").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.costs = element.attributeNode("cost").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popindex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();

    dataList.drivetrainList.push_back(data);
}


//Read- EngineComponents  layout
void ComponentsManager::engineLayotRead(QDomElement element)
{
    EngineLayoutComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.valve = element.attributeNode("valve").value().toInt();
        data.cost = element.attributeNode("cost").value().toDouble();
        data.designCosts = element.attributeNode("designcost").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();


        data.reliability = element.attributeNode("reliability").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();

        data.finish = element.attributeNode("finish").value().toDouble();
        data.width = element.attributeNode("width").value().toDouble();
        data.length = element.attributeNode("length").value().toDouble();
        data.smooth = element.attributeNode("smooth").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.possibleCyl = element.attributeNode("possiblecyl").value();
        data.possibleFuel = element.attributeNode("possiblefuel").value();
        data.possibleInduction = element.attributeNode("possibleinduction").value();
        data.layoutFuel  = element.attributeNode("layoutfuel").value().toDouble();
        data.layoutpower = element.attributeNode("layoutpower").value().toDouble();
        data.picture = element.attributeNode("picture").value();
        data.useDisplacement = element.attributeNode("useDisplacement").value().toInt();
        data.cylinderLengthArrangment = element.attributeNode(
                    "layoutCylinderLengthArrangment").value().toInt();

        if(!element.attributeNode("turbine").isNull())
            data.turbine = element.attributeNode("turbine").value().toInt();
        else
            data.turbine = 0;

        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();

        data.supportCyl = data.possibleCyl.split("/");
        data.supportFuel = data.possibleFuel.split("/");
        data.supportInduct = data.possibleInduction.split("/");


    dataList.engineLayoutList.push_back(data);
}


//Read- EngineComponents  cylinder
void ComponentsManager::engineCylinderRead(QDomElement element)
{
    EngineCylinderComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.cylCosts = element.attributeNode("cylcost").value().toDouble();
        data.designCosts = element.attributeNode("designcost").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.power = element.attributeNode("power").value().toDouble();
        data.fuel = element.attributeNode("fuel").value().toDouble();
        data.reliability = element.attributeNode("reliability").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.finish = element.attributeNode("finish").value().toDouble();
        data.numberCyl = element.attributeNode("numbercyl").value().toInt();
        data.smooth = element.attributeNode("smooth").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.engineCylinderList.push_back(data);
}

//Read- EngineComponents  fuel
void ComponentsManager::engineFuelRead(QDomElement element)
{
    EngineFuelComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.costs = element.attributeNode("cost").value().toDouble();
        data.designCost = element.attributeNode("designcost").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.power = element.attributeNode("power").value().toDouble();
        data.fuel = element.attributeNode("fuel").value().toDouble();
        data.reliability = element.attributeNode("reliability").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.finish = element.attributeNode("finish").value().toDouble();
        data.rpm = element.attributeNode("rpm").value().toDouble();
        data.smooth = element.attributeNode("smooth").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.engineFuelList.push_back(data);
}

//Read- EngineComponents  valve
void ComponentsManager::engineValveRead(QDomElement element)
{
    EngineValveComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.group = element.attributeNode("limit").value().toInt();
        data.cost  = element.attributeNode("cost").value().toDouble();
        data.designCost = element.attributeNode("designcost").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.fuel = element.attributeNode("fuel").value().toDouble();
        data.reliability = element.attributeNode("reliability").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.finish = element.attributeNode("finish").value().toDouble();
        data.rpm = element.attributeNode("rpm").value().toDouble();
        data.smooth = element.attributeNode("smooth").value().toDouble();
        data.size = element.attributeNode("size").value().toDouble();
        if(!element.attributeNode("power").isNull())
            data.power = element.attributeNode("power").value().toDouble();
        else
            data.power = 1;

        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.engineValveList.push_back(data);
}


//Read- EngineComponents  induction
void ComponentsManager::engineInductionRead(QDomElement element)
{
    EngineInductionComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.costs  = element.attributeNode("cost").value().toDouble();
        data.designCosts = element.attributeNode("designcost").value().toDouble();
        data.design = element.attributeNode("design").value().toDouble();
        data.manu = element.attributeNode("manu").value().toDouble();
        data.power = element.attributeNode("power").value().toDouble();
        data.fuel = element.attributeNode("fuel").value().toDouble();
        data.reliability = element.attributeNode("reliability").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.finish = element.attributeNode("finish").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.engineInductionList.push_back(data);
}


//Read- GearboxComponents  gearbox
void ComponentsManager::gearboxTypeRead(QDomElement element)
{
    GearBoxComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.shift = element.attributeNode("shift").value();
        data.costs  = element.attributeNode("cost").value().toDouble();
        data.designCosts = element.attributeNode("designcost").value().toDouble();
        data.weight = element.attributeNode("weight").value().toDouble();
        data.complex = element.attributeNode("complex").value().toDouble();
        data.smooth = element.attributeNode("smooth").value().toDouble();
        data.comfort = element.attributeNode("comfort").value().toDouble();
        data.fuel = element.attributeNode("fuel").value().toDouble();
        data.performance = element.attributeNode("performance").value().toDouble();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.possibleGears = element.attributeNode("possiblegears").value();
        data.possibleAddons = element.attributeNode("possibleaddons").value();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();


        QStringList addonList = data.possibleAddons.split("/");
        data.trans = 0, data.reverse = 0, data.overdrive = 0, data.limited = 0;

        for(QStringList::Iterator it = addonList.begin(); it != addonList.end(); ++it)
        {
            if((*it) == "13156")
            {
              data.trans = 1;
            }
            else if((*it) == "13157")
            {
              data.reverse = 1;
            }
            else if((*it) == "13158")
            {
              data.overdrive = 1;
            }
            else if((*it) == "13159")
            {
              data.limited = 1;
            }
        }

        data.possiblegearsList = data.possibleGears.split("/");


    dataList.gearboxTypeList.push_back(data);
}


//Read- GearboxComponents  gears
void ComponentsManager::gearboxGearsRead(QDomElement element)
{
    GearGearsComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.gears = element.attributeNode("gears").value().toInt();
        data.skill = element.attributeNode("skill").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.gearboxGearsList.push_back(data);
}

//Read- GearboxComponents  addons
void ComponentsManager::gearboxAddonsRead(QDomElement element)
{
    GearAddonsComps data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.popIndex = element.attributeNode("popindex").value().toDouble();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();
        data.picture = element.attributeNode("picture").value();
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.gearboxAddonsList.push_back(data);
}
//Read- CarModels  car
void ComponentsManager::carModelRead(QDomElement element)
{
    CarModels data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.model = element.attributeNode("model").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.designReq = element.attributeNode("designreq").value().toInt();
        data.pass = element.attributeNode("pass").value().toInt();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
    dataList.carModelsList.push_back(data);
}


//Read- AccessoriesModels  access
void ComponentsManager::accessoryModelRead(QDomElement element)
{
    AccessoriesModels data;
        data.name = element.attributeNode("name").value();
        data.typeName = element.attributeNode("type").value();
        data.model = element.attributeNode("model").value();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.picture = element.attributeNode("image").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
        data.autoMirror = element.attributeNode("autoMirror").value().toInt();
        data.autoPaint = element.attributeNode("autoPaint").value().toInt();

        if(!element.attributeNode("width").isNull())
        {
            data.decal = 1;
            data.width = element.attributeNode("width").value().toInt();
            data.height = element.attributeNode("height").value().toInt();
        }
        else
        {
            data.decal = 0;
        }

        if(!element.attributeNode("Skill").isNull())
        {
            data.designreq = element.attributeNode("Skill").value().toInt();
        }

    dataList.accessoriesList.push_back(data);
}


//Read- NewCarType  car
void ComponentsManager::carTypeRead(QDomElement element)
{
    CarTypes data;
        data.type = element.attributeNode("type").value();
        data.rPerformance = element.attributeNode("Rating_Performance").value().toDouble();
        data.rDrivability = element.attributeNode("Rating_Drivability").value().toDouble();
        data.rLuxury = element.attributeNode("Rating_Luxury").value().toDouble();
        data.rSafety = element.attributeNode("Rating_Safety").value().toDouble();
        data.rFuel = element.attributeNode("Rating_Fuel").value().toDouble();
        data.rPower = element.attributeNode("Rating_Power").value().toDouble();
        data.rCargo = element.attributeNode("Rating_Cargo").value().toDouble();
        data.rDependability = element.attributeNode("Rating_Dependability").value().toDouble();

        data.popNA = element.attributeNode("popNA").value().toDouble();
        data.popEU = element.attributeNode("popEU").value().toDouble();
        data.popSA = element.attributeNode("popSA").value().toDouble();
        data.popAF = element.attributeNode("popAF").value().toDouble();
        data.popAS = element.attributeNode("popAS").value().toDouble();
        data.popAU = element.attributeNode("popAU").value().toDouble();
        data.pop = element.attributeNode("pop").value().toDouble();

        if(element.attributeNode("aval").value().toLower() == "true")
            data.aval = true;
        else
            data.aval = false;
        data.wealth = element.attributeNode("wealth").value().toInt();
        data.age = element.attributeNode("age").value().toInt();
        data.about = element.attributeNode("about").value();
        data.picture = element.attributeNode("picture").value();

        data.cargo = element.attributeNode("cargo").value().toDouble();
        data.pass = element.attributeNode("pass").value().toDouble();
        data.year = element.attributeNode("year").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();
        data.milFleet = element.attributeNode("milFleet").value().toInt();
        data.civFleet = element.attributeNode("civFleet").value().toInt();
        data.civ = element.attributeNode("civ").value().toInt();
        data.selectionIndex = element.attributeNode("selectionIndex").value().toInt();

    dataList.cartypesList.push_back(data);
}

//Read- RacingSeries  series
/*void ComponentsManager::racingSeriesRead(QDomElement element)
{
    RacingSeries data;
        data.name = element.attributeNode("name").value();
        data.id = element.attributeNode("id").value().toInt();
        data.seriesType = element.attributeNode("type").value();
        data.supply = element.attributeNode("supply").value().toInt();
        data.carType = element.attributeNode("cartype").value();
        data.ccRestrict = element.attributeNode("ccrestrict").value().toInt();
        data.funding = element.attributeNode("funding").value().toDouble();
        data.popularity = element.attributeNode("popularity").value().toDouble();
        data.country = element.attributeNode("country").value();
        data.start = element.attributeNode("start").value().toInt();
        data.end = element.attributeNode("end").value().toInt();

        data.year = element.attributeNode("birth").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.tracks = element.attributeNode("tracks").value().toInt();
        data.teams = element.attributeNode("teams").value().toInt();

        if(element.attributeNode("singleMake").value().toLower() == "true")
            data.singleMake = true;
        else
            data.singleMake = false;

        if(element.attributeNode("singleConstructor").value().toLower() == "true")
            data.singleConstructor = true;
        else
            data.singleConstructor = false;
        data.image = element.attributeNode("image").value();
        data.trophy = element.attributeNode("trophy").value();
        data.manuInvolve = element.attributeNode("manuInvolve").value().toInt();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();

    dataList.racingSeriesList.push_back(data);
}


//Read- ContractingEngines  engine
void ComponentsManager::contractEngineRead(QDomElement element)
{
    ContractEngines data;
        data.nameFor = element.attributeNode("for").value();
        data.type = element.attributeNode("type").value().toInt();
        data.military = element.attributeNode("military").value().toInt();
        data.birth = element.attributeNode("birth").value().toInt();
        data.death = element.attributeNode("death").value().toInt();
        data.ccMod = element.attributeNode("ccModifier").value().toDouble();
        data.weightMod = element.attributeNode("weightModifier").value().toDouble();
        data.fuelMod = element.attributeNode("fuelModifier").value().toDouble();
        data.hpMod = element.attributeNode("hpModifier").value().toDouble();
        data.widthMod = element.attributeNode("widthModifier").value().toDouble();
        data.lengthMod = element.attributeNode("lengthModifier").value().toDouble();
        data.localeName = element.attributeNode("localName").value().toInt();

    dataList.contractEnginesList.push_back(data);
}*/

//Fill component selector combo boxes with the component and vehicel types
//Also include the selector id in the item data
void ComponentsManager::createComponentSelectorCombo(QComboBox *combo,
                                                     QComboBox *carCombo,                                                     
                                                     LocalizationManager *lm)
{

    for(QList<ChassisFrameComps>::iterator it = dataList.frameList.begin();
        it !=  dataList.frameList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Chassis Frame-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));

        }
        else
        {
            combo->addItem("Chassis Frame-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }

    for(QList<SuspensionComps>::iterator it = dataList.suspensionList.begin();
        it !=  dataList.suspensionList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Suspension-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Suspension-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }

    for(QList<DriveTrainComps>::iterator it = dataList.drivetrainList.begin();
        it !=  dataList.drivetrainList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Drivetrain-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Drivetrain-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }

    for(QList<EngineLayoutComps>::iterator it = dataList.engineLayoutList.begin();
        it !=  dataList.engineLayoutList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Engine Layout-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Engine Layout-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }

    for(QList<EngineCylinderComps>::iterator it = dataList.engineCylinderList.begin();
        it !=  dataList.engineCylinderList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Engine Cylinder-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Engine Cylinder-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }

    for(QList<EngineFuelComps>::iterator it = dataList.engineFuelList.begin();
        it !=  dataList.engineFuelList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Engine Fuel-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Engine Fuel-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }


    for(QList<EngineValveComps>::iterator it = dataList.engineValveList.begin();
        it !=  dataList.engineValveList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Engine Valves-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Engine Valves-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }


    for(QList<EngineInductionComps>::iterator it = dataList.engineInductionList.begin();
        it !=  dataList.engineInductionList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Engine Induction-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Engine Induction-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }


    for(QList<GearBoxComps>::iterator it = dataList.gearboxTypeList.begin();
        it !=  dataList.gearboxTypeList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Gearbox-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Gearbox-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }


    for(QList<GearGearsComps>::iterator it = dataList.gearboxGearsList.begin();
        it !=  dataList.gearboxGearsList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Gearbox Gears-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Gearbox Gears-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }


    for(QList<GearAddonsComps>::iterator it = dataList.gearboxAddonsList.begin();
        it !=  dataList.gearboxAddonsList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            combo->addItem("Gearbox Addons-"+lm->getWord((*it).name.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            combo->addItem("Gearbox Addons-"+(*it).name,
                           QVariant((*it).selectionIndex));
        }
    }


    for(QList<CarTypes>::iterator it = dataList.cartypesList.begin();
        it !=  dataList.cartypesList.end(); ++it)
    {
        if((*it).localName == 1 && lm != 0)
        {
            carCombo->addItem(lm->getWord((*it).type.toInt()),
                           QVariant((*it).selectionIndex));
        }
        else
        {
            carCombo->addItem((*it).type,
                           QVariant((*it).selectionIndex));
        }
    }



}

//Check to see if Componenets Manager processed fine
bool  ComponentsManager::isGood()
{
    return processedFile;
}

ComponentsManager::ComponentDataLists ComponentsManager::returnDataList()
{
    return dataList;
}



//Open ComponentsStartingPopularity XML file.
bool ComponentsManager::parseAIComponentsPopularity(QString openFileName, QWidget *widget)
{

    //Open the ComponentsStartingPopularity.xml file
    QFile file(openFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(widget,"Could Not open file!!!!",
                              "We had a problem reading this file!");

             return false;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement BaseElement;
    QDomElement subElement;



    //Error in prasing XML file
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        QMessageBox::critical(widget,"Error opening file!!!!",
                              "This file is not valid xml!");
       return false;
    }

    //Begin Reading and putting the data into the list
    QDomElement rootNode = doc.firstChildElement("StartingPops");
    if(rootNode.isNull())
    {
        QMessageBox::critical(widget,"Error opening file!!!!",
                              "This file is a components xml for the game!");
        return false;
    }

    //
    //Get components and read data
    //
    rootNode = doc.documentElement();

    BaseElement = rootNode.firstChildElement("year");
    while(!BaseElement.isNull())
    {
        int year = BaseElement.attributeNode("y").value().toInt();

        subElement = BaseElement.firstChildElement("comp");
        while(!subElement.isNull())
        {
            aiPopData ai;
            ai.year = year;
            ai.fuel = subElement.attributeNode("aiPopFuel").value().toDouble();
            ai.performance = subElement.attributeNode("aiPopPerformance").value().toDouble();
            ai.luxury = subElement.attributeNode("aiPopLuxury").value().toDouble();
            ai.costs = subElement.attributeNode("aiPopCosts").value().toDouble();
            ai.ovrpop = subElement.attributeNode("popindex").value().toDouble();
            ai.id = subElement.attributeNode("selectionIndex").value().toInt();

             if(subElement.attributeNode("name").isNull())
             {
                 ai.name = subElement.attributeNode("selectionIndex").value();
             }
             else
             {
                  ai.name = subElement.attributeNode("name").value();
             }

             aiPops.insert(QString::number(year)+"-"+QString::number(ai.id),ai);
             subElement = subElement.nextSiblingElement("comp");
        }

        BaseElement = BaseElement.nextSiblingElement("year");
    }

        file.close();

        return true;
}


QMap<QString, ComponentsManager::aiPopData> ComponentsManager::returnPopsMap()
{
    return aiPops;
}


bool ComponentsManager::saveComponentsXMLFile(QString componentsFileName,
                                              ComponentsManager::ComponentDataLists saveData)
{

    QFile saveFile(componentsFileName);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
    {

       return false;
    }


    QXmlStreamWriter xmlWriter(&saveFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("Components");


    //Chassis/Frame
    xmlWriter.writeStartElement("ChassisComponents");
    for(QList<ChassisFrameComps>::Iterator it = saveData.frameList.begin();
        it != saveData.frameList.end(); ++it)
    {
          xmlWriter.writeStartElement("chassis");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","frame");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));
               xmlWriter.writeAttribute("str",QString::number((*it).str));
               xmlWriter.writeAttribute("performance",QString::number((*it).performance));
               xmlWriter.writeAttribute("safety",QString::number((*it).safety));
               xmlWriter.writeAttribute("durability",QString::number((*it).durability));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));
               xmlWriter.writeAttribute("cost",QString::number((*it).costs));
               xmlWriter.writeAttribute("skill",QString::number((*it).skill));
               xmlWriter.writeAttribute("popindex",QString::number((*it).popindex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //chassis


    }
    xmlWriter.writeEndElement(); //Chassis Components

    xmlWriter.writeStartElement("SuspensionComponents");
    for(QList<SuspensionComps>::Iterator it = saveData.suspensionList.begin();
        it != saveData.suspensionList.end(); ++it)
    {

        xmlWriter.writeStartElement("suspension");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","suspension");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));
               xmlWriter.writeAttribute("steering",QString::number((*it).steering));
               xmlWriter.writeAttribute("braking",QString::number((*it).braking));
               xmlWriter.writeAttribute("comfort",QString::number((*it).comfort));
               xmlWriter.writeAttribute("performance",QString::number((*it).performance));
               xmlWriter.writeAttribute("durability",QString::number((*it).durability));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("cost",QString::number((*it).costs));
               xmlWriter.writeAttribute("skill",QString::number((*it).skill));
               xmlWriter.writeAttribute("popindex",QString::number((*it).popindex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //suspension

    }
    xmlWriter.writeEndElement(); //SuspensionComponents


    xmlWriter.writeStartElement("DrivetrainComponents");
    for(QList<DriveTrainComps>::Iterator it = saveData.drivetrainList.begin();
        it != saveData.drivetrainList.end(); ++it)
    {


        xmlWriter.writeStartElement("drivetrain");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","drivetrain");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));

               xmlWriter.writeAttribute("ridePerformance",QString::number((*it).ridePerformance));
               xmlWriter.writeAttribute("rideSteering",QString::number((*it).rideSteering));
               xmlWriter.writeAttribute("duriblity",QString::number((*it).duriblity));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));
               xmlWriter.writeAttribute("carPerformance",QString::number((*it).carPerformance));
               xmlWriter.writeAttribute("engineWidth",QString::number((*it).engineWidth));
               xmlWriter.writeAttribute("engineLength",QString::number((*it).engineLength));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("cost",QString::number((*it).costs));
               xmlWriter.writeAttribute("skill",QString::number((*it).skill));
               xmlWriter.writeAttribute("popindex",QString::number((*it).popindex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //drivetrain


    }
    xmlWriter.writeEndElement(); //DrivetrainComponents


    xmlWriter.writeStartElement("EngineComponents");
    for(QList<EngineLayoutComps>::Iterator it = saveData.engineLayoutList.begin();
        it != saveData.engineLayoutList.end(); ++it)
    {


        xmlWriter.writeStartElement("layout");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","layout");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));

               xmlWriter.writeAttribute("valve",QString::number((*it).valve));
               xmlWriter.writeAttribute("cost",QString::number((*it).cost));
               xmlWriter.writeAttribute("designcost",QString::number((*it).designCosts));
               xmlWriter.writeAttribute("design",QString::number((*it).design));

               xmlWriter.writeAttribute("manu",QString::number((*it).manu));
               xmlWriter.writeAttribute("reliability",QString::number((*it).reliability));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));
               xmlWriter.writeAttribute("finish",QString::number((*it).finish));

               xmlWriter.writeAttribute("width",QString::number((*it).width));
               xmlWriter.writeAttribute("length",QString::number((*it).length));
               xmlWriter.writeAttribute("smooth",QString::number((*it).smooth));

               xmlWriter.writeAttribute("skill",QString::number((*it).skill));
               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               QString cyl = "";

               for(QStringList::Iterator sl = (*it).supportCyl.begin();
                   sl != (*it).supportCyl.end(); ++sl)
               {
                   cyl += (*sl)+"/";
               }

               if(!cyl.isEmpty())
                  cyl = cyl.remove(cyl.size()-1,1);


               QString fuel = "";

               for(QStringList::Iterator sl = (*it).supportFuel.begin();
                   sl != (*it).supportFuel.end(); ++sl)
               {
                   fuel += (*sl)+"/";
               }

               if(!fuel.isEmpty())
                 fuel = fuel.remove(fuel.size()-1,1);


               QString induct = "";

               for(QStringList::Iterator sl = (*it).supportInduct.begin();
                   sl != (*it).supportInduct.end(); ++sl)
               {
                   induct += (*sl)+"/";
               }

               if(!induct.isEmpty())
                induct = induct.remove(induct.size()-1,1);

               xmlWriter.writeAttribute("possiblecyl",cyl);
               xmlWriter.writeAttribute("possiblefuel",fuel);
               xmlWriter.writeAttribute("possibleinduction",induct);


               xmlWriter.writeAttribute("layoutfuel",QString::number((*it).layoutFuel));
               xmlWriter.writeAttribute("layoutpower",QString::number((*it).layoutpower));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("useDisplacement",QString::number((*it).useDisplacement));
               xmlWriter.writeAttribute("layoutCylinderLengthArrangment",
                                        QString::number((*it).cylinderLengthArrangment));

               xmlWriter.writeAttribute("turbine", QString::number((*it).turbine));

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //layout


    }


    for(QList<EngineCylinderComps>::Iterator it = saveData.engineCylinderList.begin();
        it != saveData.engineCylinderList.end(); ++it)
    {

        xmlWriter.writeStartElement("cylinder");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","cylinder");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));

               xmlWriter.writeAttribute("cylcost",QString::number((*it).cylCosts));
               xmlWriter.writeAttribute("designcost",QString::number((*it).designCosts));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));

               xmlWriter.writeAttribute("power",QString::number((*it).power));
               xmlWriter.writeAttribute("fuel",QString::number((*it).fuel));
               xmlWriter.writeAttribute("reliability",QString::number((*it).reliability));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));

               xmlWriter.writeAttribute("finish",QString::number((*it).finish));
               xmlWriter.writeAttribute("numbercyl",QString::number((*it).numberCyl));
               xmlWriter.writeAttribute("smooth",QString::number((*it).smooth));
               xmlWriter.writeAttribute("skill",QString::number((*it).skill));


               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //cylinder


    }


    for(QList<EngineFuelComps>::Iterator it = saveData.engineFuelList.begin();
        it != saveData.engineFuelList.end(); ++it)
    {
        xmlWriter.writeStartElement("fuel");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","fuel");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));

               xmlWriter.writeAttribute("cost",QString::number((*it).costs));
               xmlWriter.writeAttribute("designcost",QString::number((*it).designCost));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));

               xmlWriter.writeAttribute("power",QString::number((*it).power));
               xmlWriter.writeAttribute("fuel",QString::number((*it).fuel));
               xmlWriter.writeAttribute("reliability",QString::number((*it).reliability));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));

               xmlWriter.writeAttribute("finish",QString::number((*it).finish));
               xmlWriter.writeAttribute("rpm",QString::number((*it).rpm));
               xmlWriter.writeAttribute("smooth",QString::number((*it).smooth));
               xmlWriter.writeAttribute("skill",QString::number((*it).skill));


               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //fuel
    }


    for(QList<EngineValveComps>::Iterator it = saveData.engineValveList.begin();
        it != saveData.engineValveList.end(); ++it)
    {
        xmlWriter.writeStartElement("valve");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","valve");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));

               xmlWriter.writeAttribute("limit",QString::number((*it).group));

               xmlWriter.writeAttribute("cost",QString::number((*it).cost));
               xmlWriter.writeAttribute("designcost",QString::number((*it).designCost));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));

              xmlWriter.writeAttribute("fuel",QString::number((*it).fuel));
               xmlWriter.writeAttribute("reliability",QString::number((*it).reliability));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));

               xmlWriter.writeAttribute("finish",QString::number((*it).finish));
               xmlWriter.writeAttribute("rpm",QString::number((*it).rpm));
               xmlWriter.writeAttribute("smooth",QString::number((*it).smooth));
               xmlWriter.writeAttribute("size",QString::number((*it).size));
               xmlWriter.writeAttribute("power",QString::number((*it).power));


               xmlWriter.writeAttribute("skill",QString::number((*it).skill));


               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //valve


    }


    for(QList<EngineInductionComps>::Iterator it = saveData.engineInductionList.begin();
        it != saveData.engineInductionList.end(); ++it)
    {

        xmlWriter.writeStartElement("induction");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","induction");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));


               xmlWriter.writeAttribute("cost",QString::number((*it).costs));
               xmlWriter.writeAttribute("designcost",QString::number((*it).designCosts));
               xmlWriter.writeAttribute("design",QString::number((*it).design));
               xmlWriter.writeAttribute("manu",QString::number((*it).manu));

               xmlWriter.writeAttribute("power",QString::number((*it).power));
              xmlWriter.writeAttribute("fuel",QString::number((*it).fuel));
               xmlWriter.writeAttribute("reliability",QString::number((*it).reliability));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));

               xmlWriter.writeAttribute("finish",QString::number((*it).finish));

               xmlWriter.writeAttribute("skill",QString::number((*it).skill));


               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //induction


    }
    xmlWriter.writeEndElement(); //EngineComponents


    xmlWriter.writeStartElement("GearboxComponents");
    for(QList<GearBoxComps>::Iterator it = saveData.gearboxTypeList.begin();
        it != saveData.gearboxTypeList.end(); ++it)
    {

        xmlWriter.writeStartElement("gearbox");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","transmission");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));
               xmlWriter.writeAttribute("shift",(*it).shift);

               xmlWriter.writeAttribute("costs",QString::number((*it).costs));
               xmlWriter.writeAttribute("designcosts",QString::number((*it).designCosts));
               xmlWriter.writeAttribute("weight",QString::number((*it).weight));
               xmlWriter.writeAttribute("complex",QString::number((*it).complex));

               xmlWriter.writeAttribute("smooth",QString::number((*it).smooth));
              xmlWriter.writeAttribute("comfort",QString::number((*it).comfort));
               xmlWriter.writeAttribute("fuel",QString::number((*it).fuel));
               xmlWriter.writeAttribute("performance",QString::number((*it).performance));

               xmlWriter.writeAttribute("skill",QString::number((*it).skill));

               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               QString gears = "";

               for(QStringList::Iterator sl = (*it).possiblegearsList.begin();
                   sl != (*it).possiblegearsList.end(); ++sl)
               {
                   gears += (*sl)+"/";
               }

               if(!gears.isEmpty())
                  gears = gears.remove(gears.size()-1,1);

               QString addons = "";

               if((*it).trans)
                   addons += "13156/";
               if((*it).reverse)
                   addons += "13157/";
               if((*it).overdrive)
                   addons += "13158/";
               if((*it).limited)
                   addons += "13159/";

               if(!addons.isEmpty())
                  addons = addons.remove(addons.size()-1,1);

               xmlWriter.writeAttribute("possiblegears",gears);
               xmlWriter.writeAttribute("possibleaddons",addons);

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //gearbox


    }

    for(QList<GearGearsComps>::Iterator it = saveData.gearboxGearsList.begin();
        it != saveData.gearboxGearsList.end(); ++it)
    {


        xmlWriter.writeStartElement("gears");
               xmlWriter.writeAttribute("name",(*it).name);
               xmlWriter.writeAttribute("type","gears");
               xmlWriter.writeAttribute("year",QString::number((*it).year));
               xmlWriter.writeAttribute("death",QString::number((*it).death));

               xmlWriter.writeAttribute("gears",QString::number((*it).gears));
               xmlWriter.writeAttribute("skill",QString::number((*it).skill));


               xmlWriter.writeAttribute("popindex",QString::number((*it).popIndex));
               xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

               xmlWriter.writeAttribute("picture",(*it).picture);
               xmlWriter.writeAttribute("about",(*it).about);

               xmlWriter.writeAttribute("localName",QString::number((*it).localName));
               xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
               xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //gears
    }


    xmlWriter.writeStartElement("addons");
        xmlWriter.writeAttribute("name","13156");
        xmlWriter.writeAttribute("type","addon");
        xmlWriter.writeAttribute("year","1890");
        xmlWriter.writeAttribute("death","2222");

        xmlWriter.writeAttribute("popindex","0.2");

        xmlWriter.writeAttribute("selectionIndex","-1");

        xmlWriter.writeAttribute("picture","TransaxleGearAddon.dds");
        xmlWriter.writeAttribute("about","13165");

        xmlWriter.writeAttribute("localName","1");
        xmlWriter.writeAttribute("localDesc","1");
        xmlWriter.writeAttribute("localParam","0");
    xmlWriter.writeEndElement(); //addons

    xmlWriter.writeStartElement("addons");
        xmlWriter.writeAttribute("name","13157");
        xmlWriter.writeAttribute("type","addon");
        xmlWriter.writeAttribute("year","1890");
        xmlWriter.writeAttribute("death","2222");

        xmlWriter.writeAttribute("popindex","0.5");

        xmlWriter.writeAttribute("selectionIndex","-2");

        xmlWriter.writeAttribute("picture","TransversedGearAddon.dds");
        xmlWriter.writeAttribute("about","13166");

        xmlWriter.writeAttribute("localName","1");
        xmlWriter.writeAttribute("localDesc","1");
        xmlWriter.writeAttribute("localParam","0");
    xmlWriter.writeEndElement(); //addons


    xmlWriter.writeStartElement("addons");
        xmlWriter.writeAttribute("name","13158");
        xmlWriter.writeAttribute("type","addon");
        xmlWriter.writeAttribute("year","1932");
        xmlWriter.writeAttribute("death","2222");

        xmlWriter.writeAttribute("popindex","0.85");

        xmlWriter.writeAttribute("selectionIndex","-3");

        xmlWriter.writeAttribute("picture","OverdriveGearAddon.dds");
        xmlWriter.writeAttribute("about","13167");

        xmlWriter.writeAttribute("localName","1");
        xmlWriter.writeAttribute("localDesc","1");
        xmlWriter.writeAttribute("localParam","0");
    xmlWriter.writeEndElement(); //addons

    xmlWriter.writeStartElement("addons");
        xmlWriter.writeAttribute("name","13159");
        xmlWriter.writeAttribute("type","addon");
        xmlWriter.writeAttribute("year","1955");
        xmlWriter.writeAttribute("death","2222");

        xmlWriter.writeAttribute("popindex","0.25");

        xmlWriter.writeAttribute("selectionIndex","-4");

        xmlWriter.writeAttribute("picture","LimitedSlipGearAddon.dds");
        xmlWriter.writeAttribute("about","13168");

        xmlWriter.writeAttribute("localName","1");
        xmlWriter.writeAttribute("localDesc","1");
        xmlWriter.writeAttribute("localParam","0");
    xmlWriter.writeEndElement(); //addons


    xmlWriter.writeEndElement(); //GearboxComponents




    xmlWriter.writeStartElement("CarModels");
    for(QList<CarModels>::Iterator it = saveData.carModelsList.begin();
        it != saveData.carModelsList.end(); ++it)
    {

        xmlWriter.writeStartElement("car");
           xmlWriter.writeAttribute("name",(*it).name);
           xmlWriter.writeAttribute("type",(*it).typeName);
           xmlWriter.writeAttribute("model",(*it).model);

           xmlWriter.writeAttribute("year",QString::number((*it).year));
           xmlWriter.writeAttribute("death",QString::number((*it).death));


           xmlWriter.writeAttribute("designreq",QString::number((*it).designReq));
           xmlWriter.writeAttribute("pass",QString::number((*it).pass));


           xmlWriter.writeAttribute("localName",QString::number((*it).localName));
           xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
           xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

         xmlWriter.writeEndElement(); //car
    }
    xmlWriter.writeEndElement(); //CarModels


    xmlWriter.writeStartElement("AccessoriesModels");
    for(QList<AccessoriesModels>::Iterator it = saveData.accessoriesList.begin();
        it != saveData.accessoriesList.end(); ++it)
    {
        xmlWriter.writeStartElement("access");
           xmlWriter.writeAttribute("name",(*it).name);
           xmlWriter.writeAttribute("type",(*it).typeName);
           xmlWriter.writeAttribute("model",(*it).model);

           xmlWriter.writeAttribute("year",QString::number((*it).year));
           xmlWriter.writeAttribute("death",QString::number((*it).death));
           xmlWriter.writeAttribute("image",(*it).picture);

           if((*it).decal)
           {
               xmlWriter.writeAttribute("width",QString::number((*it).width));
               xmlWriter.writeAttribute("height",QString::number((*it).height));
           }


           xmlWriter.writeAttribute("localName",QString::number((*it).localName));
           xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
           xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

           xmlWriter.writeAttribute("autoMirror",QString::number((*it).autoMirror));
           xmlWriter.writeAttribute("autoPaint",QString::number((*it).autoPaint));

         xmlWriter.writeEndElement(); //access
    }
    xmlWriter.writeEndElement(); //AccessoriesModels


    xmlWriter.writeStartElement("NewCarType");
    for(QList<CarTypes>::Iterator it = saveData.cartypesList.begin();
        it != saveData.cartypesList.end(); ++it)
    {
        xmlWriter.writeStartElement("car");
           xmlWriter.writeAttribute("type",(*it).type);

            xmlWriter.writeAttribute("Rating_Performance",QString::number((*it).rPerformance));
            xmlWriter.writeAttribute("Rating_Drivability",QString::number((*it).rDrivability));
            xmlWriter.writeAttribute("Rating_Luxury",QString::number((*it).rLuxury));
            xmlWriter.writeAttribute("Rating_Safety",QString::number((*it).rSafety));
            xmlWriter.writeAttribute("Rating_Fuel",QString::number((*it).rFuel));
            xmlWriter.writeAttribute("Rating_Power",QString::number((*it).rPower));
            xmlWriter.writeAttribute("Rating_Cargo",QString::number((*it).rCargo));
            xmlWriter.writeAttribute("Rating_Dependability",QString::number((*it).rDependability));
            xmlWriter.writeAttribute("popNA",QString::number((*it).popNA));
            xmlWriter.writeAttribute("popEU",QString::number((*it).popEU));

            xmlWriter.writeAttribute("popSA",QString::number((*it).popSA));
            xmlWriter.writeAttribute("popAF",QString::number((*it).popAF));
            xmlWriter.writeAttribute("popAS",QString::number((*it).popAS));
            xmlWriter.writeAttribute("popAU",QString::number((*it).popAU));
            xmlWriter.writeAttribute("pop",QString::number((*it).pop));

            xmlWriter.writeAttribute("aval","True");
            xmlWriter.writeAttribute("wealth",QString::number((*it).wealth));
            xmlWriter.writeAttribute("age",QString::number((*it).age));

           xmlWriter.writeAttribute("about",(*it).about);
           xmlWriter.writeAttribute("picture",(*it).picture);

           xmlWriter.writeAttribute("cargo",QString::number((*it).cargo));
           xmlWriter.writeAttribute("pass",QString::number((*it).pass));
           xmlWriter.writeAttribute("year",QString::number((*it).year));
           xmlWriter.writeAttribute("death",QString::number((*it).death));


           xmlWriter.writeAttribute("localName",QString::number((*it).localName));
           xmlWriter.writeAttribute("localDesc",QString::number((*it).localDescription));
           xmlWriter.writeAttribute("localParam",QString::number((*it).localPara));

           xmlWriter.writeAttribute("milFleet",QString::number((*it).milFleet));
           xmlWriter.writeAttribute("civFleet",QString::number((*it).civFleet));
           xmlWriter.writeAttribute("civ",QString::number((*it).civ));
           xmlWriter.writeAttribute("selectionIndex",QString::number((*it).selectionIndex));

         xmlWriter.writeEndElement(); //car
    }
    xmlWriter.writeEndElement(); //NewCarType


    //close the final tag and close the file.
     xmlWriter.writeEndDocument();

     saveFile.close();

}


bool ComponentsManager::saveComponentsPopXMLFile(QString popFileName,
                                              QMap<QString, ComponentsManager::aiPopData> savePop)
{
    QFile saveFile(popFileName);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
    {

       return false;
    }


    QXmlStreamWriter xmlWriter(&saveFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("StartingPops");
    QList<int> years;

    for(QMap<QString, ComponentsManager::aiPopData>::Iterator it = savePop.begin();
        it != savePop.end(); ++it)
    {
        if(!years.contains((*it).year))
        {
            years.push_back((*it).year);
        }
    }

    for(QList<int>::Iterator it = years.begin(); it != years.end(); ++it)
    {
        xmlWriter.writeStartElement("year");
             xmlWriter.writeAttribute("y",QString::number((*it)));

         for(QMap<QString, ComponentsManager::aiPopData>::Iterator sp = savePop.begin();
             sp != savePop.end(); ++sp)
         {
             if((*sp).year == (*it))
             {
                 xmlWriter.writeStartElement("comp");


                     xmlWriter.writeAttribute("aiPopFuel",QString::number((*sp).fuel));
                     xmlWriter.writeAttribute("aiPopPerformance",QString::number((*sp).performance));
                     xmlWriter.writeAttribute("aiPopLuxury",QString::number((*sp).luxury));
                     xmlWriter.writeAttribute("aiPopCosts",QString::number((*sp).costs));
                     xmlWriter.writeAttribute("popindex",QString::number((*sp).ovrpop));
                     xmlWriter.writeAttribute("selectionIndex",QString::number((*sp).id));
                     xmlWriter.writeAttribute("name",(*sp).name);




                  xmlWriter.writeEndElement(); //comp
             }
         }

         xmlWriter.writeEndElement(); //year
    }

    xmlWriter.writeEndDocument();

    saveFile.close();

}
