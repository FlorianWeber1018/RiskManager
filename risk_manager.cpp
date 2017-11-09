#include "risk_manager.h"
#include "ui_risk_manager.h"
#include "qtreewidget_toxml.h"
#include "editseletableriskreduction.h"
#include "editdevicespecifications.h"
#include "qtreewidget_totextable.h"
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>
#include <QFileDialog>
#include <QString>
#include <string>
#include <QDir>



Risk_Manager::Risk_Manager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Risk_Manager)
{
    this->strg_pressed=false;

    ui->setupUi(this);
    ui->data_edit_dataTree->setSortingEnabled(true);
    ui->data_edit_dataTree->setColumnWidth(0,250);
    ui->data_edit_dataTree->setColumnWidth(1,20);
    ui->data_edit_dataTree->setColumnWidth(2,200);
    ui->data_edit_dataTree->setColumnWidth(3,200);
    ui->data_edit_dataTree->setColumnWidth(4,200);

    ui->doc_edit_treeWidget_database->setColumnWidth(0,200);

    ui->doc_edit_treeWidget_document->setColumnWidth(0,250);
    ui->doc_edit_treeWidget_document->setColumnWidth(1,50);
    ui->doc_edit_treeWidget_document->setColumnWidth(2,200);
    ui->doc_edit_treeWidget_document->setColumnWidth(3,250);
    ui->doc_edit_treeWidget_document->setColumnWidth(4,250);
    ui->doc_edit_treeWidget_document->setColumnWidth(5,100);
    ui->doc_edit_treeWidget_document->setColumnWidth(6,150);
    ui->doc_edit_treeWidget_document->setColumnWidth(7,35);
    ui->doc_edit_treeWidget_document->setColumnWidth(8,35);
    ui->doc_edit_treeWidget_document->setColumnWidth(9,35);
}

QTreeWidgetItem* Risk_Manager::AddRoot(QTreeWidget* tree, QString name, QString Id)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(tree);
    itm->setText(0,name);
    itm->setText(1,Id);
    tree->addTopLevelItem(itm);
    return itm;
}
QTreeWidgetItem* Risk_Manager::AddChild(QTreeWidgetItem *parent, QString name, QString Id)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    itm->setText(0,name);
    itm->setText(1,Id);
    itm->setFlags(itm->flags() | Qt::ItemIsEditable);
    parent->addChild(itm);

    return itm;
}
void Risk_Manager::copyallcols(QTreeWidgetItem *dest, QTreeWidgetItem *src){
    for(int i=0;i<src->columnCount();i++){
        dest->setText(i,src->text(i));
    }
}
Risk_Manager::~Risk_Manager()
{
    delete ui;
}

