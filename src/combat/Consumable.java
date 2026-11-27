package combat;

/**
 * Расходник: предмет одноразового применения, восстанавливает здоровье бойца.
 * final: от расходника не наследуются, дальнейшая специализация не предусмотрена.
 */
public final class Consumable extends BaseItem {
    private final int healAmount;

    /** Конструктор по умолчанию: малое зелье здоровья. */
    public Consumable() {
        this("Small Health Potion", 20, new Effect("None", 0));   // this(...) — другой конструктор этого же класса
    }

    public Consumable(String name, int healAmount, Effect effect) {
        super(name, effect);                    // конструктор базового класса — первой строкой
        // Правило: зелье восстанавливает здоровье, а не отнимает.
        this.healAmount = Math.max(0, healAmount);
        System.out.println("[Consumable] Consumable('" + name + "'): restores +" + this.healAmount);
    }

    /** Конструктор копирования: глубокая копия. */
    public Consumable(Consumable other) {
        this(other, CopyMode.DEEP);
    }

    /** Копия с выбором режима: DEEP — свой эффект, SHALLOW — общий эффект. */
    public Consumable(Consumable other, CopyMode mode) {
        super(other, mode);
        this.healAmount = other.healAmount;
    }

    @Override
    public void apply(Fighter target) {
        System.out.println("[Consumable] '" + getName() + "' is applied to fighter " + target.getId());
        target.restoreHealth(healAmount);
    }

    /** Дополняет описание базового класса вызовом super.describe(). */
    @Override
    public String describe() {
        return super.describe() + ", consumable: restores +" + healAmount;
    }

    @Override
    public Consumable copy() {
        return new Consumable(this);            // копию нужного типа делает сам предмет
    }

    public int getHealAmount() {
        return healAmount;
    }

    @Override
    public boolean equals(Object o) {
        // super.equals проверяет класс, название и эффект
        if (!super.equals(o)) {
            return false;
        }
        Consumable consumable = (Consumable) o;
        return healAmount == consumable.healAmount;
    }

    @Override
    public int hashCode() {
        return 31 * super.hashCode() + healAmount;
    }
}
