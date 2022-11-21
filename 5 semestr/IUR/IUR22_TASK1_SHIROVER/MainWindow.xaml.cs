using System;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using WeatherNet.Model;

namespace IUR_P02_SHIROVER
{

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            WeatherNet.Util.Api.ApiClient.ProvideApiKey("4bf39896f5f022c9a016b0935688e610");

            // Set header name for last tab item
            DateTime day = DateTime.Today.AddDays(2);
            Tdat_tabItem.Header = day.ToString("dddd dd/MM/yyyy", new CultureInfo("en-EN"));
       
        }

        // allows to drag the window
        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                this.DragMove();
            }
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            
            if (e.AddedItems.Count > 0)
            {
                string cityName = (e.AddedItems[0] as ComboBoxItem).Content.ToString();
                var data = WeatherNet.Forecast.GetByCityName(cityName, "Czechia", "en", "metric");

                if (data.Success)
                {
                    // Now
                    TempTextBlock.Text = String.Format("{0} °C", data.Items[0].Temp.ToString());
                    HumidityTextBlock.Text = String.Format("{0}%", data.Items[0].Humidity.ToString());

                    //Tommorow
                    TempTomorrow_TextBlock.Text = String.Format("{0} °C", data.Items[8].Temp.ToString());
                    HumidityTomorrow_TextBlock.Text = String.Format("{0}%", data.Items[8].Humidity.ToString());
                    AtTomorrow_TextBlock.Text = String.Format("{0}", data.Items[8].Date.ToShortTimeString());

                    //The day after tomorrow
                    TempTdat_TextBlock.Text = String.Format("{0} °C", data.Items[16].Temp.ToString());
                    HumidityTdat_TextBlock.Text = String.Format("{0}%", data.Items[16].Humidity.ToString());
                    AtTdat_TextBlock.Text = String.Format("{0}", data.Items[16].Date.ToShortTimeString());
                }
                else
                {
                    // Now
                    TempTextBlock.Text = "-";
                    HumidityTextBlock.Text = "-";

                    //Tommorow
                    TempTomorrow_TextBlock.Text = "-";
                    HumidityTomorrow_TextBlock.Text = "-";
                    AtTomorrow_TextBlock.Text = "-";

                    //The day after tomorrow
                    TempTdat_TextBlock.Text = "-";
                    HumidityTdat_TextBlock.Text = "-";
                    AtTdat_TextBlock.Text = "-";
                }
                

            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            var manageCiteis = new ManageCities(this);

            if (manageCiteis.ShowDialog() == true) { }

        }

        // Minimize application window
        private void Minimize_app(object sender, MouseButtonEventArgs e)
        {
            this.WindowState = WindowState.Minimized;
        }

        // Close application
        private void Close_app(object sender, MouseButtonEventArgs e)
        {
            Close();
        }
    }
}
