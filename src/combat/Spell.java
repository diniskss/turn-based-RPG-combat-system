package combat;

public class Spell {
    private final String name;
    private final int manaCost;

    public Spell(String name, int manaCost) {
        this.name = name;
        this.manaCost = manaCost;
        System.out.println("[Spell] Created spell: " + name);
    }

    public String getName() { return name; }
    public int getManaCost() { return manaCost; }

    public void use(){
        System.out.println("[Spell] Spell '" + name + "' casted, cost: " + manaCost + ".");
    }

    @Override
    public String toString() { return name + " (cost " + manaCost + ")"; }  
}