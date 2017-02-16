#ifndef MATERIALMAKERWINDOW_H
#define MATERIALMAKERWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MaterialMakerWindow;
}

class MaterialMakerWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MaterialMakerWindow(QWidget *parent = 0);
    ~MaterialMakerWindow();
    
private slots:
    void on_Button_MatEditor_OpenMatFile_clicked();
    void on_Button_MatEditor_SaveMatFile_clicked();
    void on_Button_MatEditor_AddEditMat_clicked();
    void on_Button_MatEditor_ClearMatInfo_clicked();
    void on_Button_MatEditor_DeleteMat_clicked();
    void on_Table_MatEditor_Mats_cellClicked(int row, int column);


private:
    void fillTable();

    Ui::MaterialMakerWindow *ui;
    QMap<QString,QString> materialMap;
};

#endif // MATERIALMAKERWINDOW_H
