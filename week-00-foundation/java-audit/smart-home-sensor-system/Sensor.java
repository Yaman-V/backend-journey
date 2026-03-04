import java.util.ArrayList;
import java.util.List;

public abstract class Sensor {
    // This is the "Subject" in the Observer pattern
    private List<SmartDevice> devices = new ArrayList<>();

    // Methods to attach/detach listeners
    public void attach(SmartDevice device) {
        devices.add(device);
        System.out.println("Device attached.");
    }

    public void detach(SmartDevice device) {
        devices.remove(device);
        System.out.println("Device detached.");
    }

    // The method to broadcast updates to all listeners
    protected void notifyDevices(boolean motionDetected) {
        // Check if there are any devices to notify
        if (devices.isEmpty()) {
            System.out.println("No devices to notify.");
            return;
        }

        // Notify all attached devices
        System.out.println("\nBroadcasting update: Motion = " + motionDetected);
        for (SmartDevice device : devices) {
            device.update(motionDetected);
        }
    }
}