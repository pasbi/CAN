#ifndef SETLISTCHANGESONGCOMMAND_H
#define SETLISTCHANGESONGCOMMAND_H

#include "Commands/modelcommand.h"
#include <QModelIndex>

class SetlistItem;
class Song;
class SetlistItemChangeSongCommand : public ModelCommand<SetlistItem>
{
public:
    SetlistItemChangeSongCommand(SetlistItem* setlistItem, const Song* newSong);
    void undo();
    void redo();

private:
    const Song* const m_newSong;
    const Song* const m_oldSong;
};

#endif // SETLISTCHANGESONGCOMMAND_H
