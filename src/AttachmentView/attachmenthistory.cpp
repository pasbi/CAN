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

//    qDebug() << "   Append " << a;
//    debug();
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

//    qDebug() << "   Remove " << a;
//    debug();
}

bool AttachmentHistory::hasNextAttachment() const
{
    if (m_history)
    {
        qDebug() << "index = " << m_index;
        qDebug() << "history length = " << m_history->length() - 1 << *m_history;
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


//#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
//#include "Database/SongDatabase/songdatabase.h"

//void AttachmentHistory::debug()
//{
//    qDebug() << "----------------------";
//    if (m_history)
//    {
//        int i = 0;
//        for (Attachment* a : *m_history)
//        {
//            qDebug() << QString("%1[%2] %3").arg(a->type()).arg(a->name()).arg(i==m_index?"<-":"").toStdString().c_str();
//            i++;
//        }
//    }
//    qDebug() << "^^^^^^^^^^^^^^^^^^^^^^";
//}

//void testHist()
//{
//    Project* project = new Project();
//    SongDatabase* sd = new SongDatabase(project);
//    Song* s0 = new Song(sd);
////    Song* s1 = new Song(sd);
//    AttachmentHistory* hist = new AttachmentHistory();
//    ChordPatternAttachment* c0 = new ChordPatternAttachment();
//    c0->setSong(s0);
//    ChordPatternAttachment* c1 = new ChordPatternAttachment();
//    c1->setSong(s0);
//    ChordPatternAttachment* c2 = new ChordPatternAttachment();
//    c2->setSong(s0);
//    ChordPatternAttachment* c3 = new ChordPatternAttachment();
//    c3->setSong(s0);
//    ChordPatternAttachment* c4 = new ChordPatternAttachment();
//    c4->setSong(s0);
//    QList<Attachment*> attachments = QList<Attachment*>({ c0, c1, c2, c3, c4 });

//    auto ind = [attachments](Attachment* a)
//    {
//        return attachments.indexOf(a);
//    };

//    auto debug = [hist, ind]()
//    {
//        qDebug() << "=====================";
//        int i = 0;
//        for (Attachment* a : *hist->m_history)
//        {
//            qDebug() << QString("c%1 %2").arg(ind(a)).arg(i==hist->m_index?"<-":"").toStdString().c_str();
//            i++;
//        }
//    };

//    hist->appendAttachment(c0);
//    hist->appendAttachment(c1);
//    hist->appendAttachment(c2);
//    hist->appendAttachment(c1);
//    hist->appendAttachment(c2);
//    hist->appendAttachment(c1);
//    hist->appendAttachment(c1);
//    hist->appendAttachment(c3);

//    hist->previousAttachment();
//    hist->previousAttachment();
//    hist->previousAttachment();
//    hist->previousAttachment();

//    debug();

//    hist->removeAttachment(c1);

//    debug();

//    hist->appendAttachment(c1);

//    debug();

//    hist->removeAttachment(c0);

//    debug();

//    hist->removeAttachment(c1);

//    debug();

//    qDebug() << hist->currentAttachment();

//}
















