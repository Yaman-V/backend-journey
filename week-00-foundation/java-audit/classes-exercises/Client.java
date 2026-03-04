public class Client {
    public static void main(String[] args) {
        System.out.println("customer bought a new computer");
        ComputerStore store = new NewComputerStore();
        Computer customer1 = store.getComputer();
        customer1.display();

        System.out.println("customer bought a old computer");
        store = new OldComputerStore();
        Computer customer2 = store.getComputer();
        customer2.display();
        
    }
}