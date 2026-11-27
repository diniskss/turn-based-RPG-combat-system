package combat;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Демонстрации лабораторной работы 4: иерархия предметов, копирование,
 * this, статические члены и обработка строк.
 */
public final class InheritanceDemo {
    private InheritanceDemo() {
    }

    public static void runAll() {
        constructors();
        Inventory stash = polymorphism();
        shallowAndDeepCopy();
        copyThroughBaseReference(stash);
        equalityByContent();
        strings(stash);
        staticMembers();
    }

    private static void header(String title) {
        System.out.println("\n=== " + title + " ===");
    }

    /** 1. Конструкторы: по умолчанию и с параметрами; конструктор базового класса выполняется первым. */
    private static void constructors() {
        header("1. Constructors: default and with parameters; the base constructor runs first");
        // new Weapon() вызывает this("Wooden Sword", 1, ...), а тот — super(name, effect):
        // сообщение BaseItem печатается раньше сообщения Weapon.
        Weapon trainingWeapon = new Weapon();
        System.out.println("default constructor: " + trainingWeapon.describe());

        Weapon sword = new Weapon("Fire Sword", 15, new Effect("Bleeding", 3));
        System.out.println("parameterized constructor: " + sword.describe());
    }

    /** 2. Полиморфизм: список BaseItem с разными наследниками. */
    private static Inventory polymorphism() {
        header("2. Polymorphism: a list of BaseItem with different heirs");
        Fighter hero = new Fighter(11, 100, 50, 1, 10, 5);
        List<BaseItem> loadout = new ArrayList<>();
        loadout.add(new Weapon("Fire Sword", 15, new Effect("Bleeding", 3)));
        loadout.add(new Armor("Knight Armor", 12, new Effect("Protection", 2)));
        loadout.add(new Consumable("Health Potion", 40, new Effect("Regeneration", 2)));

        hero.takeDamage(60);
        for (BaseItem item : loadout) {
            // Ссылка на базовый класс: describe() вызывается у наследника
            System.out.println("  - " + item.describe());
        }
        for (BaseItem item : loadout) {
            item.apply(hero);
        }
        System.out.println("fighter after the items were applied: " + hero);

        // В Java нет передачи владения: инвентарь и список ссылаются на одни и те же предметы.
        Inventory stash = new Inventory("Stash");
        for (BaseItem item : loadout) {
            stash.add(item);
        }
        return stash;
    }

    /** 3. Поверхностная и глубокая копия. */
    private static void shallowAndDeepCopy() {
        header("3. A shallow copy shares the nested effect, a deep copy owns its own");
        Weapon dagger = new Weapon("Poisoned Dagger", 8, new Effect("Poison", 3));

        Weapon deepCopy = new Weapon(dagger);                                  // эффект копируется
        Weapon shallowCopy = new Weapon(dagger, BaseItem.CopyMode.SHALLOW);    // эффект общий с оригиналом

        // == сравнивает ссылки, а не содержимое
        System.out.println("same effect object as the original: deep copy "
                + (dagger.getEffect() == deepCopy.getEffect())
                + ", shallow copy " + (dagger.getEffect() == shallowCopy.getEffect()));

        deepCopy.getEffect().setDuration(9);     // меняет только эффект своей копии
        System.out.println("after deepCopy.setDuration(9): original duration "
                + dagger.getEffect().getDuration() + " (unchanged)");

        shallowCopy.getEffect().setDuration(1);  // меняет общий эффект
        System.out.println("after shallowCopy.setDuration(1): original duration "
                + dagger.getEffect().getDuration() + " (changed)");
    }

    /** 4. copy(): копию своего типа делает сам объект через ссылку на базовый класс. */
    private static void copyThroughBaseReference(Inventory stash) {
        header("4. copy(): the object makes a copy of its own type through a base reference");
        BaseItem baseReference = new Consumable("Mana Potion", 30, new Effect("Clarity", 1));
        BaseItem twin = baseReference.copy();   // тип копии определяет сам объект

        System.out.println("original: " + baseReference.describe());
        System.out.println("copy:    " + twin.describe());
        System.out.println("the copy has the same class as the original: "
                + (twin.getClass() == baseReference.getClass()));

        // this: предмет передаёт себя (this) инвентарю, инвентарь кладёт его копию.
        boolean stored = baseReference.storeCopyIn(stash);
        System.out.println("copy stored in the inventory: " + stored);
    }

