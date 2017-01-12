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


#include "AIManager.h"
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QMessageBox>


//AI Manager loads AI companies from XML file and puts them into the AI Map.
AIManager::AIManager(QString fileName, QWidget *widget)
{
    AIManager::AIInfo data;

    //Open File
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::critical(widget,"File Error",
                             QString("Whopse, opening %1 failed.").arg(fileName));
       return;
    }

    //Process XML File.
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement aiElement;
    QDomElement aiChildElements;

    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
       QMessageBox::critical(widget,"Error Base Video",
                            QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                            errorMsg,errorLine,errorColumn));
        return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("AINode");
    if(rootNode.isNull())
    {
        QMessageBox::critical(widget,"Error","AI Script is malformed.");
        return;
    }

    //Get first city, and lets go
    rootNode = doc.documentElement();
    aiElement = rootNode.firstChildElement("Company");

    if(aiElement.isNull())
        return;

    do
    {
      //Company Attributes
        data.id = aiElement.attributeNode("ID").value().toInt();
        data.name = aiElement.attributeNode("Name").value();
        data.ownerID = aiElement.attributeNode("OwnerID").value().toInt();
        data.hq = aiElement.attributeNode("HQ").value().toInt();
        data.startYear = aiElement.attributeNode("Founded").value().toInt();
        data.deathYear = aiElement.attributeNode("Death").value().toInt();
        data.logo = aiElement.attributeNode("Logo").value();

        aiChildElements = aiElement.firstChildElement("Funds");
      if(!aiChildElements.isNull())
      {
        data.funds = aiChildElements.attributeNode("OnHand").value().toLongLong();
        data.credit = aiChildElements.attributeNode("Credit").value().toInt();
        data.loan = aiChildElements.attributeNode("Loan").value().toLongLong();
      }

      aiChildElements = aiElement.firstChildElement("Skills");
      if(!aiChildElements.isNull())
      {
        data.manuSkills = aiChildElements.attributeNode("Manufactoring").value().toInt();
        data.rndSkills = aiChildElements.attributeNode("RnD").value().toInt();
        data.adminSkills = aiChildElements.attributeNode("Admin").value().toInt();
        data.marketingSkills = aiChildElements.attributeNode("Marketing").value().toInt();
        data.dealerSkills = aiChildElements.attributeNode("Dealers").value().toInt();
      }

      aiChildElements = aiElement.firstChildElement("Design");
      if(!aiChildElements.isNull())
      {
        data.engineDesign = aiChildElements.attributeNode("Engine").value().toInt();
        data.chassisDesign = aiChildElements.attributeNode("Chassis").value().toInt();
        data.gearboxDesign = aiChildElements.attributeNode("Transmission").value().toInt();
        data.bodyDesign= aiChildElements.attributeNode("Body").value().toInt();
        data.luxDesign = aiChildElements.attributeNode("Lux").value().toInt();
        data.safeDesign = aiChildElements.attributeNode("safety").value().toInt();
      }


      aiChildElements = aiElement.firstChildElement("Image");
      if(!aiChildElements.isNull())
      {
        data.globalImage = aiChildElements.attributeNode("GeneralGlobal").value().toInt();
        data.qualityImage = aiChildElements.attributeNode("Quality").value().toInt();
        data.racingImage = aiChildElements.attributeNode("Racing").value().toInt();
        data.workImage = aiChildElements.attributeNode("Work").value().toInt();
      }


      aiChildElements = aiElement.firstChildElement("Behavior");
      if(!aiChildElements.isNull())
      {
        data.genericDesigner = aiChildElements.attributeNode("GenericDesigner").value().toDouble();
        data.performanceBehavoir = aiChildElements.attributeNode("Rating_Performance").value().toDouble();
        data.drivingBehavoir = aiChildElements.attributeNode("Rating_Drivability").value().toDouble();
        data.luxuryBehavoir = aiChildElements.attributeNode("Rating_Luxury").value().toDouble();
        data.safetyBehavoir = aiChildElements.attributeNode("Rating_Safety").value().toDouble();
        data.fuelBehavoir = aiChildElements.attributeNode("Rating_Fuel").value().toDouble();

        data.powerBehavoir = aiChildElements.attributeNode("Rating_Power").value().toDouble();
        data.cargoBehavoir = aiChildElements.attributeNode("Rating_Cargo").value().toDouble();
        data.dependabilityBehavoir = aiChildElements.attributeNode("Rating_Dependability").value().toDouble();
        data.designAgression = aiChildElements.attributeNode("DesignAggression").value().toDouble();
        data.sellAgression = aiChildElements.attributeNode("SellAggression").value().toDouble();
        data.buildAgression = aiChildElements.attributeNode("BuildAggression").value().toDouble();
        data.marketingAgression = aiChildElements.attributeNode("MarketingAggression").value().toDouble();

        data.costAgression = aiChildElements.attributeNode("CostAggression").value().toDouble();
        data.qualityAgression= aiChildElements.attributeNode("QualityAggression").value().toDouble();
        data.priceAgression= aiChildElements.attributeNode("PriceAggression").value().toDouble();
        data.expansionAgression = aiChildElements.attributeNode("ExpansionAggression").value().toDouble();
        data.clusting = aiChildElements.attributeNode("ClusterSpace").value().toDouble();
        data.exportDesigns = aiChildElements.attributeNode("ExportDesigns").value().toDouble();
        data.importDesigns = aiChildElements.attributeNode("ImportDesigns").value().toDouble();
      }

      aiMap.insert(data.id,data);

      aiElement = aiElement.nextSiblingElement("Company");
    }
    while(!aiElement.isNull());

    file.close();

}

//Return created AIMap to caller
QMap<int, AIManager::AIInfo> AIManager::getMap()
{
    return aiMap;
}
