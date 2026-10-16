// Отчёт по лабораторной работе №2 (ООП, C++)
// Компиляция: typst compile report.typ

#set document(title: "Лабораторная работа №2. Классы, агрегация и композиция")
#set text(lang: "ru", size: 14pt, font: ("Times New Roman", "Liberation Serif", "Tinos", "DejaVu Serif"))
#set par(justify: true, leading: 0.8em, first-line-indent: (amount: 1.25cm, all: true))
#set page(paper: "a4", margin: (left: 30mm, right: 10mm, top: 20mm, bottom: 20mm))
#set heading(numbering: "1.1")
#set figure.caption(separator: [ — ])
#set table(stroke: 0.5pt, inset: 6pt)

#show heading: set text(size: 14pt, weight: "bold")
#show heading: it => block(above: 1.2em, below: 0.8em)[#it]
#show heading.where(level: 1): it => block(above: 1.4em, below: 0.9em)[#it]
#show raw: set text(font: ("JetBrains Mono", "DejaVu Sans Mono", "Liberation Mono"))
#show raw.where(block: false): set text(size: 12pt)
#show raw.where(block: true): set text(size: 9pt)
#show raw.where(block: true): it => block(width: 100%, fill: luma(245), stroke: 0.4pt + luma(200), inset: 8pt, radius: 2pt, breakable: true, it)
#show figure.where(kind: raw): set block(breakable: true)
#show figure.where(kind: raw): set figure.caption(position: top)
#show figure.where(kind: table): set figure.caption(position: top)
#show figure.where(kind: table): set block(breakable: true)
#show table: set text(size: 12pt)
#show table: set par(justify: false, first-line-indent: 0pt)
#show figure: set par(first-line-indent: 0pt)

// ---------------- ТИТУЛЬНЫЙ ЛИСТ ----------------
#page(numbering: none)[
  #set par(first-line-indent: 0pt, justify: false)
  #set align(center)
  #set text(size: 14pt)

  МИНИСТЕРСТВО НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ \
  Федеральное государственное бюджетное образовательное учреждение высшего образования \
  *«Алтайский государственный технический университет им. И. И. Ползунова»* \
  (АлтГТУ)

  #v(1em)
  Кафедра #box(width: 8cm, height: 1em, stroke: (bottom: 0.5pt))

  #v(1fr)
  *ОТЧЁТ* \
  по лабораторной работе №2 \
  по дисциплине «Объектно-ориентированное программирование»

  #v(0.8em)
  Тема: «Классы, агрегация и композиция. \
  Язык программирования с ручным управлением памятью»

  #v(1fr)
  #align(right)[
    #block(width: 11cm)[
      #set align(left)
      #set par(first-line-indent: 0pt)
      Выполнил: студент группы ПИ-53, \
      направление 09.03.04 «Программная инженерия» \
      #v(0.4em)
      #box(width: 3.8cm, height: 1em, stroke: (bottom: 0.5pt)) / #box(width: 4.2cm, height: 1em, stroke: (bottom: 0.5pt)) / \
      #v(0.9em)
      Проверил: \
      #v(0.4em)
      #box(width: 3.8cm, height: 1em, stroke: (bottom: 0.5pt)) / #box(width: 4.2cm, height: 1em, stroke: (bottom: 0.5pt)) /
    ]
  ]

  #v(2fr)
  Барнаул, 2026
]

#set page(numbering: "1", number-align: center + bottom)

// ---------------- СОДЕРЖАНИЕ ----------------
#outline(title: "Содержание", indent: auto)
#pagebreak()

= Цель работы

Освоить создание классов в C++ и реализацию связей между ними (агрегация и композиция), а также ручное управление памятью: операторы `new` и `delete`, порядок вызова конструкторов и деструкторов, различие стековых и динамических объектов.

= Постановка задачи

Реализовать три связанных класса предметной области «бой»:

