#include "optionpushbutton.h"

OptionPushButton::OptionPushButton(QWidget *parent) :
    QToolButton(parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(next()));
    setCurrentIndex(0);
}

void OptionPushButton::setOptions(const QList<Option>& options)
{
    m_options = options;
    setCurrentIndex(0);
}

void OptionPushButton::setCurrentIndex(int index)
{
    if (index < 0)
    {
        index = 0;
    }
    if (index >= count())
    {
        index = count() - 1;
    }
    if (m_currentIndex != index)
    {
        m_currentIndex = index;
        emit currentIndexChanged(index);
    }

    // set the icon even if m_currentIndex == index, maybe icons have changed.
    if (m_currentIndex >= 0)
    {
        setIcon(QIcon(m_options[m_currentIndex].iconFilename));
        setToolTip(m_options[m_currentIndex].toolTip);
    }
}

void OptionPushButton::next()
{
    if (count() == 0)
    {
        return;
    }
    else
    {
        int nextIndex = (currentIndex() + 1) % count();
        setCurrentIndex(nextIndex);
    }
}

int OptionPushButton::currentIndex() const
{
    return m_currentIndex;
}

int OptionPushButton::count() const
{
    return m_options.count();
}
