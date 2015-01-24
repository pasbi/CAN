#ifndef TAGGABLE_H
#define TAGGABLE_H

#include "persistentobject.h"
#include <QSet>

/**
 * @brief The Taggable class capsels the ability to be tagged.
 */
class Taggable : public PersistentObject
{
public:
    Taggable();
    virtual ~Taggable();
    void addTag(const QString & tag) { m_tags.insert(tag); }
    void removeTag(const QString & tag) { m_tags.remove(tag); }
    bool hasTag(const QString & tag) const { return m_tags.contains(tag); }

    virtual void restoreFromJsonObject( const QJsonObject & object );
    QJsonObject toJsonObject() const;

private:
    QSet<QString> m_tags;

};

#endif // TAGGABLE_H
