#include "editdevicespecifications.h"
#include "ui_editdevicespecifications.h"
#include "risk_manager.h"
editDeviceSpecifications::editDeviceSpecifications(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editDeviceSpecifications)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnWidth(0, 150);
    ui->treeWidget->setColumnWidth(1, 40);
    ui->treeWidget->setColumnWidth(2, 23);
}

editDeviceSpecifications::~editDeviceSpecifications()
{
    delete ui;
}
void editDeviceSpecifications::alignTargetTree(QTreeWidget* tree){
    this->targettree=tree;
}
QTreeWidget* editDeviceSpecifications::getTree(){
    return ui->treeWidget;
}


void editDeviceSpecifications::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(column==colCheck){
        if(item->text(colCheck)!="X"){
            item->setText(colCheck, "X");
        }else{
            item->setText(colCheck, "");
        }
    }

}

void editDeviceSpecifications::on_treeWidget_doubleClicked(const QModelIndex &index)
{

}
QTreeWidgetItem* editDeviceSpecifications::AddRoot(QTreeWidget* tree, QString name, QString Id)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(tree);
    itm->setText(0,name);
    itm->setText(1,Id);
    QFont font;
    font.setBold(true);
    itm->setFont(0, font);
    tree->addTopLevelItem(itm);
    return itm;
}
void editDeviceSpecifications::on_editDeviceSpecifications_accepted()
{
    QList<QTreeWidgetItem*> listCheckedItems = ui->treeWidget->findItems("X", Qt::MatchExactly | Qt::MatchRecursive, colCheck);
     QTreeWidgetItem* deviceSpecification;
    QList<QString> listCheckedIds;
    foreach (QTreeWidgetItem* item, listCheckedItems) {
        listCheckedIds.append(item->text(colId));
    }
    QString IdString=IdCheckbox;
    IdString.append("0");
    QList<QTreeWidgetItem*> listDeviceSpecification = targettree->findItems(IdString, Qt::MatchExactly, colId);
    if(!listDeviceSpecification.isEmpty()){
        deviceSpecification = listDeviceSpecification.first();
        for(int i=2;i<deviceSpecification->columnCount();i++){
            deviceSpecification->setText(i,"");
        }
    }else{
        deviceSpecification = AddRoot(targettree, "Geräte Spezifikation", IdString);

    }
    for(int i=0;i<listCheckedIds.size();i++){
        deviceSpecification->setText(i+colCheckedBoxesStart, listCheckedIds.at(i));
    }
}
