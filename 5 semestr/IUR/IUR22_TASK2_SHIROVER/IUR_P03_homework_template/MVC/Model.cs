using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WeatherConnectorLib;

namespace MVC
{
    public class Model
    {
        public event ModelUpdatedDelegate ModelUpdated;
        public delegate void ModelUpdatedDelegate(WeatherData data);

        private string _city;

        public Model()
        {
            WeatherConnector.ApiKey = "92a84c52681392d5398ef15eeb322913";
        }

        public void SetCity(string city)
        {
            _city = city;
            ModelUpdated?.Invoke(WeatherConnector.GetWeatherForCity(_city));
        }

        public WeatherData Weather
        {
            get { return WeatherConnector.GetWeatherForCity(_city); }
        }
    }
}
