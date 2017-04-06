#ifndef NEWSPAPERARTICLEEDITOR_H
#define NEWSPAPERARTICLEEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"

namespace Ui {
class NewspaperArticleEditor;
}

class NewspaperArticleEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit NewspaperArticleEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~NewspaperArticleEditor();
    
private slots:
    void on_button_addArticle_clicked();

    void on_button_removeArticle_clicked();

    void on_table_Newspaper_cellClicked(int row, int column);

    void on_button_newNewspaper_clicked();

    void on_button_OpenNewspaper_clicked();

    void on_button_SaveNewspaper_clicked();

    void on_button_ReturnToPrevious_clicked();

private:
    class articleData
    {
        public:
            int localized;
            int startingYear;
            int endingYear;
            QString article;
    };

    Ui::NewspaperArticleEditor *ui;
    QVector<articleData> articles;
    widgetContainerStorage cp_wsc;

    void fillArticleTable();
};

#endif // NEWSPAPERARTICLEEDITOR_H
