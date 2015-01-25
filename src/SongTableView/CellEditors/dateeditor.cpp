#include "dateeditor.h"

DEFN_CREATABLE(DateEditor, CellEditor);

DateEditor::DateEditor(QWidget *parent) :
    CellEditor(parent)
{

}

void DateEditor::polish()
{

}

QVariant DateEditor::editedData() const
{
    return QVariant("date");
}
