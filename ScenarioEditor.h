#ifndef SCENARIOEDITOR_H
#define SCENARIOEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"

namespace Ui {
class ScenarioEditor;
}

class ScenarioEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScenarioEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~ScenarioEditor();
    
private:
    Ui::ScenarioEditor *ui;
};

#endif // SCENARIOEDITOR_H
