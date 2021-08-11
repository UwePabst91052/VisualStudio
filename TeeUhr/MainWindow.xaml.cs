using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Timers;
using System.Media;

namespace TeeUhr
{
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Timer timer = new Timer(1000);
        private int _sec = 0;
        private int _min = 0;
        private TimeSpan time = new TimeSpan(0);
        private SoundPlayer player = new SoundPlayer(@"..\salamisound-4708069-wecker-klingeln-tonlage-ganz.wav");
        private int soundLoops = 1;

        public MainWindow()
        {
            InitializeComponent();
            timer.Elapsed += Timer_Elapsed;
            displayTime.Text = String.Format("{0,2:D2}:{1,2:D2}", _min, _sec);
            player.Load();
        }

        private void Timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            int seconds = (_min * 60) + _sec;
            if (seconds > 0)
            {
                seconds--;
                _min = seconds / 60;
                _sec = seconds % 60;
                Dispatcher.Invoke(() => { FormatTime(); }); ;
                timer.Start();
            }
            else
            {
                timer.Stop();
                player.PlaySync();
            }
        }

        private void onClickSeven(object sender, RoutedEventArgs e)
        {
            _min = 7;
            _sec = 0;
            FormatTime();
            timer.Start();
        }

        private void onClickThree(object sender, RoutedEventArgs e)
        {
            _min = 3;
            _sec = 0;
            FormatTime();
            timer.Start();
        }

        private void onClickFive(object sender, RoutedEventArgs e)
        {
            _min = 5;
            _sec = 0;
            FormatTime();
            timer.Start();
        }

        private void onClickSixThirty(object sender, RoutedEventArgs e)
        {
            _min = 6;
            _sec = 30;
            FormatTime();
            timer.Start();
        }

        private void onClickStart(object sender, RoutedEventArgs e)
        {
            timer.Start();
        }

        private void onScrollSeconds(object sender, System.Windows.Controls.Primitives.ScrollEventArgs e)
        {
            _sec = (int)e.NewValue;
            FormatTime();
        }

        private void onScrollMinutes(object sender, System.Windows.Controls.Primitives.ScrollEventArgs e)
        {
            _min = (int)e.NewValue;
            FormatTime();
        }

        private void FormatTime()
        {
            displayTime.Text = String.Format("{0,2:D2}:{1,2:D2}", _min, _sec);
            scrollMinutes.Track.Value = _min;
            scrollSeconds.Track.Value = _sec;
        }
    }
}
