#ifndef PREMADEVEHICLEEDITOR_H
#define PREMADEVEHICLEEDITOR_H

#include <QWidget>
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
    
private:
    Ui::PreMadeVehicleEditor *ui;
};

#endif // PREMADEVEHICLEEDITOR_H
