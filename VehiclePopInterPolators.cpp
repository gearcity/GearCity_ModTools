#include "VehiclePopInterPolators.h"
#include "ui_VehiclePopInterPolators.h"
#include <QTableView>

VehiclePopInterPolators::VehiclePopInterPolators( QList<ComponentsManager::CarTypes> cl ,
                                                 int selectedID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VehiclePopInterPolators)
{
    double napop = 0;
    double sapop = 0;
    double eupop = 0;
    double afpop = 0;
    double aspop = 0;
    double aupop = 0;
    double pop = 0;

  //  ComponentsManager *cm = currentComponent;

    ui->setupUi(this);
    for(QList<ComponentsManager::CarTypes>::iterator it = cl.begin(); it != cl.end(); ++it)
    {
        if((*it).selectionIndex == selectedID)
        {
            napop = (*it).popNA;
            sapop = (*it).popSA;
            eupop = (*it).popEU;
            afpop = (*it).popAF;
            aspop = (*it).popAS;
            aupop = (*it).popAU;
            pop =  (*it).pop;
                break;
        }
    }

    /*
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

        currentComponents->CarTypes
    ui->Table_VehiclePopInterPolators->*/
}

VehiclePopInterPolators::~VehiclePopInterPolators()
{
    delete ui;
}

void VehiclePopInterPolators::interpolateAndDisplay()
{

}