    /** 5. equals и hashCode: сравнение по содержимому. */
    private static void equalityByContent() {
        header("5. equals and hashCode: comparison by content");
        Weapon first = new Weapon("Steel Sword", 12, new Effect("None", 0));
        Weapon sameContent = new Weapon("Steel Sword", 12, new Effect("None", 0));
        Weapon other = new Weapon("Steel Sword", 15, new Effect("None", 0));

        System.out.println("first == sameContent (references): " + (first == sameContent));
        System.out.println("first.equals(sameContent) (content): " + first.equals(sameContent));
        System.out.println("first.equals(other) (content): " + first.equals(other));
        System.out.println("hashCode of equal objects is equal: " + (first.hashCode() == sameContent.hashCode()));

        // Равные объекты с равными хешами находятся в HashSet по содержимому.
        Set<Weapon> armoury = new HashSet<>();
        armoury.add(first);
        System.out.println("armoury.contains(sameContent): " + armoury.contains(sameContent));
        System.out.println("armoury.contains(other): " + armoury.contains(other));
    }

    /** 6. Обработка строк: конкатенация, indexOf, substring, сравнение. */
    private static void strings(Inventory stash) {
        header("6. String: concatenation, indexOf, substring, comparison");
        // Конкатенация: код предмета собирается из частей.
        String itemCode = "weapon:" + "Fire Sword:" + 15 + ":Bleeding:3";
        System.out.println("item code: " + itemCode);

        // indexOf и substring разбирают код, equals выбирает наследника.
        BaseItem parsed = makeItemFromCode(itemCode);
        System.out.println("parsed: " + parsed.describe());
        boolean parsedStored = stash.add(parsed);
        System.out.println("parsed item stored: " + parsedStored);

        BaseItem unknown = makeItemFromCode("scroll:Fire Scroll:10:Burning:2");
        System.out.println("unknown item type gives null: " + (unknown == null));

        // indexOf: поиск предметов по части названия.
        System.out.println("items with 'Potion' in the name:");
        for (String description : stash.findByName("Potion")) {
            System.out.println("  - " + description);
        }

        // Правило: в инвентаре не больше Inventory.MAX_CAPACITY предметов.
        Weapon spare = new Weapon("Spare Sword", 1, new Effect("None", 0));
        while (!stash.isFull()) {
            stash.add(new Consumable("Small Potion", 5, new Effect("None", 0)));
        }
        boolean accepted = stash.add(spare);
        System.out.println("adding to a full inventory accepted: " + accepted
                + " (the rejected item is not destroyed: the reference stays with the caller)");

        // Сравнение (compareTo): сортировка по названию.
        stash.sortByName();
        stash.printContents();
    }

    /** 7. Статические члены: один счётчик созданных предметов на весь класс. */
    private static void staticMembers() {
        header("7. Static members: one created-items counter for the whole class");
        System.out.println("BaseItem.getCreatedCount() is called without any object: " + BaseItem.getCreatedCount());
        Weapon temporary = new Weapon("Broken Sword", 2, new Effect("None", 0));
        System.out.println("after one more item was created: " + BaseItem.getCreatedCount());
        System.out.println("item number of the new weapon: " + temporary.getItemNumber()
                + " (the number was issued by the static counter)");
    }

    /**
     * Обработка строк по теме проекта: разбирает код предмета вида
     * "type:name:power:effect:duration" (например, "weapon:Fire Sword:15:Bleeding:3")
     * и создаёт предмет нужного наследника. Возвращает null, если код неверен.
     */
    private static BaseItem makeItemFromCode(String code) {
        // indexOf: позиции разделителей; -1, если разделителя нет.
        int first = code.indexOf(':');
        int second = code.indexOf(':', first + 1);
        int third = code.indexOf(':', second + 1);
        int fourth = code.indexOf(':', third + 1);
        if (first < 0 || second < 0 || third < 0 || fourth < 0) {
            System.out.println("[Item Code Error] expected type:name:power:effect:duration, got '" + code + "'");
            return null;
        }

        // substring: части строки между разделителями.
        String type = code.substring(0, first);
        String name = code.substring(first + 1, second);
        String powerText = code.substring(second + 1, third);
        String effectName = code.substring(third + 1, fourth);
        String durationText = code.substring(fourth + 1);

        int power;
        int duration;
        try {
            power = Integer.parseInt(powerText);
            duration = Integer.parseInt(durationText);
        } catch (NumberFormatException e) {
            System.out.println("[Item Code Error] power and duration must be numbers, got '" + code + "'");
            return null;
        }

        Effect effect = new Effect(effectName, duration);

        // Сравнение строк: equals сравнивает содержимое, == сравнивал бы ссылки.
        if (type.equals("weapon")) {
            return new Weapon(name, power, effect);
        }
        if (type.equals("armor")) {
            return new Armor(name, power, effect);
        }
        if (type.equals("consumable")) {
            return new Consumable(name, power, effect);
        }

        System.out.println("[Item Code Error] unknown item type '" + type + "'");
        return null;
    }
}
