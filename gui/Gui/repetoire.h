#ifndef REPETOIRE_H
#define REPETOIRE_H

#include <QWidget>

namespace Ui {
class Repetoire;
}

class Repetoire : public QWidget
{
    Q_OBJECT

public:
    explicit Repetoire(QWidget *parent = 0);
    ~Repetoire();

private:
    Ui::Repetoire *ui;
};

#endif // REPETOIRE_H
