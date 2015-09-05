//#include "stringeditor.h"
//#include <QLineEdit>

//DEFN_CREATABLE(StringEditor, CellEditor);

//StringEditor::StringEditor(QWidget *parent) :
//    CellEditor(parent),
//    m_lineEdit( new QLineEdit(this) )
//{
//    setSolitaryWidget(m_lineEdit);
//}

//void StringEditor::polish()
//{
//    m_lineEdit->setText( currentData().toString() );
//    m_lineEdit->selectAll();
//    m_lineEdit->setCursorPosition( currentData().toString().length() );
//}

//QVariant StringEditor::editedData() const
//{
//    return m_lineEdit->text();
//}