- `Suite` (свита) хранит бойцов;
- `Fighter` (боец) владеет заклинанием;
- `Spell` (заклинание).

Связи между классами:

- *агрегация* `Suite` → `Fighter`: свита хранит указатели на бойцов, которые создаются снаружи и живут независимо от неё;
- *композиция* `Fighter` → `Spell`: заклинание является частью бойца и уничтожается вместе с ним.

Требования к реализации: инкапсуляция (закрытые поля, доступ через методы), отсутствие глобальных переменных, разделение на `.hpp` и `.cpp`, защита заголовков `#pragma once`, у классов есть содержательные методы с проверкой правил предметной области. В `main` нужно показать статическую и динамическую инициализацию, работу через ссылку и указатель, агрегацию, массив динамических объектов и композицию.

= Теоретические сведения

== Класс и инкапсуляция

Класс описывает тип данных: поля (состояние) и методы (поведение). Спецификаторы `private` и `public` ограничивают доступ: поля закрыты, менять состояние можно только через методы, которые проверяют допустимость операции. Методы, не меняющие объект, помечаются `const`.

== Конструкторы, деструкторы, список инициализации

Конструктор вызывается при создании объекта, деструктор при уничтожении. *Список инициализации* (`: m_id(id), ...`) задаёт значения полей до входа в тело конструктора. Он обязателен для полей-объектов без конструктора по умолчанию. Порядок инициализации определяется порядком объявления полей в классе, а не порядком в списке.

== Стек и куча

Объект, объявленный как локальная переменная, лежит на стеке и уничтожается автоматически при выходе из блока `{ }`. Объект, созданный через `new`, лежит в куче: `new` выделяет память и вызывает конструктор, а `delete` вызывает деструктор и освобождает память. Если `delete` не вызван, происходит утечка памяти; повторный `delete` приводит к неопределённому поведению.

== Агрегация и композиция

#figure(
  table(
    columns: (auto, 1fr, 1fr),
    align: (left, left, left),
    table.header([*Признак*], [*Агрегация*], [*Композиция*]),
    [Отношение], [«имеет» (слабое)], [«состоит из» (сильное)],
    [Владение], [целое не владеет частью], [целое владеет частью],
    [Время жизни], [часть живёт независимо], [часть умирает вместе с целым],
    [В коде], [указатель или ссылка], [поле-объект внутри класса],
    [Пример], [`Suite` → `Fighter`], [`Fighter` → `Spell`],
  ),
  caption: [Сравнение агрегации и композиции],
)

= Реализация

== Структура проекта

#figure(
  table(
    columns: (auto, 1fr),
    table.header([*Файл*], [*Назначение*]),
    [`spell.hpp`, `spell.cpp`], [класс `Spell`: название и стоимость заклинания в мане],
    [`fighter.hpp`, `fighter.cpp`], [класс `Fighter`: характеристики бойца, получение урона, применение заклинания],
    [`suite.hpp`, `suite.cpp`], [класс `Suite`: список указателей на бойцов],
    [`main.cpp`], [демонстрация всех требований лабораторной],
  ),
  caption: [Файлы проекта],
)

Все классы находятся в пространстве имён `combat`. Глобальных переменных нет.

== Диаграмма классов

#let cls(name, fields, methods) = block(width: 100%, stroke: 0.7pt, breakable: false)[
  #set par(first-line-indent: 0pt, justify: false, leading: 0.5em)
  #set align(left)
  #align(center, pad(y: 4pt, strong(text(size: 11pt, name))))
  #line(length: 100%, stroke: 0.7pt)
  #pad(x: 5pt, y: 4pt, text(size: 8.5pt, fields.join(linebreak())))
  #line(length: 100%, stroke: 0.7pt)
  #pad(x: 5pt, y: 4pt, text(size: 8.5pt, methods.join(linebreak())))
]

