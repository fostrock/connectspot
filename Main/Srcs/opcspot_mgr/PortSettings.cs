//------------------------------------------------------------------------------------------
// File: <ZSSerial.cs>
// Purpose: implement <ZS weight port settings.>
//
// @author <Yun Hua>
// @version 1.0 2010/03/31
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace opcspot_mgr
{
    internal enum SerialParity
    {
        odd, even, none
    }

    internal class PortParam
    {
        #region Private Fields

        private string devName;
        private uint baudRate = 9600;
        private uint dataBits = 8;
        private uint stopBits = 1; // we don't support 1.5 here
        private SerialParity parity = SerialParity.none;
        private Dictionary<uint, bool> stations = new Dictionary<uint, bool>();
        private char[] delimiter = new char[] { ',' };

        #endregion

        #region Constructors

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="devName">The device name. For example, "COM3".</param>
        /// <param name="baudRate">The data bits per second</param>
        public PortParam(string devName, uint baudRate)
        {
            if (string.IsNullOrEmpty(devName))
            {
                throw new ArgumentNullException("devName");
            }

            this.devName = devName;
            this.baudRate = baudRate;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="devName">The device name. For example, "COM3".</param>
        /// <param name="setting">The setting string in the format: 9600,8,N,1</param>
        /// <param name="station">The station string in the format: 1,N,5,Y</param>
        public PortParam(string devName, string setting, string station)
        {
            if (string.IsNullOrEmpty(devName))
            {
                throw new ArgumentNullException("devName");
            }

            if (string.IsNullOrEmpty(setting))
            {
                throw new ArgumentNullException("setting");
            }

            if (string.IsNullOrEmpty(station))
            {
                throw new ArgumentNullException("station");
            }

            if (!ParsePortSetting(setting))
            {
                throw new ArgumentException("The serial port setting format is invalid.", "setting");
            }

            if (!ParseStationSetting(station))
            {
                throw new ArgumentException("The RS485 station format is invalid.", "station");
            }

            this.devName = devName;
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Format the serial port setting to a string using "," as delimiter
        /// </summary>
        /// <returns>The formated string</returns>
        public string FormatPortSetting()
        {
            string setting = string.Empty;
            setting += baudRate.ToString();
            setting += dataBits.ToString();
            if (SerialParity.none == parity)
            {
                setting += "N";
            }
            else if (SerialParity.even == parity)
            {
                setting += "E";
            }
            else
            {
                setting += "O";
            }
            setting += stopBits.ToString();
            return setting;
        }

        /// <summary>
        /// Format the station to a string using "," as delimiter. 
        /// </summary>
        /// <returns>The formated string</returns>
        public string FormatStationSetting()
        {
            StringBuilder setting = new StringBuilder();
            foreach (KeyValuePair<uint, bool> pair in stations)
            {
                setting.Append(pair.Key.ToString());
                if (pair.Value)
                {
                    setting.Append("Y");
                }
                else
                {
                    setting.Append("N");
                }
            }
            return setting.ToString();
        }

        #endregion

        #region Private Methods

        /// <summary>
        /// Parse the serial port setting.
        /// </summary>
        /// <param name="setting">The serial port setting string.</param>
        /// <returns>True if the operation succeeded, otherwise false</returns>
        bool ParsePortSetting(string setting)
        {
            string[] result = setting.Split(delimiter);
            if (result.Length != 4)
            {
                return false;
            }

            if (!UInt32.TryParse(result[0], out baudRate))
            {
                return false;
            }

            if (!UInt32.TryParse(result[1], out dataBits))
            {
                return false;
            }

            if (0 == string.Compare(result[2], "N", StringComparison.OrdinalIgnoreCase))
            {
                parity = SerialParity.none;
            }
            else if (0 == string.Compare(result[2], "E", StringComparison.OrdinalIgnoreCase))
            {
                parity = SerialParity.even;
            }
            else if (0 == string.Compare(result[2], "O", StringComparison.OrdinalIgnoreCase))
            {
                parity = SerialParity.odd;
            }
            else
            {
                return false;
            }

            if (!UInt32.TryParse(result[3], out stopBits))
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// Parse the RS485 station setting.
        /// </summary>
        /// <param name="setting">The station setting in format: 1,N,2,Y</param>
        /// <returns></returns>
        bool ParseStationSetting(string setting)
        {
            stations.Clear();
            string[] result = setting.Split(delimiter);
            if (0 != result.Length % 2) 
            {
                return false;
            }

            for (int i = 0; i < result.Length / 2; ++i)
            {
                uint station = 0;
                bool isActive = false;
                if (!UInt32.TryParse(result[2 * i], out station))
                {
                    return false;
                }

                if (0 == string.Compare(result[2 * i + 1], "Y", StringComparison.OrdinalIgnoreCase))
                {
                    isActive = true;
                }
                else if (0 == string.Compare(result[2 * i + 1], "N", StringComparison.OrdinalIgnoreCase))
                {
                    isActive = false;
                }
                else
                {
                    return false;
                }

                try
                {
                    stations.Add(station, isActive);
                }
                catch (System.ArgumentException)
                {
                    return false;
                }
            }

            return true;
        }

        #endregion
    }

    /// <summary>
    /// Port setting user control
    /// </summary>
    public partial class PortSettings : UserControl
    {
        #region Fields

        #endregion

        #region Constructors

        public PortSettings()
        {
            InitializeComponent();
        }
        #endregion

        #region Private Methods

        /// <summary>
        /// Load settings from XML file
        /// </summary>
        private void LoadSettings()
        {

        }

        /// <summary>
        /// Save settings to XML file
        /// </summary>
        private void SaveSettings()
        {

        }

        #endregion

        #region Public Methods

        #endregion
    }
}
