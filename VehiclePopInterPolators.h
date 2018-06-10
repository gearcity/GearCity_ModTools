#ifndef VEHICLEPOPINTERPOLATORS_H
#define VEHICLEPOPINTERPOLATORS_H

#include <QMainWindow>
#include "ComponentsManager.h"

namespace Ui {
class VehiclePopInterPolators;
}

class VehiclePopInterPolators : public QMainWindow
{
    Q_OBJECT

public:
    explicit VehiclePopInterPolators( QList<ComponentsManager::CarTypes> cl,
                                     int selectedID = 0,QWidget *parent = 0);
    ~VehiclePopInterPolators();

    void interpolateAndDisplay();

private:
    Ui::VehiclePopInterPolators *ui;
};

#endif // VEHICLEPOPINTERPOLATORS_H
