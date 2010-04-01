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
using System.IO;
using System.Xml;

namespace opcspot_mgr
{
    /// <summary>
    /// Port setting user control
    /// </summary>
    public partial class PortSettings : UserControl
    {
        #region Fields

        private string xmlFile;
        private List<PortParam> ports = new List<PortParam>();
        private readonly string ATTR_PORT = "port";
        private readonly string ATTR_SETTING = "setting";
        private readonly string ATTR_STATION = "stations";

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
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                xmlDoc.Load(xmlFile);
                XmlNodeList nodes = xmlDoc.SelectNodes("//zsdriver/serialport");
                foreach (XmlNode node in nodes)
                {
                    string devName = node.Attributes[ATTR_PORT].Value;
                    string setting = node.Attributes[ATTR_SETTING].Value;
                    string stations = node.Attributes[ATTR_STATION].Value;
                    PortParam port = new PortParam(devName, setting, stations);
                    ports.Add(port);
                }
            }
            catch
            {
                throw new InvalidOperationException("Parse the config file failed.");          
            }

            // Fill the port list control
            this.listBoxPort.Items.Clear();
            foreach (PortParam port in ports)
            {
                this.listBoxPort.Items.Add(port.DevName);
            }
            if (this.listBoxPort.Items.Count > 0)
            {
                this.listBoxPort.SelectedIndex = 0;
            }
        }

        /// <summary>
        /// Save settings to XML file
        /// </summary>
        private void SaveSettings()
        {
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                xmlDoc.Load(xmlFile);

                // Clear old nodes firstly
                XmlNodeList nodes = xmlDoc.SelectNodes("//zsdriver/serialport");
                foreach(XmlNode node in nodes)
                {
                    xmlDoc.DocumentElement.RemoveChild(node);
                }

                // Add new nodes
                foreach (PortParam port in ports)
                {
                    if (0 == port.Stations.Count) // Skip the empty content port
                    {
                        continue;
                    }

                    XmlElement element = xmlDoc.CreateElement("serialport");
                    element.SetAttribute(ATTR_PORT, port.DevName);
                    element.SetAttribute(ATTR_SETTING, port.FormatPortSetting());
                    element.SetAttribute(ATTR_STATION, port.FormatStationSetting());
                    xmlDoc.DocumentElement.AppendChild(element);
                }
                xmlDoc.Save(xmlFile);
            }
            catch
            {
                throw new InvalidOperationException("Parse the config file failed.");
            }
        }

        /// <summary>
        /// Prepare environment including the xml config file's location.
        /// </summary>
        private void PrepareEnvironment()
        {
            string mainPath = Utilities.GetMainPath();
            mainPath += "\\config\\zsdriver.xml";
            if (!File.Exists(mainPath))
            {
                throw new InvalidOperationException("Config file is not prepared");
            }
            xmlFile = mainPath;
        }

        /// <summary>
        /// Add a port.
        /// </summary>
        /// <param name="devName">The port name.</param>
        /// <returns>True if a new port is added in, otherwise false.</returns>
        private bool AddPort(string devName)
        {
            if (string.IsNullOrEmpty(devName))
            {
                throw new ArgumentException("The port name is invalid.", "devName");
            }

            foreach (PortParam item in ports)
            {
                if (0 == string.Compare(item.DevName, devName))
                {
                    return false;
                }
            }

            ports.Add(new PortParam(devName, 9600));
            return true;
        }

        /// <summary>
        /// Remove a port.
        /// </summary>
        /// <param name="devName">The port name.</param>
        private void RemovePort(string devName)
        {
            if (string.IsNullOrEmpty(devName))
            {
                throw new ArgumentException("The port name is invalid.", "devName");
            }

            PortParam port = null;
            foreach (PortParam item in ports)
            {
                if (0 == string.Compare(item.DevName, devName))
                {
                    port = item;
                }
            }
            if (null != port)
            {
                ports.Remove(port);
            }
        }

        #endregion

        #region UI Control Handlers

        /// <summary>
        /// Handler dealing with the selection changing in the list control.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void listBoxPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (null == this.listBoxPort.SelectedItem)
            {
                return;
            }

            string devName = this.listBoxPort.SelectedItem.ToString();
            foreach (PortParam port in ports)
            {
                if (0 ==string.Compare(port.DevName, devName, StringComparison.OrdinalIgnoreCase))
                {
                    int index = -1;
                    index = this.comboBoxBaud.Items.IndexOf(port.BaudRate.ToString());
                    this.comboBoxBaud.SelectedIndex = index;
                    index = this.comboBoxDataBits.Items.IndexOf(port.DataBits.ToString());
                    this.comboBoxDataBits.SelectedIndex = index;
                    index = this.comboBoxParity.Items.IndexOf(port.Parity.ToString());
                    this.comboBoxParity.SelectedIndex = index;
                    index = this.comboBoxStopBits.Items.IndexOf(port.StopBits.ToString());
                    this.comboBoxStopBits.SelectedIndex = index;

                    this.checkedListBoxStations.Items.Clear();
                    foreach (KeyValuePair<uint, bool> pair in port.Stations)
                    {
                        this.checkedListBoxStations.Items.Add(pair.Key.ToString(), pair.Value);
                    }
                }
            }
        }

        /// <summary>
        /// Add a port to the UI and the port model.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void addPortToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AddItemDlg dlg = new AddItemDlg();
            dlg.ItemLable = "COM";
            DialogResult res = dlg.ShowDialog();
            if (DialogResult.OK == res)
            {
                uint[] output = dlg.Indices;
                for (int i = 0; i < output.Length; ++i)
                {
                    if (AddPort(dlg.ItemLable + output[i].ToString()))
                    {
                        this.listBoxPort.Items.Add(dlg.ItemLable + output[i].ToString());
                    }
                }
            }
        }

        /// <summary>
        /// Remove a port from the UI and the port model.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void removePortToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListBox.SelectedIndexCollection indices = this.listBoxPort.SelectedIndices;
            List<string> items = new List<string>(indices.Count);
            foreach (int i in indices)
            {
                RemovePort(this.listBoxPort.Items[i].ToString());
                this.listBoxPort.Items.Remove(this.listBoxPort.Items[i].ToString());
            }
        }

        /// <summary>
        /// Set status for the menu strips while opening.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void contextMenuStripPort_Opening(object sender, CancelEventArgs e)
        {
            if (this.listBoxPort.Items.Count <= 1)
            {
                this.removePortToolStripMenuItem.Enabled = false;
            }
            else
            {
                this.removePortToolStripMenuItem.Enabled = true;
            }
        }

        /// <summary>
        /// Set status for the menu strips while opening.
        /// </summary>
        /// <param name="sender">The event handler.</param>
        /// <param name="e">The event argument.</param>
        private void contextMenuStripStation_Opening(object sender, CancelEventArgs e)
        {
            if (null == this.listBoxPort.SelectedItem)
            {
                this.addStationToolStripMenuItem.Enabled = false;
                this.removeStationToolStripMenuItem.Enabled = false;
            }
            else
            {
                this.addStationToolStripMenuItem.Enabled = true;
                this.removeStationToolStripMenuItem.Enabled = true;
            }

            if (null == this.checkedListBoxStations.SelectedItem)
            {
                this.removeStationToolStripMenuItem.Enabled = false;
            }
            else
            {
                this.removeStationToolStripMenuItem.Enabled = true;
            }
        }

        /// <summary>
        /// Add stations for the current port.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void addStationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null == this.listBoxPort.SelectedItem)
            {
                return;
            }

            AddItemDlg dlg = new AddItemDlg();
            dlg.ItemLable = "Station";
            DialogResult res = dlg.ShowDialog();
            if (DialogResult.OK == res)
            {
                uint[] output = dlg.Indices;
                PortParam port = null;
                foreach (PortParam item in ports)
                {
                    if (0 == string.Compare(item.DevName, this.listBoxPort.SelectedItem.ToString(), 
                        StringComparison.OrdinalIgnoreCase))
                    {
                        port = item;
                        break;
                    }

                }
                for (int i = 0; i < output.Length; ++i)
                {
                    if (port.AddStation(output[i]))
                    {
                        this.checkedListBoxStations.Items.Add(output[i].ToString(), true);
                    }
                }
            }
        }

        /// <summary>
        /// Remove a station from the UI.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void removeStationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (null == this.listBoxPort.SelectedItem)
            {
                return;
            }

            PortParam port = null;
            foreach (PortParam item in ports)
            {
                if (0 == string.Compare(item.DevName, this.listBoxPort.SelectedItem.ToString(),
                    StringComparison.OrdinalIgnoreCase))
                {
                    port = item;
                    break;
                }
            }

            port.RemoveStation(
                UInt32.Parse(this.checkedListBoxStations.SelectedItem.ToString()));
            this.checkedListBoxStations.Items.Remove(this.checkedListBoxStations.SelectedItem);
        }

        /// <summary>
        /// The item's check changing handler.
        /// </summary>
        /// <param name="sender">The event sender.</param>
        /// <param name="e">The event argument.</param>
        private void checkedListBoxStations_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (null == this.checkedListBoxStations.SelectedItem)
            {
                return;
            }

            PortParam port = null;
            foreach (PortParam item in ports)
            {
                if (0 == string.Compare(item.DevName, this.listBoxPort.SelectedItem.ToString(),
                    StringComparison.OrdinalIgnoreCase))
                {
                    port = item;
                    break;
                }
            }

            port.UpdateStation(
                UInt32.Parse(this.checkedListBoxStations.SelectedItem.ToString()), 
                e.NewValue == CheckState.Checked ? true : false);
        }

        private void comboBoxBaud_SelectedIndexChanged(object sender, EventArgs e)
        {
            PortParam port = null;
            foreach (PortParam item in ports)
            {
                if (0 == string.Compare(item.DevName, this.listBoxPort.SelectedItem.ToString(),
                    StringComparison.OrdinalIgnoreCase))
                {
                    port = item;
                    break;
                }
            }

            port.BaudRate = UInt32.Parse(this.comboBoxBaud.SelectedItem.ToString());
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Load settings.
        /// </summary>
        public void LoadAll()
        {
            PrepareEnvironment();
            LoadSettings();
        }

        /// <summary>
        /// Save settings.
        /// </summary>
        public void SaveAll()
        {
            SaveSettings();
        }

        #endregion
    }
}
