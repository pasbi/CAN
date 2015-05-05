#ifndef COMBOEDITOR_H
#define COMBOEDITOR_H

#include "celleditor.h"
#include <QComboBox>

class ComboEditor : public CellEditor
{
    Q_OBJECT
    DECL_CREATABLE(ComboEditor)
public:
    explicit ComboEditor(QWidget *parent = 0);

    void polish();
    QVariant editedData() const;

private:
    QComboBox* m_combo;


};

#endif // COMBOEDITOR_H
