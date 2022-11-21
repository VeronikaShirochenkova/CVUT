using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WeatherConnectorLib;

namespace MVP
{
    public class Model
    {
        public event ModelUpdatedDelegate ModelUpdated;
        public delegate void ModelUpdatedDelegate(WeatherData data);

        private string _city;
        private string _language;
        private string _units;

        public Model()
        {
            WeatherConnector.ApiKey = "92a84c52681392d5398ef15eeb322913";
            _language = "en";
            _units = "metric";
        }

        public void SetCity(string city)
        {
            _city = city;

            ModelUpdated?.Invoke(WeatherConnector.GetWeatherForCity(_city, _language, _units));
        }

        public string GetLanguage()
        {
            return _language;
        }

        public void SetLanguage(string language)
        {
            _language = language;
        }
        
        public string GetUnits()
        {
            return _units;
        }

        public void SetUnits(string units)
        {
            switch(units)
            {
                case "m":
                    _units = "metric";
                    break;
                case "i":
                    _units = "imperial";
                    break;

            }
            
        }

        public WeatherData Weather
        {
            get { return WeatherConnector.GetWeatherForCity(_city, _language, _units); }
        }
    }
}
