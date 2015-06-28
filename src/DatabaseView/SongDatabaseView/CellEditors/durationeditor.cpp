#include "durationeditor.h"

DEFN_CREATABLE(DurationEditor, CellEditor);

DurationEditor::DurationEditor(QWidget *parent) :
    CellEditor(parent),
    m_timeEdit( new QTimeEdit(this) )
{
    m_timeEdit->setDisplayFormat( "hh:mm:ss" );
    setSolitaryWidget(m_timeEdit);
}

void DurationEditor::polish()
{
    m_timeEdit->setTime( currentData().toTime() );
    m_timeEdit->selectAll();
}

QVariant DurationEditor::editedData() const
{
    return m_timeEdit->time();
}

