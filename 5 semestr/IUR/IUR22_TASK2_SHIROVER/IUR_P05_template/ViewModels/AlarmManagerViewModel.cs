using IUR_P05_solved.Support;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace IUR_P05_solved.ViewModels
{

    class AlarmManagerViewModel : ViewModelBase
    {
        public ObservableCollection<AlarmItemViewModel> Alarms { get; set; } = new ObservableCollection<AlarmItemViewModel>();
        private AlarmItemViewModel _selectedAlarmDetail;

        // ADD
        private bool _addAllowed = true;
        public bool AddAllowed
        { 
            get { return _addAllowed; } 
            set 
            { 
                _addAllowed = value;
                OnPropertyChanged(nameof(AddAllowed));
            } 
        }

        // DELETE
        private bool _deleteAllowed = true;
        public bool DeleteAllowed
        {
            get { return _deleteAllowed; }
            set
            {
                _deleteAllowed = value;
                OnPropertyChanged(nameof(DeleteAllowed));
            }
        }

        // SELECT
        public AlarmItemViewModel SelectedAlarmDetail 
        { 
            get { return _selectedAlarmDetail; } 
            set 
            { 
                _selectedAlarmDetail = value;
                OnPropertyChanged(nameof(SelectedAlarmDetail));
            }

        }
        private int _selectedAlarmIndex;
        public int SelectedAlarmIndex
        {
            get { return _selectedAlarmIndex; }
            set
            {
                _selectedAlarmIndex = value;
                if (_selectedAlarmIndex == -1)
                {
                    DeleteAllowed = false;
                }
                else
                {
                    DeleteAllowed = true;
                }
                OnPropertyChanged(nameof(SelectedAlarmIndex));
            }

        }


        public AlarmManagerViewModel()
        {      

            //Alarms.Add(new AlarmItemViewModel());
            for (int i = 0; i < 5; i++)
            {
                AddAlarm(new object());
            }
        }

        // ADD
        public void AddAlarm(object obj)
        {
            Alarms.Add(new AlarmItemViewModel());
            if (Alarms.Count >= 10)
            {
                AddAllowed = false;
            }
        }
        public bool AddAllowed_(object obj)
        {
            return _addAllowed;
        }


        // DELETE
        public void DeleteAlarm(object obj)
        {
            Alarms.RemoveAt(_selectedAlarmIndex);
        }
        public bool DeleteAllowed_(object obj)
        {
            return _deleteAllowed;
        }


        // Add command for button
        private ICommand _addCommand;     
        public ICommand AddCommand
        {

            get
            {
                return _addCommand ?? (_addCommand = new RelayCommand(AddAlarm, AddAllowed_));
            }
        }

        // Delete command for button
        private ICommand _deleteCommand;
        public ICommand DeleteCommand
        {

            get
            {
                return _deleteCommand ?? (_deleteCommand = new RelayCommand(DeleteAlarm, DeleteAllowed_));
            }
        }
    }
}