bool Risk_Manager::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if(!ke->isAutoRepeat()){
            if(ke->key() == Qt::Key_Delete) {
                if(ui->tabWidget->currentIndex()==0) on_data_edit_button_remove_clicked();
                return true;
            }
            if(ke->key() == Qt::Key_Insert) {
                if(ui->tabWidget->currentIndex()==0) on_data_edit_button_link_risk_to_group_clicked();
                return true;
            }
            if (ke->key() == Qt::Key_Control) {
                this->strg_pressed=true;
                return true;
            }

            if(ke->key() == Qt::Key_Escape){
                if(ui->tabWidget->currentIndex()==0) data_edit_clear_selection();
                if(ui->tabWidget->currentIndex()==1) doc_edit_clear_selection();
                return true;
            }
        }
        if (ke->key() == Qt::Key_F1) {
            UpDnHandler(true);
            return true;
        }
        if (ke->key() == Qt::Key_F2) {
            UpDnHandler(false);
            return true;
        }
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Control) {
                this->strg_pressed=false;
            return true;
        }
    }

    return QWidget::event(event);
}
void Risk_Manager::UpDnHandler(bool dir){//dir=false->up dir=true->down
    QMessageBox msgBox;
    if(ui->tabWidget->currentIndex()==0){
        QList<QTreeWidgetItem*> selectedItems = ui->data_edit_dataTree->selectedItems();
        if(selectedItems.count()==1){
            QTreeWidgetItem* item_selected = selectedItems.first();
            QTreeWidgetItem* parent = item_selected->parent();
            int index_selected = parent->indexOfChild(item_selected);
            if(dir){
                if(index_selected==0){
                    return;
                }else{
                     parent->insertChild(index_selected-1,parent->takeChild(index_selected));
                     parent->child(index_selected-1)->setSelected(true);
                }
            }else{
                if(index_selected==parent->childCount()-1){
                    return;
                }else{
                     parent->insertChild(index_selected+1,parent->takeChild(index_selected));
                     parent->child(index_selected+1)->setSelected(true);
                }
            }
        }else{
            msgBox.setText("Verschieben nur möglich wenn genau ein Element selektiert ist");
            msgBox.exec();
            return;
        }
    }
}
void Risk_Manager::show_editSeletableRiskReduction(QTreeWidgetItem* item){
    editSeletableRiskReduction* window = new editSeletableRiskReduction(this);
    window->alignTargetItem(item);
    QTreeWidget* tree = window->getTree();
    QString idRoot=IdRiskReduction;
    idRoot.append("0");
    QTreeWidgetItem* srcRootItem=nullptr;
    QList<QTreeWidgetItem*> listSrcRootItem=ui->data_edit_dataTree->findItems(idRoot, Qt::MatchExactly, colId);
    if(!listSrcRootItem.isEmpty()){
        srcRootItem=listSrcRootItem.first();
    }
    if(srcRootItem!=nullptr){
        copy_root_Item_to_dest_tree(srcRootItem, tree);
    }
    QList<QTreeWidgetItem*>listRiskReductionMatchsCurrentTraget = tree->findItems(item->text(colRiskReduction),Qt::MatchExactly | Qt::MatchRecursive ,colRiskReduction );
    if(!listRiskReductionMatchsCurrentTraget.isEmpty()){
        listRiskReductionMatchsCurrentTraget.first()->setSelected(true);
        if(listRiskReductionMatchsCurrentTraget.first()->parent()!=nullptr){
            tree->expandItem(listRiskReductionMatchsCurrentTraget.first()->parent());
        }

    }


    window->setWindowModality(Qt::WindowModal);
    window->show();
}
void Risk_Manager::show_editDeviceSpecifications(){

    QMessageBox msgBox;
    editDeviceSpecifications* window = new editDeviceSpecifications(this);
    window->alignTargetTree(ui->doc_edit_treeWidget_document);
    QTreeWidget* tree = window->getTree();
    QString IdRoot=IdCheckbox;
    IdRoot.append("0");
    QTreeWidgetItem* srcRootItem=nullptr;
    QList<QTreeWidgetItem*> listSrcRootItem=ui->data_edit_dataTree->findItems(IdRoot, Qt::MatchExactly, colId);

    if(!listSrcRootItem.isEmpty()){
        srcRootItem=listSrcRootItem.first();
    }else{
        return;
    }

    if(srcRootItem!=nullptr){
        copy_root_Item_to_dest_tree(srcRootItem, tree);
    }else{
        return;
    }
    QList<QTreeWidgetItem*>listRootItemCheckboxDoc = ui->doc_edit_treeWidget_document->findItems(IdRoot, Qt::MatchExactly, colId);

    if(!listRootItemCheckboxDoc.isEmpty()){
        QList<QString> allCheckedIds;
        QTreeWidgetItem* RootItemCheckboxDoc=listRootItemCheckboxDoc.first();
        for(int i=colCheckedBoxesStart; i<RootItemCheckboxDoc->columnCount(); i++){
            if(!RootItemCheckboxDoc->text(i).isEmpty()){
                allCheckedIds.append(RootItemCheckboxDoc->text(i));
            }else{
                break;
            }
        }
        QList<QTreeWidgetItem*> allCheckboxItemsInTree = tree->findItems(IdCheckbox, Qt::MatchStartsWith | Qt::MatchRecursive, colId);
        foreach (QTreeWidgetItem* item, allCheckboxItemsInTree) {
            foreach(QString cmp, allCheckedIds){
                if(cmpStrings(cmp, item->text(colId), 0, cmp.length()-1, 0)){
                    item->setText(colCheck, "X");
                    allCheckedIds.removeAll(cmp);
                }
            }
        }
        if(!allCheckedIds.isEmpty()){
            QString msgtxt="Es wurden checkboxen im dokument gefunden, welche nicht in der geöffneten Datenbank vorhanden sind.\n";
            msgtxt.append("Beim Klicken auf \"OK\" werden folgende Checkboxen aus dem Dokument entfernt:\n\n");
            foreach (QString leftItem, allCheckedIds) {
                msgtxt.append(leftItem);
                msgtxt.append("\n");
            }
            msgBox.setText(msgtxt);
            msgBox.exec();
        }

    }
    window->setWindowModality(Qt::WindowModal);
    window->show();
}
void Risk_Manager::on_data_edit_button_remove_clicked()
{
    QMessageBox msgBox;
    if(ui->data_edit_dataTree->selectedItems().isEmpty())                                                           //wurde kein Item ausgewählt?
    {
        msgBox.setText("Kein Element selektiert");
        msgBox.exec();
        return;
    }else{
        foreach(QTreeWidgetItem* item, ui->data_edit_dataTree->selectedItems()){
            if(item->parent()!=nullptr){
                delete item;
            }else{
                msgBox.setText("Root Elemente können nicht gelöscht werden");
                msgBox.exec();
            }
        }
    }
}
void Risk_Manager::on_data_edit_button_add_group_clicked()
{
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultGroup,Qt::MatchExactly | Qt::MatchRecursive, 1);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->data_edit_dataTree,IdGroup,defaultItem);
}
void Risk_Manager::on_data_edit_button_add_risk_clicked(){
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultRisk,Qt::MatchExactly | Qt::MatchRecursive, 1);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->data_edit_dataTree,IdRisk,defaultItem);
}
void Risk_Manager::on_data_edit_button_add_risk_reduction_clicked()
{
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultRiskReduction,Qt::MatchExactly | Qt::MatchRecursive, 1);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->data_edit_dataTree,IdRiskReduction,defaultItem);
}
void Risk_Manager::on_data_edit_button_add_lifecycle_clicked()
{
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultLifecycle,Qt::MatchExactly | Qt::MatchRecursive, colId);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->data_edit_dataTree,IdLifecycle,defaultItem);
}
void Risk_Manager::on_data_edit_button_add_checkbox_clicked()
{
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultCheckbox,Qt::MatchExactly | Qt::MatchRecursive, 1);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->data_edit_dataTree,IdCheckbox,defaultItem);
}
void Risk_Manager::on_data_edit_button_add_link_clicked()
{
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultLink,Qt::MatchExactly | Qt::MatchRecursive, 1);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->data_edit_dataTree,IdLink,defaultItem);
}
void Risk_Manager::on_data_edit_dataTree_itemClicked(QTreeWidgetItem *item, int column)
{

    if(cmpStrings(IdRisk, item->text(colId), 0,sizeof(IdRisk)-2,0)){
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                if(!strg_pressed){
                    foreach(QTreeWidgetItem* selecteditem, ui->data_edit_dataTree->selectedItems()){
                        if(cmpStrings(IdRisk, selecteditem->text(colId), 0,sizeof(IdRisk)-2,0)){
                            selecteditem->setSelected(false);
                        }
                    }
                }
                item->setSelected(true);
            }
        }
    }
    if(cmpStrings(IdGroup, item->text(colId), 0,sizeof(IdGroup)-2,0)){
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                if(!strg_pressed){
                    foreach(QTreeWidgetItem* selecteditem, ui->data_edit_dataTree->selectedItems()){
                        if(cmpStrings(IdGroup, selecteditem->text(colId), 0,sizeof(IdGroup)-2,0)){
                            selecteditem->setSelected(false);
                        }
                    }
                }
                item->setSelected(true);
            }
        }
    }
    if(cmpStrings(IdRiskReduction, item->text(colId), 0,sizeof(IdRiskReduction)-2,0)){
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                foreach(QTreeWidgetItem* selecteditem, ui->data_edit_dataTree->selectedItems()){
                        selecteditem->setSelected(false);
                }
                item->setSelected(true);
            }
        }
    }
    if(cmpStrings(IdLifecycle, item->text(colId), 0,sizeof(IdLifecycle)-2,0)){
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                foreach(QTreeWidgetItem* selecteditem, ui->data_edit_dataTree->selectedItems()){
                        selecteditem->setSelected(false);
                }
                item->setSelected(true);
            }
        }
    }
    if(cmpStrings(IdCheckbox, item->text(colId), 0,sizeof(IdCheckbox)-2,0)){
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                foreach(QTreeWidgetItem* selecteditem, ui->data_edit_dataTree->selectedItems()){
                        selecteditem->setSelected(false);
                }
                item->setSelected(true);
            }
        }
    }
    if(cmpStrings(IdLink, item->text(colId), 0,sizeof(IdLink)-2,0)){
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                foreach(QTreeWidgetItem* selecteditem, ui->data_edit_dataTree->selectedItems()){
                        selecteditem->setSelected(false);
                }
                item->setSelected(true);
            }
        }
    }

}
void Risk_Manager::on_data_edit_dataTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()==nullptr){
        return;
    }
    if(column==colId){
        return;
    }
    if(column>colDamageProbability){
        return;
    }
    if(cmpStrings(IdLink, item->text(colId), 0, sizeof(IdLink)-2,0)){
        if(column<=coldest){
            ui->data_edit_dataTree->editItem(item, column);
            return;
        }
    }
    if(cmpStrings(IdRisk, item->text(colId), 0, sizeof(IdRisk)-2,0)){
        if(column==colRiskReduction){
            show_editSeletableRiskReduction(item);
        }else{
            ui->data_edit_dataTree->editItem(item, column);
        }
        return;
    }
    if(cmpStrings(IdRiskReduction, item->text(colId), 0, sizeof(IdRiskReduction)-2,0)){
        if(column==colRiskReduction){
            ui->data_edit_dataTree->editItem(item, column);
        }
        if(column==colName){
            ui->data_edit_dataTree->editItem(item, column);
        }
        return;
    }
    if(cmpStrings(IdRiskReduction, item->text(colId), 0, sizeof(IdRiskReduction)-2,0)){
        if(column==colRiskReduction){
            ui->data_edit_dataTree->editItem(item, column);
        }
        return;
    }
    if(cmpStrings(IdLifecycle, item->text(colId), 0, sizeof(IdLifecycle)-2,0)){
        if(column==colName){
            ui->data_edit_dataTree->editItem(item, column);
        }
        return;
    }
    if(cmpStrings(IdGroup, item->text(colId), 0, sizeof(IdGroup)-2,0)){
        if(column==colName){
            ui->data_edit_dataTree->editItem(item, column);
        }
        return;
    }
    if(cmpStrings(IdSetting, item->text(colId), 0, sizeof(IdSetting)-2,0)){
        if(cmpStrings(SettingDefaultRisk, item->text(colId), 0, sizeof(SettingDefaultRisk)-2,0)){ //default Risiko
            if(column<=colEvent){
               ui->data_edit_dataTree->editItem(item, column);
            }
            if(column==colRiskReduction){
                show_editSeletableRiskReduction(item);
            }
        }
        if(cmpStrings(SettingDefaultGroup, item->text(colId), 0, sizeof(SettingDefaultGroup)-2,0)){ //default Group
            if(column==colName){
               ui->data_edit_dataTree->editItem(item, column);
            }
        }
        if(cmpStrings(SettingDefaultRiskReduction, item->text(colId), 0, sizeof(SettingDefaultRiskReduction)-2,0)){ //default Risiko Minderung
            if(column==colName){
               ui->data_edit_dataTree->editItem(item, column);
            }
            if(column==colRiskReduction){
                show_editSeletableRiskReduction(item);
            }
        }
        if(cmpStrings(SettingDefaultLifecycle, item->text(colId), 0, sizeof(SettingDefaultLifecycle)-2,0)){ //default Lebensphase
            if(column==colName){
               ui->data_edit_dataTree->editItem(item, column);
            }
        }
        if(cmpStrings(SettingDefaultCheckbox, item->text(colId), 0, sizeof(SettingDefaultCheckbox)-2,0)){ //default Checkbox
            if(column==colName){
               ui->data_edit_dataTree->editItem(item, column);
            }
        }
        if(cmpStrings(SettingDefaultLink, item->text(colId), 0, sizeof(SettingDefaultLink)-2,0)){ //default Verknüpfung
            if(column<=coldest){
               ui->data_edit_dataTree->editItem(item, column);
            }
        }

        return;
    }
    if(cmpStrings(IdCheckbox, item->text(colId), 0, sizeof(IdCheckbox)-2,0)){
        if(column==colName){
            ui->data_edit_dataTree->editItem(item, column);
        }
        return;
    }
}
void Risk_Manager::on_data_edit_button_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"), "", tr("RiskManagerDatabase (*.Rxmldb)"));
    if(data_edit_open(fileName)){
        QMessageBox::information(this, "Datei nicht geöffnet", "Datei konnte nicht erfolgreich geöffnet werden.");
    }else{
        QMessageBox::information(this, "Datei geöffnet", "Datei konnte erfolgreich geöffnet werden.");
    }
}
void Risk_Manager::on_data_edit_button_save_clicked()
{
     QString fileName = ui->data_edit_lineEdit_currentOpenedFile->text();
     if(fileName.isEmpty()){
         on_data_edit_button_save_as_clicked();
         return;
     }else{
         if(data_edit_save(fileName)){
             QMessageBox::information(this, "Datei nicht gespeichert", "Datei konnte nicht erfolgreich gespeichert werden.");
         }else{
             QMessageBox::information(this, "Datei gespeichert", "Datei konnte erfolgreich gespeichert werden.");
             syncDatabases=false;
                data_edit_open(fileName);
             syncDatabases=true;
         }
     }
}
void Risk_Manager::on_data_edit_button_save_as_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Database"), "", tr("RiskManagerDatabase (*.Rxmldb)"));
    ui->data_edit_lineEdit_currentOpenedFile->setText(fileName);
    on_data_edit_button_save_clicked();
}
void Risk_Manager::on_data_edit_button_link_risk_to_group_clicked(){
    QMessageBox msgBox;
    QList<QTreeWidgetItem*> select_list = ui->data_edit_dataTree->selectedItems();
    QList<QTreeWidgetItem*> src_list;
    QList<QTreeWidgetItem*> dest_list;
    foreach(QTreeWidgetItem* selectedItem, select_list){
        if(cmpStrings(IdRisk,selectedItem->text(colId),0,sizeof(IdRisk)-2,0)){
            src_list.append(selectedItem);
        }else if(cmpStrings(IdGroup,selectedItem->text(colId),0,sizeof(IdGroup)-2,0)){
            dest_list.append(selectedItem);
        }else{
            QString msgText = "Folgendes Element ist kein Risiko oder eine Gruppe und wird desshalb nicht behandelt:\n";
            msgText.append(selectedItem->text(colId));
            msgBox.setText(msgText);
            msgBox.exec();
        }
    }
    if(src_list.empty()){
        msgBox.setText("Kein Risiko selektiert");
        msgBox.exec();
        return;
    }
    if(dest_list.empty()){
        msgBox.setText("Keine Ziel Gruppe selektiert, bitte Ziel angeben");
        msgBox.exec();
        return;
    }
    copy_list(src_list, dest_list, true);
}

