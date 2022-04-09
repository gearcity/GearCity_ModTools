#ifndef DYKEDITOR_H
#define DYKEDITOR_H

#include <QtWidgets/QWidget>
#include "BaseContainerStorage.h"
#include <QtWidgets/QListWidgetItem>

namespace Ui {
class DYKEditor;
}

class DYKEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit DYKEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~DYKEditor();
    
private slots:


    void on_button_add_clicked();

    void on_button_remove_clicked();

    void on_Button_new_clicked();

    void on_button_open_clicked();

    void on_button_save_clicked();

    void on_listWidget_dyk_itemClicked(QListWidgetItem *item);

    void on_button_ReturnToMain_clicked();

    void on_Button_NewDYK_clicked();

private:
    Ui::DYKEditor *ui;

    widgetContainerStorage cp_wsc;

    QStringList dykList;
    void fillListWidget();
};

#endif // DYKEDITOR_H