#let rel(label, filled) = align(center + horizon)[
  #set par(first-line-indent: 0pt, justify: false)
  #text(size: 9pt, label)
  #v(2pt)
  #box(width: 100%, height: 11pt)[
    #place(left + horizon, line(start: (6pt, 0pt), length: 100% - 6pt, stroke: 0.8pt))
    #place(left + horizon, polygon(fill: if filled { black } else { white }, stroke: 0.8pt, (0pt, 0pt), (6pt, -4pt), (12pt, 0pt), (6pt, 4pt)))
    #place(right + horizon, polygon(fill: black, stroke: 0.8pt, (0pt, 0pt), (-7pt, -3.5pt), (-7pt, 3.5pt)))
  ]
]

#figure(
  grid(
    columns: (1fr, 0.55fr, 1fr, 0.55fr, 1fr),
    align: horizon,
    column-gutter: 2pt,
    cls("Suite",
      ("-m_name: std::string", "-m_fighters: vector<Fighter*>"),
      ("+GetName()", "+AddFighter(Fighter*)", "+InspectSuite()")),
    rel([агрегация], false),
    cls("Fighter",
      ("-m_id, -m_health, -m_mana", "-m_maxHealth, -m_maxMana", "-m_actionPoints", "-m_spell: Spell"),
      ("+GetId(), GetHealth()", "+GetMana(), GetActionPoints()", "+GetSpell()", "+TakeDamage(amount)", "+CastSpell(): bool")),
    rel([композиция], true),
    cls("Spell",
      ("-m_name: std::string", "-m_manaCost: int32_t"),
      ("+GetName()", "+GetManaCost()", "+Use()")),
  ),
  caption: [Диаграмма классов (ромб пустой: агрегация, закрашенный: композиция)],
)

== Класс `Spell`

#figure(
  table(
    columns: (auto, auto, 1fr),
    table.header([*Член*], [*Тип*], [*Назначение*]),
    [`m_name`], [`std::string`], [название заклинания],
    [`m_manaCost`], [`std::int32_t`], [стоимость в мане],
    [`GetName()`, `GetManaCost()`], [`const`-геттеры], [чтение закрытых полей],
    [`Use()`], [`void`, `const`], [выводит сообщение об использовании заклинания],
  ),
  caption: [Класс `Spell`],
)

== Класс `Fighter`

#figure(
  table(
    columns: (auto, auto, 1fr),
    table.header([*Член*], [*Тип*], [*Назначение*]),
    [`m_id`], [`std::int32_t`], [уникальный идентификатор бойца],
    [`m_health`, `m_maxHealth`], [`std::int32_t`], [текущее и максимальное здоровье],
    [`m_mana`, `m_maxMana`], [`std::int32_t`], [текущий и максимальный запас маны],
    [`m_actionPoints`], [`std::int32_t`], [очки действий],
    [`m_spell`], [`Spell`], [*композиция*: заклинание хранится по значению внутри бойца],
    [`TakeDamage(amount)`], [`void`], [отнимает здоровье; правило: здоровье не опускается ниже 0],
    [`CastSpell()`], [`bool`], [применяет своё заклинание; правило: стоимость не должна превышать текущую ману, иначе отказ (`false`)],
  ),
  caption: [Класс `Fighter`],
)

Заклинание создаётся в списке инициализации конструктора `Fighter` (`m_spell(spellName, spellCost)`), то есть до выполнения тела конструктора. Это единственный способ его создать, так как конструктор по умолчанию у `Spell` удалён. Деструктор поля `m_spell` вызывается автоматически после тела `~Fighter()`.

== Класс `Suite`

#figure(
  table(
    columns: (auto, auto, 1fr),
    table.header([*Член*], [*Тип*], [*Назначение*]),
    [`m_name`], [`std::string`], [название свиты],
    [`m_fighters`], [`std::vector<Fighter*>`], [*агрегация*: хранит только адреса бойцов],
    [`AddFighter(fighter)`], [`void`], [добавляет бойца; нулевой указатель игнорируется],
    [`InspectSuite()`], [`void`, `const`], [выводит состав свиты и параметры бойцов],
  ),
  caption: [Класс `Suite`],
)