void Risk_Manager::on_doc_edit_button_open_document_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), "", tr("RiskManagerDocument (*.Rxmldoc)"));
    if(fileName.isEmpty()){
        return;
    }else{
        QString InString = read_file(fileName);
        qtreewidget_toxml myxmlconv;
        myxmlconv.aligntree(ui->doc_edit_treeWidget_document);
        myxmlconv.convert_xmltotree(InString);
        ui->doc_edit_lineEdit_currentOpened_document->setText(fileName);
    }
}
void Risk_Manager::on_doc_edit_button_save_document_clicked()
{
    QMessageBox msgBox;
    QString fileName = ui->doc_edit_lineEdit_currentOpened_document->text();
    if(fileName.isEmpty()){
        msgBox.setText("Dateiname ist leer, Bitte statt \"SAVE\", \"SAVE AS\" verwenden");
        msgBox.exec();
        return;
    }else{
        qtreewidget_toxml myxmlconv;
        QString outString=myxmlconv.convert_treetoxml(ui->doc_edit_treeWidget_document);
        if(write_file(outString, fileName)){
            msgBox.setText("Datei nicht erfolgreich gespeichert");
            msgBox.exec();
        }else{
            msgBox.setText("Datei erfolgreich gespeichert");
            msgBox.exec();
        }
    }
}
void Risk_Manager::on_doc_edit_button_save_as_document_clicked()
{
    QMessageBox msgBox;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Document"), "", tr("RiskManagerDatabase (*.Rxmldoc)"));
    if(fileName.isEmpty()){
        return;
    }else{
        qtreewidget_toxml myxmlconv;
        QString outString=myxmlconv.convert_treetoxml(ui->doc_edit_treeWidget_document);
        if(write_file(outString, fileName)){
            msgBox.setText("Datei nicht erfolgreich gespeichert");
            msgBox.exec();
        }else{
            msgBox.setText("Datei erfolgreich gespeichert");
            msgBox.exec();
            ui->doc_edit_lineEdit_currentOpened_document->setText(fileName);
        }
    }
}
void Risk_Manager::on_doc_edit_treeWidget_database_itemClicked(QTreeWidgetItem *item, int column)
{
        if(item->parent()!=nullptr){
            if(item->isSelected()){
                item->setSelected(false);
            }else{
                if(!strg_pressed){
                    foreach(QTreeWidgetItem* selecteditem, ui->doc_edit_treeWidget_database->selectedItems()){
                        selecteditem->setSelected(false);
                    }
                }
                item->setSelected(true);
            }
        }
}
void Risk_Manager::on_doc_edit_treeWidget_document_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->parent()!=nullptr){
        if(item->isSelected()){
            item->setSelected(false);
        }else{
            if(!strg_pressed){
                foreach(QTreeWidgetItem* selecteditem, ui->doc_edit_treeWidget_document->selectedItems()){
                    selecteditem->setSelected(false);
                }
            }
            item->setSelected(true);
        }
    }
}


