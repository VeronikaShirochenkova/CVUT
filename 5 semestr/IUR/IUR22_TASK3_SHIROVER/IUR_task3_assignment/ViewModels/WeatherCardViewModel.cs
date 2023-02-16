using IUR_P07_solved.Support;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WeatherInfoCustomControl.Support;

namespace IUR_P07_solved.ViewModels
{
    public class WeatherCardViewModel : ViewModelBase
    {
        public WeatherCardViewModel(MainViewModel mainViewModelReference, string location)
        {
            _mainViewModelReference = mainViewModelReference;
            Location = location;

        }

        private RelayCommand _removeCityCommand;
        private MainViewModel _mainViewModelReference;
        private string _location;
        private double _temperature;
        private double _humidity;
        private string _conditions;
        private string _iconPath;

        public double Temperature
        {
            get => _temperature;
            set => SetProperty(ref _temperature, value);
        }

        public double Humidity
        {
            get => _humidity;
            set => SetProperty(ref _humidity, value);
        }

        public string Conditions
        {
            get => _conditions;
            set => SetProperty(ref _conditions, value);
        }

        public string IconPath
        {
            get => _iconPath;
            set => SetProperty(ref _iconPath, value);
        }

        public string Location
        {
            get => _location;
            set
            {
                SetProperty(ref _location, value);
                UpdateWeather();
            }
        }

        public RelayCommand RemoveCityCommand
        {
            get { return _removeCityCommand ?? (_removeCityCommand = new RelayCommand(RemoveCity, RemoveCityCanExecute)); }
        }

        private void RemoveCity(object obj)
        {
            _mainViewModelReference.WeatherCards.Remove(this);
        }

        private bool RemoveCityCanExecute(object obj)
        {
            return true;
        }

        private void UpdateWeather()
        {
            var singleResult = WeatherNet.Current.GetByCityName(Location, "Czechia", "en", "metric");

            if (singleResult.Item != null)
            {
                //Trace.WriteLine("tut pogoda\n");
                Temperature = singleResult.Item.Temp;
                Humidity = singleResult.Item.Humidity;
                Conditions = singleResult.Item.Description;
                IconPath = singleResult.Item.Icon;
                //Location = singleResult.Item.City;
            }
        }
    }
}
