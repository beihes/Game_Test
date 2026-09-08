#include "../inc/time_string.hpp"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

namespace datetime {
    std::string SDL_Date_To_String(const SDL_DateTime& dateTime) {
        char date[16];
        SDL_snprintf(date, sizeof(date), "%04d%02d%02d", dateTime.year, dateTime.month, dateTime.day);
        return std::string(date);
    }

    std::string SDL_Time_To_String_In_Microsecond(const SDL_DateTime& dateTime) {
        char time[16];
        SDL_snprintf(time, sizeof(time), "%02d%02d%02d%06d", dateTime.hour, dateTime.minute, dateTime.second, SDL_NS_TO_US(dateTime.nanosecond));
        return std::string(time);
    }

    std::string SDL_Time_To_String_In_Second(const SDL_DateTime& dateTime) {
        char time[16];
        SDL_snprintf(time, sizeof(time), "%02d%02d%02d", dateTime.hour, dateTime.minute, dateTime.second);
        return std::string(time);
    }

    std::string SDL_DateTime_To_String_In_Microsecond(const SDL_DateTime& dateTime) {
        char time[32];
        SDL_snprintf(time, sizeof(time), "%04d%02d%02d%02d%02d%02d%06d", dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second, SDL_NS_TO_US(dateTime.nanosecond));
        return std::string(time);
    }

    std::string SDL_DateTime_To_String_In_Second(const SDL_DateTime& dateTime) {
        char time[16];
        SDL_snprintf(time, sizeof(time), "%04d%02d%02d%02d%02d%02d", dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
        return std::string(time);
    }

    std::string Get_Current_Time_String() {
        static uint64_t unknownCount = 0;
        SDL_Time nowTime;  // 获取当前纳秒时间戳
        SDL_DateTime dateTime;
        if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) { unknownCount++; return "Unknown-" + std::to_string(unknownCount); }
        return SDL_DateTime_To_String_In_Microsecond(dateTime);
    }

    std::string Get_Current_Time_String_In_Second() {
        static uint64_t unknownCount = 0;
        SDL_Time nowTime;  // 获取当前纳秒时间戳
        SDL_DateTime dateTime;
        if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) { unknownCount++; return "Unknown-" + std::to_string(unknownCount); }
        return SDL_DateTime_To_String_In_Second(dateTime);
    }

    std::string Get_Current_Date_String() {
        static uint64_t unknownCount = 0;
        SDL_Time nowTime = 0;  // 获取当前纳秒时间戳
        SDL_DateTime dateTime;
        if (!SDL_GetCurrentTime(&nowTime) || !SDL_TimeToDateTime(nowTime, &dateTime, true)) { unknownCount++; return "Unknown-" + std::to_string(unknownCount); }
        return SDL_Date_To_String(dateTime);
    }

    uint64_t Get_Timestamp_Microseconds() {
        SDL_Time nowTime = 0;  // 获取当前纳秒时间戳
        if (!SDL_GetCurrentTime(&nowTime)) { return 0; }
        return SDL_NS_TO_US(nowTime);
    }

    // template<typename Duration>
    // uint64_t Get_Timestamp() {
    //     SDL_Time nowTime = 0;  /* 获取当前纳秒时间戳 */
    //     if (!SDL_GetCurrentTime(&nowTime)) { return 0; }
    //     return std::chrono::duration_cast<Duration>(std::chrono::nanoseconds(nowTime)).count();
    // }
} // namespace datetime
