using WeatherNet.Model;

namespace WeatherConnectorLib
{
    public class WeatherData
    {
        public WeatherData(WeatherCurrent currentWeatherResult)
        {
            if (currentWeatherResult != null)
            {
                City = currentWeatherResult.City;
                Country = currentWeatherResult.Country;
                Temp = currentWeatherResult.Temp;
                TempMax = currentWeatherResult.TempMax;
                TempMin = currentWeatherResult.TempMin;
                Humidity = currentWeatherResult.Humidity;
            }
            else
            {
                City = "Place not found";
            }
        }

        public string City { get; }

        public double Temp { get; }

        public double TempMax { get; }

        public double TempMin { get; }

        public double Humidity { get; }

        public string Country { get; }
        
    }
}
