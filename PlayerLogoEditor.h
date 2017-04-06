#ifndef PLAYERLOGOEDITOR_H
#define PLAYERLOGOEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"
#include <QMap>

namespace Ui {
class PlayerLogoEditor;
}

class PlayerLogoEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayerLogoEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~PlayerLogoEditor();
    
private slots:
    void on_button_selectLogoFolder_clicked();

    void on_button_addLogo_clicked();

    void on_button_removeLogo_clicked();

    void on_table_logo_cellClicked(int row, int column);

    void on_button_newLogo_clicked();

    void on_button_openLogoFile_clicked();

    void on_button_saveLogoFile_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::PlayerLogoEditor *ui;
    widgetContainerStorage cp_wsc;
    QMap<QString,QString> logoMap;

    void fillTable();
};

#endif // PLAYERLOGOEDITOR_H
