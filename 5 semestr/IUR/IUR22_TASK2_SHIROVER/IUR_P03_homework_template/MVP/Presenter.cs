using System;

namespace MVP
{
    internal class Presenter
    {
        private Model _model;
        private IView _view;

        public Presenter(Model model, IView view)
        {
            _model = model;
            _view = view;

            _view.Language = _model.GetLanguage();


            _model.ModelUpdated += _model_ModelUpdated;
        }

        public void ProcessInput()
        {
            while (true)
            {
                string command = _view.GetInput();
                ParseCommand(command);
            }
        }

        private void _model_ModelUpdated(WeatherConnectorLib.WeatherData data)
        {
            _view.City = data.City;
            _view.Country = data.Country;
            _view.Humidity = data.Humidity;
            _view.Temp = data.Temp;
            _view.TempMax = data.TempMax;
            _view.TempMin = data.TempMin;

            _view.Language = _model.GetLanguage();
            _view.Units = _model.GetUnits();

            

            _view.Render();
        }

        internal void ParseCommand(string command)
        {
            if (command == "x")
            {
                
                Console.Write("  ______     ________     __  _            _  __   \n");
                Console.Write(" |  _ \\ \\   / /  ____|   / / | |          | | \\ \\  \n");
                Console.Write(" | |_) \\ \\_/ /| |__     | |  | |          | |  | | \n");
                Console.Write(" |  _ < \\   / |  __|    | |  | |          | |  | | \n");
                Console.Write(" | |_) | | |  | |____   | |  | |          | |  | | \n");
                Console.Write(" |____/  |_|  |______|  | |  |_|          |_|  | | \n");
                Console.Write("                         \\_\\      ______      /_/  \n");
                Console.Write("                                 |______|          \n");
                
                Console.Write("\n");
                Environment.Exit(0);
            }

            if (command == "l")
            {
                Console.ForegroundColor = ConsoleColor.DarkGray;
                string l = _model.GetLanguage();
                string language = l;
                switch (l)
                {
                    case "en":
                        Console.Write("Now you can choose language. You can choose Czech, English, Russian;\nYou need to write 'cz', 'en' or 'ru': ");
                        Console.ForegroundColor = ConsoleColor.White;
                        language = Console.ReadLine();
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        Console.Write("You choose: " + language + "\n");
                        break;
                    case "cz":
                        Console.Write("Nyní si můžete vybrat jazyk. Můžete si vybrat češtinu, angličtinu nebo ruštinu;\nMusíte napsat 'cz', 'en' nebo 'ru': ");
                        Console.ForegroundColor = ConsoleColor.White;
                        language = Console.ReadLine();
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        Console.Write("Vybrali jste: " + language + "\n");
                        break;
                    case "ru":
                        Console.Write("Теперь вы можете выбрать язык. Вы можете выбрать чешский, английский, русский;\nВам нужно написать 'cz', 'en' или 'ru': ");
                        Console.ForegroundColor = ConsoleColor.White;
                        language = Console.ReadLine();
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        Console.Write("Вы выбрали: " + language + "\n");
                        break;
                }
                Console.ForegroundColor = ConsoleColor.White;
                _model.SetLanguage(language);
                _view.Language = language;

            }
            else if (command == "u") 
            {
                Console.ForegroundColor = ConsoleColor.DarkGray;
                string u = _model.GetUnits();
                string l = _model.GetLanguage();
                string units = u;
                switch (l)
                {
                    case "en":
                        Console.Write("Now you can choose units. You can choose metric or imperial;\nYou need to write 'm' or 'i': ");
                        Console.ForegroundColor = ConsoleColor.White;
                        units = Console.ReadLine();
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        if (units == "m")
                        {
                            Console.Write("You choose: metric\n");
                        }
                        else if (units == "i")
                        {
                            Console.Write("You choose: imperial\n");
                        }
                        else
                        {
                            Console.Write("ERROR: Wrong input\n");
                        }
                        break;
                    case "cz":
                        Console.Write("Nyní si můžete vybrat jednotky. Můžete zvolit metrické nebo imperiální jednotky;\nMusíte napsat 'm' nebo 'i': ");
                        Console.ForegroundColor = ConsoleColor.White;
                        units = Console.ReadLine();
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        if (units == "m")
                        {
                            Console.Write("Vybrali jste: metrické\n");
                        }
                        else if (units == "i")
                        {
                            Console.Write("Vybrali jste: imperiální\n");
                        }
                        else
                        {
                            Console.Write("ERROR: Špatný vstup\n");
                        }
                        break;
                    case "ru":
                        Console.Write("Теперь вы можете выбрать единицы измерения. Вы можете выбрать метрические или имперские;\nВы должны написать 'm' или 'i': ");
                        Console.ForegroundColor = ConsoleColor.White;
                        units = Console.ReadLine();
                        Console.ForegroundColor = ConsoleColor.DarkGray;
                        if (units == "m")
                        {
                            Console.Write("Вы выбрали: метрические\n");
                        }
                        else if (units == "i")
                        {
                            Console.Write("Вы выбрали: имперские\n");
                        }
                        else
                        {
                            Console.Write("ERROR: Неправильный ввод\n");
                        }
                        break;
                }
                Console.ForegroundColor = ConsoleColor.White;
                _model.SetUnits(units);
            }
            else
            {
                _model.SetCity(command); 
            }

        }
    }
}
