public class Main {
    static {
        System.load(Main.class.getProtectionDomain().getCodeSource().getLocation().getPath().substring(1).replaceAll("/", "\\\\") + "CPP_LIB\\Main.so");
    }

    public int var = 7;

public static void main(String[] args) {
        Main t = new Main();
        System.out.println("2 + 7 = " + String.valueOf(t.sum(2, 7)));
        int tv = t.var;
        System.out.println(String.valueOf(tv) + " * 2 * 7 = " + String.valueOf(t.multiply(2, 7)));
        System.out.println("Now t.var = " + String.valueOf(t.var));

        System.out.println(t.stringFunc("This is source string"));
        }

native int sum(int a, int b);

native int multiply(int a, int b);

native String stringFunc(String data);

}
