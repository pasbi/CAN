#ifndef OPTIONPUSHBUTTON_H
#define OPTIONPUSHBUTTON_H

#include <QToolButton>


class OptionPushButton : public QToolButton
{
    Q_OBJECT
public:
    struct Option
    {
        Option(const QString& iconFilename, const QString& toolTip) :
            iconFilename(iconFilename),
            toolTip(toolTip)
        {

        }
        QString iconFilename;
        QString toolTip;
    };

    explicit OptionPushButton(QWidget *parent = 0);

    void setOptions(const QList<Option>& options);
    void setCurrentIndex(int index);
    int currentIndex() const;
    int count() const;
    void reset();

public slots:
    void next();

signals:
    void currentIndexChanged(int);

private:
    QList<Option> m_options;
    int m_currentIndex;


};

#endif // OPTIONPUSHBUTTON_H
