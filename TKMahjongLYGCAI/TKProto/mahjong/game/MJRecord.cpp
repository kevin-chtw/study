#include "MJRecord.h"
#include "MJGame.h"
#include "MJString.h"

#include <iostream>
#include <fstream>

const int EAID_NetBreak = 18;
const int EAID_NetResume = 19;
const int EAID_ExitMatch = 20;

MJRecordItem::MJRecordItem(int actionID)
{
    ResetAction(actionID);
}

MJRecordItem::MJRecordItem(std::string const& actionName)
{
    ResetAction(actionName);
}

void MJRecordItem::ResetAction(int actionID)
{
    m_ActionID = actionID;
    m_ActionName = std::to_string(actionID);
    m_Properties.clear();
}

void MJRecordItem::ResetAction(std::string const& actionName)
{
    m_ActionID = -1;
    m_ActionName = actionName;
    m_Properties.clear();
}

std::string MJRecordItem::ToString() const
{
    std::string res{};
    res += "id=" + MJ::ToQuoteString(m_ActionName);

    for (auto const& p : m_Properties)
    {
        res += " " + p.first + "=" + MJ::ToQuoteString(p.second);
    }
    return res;
}

void MJRecord::Initialize()
{
    m_HasPBRecord = false;  // override Initialize if mean to true.

    m_MsgID = m_Game->GetMsgType();
    if (m_HasPBRecord)
    {
        this->RecordPBBegin();
    }
}

void MJRecord::Finish()
{
    if (m_HasPBRecord)
    {
        this->RecordPBEnd();
    }
}

void MJRecord::RecordPBBegin()
{
    MJRecordItem item(MJRECORD_PB_FLAG);
    item.AddProperty("f", 1);
    item.AddProperty("msg", m_MsgID);
    this->AddItem(item);
}

void MJRecord::RecordPBAction(const google::protobuf::Message* ack, ISeatID seat)
{
    auto data = MJ::Base64Encode(ack->SerializeAsString());
    MJRecordItem item(MJRECORD_PB_FLAG);
    item.AddProperty("f", 2);
    item.AddProperty("s", seat);
    item.AddProperty("pb", data);
    this->AddItem(item);
}

void MJRecord::RecordPBResult(std::vector<ISeatID> const& winners)
{
    MJRecordItem item(MJRECORD_PB_FLAG);
    item.AddProperty("f", 3);
    item.AddProperty("w", MJ::ToString(winners));
    this->AddItem(item);
}

void MJRecord::RecordPBEnd()
{
    MJRecordItem item(MJRECORD_PB_FLAG);
    item.AddProperty("f", 0);
    this->AddItem(item);
}

void MJRecord::AddPlayerNetBreak(ISeatID seat)
{
    m_curAction.ResetAction(EAID_NetBreak);
    m_curAction.AddProperty("seat", seat);
    this->AddItem(m_curAction);
}

void MJRecord::AddPlayerNetResume(ISeatID seat)
{
    m_curAction.ResetAction(EAID_NetResume);
    m_curAction.AddProperty("seat", seat);
    this->AddItem(m_curAction);
}

void MJRecord::AddPlayerExitMatch(ISeatID seat)
{
    m_curAction.ResetAction(EAID_ExitMatch);
    m_curAction.AddProperty("seat", seat);
    this->AddItem(m_curAction);
}

bool MJRecord::HasPBRecord() const
{
    return m_HasPBRecord;
}

void MJRecord::AddItem(MJRecordItem const& item)
{
    auto duration = std::chrono::system_clock::now() - m_StartTime;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    std::string res = "<a rt=" + MJ::ToQuoteString(std::to_string(ms.count())) + " " + item.ToString() + "/>";
#if (MJ_DEBUG && 0)
    RecoreLog(res);
#endif

    m_Game->AddGameAction((char*)res.c_str());
}

void MJRecord::RecoreLog(const std::string& str)
{
    std::ofstream outfile("./mahjong_record.txt", std::ios::app);
    outfile << str.c_str() << std::endl;
    outfile.close();
}
