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


#include "CityData.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QDomNode>
#include <QMessageBox>


//Read CityScript file and save information into a map of cities.
CityData::CityData(QString openFileName, QWidget *widget)
{
    //XML read in and spit out to ds file.

    dataStore dsData;

        QFile file(openFileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
           QMessageBox::critical(widget,"File Error",
                                 QString("Whopse, opening %1 failed.").arg(openFileName));
           return;
        }

        //Process XML File.
        QString errorMsg;
        int errorLine, errorColumn;
        QDomDocument doc;
        QDomElement cityElement;


        if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
        {
           QMessageBox::critical(widget,"Error City Script",
                                QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                                errorMsg,errorLine,errorColumn));
            return;
        }

        //Begin Reading and putting the data into the map
        QDomElement rootNode = doc.firstChildElement("CreateCityDB");
        if(rootNode.isNull())
        {
            QMessageBox::critical(widget,"Error","City Script is malformed.");
            return;
        }

        //Get first city, and lets go
        rootNode = doc.documentElement();
        cityElement = rootNode.firstChildElement("City");

      do
      {
       dsData.id = cityElement.firstChildElement("ID").attributeNode("id").value().toInt();
       dsData.cityName = cityElement.firstChildElement("NAME").attributeNode("name").value();
       dsData.cityCountry = cityElement.firstChildElement("COUNTRY").attributeNode("nation").value();
       dsData.population = cityElement.firstChildElement("POPULATION").attributeNode("pop").value().toLongLong();
       dsData.popGrowth = cityElement.firstChildElement("POPGROWTH").attributeNode("pg").value().toDouble();
       dsData.perCapita = cityElement.firstChildElement("CAPITA").attributeNode("cap").value().toInt();
       dsData.ecoGrowth = cityElement.firstChildElement("ECOGROWTH").attributeNode("ecog").value().toDouble();
       dsData.resourceRates = cityElement.firstChildElement("RESOURCES").attributeNode("res").value().toDouble();
       dsData.fuelCosts = cityElement.firstChildElement("GAS").attributeNode("gastax").value().toDouble();
       dsData.infRates = cityElement.firstChildElement("INF").attributeNode("infit").value().toInt();
       dsData.skillRates = cityElement.firstChildElement("SKILL").attributeNode("ski").value().toInt();
       dsData.avgWages = cityElement.firstChildElement("WAGE").attributeNode("empwage").value().toInt();
       dsData.manuSkill = cityElement.firstChildElement("MNFBASE").attributeNode("manub").value().toInt();
       dsData.manuGrowth = cityElement.firstChildElement("MANUGROWTH").attributeNode("manug").value().toDouble();
       dsData.gov = cityElement.firstChildElement("GOVERN").attributeNode("gov").value().toInt();
       dsData.taxRate = cityElement.firstChildElement("TAX").attributeNode("taxrate").value().toDouble();
       dsData.flagFileName = cityElement.firstChildElement("FLAG").attributeNode("flagfile").value();
       dsData.headingFileName = cityElement.firstChildElement("PIC").attributeNode("picture").value();
       dsData.cordLat = cityElement.firstChildElement("CORDS").attributeNode("lat").value().toDouble();
       dsData.cordLong = cityElement.firstChildElement("CORDS").attributeNode("long").value().toDouble();
       dsData.region = cityElement.firstChildElement("REGION").text().toInt();
       dsData.buyerRate = cityElement.firstChildElement("BUYRATE").text().toDouble();


        dsMap.insert(dsData.id,dsData);


       cityElement = cityElement.nextSiblingElement("City");
      }
      while(!cityElement.isNull());

      file.close();
}

//Return citymap to caller
QMap<int, CityData::dataStore> CityData::returnCopyOfDSMap()
{
    return dsMap;
}

//Return the number of cities in the citymap
int CityData::size()
{
    return dsMap.size();
}
