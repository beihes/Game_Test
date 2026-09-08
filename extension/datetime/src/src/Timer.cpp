#include "../inc/Timer.hpp"

namespace datetime {
    Timer::Timer() {

    }

    Timer::~Timer() {
        if (this->timerState_) {
            this->Stop();
        }
    }

    bool Timer::Start(Uint32 interval) {
        if (this->timerState_) { return false; }
        this->timerID_ = SDL_AddTimer(interval, Timer::TimerCallbackFunction, this);
        if (!this->timerID_) { return false; }
        this->timerState_ = false;
        this->interval_ = interval;
        this->currentTime_ = SDL_GetTicksNS();
        this->startTime_ = this->currentTime_;
        return true;
    }

    bool Timer::Stop() {
        if (!SDL_RemoveTimer(this->timerID_)) { return false; }
        this->timerState_ = false;
        return true;
    }

    void Timer::Update() {
        this->currentTime_ = SDL_GetTicksNS();
    }

    void Timer::Start_FrameTime() {
        if (this->endFrameTimeState_) {
            this->startFrameTime_ = SDL_GetTicksNS();
            this->startFrameTimeState_ = true;
            this->endFrameTimeState_ = false;
        }
    }
    void Timer::End_FrameTime() {
        if (this->startFrameTimeState_) {
            this->endFrameTime_ = SDL_GetTicksNS();
            this->frameTime_ = this->endFrameTime_ - this->startFrameTime_;
            this->startFrameTimeState_ = false;
            this->endFrameTimeState_ = true;
        }
    }

    void Timer::Set_Interval(Uint32 interval) {
        this->interval_ = interval;
    }

    void Timer::Set_TimerState(bool midState) {
        this->timerState_.store(midState);
    }

    void Timer::Set_UserCallback(SDL_TimerCallback midCallback) {
        this->userCallback_ = midCallback;
    }

    SDL_TimerID Timer::Get_TimerID() const {
        return this->timerID_;
    }

    Uint32 Timer::Get_Interval() const {
        return this->interval_;
    }

    bool Timer::Get_TimerState() {
        return this->timerState_.exchange(false);
        // return this->timerState_.load();
    }

    Uint64 Timer::Get_NowTime_NS_Uint64() {
        return SDL_GetTicksNS();
    }

    Uint64 Timer::Get_FrameTime_NS_Uint64() {
        return this->frameTime_;
    }

    double Timer::Get_FrameTime_Ms_Double() {
        return this->frameTime_ / 1000000.0;
    }

    double Timer::Get_FrameTime_S_Double() {
        return this->frameTime_ / 1000000000.0;
    }

    float Timer::Get_FrameTime_S_Float() {
        return static_cast<float>(this->frameTime_ / 1000000000.0);
    }

    Uint32 Timer::TimerCallbackFunction(void* userdata, SDL_TimerID timerID, Uint32 interval) {
        auto midData = static_cast<Timer*>(userdata);
        midData->timerState_.store(true);
        if (midData->userCallback_) {
            return midData->userCallback_(userdata, timerID, interval);
        }
        return interval;
    }
} // namespace datetime
