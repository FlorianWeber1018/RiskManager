#include "qtreewidget_totextable.h"

qtreewidget_totextable::qtreewidget_totextable()
{
    col0="";
}
QString qtreewidget_totextable::convert_treetotextable(QTreeWidget *tree){
    QString rtnString="";
    QList<QTreeWidgetItem*> listlpRootItems= tree->findItems(IdLifecycle ,Qt::MatchStartsWith, colId);
    QTreeWidgetItem* rootitem;
    if(listlpRootItems.isEmpty()){
        return rtnString;
    }else{
        rootitem=listlpRootItems.first();
    }

    for(int i =0;i<rootitem->childCount();i++) {
        rtnString.append(plot_child(rootitem->child(i)));
    }
    return rtnString;
}
QString qtreewidget_totextable::plot_child(QTreeWidgetItem *item){
    QString rtnstr="";
    QString idstr=item->text(colId);
    if(cmpStrings(IdLifecycle,idstr,0,sizeof(IdLifecycle)-2,0) && !idstr.contains('.')){                             // Lebensphase ohne . in ID
        idstr=idstr.right(idstr.length()-(sizeof(IdLifecycle)-1));                            //       L0001->0001

        rtnstr.append("\\multicolumn{9}{|l|}{{\\cellcolor[HTML]{D9D9D9}}\\textbf{");
        rtnstr.append(idstr);
        rtnstr.append(" ");
        rtnstr.append(item->text(0));
        rtnstr.append("}}\\\\ \\hline");
        rtnstr.append("\n");
    }
    if(cmpStrings(IdLifecycle,idstr,0,sizeof(IdLifecycle)-2,0) && idstr.contains('.')){
        idstr=idstr.right(idstr.length()-(sizeof(IdLifecycle)-1));

        col0=idstr;
        col0.append(" ");
        col0.append(item->text(0));
    }
    if(cmpStrings(IdRisk,idstr,0,sizeof(IdRisk)-2,0)){
        rtnstr.append(col0);
        col0="";
        int i=2;
        while(i<item->columnCount() && i<10){
            rtnstr.append("&");
            if(i>4){
                rtnstr.append("\\begin{center}");
            }
            rtnstr.append(item->text(i));
            if(i>4){
                rtnstr.append("\\end{center}");
            }
            i++;
        }
        while(i<10){
            rtnstr.append("&");
            rtnstr.append(" ");
            i++;
        }
        rtnstr.append("\\\\ \\hline");
        rtnstr.append("\n");
    }
    for(int i=0;i<item->childCount();i++){
        rtnstr.append(plot_child(item->child(i)));
    }
    return rtnstr;
}
