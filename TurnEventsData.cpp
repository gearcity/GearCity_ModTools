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



#include "TurnEventsData.h"
#include <QtXml/QDomDocument>

#include <QFile>
#include <QtWidgets/QMessageBox>


//Create TurnData object which is mainly for TurnEvents map
TurnData::TurnData(QString fileName, QWidget *widget, LocalizationManager *lm)
{
    if(fileName == "")
        return;

    //Open turn events file
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(widget,"File Error",
                              QString("Whopse, opening %1 failed.").arg(fileName));
             return;
    }

    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument doc;
    QDomElement yearElement;
    QDomElement turnElement;
    QDomElement baseElement;
    QDomElement subElement;

    //Parse TurnEvents.xml file.
    if(!doc.setContent(&file,&errorMsg,&errorLine,&errorColumn))
    {
        QMessageBox::critical(widget,"Error Processing TurnEvents File",
                             QString("Error Message: %1\nLine: %2\nColumn: %3").arg(
                             errorMsg,errorLine,errorColumn));
       return;
    }

    //Begin Reading and putting the data into the map
    QDomElement rootNode = doc.firstChildElement("Evts");
    if(rootNode.isNull())
    {
         QMessageBox::critical(widget,"Error","Turn Events is malformed.");
        return;
    }

    //Get first year, and lets go
    rootNode = doc.documentElement();
    yearElement = rootNode.firstChildElement("year");

    int year = 0, turn = 0;

    //Loop through the years
    while(!yearElement.isNull())
    {
        //Store year
       year = yearElement.attributeNode("y").value().toInt();

       //For Old Format, start of the year values, we're going to save these to turn 1.
       baseElement = turnElement.firstChildElement("GameEvts");
       if(!baseElement.isNull())
       {
         turn = 1;
         gameEvents(baseElement,year,turn);
       }

       //grab first turn in year
       turnElement = yearElement.firstChildElement("turn");

       //Loop through turns
       while(!turnElement.isNull())
       {
           //Grab Turn Value
           turn = turnElement.attributeNode("t").value().toInt();

           //Grab world events, parse the sub elements
           baseElement = turnElement.firstChildElement("WorldEvts");
           if(!baseElement.isNull())
           {
                subElement = baseElement.firstChildElement("cityChange");
                if(!subElement.isNull())
                {
                   cityChange(subElement, year, turn);
                }

           }

            //Grab news events, parse the sub elements
           baseElement = turnElement.firstChildElement("NewsEvts");
           if(!baseElement.isNull())
           {
               subElement = baseElement.firstChildElement("comment");
               while(!subElement.isNull())
               {
                   TE_NewsEvts tene;
                   if(subElement.hasAttribute("glow"))
                       tene.glow = subElement.attributeNode("glow").value().toInt();
                   else
                       tene.glow = false;

                   if(subElement.hasAttribute("localization"))
                       tene.localization = subElement.attributeNode("localization").value().toInt();
                   else
                       tene.localization = false;

                   if(subElement.hasAttribute("type"))
                       tene.type = subElement.attributeNode("type").value().toInt();
                   else
                       tene.type = 0;

                   if(subElement.hasAttribute("headline"))
                       tene.headline = subElement.attributeNode("headline").value();
                   else
                       tene.headline = "";

                   if(subElement.hasAttribute("body"))
                       tene.body = subElement.attributeNode("body").value();
                   else
                       tene.body = "";


                   if(subElement.hasAttribute("image"))
                       tene.image = subElement.attributeNode("image").value();
                   else
                       tene.image = "";

                   //If month since 0AD is in the map, we just insert into the NewsEventList
                   if(tdMap.contains((year*12)+turn))
                   {
                       tdMap.find((year*12)+turn).value().NewsEvtList.push_back(tene);
                   }
                   else//The month is not in the map,create a blank data and then insert into news.
                   {
                       TE_Data ted = generateBlankTEData();
                       tdMap.insert((year*12)+turn,ted);
                       tdMap.find((year*12)+turn).value().NewsEvtList.push_back(tene);
                   }

                   subElement = subElement.nextSiblingElement("comment");
               }
           }

           //Grab game events, parse the sub elements
           baseElement = turnElement.firstChildElement("GameEvts");
           if(!baseElement.isNull())
           {
               gameEvents(baseElement,year,turn);
           }

            //Next Turn
           turnElement = turnElement.nextSiblingElement("turn");
       }

        //Next Year
        yearElement = yearElement.nextSiblingElement("year");
    }

    file.close();

}

