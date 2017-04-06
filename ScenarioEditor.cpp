#include "ScenarioEditor.h"
#include "ui_ScenarioEditor.h"

ScenarioEditor::ScenarioEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScenarioEditor)
{
    ui->setupUi(this);
}

ScenarioEditor::~ScenarioEditor()
{
    delete ui;
}
