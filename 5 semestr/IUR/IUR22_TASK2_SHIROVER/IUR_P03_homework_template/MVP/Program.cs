// See https://aka.ms/new-console-template for more information
using MVP;

Console.Write(" __          ________       _______ _    _ ______ _____                 _____  _____  \n");
Console.Write(" \\ \\        / /  ____|   /\\|__   __| |  | |  ____|  __ \\          /\\   |  __ \\|  __ \\ \n");
Console.Write("  \\ \\  /\\  / /| |__     /  \\  | |  | |__| | |__  | |__) |        /  \\  | |__) | |__) |\n");
Console.Write("   \\ \\/  \\/ / |  __|   / /\\ \\ | |  |  __  |  __| |  _  /        / /\\ \\ |  ___/|  ___/\n");
Console.Write("    \\  /\\  /  | |____ / ____ \\| |  | |  | | |____| | \\ \\       / ____ \\| |    | |\n");
Console.Write("     \\/  \\/   |______/_/    \\_\\_|  |_|  |_|______|_|  \\_\\     /_/    \\_\\_|    |_|\n");
Console.Write("\n");


Model model = new Model();
IView view = new ConsoleView();
//IView view = new SpeechView();
Presenter presenter = new Presenter(model, view);
presenter.ProcessInput();
