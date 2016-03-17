#include "browseattachmentbuttonbox.h"
#include "ui_browseattachmentbuttonbox.h"
#include <QMenu>
#include <QAction>
#include "Attachments/attachment.h"
#include "Database/SongDatabase/song.h"

QStringList attachmentKeys()
{
    return { "ChordPatternAttachment", "AudioAttachment", "PDFAttachment" };
}

BrowseAttachmentButtonBox::BrowseAttachmentButtonBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowseAttachmentButtonBox),
    m_currentSong(nullptr)
{
    ui->setupUi(this);

    m_toolButtons.insert(attachmentKeys()[0], ui->buttonBrowseChordAttachments);
    m_toolButtons.insert(attachmentKeys()[1], ui->buttonBrowseAudioAttachments);
    m_toolButtons.insert(attachmentKeys()[2], ui->buttonBrowsePDFAttachments);

    for (const QString& key : attachmentKeys())
    {
        m_menus.insert(key, new QMenu(this));
        m_toolButtons[key]->setMenu(m_menus[key]);

        // update default
        connect(m_toolButtons[key]->menu(), &QMenu::triggered, [this, key](QAction* action)
        {
            if (m_currentSong)
            {
                Attachment* attachment = m_attachmentMap[action];
                m_defaultAttachment[key][m_currentSong] = attachment;
            }
        });

        connect(m_toolButtons[key]->menu(), &QMenu::triggered, [this, key](QAction* action)
        {
            if (m_currentSong)
            {
                Attachment* attachment = m_attachmentMap[action];
                int index = m_currentSong->attachments().indexOf(attachment);
                setAttachment(index);
                emit attachmentSelected(index);
            }
        });

        // trigger default
        connect(m_toolButtons[key], &QToolButton::clicked, [this, key]()
        {
            if (m_currentSong)
            {
                emit attachmentSelected(m_currentSong->attachments().indexOf(const_cast<Attachment*>(defaultAttachment(key))));
            }
        });
    }
}

BrowseAttachmentButtonBox::~BrowseAttachmentButtonBox()
{
    delete ui;
}

// chordpattern and chordpatternproxy attachments share one button.
QString typeMap(const QString& type)
{
    if (type == "ChordPatternProxyAttachment")
    {
        return "ChordPatternAttachment";
    }
    else
    {
        return type;
    }
}

void BrowseAttachmentButtonBox::setAttachment(int index)
{
    if (index < 0)
    {
        setSong(nullptr);
    }
    else
    {
        setSong(m_currentSong);
    }
    if (m_currentSong && index >= 0 && index < m_currentSong->attachments().length())
    {
        Attachment* attachment = m_currentSong->attachments()[index];
        m_defaultAttachment[typeMap(attachment->type())][m_currentSong] = attachment;
    }
}


void BrowseAttachmentButtonBox::setSong(const Song * song)
{
    m_currentSong = song;

    for (const QString& key : attachmentKeys())
    {
        m_menus[key]->clear();
    }
    m_attachmentMap.clear();

    if (song)
    {
        for (Attachment* attachment : song->attachments())
        {
            QMenu* menu = m_menus[typeMap(attachment->type())];
            Q_ASSERT(menu);

            QAction* action = menu->addAction(attachment->name());
            m_attachmentMap[action] = attachment;
            action->setData(QVariant::fromValue<void*>(attachment));
            m_attachmentMap.insert(action, attachment);
        }
    }

    for (const QString& key : attachmentKeys())
    {
        m_toolButtons[key]->setEnabled( !m_menus[key]->actions().isEmpty() );
    }
}

const Attachment *BrowseAttachmentButtonBox::defaultAttachment(const QString &type)
{
    const Attachment* attachment = nullptr;
    if (m_currentSong)
    {
        attachment = m_defaultAttachment[type][m_currentSong];  // get the default value

        // if this attachment was removed from song or the song has no attachment of this type, get a fallback
        if (!attachment || m_currentSong->attachments().contains(const_cast<Attachment*>(attachment)))
        {
            for (const Attachment* a : m_currentSong->attachments())
            {
                if (typeMap(a->type()) == type)
                {
                    attachment = a;
                    break;
                }
            }
        }

        // if such a fallback was found, set it as new default attachment
        if (attachment)
        {
            m_defaultAttachment[type][m_currentSong] = attachment;
        }

    }
    return attachment;
}

