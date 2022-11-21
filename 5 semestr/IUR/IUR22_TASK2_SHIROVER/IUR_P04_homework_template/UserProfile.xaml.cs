using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media.Imaging;

namespace IUR_P04_homework_template
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public string UserName { get; set; }
        public string UserSurname { get; set; }

        public string Zajem { get; set; }

        public string Reklama { get; set; }


        public ObservableCollection<string> InterestsList { get; set; } = new ObservableCollection<string> { "Zajem1", "Zajem2", "Zajem3" };
        public List<string> Colors { get; set; } = new List<string>() { "White", "Blue", "Red", "Yellow"};
        public List<string> Genders { get; set; } = new List<string>() { "men", "women" };

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
            UserName = "Pavel";
            UserSurname = "Puntik"; 
            
        }

        private void Button_Click_Add(object sender, RoutedEventArgs e)
        {
            if (Zajem != null && Zajem != "")
            {
                InterestsList.Add(Zajem);
            }
                

        }

        private void Button_Click_Delete(object sender, RoutedEventArgs e)
        {

            if (Zajmy.SelectedIndex != -1)
            {
                InterestsList.Remove(Zajem);
            }
                
            
                
        }
    }

    public class ImageConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value != null)
            {
                string imagename = value.ToString();
                string dir = System.IO.Directory.GetCurrentDirectory();
                //Trace.WriteLine(dir + String.Format("/Images/{0}.png", imagename));
                return new BitmapImage(new Uri(dir + String.Format("/Images/{0}.png", imagename)));
            }
            return "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class ReklamaConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool ch = false;
            if (value != null)
            {
               ch = (bool)value;
            }
            return !(bool)value;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

}
