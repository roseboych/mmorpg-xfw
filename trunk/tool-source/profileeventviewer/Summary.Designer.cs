namespace ProfileEventViewer
{
    partial class Summary
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_SummaryList = new System.Windows.Forms.ListView();
            this.ProfileName = new System.Windows.Forms.ColumnHeader();
            this.Time = new System.Windows.Forms.ColumnHeader();
            this.Calls = new System.Windows.Forms.ColumnHeader();
            this.rawTime = new System.Windows.Forms.ColumnHeader();
            this.ThreadName = new System.Windows.Forms.ColumnHeader();
            this.ThreadSelectionCombo = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // m_SummaryList
            // 
            this.m_SummaryList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_SummaryList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ProfileName,
            this.Time,
            this.Calls,
            this.rawTime,
            this.ThreadName});
            this.m_SummaryList.Location = new System.Drawing.Point(0, 39);
            this.m_SummaryList.Name = "m_SummaryList";
            this.m_SummaryList.Size = new System.Drawing.Size(535, 391);
            this.m_SummaryList.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.m_SummaryList.TabIndex = 0;
            this.m_SummaryList.UseCompatibleStateImageBehavior = false;
            this.m_SummaryList.View = System.Windows.Forms.View.Details;
            this.m_SummaryList.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.m_SummaryList_ColumnClick);
            // 
            // ProfileName
            // 
            this.ProfileName.Text = "Profile Name";
            this.ProfileName.Width = 202;
            // 
            // Time
            // 
            this.Time.Text = "Time";
            this.Time.Width = 71;
            // 
            // Calls
            // 
            this.Calls.Text = "Calls";
            this.Calls.Width = 110;
            // 
            // rawTime
            // 
            this.rawTime.Text = "Raw Time";
            this.rawTime.Width = 0;
            // 
            // ThreadName
            // 
            this.ThreadName.Text = "Thread";
            // 
            // ThreadSelectionCombo
            // 
            this.ThreadSelectionCombo.FormattingEnabled = true;
            this.ThreadSelectionCombo.Location = new System.Drawing.Point(12, 12);
            this.ThreadSelectionCombo.Name = "ThreadSelectionCombo";
            this.ThreadSelectionCombo.Size = new System.Drawing.Size(148, 21);
            this.ThreadSelectionCombo.TabIndex = 1;
            this.ThreadSelectionCombo.SelectedIndexChanged += new System.EventHandler(this.ThreadSelectionCombo_SelectedIndexChanged);
            // 
            // Summary
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(535, 430);
            this.Controls.Add(this.ThreadSelectionCombo);
            this.Controls.Add(this.m_SummaryList);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "Summary";
            this.Text = "Selection Summary";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Summary_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColumnHeader ProfileName;
        private System.Windows.Forms.ColumnHeader Time;
        private System.Windows.Forms.ColumnHeader Calls;
        private System.Windows.Forms.ListView m_SummaryList;
        private System.Windows.Forms.ColumnHeader rawTime;
        private System.Windows.Forms.ColumnHeader ThreadName;
        private System.Windows.Forms.ComboBox ThreadSelectionCombo;
    }
}