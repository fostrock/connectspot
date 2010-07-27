//------------------------------------------------------------------------------------------
// File: <PortParam.cs>
// Purpose: implement <Serial port parameter wrapper class.>
//
// @author <Yun Hua>
// @version 1.0 2010/04/01
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//
using System;
using System.Collections.Generic;
using System.Text;

namespace opcspot_mgr
{
    internal enum SerialParity
    {
        Odd, Even, None
    }

    internal class PortParam
    {
        #region Private Fields

        private string devName;
        private uint baudRate = 9600;
        private uint dataBits = 8;
        private uint stopBits = 1; // we don't support 1.5 here
        private SerialParity parity = SerialParity.None;
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

        #region Properties

        /// <summary>
        /// Get the device name.
        /// </summary>
        public string DevName
        {
            get { return devName; }
        }

        /// <summary>
        /// Get and set the device name.
        /// </summary>
        public uint BaudRate
        {
            get { return baudRate; }
            set { baudRate = value; }
        }

        /// <summary>
        /// Get the data bits.
        /// </summary>
        public uint DataBits
        {
            get { return dataBits; }
        }

        /// <summary>
        /// Get the stop bits.
        /// </summary>
        public uint StopBits
        {
            get { return stopBits; }
        }

        /// <summary>
        /// Get the parity.
        /// </summary>
        public SerialParity Parity
        {
            get { return parity; }
        }

        public Dictionary<uint, bool> Stations
        {
            get { return stations; }
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
            setting += delimiter[0];
            setting += dataBits.ToString();
            setting += delimiter[0];
            if (SerialParity.None == parity)
            {
                setting += "N";
            }
            else if (SerialParity.Even == parity)
            {
                setting += "E";
            }
            else
            {
                setting += "O";
            }
            setting += delimiter[0];
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
                setting.Append(delimiter[0]);
                if (pair.Value)
                {
                    setting.Append("Y");
                }
                else
                {
                    setting.Append("N");
                }
                setting.Append(delimiter[0]);
            }

            if (setting.Length > 0)
            {
                setting.Remove(setting.Length - 1, 1); // remove the last ","
            }
            return setting.ToString();
        }

        /// <summary>
        /// Update station for the specified port.
        /// </summary>
        /// <param name="station">The station No.</param>
        /// <param name="isActive">Whether the station is active.</param>
        /// <returns>True if add a new station, otherwise updating a station status</returns>
        public void UpdateStation(uint station, bool isActive)
        {
            if (stations.ContainsKey(station))
            {
                stations[station] = isActive;
            }
            else
            {
                stations.Add(station, isActive);
            }
        }

        /// <summary>
        /// Add a station to the specified port. The default active state is true.
        /// </summary>
        /// <param name="station">The station No.</param>
        /// <returns>True if a new station is added in, otherwise false.</returns>
        public bool AddStation(uint station)
        {
            if (stations.ContainsKey(station))
            {
                return false;
            }
            else
            {
                stations.Add(station, true);
                return true;
            }
        }

        /// <summary>
        /// Remove a station from the specified port.
        /// </summary>
        /// <param name="station">The station No.</param>
        public void RemoveStation(uint station)
        {
             stations.Remove(station);
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
                parity = SerialParity.None;
            }
            else if (0 == string.Compare(result[2], "E", StringComparison.OrdinalIgnoreCase))
            {
                parity = SerialParity.Even;
            }
            else if (0 == string.Compare(result[2], "O", StringComparison.OrdinalIgnoreCase))
            {
                parity = SerialParity.Odd;
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
}