В деструкторе `~Suite()` нет `delete`: свита не создавала бойцов и не владеет ими, поэтому удаляет их тот, кто выполнил `new` (в нашем случае `main`).

== Применённые приёмы

- `#pragma once` защищает заголовки от повторного включения (`suite.hpp` включает `fighter.hpp`, а `main.cpp` включает оба).
- `= delete` для конструктора по умолчанию: объект не имеет смысла без характеристик.
- `= delete` для копирования и перемещения: у бойца есть идентичность (`m_id`), копия дала бы двух бойцов с одним идентификатором. У `Suite` копирование запрещено, чтобы не копировать список указателей без договорённости о владении.
- `explicit` у конструктора `Suite(std::string_view)` запрещает неявное преобразование строки в свиту.
- `[[nodiscard]]` у геттеров и `CastSpell()` заставляет компилятор предупреждать, если результат вызова проигнорирован.
- `const` у методов: гарантия, что метод не меняет объект; такие методы можно вызывать через `const`-указатель (как в `InspectSuite()`).
- `std::string_view` в параметрах конструкторов позволяет принимать строку без копирования, а поля хранят `std::string` (собственная копия).

= Демонстрация работы программы

== Сборка и запуск

Сборка выполняется компилятором `g++` (задача VS Code `C/C++: g++ build project`):

```bash
g++ -g *.cpp -o main
./main
```

== Блоки демонстрации в `main`

#figure(
  table(
    columns: (auto, 1fr),
    table.header([*Блок*], [*Что демонстрирует*]),
    [1], [объект на стеке (статическая инициализация), доступ через ссылку `Fighter&` и указатель `Fighter*`],
    [2], [объект в куче (`new`), агрегация: боец передаётся в свиту, затем свита уничтожается при выходе из вложенного блока],
    [3], [проверка, что боец жив после уничтожения свиты; явный `delete`],
    [4], [массив указателей на динамические объекты, удаление каждого элемента],
    [5], [композиция: порядок вызова деструкторов бойца и его заклинания],
  ),
  caption: [Блоки программы `main`],
)

== Вывод программы

#figure(caption: [Консольный вывод])[
```text
=== 1. Static initialization & pointer/reference demo ===
[Spell] Created spell: Fireball
[Fighter] Created fighter ID: 1
[Fighter] ID: 1 took 30 damage. HP: 100 -> 70
[Spell] Using spell 'Fireball' (Cost: 20)
[Fighter] ID: 1 successfully cast spell. Remaining mana: 30

=== 2. Dynamic initialization (new / delete) & Aggregation demo ===
[Spell] Created spell: Ice Lance
[Fighter] Created fighter ID: 2
[Suite] Created suite: Alpha Squad
[Suite] Fighter added to suite Alpha Squad
[Suite] Inspecting suite 'Alpha Squad'. Total fighters: 1
  - Fighter ID: 2, HP: 120, Mana: 80
[Spell] Using spell 'Ice Lance' (Cost: 50)
[Fighter] ID: 2 successfully cast spell. Remaining mana: 30
[Fighter Error] ID: 2 cannot cast spell 'Ice Lance': required mana 50, available mana 30 (Action rejected)

--- Leaving suite scope (Suite destruction) ---
[Suite] Destroyed suite: Alpha Squad (fighters remain intact)

=== 3. Verification that aggregated object is still alive ===
External fighter HP after suite is gone: 120
[Fighter] Destroyed fighter ID: 2
[Spell] Destroyed spell: Ice Lance

=== 4. Dynamic array and array of dynamic objects demo ===
[Spell] Created spell: Spark
[Fighter] Created fighter ID: 3
[Spell] Created spell: Shield
[Fighter] Created fighter ID: 4
[Fighter] ID: 3 took 50 damage. HP: 90 -> 40
[Spell] Using spell 'Shield' (Cost: 10)
[Fighter] ID: 4 successfully cast spell. Remaining mana: 50
[Fighter] Destroyed fighter ID: 3
[Spell] Destroyed spell: Spark
[Fighter] Destroyed fighter ID: 4
[Spell] Destroyed spell: Shield

=== 5. Composition demo (destruction order) ===
[Spell] Created spell: Lightning
[Fighter] Created fighter ID: 5
--- Leaving block (Fighter destruction) ---
[Fighter] Destroyed fighter ID: 5
[Spell] Destroyed spell: Lightning
=== Program finished successfully ===
[Fighter] Destroyed fighter ID: 1
[Spell] Destroyed spell: Fireball
```
]