void Risk_Manager::on_doc_edit_button_remove_clicked()
{
    QMessageBox msgBox;
    if(ui->doc_edit_treeWidget_document->selectedItems().isEmpty())                                                           //wurde kein Item ausgewählt?
    {
        msgBox.setText("Kein Element selektiert");
        msgBox.exec();
        return;
    }else{
        foreach(QTreeWidgetItem* item, ui->doc_edit_treeWidget_document->selectedItems()){
            if(item->parent()!=nullptr){
                delete item;
            }else{
                msgBox.setText("Root Elemente können nicht gelöscht werden");
                msgBox.exec();
            }
        }
    }
}
void Risk_Manager::on_doc_edit_button_add_lifecycle_clicked()
{
    QList<QTreeWidgetItem*> listFoundedSettings = ui->data_edit_dataTree->findItems(SettingDefaultLifecycle,Qt::MatchExactly | Qt::MatchRecursive, colId);
    QTreeWidgetItem* defaultItem = nullptr;
    if(!listFoundedSettings.isEmpty()){
        defaultItem=listFoundedSettings.first();
    }
    addNewTreeWidgetItem(ui->doc_edit_treeWidget_document,IdLifecycle,defaultItem);
}
void Risk_Manager::on_doc_edit_button_link_risk_to_lifecycle_clicked()
{
    QMessageBox msgBox;
    QList<QTreeWidgetItem*> src_list = ui->doc_edit_treeWidget_database->selectedItems();
    QList<QTreeWidgetItem*> dest_list = ui->doc_edit_treeWidget_document->selectedItems();

    foreach(QTreeWidgetItem* item, dest_list){
        if(!cmpStrings(IdLifecycle,item->text(colId),0,sizeof(IdLifecycle)-2,0)){
            dest_list.removeOne(item);
        }
    }
    foreach(QTreeWidgetItem* item, src_list){
        if(!(cmpStrings(IdRisk,item->text(colId),0,sizeof(IdRisk)-2,0)||cmpStrings(IdGroup,item->text(colId),0,sizeof(IdGroup)-2,0))){
            src_list.removeOne(item);
        }
    }
    if(dest_list.empty()){
        msgBox.setText("Kein gültiges Ziel Element im Dokument Baum (Rechts) selektiert, Kopiervorgang wird abgebrochen");
        msgBox.exec();
        return;
    }
    if(src_list.empty()){
        msgBox.setText("Kein gültiges Risiko im Datenbank Baum (Links) selektiert, Kopiervorgang wird abgebrochen");
        msgBox.exec();
        return;
    }


    copy_list(src_list, dest_list, true);
}

