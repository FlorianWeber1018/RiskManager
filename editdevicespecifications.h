#ifndef EDITDEVICESPECIFICATIONS_H
#define EDITDEVICESPECIFICATIONS_H

#include <QDialog>
#include <QTreeWidgetItem>
namespace Ui {
class editDeviceSpecifications;
}

class editDeviceSpecifications : public QDialog
{
    Q_OBJECT

public:
    explicit editDeviceSpecifications(QWidget *parent = 0);
    ~editDeviceSpecifications();
    void alignTargetTree(QTreeWidget* tree);
    QTreeWidget* getTree();

private slots:
    void on_editDeviceSpecifications_accepted();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_doubleClicked(const QModelIndex &index);

private:
    QTreeWidget* targettree;
    QTreeWidgetItem* AddRoot(QTreeWidget* tree, QString name, QString Id);
    Ui::editDeviceSpecifications *ui;
};

#endif // EDITDEVICESPECIFICATIONS_H
