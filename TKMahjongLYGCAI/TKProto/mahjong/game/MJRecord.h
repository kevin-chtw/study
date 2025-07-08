#pragma once

#include <google/protobuf/message.h>
#include <memory>
#include <chrono>
#include "MJModule.h"
#include "MJString.h"

static constexpr int MJRECORD_PB_FLAG = 512;  // f=<0:begin,1:end>

class MJGame;
struct MJRecordItem
{
public:
    MJRecordItem(int actionID = 0);
    MJRecordItem(std::string const& actionName);
    void ResetAction(int actionID);
    void ResetAction(std::string const& actionName);

    template <typename Type>
    void AddProperty(std::string const& name, Type const& value)
    {
        m_Properties[name] = MJ::ToString(value);
    }

    std::string ToString() const;

    int m_ActionID{};
    std::string m_ActionName;
    std::map<std::string, std::string> m_Properties;
};

class MJRecord : public MJGameModule
{
public:
    using MJGameModule::MJGameModule;

    virtual ~MJRecord() = default;

public:
    // if pb=false, ignore msgID; else set msgID like TK_MSG_GAME_MJYJ
    virtual void Initialize();

    void Finish();  // call this on game_over.

    virtual void RecordAction(const google::protobuf::Message* ack, ISeatID seat)
    {
    }

    //特殊,断线，重连，退赛
    virtual void AddPlayerNetBreak(ISeatID seat);
    virtual void AddPlayerNetResume(ISeatID seat);
    virtual void AddPlayerExitMatch(ISeatID seat);

    bool HasPBRecord() const;

protected:
    void AddItem(MJRecordItem const& item);
    void RecoreLog(const std::string& str);

protected:
    void RecordPBBegin();                                                     // f="1", msg="msgid"
    void RecordPBAction(const google::protobuf::Message* ack, ISeatID seat);  // f="2",s="seat", pb="..."
    void RecordPBResult(std::vector<ISeatID> const& winners);                 // f="3",w="winners"
    void RecordPBEnd();                                                       // f="0"

protected:
    std::chrono::system_clock::time_point m_StartTime = std::chrono::system_clock::now();
    MJRecordItem m_curAction;
    int m_MsgID{};
    bool m_HasPBRecord{};
};
