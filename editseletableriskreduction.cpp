#include "editseletableriskreduction.h"
#include "ui_editseletableriskreduction.h"
#include "risk_manager.h"

editSeletableRiskReduction::editSeletableRiskReduction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editSeletableRiskReduction)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnWidth(0, 150);
    ui->treeWidget->setColumnWidth(1, 30);
    ui->treeWidget->setColumnWidth(2, 5);
    ui->treeWidget->setColumnWidth(3, 5);
}

editSeletableRiskReduction::~editSeletableRiskReduction()
{
    delete ui;
}
void editSeletableRiskReduction::alignTargetItem(QTreeWidgetItem* item){
    this->targetItem=item;
}
QTreeWidget* editSeletableRiskReduction::getTree(){
    return ui->treeWidget;
}

void editSeletableRiskReduction::on_editSeletableRiskReduction_accepted()
{
    QList<QTreeWidgetItem*> listSelectedItems = ui->treeWidget->selectedItems();
    if(!listSelectedItems.isEmpty()){
        QTreeWidgetItem* selectedItem = listSelectedItems.first();
        targetItem->setText(colRiskReduction, selectedItem->text(colRiskReduction));
    }

}



