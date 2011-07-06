using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ProfileEventViewer
{
    public partial class BookmarksList : Form
    {
        public ProfileEventsViewer m_parent;
        private ProfileEventViewer.Summary.ListViewColumnSorter lvwColumnSorter;
        public Dictionary<ListViewItem, ProfileEventData.ProfileContext.Event> m_eventDictionnary;

        public BookmarksList(ProfileEventsViewer parent)
        {
            InitializeComponent();
            m_eventDictionnary = new Dictionary<ListViewItem, ProfileEventData.ProfileContext.Event>();
            lvwColumnSorter = new ProfileEventViewer.Summary.ListViewColumnSorter();
            m_BookmarksList.ListViewItemSorter = lvwColumnSorter;
            m_parent = parent;
        }

        public void PostShow(ProfileEventsViewer pev)
        {
            Update(pev);
        }

        public void Update(ProfileEventsViewer pev)
        {
            ListViewItem listviewitem;

            m_parent.SetActiveMarker(null);
            m_BookmarksList.View = View.Details;
            m_BookmarksList.Items.Clear();
            m_eventDictionnary.Clear();

            if (pev.Data == null || pev.Data.Contexts.Count == 0)
                return;

            foreach (ProfileEventData.ProfileContext context in pev.Data.Contexts.Values)
            {
                if (context.Markers.Count == 0)
                    continue;

                foreach (ProfileEventData.ProfileContext.Event profileEvent in context.Markers)
                {
                    listviewitem = new ListViewItem(pev.Data.GetProfileName(profileEvent.ProfileID));
                    listviewitem.SubItems.Add(pev.TimeToString((UInt64)profileEvent.Length));
                    listviewitem.SubItems.Add(((UInt64)0).ToString());
                    listviewitem.SubItems.Add(((UInt64)profileEvent.Length).ToString());
                    m_BookmarksList.Items.Add(listviewitem);

                    m_eventDictionnary.Add(listviewitem, profileEvent);
                }
            }
        }

        private void BookmarksList_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                m_parent.SetActiveMarker(null);
                e.Cancel = true;
                Hide();
            }
        }

        private void m_BookmarksList_ColumnClick(object sender, ColumnClickEventArgs e)
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

            m_BookmarksList.Sort();
        }

        private void m_BookmarksList_ItemActivate(object sender, EventArgs e)
        {
            ListViewItem item = m_BookmarksList.SelectedItems[0];
            m_parent.SetActiveMarker(m_eventDictionnary[item]);
        }
    }
}