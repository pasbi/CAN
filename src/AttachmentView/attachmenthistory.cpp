#include "attachmenthistory.h"
#include "Attachments/attachment.h"
#include "Database/SongDatabase/song.h"

AttachmentHistory::AttachmentHistory() :
    m_history(nullptr),
    m_index(-1),
    m_currentSong(nullptr)
{
}

void AttachmentHistory::appendAttachment(Attachment *a)
{
    Song* song = a ? a->song() : nullptr;

    loadHistory(song);

    if (m_history)
    {
        for (int i = m_history->length() - 1; i > m_index; --i)
        {
            m_history->removeAt(i);
        }

        // do not allow the same attachment subsequently
        if (m_history->isEmpty() || m_history->last() != a)
        {
            m_history->append(a);
            m_index++;
        }
    }
}

void AttachmentHistory::removeAttachment(Attachment *a)
{
    for (int i = m_history->length() - 1; i >= 0; --i)
    {
        bool removeItem = false;
        if (a == m_history->value(i))
        {
            // current item is the to-delete-item
            removeItem = true;
        }
        else if (i < m_history->length() - 1)
        {
            if (m_history->value(i) == m_history->value(i+1))
            {
                //duplicate
                removeItem = true;
            }
        }

        if (removeItem)
        {
            m_history->removeAt(i);
            if (i <= m_index)
            {
                m_index--;
            }
        }

    }
}

bool AttachmentHistory::hasNextAttachment() const
{
    if (m_history)
    {
        return m_index < m_history->length() - 1;
    }
    else
    {
        return false;
    }
}

bool AttachmentHistory::hasPreviousAttachment() const
{
    if (m_history)
    {
        return m_index > 0;
    }
    else
    {
        return false;
    }
}

Attachment* AttachmentHistory::nextAttachment()
{
    if (hasNextAttachment())
    {
        m_index++;
        return currentAttachment();
    }
    else
    {
        return nullptr;
    }
}

Attachment* AttachmentHistory::previousAttachment()
{
    if (hasPreviousAttachment())
    {
        m_index--;
        return currentAttachment();
    }
    else
    {
        return nullptr;
    }
}

Attachment* AttachmentHistory::currentAttachment() const
{
    if (m_history && m_index >= 0)
    {
        return m_history->value(m_index);
    }
    else
    {
        return nullptr;
    }
}

void AttachmentHistory::loadHistory(Song *song)
{
    if (song)
    {
        if (song != m_currentSong)
        {
            m_currentSong = song;
            m_history = &m_histories[m_currentSong];
            m_index = m_history->length() - 1;
        }
    }
    else
    {
        m_history = nullptr;
        m_index = -1;
        m_currentSong = nullptr;
    }
}

void AttachmentHistory::removeObsoleteAttachments()
{
    if (m_history)
    {
        QSet<Attachment*> attachments = m_history->toSet(); // remove all duplicates, order does not matter.
        for (Attachment* a : attachments)
        {
            if (!a->song()->attachments().contains(a))
            {
                removeAttachment(a);
            }
        }
    }
}

