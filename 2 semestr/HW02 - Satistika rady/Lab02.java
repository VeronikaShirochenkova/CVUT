/*
 * File name: Lab06.java
 * Date:      2014/08/26 21:39
 * Author:    @author
 */
package cz.cvut.fel.pjv;

public class Lab02 {

    public void start(String[] args) {
    }

    public static void homework(String[] args) {            

            String element;

            TextIO textio = new TextIO();

            double[] myList = new double[10];
            int counter = 0;
            int linecounter = 0;
            double prumer = 0;
            double odchylka = 0;


            
            do {            
                element = textio.getLine();

                if (element != "") {
                    linecounter++;
                    if (TextIO.isDouble(element) || TextIO.isFloat(element)) {
                        myList[counter] = Double.parseDouble(element);
                        counter++;
                    } else {
                        System.out.println("A number has not been parsed from line " + linecounter);
                    }
                } else {
                    System.err.println("End of input detected!");
                    if (counter > 1 ) {
                        for (int i = 0; i < counter; i++) {
                            prumer = prumer + myList[i];
                        }
                        prumer = prumer / counter;
                        for (int i = 0; i < counter; i++) {
                            odchylka = odchylka + (myList[i] - prumer) * (myList[i] - prumer);
                        }
                        odchylka = Math.sqrt(odchylka / counter);
                        String str = String.format("%2d %.3f %.3f", counter, prumer, odchylka);
                        System.out.println(str);
                        counter = 0;
                        prumer = 0;
                        odchylka = 0;
                    }
                    break;
                }
                if (counter == 10) {
                    for (int i = 0; i < 10; i++) {
                        prumer = prumer + myList[i];
                    }
                    prumer = prumer / counter;
                    for (int i = 0; i < 10; i++) {
                        odchylka = odchylka + (myList[i] - prumer) * (myList[i] - prumer);
                    }
                    odchylka = Math.sqrt(odchylka / counter);
                    String str = String.format("%2d %.3f %.3f", counter, prumer, odchylka);
                    System.out.println(str);
                    counter = 0;
                    prumer = 0;
                    odchylka = 0;
                }
            }
            while (element != "");      
    }
}
