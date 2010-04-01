namespace opcspot_mgr
{
    partial class PortSettings
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.labelStations = new System.Windows.Forms.Label();
            this.checkedListBoxStations = new System.Windows.Forms.CheckedListBox();
            this.contextMenuStripStation = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addStationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeStationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBoxPortSettings = new System.Windows.Forms.GroupBox();
            this.labelBaud = new System.Windows.Forms.Label();
            this.comboBoxStopBits = new System.Windows.Forms.ComboBox();
            this.labelDataBits = new System.Windows.Forms.Label();
            this.comboBoxParity = new System.Windows.Forms.ComboBox();
            this.labelParity = new System.Windows.Forms.Label();
            this.comboBoxDataBits = new System.Windows.Forms.ComboBox();
            this.labelStopBits = new System.Windows.Forms.Label();
            this.comboBoxBaud = new System.Windows.Forms.ComboBox();
            this.listBoxPort = new System.Windows.Forms.ListBox();
            this.contextMenuStripPort = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addPortToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removePortToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1.SuspendLayout();
            this.contextMenuStripStation.SuspendLayout();
            this.groupBoxPortSettings.SuspendLayout();
            this.contextMenuStripPort.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.labelStations);
            this.panel1.Controls.Add(this.checkedListBoxStations);
            this.panel1.Controls.Add(this.groupBoxPortSettings);
            this.panel1.Controls.Add(this.listBoxPort);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(367, 277);
            this.panel1.TabIndex = 0;
            // 
            // labelStations
            // 
            this.labelStations.AutoSize = true;
            this.labelStations.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelStations.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelStations.Location = new System.Drawing.Point(130, 122);
            this.labelStations.Name = "labelStations";
            this.labelStations.Size = new System.Drawing.Size(84, 13);
            this.labelStations.TabIndex = 11;
            this.labelStations.Text = "RS-485 Stations";
            // 
            // checkedListBoxStations
            // 
            this.checkedListBoxStations.ContextMenuStrip = this.contextMenuStripStation;
            this.checkedListBoxStations.FormattingEnabled = true;
            this.checkedListBoxStations.Location = new System.Drawing.Point(129, 139);
            this.checkedListBoxStations.Name = "checkedListBoxStations";
            this.checkedListBoxStations.Size = new System.Drawing.Size(232, 132);
            this.checkedListBoxStations.Sorted = true;
            this.checkedListBoxStations.TabIndex = 10;
            this.checkedListBoxStations.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.checkedListBoxStations_ItemCheck);
            // 
            // contextMenuStripStation
            // 
            this.contextMenuStripStation.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addStationToolStripMenuItem,
            this.removeStationToolStripMenuItem});
            this.contextMenuStripStation.Name = "contextMenuStripStation";
            this.contextMenuStripStation.Size = new System.Drawing.Size(155, 48);
            this.contextMenuStripStation.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStripStation_Opening);
            // 
            // addStationToolStripMenuItem
            // 
            this.addStationToolStripMenuItem.Name = "addStationToolStripMenuItem";
            this.addStationToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.addStationToolStripMenuItem.Text = "Add Station";
            this.addStationToolStripMenuItem.Click += new System.EventHandler(this.addStationToolStripMenuItem_Click);
            // 
            // removeStationToolStripMenuItem
            // 
            this.removeStationToolStripMenuItem.Name = "removeStationToolStripMenuItem";
            this.removeStationToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.removeStationToolStripMenuItem.Text = "Remove Station";
            this.removeStationToolStripMenuItem.Click += new System.EventHandler(this.removeStationToolStripMenuItem_Click);
            // 
            // groupBoxPortSettings
            // 
            this.groupBoxPortSettings.Controls.Add(this.labelBaud);
            this.groupBoxPortSettings.Controls.Add(this.comboBoxStopBits);
            this.groupBoxPortSettings.Controls.Add(this.labelDataBits);
            this.groupBoxPortSettings.Controls.Add(this.comboBoxParity);
            this.groupBoxPortSettings.Controls.Add(this.labelParity);
            this.groupBoxPortSettings.Controls.Add(this.comboBoxDataBits);
            this.groupBoxPortSettings.Controls.Add(this.labelStopBits);
            this.groupBoxPortSettings.Controls.Add(this.comboBoxBaud);
            this.groupBoxPortSettings.Location = new System.Drawing.Point(129, -3);
            this.groupBoxPortSettings.Name = "groupBoxPortSettings";
            this.groupBoxPortSettings.Size = new System.Drawing.Size(232, 118);
            this.groupBoxPortSettings.TabIndex = 9;
            this.groupBoxPortSettings.TabStop = false;
            // 
            // labelBaud
            // 
            this.labelBaud.AutoSize = true;
            this.labelBaud.Location = new System.Drawing.Point(6, 15);
            this.labelBaud.Name = "labelBaud";
            this.labelBaud.Size = new System.Drawing.Size(101, 12);
            this.labelBaud.TabIndex = 1;
            this.labelBaud.Text = "Bits per second:";
            // 
            // comboBoxStopBits
            // 
            this.comboBoxStopBits.Enabled = false;
            this.comboBoxStopBits.FormattingEnabled = true;
            this.comboBoxStopBits.Items.AddRange(new object[] {
            "1",
            "1.5",
            "2"});
            this.comboBoxStopBits.Location = new System.Drawing.Point(113, 90);
            this.comboBoxStopBits.Name = "comboBoxStopBits";
            this.comboBoxStopBits.Size = new System.Drawing.Size(103, 20);
            this.comboBoxStopBits.TabIndex = 8;
            // 
            // labelDataBits
            // 
            this.labelDataBits.AutoSize = true;
            this.labelDataBits.Location = new System.Drawing.Point(37, 41);
            this.labelDataBits.Name = "labelDataBits";
            this.labelDataBits.Size = new System.Drawing.Size(65, 12);
            this.labelDataBits.TabIndex = 2;
            this.labelDataBits.Text = "Data bits:";
            // 
            // comboBoxParity
            // 
            this.comboBoxParity.Enabled = false;
            this.comboBoxParity.FormattingEnabled = true;
            this.comboBoxParity.Items.AddRange(new object[] {
            "Even",
            "Odd",
            "None"});
            this.comboBoxParity.Location = new System.Drawing.Point(113, 65);
            this.comboBoxParity.Name = "comboBoxParity";
            this.comboBoxParity.Size = new System.Drawing.Size(103, 20);
            this.comboBoxParity.TabIndex = 7;
            // 
            // labelParity
            // 
            this.labelParity.AutoSize = true;
            this.labelParity.Location = new System.Drawing.Point(53, 67);
            this.labelParity.Name = "labelParity";
            this.labelParity.Size = new System.Drawing.Size(47, 12);
            this.labelParity.TabIndex = 3;
            this.labelParity.Text = "Parity:";
            // 
            // comboBoxDataBits
            // 
            this.comboBoxDataBits.Enabled = false;
            this.comboBoxDataBits.FormattingEnabled = true;
            this.comboBoxDataBits.Items.AddRange(new object[] {
            "4",
            "5",
            "6",
            "7",
            "8"});
            this.comboBoxDataBits.Location = new System.Drawing.Point(113, 38);
            this.comboBoxDataBits.Name = "comboBoxDataBits";
            this.comboBoxDataBits.Size = new System.Drawing.Size(103, 20);
            this.comboBoxDataBits.TabIndex = 6;
            // 
            // labelStopBits
            // 
            this.labelStopBits.AutoSize = true;
            this.labelStopBits.Location = new System.Drawing.Point(37, 93);
            this.labelStopBits.Name = "labelStopBits";
            this.labelStopBits.Size = new System.Drawing.Size(65, 12);
            this.labelStopBits.TabIndex = 4;
            this.labelStopBits.Text = "Stop Bits:";
            // 
            // comboBoxBaud
            // 
            this.comboBoxBaud.FormattingEnabled = true;
            this.comboBoxBaud.Items.AddRange(new object[] {
            "9600",
            "19200"});
            this.comboBoxBaud.Location = new System.Drawing.Point(113, 12);
            this.comboBoxBaud.Name = "comboBoxBaud";
            this.comboBoxBaud.Size = new System.Drawing.Size(103, 20);
            this.comboBoxBaud.TabIndex = 5;
            this.comboBoxBaud.SelectedIndexChanged += new System.EventHandler(this.comboBoxBaud_SelectedIndexChanged);
            // 
            // listBoxPort
            // 
            this.listBoxPort.ContextMenuStrip = this.contextMenuStripPort;
            this.listBoxPort.FormattingEnabled = true;
            this.listBoxPort.ItemHeight = 12;
            this.listBoxPort.Location = new System.Drawing.Point(3, 3);
            this.listBoxPort.Name = "listBoxPort";
            this.listBoxPort.Size = new System.Drawing.Size(120, 268);
            this.listBoxPort.Sorted = true;
            this.listBoxPort.TabIndex = 0;
            this.listBoxPort.SelectedIndexChanged += new System.EventHandler(this.listBoxPort_SelectedIndexChanged);
            // 
            // contextMenuStripPort
            // 
            this.contextMenuStripPort.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addPortToolStripMenuItem,
            this.removePortToolStripMenuItem});
            this.contextMenuStripPort.Name = "contextMenuStripPort";
            this.contextMenuStripPort.Size = new System.Drawing.Size(137, 48);
            this.contextMenuStripPort.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStripPort_Opening);
            // 
            // addPortToolStripMenuItem
            // 
            this.addPortToolStripMenuItem.Name = "addPortToolStripMenuItem";
            this.addPortToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.addPortToolStripMenuItem.Text = "Add Port";
            this.addPortToolStripMenuItem.Click += new System.EventHandler(this.addPortToolStripMenuItem_Click);
            // 
            // removePortToolStripMenuItem
            // 
            this.removePortToolStripMenuItem.Name = "removePortToolStripMenuItem";
            this.removePortToolStripMenuItem.Size = new System.Drawing.Size(136, 22);
            this.removePortToolStripMenuItem.Text = "Remove Port";
            this.removePortToolStripMenuItem.Click += new System.EventHandler(this.removePortToolStripMenuItem_Click);
            // 
            // PortSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PortSettings";
            this.Size = new System.Drawing.Size(367, 277);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.contextMenuStripStation.ResumeLayout(false);
            this.groupBoxPortSettings.ResumeLayout(false);
            this.groupBoxPortSettings.PerformLayout();
            this.contextMenuStripPort.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ListBox listBoxPort;
        private System.Windows.Forms.ComboBox comboBoxStopBits;
        private System.Windows.Forms.ComboBox comboBoxParity;
        private System.Windows.Forms.ComboBox comboBoxDataBits;
        private System.Windows.Forms.ComboBox comboBoxBaud;
        private System.Windows.Forms.Label labelStopBits;
        private System.Windows.Forms.Label labelParity;
        private System.Windows.Forms.Label labelDataBits;
        private System.Windows.Forms.Label labelBaud;
        private System.Windows.Forms.GroupBox groupBoxPortSettings;
        private System.Windows.Forms.CheckedListBox checkedListBoxStations;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripStation;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripPort;
        private System.Windows.Forms.ToolStripMenuItem addStationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeStationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addPortToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removePortToolStripMenuItem;
        private System.Windows.Forms.Label labelStations;
    }
}
