package combat;

import java.util.Objects;

/**
 * Эффект (бафф/дебафф), который накладывает предмет.
 * В BaseItem хранится как ссылка: это вложенный объект, на котором видна разница
 * между поверхностной и глубокой копией предмета.
 */
public class Effect {
    private final String name;
    private int duration;

    public Effect(String name, int duration) {
        this.name = name;
        this.duration = Math.max(0, duration);   // правило: длительность эффекта не бывает отрицательной
    }

    /** Конструктор копирования: создаёт независимый экземпляр эффекта. */
    public Effect(Effect other) {
        this(other.name, other.duration);        // this(...) — другой конструктор этого же класса
    }

    public String getName() {
        return name;
    }

    public int getDuration() {
        return duration;
    }

    // Правило: длительность эффекта не бывает отрицательной
    public void setDuration(int duration) {
        this.duration = Math.max(0, duration);
    }

    @Override
    public String toString() {
        return name + " (" + duration + " rounds)";
    }

    // Сравнение по содержимому: равные объекты обязаны иметь одинаковый хеш
    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        Effect effect = (Effect) o;
        return duration == effect.duration && name.equals(effect.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, duration);
    }
}
