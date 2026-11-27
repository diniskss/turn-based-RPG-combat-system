package combat;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

/**
 * Инвентарь: общее хранилище предметов свиты.
 * Содержит предметы и работает с ними только через ссылку на базовый класс.
 */
public class Inventory {
    /** Правило предметной области: в инвентаре не больше MAX_CAPACITY предметов. */
    public static final int MAX_CAPACITY = 8;

    private final String name;
    private final List<BaseItem> items = new ArrayList<>();

    public Inventory(String name) {
        this.name = name;
        System.out.println("[Inventory] Created inventory '" + name + "' (max " + MAX_CAPACITY + ")");
    }

    /** Проверка правила: инвентарь заполнен, добавить предмет нельзя. */
    public boolean isFull() {
        return items.size() >= MAX_CAPACITY;
    }

    /** Правило: в инвентаре не больше MAX_CAPACITY предметов. */
    public boolean add(BaseItem item) {
        if (isFull()) {
            System.out.println("[Inventory Error] '" + name + "' is full (max " + MAX_CAPACITY + "), item rejected");
            return false;
        }
        if (item == null) {
            System.out.println("[Inventory Error] cannot add null to '" + name + "'");
            return false;
        }

        items.add(item);
        System.out.println("[Inventory] '" + item.getName() + "' added to '" + name + "'");
        return true;
    }

    /** Кладёт глубокую копию предмета: копию делает сам предмет (copy()). */
    public boolean addClone(BaseItem item) {
        if (isFull()) {
            System.out.println("[Inventory Error] '" + name + "' is full (max " + MAX_CAPACITY
                    + "), clone of '" + item.getName() + "' rejected");
            return false;
        }

        items.add(item.copy());
        System.out.println("[Inventory] clone of '" + item.getName() + "' added to '" + name + "'");
        return true;
    }

    public int getSize() {
        return items.size();
    }

    /** Полиморфизм: describe() выбирается по фактическому типу предмета. */
    public void printContents() {
        System.out.println("[Inventory] '" + name + "' contains " + items.size() + " of " + MAX_CAPACITY + ":");
        for (BaseItem item : items) {
            // Ссылка на базовый класс: describe() вызывается у наследника
            System.out.println("  - " + item.describe());
        }
    }

    /** Описания предметов, в названии которых встречается подстрока query. */
    public List<String> findByName(String query) {
        List<String> found = new ArrayList<>();
        for (BaseItem item : items) {
            // indexOf: позиция подстроки в названии или -1, если её нет.
            if (item.getName().indexOf(query) >= 0) {
                found.add(item.describe());
            }
        }
        return found;
    }

    /** Сортирует предметы по названию. */
    public void sortByName() {
        items.sort(Comparator.comparing(BaseItem::getName));
        System.out.println("[Inventory] '" + name + "' sorted by name");
    }
}
