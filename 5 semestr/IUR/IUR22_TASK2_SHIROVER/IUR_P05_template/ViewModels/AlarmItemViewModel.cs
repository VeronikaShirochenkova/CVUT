using IUR_P05_solved.Support;
using IUR_P05_solved.ViewModels.Types;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media.Imaging;

namespace IUR_P05_solved.ViewModels
{
    public class AlarmItemViewModel : ViewModelBase
    {
        public ObservableCollection<BitmapImage> AlarmIcons { get; set; } = new ObservableCollection<BitmapImage>();

        private string _alarmName = string.Empty;
        public string AlarmName 
        {
            get { return _alarmName; }
            set
            {
                _alarmName = value;
                OnPropertyChanged(nameof(AlarmName));
            }
        }

        private AlarmType _alarmEnumType;
        public AlarmType AlarmEnumType 
        {
            get 
            {
                Trace.WriteLine(_alarmEnumType);
                return _alarmEnumType; 
            }
            set
            {
                _alarmEnumType = value;
                Trace.WriteLine(_alarmEnumType);
                OnPropertyChanged(nameof(AlarmEnumType));
            }
        }

        private BitmapImage _selectedImage;
        public BitmapImage SelectedImage 
        {
            get { return _selectedImage; }
            set
            {
                _selectedImage = value;
                OnPropertyChanged(nameof(SelectedImage));
            }
        }

       
        public string AlarmMessage { get; set; }
        public float AlarmSlider { get; set; }





        public AlarmItemViewModel()
        {

            AlarmName = "Alarm1";
            AlarmEnumType = AlarmType.MAX;
            AlarmMessage = "Zprava1";
            AlarmSlider = 10.0f;
            
            AlarmIcons.Add(new BitmapImage(new Uri("pack://application:,,,/Images/cold1.png")));
            AlarmIcons.Add(new BitmapImage(new Uri("pack://application:,,,/Images/cold2.png")));
            AlarmIcons.Add(new BitmapImage(new Uri("pack://application:,,,/Images/cold3.png")));
            AlarmIcons.Add(new BitmapImage(new Uri("pack://application:,,,/Images/hot1.png")));
            AlarmIcons.Add(new BitmapImage(new Uri("pack://application:,,,/Images/hot2.png")));
            AlarmIcons.Add(new BitmapImage(new Uri("pack://application:,,,/Images/hot3.png")));

            //SelectedImage = AlarmIcons[3];

        }
    }
}
