using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace opcspot_mgr
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            this.portSettings1.LoadAll();
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {
            if (this.portSettings1.SaveAll())
            {
                this.Close();
            }   
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void portSettings1_Load(object sender, EventArgs e)
        {

        }
    }
}
