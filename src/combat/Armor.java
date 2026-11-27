package combat;

/** Броня: повышает защиту бойца. */
public class Armor extends BaseItem {
    private final int defenseBonus;

    /** Конструктор по умолчанию: тканевая рубаха новичка. */
    public Armor() {
        this("Cloth Shirt", 1, new Effect("None", 0));    // this(...) — другой конструктор этого же класса
    }

    public Armor(String name, int defenseBonus, Effect effect) {
        super(name, effect);                    // конструктор базового класса — первой строкой
        // Правило: броня повышает защиту, а не снижает.
        this.defenseBonus = Math.max(0, defenseBonus);
        System.out.println("[Armor] Armor('" + name + "'): defense +" + this.defenseBonus);
    }

    /** Конструктор копирования: глубокая копия. */
    public Armor(Armor other) {
        this(other, CopyMode.DEEP);
    }

    /** Копия с выбором режима: DEEP — свой эффект, SHALLOW — общий эффект. */
    public Armor(Armor other, CopyMode mode) {
        super(other, mode);
        this.defenseBonus = other.defenseBonus;
    }

    @Override
    public void apply(Fighter target) {
        System.out.println("[Armor] '" + getName() + "' is applied to fighter " + target.getId());
        target.increaseDefense(defenseBonus);
    }

    /** Полностью заменяет описание базового класса — без вызова super.describe(). */
    @Override
    public String describe() {
        // Общую часть наследник берёт из protected-метода header().
        return "Armor " + header() + ": defense +" + defenseBonus + ", effect '" + getEffect().getName() + "'";
    }

    @Override
    public Armor copy() {
        return new Armor(this);                 // копию нужного типа делает сам предмет
    }

    public int getDefenseBonus() {
        return defenseBonus;
    }

    @Override
    public boolean equals(Object o) {
        // super.equals проверяет класс, название и эффект
        if (!super.equals(o)) {
            return false;
        }
        Armor armor = (Armor) o;
        return defenseBonus == armor.defenseBonus;
    }

    @Override
    public int hashCode() {
        return 31 * super.hashCode() + defenseBonus;
    }
}
