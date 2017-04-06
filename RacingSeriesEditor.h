#ifndef RACINGSERIESEDITOR_H
#define RACINGSERIESEDITOR_H

#include <QWidget>
#include "BaseContainerStorage.h"

namespace Ui {
class RacingSeriesEditor;
}

class RacingSeriesEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit RacingSeriesEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~RacingSeriesEditor();
    
private slots:
    void on_button_add_clicked();

    void on_button_remove_clicked();

    void on_table_series_cellClicked(int row, int column);

    void on_button_new_clicked();

    void on_button_open_clicked();

    void on_button_save_clicked();

    void on_button_return_clicked();

private:
    class seriesData
    {
        public:
            QString name, type, cartype, country;
            int id, supply, ccrestrict, start, end, birth, death, tracks, teams, manuInvolve;
            int localName, localDesc,localParam;
            float funding, popularity;

    };

    Ui::RacingSeriesEditor *ui;
    QList<seriesData> seriesList;

    widgetContainerStorage cp_wsc;

    void fillTable();
};

#endif // RACINGSERIESEDITOR_H
