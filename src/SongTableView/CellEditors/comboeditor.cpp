#include "comboeditor.h"
#include <QHBoxLayout>
#include "global.h"


DEFN_CREATABLE(ComboEditor, CellEditor);

ComboEditor::ComboEditor(QWidget *parent) :
    CellEditor(parent),
    m_combo( new QComboBox(this) )
{
    setSolitaryWidget(m_combo);
    installEventFilter(this);
    installEventFilter(m_combo);
    m_combo->installEventFilter(this);

}

void ComboEditor::polish()
{
    m_combo->setEditable(true);

    QStringList items;
    for (int i = 0; i < model()->rowCount(); ++i)
    {
        QString item = model()->data( i, index().column(), Qt::DisplayRole ).toString();
        if (!item.isEmpty() && !items.contains(item))
        {
            items.append(item);
        }
    }
    m_combo->addItems(items);

    m_combo->setInsertPolicy(QComboBox::InsertAtTop);
    m_combo->setCurrentText(currentData().toString());
}

QVariant ComboEditor::editedData() const
{
    return m_combo->currentText();
}


