﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace IUR_P05_solved.ViewModels.Converters
{
    public class EnumBooleanConverter : IValueConverter
    {
        #region IValueConverter Members
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string parameterString = parameter.ToString();
            //Trace.WriteLine("par: " + parameter + " parStr: " + parameterString);
            if (parameterString == null)
            {
                //Trace.WriteLine("1if");
                return DependencyProperty.UnsetValue;
            }
                

            if (Enum.IsDefined(value.GetType(), value) == false)
            {
                //Trace.WriteLine("2if");
                return DependencyProperty.UnsetValue;
            }
                

            object parameterValue = Enum.Parse(value.GetType(), parameterString);

            
            return parameterValue.Equals(value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string parameterString = parameter as string;
            if (parameterString == null)
                return DependencyProperty.UnsetValue;

            return Enum.Parse(targetType, parameterString);
        }
        #endregion
    }
}
