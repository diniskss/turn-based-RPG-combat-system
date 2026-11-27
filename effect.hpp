#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace combat
{

// Эффект (бафф/дебафф), который накладывает предмет.
// В BaseItem хранится через std::shared_ptr: это вложенный объект,
// на котором видна разница между поверхностной и глубокой копией предмета.
//
// Правило нуля: поля std::string и std::int32_t сами умеют копироваться
// и освобождаться, поэтому ни одна из пяти специальных операций не написана.
class Effect
{
private:
    std::string m_name;
    std::int32_t m_duration;

public:
    Effect(std::string_view name, std::int32_t duration);

    Effect() = delete;

    [[nodiscard]] std::string_view GetName() const;
    [[nodiscard]] std::int32_t GetDuration() const;

    // Правило: длительность эффекта не бывает отрицательной
    void SetDuration(std::int32_t duration);

    [[nodiscard]] std::string Describe() const;
};

} // namespace combat
