package Mafia;

import java.util.Random;
import java.util.Date;

public class Randomizer {
    private static Random random;

    static {
        random = new Random();
        random.setSeed(new Date().getTime());
    }

    public static int randint(){
        return random.nextInt();
    }

}
