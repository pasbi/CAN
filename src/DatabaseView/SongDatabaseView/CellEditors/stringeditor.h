#ifndef STRINGEDITOR_H
#define STRINGEDITOR_H

#include "celleditor.h"
#include "QLineEdit"

class StringEditor : public CellEditor
{
    Q_OBJECT
    DECL_CREATABLE(StringEditor)
public:
    explicit StringEditor(QWidget *parent = 0);

    void polish();
    QVariant editedData() const;

private:
    QLineEdit* m_lineEdit;

};

#endif // STRINGEDITOR_H
