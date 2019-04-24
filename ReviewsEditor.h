#ifndef REVIEWSEDITOR_H
#define REVIEWSEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"
#include "LocalizationManager.h"
#include "ComponentsManager.h"

namespace Ui {
class ReviewsEditor;
}

class ReviewsEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ReviewsEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~ReviewsEditor();
    
private slots:
    void on_button_Intro_Add_clicked();

    void on_button_Intro_Remove_clicked();

    void on_button_Intro_0Brand_clicked();

    void on_button_Intro_1Model_clicked();

    void on_button_Intro_2Type_clicked();

    void on_button_Intro_3BestCompetition_clicked();

    void on_button_Intro_4WorstCompetition_clicked();

    void on_table_intro_cellClicked(int row, int column);

    void on_button_perf_add_clicked();

    void on_button_Perf_Remove_clicked();

    void on_button_perf_0Brand_clicked();

    void on_button_perf_1Model_clicked();

    void on_button_perf_2HP_clicked();

    void on_button_perf_3TopSpeed_clicked();

    void on_button_perf_4Acell_clicked();

    void on_button_perf_5Engine_clicked();

    void on_button_perf_6Type_clicked();

    void on_button_perf_7Power_clicked();

    void on_button_perf_8Drive_clicked();

    void on_table_perf_cellClicked(int row, int column);



    void on_button_drive_add_clicked();

    void on_button_drive_remove_clicked();

    void on_button_drive_0Brand_clicked();

    void on_button_drive_1Model_clicked();

    void on_button_drive_2Type_clicked();

    void on_button_drive_3Rating_clicked();

    void on_table_drive_cellClicked(int row, int column);

    void on_button_lux_add_clicked();

    void on_button_lux_remove_clicked();

    void on_button_lux_0Brand_clicked();

    void on_button_lux_1Model_clicked();

    void on_button_lux_2Lux_clicked();

    void on_table_lux_cellClicked(int row, int column);

    void on_button_safe_add_clicked();

    void on_button_safe_remove_clicked();

    void on_button_safe_0Brand_clicked();

    void on_button_safe_1Model_clicked();

    void on_table_safe_cellClicked(int row, int column);

    void on_button_fuel_add_clicked();

    void on_button_fuel_remove_clicked();

    void on_button_fuel_0Brand_clicked();

    void on_button_fuel_1Model_clicked();

    void on_button_fuel_2FuelCity_clicked();

    void on_button_fuel_3FuelHwy_clicked();

    void on_table_fuel_cellClicked(int row, int column);

    void on_button_power_add_clicked();

    void on_button_power_remove_clicked();

    void on_button_power_0Torque_clicked();

    void on_button_power_1Towing_clicked();

    void on_table_power_cellClicked(int row, int column);

    void on_button_cargo_add_clicked();

    void on_button_cargo_remove_clicked();

    void on_button_cargo_0Brand_clicked();

    void on_button_cargo_1Model_clicked();

    void on_button_cargo_2CargoSpace_clicked();

    void on_table_cargo_cellClicked(int row, int column);

    void on_button_qual_add_clicked();

    void on_button_qual_remove_clicked();

    void on_button_qual_0Brand_clicked();

    void on_button_qual_1Model_clicked();

    void on_table_qual_cellClicked(int row, int column);

    void on_button_depend_add_clicked();

    void on_button_depend_remove_clicked();

    void on_button_depend_0Brand_clicked();

    void on_button_depend_1Model_clicked();

    void on_table_depend_cellClicked(int row, int column);

    void on_button_overall_add_clicked();

    void on_button_overall_remove_clicked();

    void on_button_overall_0Brand_clicked();

    void on_button_overall_1Model_clicked();

    void on_button_overall_2Type_clicked();

    void on_table_overall_cellClicked(int row, int column);

    void on_button_NewReview_clicked();

    void on_button_OpenReviews_clicked();

    void on_button_SaveReviews_clicked();

    void on_button_PreviousMenu_clicked();



    void on_toolButton_select_LocalizationFile_clicked();

    void on_toolButton_selectComponentsFile_clicked();

    void fillCarTypeCombos();

private:
    class reviewData
    {
        public:
           int min;
           int max;
           int localized;
           QString review;
           QString carType;
           int carLocalize;
    };

    QVector<reviewData> reviewsIntro;
    QVector<reviewData> reviewsPerformance;
    QVector<reviewData> reviewsDrive;
    QVector<reviewData> reviewsLux;
    QVector<reviewData> reviewsSafe;
    QVector<reviewData> reviewsFuel;
    QVector<reviewData> reviewsPower;
    QVector<reviewData> reviewsCargo;
    QVector<reviewData> reviewsQuality;
    QVector<reviewData> reviewsDepend;
    QVector<reviewData> reviewsOverall;

    Ui::ReviewsEditor *ui;
    widgetContainerStorage cp_wsc;
    LocalizationManager *lm;
    ComponentsManager *cm;

    void fillIntroTable();
    void fillPerformanceTable();
    void fillDriveTable();
    void fillLuxTable();
    void fillSafeTable();
    void fillFuelTable();
    void fillPowerTable();
    void fillCargoTable();
    void fillQualTable();
    void fillDependTable();
    void fillOverallTable();
};

#endif // REVIEWSEDITOR_H
