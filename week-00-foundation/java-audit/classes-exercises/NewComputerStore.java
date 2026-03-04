public class NewComputerStore implements ComputerStore{
    @Override
    public Computer getComputer() {
        return new NewComputer();
    }
}
