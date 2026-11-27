package combat;

import java.lang.ref.Cleaner;
import java.util.ArrayList;
import java.util.List;

public class Fighter{
    private static final Cleaner CLEANER = Cleaner.create();

    // Параметры бойца по умолчанию, если оружие и броня не заданы.
    private static final int DEFAULT_ATTACK = 10;
    private static final int DEFAULT_DEFENSE = 5;

    private final int id;
    private int health;
    private int maxHealth;
    private int mana;
    private int maxMana;
    private int actionPoints;
    private int attack;
    private int defense;
    private final List<Spell> spells = new ArrayList<>();

    public Fighter(int id, int health, int mana, int actionPoints){
        this(id, health, mana, actionPoints, DEFAULT_ATTACK, DEFAULT_DEFENSE);   // this(...) — другой конструктор этого же класса
    }

    public Fighter(int id, int health, int mana, int actionPoints, int attack, int defense){
        this.id = id;
        this.health = health;
        this.maxHealth = health;
        this.mana = mana;
        this.maxMana = mana;
        this.actionPoints = actionPoints;
        this.attack = attack;
        this.defense = defense;
        System.out.println("[Fighter] Created fighter ID: " + id);
        CLEANER.register(this, new CleanupAction(id));
    }

    private static class CleanupAction implements Runnable {
        private final int id;
        CleanupAction(int id) { this.id = id; }
        @Override
        public void run() {
            System.out.println("[GC] Fighter ID: " + id + " collected by garbage collector");
        }
    }

    public int getId() { return id; }
    public int getHealth() { return health; }
    public int getMana() { return mana; }
    public int getActionPoints() { return actionPoints; }
    public int getSpellCount() { return spells.size(); }
    public int getAttack() { return attack; }
    public int getDefense() { return defense; }
    public void setMaxHealth(int newMax) {
    if (newMax <= 0) {
        System.out.println("[Fighter Error] ID: " + id + " max HP must be positive");
        return;
    }
    maxHealth = newMax;
    health = Math.min(health, maxHealth);   // текущее HP не выше нового максимума
}

    public void takeDamage(int amount) {
        int old = health;
        health = Math.max(0, health - amount);   // правило: HP не ниже 0
        System.out.println("[Fighter] ID: " + id + " took " + amount + " damage. HP: " + old + " -> " + health);
    }

    public void restoreHealth(int amount) {
        int old = health;
        if((health + amount) > maxHealth){
            health = maxHealth;
        }
        else{
            health += amount;
        }
        System.out.println("[Fighter] ID: " + id + " took " + amount + " health. HP: " + old + " -> " + health);
    }

    public void restoreMana(int amount){
        int old = mana;
        if((mana + amount) > maxMana){
            mana = maxMana;
        }
        else{
            mana += amount;
        }
        System.out.println("[Fighter] ID: " + id + " took " + amount + " mana. Mana: " + old + " -> " + mana);
    }

    /** Применение к бойцу оружия и брони. Правила: предмет повышает параметр, а не снижает. */
    public void increaseAttack(int bonus){
        if (bonus <= 0) {
            System.out.println("[Fighter Error] ID: " + id + " attack bonus must be positive (Action rejected)");
            return;
        }
        int old = attack;
        attack += bonus;
        System.out.println("[Fighter] ID: " + id + " attack: " + old + " -> " + attack);
    }

    public void increaseDefense(int bonus){
        if (bonus <= 0) {
            System.out.println("[Fighter Error] ID: " + id + " defense bonus must be positive (Action rejected)");
            return;
        }
        int old = defense;
        defense += bonus;
        System.out.println("[Fighter] ID: " + id + " defense: " + old + " -> " + defense);
    }

    public void learnSpell(String name, int cost) {
        spells.add(new Spell(name, cost));
    }

    public boolean castSpell(int index) {
        if (index < 0 || index >= spells.size()) {
            System.out.println("[Fighter Error] ID: " + id + " has no spell #" + index);
            return false;
        }
        Spell spell = spells.get(index);
        int cost = spell.getManaCost();
        if (mana < cost) {
            System.out.println("[Fighter Error] ID: " + id + " cannot cast '" + spell.getName()
                    + "': required " + cost + ", available " + mana + " (Action rejected)");
            return false;
        }
        mana -= cost;
        spell.use();
        System.out.println("[Fighter] ID: " + id + " successfully cast spell. Remaining mana: " + mana);
        return true;
    }
    @Override
    public String toString() {
        return "Fighter{id=" + id + ", hp=" + health + "/" + maxHealth + ", mana=" + mana + "/" + maxMana + ", spells=" + spells + "}";
    }
}