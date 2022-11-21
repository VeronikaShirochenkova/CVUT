using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WeatherConnectorLib;

namespace MVC
{
    public class View
    {
        private string dataPresentation;

        public View(Model model)
        {
            model.ModelUpdated += Model_ModelUpdated;
        }

        private void Model_ModelUpdated(WeatherData data)
        {
            dataPresentation = string.Format("Current weather for city {0}, {1}{2}", data.City, data.Country, Environment.NewLine);
            dataPresentation += string.Format("  Temperature is {0}°C, maximum {1}°C, minimum {2}°C. {3}", data.Temp, data.TempMax, data.TempMin, Environment.NewLine);
            dataPresentation += string.Format("  Humidity is {0}%, {1}", data.Humidity , Environment.NewLine);

            Console.Write(dataPresentation);
        }
    }
}
