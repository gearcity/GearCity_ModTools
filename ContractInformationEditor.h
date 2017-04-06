#ifndef CONTRACTINFORMATIONEDITOR_H
#define CONTRACTINFORMATIONEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"

namespace Ui {
class ContractInformationEditor;
}

class ContractInformationEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ContractInformationEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~ContractInformationEditor();
    
private:
    Ui::ContractInformationEditor *ui;
};

#endif // CONTRACTINFORMATIONEDITOR_H