void Risk_Manager::on_doc_edit_button_plot_pdf_clicked()
{
    QMessageBox msgBox;
    qtreewidget_totextable mytexconv;
    QString texString = "";
    texString.append(read_file(QCoreApplication::applicationDirPath()+"/Head.tex"));

    texString.append(mytexconv.convert_treetotextable(ui->doc_edit_treeWidget_document));
    texString.append(read_file(QCoreApplication::applicationDirPath()+"/Feet.tex"));


    QString Path_Rxmldoc = ui->doc_edit_lineEdit_currentOpened_document->text();
    QString Workdir=Path_Rxmldoc;
    QString Path_tex=Path_Rxmldoc;
    while(Workdir.length()>0){
        if(Workdir[Workdir.length()-1]!='/'){
            Workdir=Workdir.left(Workdir.length()-1);
        }else{
            Workdir=Workdir.left(Workdir.length()-1);
            break;
        }
    }
    while(Path_tex.length()>0){
        if(Path_tex[Path_tex.length()-1]!='.'){
            Path_tex=Path_tex.left(Path_tex.length()-1);
        }else{
            break;
        }
    }
    if(Workdir.isEmpty()||Path_tex.isEmpty()){
        msgBox.setText("falscher dateiname  des docRxml files konnte \".\" oder \"\\\" nicht finden");
        msgBox.exec();
        return;
    }else{
        Path_tex.append("tex");
        if(write_file(texString, Path_tex)){
            msgBox.setText("tex datei konnte nicht gespeichert werden");
            msgBox.exec();
        }

        QProcess *proc = new QProcess;
        QString compiler_cmd="xelatex -output-directory " + Workdir + " " + Path_tex;
        proc->start(compiler_cmd);
        proc->waitForFinished();

        msgBox.setText("PDF Datei wurde gespeichert\ncompiler Ausgabe:" + proc->readAllStandardOutput());
        msgBox.exec();

    }

}


QList<QTreeWidgetItem*> Risk_Manager::searchItems(QList<QTreeWidgetItem*> ItemListcomplete, QString searchString, Qt::MatchFlag matchcondition, int col){
    QList<QTreeWidgetItem*> returnList;
    foreach (QTreeWidgetItem* item, ItemListcomplete) {
        if(matchcondition==Qt::MatchExactly){
            if(item->text(col)==searchString){
                returnList.append(item);
            }
        }
        if(matchcondition==Qt::MatchStartsWith){
            if(cmpStrings(searchString, item->text(col),0,searchString.length()-1,0)){
                returnList.append(item);
            }
        }
        if(matchcondition==Qt::MatchEndsWith){
            if(cmpStrings(searchString, item->text(col), 0, searchString.length()-1, item->text(col).length()-searchString.length())){
                returnList.append(item);
            }
        }
    }
    return returnList;
}
QString Risk_Manager::determineNextAvId(QTreeWidgetItem* parent){
    if(parent==nullptr){
        return "";
    }
    QString parentId = parent->text(colId);
    QString parentIdType="";
    foreach(QChar Char, parentId){
        if(Char.isNumber()){
            break;
        }else{
            parentIdType.append(Char);
        }
    }
    return determineNextAvId(parent, parentIdType);
}
QString Risk_Manager::determineNextAvId(QTreeWidgetItem* parent, QString itemType){
    if(parent==nullptr){
        return "";
    }
    QString ID="";
    if(itemType==""){
        return ID;
    }
    QString parentId=parent->text(colId);
    QList<QTreeWidgetItem*> childItems;
    for(int i=0; i<parent->childCount(); i++){
        childItems.append(parent->child(i));
    }
    QList<QTreeWidgetItem*> listItems = searchItems(childItems, itemType, Qt::MatchStartsWith, colId);
    if(parent->parent()==nullptr){
        ID=itemType;
    }else{
        ID=parentId.append(".");
    }
    int currentItemIdNumber=1;
    while(!searchItems(listItems, ID + QString::number(currentItemIdNumber), Qt::MatchExactly, colId).isEmpty()){
        currentItemIdNumber++;
    }
    return ID.append(QString::number(currentItemIdNumber));
}
QString Risk_Manager::read_file(QString pathcomplete){

    QFile file(pathcomplete);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, "Unable to open file" + pathcomplete, file.errorString());
        return "";
    }
    QTextStream in(&file);
    QString InString = in.readAll().toLocal8Bit();

    return InString;
}
bool Risk_Manager::write_file(QString data, QString pathcomplete){

    QFile file(pathcomplete);
    if(!file.open(QIODevice::WriteOnly)){
        return true;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_9);
    if(out.writeRawData(data.toUtf8(), data.toUtf8().length())!=-1){
        file.close();
        return false;
    }
    return true;
}

bool Risk_Manager::data_edit_open(QString fileName){
    syncDatabases=false;
    if(fileName.isEmpty()){
        syncDatabases=true;
        return true;
    }else{
        QString InString = read_file(fileName);
        qtreewidget_toxml myxmlconv;
        myxmlconv.aligntree(ui->data_edit_dataTree);
        myxmlconv.convert_xmltotree(InString);
        ui->data_edit_lineEdit_currentOpenedFile->setText(fileName);
        myxmlconv.aligntree(ui->doc_edit_treeWidget_database);
        myxmlconv.convert_xmltotree(InString);
        QList<QString>removeList;
        removeList.append("S");
        removeList.append("M");
        removeList.append("L");
        removeList.append("C");
        removeList.append("V");
        removeAllItemsWhosMatch(ui->doc_edit_treeWidget_database, removeList, Qt::MatchStartsWith, colId);
    }
    syncDatabases=true;
    return false;
}
bool Risk_Manager::data_edit_save(QString fileName){
    if(fileName.isEmpty()){
        return true;
    }
    qtreewidget_toxml myxmlconv;
    QString outString=myxmlconv.convert_treetoxml(ui->data_edit_dataTree);
    if(write_file(outString, fileName)){
        return true;
    }else{
        return false;
    }

}

