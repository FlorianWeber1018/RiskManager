#ifndef QTREEWIDGET_TOTEXTABLE_H
#define QTREEWIDGET_TOTEXTABLE_H
#include <QString>
#include <QTreeWidget>
#include <risk_manager.h>
class qtreewidget_totextable
{
public:
    qtreewidget_totextable();
    QString convert_treetotextable(QTreeWidget *tree);
private:
    QString plot_child(QTreeWidgetItem*);
    QString col0;
};

#endif // QTREEWIDGET_TOTEXTABLE_H
