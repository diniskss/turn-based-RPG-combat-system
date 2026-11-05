package combat;

public class BattleLog implements AutoCloseable {
    private final String title;

    public BattleLog(String title) {
        this.title = title;
        System.out.println("[BattleLog] Opened: " + title);
    }

    public void record(String message) {
        System.out.println("[BattleLog] " + title + ": " + message);
    }

    @Override
    public void close() {
        System.out.println("[BattleLog] Closed: " + title);
    }
}