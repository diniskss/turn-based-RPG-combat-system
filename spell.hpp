#pragma once

#include <string>
#include <string_view>
#include <cstdint>

namespace combat
{

class Spell
{
private:
    static constexpr auto TAG = "spell";

    std::string m_name;
    std::int32_t m_manaCost;

public:
    Spell(std::string_view name, std::int32_t manaCost);

    Spell() = delete;
    Spell(const Spell&) = delete;
    Spell(Spell&&) = delete;
    Spell& operator=(const Spell&) = delete;
    Spell& operator=(Spell&&) = delete;

    ~Spell();

    [[nodiscard]] std::string_view GetName() const;
    [[nodiscard]] std::int32_t GetManaCost() const;

    void Use() const;
};

} // namespace combat