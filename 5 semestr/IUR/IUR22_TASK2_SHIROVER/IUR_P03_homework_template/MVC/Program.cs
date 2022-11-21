// See https://aka.ms/new-console-template for more information
using MVC;

Model model = new Model();
View view = new View(model);
Controller controller = new Controller(model);
while (true)
{
    Console.Write("Command for controller (x to exit): ");
    string command = Console.ReadLine();
    controller.ParseCommand(command);
}

