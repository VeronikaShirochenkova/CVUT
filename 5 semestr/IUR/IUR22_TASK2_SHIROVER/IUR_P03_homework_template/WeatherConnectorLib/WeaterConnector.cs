using System;

namespace WeatherConnectorLib
{
    public static class WeatherConnector
    {
        static WeatherConnector()
        {
        }

        public static string ApiKey { set { WeatherNet.Util.Api.ApiClient.ProvideApiKey(value); } }

        public static WeatherData GetWeatherForCity(string cityName, string language, string units)
        {
            var data = WeatherNet.Current.GetByCityName(cityName, "Czechia", language, units);
            //var data = WeatherNet.Current.GetByCityName(cityName, "Czechia", language, "imperial");
            return new WeatherData(data.Item);
        }
    }
}
