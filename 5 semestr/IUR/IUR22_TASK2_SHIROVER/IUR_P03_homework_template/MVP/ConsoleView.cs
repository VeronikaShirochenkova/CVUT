using System;
using System.Speech.Synthesis.TtsEngine;

namespace MVP
{
    internal class ConsoleView : IView
    {
        public string City { set; get; }
        public string Country { set; get; }
        public double Temp { set; get; }
        public double TempMax { set; get; }
        public double TempMin { set; get; }
        public double Humidity { set; get; }

        public string Language { set; get; }
        public string Units { set; get; }

        public string GetInput()
        {
            Console.ForegroundColor = ConsoleColor.DarkGray;
            switch (Language)
            {
                case "en":
                    Console.Write("\nWrite the name of the city to get the weather or write one of the commands:\n");
                    Console.Write("-'x' to exit;\n-'l' to choose language;\n-'u' to choose units;\n");
                    Console.Write("Please, write: ");
                    break;
                case "ru":
                    Console.Write("\nНапишите название города, чтобы узнать погоду или напишите одну из команд:\n");
                    Console.Write("-'x' для выхода;\n-'l' для выбора языка;\n-'u' для выбора единиц измерения;\n");
                    Console.Write("Пожалуйста, напишите: ");
                    break;
                case "cz":
                    Console.Write("\nNapište název města, ve kterém chcete zjistit počasí, nebo napište jeden z příkazů:\n");
                    Console.Write("-'x' pro ukončení;\n-'l' pro výběr jazyka;\n-'u' pro výběr jednotek;\n");
                    Console.Write("Prosím, napište: ");
                    break;
            }
            Console.ForegroundColor = ConsoleColor.White;
            
            string command = Console.ReadLine();
            Console.Write("\n");
            return command;
        }


        public void Render()
        {
            string dataPresentation;
            string u = "°C"; ;
            if (Units == "metric") u = "°C";
            if (Units == "imperial") u = "°F";

            if (City == "Place not found")
            {
                switch (Language)
                {
                    case "en":
                        Console.Write("City with this name was not found\n");
                        break;
                    case "cz":
                        Console.Write("Město s tímto názvem nebylo nalezeno\n");
                        break;
                    case "ru":
                        Console.Write("Город с таким названием не найден\n");
                        break;
                }
            }
            else {
                switch (Language)
                {
                    case "en":
                        dataPresentation = string.Format("Current weather for city {0}, {1}{2}", City, Country, Environment.NewLine);
                        dataPresentation += string.Format("  Temperature is {0}{4}, maximum {1}{4}, minimum {2}{4}. {3}", Temp, TempMax, TempMin, Environment.NewLine, u);
                        dataPresentation += string.Format("  Humidity is {0}%. {1}", Humidity, Environment.NewLine);

                        Console.Write(dataPresentation);
                        break;
                    case "cz":
                        dataPresentation = string.Format("Aktuální počasí pro město {0}, {1}{2}", City, Country, Environment.NewLine);
                        dataPresentation += string.Format("  Teplota je {0}{4}, maximální {1}{4}, minimální {2}{4}. {3}", Temp, TempMax, TempMin, Environment.NewLine, u);
                        dataPresentation += string.Format("  Vlhkost je {0}%. {1}", Humidity, Environment.NewLine);

                        Console.Write(dataPresentation);
                        break;
                    case "ru":
                        dataPresentation = string.Format("Текущая погода для города {0}, {1}{2}", City, Country, Environment.NewLine);
                        dataPresentation += string.Format("  Температура {0}{4}, максимальная {1}{4}, минимальная {2}{4}. {3}", Temp, TempMax, TempMin, Environment.NewLine, u);
                        dataPresentation += string.Format("  Влажность {0}%. {1}", Humidity, Environment.NewLine);

                        Console.Write(dataPresentation);
                        break;
                }
            }

        }

    }
}
