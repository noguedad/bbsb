using DevExpress.Xpf.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;

namespace winBBSB
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : ThemedWindow
    {
        BluetoothClient btClient = new BluetoothClient();
        BluetoothDeviceInfo btDevice = null;
        System.IO.Stream stream = null;

        public MainWindow()
        {
            InitializeComponent();
          
        }

        private void Cancel_Wizard(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void DevideUpdate()
        {
            device_list.Gallery.Groups[0].Items.Clear();
            
            foreach (var device in btClient.DiscoverDevices())
            {
                if (true) {
                        device_list.Gallery.Groups[0].Items.Add(new DevExpress.Xpf.Bars.GalleryItem() { Name = device.DeviceName, Caption = $"{device.DeviceName}", Tag = device.DeviceAddress });
            } 
            }
        }

        private void click_update(object sender, RoutedEventArgs e)
        {
            DevideUpdate();
            
        }

        private void Finish_Wizard(object sender, System.ComponentModel.CancelEventArgs e)
        {
            MessageBox.Show("Terminado");
        }

        private void ThemedWindow_Loaded(object sender, RoutedEventArgs e)
        {
  device_list.Gallery.Groups.Add(new DevExpress.Xpf.Bars.GalleryItemGroup() { Caption = "Dispositivos Bluetooth" });
            DevideUpdate(); 
        }
    }
}
