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


#ifndef TURNEVENTSDATA_H
#define TURNEVENTSDATA_H
#include <QList>
#include <QString>
#include <QMap>
#include "LocalizationManager.h"
#include <QDomNode>
class TurnData
{
public:



    class TE_ComponentEvts
    {
        public:
        int selectedID;
        double popChange;
    };

    class TE_CityEvts
    {
        public:
            int id;
            QString Flag;
            QString Nation;
            double infChange, manuChange;
            int gov;
            bool bNationOrFlagChange;
            bool bInfManuChange;
            bool bgovChange;

    };

    class TE_NewsEvts
    {
        public:
        int type;
        bool localization;
        QString headline;
        QString body;
        QString image;
        bool glow;
    };

    class TE_VehiclePop
    {
        public:
            int selectorID;
            double pop;
            double popR1,popR2,popR3,popR4,popR5,popR6;

    };

    class TE_Data
    {
        public:
        QString officeFile;
        double globalInterest;
        double carInflation;
        double globalFuelRate;
        double buyerRate;
        double pensionRate;
        double stockRate;
        QList<TE_ComponentEvts> CompEvtList;
        QList<TE_CityEvts> CityEvtList;
        QList<TE_NewsEvts> NewsEvtList;
        QList<TE_VehiclePop> vehiclePopList;


    };

    TurnData(QString fileName, QWidget *widget, LocalizationManager *lm = 0);
    QMap<int,TurnData::TE_Data> getTurnMap();
    TE_Data generateBlankTEData();

private:
    QMap<int,TurnData::TE_Data> tdMap;
    void cityChange(QDomElement cityElement, int year, int turn);
    void officeChange(QDomElement element, int year, int turn);
    void buyRateChange(QDomElement element, int year, int turn);
    void pensionGrowthChange(QDomElement element, int year, int turn);
    void stockGrowthChange(QDomElement element, int year, int turn);
    void globalInterestChange(QDomElement element, int year, int turn);
    void gasChange(QDomElement element, int year, int turn);
    void carPriceChange(QDomElement element, int year, int turn);
    void componentProcessor(QDomElement cityElement, int year, int turn);
    void vehicleProcessor(QDomElement element, int year, int turn);
    void gameEvents(QDomElement baseElement, int year, int turn);


};

#endif // TURNEVENTSDATA_H
