#ifndef NAMELISTEDITOR_H
#define NAMELISTEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"

namespace Ui {
class NameListEditor;
}

class NameListEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit NameListEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~NameListEditor();
    
private slots:
    void on_button_addName_clicked();

    void on_button_removeName_clicked();

    void on_list_Names_currentTextChanged(const QString &currentText);

    void on_button_NewNameList_clicked();

    void on_button_OpenNameList_clicked();

    void on_button_SaveNameList_clicked();

    void on_button_ReturnToPrevious_clicked();

private:
    void fillList();
    Ui::NameListEditor *ui;

    QStringList nameList;
    widgetContainerStorage cp_wsc;
};

#endif // NAMELISTEDITOR_H