//Parse Office Change Element
void TurnData::officeChange(QDomElement element, int year, int turn)
{
    QString officeFile = "";

    if(!element.attributeNode("file").isNull())
     officeFile = element.attributeNode("file").value();

    if(officeFile != "")
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().officeFile = officeFile;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().officeFile = officeFile;
        }
    }

}

//Parse Buy Rate Change Element
void TurnData::buyRateChange(QDomElement element, int year, int turn)
{
    double buyRate = -1;

    if(!element.attributeNode("rate").isNull())
     buyRate = element.attributeNode("rate").value().toDouble();

    if(buyRate > 0)
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().buyerRate = buyRate;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().buyerRate = buyRate;
        }
    }

}

//Parse Pension Growth Change Element
void TurnData::pensionGrowthChange(QDomElement element, int year, int turn)
{
    double pensionGrowth = -1;

    if(!element.attributeNode("rate").isNull())
     pensionGrowth = element.attributeNode("rate").value().toDouble();

    if(pensionGrowth > 0)
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().pensionRate = pensionGrowth;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().pensionRate = pensionGrowth;
        }
    }

}


//Parse Stock Growth Change Element
void TurnData::stockGrowthChange(QDomElement element, int year, int turn)
{
    double stockGrowth = -1;

    if(!element.attributeNode("rate").isNull())
     stockGrowth = element.attributeNode("rate").value().toDouble();

    if(stockGrowth > 0)
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().stockRate = stockGrowth;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().stockRate = stockGrowth;
        }
    }

}

//Parse Global Interest Rate Change Element
void TurnData::globalInterestChange(QDomElement element, int year, int turn)
{
    double globalInterest = -1;

    if(!element.attributeNode("global").isNull())
     globalInterest = element.attributeNode("global").value().toDouble();

    if(globalInterest > 0)
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().globalInterest = globalInterest;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().globalInterest = globalInterest;
        }
    }

}

//Parse Fuel Rate Change Element
void TurnData::gasChange(QDomElement element, int year, int turn)
{
    double gas = -1;

    if(!element.attributeNode("rate").isNull())
     gas = element.attributeNode("rate").value().toDouble();

    if(gas > 0)
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().globalFuelRate = gas;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().globalFuelRate = gas;
        }
    }

}

//Parse Car Price Change Element
void TurnData::carPriceChange(QDomElement element, int year, int turn)
{
    double carPrice = -1;

    if(!element.attributeNode("rate").isNull())
     carPrice = element.attributeNode("rate").value().toDouble();

    if(carPrice > 0)
    {
        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().carInflation = carPrice;
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().carInflation = carPrice;
        }
    }

}

//Parse Component Change Element
void TurnData::componentProcessor(QDomElement element, int year, int turn)
{
    while(!element.isNull())
    {
        TE_ComponentEvts tece;


        tece.selectedID = element.attributeNode("selectedIndex").value().toInt();

        if(!element.attributeNode("popChange").isNull())
        {
         tece.popChange = element.attributeNode("popChange").value().toDouble();
        }



        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().CompEvtList.push_back(tece);
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().CompEvtList.push_back(tece);
        }

        element = element.nextSiblingElement("component");
    }
}

