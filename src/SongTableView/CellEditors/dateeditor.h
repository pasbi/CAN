#ifndef DATEEDITOR_H
#define DATEEDITOR_H

#include "celleditor.h"


class DateEditor : public CellEditor
{
    Q_OBJECT
    DECL_CREATABLE(DateEditor);
public:
    explicit DateEditor(QWidget *parent = 0);

    void polish();
    QVariant editedData() const;

};

#endif // DATEEDITOR_H
