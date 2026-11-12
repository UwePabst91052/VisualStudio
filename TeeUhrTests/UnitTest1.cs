using System;
using System.Reflection;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using NUnit.Framework;
using TeeUhr;

namespace TeeUhrTests
{
    [TestFixture]
    [Apartment(ApartmentState.STA)]
    public class MainWindowTests
    {
        private MainWindow _window = null!;

        [SetUp]
        public void Setup()
        {
            _window = new MainWindow();
        }

        [TearDown]
        public void TearDown()
        {
            // Stop the internal timer so it doesn't keep firing after test ends.
            GetTimerField().Stop();
        }

        private System.Timers.Timer GetTimerField()
        {
            var field = typeof(MainWindow).GetField("timer", BindingFlags.Instance | BindingFlags.NonPublic);
            return (System.Timers.Timer)field!.GetValue(_window)!;
        }

        private int GetMin()
        {
            var field = typeof(MainWindow).GetField("_min", BindingFlags.Instance | BindingFlags.NonPublic);
            return (int)field!.GetValue(_window)!;
        }

        private int GetSec()
        {
            var field = typeof(MainWindow).GetField("_sec", BindingFlags.Instance | BindingFlags.NonPublic);
            return (int)field!.GetValue(_window)!;
        }

        private void SetMin(int value)
        {
            var field = typeof(MainWindow).GetField("_min", BindingFlags.Instance | BindingFlags.NonPublic);
            field!.SetValue(_window, value);
        }

        private void SetSec(int value)
        {
            var field = typeof(MainWindow).GetField("_sec", BindingFlags.Instance | BindingFlags.NonPublic);
            field!.SetValue(_window, value);
        }

        private void InvokePrivateMethod(string name, params object[] args)
        {
            var method = typeof(MainWindow).GetMethod(name, BindingFlags.Instance | BindingFlags.NonPublic);
            method!.Invoke(_window, args);
        }

        private TextBox GetDisplayTime()
        {
            var field = typeof(MainWindow).GetField("displayTime", BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            return (TextBox)field!.GetValue(_window)!;
        }

        private ScrollBar GetScrollMinutes()
        {
            var field = typeof(MainWindow).GetField("scrollMinutes", BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            return (ScrollBar)field!.GetValue(_window)!;
        }

        private ScrollBar GetScrollSeconds()
        {
            var field = typeof(MainWindow).GetField("scrollSeconds", BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public);
            return (ScrollBar)field!.GetValue(_window)!;
        }

        [Test]
        public void Constructor_InitializesDisplayTimeToZero()
        {
            Assert.That(GetDisplayTime().Text, Is.EqualTo("00:00"));
        }

        [Test]
        public void OnClickSeven_SetsTenMinutes()
        {
            InvokePrivateMethod("onClickSeven", null!, new RoutedEventArgsStub().EventArgs);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(10));
                Assert.That(GetSec(), Is.EqualTo(0));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("10:00"));
                Assert.That(GetTimerField().Enabled, Is.True);
            });
        }

        [Test]
        public void OnClickThree_SetsThreeMinutes()
        {
            InvokePrivateMethod("onClickThree", null!, new RoutedEventArgsStub().EventArgs);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(3));
                Assert.That(GetSec(), Is.EqualTo(0));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("03:00"));
                Assert.That(GetTimerField().Enabled, Is.True);
            });
        }

        [Test]
        public void OnClickFive_SetsFiveMinutes()
        {
            InvokePrivateMethod("onClickFive", null!, new RoutedEventArgsStub().EventArgs);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(5));
                Assert.That(GetSec(), Is.EqualTo(0));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("05:00"));
                Assert.That(GetTimerField().Enabled, Is.True);
            });
        }

        [Test]
        public void OnClickSixThirty_SetsSixMinutesThirtySeconds()
        {
            InvokePrivateMethod("onClickSixThirty", null!, new RoutedEventArgsStub().EventArgs);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(6));
                Assert.That(GetSec(), Is.EqualTo(30));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("06:30"));
                Assert.That(GetTimerField().Enabled, Is.True);
            });
        }

        [Test]
        public void OnClickStart_StartsTimerWithoutChangingTime()
        {
            SetMin(2);
            SetSec(15);

            InvokePrivateMethod("onClickStart", null!, new RoutedEventArgsStub().EventArgs);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(2));
                Assert.That(GetSec(), Is.EqualTo(15));
                Assert.That(GetTimerField().Enabled, Is.True);
            });
        }

        [Test]
        public void OnScrollSeconds_UpdatesSecondsAndDisplay()
        {
            var args = new ScrollEventArgs(ScrollEventType.ThumbPosition, 42);

            InvokePrivateMethod("onScrollSeconds", null!, args);

            Assert.Multiple(() =>
            {
                Assert.That(GetSec(), Is.EqualTo(42));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("00:42"));
                Assert.That(GetScrollSeconds().Value, Is.EqualTo(42));
            });
        }

        [Test]
        public void OnScrollMinutes_UpdatesMinutesAndDisplay()
        {
            var args = new ScrollEventArgs(ScrollEventType.ThumbPosition, 17);

            InvokePrivateMethod("onScrollMinutes", null!, args);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(17));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("17:00"));
                Assert.That(GetScrollMinutes().Value, Is.EqualTo(17));
            });
        }

        [Test]
        public void FormatTime_FormatsDisplayAndUpdatesScrollBars()
        {
            SetMin(4);
            SetSec(8);

            InvokePrivateMethod("FormatTime");

            Assert.Multiple(() =>
            {
                Assert.That(GetDisplayTime().Text, Is.EqualTo("04:08"));
                Assert.That(GetScrollMinutes().Value, Is.EqualTo(4));
                Assert.That(GetScrollSeconds().Value, Is.EqualTo(8));
            });
        }

        [Test]
        public void TimerElapsed_DecrementsSecondWhenSecondsRemain()
        {
            SetMin(0);
            SetSec(5);

            InvokePrivateMethod("Timer_Elapsed", null!, EventArgs.Empty);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(0));
                Assert.That(GetSec(), Is.EqualTo(4));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("00:04"));
                Assert.That(GetTimerField().Enabled, Is.True);
            });
        }

        [Test]
        public void TimerElapsed_RollsOverMinuteWhenSecondsReachZero()
        {
            SetMin(1);
            SetSec(0);

            InvokePrivateMethod("Timer_Elapsed", null!, EventArgs.Empty);

            Assert.Multiple(() =>
            {
                Assert.That(GetMin(), Is.EqualTo(0));
                Assert.That(GetSec(), Is.EqualTo(59));
                Assert.That(GetDisplayTime().Text, Is.EqualTo("00:59"));
            });
        }

        [Test]
        public void TimerElapsed_StopsTimerWhenTimeReachesZero()
        {
            SetMin(0);
            SetSec(0);
            GetTimerField().Start();

            InvokePrivateMethod("Timer_Elapsed", null!, EventArgs.Empty);

            Assert.That(GetTimerField().Enabled, Is.False);
        }

        /// <summary>
        /// Helper to produce a RoutedEventArgs instance for invoking click handlers via reflection.
        /// </summary>
        private class RoutedEventArgsStub
        {
            public System.Windows.RoutedEventArgs EventArgs { get; } = new System.Windows.RoutedEventArgs();
        }
    }
}
