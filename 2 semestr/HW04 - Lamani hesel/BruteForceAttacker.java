package cz.cvut.fel.pjv;

public class BruteForceAttacker extends Thief {

    @Override
    public void breakPassword(int sizeOfPassword) {
        char[] arr = new char[sizeOfPassword];
        int counter = 0;
        if (sizeOfPassword == 0 || getCharacters()== null)
            tryOpen(arr);


        for (char chr : getCharacters()) {
            if (isOpened())
                break;
            arr[0] = chr; // put one letter from getCharacters() and generate the rest
            try_break(sizeOfPassword, arr, counter + 1);
        }
    }

    private void try_break(int sizeOfPassword, char[] arr, int counter) {
        if (isOpened())
            return;
        if (counter == sizeOfPassword) {
            tryOpen(arr);
            return;
        }
        for (char chr : getCharacters()) {
            arr[counter] = chr;
            try_break(sizeOfPassword, arr, counter + 1);
        }
    }
}