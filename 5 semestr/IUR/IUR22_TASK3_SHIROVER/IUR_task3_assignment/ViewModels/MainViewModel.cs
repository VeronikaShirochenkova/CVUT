using IUR_P07_solved.Support;
using IUR_P11_solved.ValidationRules;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Input;
using WeatherInfoCustomControl.Support;

namespace IUR_P07_solved.ViewModels
{
    public class MainViewModel : ViewModelBase
    {
        public ObservableCollection<WeatherCardViewModel> WeatherCards { get; set; } = new ObservableCollection<WeatherCardViewModel>();
        private RelayCommand _addCityCommand;
        private string _cityToBeAdded = "";

        private WeatherCardViewModel _selectedWeatherCard { get; set; }
        public WeatherCardViewModel SelectedWeatherCard
        {
            get { return _selectedWeatherCard; }
            set
            {
                _selectedWeatherCard = value;
                OnPropertyChanged(nameof(SelectedWeatherCard));
            }

        }

        public RelayCommand AddCityCommand
        {
            get { return _addCityCommand ?? (_addCityCommand = new RelayCommand(AddCity, AddCityCanExecute)); }
        }

        private void AddCity(object obj)
        {
            if (CityToBeAdded.Length > 1)
            {
                WeatherCards.Add(new WeatherCardViewModel(this, CityToBeAdded));
            }
            
            CityToBeAdded = "";
        }

        private bool AddCityCanExecute(object obj)
        {
            return true;
        }

        public string CityToBeAdded
        {
            get => _cityToBeAdded;

            set
            {
                SetProperty(ref _cityToBeAdded, value);
            }
        }

        public MainViewModel()
        {
            WeatherNet.Util.Api.ApiClient.ProvideApiKey("92a84c52681392d5398ef15eeb322913");

            WeatherCards.Add(new WeatherCardViewModel(this, "Praha"));
            WeatherCards.Add(new WeatherCardViewModel(this, "Brno"));
            WeatherCards.Add(new WeatherCardViewModel(this, "Ostrava"));
            WeatherCards.Add(new WeatherCardViewModel(this, "Jihlava"));
            WeatherCards.Add(new WeatherCardViewModel(this, "Rakovník"));

        }
    }
}
