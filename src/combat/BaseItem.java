package combat;

import java.util.Objects;

/**
 * Абстрактный базовый класс иерархии предметов.
 * Создать объект BaseItem нельзя: в нём есть абстрактные методы.
 * Существуют только наследники: {@link Weapon}, {@link Armor}, {@link Consumable}.
 */
public abstract class BaseItem {
    /** Способ копирования вложенного эффекта. */
    public enum CopyMode {
        /** Свой экземпляр эффекта. */
        DEEP,
        /** Общий с оригиналом экземпляр эффекта. */
        SHALLOW
    }

    /** Статическое поле: одно на весь класс, а не по одному у каждого предмета. */
    private static int createdCount = 0;

    private final int itemNumber;
    private final String name;

    // Вложенный объект: эффект, который накладывает предмет.
    private final Effect effect;

    /** Создать предмет может только наследник. */
    protected BaseItem(String name, Effect effect) {
        this.itemNumber = nextItemNumber();
        this.name = name;
        // У предмета всегда есть эффект: если эффект не задан, подставляется нейтральный «None».
        this.effect = (effect != null) ? effect : new Effect("None", 0);
        System.out.println("[BaseItem] BaseItem(): #" + itemNumber + " '" + name + "'");
    }

    /** Глубокая копия: своё имя и свой экземпляр эффекта. */
    protected BaseItem(BaseItem other) {
        this(other, CopyMode.DEEP);              // this(...) — другой конструктор этого же класса
    }

    /**
     * Копия с выбором режима. Protected: копией пользуются наследники и copy(),
     * а остальной код создаёт копию только самим предметом (copy()), тип которой
     * всегда совпадает с типом оригинала.
     */
    protected BaseItem(BaseItem other, CopyMode mode) {
        this(other.name, mode == CopyMode.SHALLOW ? other.effect : new Effect(other.effect));
    }

    /** У каждого наследника своя логика применения (аналог чисто виртуального метода). */
    public abstract void apply(Fighter target);

    /** Копию делает сам объект, знающий свой тип (аналог Clone() в C++). */
    public abstract BaseItem copy();

    /** В базе есть реализация: наследник может дополнить её (super.describe()) или заменить полностью. */
    public String describe() {
        return header() + ", effect: " + effect;
    }

    public int getItemNumber() {
        return itemNumber;
    }

    public String getName() {
        return name;
    }

    public Effect getEffect() {
        return effect;
    }

    /** this: предмет передаёт себя (this) инвентарю, а инвентарь кладёт его копию. */
    public boolean storeCopyIn(Inventory inventory) {
        return inventory.addClone(this);
    }

    /** Статический метод: вызывается без объекта и работает только со статическим полем. */
    public static int getCreatedCount() {
        return createdCount;
    }

    /** Очередной инвентарный номер: нужен только наследникам при создании предмета. */
    protected static int nextItemNumber() {
        return ++createdCount;
    }

    /** Общая часть описания: нужна наследникам для describe(), но не остальному коду. */
    protected String header() {
        return "#" + itemNumber + " '" + name + "'";
    }

    /** Предметы равны, если совпадают класс, название и эффект. Инвентарный номер — идентичность, он не участвует. */
    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        BaseItem baseItem = (BaseItem) o;
        return name.equals(baseItem.name) && effect.equals(baseItem.effect);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, effect);
    }
}
