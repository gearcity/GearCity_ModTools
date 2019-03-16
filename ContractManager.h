#ifndef CONTRACTMANAGER_H
#define CONTRACTMANAGER_H

#include "QString"
#include "QList"
#include "QStringList"


class ContractManager
{
public:
    ContractManager();

    class vehicleContract
    {
        public:
            QString name;
            int type;
            int military;
            int start;
            int end;
            double ccMod;
            double weightMod;
            double fuelMod;
            double hpMod;
            double cargoMod;
            double latgMod;
            double torqueMod;
            double topSpeedMod;
            double contractSize;
            double contractPrice;

            QStringList fuelLimit;
            QStringList vehicleTypeLimit;

            int localizeName;
            int localizeFuel;
            int localizeType;

    };

    class engineContract
    {
        public:
            QString name;
            int type;
            int military;
            int start;
            int end;
            double ccMod;
            double weightMod;
            double fuelMod;
            double hpMod;
            double widthMod;
            double lengthMod;
            double torqueMod;
            double contractSize;
            double contractPrice;

            QStringList fuelLimit;
            QStringList layoutLimit;
            QStringList cylinderLimit;

            int localizeName;
            int localizeFuel;
            int localizeLayout;
            int localizeCylinder;
    };

    class chassisContract
    {
        public:
            QString name;
            int type;
            int military;
            int start;
            int end;
            double weightMod;
            double minWidth;
            double maxWidth;
            double minLength;
            double maxLength;
            double contractSize;
            double contractPrice;

            int localizeName;
    };

    class gearboxContract
    {
        public:
            QString name;
            int type;
            int military;
            int start;
            int end;
            double weightMod;
            double contractSize;
            double contractPrice;


            QStringList gearLimit;
            QStringList typeLimit;

            int localizeName;
            int localizeGear;
            int localizeType;

    };

    bool openContractsXML(QString fileName, QString& errorHeader, QString& errorBody);
    bool saveContractsXML(QString fileName, QList<vehicleContract> svcList,
    QList<engineContract> secList, QList<chassisContract> sccList, QList<gearboxContract> sgcList);
    void clearContractsXML();

    QList<vehicleContract> getVCList();
    QList<engineContract> getECList();
    QList<chassisContract> getCCList();
    QList<gearboxContract> getGCList();



private:
    QList<vehicleContract> vcList;
    QList<engineContract> ecList;
    QList<chassisContract> ccList;
    QList<gearboxContract> gcList;

};

#endif // CONTRACTMANAGER_H