== Анализ вывода

*Агрегация (блоки 2 и 3).* При выходе из вложенного блока уничтожается только свита:

```text
[Suite] Destroyed suite: Alpha Squad (fighters remain intact)
External fighter HP after suite is gone: 120
[Fighter] Destroyed fighter ID: 2
```

Сообщения `Destroyed fighter` в момент уничтожения свиты нет, боец продолжает отвечать на вызовы (`HP: 120`) и уничтожается только после явного `delete externalFighter`. Время жизни бойца не зависит от свиты.

*Композиция (блок 5).* Для бойца на стеке порядок такой:

```text
[Spell] Created spell: Lightning
[Fighter] Created fighter ID: 5
--- Leaving block (Fighter destruction) ---
[Fighter] Destroyed fighter ID: 5
[Spell] Destroyed spell: Lightning
```

Заклинание создаётся раньше бойца (в списке инициализации) и уничтожается позже (после тела деструктора). Для динамических бойцов (блоки 2 и 4) каждый `delete` тоже приводит к паре сообщений `Destroyed fighter` и `Destroyed spell`, то есть заклинание не может пережить своего владельца.

*Проверка правил.* Блок 2: у бойца 80 маны, первое заклинание стоит 50 и проходит (остаётся 30), второе отклоняется, так как 30 < 50. Блок 1 и 4: `TakeDamage` уменьшает здоровье (`100 -> 70`, `90 -> 40`).

*Завершение программы.* Строки `Destroyed fighter ID: 1` и `Destroyed spell: Fireball` появляются после сообщения «Program finished»: боец `staticFighter` лежит на стеке `main` и уничтожается при выходе из функции, то есть после `return 0`.

= Вывод

В ходе работы реализованы три класса `Spell`, `Fighter` и `Suite` с инкапсуляцией, разделением на заголовочные файлы и файлы реализации и проверкой правил предметной области. Агрегация между `Suite` и `Fighter` реализована через указатели без владения: уничтожение свиты не затрагивает бойцов. Композиция между `Fighter` и `Spell` реализована полем-объектом: заклинание создаётся и уничтожается вместе с бойцом, порядок вызова конструкторов и деструкторов подтверждён выводом программы. Освоено ручное управление памятью: объекты на стеке уничтожаются автоматически, объекты в куче требуют явного `delete`, а неверный порядок удаления при агрегации с сырыми указателями может привести к висячим указателям.

Репозиторий проекта: https://github.com/diniskss/turn-based-RPG-combat-system.git.

#pagebreak()

= Приложение А. Исходный код

#figure(caption: [Файл `spell.hpp`])[
```cpp
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
```
]

#figure(caption: [Файл `spell.cpp`])[
```cpp
#include "spell.hpp"
#include <iostream>

namespace combat
{

Spell::Spell(std::string_view name, std::int32_t manaCost)
    : m_name(name)
    , m_manaCost(manaCost)
{
    std::cout << "[Spell] Created spell: " << m_name << "\n";
}

Spell::~Spell()
{
    std::cout << "[Spell] Destroyed spell: " << m_name << "\n";
}

std::string_view Spell::GetName() const
{
    return m_name;
}

std::int32_t Spell::GetManaCost() const
{
    return m_manaCost;
}

void Spell::Use() const
{
    std::cout << "[Spell] Using spell '" << m_name << "' (Cost: " << m_manaCost << ")\n";
}

} // namespace combat
```
]

