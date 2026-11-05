package combat;

import java.util.ArrayList;
import java.util.List;

public class Suite {
    private final String name;
    private final List<Fighter> fighters = new ArrayList<>();
    private int maxFighters;

    public Suite(String name, int maxFighters){
        this.name = name;
        this.maxFighters = maxFighters;
        System.out.println("[Suite] Created suite " + name);
    }

    public String getName(){return name;}

    public void addFighter(Fighter fighter){
        if (fighter == null)                { System.out.println("[Suite Error] fighter is null"); return; }
        if (fighters.size() >= maxFighters) { System.out.println("[Suite Error] '" + name + "' is full (max " + maxFighters + ")"); return; }
        fighters.add(fighter);              System.out.println("[Suite] Fighter added to suite");
    }

    public void inspect(){
        System.out.println("[Suite] Inspecting '" + name + "'. Total fighters: " + fighters.size());
        for (Fighter f : fighters) {
            System.out.println(" - " + f);
        }
    }
}