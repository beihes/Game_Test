#pragma once

#include <string>

namespace datetime {
    /* 获取当前时间字符串 */
    std::string Get_Current_Time_String();/* 获取当前时间字符串 */
    std::string Get_Current_Time_String_In_Second();/* 获取当前时间字符串（秒） */
    std::string Get_Current_Date_String();/* 获取当前日期字符串 */

    uint64_t Get_Timestamp_Microseconds();/* 获取时间戳（微秒） */
    // template<typename Duration>
    // uint64_t Get_Timestamp();
} // namespace datetime
