#ifndef PROGRAM_H
#define PROGRAM_H

#include "persistentobject.h"

class Program : public PersistentObject
{
public:
    Program();
    ~Program();

    const static Program INVALID;

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

    bool isValid() const { return m_valid; }
    int bank() const { return m_bank; }
    int program() const { return m_program; }
    bool& isValid() { return m_valid; }
    int& bank() { return m_bank; }
    int& program() { return m_program; }

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    int m_bank;
    int m_program;
    bool m_valid;
};


#endif // PROGRAM_H
