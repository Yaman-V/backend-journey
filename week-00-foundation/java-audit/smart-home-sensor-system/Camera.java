public class Camera implements SmartDevice {
    // This is a "Concrete Observer/listener" in the Observer pattern
    @Override
    public void update(boolean motionDetected) {
        if (motionDetected) {
            System.out.println("  -> Camera.... Starting recording...");
        } else {
            System.out.println("  -> Camera.... Stopping recording.");
        }
    }
}