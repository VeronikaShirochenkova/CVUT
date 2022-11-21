using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVC
{
    public class Controller
    {
        private Model _model;

        public Controller(Model model)
        {
            _model = model;
        }

        public void ParseCommand(string command)
        {
            if (command == "x") Environment.Exit(0);

            _model.SetCity(command);
        }
    }
}
