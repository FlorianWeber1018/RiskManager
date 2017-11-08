#ifndef EDITSELETABLERISKREDUCTION_H
#define EDITSELETABLERISKREDUCTION_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
class editSeletableRiskReduction;
}

class editSeletableRiskReduction : public QDialog
{
    Q_OBJECT

public:

    explicit editSeletableRiskReduction(QWidget *parent = 0);
    void alignTargetItem(QTreeWidgetItem* item);
    QTreeWidget* getTree();
    ~editSeletableRiskReduction();

private slots:
    void on_editSeletableRiskReduction_accepted();
private:
    QTreeWidgetItem* targetItem;
    QTreeWidget* srcTree;
    Ui::editSeletableRiskReduction *ui;
    QTreeWidgetItem* treeItem;
};

#endif // EDITSELETABLERISKREDUCTION_H
