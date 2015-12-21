#include "keyeditor.h"
#include <QLineEdit>
#include "chord.h"
#include "global.h"
#include <QFocusEvent>


KeyEditor::KeyEditor(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this, &KeyEditor::textChanged, [this]()
    {
        setKey(Chord(text()));
    });
}

void KeyEditor::setKey(Chord chord)
{
    m_key = chord;
    setText(chord.key());
}

Chord KeyEditor::key() const
{
    return m_key;
}


