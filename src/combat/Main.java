package combat;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        // Лабораторная работа 4 идёт первой: её вывод не зависит от сборщика мусора.
        System.out.println("##### Laboratory work 4: inheritance #####");
        InheritanceDemo.runAll();

        System.out.println("\n##### Laboratory work 3: managed memory #####");
        Demo.composition();
        Demo.aggregationAndRule();
        Demo.referencesAndCopies();
        Demo.autoCloseable();
        Demo.garbageCollector();
    }
}
