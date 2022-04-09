#ifndef MUSICEDITOR_H
#define MUSICEDITOR_H

#include <QtWidgets/QWidget>
#include "BaseContainerStorage.h"
#include <QMap>

namespace Ui {
class MusicEditor;
}

class MusicEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit MusicEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~MusicEditor();
    
private slots:


    void on_button_selectMusicFolder_clicked();

    void on_button_add_clicked();

    void on_button_remove_clicked();

    void on_table_music_cellClicked(int row, int column);

    void on_button_newList_clicked();

    void on_button_openList_clicked();

    void on_button_saveList_clicked();

    void on_button_ReturnToMain_clicked();    

    void sectionDoubleClickedSlot(int index);

private:
    class MusicData
    {
        public:
        int startYear, endYear, use;
        QString Artist, Name, File, Comments;
    };

    Ui::MusicEditor *ui;
    widgetContainerStorage cp_wsc;
    QMap<QString, MusicData> musicMap;

    void fillTable();
};

#endif // MUSICEDITOR_H
