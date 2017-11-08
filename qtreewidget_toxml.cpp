#include "qtreewidget_toxml.h"


qtreewidget_toxml::qtreewidget_toxml(){
    targetItem=nullptr;
    targettree=nullptr;
}
QString qtreewidget_toxml::convert_treetoxml(QTreeWidget *tree){
    QString rtnString="<tree>\n";
    for(int i =0;i<tree->topLevelItemCount();i++) {
        rtnString.append(plot_child(tree->topLevelItem(i)));
    }
    rtnString.append("</tree>\n");
    return rtnString;
}
QString qtreewidget_toxml::plot_child(QTreeWidgetItem *item){
    QString rtnstr="";
    if(item->parent()==nullptr){
        rtnstr.append("<rootitem>\n");
    }else{
        rtnstr.append("<item>\n");
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
    if(item->parent()==nullptr){
        rtnstr.append("</rootitem>\n");
    }else{
        rtnstr.append("</item>\n");
    }
    return rtnstr;
}
void qtreewidget_toxml::convert_xmltotree(QString xml){
    QString currentString="";
    currentString=xml.right(xml.length()-(get_index(xml,"<tree>",direction::begin)+6));
    currentString=currentString.left(get_index(currentString,"</tree>",direction::end));
    while(currentString.length()>0){
        currentString = TagHandler(currentString);
    }

}
void qtreewidget_toxml::AddChild()
{
    QTreeWidgetItem *itm = new QTreeWidgetItem();
    targetItem->addChild(itm);
    itm->setFlags(itm->flags() | Qt::ItemIsEditable);
    targetItem=itm;
}
void qtreewidget_toxml::AddRoot()
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(targettree);
    QFont font;
    font.setBold(true);
    itm->setFont(0, font);
    targettree->addTopLevelItem(itm);
    targetItem=itm;
}
QString qtreewidget_toxml::setcol(QString Name, QString currentString)
{
    int col = Name.right(Name.length()-3).toInt();
    QString text = currentString.left(get_index(currentString, "<", direction::begin));
    for(int i=0; i<text.size(); i++){
        if(text[i]=='\n'){
            text[i]=' ';
        }
    }
    targetItem->setText(col, text);
    currentString = currentString.right(currentString.length()-get_index(currentString, "<", direction::begin));
    return currentString;
}
QString qtreewidget_toxml::InTagHandler(QString currentString, QString Name){
    if(Name=="rootitem"){
        AddRoot();
    }
    if(Name=="item"){
        AddChild();
    }
    if(Name.left(3)=="col"){
        currentString = setcol(Name, currentString);
    }
    return currentString;
}
QString qtreewidget_toxml::OutTagHandler(QString currentString, QString Name){
    if(Name=="/rootitem"){
        targetItem=nullptr;
    }
    if(Name=="/item"){
        targetItem = targetItem->parent();
    }
    if(Name.left(4)=="/col"){
        //NIX
    }
    return currentString;
}
QString qtreewidget_toxml::TagHandler(QString currentString){
    currentString=currentString.right(currentString.length()-get_index(currentString, "<", direction::begin));
    QString tagname;
    int indexInTagStart = get_index(currentString,"<",direction::begin);
    int indexInTagEnd   = get_index(currentString,">",direction::begin);
    tagname=currentString.left(indexInTagEnd);
    tagname=tagname.right(indexInTagEnd-(indexInTagStart+1));
    currentString=currentString.right(currentString.length()-(indexInTagEnd+1));
    if(tagname[0]=='/'){
        currentString = OutTagHandler(currentString, tagname);
    }else{
        currentString = InTagHandler(currentString, tagname);
    }
    return currentString;
}
int qtreewidget_toxml::get_index(QString wholeString, QString keyString, qtreewidget_toxml::direction start_index){
    switch(start_index){
        case qtreewidget_toxml::direction::begin:{
            for(int i=0;i<=(wholeString.length()-keyString.length());i++){
                bool found=true;
                for(int j=0;j<keyString.length();j++){
                    if(wholeString[i+j]!=keyString[j]){
                        found=false;
                        break;
                    }
                }
                if(found){
                    return i;
                }
            }
        }break;
        case qtreewidget_toxml::direction::end:{
            for(int i=wholeString.length()-keyString.length();i>=0;i--){
                bool found=true;
                for(int j=0;j<keyString.length();j++){
                    if(wholeString[i+j]!=keyString[j]){
                        found=false;
                        break;
                    }
                }
                if(found){
                    return i;
                }
            }

        }
    }
    return 0;
}
void qtreewidget_toxml::aligntree(QTreeWidget *tree){
    this->targettree=tree;
    while (tree->topLevelItemCount()) {
        delete tree->takeTopLevelItem(0);
    }

}

