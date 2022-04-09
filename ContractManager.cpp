#include "ContractManager.h"
#include <QFile>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QDomNode>

#include <QtWidgets/QMessageBox>

ContractManager::ContractManager()
{

}

bool ContractManager::openContractsXML(QString fileName, QString& errorHeader, QString& errorBody)
{
    bool allGood = true;
     errorHeader = "";
     errorBody = "";

    vehicleContract vc;
    engineContract ec;
    chassisContract cc;
    gearboxContract gc;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        errorHeader = "Could Not open file!!!!";
        errorBody = "We had a problem reading this file!";

        allGood = false;
             return allGood;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement BaseElement;
    QDomElement subElement;



    //Error in prasing XML file
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        errorHeader = "Could Not open file!!!!";
        errorBody = "This file is not valid xml!";

       allGood = false;
       return allGood;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("ContractingEngines");
    if(rootNode.isNull())
    {
        errorHeader = "Error opening file!!!";
        errorBody = "This file is a components xml for the game!";

        allGood = false;
        return allGood;
    }

    //
    //Get engine contracts and read data
    //

    BaseElement = rootNode.firstChildElement("engine");
    while(!BaseElement.isNull())
    {
        if(!BaseElement.attributeNode("localName").isNull())
          ec.localizeName = BaseElement.attributeNode("localName").value().toInt();
        else
          ec.localizeName = 1;

        if(!BaseElement.attributeNode("localFuel").isNull())
          ec.localizeFuel = BaseElement.attributeNode("localFuel").value().toInt();
        else
          ec.localizeFuel = 1;

        if(!BaseElement.attributeNode("localLayout").isNull())
          ec.localizeLayout = BaseElement.attributeNode("localLayout").value().toInt();
        else
          ec.localizeLayout = 1;

        if(!BaseElement.attributeNode("localCyl").isNull())
           ec.localizeCylinder = BaseElement.attributeNode("localCyl").value().toInt();
        else
           ec.localizeCylinder = 1;

      if(ec.localizeName == 1)
      {
          ec.name = BaseElement.attributeNode("for").value();
      }
      else
      {
          ec.name = BaseElement.attributeNode("for").value();
      }

       ec.type = BaseElement.attributeNode("type").value().toInt();
       ec.military = BaseElement.attributeNode("military").value().toInt();
       ec.start = BaseElement.attributeNode("birth").value().toInt();
       ec.end = BaseElement.attributeNode("death").value().toInt();
       ec.ccMod = BaseElement.attributeNode("ccModifier").value().toDouble();
       ec.weightMod = BaseElement.attributeNode("weightModifier").value().toDouble();
       ec.fuelMod = BaseElement.attributeNode("fuelModifier").value().toDouble();
       ec.hpMod = BaseElement.attributeNode("hpModifier").value().toDouble();
       ec.widthMod = BaseElement.attributeNode("widthModifier").value().toDouble();
       ec.lengthMod = BaseElement.attributeNode("lengthModifier").value().toDouble();
       ec.torqueMod = BaseElement.attributeNode("minTorqueModifier").value().toDouble();



       if(!BaseElement.attributeNode("unitNumbers").isNull())
         ec.contractSize = BaseElement.attributeNode("unitNumbers").value().toDouble();
       else
         ec.contractSize = 1.0;

       if(!BaseElement.attributeNode("unitPrice").isNull())
         ec.contractPrice = BaseElement.attributeNode("unitPrice").value().toDouble();
       else
         ec.contractPrice = 1.0;



      ec.fuelLimit = BaseElement.attributeNode("fuelLimit").value().split("/");
      ec.layoutLimit = BaseElement.attributeNode("layoutLimit").value().split("/");
      ec.cylinderLimit = BaseElement.attributeNode("cylinderLimit").value().split("/");


       ecList.push_back(ec);

      BaseElement = BaseElement.nextSiblingElement("engine");
    }



    BaseElement = rootNode.firstChildElement("car");
    while(!BaseElement.isNull())
    {
        if(!BaseElement.attributeNode("localName").isNull())
          vc.localizeName = BaseElement.attributeNode("localName").value().toInt();
        else
           vc.localizeName = 1;

        if(!BaseElement.attributeNode("localFuel").isNull())
          vc.localizeFuel = BaseElement.attributeNode("localFuel").value().toInt();
        else
          vc.localizeFuel = 1;

        if(!BaseElement.attributeNode("localizeType").isNull())
          vc.localizeType = BaseElement.attributeNode("localizeType").value().toInt();
        else
          vc.localizeType = 1;


      if(vc.localizeName == 1)
      {
          vc.name = BaseElement.attributeNode("for").value();
      }
      else
      {
          vc.name = BaseElement.attributeNode("for").value();
      }

       vc.type = BaseElement.attributeNode("type").value().toInt();
       vc.military = BaseElement.attributeNode("military").value().toInt();
       vc.start = BaseElement.attributeNode("birth").value().toInt();
       vc.end = BaseElement.attributeNode("death").value().toInt();
       vc.ccMod = BaseElement.attributeNode("ccModifier").value().toDouble();
       vc.weightMod = BaseElement.attributeNode("weightModifier").value().toDouble();
       vc.fuelMod = BaseElement.attributeNode("fuelModifier").value().toDouble();
       vc.hpMod = BaseElement.attributeNode("hpModifier").value().toDouble();
       vc.cargoMod = BaseElement.attributeNode("cargoSpecModifier").value().toDouble();
       vc.latgMod = BaseElement.attributeNode("latgModifier").value().toDouble();
       vc.torqueMod = BaseElement.attributeNode("minTorqueModifier").value().toDouble();
       vc.topSpeedMod = BaseElement.attributeNode("topSpeedModifier").value().toDouble();


       if(!BaseElement.attributeNode("unitNumbers").isNull())
         vc.contractSize = BaseElement.attributeNode("unitNumbers").value().toDouble();
       else
         vc.contractSize = 1.0;

       if(!BaseElement.attributeNode("unitPrice").isNull())
         vc.contractPrice = BaseElement.attributeNode("unitPrice").value().toDouble();
       else
         vc.contractPrice = 1.0;


       vc.fuelLimit = BaseElement.attributeNode("fuelLimit").value().split("/");
       vc.vehicleTypeLimit = BaseElement.attributeNode("vehicleTypeLimit").value().split("/");

       vcList.push_back(vc);

         BaseElement = BaseElement.nextSiblingElement("car");
    }


    BaseElement = rootNode.firstChildElement("chassis");
    while(!BaseElement.isNull())
    {
        if(!BaseElement.attributeNode("localName").isNull())
          cc.localizeName = BaseElement.attributeNode("localName").value().toInt();
        else
           cc.localizeName = 1;




          if(cc.localizeName == 1)
          {
              cc.name = BaseElement.attributeNode("for").value();
          }
          else
          {
              cc.name = BaseElement.attributeNode("for").value();
          }

           cc.type = BaseElement.attributeNode("type").value().toInt();
           cc.military = BaseElement.attributeNode("military").value().toInt();
           cc.start = BaseElement.attributeNode("birth").value().toInt();
           cc.end = BaseElement.attributeNode("death").value().toInt();
           cc.weightMod = BaseElement.attributeNode("weightModifier").value().toDouble();
           cc.minWidth = BaseElement.attributeNode("minWidthModifier").value().toDouble();
           cc.maxWidth = BaseElement.attributeNode("maxWidthModifier").value().toDouble();
           cc.minLength = BaseElement.attributeNode("minLengthModifier").value().toDouble();
           cc.maxLength = BaseElement.attributeNode("maxLengthModifier").value().toDouble();

           if(!BaseElement.attributeNode("unitNumbers").isNull())
             cc.contractSize = BaseElement.attributeNode("unitNumbers").value().toDouble();
           else
             cc.contractSize = 1.0;

           if(!BaseElement.attributeNode("unitPrice").isNull())
             cc.contractPrice = BaseElement.attributeNode("unitPrice").value().toDouble();
           else
             cc.contractPrice = 1.0;

            ccList.push_back(cc);

            BaseElement = BaseElement.nextSiblingElement("chassis");
    }


    BaseElement = rootNode.firstChildElement("gearbox");
    while(!BaseElement.isNull())
    {
        if(!BaseElement.attributeNode("localName").isNull())
          gc.localizeName = BaseElement.attributeNode("localName").value().toInt();
        else
           gc.localizeName = 1;

        if(!BaseElement.attributeNode("localizeGear").isNull())
          gc.localizeGear = BaseElement.attributeNode("localizeGear").value().toInt();
        else
           gc.localizeGear = 1;

        if(!BaseElement.attributeNode("localizeType").isNull())
          gc.localizeType = BaseElement.attributeNode("localizeType").value().toInt();
        else
           gc.localizeType = 1;

          if(gc.localizeName == 1)
          {
              gc.name = BaseElement.attributeNode("for").value();
          }
          else
          {
              gc.name = BaseElement.attributeNode("for").value();
          }

           gc.type = BaseElement.attributeNode("type").value().toInt();
           gc.military = BaseElement.attributeNode("military").value().toInt();
           gc.start = BaseElement.attributeNode("birth").value().toInt();
           gc.end = BaseElement.attributeNode("death").value().toInt();
           gc.weightMod = BaseElement.attributeNode("weightModifier").value().toDouble();

           if(!BaseElement.attributeNode("unitNumbers").isNull())
             gc.contractSize = BaseElement.attributeNode("unitNumbers").value().toDouble();
           else
             gc.contractSize = 1.0;

           if(!BaseElement.attributeNode("unitPrice").isNull())
             gc.contractPrice = BaseElement.attributeNode("unitPrice").value().toDouble();
           else
             gc.contractPrice = 1.0;


           gc.gearLimit = BaseElement.attributeNode("gearLimit").value().split("/");
           gc.typeLimit = BaseElement.attributeNode("typeLimit").value().split("/");


            gcList.push_back(gc);

            BaseElement = BaseElement.nextSiblingElement("gearbox");
    }



    file.close();

    return allGood;
}

