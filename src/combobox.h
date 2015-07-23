#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QPaintEvent>

class ComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBox(QWidget *parent = 0);

    void setInvalidText( const QString& text );
    QString invalidText() const { return m_text; }

protected:
    void paintEvent(QPaintEvent *e);

private:
    QString m_text;


};

#endif // COMBOBOX_H
