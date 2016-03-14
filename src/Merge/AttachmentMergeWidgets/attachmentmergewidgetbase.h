#ifndef ATTACHMENTMERGEWIDGETBASE_H
#define ATTACHMENTMERGEWIDGETBASE_H

#include <QWidget>
#include "Merge/mergeitem.h"
#include <functional>

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

    void addEditorWidget(QWidget* masterWidget, QWidget* slaveWidget, const QString& key, const QString& label = "");

    MergeItem* m_mergeItem;

private:
    QVBoxLayout* m_layout;
    QList<MergeItem::ModifyDetail> m_modifyDetails;

public:
    QList<MergeItem::ModifyDetail> modifyDetails() const;


};

#endif // ATTACHMENTMERGEWIDGETBASE_H
