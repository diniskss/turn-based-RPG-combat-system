package combat;

/** Оружие: повышает атаку бойца. */
public class Weapon extends BaseItem {
    private final int attackBonus;

    /** Конструктор по умолчанию: тренировочное оружие новичка. */
    public Weapon() {
        this("Wooden Sword", 1, new Effect("None", 0));   // this(...) — другой конструктор этого же класса
    }

    public Weapon(String name, int attackBonus, Effect effect) {
        super(name, effect);                    // конструктор базового класса — первой строкой
        // Правило: оружие повышает атаку, а не снижает.
        this.attackBonus = Math.max(0, attackBonus);
        System.out.println("[Weapon] Weapon('" + name + "'): attack +" + this.attackBonus);
    }

    /** Конструктор копирования: глубокая копия. */
    public Weapon(Weapon other) {
        this(other, CopyMode.DEEP);
    }

    /** Копия с выбором режима: DEEP — свой эффект, SHALLOW — общий эффект. */
    public Weapon(Weapon other, CopyMode mode) {
        super(other, mode);
        this.attackBonus = other.attackBonus;
    }

    @Override
    public void apply(Fighter target) {
        System.out.println("[Weapon] '" + getName() + "' is applied to fighter " + target.getId());
        target.increaseAttack(attackBonus);
    }

    /** Дополняет описание базового класса вызовом super.describe(). */
    @Override
    public String describe() {
        return super.describe() + ", weapon: attack +" + attackBonus;
    }

    @Override
    public Weapon copy() {
        return new Weapon(this);                // копию нужного типа делает сам предмет
    }

    public int getAttackBonus() {
        return attackBonus;
    }

    @Override
    public boolean equals(Object o) {
        // super.equals проверяет класс, название и эффект
        if (!super.equals(o)) {
            return false;
        }
        Weapon weapon = (Weapon) o;
        return attackBonus == weapon.attackBonus;
    }

    @Override
    public int hashCode() {
        return 31 * super.hashCode() + attackBonus;
    }
}
