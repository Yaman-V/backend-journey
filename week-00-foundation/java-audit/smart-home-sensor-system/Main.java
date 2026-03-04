public class Main {
    // This is Part 3 : Testing the Observer Pattern Implementation
    public static void main(String[] args) {
        MotionSensor livingRoomSensor = new MotionSensor();

        // smart device objects
        SmartDevice mainAlarm = new Alarm();
        SmartDevice ceilingLight = new Light();
        SmartDevice securityCamera = new Camera();

        // Register the devices with the sensor
        System.out.println("--- Setup Phase ---");
        livingRoomSensor.attach(mainAlarm);
        livingRoomSensor.attach(ceilingLight);
        livingRoomSensor.attach(securityCamera);

        // Test casees

        // case A: Motion Detected (True)
        System.out.println("\n--- case A: Someone enters the room ---");
        livingRoomSensor.setMotionState(true);

        // case B: Motion Stopped (False)
        System.out.println("\n--- case B: Room is clear ---");
        livingRoomSensor.setMotionState(false);

        // removing an observer
        System.out.println("\n---/removing/Disabling the Alarm ---");
        livingRoomSensor.detach(mainAlarm);

        // Case C: Motion Detected again (True) without Alarm
        System.out.println("\n--- case C: Someone enters the room (Alarm disabled!) ---");
        livingRoomSensor.setMotionState(true);
    }
}