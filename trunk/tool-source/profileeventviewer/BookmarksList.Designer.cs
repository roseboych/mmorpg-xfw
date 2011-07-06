namespace ProfileEventViewer
{
    partial class BookmarksList
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
            this.m_BookmarksList = new System.Windows.Forms.ListView();
            this.ProfileName = new System.Windows.Forms.ColumnHeader();
            this.Time = new System.Windows.Forms.ColumnHeader();
            this.Calls = new System.Windows.Forms.ColumnHeader();
            this.Raw = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // m_BookmarksList
            // 
            this.m_BookmarksList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.m_BookmarksList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ProfileName,
            this.Time,
            this.Calls,
            this.Raw});
            this.m_BookmarksList.Location = new System.Drawing.Point(12, 12);
            this.m_BookmarksList.Name = "m_BookmarksList";
            this.m_BookmarksList.Size = new System.Drawing.Size(581, 383);
            this.m_BookmarksList.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.m_BookmarksList.TabIndex = 1;
            this.m_BookmarksList.UseCompatibleStateImageBehavior = false;
            this.m_BookmarksList.View = System.Windows.Forms.View.Details;
            this.m_BookmarksList.ItemActivate += new System.EventHandler(this.m_BookmarksList_ItemActivate);
            this.m_BookmarksList.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.m_BookmarksList_ColumnClick);
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
            this.Calls.Width = 0;
            // 
            // Raw
            // 
            this.Raw.Width = 120;
            // 
            // BookmarksList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(605, 407);
            this.Controls.Add(this.m_BookmarksList);
            this.Name = "BookmarksList";
            this.Text = "Bookmarks";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.BookmarksList_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView m_BookmarksList;
        private System.Windows.Forms.ColumnHeader ProfileName;
        private System.Windows.Forms.ColumnHeader Time;
        private System.Windows.Forms.ColumnHeader Calls;
        private System.Windows.Forms.ColumnHeader Raw;
    }
}