#figure(caption: [Файл `fighter.hpp`])[
```cpp
#pragma once

#include <cstdint>
#include <string_view>
#include "spell.hpp"

namespace combat
{

class Fighter
{
private:
    static constexpr auto TAG = "fighter";

    std::int32_t m_id;
    std::int32_t m_health;
    std::int32_t m_maxHealth;
    std::int32_t m_mana;
    std::int32_t m_maxMana;
    std::int32_t m_actionPoints;

    // Композиция: заклинание принадлежит бойцу и живёт ровно столько же
    Spell m_spell;

public:
    Fighter(std::int32_t id, std::int32_t health, std::int32_t mana, std::int32_t actionPoints,
            std::string_view spellName, std::int32_t spellCost);

    Fighter() = delete;
    Fighter(const Fighter&) = delete;
    Fighter(Fighter&&) = delete;
    Fighter& operator=(const Fighter&) = delete;
    Fighter& operator=(Fighter&&) = delete;

    ~Fighter();

    [[nodiscard]] std::int32_t GetId() const;
    [[nodiscard]] std::int32_t GetHealth() const;
    [[nodiscard]] std::int32_t GetMana() const;
    [[nodiscard]] std::int32_t GetActionPoints() const;
    [[nodiscard]] const Spell& GetSpell() const;

    // Содержательный метод 1: получение урона с проверкой (здоровье >= 0)
    void TakeDamage(std::int32_t amount);

    // Содержательный метод 2: применение своего заклинания с проверкой (хватает ли маны)
    [[nodiscard]] bool CastSpell();
};

} // namespace combat
```
]

#figure(caption: [Файл `fighter.cpp`])[
```cpp
#include "fighter.hpp"
#include <iostream>

namespace combat
{

Fighter::Fighter(std::int32_t id, std::int32_t health, std::int32_t mana, std::int32_t actionPoints,
                 std::string_view spellName, std::int32_t spellCost)
    : m_id(id)
    , m_health(health)
    , m_maxHealth(health)
    , m_mana(mana)
    , m_maxMana(mana)
    , m_actionPoints(actionPoints)
    , m_spell(spellName, spellCost)   // Spell создаётся здесь, ДО тела конструктора
{
    std::cout << "[Fighter] Created fighter ID: " << m_id << "\n";
}

Fighter::~Fighter()
{
    // m_spell уничтожится автоматически ПОСЛЕ выполнения этого тела
    std::cout << "[Fighter] Destroyed fighter ID: " << m_id << "\n";
}

std::int32_t Fighter::GetId() const { return m_id; }
std::int32_t Fighter::GetHealth() const { return m_health; }
std::int32_t Fighter::GetMana() const { return m_mana; }
std::int32_t Fighter::GetActionPoints() const { return m_actionPoints; }
const Spell& Fighter::GetSpell() const { return m_spell; }

void Fighter::TakeDamage(std::int32_t amount)
{
    const auto oldHealth = m_health;
    m_health -= amount;

    // Правило: здоровье не может быть меньше 0
    if (m_health < 0)
    {
        m_health = 0;
    }
    std::cout << "[Fighter] ID: " << m_id << " took " << amount
              << " damage. HP: " << oldHealth << " -> " << m_health << "\n";
}

bool Fighter::CastSpell()
{
    const auto cost = m_spell.GetManaCost();

    // Правило: стоимость заклинания не может превышать текущий запас маны
    if (m_mana < cost)
    {
        std::cout << "[Fighter Error] ID: " << m_id
                  << " cannot cast spell '" << m_spell.GetName() << "': required mana " << cost
                  << ", available mana " << m_mana << " (Action rejected)\n";
        return false;
    }

    m_mana -= cost;
    m_spell.Use();
    std::cout << "[Fighter] ID: " << m_id << " successfully cast spell. Remaining mana: " << m_mana << "\n";
    return true;
}

} // namespace combat
```
]

