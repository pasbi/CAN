#ifndef ATTACHMENTMERGEWIDGETBASE_H
#define ATTACHMENTMERGEWIDGETBASE_H

#include <QWidget>
#include "Merge/mergeitem.h"

class QVBoxLayout;
class AttachmentMergeWidgetBase : public QWidget
{
protected:
    AttachmentMergeWidgetBase(MergeItem* mergeItem, QWidget* parent = nullptr);
    template<class T> T* masterAttachment() const
    {
        return qobject_assert_cast<T>(m_mergeItem->master());
    }

    template<class T> T* slaveAttachment() const
    {
        return qobject_assert_cast<T>(m_mergeItem->slave());
    }

    virtual void apply() = 0;

    void addEditorWidget(QWidget* masterWidget, QWidget* slaveWidget, const QString& label = "");

    MergeItem* m_mergeItem;

private:
    QVBoxLayout* m_layout;
};

#endif // ATTACHMENTMERGEWIDGETBASE_H
