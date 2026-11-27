#include "effect.hpp"

namespace combat
{

Effect::Effect(std::string_view name, std::int32_t duration)
    : m_name(name)
    , m_duration(duration < 0 ? 0 : duration)
{
}

std::string_view Effect::GetName() const
{
    return m_name;
}

std::int32_t Effect::GetDuration() const
{
    return m_duration;
}

void Effect::SetDuration(std::int32_t duration)
{
    // Правило: длительность эффекта не бывает отрицательной
    m_duration = duration < 0 ? 0 : duration;
}

std::string Effect::Describe() const
{
    return m_name + " (" + std::to_string(m_duration) + " rounds)";
}

} // namespace combat
