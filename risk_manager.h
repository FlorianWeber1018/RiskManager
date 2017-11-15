#ifndef RISK_MANAGER_H
#define RISK_MANAGER_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include "editseletableriskreduction.h"

#define colDamageExtent 5
#define colDamageProbability 6
#define colKat1 7
#define colKat2 8
#define colKat3 9

#define colName 0
#define colId 1
#define colEndangering 2
#define colEvent 3
#define colRiskReduction 4

#define colCheck 2
#define colCheckedBoxesStart 2

#define colCondition 2
#define colsrc 3
#define coldest 4

static const int mtx[4][5]={{1,1,1,1,2},{1,1,1,2,3},{1,2,2,3,3},{2,2,3,3,3}};


#define IdSetting "S"
#define IdRisk "R"
#define IdGroup "G"
#define IdRiskReduction "M"
#define IdLifecycle "L"
#define IdCheckbox "C"
#define IdLink "V"
#define SettingDefaultRisk "S1.1.1"
#define SettingDefaultGroup "S1.2.1"
#define SettingDefaultRiskReduction "S1.3.1"
#define SettingDefaultLifecycle "S1.4.1"
#define SettingDefaultCheckbox "S1.5.1"
#define SettingDefaultLink "S1.6.1"

enum operation
{
    OR, AND
};


namespace Ui {
class Risk_Manager;
}

class Risk_Manager : public QMainWindow
{
    Q_OBJECT


public:
    explicit Risk_Manager(QWidget *parent = 0);
    ~Risk_Manager();

private slots:



    void on_data_edit_button_add_group_clicked();

    void on_data_edit_button_remove_clicked();


    void on_data_edit_button_add_risk_clicked();

    void on_data_edit_dataTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_data_edit_button_open_clicked();


    void on_data_edit_button_save_clicked();

    void on_data_edit_button_link_risk_to_group_clicked();

    bool event(QEvent *event);
    void on_data_edit_button_save_as_clicked();



    void on_doc_edit_button_open_document_clicked();

    void on_doc_edit_button_save_document_clicked();

    void on_doc_edit_button_save_as_document_clicked();

    void on_doc_edit_treeWidget_database_itemClicked(QTreeWidgetItem *item, int column);

    void on_data_edit_button_add_lifecycle_clicked();

    void on_doc_edit_button_remove_clicked();

    void on_doc_edit_button_link_risk_to_lifecycle_clicked();

    void on_doc_edit_treeWidget_document_itemClicked(QTreeWidgetItem *item, int column);

    void on_data_edit_dataTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_doc_edit_button_plot_pdf_clicked();

    void on_data_edit_button_add_risk_reduction_clicked();

    void on_data_edit_dataTree_itemChanged(QTreeWidgetItem *item, int column);



    void on_data_edit_button_add_checkbox_clicked();

    void on_data_edit_button_add_link_clicked();

    void on_doc_edit_button_add_lifecycle_clicked();

    void on_pushButton_2_clicked();

    void on_doc_edit_button_add_risks_from_rules_clicked();

    void on_doc_edit_button_set_checklist_clicked();

private:
    void expandPathToItem(QTreeWidgetItem* item, QTreeWidget* tree);
    void collapsPathToItem(QTreeWidgetItem* item, QTreeWidget* tree);
    QTreeWidgetItem* copy_root_Item_to_dest_tree(QTreeWidgetItem* src, QTreeWidget* tree);
    void show_editSeletableRiskReduction(QTreeWidgetItem* item);
    void show_editDeviceSpecifications();
    QTreeWidgetItem* AddRoot(QTreeWidget* tree, QString name, QString Id);
    QTreeWidgetItem* AddChild(QTreeWidgetItem *parent, QString name, QString Id);
    bool data_edit_open(QString filename);
    bool data_edit_save(QString fileName);
    bool syncDatabases=true;
    Ui::Risk_Manager *ui;
    bool strg_pressed;
    void UpDnHandler(bool dir);
    void data_edit_clear_selection();
    void doc_edit_clear_selection();
    QString read_file(QString pathcomplete);
    bool write_file(QString data, QString pathcomplete);

    void copyallcols(QTreeWidgetItem *dest, QTreeWidgetItem *src);

    QString determineNextAvId(QTreeWidgetItem* parent);
    QString determineNextAvId(QTreeWidgetItem* parent, QString itemType);

    QList<QTreeWidgetItem*> searchItems(QList<QTreeWidgetItem*> ItemListcomplete, QString searchString, Qt::MatchFlag matchcondition, int col);
    bool addNewTreeWidgetItem(QTreeWidget* tree, QString ItemType, QTreeWidgetItem* Itemdefault);
    int getKat(int DamageExtent, int DamageProbability);
    void removeAllItemsWhosMatch(QTreeWidget* tree, QList<QString> searchList, Qt::MatchFlag matchcondition, int searchcol);
    QString removeAllCharWhosMatch(QString input, char m_char);
    QTreeWidgetItem* copy_leaf_source_to_destination(QTreeWidgetItem* src, QTreeWidgetItem* dest);
    QTreeWidgetItem* copy_branch_source_to_destination(bool resolvebranch, QTreeWidgetItem* branch, QTreeWidgetItem* dest);
    QTreeWidgetItem* copy_dist_branch_source_to_destination(bool resolvebranch, QTreeWidgetItem* branch, QTreeWidgetItem* dest);
    void copy_list(QList<QTreeWidgetItem*> src, QList<QTreeWidgetItem*> dest, bool resolve_src);
    void copy_dist_list(QList<QTreeWidgetItem*> src, QList<QTreeWidgetItem*> dest, bool resolve_src);
    bool resolveCondition(QString Condition);
    QList<QString> convertConditionStringToList(QString Condition);
    bool resolveOperation(bool input0, operation m_operation, bool input1);
    QString resolveOperation(QString input0, QString m_operation, QString input1);
    bool determineCheckstate(QString Checkbox);
    void validateCondition(QList<QString>* Condition);
    void validateSrc(QList<QString>* Src);
    void validateDest(QList<QString>* Dest);
    bool elementIdExistInDb(QString Id);

};

QList<QString> StringToList(QString inputString, char seperator);
QString StringListToString(QList<QString> inputList, QString separator);
bool cmpStrings(QString A, QString B, int startA, int endA, int Boffset);
QTreeWidgetItem* find_child(QTreeWidgetItem* ChildToFind, QTreeWidgetItem* ItemWithChildsToCheck);
#endif // RISK_MANAGER_H
