package combat;

import java.lang.ref.WeakReference;

public class Demo {
    public static void composition() {
        System.out.println("=== 1. Composition ===");
        Fighter f = new Fighter(1, 100, 50, 1);
        f.learnSpell("Fireball", 20);
        f.learnSpell("Spark", 10);  
        System.out.println(f);
    }

    public static void aggregationAndRule() {
        System.out.println("\n=== 2. Aggregation + rule check ===");
        Fighter external = new Fighter(2, 120, 250, 50);
        Suite suite = new Suite("Alpha Squad", 3);
        suite.addFighter(external);
        suite.inspect();
        external.learnSpell("Meat Hook",110);
        external.learnSpell("Dismember",170);
        external.castSpell(0);   // 250 -> 140
        external.castSpell(1);   // 140 < 170, отказ
        external.castSpell(5);                       // нет такого заклинания
        Suite tiny = new Suite("Tiny", 1);
        tiny.addFighter(external);
        tiny.addFighter(new Fighter(9, 50, 10, 1));  // переполнение
        suite = null;           // свиты "нет", но боец жив
        System.out.println("Fighter after suite is gone: " + external);
    }

    public static void referencesAndCopies() {
        System.out.println("\n=== 3. References vs copies ===");
        Fighter a = new Fighter(3, 90, 30, 10);
        Fighter b = a;                       // копия ССЫЛКИ, а не объекта
        System.out.println("a before: " + a);
        b.takeDamage(50);
        System.out.println("a after: " + a);       // HP тоже изменился
        System.out.println("a == b: " + (a == b));
    }

    public static void autoCloseable() {
        System.out.println("\n=== 4. try-with-resources ===");

        try (BattleLog log = new BattleLog("Round 1")) {
            log.record("fight started");
        }
        System.out.println("After block: log already closed");

        try (BattleLog log = new BattleLog("Round 2")) {
            log.record("about to fail");
            throw new IllegalStateException("boom");
        } 
        catch (IllegalStateException e) {
            System.out.println("Caught: " + e.getMessage());
        }
    }

    private static WeakReference<Fighter> createTemp() {
    Fighter temp = new Fighter(5, 80, 40, 1);
    temp.learnSpell("Lightning", 15);
    return new WeakReference<>(temp);   // сам Fighter уходит из метода
}

    public static void garbageCollector() throws InterruptedException {
        System.out.println("\n=== 5. Garbage collector ===");
        WeakReference<Fighter> weak = createTemp();
        System.out.println("Method returned, alive: " + (weak.get() != null)); // true: GC ещё не пришёл

        for (int i = 0; i < 5 && weak.get() != null; i++) {
            System.gc();
            Thread.sleep(200);
        }
        Thread.sleep(300); // даём Cleaner-потоку допечатать
        System.out.println("After System.gc(), alive: " + (weak.get() != null));
    }
}