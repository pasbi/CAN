#include "program.h"

const Program Program::INVALID = Program();

QJsonObject Program::toJsonObject() const
{
    QJsonObject program;
    program["bank"] = m_bank;
    program["program"] = m_program;
    program["valid"] = m_valid;
    return program;
}

bool Program::restoreFromJsonObject(const QJsonObject& json)
{
    if (   !checkJsonObject(json, "bank",    QJsonValue::Double)
        || !checkJsonObject(json, "program", QJsonValue::Double)
        || !checkJsonObject(json, "valid",   QJsonValue::Bool)   )
    {
        return false;
    }

    m_bank = json["bank"].toDouble();
    m_program = json["program"].toDouble();
    m_valid = json["valid"].toBool();

    return true;
}
