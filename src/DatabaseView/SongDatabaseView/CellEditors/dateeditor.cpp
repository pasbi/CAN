#include "dateeditor.h"

#include <QDateTimeEdit>

DEFN_CREATABLE(DateEditor, CellEditor);

DateEditor::DateEditor(QWidget *parent) :
    CellEditor(parent),
    m_dateTimeEdit( new QDateTimeEdit(this ) )
{
    setSolitaryWidget(m_dateTimeEdit);
}

void DateEditor::polish()
{
    m_dateTimeEdit->setDateTime(currentData().toDateTime());
}

QVariant DateEditor::editedData() const
{
    return m_dateTimeEdit->dateTime();
}
