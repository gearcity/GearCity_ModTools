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

//Open Components XML file.
ComponentsManager::ComponentsManager(QString openFileName, QWidget *widget)
{
    processedFile = true;

    //Open the Components.xml file
    QFile file(openFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
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
        processedFile = false;
       return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("Components");
    if(rootNode.isNull())
    {
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
            chassisSuspensionRead(subElement);
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
    BaseElement = rootNode.firstChildElement("RacingSeries");
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
    }





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
        data.about = element.attributeNode("about").value();
        data.localName = element.attributeNode("localName").value().toInt();
        data.localDescription = element.attributeNode("localDesc").value().toInt();
        data.localPara = element.attributeNode("localParam").value().toInt();

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
        data.limit = element.attributeNode("limit").value().toInt();
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

    dataList.accessoriesList.push_back(data);
}


//Read- NewCarType  car
void ComponentsManager::carTypeRead(QDomElement element)
{
    CarTypes data;
        data.type = element.attributeNode("type").value();
        data.rPerformance = element.attributeNode("Rating_Performance").value().toDouble();
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
void ComponentsManager::racingSeriesRead(QDomElement element)
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
}

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
