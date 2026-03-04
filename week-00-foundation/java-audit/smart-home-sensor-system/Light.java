public class Light implements SmartDevice {
    // This is a "Concrete Observer/listener" in the Observer pattern
    @Override
    public void update(boolean motionDetected) {
        if (motionDetected) {
            System.out.println("  -> Light... Motion detected. Turning ON lights.");
        } else {
            System.out.println("  -> Light... Area clear. Turning OFF lights.");
        }
    }
}