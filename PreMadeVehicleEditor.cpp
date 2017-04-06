#include "PreMadeVehicleEditor.h"
#include "ui_PreMadeVehicleEditor.h"

PreMadeVehicleEditor::PreMadeVehicleEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreMadeVehicleEditor)
{
    ui->setupUi(this);
}

PreMadeVehicleEditor::~PreMadeVehicleEditor()
{
    delete ui;
}
