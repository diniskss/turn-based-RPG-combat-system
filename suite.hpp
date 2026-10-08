#pragma once

#include <vector>
#include <string>
#include <string_view>
#include "fighter.hpp"

namespace combat
{

class Suite
{
private:
    static constexpr auto TAG = "suite";

    std::string m_name;
    std::vector<Fighter*> m_fighters;

public:
    explicit Suite(std::string_view name);

    Suite() = delete;
    Suite(const Suite&) = delete;
    Suite(Suite&&) = delete;
    Suite& operator=(const Suite&) = delete;
    Suite& operator=(Suite&&) = delete;
    
    ~Suite();

    [[nodiscard]] std::string_view GetName() const;

    // Добавление внешнего бойца (агрегация)
    void AddFighter(Fighter* fighter);
    
    // Содержательный метод: проверка состояния отряда
    void InspectSuite() const;
};

} // namespace combat