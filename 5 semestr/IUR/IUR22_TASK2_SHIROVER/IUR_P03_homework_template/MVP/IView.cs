namespace MVP
{
    public interface IView
    {
        string City { set; }
        string Country { set; }
        double Temp { set; }
        double TempMax { set; }
        double TempMin { set; }
        double Humidity { set; }

        string Language { set; }    
        string Units { set; }


        void Render();
        string GetInput();
    }
}
