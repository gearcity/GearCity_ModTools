#ifndef GCVIEDITOR_H
#define GCVIEDITOR_H

#include <QtWidgets/QWidget>
#include "BaseContainerStorage.h"
#include <QtWidgets/QListWidgetItem>

namespace Ui {
class GCVIEditor;
}

class GCVIEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit GCVIEditor(widgetContainerStorage wsc, QWidget *parent = 0);
    ~GCVIEditor();

private slots:
    void on_Button_new_clicked();

    void on_button_open_clicked();

    void on_button_save_clicked();

    void on_button_ReturnToMain_clicked();

    void on_toolButton_loadSceneFile_clicked();

    void on_toolButton_Category_SelectImageFile_clicked();

    void on_pushButton_AssignModel_clicked();

    void on_listView_Models_Assigned_pressed(const QModelIndex &index);

    void on_pushButton_RemoveModel_clicked();

    void on_toolButton_Model_SelectImageFile_clicked();

    void on_toolButton_SubCategory_SelectImageFile_clicked();

    void on_pushButton_Animation_AssignAnimation_clicked();

    void on_listView_Animations_AssignedAnimations_clicked(const QModelIndex &index);

    void on_pushButton_Animation_RemoveAnimation_clicked();

    void on_listView_Presets_PresetList_clicked(const QModelIndex &index);

    void on_pushButton_Preset_CreatePreset_clicked();

    void on_pushButton_Preset_DeletePreset_clicked();

    void on_toolButton_Preset_SelectImageFile_clicked();

    void on_pushButton_Preset_AssignAnimation_clicked();

    void on_listView_Preset_AssignedAnimations_clicked(const QModelIndex &index);

    void on_pushButton_Preset_RemoveAnimation_clicked();

    void on_pushButton_Category_CreateEdit_clicked();

    void on_pushButton_Category_Delete_clicked();

    void on_lineEdit_ModelName_editingFinished();

    void on_lineEdit_ModelName_textEdited(const QString &arg1);

    void on_spinBox_Model_Localization_textChanged(const QString &arg1);

    void on_spinBox_Animations_LinkedID_textChanged(const QString &arg1);

    void on_spinBox_Model_Type_textChanged(const QString &arg1);

    void on_checkBox_ModelDefaultActive_stateChanged(int arg1);

    void on_lineEdit_SubCategory_Name_textEdited(const QString &arg1);

    void on_spinBox_SubCategory_Localization_textChanged(const QString &arg1);

    void on_lineEdit_Animation_Name_textEdited(const QString &arg1);

    void on_spinBox_Animations_Localization_textChanged(const QString &arg1);

    void on_doubleSpinBox_Animation_LowRange_textChanged(const QString &arg1);

    void on_doubleSpinBox_Animation_HighRange_textChanged(const QString &arg1);

    void on_lineEdit_Preset_Name_textEdited(const QString &arg1);

    void on_spinBox_Presets_Localization_textChanged(const QString &arg1);

    void on_spinBox_Presets_LinkedID_textChanged(const QString &arg1);

    void on_doubleSpinBox_Preset_AnimationValue_textChanged(const QString &arg1);

private:
    Ui::GCVIEditor *ui;

    widgetContainerStorage cp_wsc;

    bool loadSceneFile(QString filePath, QString filename);
    void saveXML(QString saveFileName);
    bool ConvertMeshFiles(QString filePath, QString meshFilename);
    bool ReadMeshFiles(QString meshFilename);

    class GCVIData
    {
        public:
        QString name;
        QString type;
        QString subtype;
        QString sceneFile;

        class GCVICategories
        {
            public:

            class GCVIModelOptions
            {
                public:

                class GCVIPreset
                {
                    public:
                    class GCVIPresetAnimations
                    {
                        public:
                         QString morphName;
                         QString LinkID;
                         QString animationValue;
                    };

                     QString name;
                     QString image;
                     QString localize;

                     QVector<GCVIPresetAnimations> presetAnimations;
                };

                class GCVIAnimation
                {
                    public:
                     QString name;
                     QString image;
                     QString localize;
                     QString lowRange;
                     QString highRange;
                     QString LinkID;
                     QString animationName;

                };

                QString name;
                QString image;
                QString localize;
                QString type;
                QString nodeName;
                QString defaultActive;
                QString EntityName;

                QString subName;
                QString subImage;
                QString subLocalize;

                QVector<GCVIAnimation> animations;
                QVector<GCVIPreset> presets;

            };

            QString name;
            QString image;
            QString localize;

            QVector<GCVIModelOptions> models;
        };


        QVector<GCVICategories> guiData;


    };

    GCVIData* gData;
    QMultiMap<QString,QString> ModelAnimationMap;

};

#endif // GCVIEDITOR_H
