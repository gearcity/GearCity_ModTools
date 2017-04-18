#ifndef COMPONENTSEDITOR_H
#define COMPONENTSEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"
#include "ComponentsManager.h"
#include <QMap>

namespace Ui {
class ComponentsEditor;
}

class ComponentsEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ComponentsEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~ComponentsEditor();
    
private slots:
    void on_button_frame_add_clicked();

    void on_table_frame_cellClicked(int row, int column);

    void on_button_frame_remove_clicked();


    void on_button_suspension_add_clicked();

    void on_button_suspension_remove_clicked();

    void on_table_suspension_cellClicked(int row, int column);

    void on_button_drivertrain_add_clicked();

    void on_button_drivetrain_remove_clicked();

    void on_table_drivetrain_cellClicked(int row, int column);

    void on_button_englayout_add_clicked();

    void on_button_englayout_cylrestrict_add_clicked();

    void on_button_englayout_cylrestrict_remove_clicked();

    void on_list_englayout_cylrestrict_currentTextChanged(const QString &currentText);

    void on_button_englayout_fuelrestrict_add_clicked();

    void on_button_englayout_fuelrestrict_remove_clicked();

    void on_list_englayout_fuelrestrict_currentTextChanged(const QString &currentText);

    void on_button_englayout_inductrestrict_add_clicked();

    void on_button_englayout_inductrestrict_remove_clicked();

    void on_list_englayout_inducrestrict_currentTextChanged(const QString &currentText);

    void on_button_englayout_remove_clicked();

    void on_table_englayout_cellClicked(int row, int column);

    void on_button_cyl_add_clicked();

    void on_button_cyl_remove_clicked();

    void on_table_cyl_cellClicked(int row, int column);

    void on_button_fuel_add_clicked();

    void on_button_fuel_remove_clicked();

    void on_table_fuel_cellClicked(int row, int column);

    void on_button_valve_add_clicked();

    void on_button_valve_remove_clicked();

    void on_table_valve_cellClicked(int row, int column);

    void on_button_induct_add_clicked();

    void on_button_induct_remove_clicked();

    void on_table_induction_cellClicked(int row, int column);



    void on_button_gearbox_gears_add_clicked();

    void on_button_gearbox_gear_remove_clicked();

    void on_list_gearbox_gears_currentTextChanged(const QString &currentText);

    void on_button_gearbox_add_clicked();

    void on_button_gearbox_remove_clicked();

    void on_table_gearbox_cellClicked(int row, int column);

    void on_button_gears_add_clicked();

    void on_button_gears_remove_clicked();

    void on_table_gears_cellClicked(int row, int column);

    void on_button_cartype_add_clicked();

    void on_button_cartype_remove_clicked();

    void on_table_cartype_cellClicked(int row, int column);

    void on_button_carmodel_add_clicked();

    void on_button_carmodel_remove_clicked();

    void on_Table_CarModels_cellClicked(int row, int column);

    void on_button_Accessory_add_clicked();

    void on_button_Accessory_remove_clicked();

    void on_table_Accessory_cellClicked(int row, int column);

    void on_button_aipop_add_clicked();

    void on_button_aipop_remove_clicked();

    void on_table_aipop_cellClicked(int row, int column);

    void on_button_select_folder_picture_clicked();

    void on_button_select_carmodel_folder_clicked();

    void on_button_accessories_selectfolder_clicked();

    void on_button_newComponentsFile_clicked();

    void on_button_openComponentsFiles_clicked();

    void on_button_saveComponentsFiles_clicked();



    void on_checkBox_accessory_decal_clicked();

    void on_button_returnToMain_clicked();

private:













    Ui::ComponentsEditor *ui;
    widgetContainerStorage cp_wsc;

    QVector<ComponentsManager::ChassisFrameComps> frames;
    QVector<ComponentsManager::SuspensionComps> suspensions;
    QVector<ComponentsManager::DriveTrainComps> drivetrains;
    QVector<ComponentsManager::EngineLayoutComps> layouts;
    QVector<ComponentsManager::EngineCylinderComps> cylinders;
    QVector<ComponentsManager::EngineFuelComps> fuels;
    QVector<ComponentsManager::EngineValveComps> valves;
    QVector<ComponentsManager::EngineInductionComps> inductions;
    QVector<ComponentsManager::GearBoxComps> gearboxes;
    QVector<ComponentsManager::GearGearsComps> gearVector;
    QVector<ComponentsManager::CarTypes> typeVector;
    QVector<ComponentsManager::CarModels> carModelVector;
    QVector<ComponentsManager::AccessoriesModels> accessories;

    QMap<QString, ComponentsManager::aiPopData> aiPops;



    QStringList currentCylRestrictions;
    QStringList currentFuelRestrictions;
    QStringList currentInductionRestrictions;
    QStringList currentGearRestrictions;

    QMap<int, QString> selectorIDMap;

    void fillFrameTable();
    void fillSuspensionTable();
    void fillDrivetrainTable();
    void fillEngineLayoutTable();
    void fillCylinderTable();
    void fillFuelTable();
    void fillValveTable();
    void fillInductionTable();
    void fillGearboxTable();
    void fillGearsTable();
    void fillCarTypeTable();
    void fillCarModelTable();
    void fillAccessoriesTable();
    void fillAIPopTable();
    void fillAIPopComboData();
};

#endif // COMPONENTSEDITOR_H