#figure(caption: [Файл `suite.hpp`])[
```cpp
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
```
]

#figure(caption: [Файл `suite.cpp`])[
```cpp
#include "suite.hpp"
#include <iostream>

namespace combat
{

Suite::Suite(std::string_view name)
    : m_name(name)
{
    std::cout << "[Suite] Created suite: " << m_name << "\n";
}

Suite::~Suite()
{
    // Агрегация: при уничтожении свиты сами бойцы НЕ удаляются через delete, так как они существуют независимо от свиты.
    std::cout << "[Suite] Destroyed suite: " << m_name << " (fighters remain intact)\n";
}

std::string_view Suite::GetName() const
{
    return m_name;
}

void Suite::AddFighter(Fighter* fighter)
{
    if (fighter != nullptr)
    {
        m_fighters.push_back(fighter);
        std::cout << "[Suite] Fighter added to suite " << m_name << "\n";
    }
}

void Suite::InspectSuite() const
{
    std::cout << "[Suite] Inspecting suite '" << m_name << "'. Total fighters: " << m_fighters.size() << "\n";
    for (const auto* fighter : m_fighters)
    {
        if (fighter != nullptr)
        {
            std::cout << "  - Fighter ID: " << fighter->GetId() 
                      << ", HP: " << fighter->GetHealth() 
                      << ", Mana: " << fighter->GetMana() << "\n";
        }
    }
}

} // namespace combat
```
]

#figure(caption: [Файл `main.cpp`])[
```cpp
#include <iostream>
#include "fighter.hpp"
#include "suite.hpp"

int main()
{
    std::cout << "=== 1. Static initialization & pointer/reference demo ===\n";
    combat::Fighter staticFighter(1, 100, 50, 1, "Fireball", 20);
    combat::Fighter& refFighter = staticFighter;
    combat::Fighter* ptrFighter = &staticFighter;

    refFighter.TakeDamage(30);
    ptrFighter->CastSpell();

    std::cout << "\n=== 2. Dynamic initialization (new / delete) & Aggregation demo ===\n";
    combat::Fighter* externalFighter = new combat::Fighter(2, 120, 80, 1, "Ice Lance", 50);

    {
        combat::Suite mySuite("Alpha Squad");
        mySuite.AddFighter(externalFighter);
        mySuite.InspectSuite();

        externalFighter->CastSpell(); // 80 -> 30, должно пройти
        externalFighter->CastSpell(); // 30 < 50, должно отказать

        std::cout << "\n--- Leaving suite scope (Suite destruction) ---\n";
    } // Уничтожается mySuite. Боец НЕ должен уничтожиться!

    std::cout << "\n=== 3. Verification that aggregated object is still alive ===\n";
    std::cout << "External fighter HP after suite is gone: " << externalFighter->GetHealth() << "\n";

    delete externalFighter;

    std::cout << "\n=== 4. Dynamic array and array of dynamic objects demo ===\n";
    combat::Fighter* fightersArray[2] = {
        new combat::Fighter(3, 90, 30, 1, "Spark", 10),
        new combat::Fighter(4, 110, 60, 1, "Shield", 10)
    };

    fightersArray[0]->TakeDamage(50);
    fightersArray[1]->CastSpell();

    delete fightersArray[0];
    delete fightersArray[1];

    std::cout << "\n=== 5. Composition demo (destruction order) ===\n";
    {
        combat::Fighter composed(5, 80, 40, 1, "Lightning", 15);
        std::cout << "--- Leaving block (Fighter destruction) ---\n";
    } // Сначала ~Fighter, потом автоматически ~Spell

    std::cout << "=== Program finished successfully ===\n";
    return 0;
}
```
]