//Parse Vehicle Change Element
void TurnData::vehicleProcessor(QDomElement element, int year, int turn)
{
    while(!element.isNull())
    {
        TE_VehiclePop tevp;


        tevp.selectorID = element.attributeNode("selectedIndex").value().toInt();

        if(!element.attributeNode("pop").isNull())
        {
         tevp.pop = element.attributeNode("pop").value().toDouble();
        }
        else
        {
            tevp.pop = 0;
        }

        if(!element.attributeNode("popR1").isNull())
        {
         tevp.popR1 = element.attributeNode("popR1").value().toDouble();
        }
        else
        {
         tevp.popR1 = 0;
        }

        if(!element.attributeNode("popR2").isNull())
        {
         tevp.popR2 = element.attributeNode("popR2").value().toDouble();
        }
        else
        {
            tevp.popR2 = 0;
        }

        if(!element.attributeNode("popR3").isNull())
        {
         tevp.popR3 = element.attributeNode("popR3").value().toDouble();
        }
        else
        {
            tevp.popR3 = 0;
        }

        if(!element.attributeNode("popR4").isNull())
        {
         tevp.popR4 = element.attributeNode("popR4").value().toDouble();
        }
        else
        {
            tevp.popR4 = 0;
        }

        if(!element.attributeNode("popR5").isNull())
        {
         tevp.popR5 = element.attributeNode("popR5").value().toDouble();
        }
        else
        {
            tevp.popR5 = 0;
        }

        if(!element.attributeNode("popR6").isNull())
        {
         tevp.popR6 = element.attributeNode("popR6").value().toDouble();
        }
        else
        {
            tevp.popR6 = 0;
        }



        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().vehiclePopList.push_back(tevp);
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().vehiclePopList.push_back(tevp);
        }

        element = element.nextSiblingElement("vehiclepop");
    }
}

//Parse City Change Element
void TurnData::cityChange(QDomElement cityElement, int year, int turn)
{
    while(!cityElement.isNull())
    {
        TE_CityEvts tece;
        tece.bNationOrFlagChange = false;
        tece.bInfManuChange = false;
        tece.bgovChange = false;

        tece.id = cityElement.attributeNode("id").value().toInt();

        if(!cityElement.attributeNode("Nation").isNull())
        {
            tece.bNationOrFlagChange = true;
            tece.Nation = cityElement.attributeNode("Nation").value();
            tece.Flag = cityElement.attributeNode("Flag").value();
        }
        if(!cityElement.attributeNode("infChange").isNull())
        {
            tece.bInfManuChange = true;
            tece.infChange = cityElement.attributeNode("infChange").value().toDouble();
            tece.manuChange = cityElement.attributeNode("manuChange").value().toDouble();
        }
        if(!cityElement.attributeNode("gov").isNull())
        {
            tece.bgovChange = true;
            tece.gov = cityElement.attributeNode("gov").value().toInt();
        }


        //If month since 0AD is in map, insert info,
        //if not create blank data, add to map, then insert info
        if(tdMap.contains((year*12)+turn))
        {
            tdMap.find((year*12)+turn).value().CityEvtList.push_back(tece);
        }
        else
        {
            TE_Data ted = generateBlankTEData();
            tdMap.insert((year*12)+turn,ted);
            tdMap.find((year*12)+turn).value().CityEvtList.push_back(tece);
        }

        cityElement = cityElement.nextSiblingElement("cityChange");
    }
}

//Process Game Events sub elements, this is called from the main XML reader so we don't
//flood the main reader loop with lots of stuff over and over...
void TurnData::gameEvents(QDomElement baseElement, int year, int turn)
{
    QDomElement subElement;
    subElement = baseElement.firstChildElement("office");
    if(!subElement.isNull())
    {
       officeChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("buyrate");
    if(!subElement.isNull())
    {
       buyRateChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("pensionGrowth");
    if(!subElement.isNull())
    {
       pensionGrowthChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("stockrate");
    if(!subElement.isNull())
    {
       stockGrowthChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("interest");
    if(!subElement.isNull())
    {
       globalInterestChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("gas");
    if(!subElement.isNull())
    {
       gasChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("carprice");
    if(!subElement.isNull())
    {
       carPriceChange(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("component");
    if(!subElement.isNull())
    {
       componentProcessor(subElement, year, turn);
    }

    subElement = baseElement.firstChildElement("vehiclepop");
    if(!subElement.isNull())
    {
       vehicleProcessor(subElement, year, turn);
    }

}

//Returns the final turn map to caller
QMap<int,TurnData::TE_Data> TurnData::getTurnMap()
{
    return tdMap;
}

//Generates a blank TE_Data file which is the main storage element for the turnMap.
TurnData::TE_Data TurnData::generateBlankTEData()
{
    TE_Data ted;
        ted.officeFile = "";
        ted.globalInterest=-1;
        ted.carInflation=-1;
        ted.globalFuelRate=-1;
        ted.buyerRate=-1;
        ted.pensionRate=-1;
        ted.stockRate=-1;
    return ted;
}
