#ifndef CONTRACTINFORMATIONEDITOR_H
#define CONTRACTINFORMATIONEDITOR_H

#include <QtWidgets/QWidget>
#include "BaseContainerStorage.h"
#include "LocalizationManager.h"
#include "ComponentsManager.h"
#include "ContractManager.h"
#include "QtWidgets/QListWidget"
#include "QtWidgets/QListWidgetItem"


namespace Ui {
class ContractInformationEditor;
}

class ContractInformationEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ContractInformationEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~ContractInformationEditor();
    
private slots:
    void on_button_SelectComponentsFile_clicked();

    void on_button_SelectLocalizeFile_clicked();

    void refillCombos();

    void on_button_v_addFuelLimit_clicked();

    void on_button_v_vehicleTypeAdd_clicked();

    void on_button_v_vehicleTypeRemove_clicked();

    void on_button_v_removeFuelLimit_clicked();
    

    void on_comboBox_v_fuelTypes_currentIndexChanged(int index);

    void on_comboBox_v_vehicleTypeLimit_currentIndexChanged(int index);

    void on_listWidget_v_fuelLimit_itemClicked(QListWidgetItem *item);

    void on_listWidget_v_vehicleTypeLimit_itemClicked(QListWidgetItem *item);

    void on_button_v_addContract_clicked();

    void on_pushButton_v_removeContract_clicked();

    void on_button_v_editContract_clicked();

    void on_listWidget_v_vehicleContracts_itemClicked(QListWidgetItem *item);

    void on_pushButton_e_fuelLimitAdd_clicked();

    void on_button_e_layoutLimitAdd_clicked();

    void on_button_e_cylinderLimitAdd_clicked();

    void on_pushButton_e_fuelLimitRemove_clicked();

    void on_button_e_layoutLimitsRemove_clicked();

    void on_button_e_cylinderLimitRemove_clicked();

    void on_comboBox_e_fuelLimit_currentIndexChanged(int index);

    void on_comboBox_e_LayoutLimits_currentIndexChanged(int index);

    void on_comboBox_e_CylinderLimit_currentIndexChanged(int index);

    void on_listWidget_e_FuelLimit_itemClicked(QListWidgetItem *item);

    void on_listWidget_e_layoutLimit_itemClicked(QListWidgetItem *item);

    void on_listWidget_e_cylinderLimit_itemClicked(QListWidgetItem *item);

    void on_pushButton_e_contractAdd_clicked();

    void on_pushButton_e_contractEdit_clicked();

    void on_pushButton_e_contractRemove_clicked();

    void on_listWidget_e_contracts_itemClicked(QListWidgetItem *item);

    void on_pushButton_c_contractsAdd_clicked();

    void on_pushButton_c_contractsEdit_clicked();

    void on_pushButton_c_contractsRemove_clicked();

    void on_listWidget_c_contracts_itemClicked(QListWidgetItem *item);

    void on_button_g_GearLimitsAdd_clicked();

    void on_button_g_typeLimitAdd_clicked();

    void on_button_g_GearLimitsRemove_clicked();

    void on_button_g_typeLimitRemove_clicked();

    void on_comboBox_g_GearLimits_currentIndexChanged(int index);

    void on_comboBox_g_typeLimit_currentIndexChanged(int index);

    void on_listWidget_g_GearLimits_itemClicked(QListWidgetItem *item);

    void on_listWidget_g_TypeLimit_itemClicked(QListWidgetItem *item);

    void on_button_g_contractsAdd_clicked();

    void on_button_g_contractsEdit_clicked();

    void on_button_g_contractsRemove_clicked();

    void on_listWidget_g_contracts_itemClicked(QListWidgetItem *item);

    void on_button_NewContractsFile_clicked();

    void on_button_SaveContractsFile_clicked();

    void on_button_OpenContracts_clicked();

private:
    Ui::ContractInformationEditor *ui;
    widgetContainerStorage cp_wsc;
    LocalizationManager *lm;
    ComponentsManager *cm;

    QList<ContractManager::vehicleContract> vcEditorList;
    QList<ContractManager::engineContract> ecEditorList;
    QList<ContractManager::chassisContract> ccEditorList;
    QList<ContractManager::gearboxContract> gcEditorList;
};

#endif // CONTRACTINFORMATIONEDITOR_H