QTreeWidgetItem* Risk_Manager::copy_root_Item_to_dest_tree(QTreeWidgetItem* src, QTreeWidget* tree){
    //kopiert nur wenn die id nicht bereits vorhanden ist
    QTreeWidgetItem* newItem;
    QList<QTreeWidgetItem*> listAvRootItems = tree->findItems(src->text(colId), Qt::MatchExactly, colId);
    if(listAvRootItems.isEmpty()){
        newItem = AddRoot(tree, "", "");
        copyallcols(newItem, src);
    }else{
        newItem=listAvRootItems.first();
    }

    for(int i=0; i < src->childCount();i++){
        copy_dist_branch_source_to_destination(false, src->child(i), newItem);
    }

    return newItem;
}
QTreeWidgetItem* Risk_Manager::copy_leaf_source_to_destination(QTreeWidgetItem* src, QTreeWidgetItem* dest){
    bool alreadythere=false;
    QTreeWidgetItem* newItem;
    for(int i=0; i < dest->childCount(); i++){
        if(dest->child(i)->text(colId)==src->text(colId)){
            alreadythere=true;
        }
    }

    if(alreadythere){

    }else{
        newItem = AddChild(dest,"","");
        copyallcols(newItem, src);
    }

    return newItem;
}
QTreeWidgetItem* Risk_Manager::copy_branch_source_to_destination(bool resolvebranch, QTreeWidgetItem* branch, QTreeWidgetItem* dest)
{
    QTreeWidgetItem* newItem;
    if(branch->childCount()>0){
        if(resolvebranch){

        }else{
            newItem = copy_leaf_source_to_destination(branch, dest);
        }

        for(int i=0;i < branch->childCount();i++){
            copy_branch_source_to_destination(resolvebranch, branch->child(i), newItem);
        }

    }else{
        newItem = copy_leaf_source_to_destination(branch ,dest);
    }
    return newItem;
}
void Risk_Manager::copy_list(QList<QTreeWidgetItem*> src, QList<QTreeWidgetItem*> dest, bool resolve_src){

    foreach (QTreeWidgetItem* srcItem, src) {
        foreach (QTreeWidgetItem* destItem, dest) {
            copy_branch_source_to_destination(resolve_src, srcItem, destItem);
        }
    }

}
void Risk_Manager::copy_dist_list(QList<QTreeWidgetItem*> src, QList<QTreeWidgetItem*> dest, bool resolve_src){

    foreach (QTreeWidgetItem* srcItem, src) {
        foreach (QTreeWidgetItem* destItem, dest) {
            copy_dist_branch_source_to_destination(resolve_src, srcItem, destItem);
        }
    }

}
QTreeWidgetItem* Risk_Manager::copy_dist_branch_source_to_destination(bool resolvebranch, QTreeWidgetItem* branch, QTreeWidgetItem* dest){
    QTreeWidgetItem* newItem;
    if(branch->childCount()>0){
        if(resolvebranch){

        }else{
            newItem = find_child(branch, dest);
            if(newItem == nullptr){
                newItem = copy_leaf_source_to_destination(branch ,dest);
            }
        }
        for(int i=0;i < branch->childCount();i++){
            copy_branch_source_to_destination(resolvebranch, branch->child(i), newItem);
        }
    }else{
        newItem = find_child(branch, dest);
        if(newItem == nullptr){
            newItem = copy_leaf_source_to_destination(branch ,dest);
        }
    }
    return newItem;
}
bool Risk_Manager::addNewTreeWidgetItem(QTreeWidget* tree, QString ItemType, QTreeWidgetItem* defaultItem){
    QTreeWidgetItem *parent=nullptr;
    if(tree->selectedItems().isEmpty())                                                           //wurde kein Item ausgewählt?
    {
        QList<QTreeWidgetItem*> GroupRootItems = tree->findItems(ItemType + "0",Qt::MatchExactly,colId);
        if(GroupRootItems.isEmpty()){
            return true;
        }
        parent=GroupRootItems.first();
    }else{
        QList<QTreeWidgetItem*> selectedItems = tree->selectedItems();
        selectedItems = searchItems(selectedItems, ItemType, Qt::MatchStartsWith, colId);
        if(selectedItems.isEmpty()){
            return true;
        }else{
            parent=selectedItems.first();
        }
    }



    QString ID=determineNextAvId(parent);
    if(ID==""){
        return 1;
    }


    if(defaultItem!=nullptr){
        QString backupId=defaultItem->text(colId);
        defaultItem->setText(colId, ID);
        copy_leaf_source_to_destination(defaultItem, parent);
        defaultItem->setText(colId, backupId);
    }else{
        AddChild(parent,"- newItemOfType: " + ItemType + " -", ID);
    }
    parent->setExpanded(true);
    return 0;
}
void Risk_Manager::removeAllItemsWhosMatch(QTreeWidget* tree, QList<QString> searchList, Qt::MatchFlag matchcondition, int searchcol){
    QList<QTreeWidgetItem*> removeList;
    foreach(QString searchString, searchList){
        removeList.append(tree->findItems(searchString, matchcondition, searchcol));
    }
    while(!removeList.isEmpty()){
        QTreeWidgetItem* temp = removeList.takeFirst();
        delete temp;
        while(removeList.contains(temp)){
            removeList.removeAt(removeList.indexOf(temp));
        }
    }
}
QString Risk_Manager::removeAllCharWhosMatch(QString input, char m_char){
    for(int i=0; i < input.length(); i++){
        if(input[i]==m_char){
            QString temp=input.left(i);
            temp.append(input.right(input.length()-(i+1)));
            input=temp;
        }
    }
    return input;
}
void Risk_Manager::data_edit_clear_selection(){
    foreach(QTreeWidgetItem* item, ui->data_edit_dataTree->selectedItems()){
        item->setSelected(false);
    }
}
void Risk_Manager::doc_edit_clear_selection(){
    foreach(QTreeWidgetItem* item, ui->doc_edit_treeWidget_database->selectedItems()){
        item->setSelected(false);
    }
    foreach(QTreeWidgetItem* item, ui->doc_edit_treeWidget_document->selectedItems()){
        item->setSelected(false);
    }
}


