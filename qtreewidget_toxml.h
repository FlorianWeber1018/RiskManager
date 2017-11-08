#ifndef QTREEWIDGET_TOXML_H
#define QTREEWIDGET_TOXML_H
#include <QString>
#include <QTreeWidget>

class qtreewidget_toxml
{
    public:
        qtreewidget_toxml();
        QString convert_treetoxml(QTreeWidget *tree);

        void convert_xmltotree(QString xml);
        void aligntree(QTreeWidget *tree);
        enum direction{begin, end};
        int get_index(QString wholeString, QString keyString, direction start_index);
    private:
        void AddChild();
        void AddRoot();
        QString setcol(QString Name, QString currentString);

        QString InTagHandler(QString currentString, QString Name);
        QString OutTagHandler(QString currentString, QString Name);
        QString TagHandler(QString currentString);
        QString plot_child(QTreeWidgetItem*);
        QTreeWidget* targettree;
        QTreeWidgetItem* targetItem;

};

#endif // QTREEWIDGET_TOXML_H
