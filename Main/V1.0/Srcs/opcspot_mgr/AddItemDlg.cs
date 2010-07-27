//------------------------------------------------------------------------------------------
// File: <AddItemDlg.cs>
// Purpose: implement <Add a new item dialog.>
//
// @author <Yun Hua>
// @version 1.0 2010/04/01
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace opcspot_mgr
{
    internal partial class AddItemDlg : Form
    {
        #region Fields

        private string inputStr = string.Empty;
        private char[] delimiter = new char[] { ',' };
        private string errorMsg = "Invalid format!";

        #endregion

        #region Constructors

        public AddItemDlg()
        {
            InitializeComponent();
        }

        #endregion

        #region Properties

        /// <summary>
        /// Get or set the item label.
        /// </summary>
        public string ItemLable
        {
            get { return this.labelName.Text; }
            set { this.labelName.Text = value; }
        }

        public uint[] Indices
        {
            get 
            {
                return ParseInput(this.inputStr);
            }
        }

        #endregion

        #region Private Methods

        private uint[] ParseInput(string input)
        {
            List<uint> items = new List<uint>();
            if (string.IsNullOrEmpty(input))
            {
                return items.ToArray();
            }

            string[] output =
                input.Split(delimiter, StringSplitOptions.RemoveEmptyEntries);
            if (0 ==output.Length)
            {
                return items.ToArray();
            }
            foreach (string str in output)
            {
                uint val = 0;
                if (!UInt32.TryParse(str, out val))
                {
                    return items.ToArray();
                }
            }

            foreach (string str in output)
            {
                items.Add(UInt32.Parse(str));
            }
            return items.ToArray();
        }


        #endregion

        #region Event Handlers

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            uint[] output = ParseInput(this.textBox1.Text);
            if (0 == output.Length)
            {
                this.buttonOK.Enabled = false;
                this.labelError.Text = errorMsg;
            }
            else
            {
                this.buttonOK.Enabled = true;
                this.labelError.Text = string.Empty;
                inputStr = this.textBox1.Text;
            }
        }

        #endregion

    }
}
