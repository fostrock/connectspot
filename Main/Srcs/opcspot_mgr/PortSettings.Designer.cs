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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.listViewFilterSetting = new System.Windows.Forms.ListView();
            this.colFilterName = new System.Windows.Forms.ColumnHeader();
            this.colFilterValue = new System.Windows.Forms.ColumnHeader();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxFilterThreshold = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonUpdateFilter = new System.Windows.Forms.Button();
            this.groupBoxRefresh = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxSecRatio = new System.Windows.Forms.ComboBox();
            this.textBoxMainRefresh = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
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
            this.groupBox1.SuspendLayout();
            this.groupBoxRefresh.SuspendLayout();
            this.contextMenuStripStation.SuspendLayout();
            this.groupBoxPortSettings.SuspendLayout();
            this.contextMenuStripPort.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.groupBox1);
            this.panel1.Controls.Add(this.groupBoxRefresh);
            this.panel1.Controls.Add(this.labelStations);
            this.panel1.Controls.Add(this.checkedListBoxStations);
            this.panel1.Controls.Add(this.groupBoxPortSettings);
            this.panel1.Controls.Add(this.listBoxPort);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(595, 375);
            this.panel1.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.listViewFilterSetting);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.textBoxFilterThreshold);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.buttonUpdateFilter);
            this.groupBox1.Location = new System.Drawing.Point(367, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(226, 363);
            this.groupBox1.TabIndex = 23;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Digital Filter";
            // 
            // listViewFilterSetting
            // 
            this.listViewFilterSetting.Activation = System.Windows.Forms.ItemActivation.OneClick;
            this.listViewFilterSetting.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colFilterName,
            this.colFilterValue});
            this.listViewFilterSetting.HideSelection = false;
            this.listViewFilterSetting.Location = new System.Drawing.Point(6, 19);
            this.listViewFilterSetting.MultiSelect = false;
            this.listViewFilterSetting.Name = "listViewFilterSetting";
            this.listViewFilterSetting.Size = new System.Drawing.Size(214, 271);
            this.listViewFilterSetting.TabIndex = 17;
            this.listViewFilterSetting.UseCompatibleStateImageBehavior = false;
            this.listViewFilterSetting.View = System.Windows.Forms.View.Details;
            this.listViewFilterSetting.SelectedIndexChanged += new System.EventHandler(this.listViewFilterSetting_SelectedIndexChanged);
            // 
            // colFilterName
            // 
            this.colFilterName.Text = "Name";
            this.colFilterName.Width = 147;
            // 
            // colFilterValue
            // 
            this.colFilterValue.Text = "Threshold";
            this.colFilterValue.Width = 63;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 338);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(158, 13);
            this.label5.TabIndex = 22;
            this.label5.Text = "0 means No filter will be applied.";
            // 
            // textBoxFilterThreshold
            // 
            this.textBoxFilterThreshold.Location = new System.Drawing.Point(6, 298);
            this.textBoxFilterThreshold.Name = "textBoxFilterThreshold";
            this.textBoxFilterThreshold.Size = new System.Drawing.Size(124, 20);
            this.textBoxFilterThreshold.TabIndex = 19;
            this.textBoxFilterThreshold.TextChanged += new System.EventHandler(this.textBoxFilterThreshold_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 322);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(221, 13);
            this.label4.TabIndex = 21;
            this.label4.Text = "The filter threshold shall in [0, 2] (0% ~ 200%),";
            // 
            // buttonUpdateFilter
            // 
            this.buttonUpdateFilter.Location = new System.Drawing.Point(145, 296);
            this.buttonUpdateFilter.Name = "buttonUpdateFilter";
            this.buttonUpdateFilter.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdateFilter.TabIndex = 20;
            this.buttonUpdateFilter.Text = "Update";
            this.buttonUpdateFilter.UseVisualStyleBackColor = true;
            this.buttonUpdateFilter.Click += new System.EventHandler(this.buttonUpdateFilter_Click);
            // 
            // groupBoxRefresh
            // 
            this.groupBoxRefresh.Controls.Add(this.label1);
            this.groupBoxRefresh.Controls.Add(this.comboBoxSecRatio);
            this.groupBoxRefresh.Controls.Add(this.textBoxMainRefresh);
            this.groupBoxRefresh.Controls.Add(this.label2);
            this.groupBoxRefresh.Location = new System.Drawing.Point(3, 299);
            this.groupBoxRefresh.Name = "groupBoxRefresh";
            this.groupBoxRefresh.Size = new System.Drawing.Size(358, 67);
            this.groupBoxRefresh.TabIndex = 16;
            this.groupBoxRefresh.TabStop = false;
            this.groupBoxRefresh.Text = "Data Scan Interval";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label1.Location = new System.Drawing.Point(6, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Main (10 - 2000ms)";
            // 
            // comboBoxSecRatio
            // 
            this.comboBoxSecRatio.FormattingEnabled = true;
            this.comboBoxSecRatio.Items.AddRange(new object[] {
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"});
            this.comboBoxSecRatio.Location = new System.Drawing.Point(131, 32);
            this.comboBoxSecRatio.Name = "comboBoxSecRatio";
            this.comboBoxSecRatio.Size = new System.Drawing.Size(114, 21);
            this.comboBoxSecRatio.TabIndex = 15;
            // 
            // textBoxMainRefresh
            // 
            this.textBoxMainRefresh.Location = new System.Drawing.Point(6, 32);
            this.textBoxMainRefresh.Name = "textBoxMainRefresh";
            this.textBoxMainRefresh.Size = new System.Drawing.Size(114, 20);
            this.textBoxMainRefresh.TabIndex = 12;
            this.textBoxMainRefresh.TextChanged += new System.EventHandler(this.textBoxMainRefresh_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label2.Location = new System.Drawing.Point(132, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(86, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "Secondary Ratio";
            // 
            // labelStations
            // 
            this.labelStations.AutoSize = true;
            this.labelStations.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelStations.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.labelStations.Location = new System.Drawing.Point(130, 138);
            this.labelStations.Name = "labelStations";
            this.labelStations.Size = new System.Drawing.Size(84, 13);
            this.labelStations.TabIndex = 11;
            this.labelStations.Text = "RS-485 Stations";
            // 
            // checkedListBoxStations
            // 
            this.checkedListBoxStations.ContextMenuStrip = this.contextMenuStripStation;
            this.checkedListBoxStations.FormattingEnabled = true;
            this.checkedListBoxStations.Location = new System.Drawing.Point(129, 154);
            this.checkedListBoxStations.Name = "checkedListBoxStations";
            this.checkedListBoxStations.Size = new System.Drawing.Size(232, 139);
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
            this.contextMenuStripStation.Size = new System.Drawing.Size(167, 48);
            this.contextMenuStripStation.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStripStation_Opening);
            // 
            // addStationToolStripMenuItem
            // 
            this.addStationToolStripMenuItem.Name = "addStationToolStripMenuItem";
            this.addStationToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
            this.addStationToolStripMenuItem.Text = "Add Station...";
            this.addStationToolStripMenuItem.Click += new System.EventHandler(this.addStationToolStripMenuItem_Click);
            // 
            // removeStationToolStripMenuItem
            // 
            this.removeStationToolStripMenuItem.Name = "removeStationToolStripMenuItem";
            this.removeStationToolStripMenuItem.Size = new System.Drawing.Size(166, 22);
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
            this.groupBoxPortSettings.Location = new System.Drawing.Point(129, 3);
            this.groupBoxPortSettings.Name = "groupBoxPortSettings";
            this.groupBoxPortSettings.Size = new System.Drawing.Size(232, 129);
            this.groupBoxPortSettings.TabIndex = 9;
            this.groupBoxPortSettings.TabStop = false;
            this.groupBoxPortSettings.Text = "Port Settings";
            // 
            // labelBaud
            // 
            this.labelBaud.AutoSize = true;
            this.labelBaud.Location = new System.Drawing.Point(6, 16);
            this.labelBaud.Name = "labelBaud";
            this.labelBaud.Size = new System.Drawing.Size(83, 13);
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
            this.comboBoxStopBits.Location = new System.Drawing.Point(113, 98);
            this.comboBoxStopBits.Name = "comboBoxStopBits";
            this.comboBoxStopBits.Size = new System.Drawing.Size(103, 21);
            this.comboBoxStopBits.TabIndex = 8;
            // 
            // labelDataBits
            // 
            this.labelDataBits.AutoSize = true;
            this.labelDataBits.Location = new System.Drawing.Point(37, 44);
            this.labelDataBits.Name = "labelDataBits";
            this.labelDataBits.Size = new System.Drawing.Size(52, 13);
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
            this.comboBoxParity.Location = new System.Drawing.Point(113, 70);
            this.comboBoxParity.Name = "comboBoxParity";
            this.comboBoxParity.Size = new System.Drawing.Size(103, 21);
            this.comboBoxParity.TabIndex = 7;
            // 
            // labelParity
            // 
            this.labelParity.AutoSize = true;
            this.labelParity.Location = new System.Drawing.Point(53, 73);
            this.labelParity.Name = "labelParity";
            this.labelParity.Size = new System.Drawing.Size(36, 13);
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
            this.comboBoxDataBits.Location = new System.Drawing.Point(113, 41);
            this.comboBoxDataBits.Name = "comboBoxDataBits";
            this.comboBoxDataBits.Size = new System.Drawing.Size(103, 21);
            this.comboBoxDataBits.TabIndex = 6;
            // 
            // labelStopBits
            // 
            this.labelStopBits.AutoSize = true;
            this.labelStopBits.Location = new System.Drawing.Point(37, 101);
            this.labelStopBits.Name = "labelStopBits";
            this.labelStopBits.Size = new System.Drawing.Size(52, 13);
            this.labelStopBits.TabIndex = 4;
            this.labelStopBits.Text = "Stop Bits:";
            // 
            // comboBoxBaud
            // 
            this.comboBoxBaud.FormattingEnabled = true;
            this.comboBoxBaud.Items.AddRange(new object[] {
            "4800",
            "9600",
            "19200"});
            this.comboBoxBaud.Location = new System.Drawing.Point(113, 13);
            this.comboBoxBaud.Name = "comboBoxBaud";
            this.comboBoxBaud.Size = new System.Drawing.Size(103, 21);
            this.comboBoxBaud.TabIndex = 5;
            this.comboBoxBaud.SelectedIndexChanged += new System.EventHandler(this.comboBoxBaud_SelectedIndexChanged);
            // 
            // listBoxPort
            // 
            this.listBoxPort.ContextMenuStrip = this.contextMenuStripPort;
            this.listBoxPort.FormattingEnabled = true;
            this.listBoxPort.Location = new System.Drawing.Point(3, 3);
            this.listBoxPort.Name = "listBoxPort";
            this.listBoxPort.Size = new System.Drawing.Size(120, 290);
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
            this.contextMenuStripPort.Size = new System.Drawing.Size(150, 48);
            this.contextMenuStripPort.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStripPort_Opening);
            // 
            // addPortToolStripMenuItem
            // 
            this.addPortToolStripMenuItem.Name = "addPortToolStripMenuItem";
            this.addPortToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.addPortToolStripMenuItem.Text = "Add Port...";
            this.addPortToolStripMenuItem.Click += new System.EventHandler(this.addPortToolStripMenuItem_Click);
            // 
            // removePortToolStripMenuItem
            // 
            this.removePortToolStripMenuItem.Name = "removePortToolStripMenuItem";
            this.removePortToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
            this.removePortToolStripMenuItem.Text = "Remove Port";
            this.removePortToolStripMenuItem.Click += new System.EventHandler(this.removePortToolStripMenuItem_Click);
            // 
            // PortSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.panel1);
            this.Name = "PortSettings";
            this.Size = new System.Drawing.Size(595, 375);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBoxRefresh.ResumeLayout(false);
            this.groupBoxRefresh.PerformLayout();
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
        private System.Windows.Forms.ComboBox comboBoxSecRatio;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxMainRefresh;
        private System.Windows.Forms.GroupBox groupBoxRefresh;
        private System.Windows.Forms.ListView listViewFilterSetting;
        private System.Windows.Forms.ColumnHeader colFilterName;
        private System.Windows.Forms.ColumnHeader colFilterValue;
        private System.Windows.Forms.Button buttonUpdateFilter;
        private System.Windows.Forms.TextBox textBoxFilterThreshold;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}
