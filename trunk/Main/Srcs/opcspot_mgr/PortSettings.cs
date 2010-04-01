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
            PrepareEnvironment();
            LoadSettings();
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
                    xmlDoc.RemoveChild(node);
                }

                // Add new nodes
                foreach (PortParam port in ports)
                {
                    XmlElement element = xmlDoc.CreateElement("serialport");
                    element.SetAttribute(ATTR_PORT, port.DevName);
                    element.SetAttribute(ATTR_SETTING, port.FormatPortSetting());
                    element.SetAttribute(ATTR_STATION, port.FormatStationSetting());
                    xmlDoc.AppendChild(element);
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
            foreach (int i in indices)
            {
                RemovePort(this.listBoxPort.Items[i].ToString());
            }
            for (int i = indices.Count - 1; i >= 0; i--)
            {
                this.listBoxPort.Items.RemoveAt(i);
            }
        }

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

        private void contextMenuStripStation_Opening(object sender, CancelEventArgs e)
        {

        }

        #endregion

        #region Public Methods

        #endregion
    }
}
