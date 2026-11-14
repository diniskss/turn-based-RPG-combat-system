#pragma once

#include <vector>
#include <string>
#include <string_view>
#include "fighter.hpp"
#include <memory>

namespace combat
{

class Suite
{
private:
    static constexpr auto TAG = "suite";

    std::string m_name;
    std::vector<std::weak_ptr<Fighter>> m_fighters;

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
    void AddFighter(const std::shared_ptr<Fighter>& fighter);
    
    // Содержательный метод: проверка состояния отряда
    void InspectSuite() const;
};

} // namespace combat