#ifndef KEYEDITOR_H
#define KEYEDITOR_H

#include <QLineEdit>
#include "chord.h"

class KeyEditor : public QLineEdit
{
    Q_OBJECT

public:
    explicit KeyEditor(QWidget *parent = 0);
    void setKey(Chord chord);
    Chord key() const;

private:
    Chord m_key;
};

#endif // KEYEDITOR_H
