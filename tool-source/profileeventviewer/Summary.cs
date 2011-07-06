using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO;

// Keep in sync with ProfileEvent.cs
using Time = System.Double;

namespace ProfileEventViewer
{
    public partial class Summary : Form
    {
        private ListViewColumnSorter lvwColumnSorter;
        private ProfileEventsViewer m_Parent = null;

        public Summary( ProfileEventsViewer parent)
        {
            InitializeComponent();

            lvwColumnSorter = new ListViewColumnSorter();
            m_SummaryList.ListViewItemSorter = lvwColumnSorter;
            m_Parent = parent; 
        }

        void m_SummaryList_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            if (e.Column == lvwColumnSorter.SortColumn)
            {
                if (lvwColumnSorter.Order == SortOrder.Ascending)
                {
                    lvwColumnSorter.Order = SortOrder.Descending;
                }
                else
                {
                    lvwColumnSorter.Order = SortOrder.Ascending;
                }
            }
            else
            {
                lvwColumnSorter.SortColumn = e.Column;
                lvwColumnSorter.Order = SortOrder.Ascending;
            }

            m_SummaryList.Sort();
        }

        public void PostShow(ProfileEventsViewer pev)
        {
            ThreadSelectionCombo.Items.Clear();
            ThreadSelectionCombo.Items.Add("All Threads");
            if (pev.Data != null)
            {
                foreach (ProfileEventData.ProfileContext context in pev.Data.Contexts.Values)
                {
                    ThreadSelectionCombo.Items.Add(context.Name);
                }
            }

            ThreadSelectionCombo.SelectedItem = ThreadSelectionCombo.Items[0];
        }

        public void Update(ProfileEventsViewer pev)
        {
            ListViewItem listviewitem;

            m_SummaryList.View = View.Details;

            m_SummaryList.Items.Clear();

            foreach (UInt32 id in pev.SelectionTimes.Keys)
            {
                ProfileEventsViewer.SelectionInfo info = (ProfileEventsViewer.SelectionInfo)pev.SelectionTimes[id];
                foreach( String name in info.m_context.Keys )
                {
                    if(ThreadSelectionCombo.SelectedItem.Equals(name) || ThreadSelectionCombo.SelectedItem.Equals("All Threads"))
                    {
                        ProfileEventsViewer.SelectionInfoContext selContext = (ProfileEventsViewer.SelectionInfoContext)info.m_context[name];
                        Time time = selContext.m_TotalTime;
                        if (time > (Time)0)
                        {
                            listviewitem = new ListViewItem(pev.Data.GetProfileName(id));
                            listviewitem.SubItems.Add(pev.TimeToString((UInt64)time));
                            listviewitem.SubItems.Add(selContext.m_Calls.ToString());
                            listviewitem.SubItems.Add(((UInt64)time).ToString());
                            listviewitem.SubItems.Add(name);
                            m_SummaryList.Items.Add(listviewitem);
                        }
                    }  
                }
            }
        }

        public class ListViewColumnSorter : IComparer
        {
            private int ColumnToSort;
            private SortOrder OrderOfSort;
            private CaseInsensitiveComparer ObjectCompare;

            public ListViewColumnSorter()
            {
                // Initialise la colonne sur '0'
                ColumnToSort = 0;

                // Initialise l'ordre de tri sur 'aucun'
                OrderOfSort = SortOrder.None;

                // Initialise l'objet CaseInsensitiveComparer
                ObjectCompare = new CaseInsensitiveComparer();
            }

            public int Compare(object x, object y)
            {
                int compareResult;
                ListViewItem listviewX, listviewY;
                int useColumn = ColumnToSort;

                // Switch to Raw column if using the Time column
                if (useColumn == 1)
                    useColumn = 3;

                listviewX = (ListViewItem)x;
                listviewY = (ListViewItem)y;

                if (useColumn == 0 || useColumn == 4)
                    compareResult = ObjectCompare.Compare(listviewX.SubItems[useColumn].Text, listviewY.SubItems[useColumn].Text);
                else
                {
                    Int64 v1, v2;
                    v1 = Int64.Parse(listviewX.SubItems[useColumn].Text);
                    v2 = Int64.Parse(listviewY.SubItems[useColumn].Text);

                    if (v1 < v2)
                    {
                        compareResult = -1;
                    }
                    else
                    if (v1 > v2)
                    {
                        compareResult = 1;
                    }
                    else
                    {
                        compareResult = 0;
                    }
                }

                if (OrderOfSort == SortOrder.Ascending)
                {
                    return compareResult;
                }
                else if (OrderOfSort == SortOrder.Descending)
                {
                    return (-compareResult);
                }
                else
                {
                    return 0;
                }
            }

            public int SortColumn
            {
                set
                {
                    ColumnToSort = value;
                }
                get
                {
                    return ColumnToSort;
                }
            }

            public SortOrder Order
            {
                set
                {
                    OrderOfSort = value;
                }
                get
                {
                    return OrderOfSort;
                }
            }

        }

        private void Summary_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                Hide();
            }
        }

        private void ThreadSelectionCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            Update(m_Parent);
        }

    }
}