void Risk_Manager::on_data_edit_dataTree_itemChanged(QTreeWidgetItem *item, int column)
{
    if(syncDatabases){
        if(column==colDamageExtent||column==colDamageProbability){
            int Kat = getKat(item->text(colDamageExtent).toInt(), item->text(colDamageProbability).toInt());
            if(Kat==3){
                item->setText(colKat1, "");
                item->setText(colKat2, "");
                item->setText(colKat3, "x");
            }
            if(Kat==2){
                item->setText(colKat1, "");
                item->setText(colKat2, "x");
                item->setText(colKat3, "");
            }
            if(Kat==1){
                item->setText(colKat1, "x");
                item->setText(colKat2, "");
                item->setText(colKat3, "");
            }
        }
        QList<QTreeWidgetItem*> listItemToChange = ui->data_edit_dataTree->findItems(item->text(colId),Qt::MatchExactly | Qt::MatchRecursive,colId);
        listItemToChange = searchItems(listItemToChange, IdRisk, Qt::MatchStartsWith, colId);
        foreach(QTreeWidgetItem* itemToChange, listItemToChange){
            copyallcols(itemToChange, item);
        }
        if(cmpStrings(IdLink, item->text(colId), 0, sizeof(IdLink)-2,0)){ // Verknüpfung
            if(column == colCondition){
                QList<QString> m_Condition_list = convertConditionStringToList(item->text(column));
                validateCondition(&m_Condition_list);
                item->setText(column, StringListToString(m_Condition_list, ""));
            }
            if(column == colsrc){
                QList<QString> m_src_list = StringToList(item->text(column), ' ');
                validateSrc(&m_src_list);
                item->setText(column, StringListToString(m_src_list, " "));
            }
            if(column == coldest){
                QList<QString> m_dest_list = StringToList(item->text(column), ' ');
                validateDest(&m_dest_list);
                item->setText(column, StringListToString(m_dest_list, " "));
            }
        }
    }
}

int Risk_Manager::getKat(int DamageExtent, int DamageProbability){
    if(DamageExtent >= 1 && DamageExtent <=4 && DamageProbability >=1 && DamageProbability <= 5){
            static const int mtx[4][5]={{1,1,1,1,2},{1,1,1,2,3},{1,2,2,3,3},{2,2,3,3,3}};
            return mtx[DamageExtent-1][DamageProbability-1];
    }
    return 0;
}

void Risk_Manager::validateCondition(QList<QString>* Condition){
    for(int i=0; i<Condition->length(); i++){
        QString m_str = Condition->at(i);
        m_str = removeAllCharWhosMatch(m_str, ' ');
        Condition->removeAt(i);
        Condition->insert(i, m_str);
        if(m_str=="|" || m_str=="&"){
            if(i+1==Condition->length()){
                Condition->removeAt(i);
            }else{
                if(Condition->at(i+1)=="|" || Condition->at(i+1)=="&"){
                    Condition->removeAt(i);
                }else{
                    if(i==0){
                        Condition->removeAt(i);
                    }
                }
            }
        }else{
            if(cmpStrings(IdCheckbox, m_str, 0, sizeof(IdCheckbox)-2,0)){
                if(elementIdExistInDb(m_str)){

                }else{
                    Condition->removeAt(i);
                }
            }else{
                Condition->removeAt(i);
            }

        }
    }
}
void Risk_Manager::validateSrc(QList<QString>* Src){
    for(int i=0; i<Src->length(); i++){
        QString m_str = Src->at(i);
        m_str = removeAllCharWhosMatch(m_str, ' ');
        Src->removeAt(i);
        Src->insert(i, m_str);
        if(cmpStrings(IdRisk, Src->at(i), 0, sizeof(IdRisk)-2,0) || cmpStrings(IdGroup, Src->at(i), 0, sizeof(IdGroup)-2,0)){
            if(elementIdExistInDb(Src->at(i))){

            }else{
                Src->removeAt(i);
            }
        }else{
            Src->removeAt(i);
        }
    }

}
void Risk_Manager::validateDest(QList<QString>* Dest){
    for(int i=0; i<Dest->length(); i++){
        QString m_str = Dest->at(i);
        m_str = removeAllCharWhosMatch(m_str, ' ');
        Dest->removeAt(i);
        Dest->insert(i, m_str);
        if(cmpStrings(IdLifecycle, Dest->at(i), 0, sizeof(IdLifecycle)-2,0)){
            if(elementIdExistInDb(Dest->at(i))){

            }else{
                Dest->removeAt(i);
            }
        }else{
            Dest->removeAt(i);
        }
    }

}
QTreeWidgetItem* find_child(QTreeWidgetItem* ChildToFind, QTreeWidgetItem* ItemWithChildsToCheck){
    QList<QTreeWidgetItem*> listAvChilds;
    for(int i=0;i<ItemWithChildsToCheck->childCount();i++){
        listAvChilds.append(ItemWithChildsToCheck->child(i));
    }
    foreach(QTreeWidgetItem* c_Item, listAvChilds){
        if(c_Item->text(colId) == ChildToFind->text(colId)){
            return c_Item;
        }
    }
    return nullptr;
}
bool cmpStrings(QString A, QString B, int startA, int endA, int offsetB){
    if(endA >= A.length() || endA + offsetB >= B.length() || offsetB < 0 || startA > endA){
        return false;
    }else{
        for(int i=startA; i<=endA; i++){
            if(A[i]!=B[i+offsetB]){
                return false;
            }
        }
    }
    return true;
}





