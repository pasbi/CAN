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
    void addTag(const QString & tag);
    void removeTag(const QString & tag);
    bool hasTag(const QString & tag) const;
    QStringList tags() const { return m_tags; }
    void setTags( const QStringList& tags );

    static QStringList allTags() { return m_allTags.keys(); }

    virtual void serialize(QDataStream &out) const;
    virtual void deserialize(QDataStream &in);

private:
    QStringList m_tags;
    static QMap<QString, int> m_allTags;

};

#endif // TAGGABLE_H
