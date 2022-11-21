using System;
//using System.Speech.Synthesis;

namespace MVP
{
    internal class SpeechView : IView
    {
        public string City { set; get; }
        public string Country { set; get; }
        public double Temp { set; get; }
        public double TempMax { set; get; }
        public double TempMin { set; get; }
        public double Humidity { set; get; }

        public string Language { set; get; }
        public string Units { set; get; }    

        public string GetInput()
        {
            throw new NotImplementedException();
        }

        public void Render()
        {
            string dataPresentation = string.Format("Current weather for city {0}, {1}{2}", City, Country, Environment.NewLine);
            dataPresentation += string.Format("  Temperature is {0}°C, maximum {1}°C, minimum {2}°C. {3}", Temp, TempMax, TempMin, Environment.NewLine);
            dataPresentation += string.Format("  Humidity is {0}%, {1}", Humidity, Environment.NewLine);


            System.Speech.Synthesis.SpeechSynthesizer ss = new System.Speech.Synthesis.SpeechSynthesizer();
            ss.Speak(dataPresentation);

        }
    }
}