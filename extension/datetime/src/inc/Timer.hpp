#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#ifdef __cplusplus
}
#endif // __cplusplus

#include <atomic>
#include <mutex>

namespace datetime {
    class Timer final {
    public:
        Timer();
        ~Timer();
    public:
        //删除拷贝和移动构造函数及赋值运算符
        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        Timer(Timer&&) = delete;
        Timer& operator=(Timer&&) = delete;
    public:
        bool Start(Uint32 interval);
        bool Stop();
    public:
        void Update();
        void Start_FrameTime();
        void End_FrameTime();
    public:
        void Set_Interval(Uint32 interval);
        void Set_TimerState(bool midState);
        void Set_UserCallback(SDL_TimerCallback midCallback);
    public:
        SDL_TimerID Get_TimerID() const; //获取定时器ID
        Uint32 Get_Interval() const;     //获取目标帧时间，单位毫秒
        bool Get_TimerState();     //获取定时器状态
    public:
        Uint64 Get_NowTime_NS_Uint64();
        Uint64 Get_FrameTime_NS_Uint64();
        double Get_FrameTime_Ms_Double();
        double Get_FrameTime_S_Double();
        float Get_FrameTime_S_Float();
    private:
        SDL_TimerID timerID_ = 0;
        Uint32 interval_ = 0;       //目标帧时间，单位毫秒
        Uint64 startTime_ = 0;/* 开始时间 */
        Uint64 currentTime_ = 0;/* 当前时间 */
        Uint64 stopTime_ = 0;/* 停止时间 */
        Uint64 startFrameTime_ = 0;
        bool startFrameTimeState_ = false;
        Uint64 endFrameTime_ = 0;
        bool endFrameTimeState_ = true;
        Uint64 frameTime_ = 0;/* 帧时间 */
        std::atomic<bool> timerState_ = false;
        SDL_TimerCallback userCallback_ = nullptr;
    private:
        static Uint32 TimerCallbackFunction(void* userdata, SDL_TimerID timerID, Uint32 interval);
    };
} // namespace datetime