bool ContractManager::saveContractsXML(QString fileName, QList<vehicleContract> svcList,
QList<engineContract> secList, QList<chassisContract> sccList, QList<gearboxContract> sgcList)
{
    bool allGood = true;

    QFile saveFile(fileName);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
    {
       return false;
    }


    QXmlStreamWriter xmlWriter(&saveFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("ContractingEngines");


    //Cars
    for(QList<vehicleContract>::Iterator it = svcList.begin(); it != svcList.end(); ++it)
    {
          xmlWriter.writeStartElement("car");
               xmlWriter.writeAttribute("for",(*it).name);
               xmlWriter.writeAttribute("type",QString::number((*it).type));
               xmlWriter.writeAttribute("military",QString::number((*it).military));
               xmlWriter.writeAttribute("birth",QString::number((*it).start));
               xmlWriter.writeAttribute("death",QString::number((*it).end));
               xmlWriter.writeAttribute("ccModifier",QString::number((*it).ccMod));
               xmlWriter.writeAttribute("weightModifier",QString::number((*it).weightMod));
               xmlWriter.writeAttribute("fuelModifier",QString::number((*it).fuelMod));
               xmlWriter.writeAttribute("hpModifier",QString::number((*it).hpMod));
               xmlWriter.writeAttribute("cargoSpecModifier",QString::number((*it).cargoMod));
               xmlWriter.writeAttribute("latgModifier",QString::number((*it).latgMod));
               xmlWriter.writeAttribute("minTorqueModifier",QString::number((*it).torqueMod));
               xmlWriter.writeAttribute("topSpeedModifier",QString::number((*it).topSpeedMod));

               QString fuelLimits = "";

               for(QStringList::Iterator fl = (*it).fuelLimit.begin();
                   fl != (*it).fuelLimit.end(); ++fl)
               {
                   fuelLimits += (*fl)+"/";
               }

               if(!fuelLimits.isEmpty())
                  fuelLimits = fuelLimits.remove(fuelLimits.size()-1,1);
               else
                  fuelLimits = -2;

               xmlWriter.writeAttribute("fuelLimit",fuelLimits);

               QString typeLimits = "";

               for(QStringList::Iterator tl = (*it).vehicleTypeLimit.begin();
                   tl != (*it).vehicleTypeLimit.end(); ++tl)
               {
                   typeLimits += (*tl)+"/";
               }

               if(!typeLimits.isEmpty())
                  typeLimits = typeLimits.remove(typeLimits.size()-1,1);
               else
                  typeLimits = -1;

               xmlWriter.writeAttribute("vehicleTypeLimit",typeLimits);

               xmlWriter.writeAttribute("localName",QString::number((*it).localizeName));
               xmlWriter.writeAttribute("localizeFuel",QString::number((*it).localizeFuel));
               xmlWriter.writeAttribute("localizeType",QString::number((*it).localizeType));
               xmlWriter.writeAttribute("unitNumbers",QString::number((*it).contractSize));

               xmlWriter.writeAttribute("unitPrice",QString::number((*it).contractPrice));

         xmlWriter.writeEndElement(); //car


    }




    for(QList<engineContract>::Iterator it = ecList.begin();
        it != ecList.end(); ++it)
    {

        xmlWriter.writeStartElement("engine");
               xmlWriter.writeAttribute("for",(*it).name);
               xmlWriter.writeAttribute("type",QString::number((*it).type));
               xmlWriter.writeAttribute("military",QString::number((*it).military));
               xmlWriter.writeAttribute("birth",QString::number((*it).start));
               xmlWriter.writeAttribute("death",QString::number((*it).end));
               xmlWriter.writeAttribute("ccModifier",QString::number((*it).ccMod));
               xmlWriter.writeAttribute("weightModifier",QString::number((*it).weightMod));
               xmlWriter.writeAttribute("fuelModifier",QString::number((*it).fuelMod));
               xmlWriter.writeAttribute("hpModifier",QString::number((*it).hpMod));
               xmlWriter.writeAttribute("widthModifier",QString::number((*it).widthMod));
               xmlWriter.writeAttribute("lengthModifier",QString::number((*it).lengthMod));
               xmlWriter.writeAttribute("minTorqueModifier",QString::number((*it).torqueMod));

               QString fuelLimits = "";

               for(QStringList::Iterator fl = (*it).fuelLimit.begin();
                   fl != (*it).fuelLimit.end(); ++fl)
               {
                   fuelLimits += (*fl)+"/";
               }

               if(!fuelLimits.isEmpty())
                  fuelLimits = fuelLimits.remove(fuelLimits.size()-1,1);
               else
                  fuelLimits = -1;

               xmlWriter.writeAttribute("fuelLimit",fuelLimits);



               QString layoutLimits = "";

               for(QStringList::Iterator ll = (*it).layoutLimit.begin();
                   ll != (*it).layoutLimit.end(); ++ll)
               {
                   layoutLimits += (*ll)+"/";
               }

               if(!layoutLimits.isEmpty())
                  layoutLimits = layoutLimits.remove(layoutLimits.size()-1,1);
               else
                  layoutLimits = -1;

               xmlWriter.writeAttribute("layoutLimit",layoutLimits);


               QString clyLimits = "";

               for(QStringList::Iterator cl = (*it).cylinderLimit.begin();
                   cl != (*it).cylinderLimit.end(); ++cl)
               {
                   clyLimits += (*cl)+"/";
               }

               if(!clyLimits.isEmpty())
                  clyLimits = clyLimits.remove(clyLimits.size()-1,1);
               else
                  clyLimits = -1;

               xmlWriter.writeAttribute("cylinderLimit",clyLimits);



               xmlWriter.writeAttribute("localName",QString::number((*it).localizeName));
               xmlWriter.writeAttribute("localizeFuel",QString::number((*it).localizeFuel));
               xmlWriter.writeAttribute("localizeLayout",QString::number((*it).localizeLayout));
               xmlWriter.writeAttribute("localizeCylinder",QString::number((*it).localizeCylinder));

               xmlWriter.writeAttribute("unitNumbers",QString::number((*it).contractSize));
               xmlWriter.writeAttribute("unitPrice",QString::number((*it).contractPrice));


         xmlWriter.writeEndElement(); //engine

    }



    for(QList<chassisContract>::Iterator it = ccList.begin();
        it != ccList.end(); ++it)
    {


        xmlWriter.writeStartElement("chassis");
               xmlWriter.writeAttribute("for",(*it).name);
               xmlWriter.writeAttribute("type",QString::number((*it).type));
               xmlWriter.writeAttribute("military",QString::number((*it).military));
               xmlWriter.writeAttribute("birth",QString::number((*it).start));
               xmlWriter.writeAttribute("death",QString::number((*it).end));

               xmlWriter.writeAttribute("weightModifier",QString::number((*it).weightMod));
               xmlWriter.writeAttribute("minLengthModifier",QString::number((*it).minLength));
               xmlWriter.writeAttribute("maxLengthModifier",QString::number((*it).maxLength));
               xmlWriter.writeAttribute("minWidthModifier",QString::number((*it).minWidth));
               xmlWriter.writeAttribute("maxWidthModifier",QString::number((*it).maxWidth));

               xmlWriter.writeAttribute("localName",QString::number((*it).localizeName));

               xmlWriter.writeAttribute("unitNumbers",QString::number((*it).contractSize));
               xmlWriter.writeAttribute("unitPrice",QString::number((*it).contractPrice));



         xmlWriter.writeEndElement(); //chassis


    }


    for(QList<gearboxContract>::Iterator it = gcList.begin();
        it != gcList.end(); ++it)
    {


        xmlWriter.writeStartElement("gearbox");
               xmlWriter.writeAttribute("for",(*it).name);
               xmlWriter.writeAttribute("type",QString::number((*it).type));
               xmlWriter.writeAttribute("military",QString::number((*it).military));
               xmlWriter.writeAttribute("birth",QString::number((*it).start));
               xmlWriter.writeAttribute("death",QString::number((*it).end));

               xmlWriter.writeAttribute("weightModifier",QString::number((*it).weightMod));

               QString typeLimit = "";

               for(QStringList::Iterator tl = (*it).typeLimit.begin();
                   tl != (*it).typeLimit.end(); ++tl)
               {
                   typeLimit += (*tl)+"/";
               }

               if(!typeLimit.isEmpty())
                  typeLimit = typeLimit.remove(typeLimit.size()-1,1);


               QString gearLimit = "";

               for(QStringList::Iterator gl = (*it).gearLimit.begin();
                   gl != (*it).gearLimit.end(); ++gl)
               {
                   gearLimit += (*gl)+"/";
               }

               if(!gearLimit.isEmpty())
                 gearLimit = gearLimit.remove(gearLimit.size()-1,1);


               xmlWriter.writeAttribute("typeLimit",typeLimit);
               xmlWriter.writeAttribute("gearLimit",gearLimit);

               xmlWriter.writeAttribute("localName",QString::number((*it).localizeName));
               xmlWriter.writeAttribute("localizeType",QString::number((*it).localizeType));
               xmlWriter.writeAttribute("localizeGear",QString::number((*it).localizeGear));

         xmlWriter.writeEndElement(); //gearbox

    }


    //close the final tag and close the file.
     xmlWriter.writeEndDocument();

     saveFile.close();

    return allGood;
}

void ContractManager::clearContractsXML()
{
    vcList.clear();
    ecList.clear();
    ccList.clear();
    gcList.clear();
}

QList<ContractManager::vehicleContract> ContractManager::getVCList()
{
    return vcList;
}

QList<ContractManager::engineContract> ContractManager::getECList()
{
    return ecList;
}

QList<ContractManager::chassisContract> ContractManager::getCCList()
{
    return ccList;
}

QList<ContractManager::gearboxContract> ContractManager::getGCList()
{
    return gcList;
}
