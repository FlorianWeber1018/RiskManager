#include "treetotextable.h"

treeToTexTable::treeToTexTable(){

}
QString treeToTexTable::convert_treeToTEX(QTreeWidget *tree){
    //nach lebensphasen rootitem suchen und dieses plot child übergeben
}
QString treeToTexTable::plot_child(QTreeWidgetItem* child){
    QString rtnstr="";
    if(item->parent()==NULL){
        rtnstr.append("");
    }else{
        rtnstr.append("");
    }
    for(int i=0;i<item->columnCount();i++){
        rtnstr.append("<col");
        rtnstr.append(QString::fromStdString(std::to_string(i)));
        rtnstr.append(">");
        rtnstr.append(item->text(i));
        rtnstr.append("</col");
        rtnstr.append(QString::fromStdString(std::to_string(i)));
        rtnstr.append(">");
    }
    rtnstr.append("\n");
    for(int i=0;i<item->childCount();i++){
        rtnstr.append(plot_child(item->child(i)));
    }
    if(item->parent()==NULL){
        rtnstr.append("</rootitem>\n");
    }else{
        rtnstr.append("</item>\n");
    }
    return rtnstr;
}
