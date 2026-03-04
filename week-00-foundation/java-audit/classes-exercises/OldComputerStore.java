public class OldComputerStore implements ComputerStore{
    @Override
    public Computer getComputer() {
        return new OldComputer();
    }
}
