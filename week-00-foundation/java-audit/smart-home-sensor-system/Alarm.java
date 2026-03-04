public class Alarm implements SmartDevice {
    // This is a "Concrete Observer/listener" in the Observer pattern
    @Override
    public void update(boolean motionDetected) {
        if (motionDetected) {
            System.out.println("  -> Alarm.... ALERT! Motion detected! Sounding siren!");
        } else {
            System.out.println("  -> Alarm.... Motion clear. Siren stopped.");
        }
    }
}