void Risk_Manager::on_doc_edit_button_add_risks_from_rules_clicked()
{
    if(ui->data_edit_lineEdit_currentOpenedFile->text().isEmpty()){
        QMessageBox::information(this, "Zuerst Datenbank Öffnen", "Die Regeln sind in der der Datenbank definiert. Bitte zuerst die zugehörige Datenbak öffnen und anschließend erneut versuchen.");
        return;
    }
    QString idString=IdCheckbox;
    idString.append("0");
    QList<QTreeWidgetItem*>listRootItemCheckboxDoc = ui->doc_edit_treeWidget_document->findItems(idString, Qt::MatchExactly, colId);
    if(!listRootItemCheckboxDoc.isEmpty()){
        QTreeWidgetItem* RootItemCheckboxDoc = listRootItemCheckboxDoc.first();
    }else{
        QMessageBox::information(this, "Zuerst Gerät spezifizieren", "Bitte zuerst Gerät spezifizieren und anschließend erneut versuchen.");
        return;
    }
    idString=IdLifecycle;
    idString.append("0");
    QList<QTreeWidgetItem*>listRootItemLifecycleDatabase = ui->data_edit_dataTree->findItems(idString, Qt::MatchExactly, colId);
    if(!listRootItemLifecycleDatabase.isEmpty()){

        QTreeWidgetItem* RootItemLifecycleDatabase = listRootItemLifecycleDatabase.first();
        QTreeWidgetItem* RootItemLifecycleDocument = copy_root_Item_to_dest_tree(RootItemLifecycleDatabase, ui->doc_edit_treeWidget_document);
        QFont font;
        font.setBold(true);
        RootItemLifecycleDocument->setFont(0, font);
        QList<QTreeWidgetItem*> RuleList = ui->data_edit_dataTree->findItems(IdLink, Qt::MatchStartsWith | Qt::MatchRecursive, colId);
        foreach(QTreeWidgetItem* item, RuleList){
            if(resolveCondition(item->text(colCondition))){
                QList<QString> StringListSrc = StringToList(item->text(colsrc), ' ');
                QList<QString> StringListDest = StringToList(item->text(coldest), ' ');
                QList<QTreeWidgetItem*> ItemListSrc;
                QList<QTreeWidgetItem*> ItemListDest;
                foreach(QString searchString, StringListSrc){
                    QList<QTreeWidgetItem*> foundItems = ui->data_edit_dataTree->findItems(searchString, Qt::MatchExactly|Qt::MatchRecursive, colId);
                    if(!foundItems.isEmpty()){
                        ItemListSrc.append(foundItems.first());
                    }
                }
                foreach(QString searchString, StringListDest){
                    QList<QTreeWidgetItem*> foundItems = ui->doc_edit_treeWidget_document->findItems(searchString, Qt::MatchExactly|Qt::MatchRecursive, colId);
                    if(!foundItems.isEmpty()){
                        ItemListDest.append(foundItems.first());
                    }
                }
                copy_dist_list(ItemListSrc,ItemListDest,true);
            }
        }
    }else{
        QMessageBox::information(this, "Keine Lebensphasen in Datenbank vorhanden", "Es ist keine Datenbank geöffnet oder die geöffnete Datenbank enthält keine Lebensphasen.");
        return;
    }


}
bool Risk_Manager::resolveCondition(QString Condition){
    QList<QString>ConditionList = convertConditionStringToList(Condition);
    while(ConditionList.count()>2){
        QString resultSingle = resolveOperation(ConditionList.at(0), ConditionList.at(1), ConditionList.at(2));
        ConditionList.pop_front();
        ConditionList.pop_front();
        ConditionList.pop_front();
        ConditionList.push_front(resultSingle);
    }
    if(ConditionList.at(0)=='T'){
        return true;
    }else{
        if(ConditionList.at(0)=='F'){
            return false;
        }else{
            return determineCheckstate(ConditionList.at(0));
        }
    }
}
QList<QString> Risk_Manager::convertConditionStringToList(QString Condition){
    QList<QString>ConditionList;
    QString targetString="";
    while(Condition.length() > 0){
        if(Condition[0]=='|' || Condition[0]=='&'){
            if(targetString!=""){
                ConditionList.append(targetString);
            }
            targetString=Condition[0];
            ConditionList.append(targetString);
            targetString="";
        }else{
            targetString.append(Condition[0]);
        }
        Condition=Condition.right(Condition.length()-1);
    }
    if(targetString!=""){
        ConditionList.append(targetString);
    }
    return ConditionList;
}
QString Risk_Manager::resolveOperation(QString i_input0, QString i_operation, QString i_input1){
    bool m_input0;
    bool m_input1;
    operation m_operation;
    if(i_input0 == "T"){
        m_input0=true;
    }else{
        if(i_input0 == "F"){
            m_input0=false;
        }else{
            m_input0=determineCheckstate(i_input0);
        }
    }
    if(i_input1 == "T"){
        m_input1=true;
    }else{
        if(i_input1 == "F"){
            m_input1=false;
        }else{
            m_input1=determineCheckstate(i_input1);
        }
    }
    if(i_operation == "|"){
        m_operation=OR;
    }else{
        if(i_operation == "&"){
            m_operation = AND;
        }
    }
    if(resolveOperation(m_input0, m_operation, m_input1)){
        return "T";
    }else{
        return "F";
    }
}
bool Risk_Manager::resolveOperation(bool input0, operation m_operation, bool input1){
    if(m_operation==OR){
        return input0 || input1;
    }
    if(m_operation==AND){
        return input0 && input1;
    }
    return false;
}
bool Risk_Manager::determineCheckstate(QString Checkbox){
    bool invertResult=false;
    if(Checkbox[0]=='!'){
        invertResult=true;
        Checkbox=Checkbox.right(Checkbox.length()-1);
    }
    QString IdRoot=IdCheckbox;
    IdRoot.append("0");
    QList<QTreeWidgetItem*>listRootItemCheckboxDoc = ui->doc_edit_treeWidget_document->findItems(IdRoot, Qt::MatchExactly, colId);
    if(!listRootItemCheckboxDoc.isEmpty()){
        QList<QString> allCheckedIds;
        QTreeWidgetItem* RootItemCheckboxDoc=listRootItemCheckboxDoc.first();
        for(int i=colCheckedBoxesStart; i<RootItemCheckboxDoc->columnCount(); i++){
            if(!RootItemCheckboxDoc->text(i).isEmpty()){
                allCheckedIds.append(RootItemCheckboxDoc->text(i));
            }else{
                break;
            }
        }
        if(allCheckedIds.contains(Checkbox)){
            if(!invertResult){
                return true;
            }else{
                return false;
            }

        }else{
            if(!invertResult){
                return false;
            }else{
                return true;
            }
        }
    }
    return false;
}
void Risk_Manager::on_doc_edit_button_set_checklist_clicked()
{
    show_editDeviceSpecifications();
}
bool Risk_Manager::elementIdExistInDb(QString Id){
    if(ui->data_edit_dataTree->findItems(Id, Qt::MatchExactly | Qt::MatchRecursive, colId).isEmpty()){
        return false;
    }else{
        return true;
    }
}
void Risk_Manager::on_pushButton_2_clicked()
{

}
QList<QString> StringToList(QString inputString, char separator){
    QString targetString="";
    QList<QString> ResultList;
    while(inputString.length()>0){
        if(inputString[0]==separator){
            ResultList.append(targetString);
            targetString="";
        }else{
            targetString.append(inputString[0]);
        }
        inputString=inputString.right(inputString.length()-1);
    }
    ResultList.append(targetString);
    return ResultList;
}
QString StringListToString(QList<QString> inputList, QString separator){
    QString outstr="";
    bool firstItem=true;
    foreach (QString item, inputList) {
        if(firstItem){
            outstr.append(item);
            firstItem=false;
        }else{
            outstr.append(separator);
            outstr.append(item);
        }

    }
    return outstr;
}
