#ifndef PREMADEVEHICLEEDITOR_H
#define PREMADEVEHICLEEDITOR_H

#include <QWidget>
#include <QMessageBox>
#include "BaseContainerStorage.h"

namespace Ui {
class PreMadeVehicleEditor;
}

class PreMadeVehicleEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit PreMadeVehicleEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~PreMadeVehicleEditor();
    
private slots:
    void on_Button_Premade_AddNew_clicked();

    void on_List_Premade_IncludeVehicles_cellClicked(int row, int column);

    void on_Button_Premade_EditSelected_clicked();

    void on_Button_Premade_RemoveSelected_clicked();

    void on_Button_Premade_NewList_clicked();

    void on_Button_Premade_OpenList_clicked();

    void on_Button_Premade_SaveList_clicked();

    void on_Button_Premade_ReturnToMain_clicked();

    void on_Button_Premade_GetCarFile_clicked();

    void on_Button_Premade_BulkAdd_clicked();

private:
    class premadeData
    {
        public:
            QString name, designer, carFile, meshFile;
            int startYear, endYear;
    };

    Ui::PreMadeVehicleEditor *ui;
    QList<premadeData> premadeList;
    premadeData selectedPremade;

    widgetContainerStorage cp_wsc;


    void fillTable();
};

#endif // PREMADEVEHICLEEDITOR_H
