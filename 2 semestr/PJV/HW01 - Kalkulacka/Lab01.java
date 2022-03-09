package cz.cvut.fel.pjv;

import java.util.Scanner;

public class Lab01 {

    public void start(String[] args) {
    }

    public static void homework(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
        int operace = sc.nextInt();

        switch (operace) {
            case (1):
                System.out.println("Zadej scitanec: ");
                Double scitanec1 = sc.nextDouble();

                System.out.println("Zadej scitanec: ");
                Double scitanec2 = sc.nextDouble();

                System.out.println("Zadej pocet desetinnych mist: ");
                int pocet1 = sc.nextInt();

                if (pocet1 >= 0) {
                    Double value = scitanec1 + scitanec2;

                    String num1 = String.format("%." + pocet1 + "f", scitanec1);
                    String num2 = String.format("%." + pocet1 + "f", scitanec2);
                    String result = String.format("%." + pocet1 + "f", value);

                    System.out.println(num1 + " + " + num2 + " = " + result);
                } else {
                    System.out.println("Chyba - musi byt zadane kladne cislo!");
                }
                break;
            case (2):
                System.out.println("Zadej mensenec: ");
                Double mensenec = sc.nextDouble();

                System.out.println("Zadej mensitel: ");
                Double mensitel = sc.nextDouble();

                System.out.println("Zadej pocet desetinnych mist: ");
                int pocet2 = sc.nextInt();

                if (pocet2 >= 0) {
                    Double value = mensenec - mensitel;

                    String num1 = String.format("%." + pocet2 + "f", mensenec);
                    String num2 = String.format("%." + pocet2 + "f", mensitel);
                    String result = String.format("%." + pocet2 + "f", value);

                    System.out.println(num1 + " - " + num2 + " = " + result);
                } else {
                    System.out.println("Chyba - musi byt zadane kladne cislo!");
                }
                break;
            case (3):
                System.out.println("Zadej cinitel: ");
                Double cinite1 = sc.nextDouble();

                System.out.println("Zadej cinitel: ");
                Double cinite2 = sc.nextDouble();

                System.out.println("Zadej pocet desetinnych mist: ");
                int pocet3 = sc.nextInt();

                if (pocet3 >= 0) {
                    Double value = cinite1 * cinite2;

                    String num1 = String.format("%." + pocet3 + "f", cinite1);
                    String num2 = String.format("%." + pocet3 + "f", cinite2);
                    String result = String.format("%." + pocet3 + "f", value);

                    System.out.println(num1 + " * " + num2 + " = " + result);
                } else {
                    System.out.println("Chyba - musi byt zadane kladne cislo!");
                }
                break;
            case (4):
                System.out.println("Zadej delenec: ");
                Double delenec = sc.nextDouble();

                System.out.println("Zadej delitel: ");
                Double delitel = sc.nextDouble();

                if (delitel == 0) {
                    System.out.println("Pokus o deleni nulou!");
                } else {
                    System.out.println("Zadej pocet desetinnych mist: ");
                    int pocet4 = sc.nextInt();

                    if (pocet4 >= 0) {
                        double value = delenec / delitel;

                        String num1 = String.format("%." + pocet4 + "f", delenec);
                        String num2 = String.format("%." + pocet4 + "f", delitel);
                        String result = String.format("%." + pocet4 + "f", value);

                        System.out.println(num1 + " / " + num2 + " = " + result);
                    } else {
                        System.out.println("Chyba - musi byt zadane kladne cislo!");
                    }
                }
                break;
            default:
                System.out.println("Chybna volba!");
                break;
        }
        sc.close();
    }
}
