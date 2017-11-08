#ifndef TREETOTEXTABLE_H
#define TREETOTEXTABLE_H
#include <QString>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class treeToTexTable
{
public:
    treeToTexTable();
    QString convert_treeToTEX(QTreeWidget *tree);
private:
    QString plot_child(QTreeWidgetItem*);
};

#endif // TREETOTEXTABLE_H
