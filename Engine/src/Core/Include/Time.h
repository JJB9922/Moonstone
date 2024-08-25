#ifndef TIME_H
#define TIME_H

namespace Moonstone
{

namespace Core
{

class Time
{
    public:
        static Time& GetInstance()
        {
            static Time instance;
            return instance;
        }

        inline float GetDeltaTime() const { return m_DeltaTime; }
        inline float GetLastFrame() const { return m_LastFrame; }

        void Update(float currentTime)
        {
            m_DeltaTime = currentTime - m_LastFrame;
            m_LastFrame = currentTime;
        }

        void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
        void SetLastFrame(float lastFrame) { m_LastFrame = lastFrame; }

    private:
        Time()                       = default;
        Time(const Time&)            = delete;
        Time& operator=(const Time&) = delete;

        float m_DeltaTime = 0.0f;
        float m_LastFrame = 0.0f;
};

} // namespace Core

} // namespace Moonstone

#endif // TIME_H
