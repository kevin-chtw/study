#pragma once

#define NOMINMAX  // ban the min max  from "Windows.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <algorithm>
using std::max;
using std::min;
#include "TKMahjongLYGC.pb.h"

#include "MJDefine.h"
#include "LYGCDefine.h"
#include "json/json.h"

using namespace cn::jj::service::msg::protocol;

#define DEFAULT_AI_SERVICE_URL "http://test.ml.jjweb.cn:80/api/v1/lygc/"

#define DEFAULT_AI_URL_KEY "TKMahjongLYGCBotUrl"
#define BOT_ERROR "BotError:"

using UserID = unsigned int;
constexpr size_t MAX_PATH_BUF_LEN = 256;

constexpr long long DefaultTimerInterval = 500;   // time ms
constexpr long long DefaultAiExpiredTime = 3000;  // time ms
constexpr long long NormaltAiExpiredTime = 3000;  // time ms
constexpr long long AiExpiredTimRage = 500;       // time ms

inline bool IsValidSeat(ISeatID seat)
{
    return (seat >= 0 && seat < NP4);
}

enum class AIRequestType
{
    Move,
    BetFish,
};
