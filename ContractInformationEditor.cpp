#include "ContractInformationEditor.h"
#include "ui_ContractInformationEditor.h"

ContractInformationEditor::ContractInformationEditor(widgetContainerStorage wsc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContractInformationEditor)
{
    ui->setupUi(this);
}

ContractInformationEditor::~ContractInformationEditor()
{
    delete ui;
}
