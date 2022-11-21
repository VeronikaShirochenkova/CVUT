using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace IUR_P02_SHIROVER
{

    public partial class ManageCities : Window
    {
        MainWindow wnd = (MainWindow)Application.Current.MainWindow;

        private ManageCities()
        {
            InitializeComponent();

            // set WindowStartupLocation relative to main window
            this.Left = this.wnd.Left + this.wnd.Width + 50;
            this.Top = this.wnd.Top;
        }

        private void Border_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                this.DragMove();
            }
        }

        public ManageCities(MainWindow mainWindow):this()
        {
            foreach (ComboBoxItem item in mainWindow.Cities_comboBox.Items)
            {
                ListBoxItem listBoxItem = new ListBoxItem();
                listBoxItem.Content = item.Content;
                Cities_listBox.Items.Add(listBoxItem);
            }
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            if (!CityName_txt.Text.Equals(""))
            {
                bool found = false;
                foreach (ListBoxItem item in Cities_listBox.Items)
                {
                    if (item.Content.Equals(CityName_txt.Text))
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    ListBoxItem listBoxItem = new ListBoxItem();
                    listBoxItem.Content = CityName_txt.Text;
                    Cities_listBox.Items.Add(listBoxItem);
                    ComboBoxItem comboBoxItem = new ComboBoxItem();
                    comboBoxItem.Content = CityName_txt.Text;
                    wnd.Cities_comboBox.Items.Add(comboBoxItem);
                }      

            }
            
        }

        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if(Cities_listBox.SelectedItems.Count > 0)
            {
                string deleteCity = ((ListBoxItem)Cities_listBox.SelectedItem).Content.ToString();

                Cities_listBox.Items.Remove(Cities_listBox.SelectedItem);

                foreach (ComboBoxItem item in wnd.Cities_comboBox.Items)
                {
                    if (item.Content.Equals(deleteCity))
                    {
                        wnd.Cities_comboBox.Items.Remove(item);
                        break;
                    }
                }
            }  

        }

        private void OK_button_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }

    }
}
