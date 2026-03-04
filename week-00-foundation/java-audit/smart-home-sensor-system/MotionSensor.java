public class MotionSensor extends Sensor {
    // This is the "Concrete Subject" in the Observer pattern

    private boolean isMotionDetected;

    public MotionSensor() {
        // Default
        this.isMotionDetected = false;
    }

    public void setMotionState(boolean hasMotion) {
        // Only notify if the state actually changes (optimization)
        System.out.print("MotionSensor: Detected motion = " + hasMotion + ".");
        if (this.isMotionDetected != hasMotion) {
            this.isMotionDetected = hasMotion;
            System.out.println(" State changed.... ");

            notifyDevices(this.isMotionDetected);
        } else {
            System.out.println(" No change in state.");
        }
    }
}