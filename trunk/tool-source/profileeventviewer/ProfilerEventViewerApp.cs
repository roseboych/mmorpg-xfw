using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Data;
using System.IO;

// Keep in sync with ProfileEvent.cs
using Time = System.Double;
using ProfileEventViewer.Properties;
using System.Threading;

namespace ProfileEventViewer
{
	public class ProfileEventsViewer : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.MenuItem menuItemFileMenu;
        private System.Windows.Forms.MenuItem menuItemOpenEventLog;
        private IContainer components;
        private MenuItem menuItemSummary;
        private String openFileName;
        private MenuItem menuItemBookmarks;
        private ProfileEventViewer.ProfileEventData.ProfileContext.Event m_CurrentMarker;
        private ToolStrip toolStrip1;
        private ToolStripButton toolStripButton2;

		ProfileEventData		m_Data;

        public ProfileEventData Data
        {
            get { return m_Data; }
        }

        public void SetActiveMarker(ProfileEventViewer.ProfileEventData.ProfileContext.Event profileEvent)
        {
            m_CurrentMarker = profileEvent;
            Invalidate();
        }

		private void Init()
        {
            InitializeComponent();
            m_Summary = new Summary(this);
            m_Bookmarks = new BookmarksList(this);
        }
	
		public ProfileEventsViewer()
		{
            Init();
		}

        public ProfileEventsViewer(string initFileName)
        {
            Init();
            OpenFile(initFileName);
        }
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProfileEventsViewer));
            this.mainMenu = new System.Windows.Forms.MainMenu(this.components);
            this.menuItemFileMenu = new System.Windows.Forms.MenuItem();
            this.menuItemOpenEventLog = new System.Windows.Forms.MenuItem();
            this.menuItemSummary = new System.Windows.Forms.MenuItem();
            this.menuItemBookmarks = new System.Windows.Forms.MenuItem();

            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripButton2 = new System.Windows.Forms.ToolStripButton();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            
            // 
            // mainMenu
            // 
            this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemFileMenu,
            this.menuItemSummary,
            this.menuItemBookmarks});
            // 
            // menuItemFileMenu
            // 
            this.menuItemFileMenu.Index = 0;
            this.menuItemFileMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItemOpenEventLog});
            this.menuItemFileMenu.Text = "&File";
            // 
            // menuItemOpenEventLog
            // 
            this.menuItemOpenEventLog.Index = 0;
            this.menuItemOpenEventLog.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
            this.menuItemOpenEventLog.Text = "&Open Event Log";
            this.menuItemOpenEventLog.Click += new System.EventHandler(this.menuItemOpenEventLog_Click);
            // 
            // menuItemSummary
            // 
            this.menuItemSummary.Index = 1;
            this.menuItemSummary.Text = "Show Summary";
            this.menuItemSummary.Click += new System.EventHandler(this.menuItemSummary_Click);
            // 
            // menuItemBookmarks
            // 
            this.menuItemBookmarks.Index = 2;
            this.menuItemBookmarks.Text = "Bookmarks";
            this.menuItemBookmarks.Click += new System.EventHandler(this.menuItemBookmarks_Click);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripButton2});
            this.toolStrip1.Location = new System.Drawing.Point(0, 0);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.toolStrip1.Size = new System.Drawing.Size(35, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            this.toolStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menuItemReload_Click);
            // 
            // toolStripButton2
            // 
            this.toolStripButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButton2.Image")));
            this.toolStripButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButton2.Name = "toolStripButton2";
            this.toolStripButton2.Size = new System.Drawing.Size(23, 22);
            this.toolStripButton2.Text = "Reload file";
            // 
            // ProfileEventsViewer
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(752, 337);
            this.Controls.Add(this.toolStrip1);
            this.Menu = this.mainMenu;
            this.Name = "ProfileEventsViewer";
            this.Text = "Profile Event Viewer";
            this.Load += new System.EventHandler(this.ProfileEventsViewer_Load);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ProfileEventsViewer_MouseUp);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ProfileEventsViewer_Paint);
            this.MouseEnter += new System.EventHandler(this.ProfileEventsViewer_MouseEnter);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ProfileEventsViewer_MouseDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.ProfileEventsViewer_KeyUp);
            this.Resize += new System.EventHandler(this.ProfileEventsViewer_Resize);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ProfileEventsViewer_MouseMove);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ProfileEventsViewer_KeyDown);
            this.MouseHover += new System.EventHandler(this.ProfileEventsViewer_MouseHover);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		[STAThread]
        static void Main(string[] _args) 
		{
            if (_args.GetLength(0) > 0)
            {
                Application.Run(new ProfileEventsViewer(_args[0]));
            }
            else
            {
                Application.Run(new ProfileEventsViewer());
            }
		}

		private void menuItemOpenEventLog_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog fileDialog  = new OpenFileDialog();
            fileDialog.CheckFileExists = true;
            fileDialog.CheckPathExists = true;
            fileDialog.DereferenceLinks = true;
            fileDialog.DefaultExt = "pel";

            // The Filter property requires a search string after the pipe ( | )
            fileDialog.Filter = "Profile event log (*.pel)|*.pel";
            fileDialog.Multiselect = false;
            fileDialog.Title = "Select the Profile Event Log file";
            fileDialog.ValidateNames = true;

			if (fileDialog.ShowDialog() == DialogResult.OK)
			{
				// get the file name and open a stream
				OpenFile(fileDialog.FileName);
			}
		}

        private void OpenFile(string fileName)
        {
            openFileName = fileName;

            Stream stream;
            try
            {
                // get the file name and open a stream
                stream = new FileStream(fileName, FileMode.Open);
            }
            catch
            {
                String errorMsg = "Cannot open " + fileName;
                System.Windows.Forms.MessageBox.Show(errorMsg, "Error!");
                openFileName = null;
                return;
            }
            
			if (stream != null)
			{
				m_Data = new ProfileEventData( stream);	

				// set the time start to the extent
				TimeExtent extent = m_Data.TimeExtent;

				m_TimeStart = (Time)extent.Min;
				m_TimeEnd = (Time)extent.Max;

                this.Text = "Profile Event Viewer - " + fileName;

				Invalidate();
                stream.Close();
            }

            m_Summary.PostShow(this);
            m_Bookmarks.PostShow(this);
        }

        Time m_TimeStart;
        Time m_TimeEnd;
        Int64   m_YOffset;

        Time m_RangeStart;
        Time m_RangeEnd;
        Time m_FocusTime;

        public struct SelectionInfoContext
        {
            public Time m_TotalTime;
            public UInt32 m_Calls;
            public SelectionInfoContext(Time initTime, UInt32 calls)
            {
                m_TotalTime = initTime;
                m_Calls = calls;
            }
        }

        public struct SelectionInfo
        {
            public Hashtable m_context;
            public SelectionInfo(Time initTime, UInt32 calls,String name)
            {
                m_context = new Hashtable();
                m_context.Add(name, new SelectionInfoContext(initTime, calls));
            }
        };
        
        Hashtable m_SelectionTimes = new Hashtable();

        public Hashtable SelectionTimes
        {
            get { return m_SelectionTimes; }
        }


		enum MouseMode
		{
			None,
			Pan,
			Zoom,
			ZoomOnLocation,
			MarkRange			
		};		

		bool shiftIsDown;
		bool ctrlIsDown;

		int		m_LastX;
		int		m_LastY;	
	
        ProfileEventData.ProfileContext.Event   m_eventSelection;

		MouseMode m_MouseMode = MouseMode.None;

        const int   MarkerHeight = 4;
		const int	EventHeight = 24;
		const int   TitleHeight = 30;

        struct OverDraw
        {
            public Int64 m_LastColumn;
            public Int64 Reset(Int64 newVal)
            { 
                Int64 oldColumn = m_LastColumn;
                m_LastColumn = newVal;
                return oldColumn;
            }
            public Int64 Reset() { return Reset(-1); }
            public bool ShouldDraw(Int64 column) 
            {
                if (column == m_LastColumn)
                    return false;

                m_LastColumn = column; 
                return true; 
            }
        };

		struct DrawEventContext
		{
            public int m_Y;
			public Time	m_TimeStepPerPixel;
			public Graphics	Graphics;
            public OverDraw overDraw;
		};

		Pen		rectPen = new Pen(Color.Black, 1);
        Pen     rectPenSelected = new Pen(Color.Red, 3);
        SolidBrush rectBrush = new SolidBrush(Color.DarkGray);
        Brush titleBrush1 = new SolidBrush(Color.LightGray);
		Brush	titleBrush2 = new SolidBrush(Color.Gray);
		Font	barFont = new Font("Arial", 8);
		Font	contextTitleFont = new Font("Arial Black", 16);
		Brush	fontBrush = new SolidBrush(Color.Black);
        Brush   selfBrush = new HatchBrush(HatchStyle.LightUpwardDiagonal, Color.FromArgb(80, 200, 200, 200), Color.FromArgb(80, 0, 0, 0));
        Brush   rangeBrush = new SolidBrush(Color.FromArgb(64, 64, 64, 64));
         
		int		m_LastReceivedX;
		int		m_LastReceivedY;
          
        Summary m_Summary;
        BookmarksList m_Bookmarks;

		private  Time	ConvertXToTime(int x)
		{
            Time timeStepPerPixel = TimeStepPerPixel();

			return (Time)((x * timeStepPerPixel) + m_TimeStart);
		}

		private Int64 ConvertTimeToX(Time time)
		{
            Time timeStepPerPixel = TimeStepPerPixel();

			return (Int64) ( (time - m_TimeStart) / timeStepPerPixel );
		}

		private void ProfileEventsViewer_MouseHover(object sender, System.EventArgs e)
		{
			if (m_Data != null)
			{
				//ProfileEventData.ProfileContext.Event profileEvent = GetEventFromMousePosition(m_LastReceivedX, m_LastReceivedY);

				// draw a tooltip
			}
		}

		private void ProfileEventsViewer_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			m_LastX = e.X;
			m_LastY = e.Y;

			if (e.Button == MouseButtons.Left)
			{
				if (shiftIsDown)
				{
					m_MouseMode = MouseMode.Pan;
					Capture = true;
				}
				else if (ctrlIsDown)
				{
					m_FocusTime = ConvertXToTime(e.X);
					m_MouseMode = MouseMode.ZoomOnLocation;
					Capture = true;
				}
                else
                {
                    if ( openFileName != null )
                    {
                        m_eventSelection = GetEventFromMousePosition(e.X, e.Y);
                        Invalidate();
                    }
                }
			}

			if (e.Button == MouseButtons.Right)
			{
				Capture = true;
				m_MouseMode = MouseMode.MarkRange;
				m_RangeStart = ConvertXToTime(e.X);
				m_RangeEnd = m_RangeStart;
				m_FocusTime = m_RangeStart;
			}
		}

		private void ProfileEventsViewer_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			m_MouseMode = MouseMode.None;
			Capture = false;
		}

        private Time TimeStepPerPixel()
        {
            Int64 width = ClientSize.Width;
            return (Time) ((m_TimeEnd - m_TimeStart) / (double)width);
        }

		private void ProfileEventsViewer_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			m_LastReceivedX = e.X;
			m_LastReceivedY = e.Y;

			if (m_MouseMode == MouseMode.None)
				return;

			Int64 dx = e.X - m_LastX;
			Int64 dy = e.Y - m_LastY;

			// return if nothing to do
			if (dx == 0 && dy == 0)
				return;


			// get window width
            Time timeStepPerPixel = TimeStepPerPixel();
            Time timeDx = (timeStepPerPixel * dx);
			switch (m_MouseMode)
			{
				case MouseMode.Pan:
                    m_TimeStart -= timeDx;
                    m_TimeEnd -= timeDx;
                    m_YOffset += dy;
					break;
				case MouseMode.Zoom:
                    m_TimeStart += timeDx;
                    m_TimeEnd -= timeDx;
					break;
				case MouseMode.ZoomOnLocation:
					// where removing dx * timeStepPerPixel * 2 units from the selection
					// split it arround the focus so that the focus location on screen remains
					// constant
                    Time timeToRemove = timeDx * 2;

                    // Avoid Too Much zoom in one mouse move (or else start will get after end)
                    if (timeToRemove > 0.98 * (m_TimeEnd - m_TimeStart))
                        timeToRemove = 0.98 * (m_TimeEnd - m_TimeStart);

                    double ratio = (double)(m_TimeEnd - m_FocusTime) / (double)(m_TimeEnd - m_TimeStart);
                    

                    m_TimeStart += (Time)(timeToRemove * (1 - ratio));
                    m_TimeEnd -= (Time)(timeToRemove * ratio);
					break;
				case MouseMode.MarkRange:
                    Time currentSelectedTime = ConvertXToTime(e.X);
					if (currentSelectedTime < m_FocusTime)
					{
						m_RangeEnd = m_FocusTime;
						m_RangeStart = currentSelectedTime;
					}
					else
					{
						m_RangeStart = m_FocusTime;
						m_RangeEnd = currentSelectedTime;
					}
                    if(m_Summary.Visible)
                        ComputeSelectionTimes();
					break;
			}

			//Console.WriteLine("DX {0}, width {1}, timeStep {2}, {3}-{4}", dx, width, timeStepPerPixel, m_TimeStart, m_TimeEnd);				

			m_LastX = e.X;
			m_LastY = e.Y;		
	
			Invalidate();
		}

		public string TimeToString(UInt64 length)
		{
			UInt64 frequency =  m_Data.GetFrequency();
			UInt64 frequencyInUS = frequency / 1000000;			
			
			UInt64 lengthInUS = length / frequencyInUS;

			if (lengthInUS <= 0)
			{
				return String.Format("{0} cycles", length);
			}
			else if (lengthInUS < 500)
			{
				return String.Format("{0} us", lengthInUS);
			}
			else if (lengthInUS < 1000000)
			{
				return String.Format("{0:F2} ms", ((double)lengthInUS/1000));
			}
            else if (lengthInUS < (1000000*60))
			{
				return String.Format("{0:F2} s", ((double)lengthInUS/1000000));
			}
			else
			{
				return String.Format("{0:F2} m", ((double)lengthInUS/(1000000*60)));
			}
		}

		private void DrawEvent(ProfileEventData.ProfileContext.Event profileEvent, ref DrawEventContext drawContext)
		{
            Pen     activeRectPen = rectPen;
            float rectOffset = 0;
            if (profileEvent == m_eventSelection)
            {
                activeRectPen = rectPenSelected;
                rectOffset = 1.5f;
            }

            if (profileEvent.Inside(m_TimeStart, m_TimeEnd))
            {
                Int64 screenXEnd = (Int64)((profileEvent.Time + profileEvent.Length - m_TimeStart) / drawContext.m_TimeStepPerPixel);
                if (!drawContext.overDraw.ShouldDraw(screenXEnd))
                    return;

                // compute screen X size
                Int64 screenX = (Int64)((profileEvent.Time - m_TimeStart) / drawContext.m_TimeStepPerPixel);
                Int64 screenLength = (Int64)((profileEvent.Length) / drawContext.m_TimeStepPerPixel);
                Int64 screenXE = screenX + screenLength;
                bool nulSized = (screenLength == 0);


                // draw a rectangle
                rectBrush.Color = m_Data.GetProfileColor(profileEvent.ProfileID);
                screenX = Math.Max(-20, screenX);
                screenXE = Math.Min(ClientSize.Width + 20, screenXE);
                if (screenXE == screenX)
                    screenXE++;
                screenLength = screenXE - screenX;

                int offsetY = 0;
                if (profileEvent.EventType == ProfileEventViewer.ProfileEventData.ProfileContext.Event.EventStart)
                {
                    drawContext.Graphics.FillRectangle(rectBrush, screenX, drawContext.m_Y, screenLength, EventHeight);
                    drawContext.Graphics.DrawRectangle(activeRectPen, screenX + rectOffset, drawContext.m_Y + rectOffset, screenLength - (rectOffset*2), EventHeight - (rectOffset*2));
                }
                else
                {

                    //drawContext.Graphics.FillRectangle(rectBrush, screenX, drawContext.m_Y, screenLength, MarkerHeight);
                    offsetY = -2*EventHeight;
                    
                    rectBrush.Color = Color.Red;
                    drawContext.Graphics.FillRectangle(rectBrush, screenX, drawContext.m_Y + offsetY, screenLength, EventHeight);
                    drawContext.Graphics.DrawRectangle(activeRectPen, screenX + rectOffset, drawContext.m_Y + offsetY + rectOffset, screenLength - (rectOffset*2), EventHeight - (rectOffset * 2));
                }
                

                // set the clip region and display the profile name (if there's any chance it could fit)
                if ((screenLength > 20) || (profileEvent == m_eventSelection))
                {
                    Region oldClipRegion = drawContext.Graphics.Clip;
                    drawContext.Graphics.SetClip(new Rectangle((int)screenX + 1, drawContext.m_Y + offsetY, (int)screenLength - 1, EventHeight));

                    string profileName = String.Format("{0}{1}", (screenX < 0) ? "<< " : "", m_Data.GetProfileName(profileEvent.ProfileID));
                    string profileTime = String.Format("{0}{1}", (screenX < 0) ? "<< " : "", TimeToString((UInt64)profileEvent.Length));
                    drawContext.Graphics.DrawString(profileName, barFont, fontBrush, Math.Max(screenX + 1, 0), drawContext.m_Y - 0 + offsetY);
                    drawContext.Graphics.DrawString(profileTime, barFont, fontBrush, Math.Max(screenX + 1, 0), drawContext.m_Y + 10 + offsetY);

                    drawContext.Graphics.Clip = oldClipRegion;
                }

                // must draw the childs (if we were not too small)
                if (profileEvent.Child != null && screenLength>1)
                { 
                    drawContext.Graphics.FillRectangle(selfBrush, screenX, drawContext.m_Y + EventHeight, screenLength, EventHeight);

                    drawContext.m_Y += EventHeight;
                    Int64 keepColumn = drawContext.overDraw.Reset();

                    ProfileEventData.ProfileContext.Event child = profileEvent.Child;

                    while (child != null)
                    {
                        DrawEvent(child, ref drawContext);
                        child = child.Sibling;
                    }

                    drawContext.overDraw.Reset(keepColumn);

                    drawContext.m_Y -= EventHeight;
                }
            }
		}

		// Create a compatible Graphics
		Bitmap		m_OffScreenBitmap = null;
		Graphics	m_OffScreenGraphics = null;

		void UpdateOffscreenSurface()
		{
			if (m_OffScreenBitmap != null)
				m_OffScreenBitmap.Dispose();

			if (m_OffScreenGraphics != null)
				m_OffScreenGraphics.Dispose();

            if (ClientSize.Width != 0 && ClientSize.Height != 0)
            {
                m_OffScreenBitmap = new Bitmap(ClientSize.Width, ClientSize.Height);
                m_OffScreenGraphics = Graphics.FromImage(m_OffScreenBitmap);
            }
		}

		protected override void OnPaintBackground(PaintEventArgs pevent)
		{
			// do not paint background once we've loaded data
			if (m_Data == null)
			{
				base.OnPaintBackground (pevent);
			}
		}

		struct FindEventContext
		{
            public Time Time;			
			public int		TargetDepth;
		}

		private ProfileEventData.ProfileContext.Event GetEventFromMousePositionInContext(FindEventContext findContext, ProfileEventData.ProfileContext.Event profileEvent)
		{			
			if (findContext.Time >= profileEvent.Time &&
				findContext.Time <= profileEvent.EndTime )
			{
				// could be in this one, if the depth is good
				if (findContext.TargetDepth == profileEvent.Level)
				{
					return profileEvent;
				}
				
				// depth isn't correct, search the childs 
				if (profileEvent.Child != null)
				{
					ProfileEventData.ProfileContext.Event child = profileEvent.Child;
					
					while (child != null)
					{
						profileEvent = GetEventFromMousePositionInContext(findContext, child);

						if (profileEvent != null)
							return profileEvent;

						child = child.Sibling;
					}					
				}
			}

			return null;
		}

		private ProfileEventData.ProfileContext.Event GetEventFromMousePosition(int x, int y)
		{
			if (x < 0 || x > ClientSize.Width)
				return null;

            int contextmaxy = (int)m_YOffset;
            int contextStart = (int)m_YOffset;
			
			FindEventContext findContext = new FindEventContext();

			int width = ClientSize.Width;

            findContext.Time = ConvertXToTime(x);	

			// find which context it's in
			foreach (ProfileEventData.ProfileContext context in m_Data.Contexts.Values)
			{
				if (context.Heads.Count == 0)
					continue;

				contextmaxy += TitleHeight;
				contextmaxy += ((context.MaxLevel+1) * EventHeight);

				if (y < contextmaxy)
				{
					findContext.TargetDepth = ((y - contextStart) - TitleHeight) / (EventHeight);

					foreach (ProfileEventData.ProfileContext.Event profileEvent in context.Heads)
					{
						ProfileEventData.ProfileContext.Event foundEvent = GetEventFromMousePositionInContext(findContext, profileEvent);

						if(foundEvent != null)
							return foundEvent;
					}
				}	
				
				contextStart = contextmaxy;
			}

			return null;
		}

        private void ComputeContextSelectionTimes(ProfileEventData.ProfileContext context, ProfileEventData.ProfileContext.Event profileEvent)
        {
            if (profileEvent.Inside(m_RangeStart, m_RangeEnd))
            {
                // Only consider the included time
                Time start = profileEvent.Time;
                Time end = profileEvent.Time + profileEvent.Length;

                if (start < m_RangeStart)
                    start = m_RangeStart;
                if (end > m_RangeEnd)
                    end = m_RangeEnd;

                // Add 
                // profileEvent.m_ProfileID += end-start;
                SelectionInfo oldVal = (SelectionInfo)m_SelectionTimes[profileEvent.ProfileID]; 
                if (oldVal.m_context.ContainsKey(context.Name) == false)
                {
                    oldVal.m_context.Add(context.Name,new SelectionInfoContext((end-start),1));
                }
                else
                {
                    SelectionInfoContext selContext;
                    selContext = (SelectionInfoContext)oldVal.m_context[context.Name];
                    selContext.m_TotalTime += (end - start);
                    selContext.m_Calls++;
                    oldVal.m_context[context.Name] = selContext;
                }

                m_SelectionTimes[profileEvent.ProfileID] = oldVal;
                // Childs
                if (profileEvent.Child != null)
                {
                    ProfileEventData.ProfileContext.Event child = profileEvent.Child;

                    while (child != null)
                    {
                        ComputeContextSelectionTimes(context, child);
                        child = child.Sibling;
                    }
                }
            }
        }

        private void ComputeSelectionTimes()
        {
            // Clear the times
            m_SelectionTimes.Clear();
            
            // Do nothing if no data
            if (m_Data == null)
                return;
            
            // Pre-enter all the profile ids
            foreach (UInt32 id in m_Data.Profiles.Keys)
                m_SelectionTimes.Add(id, new SelectionInfo((Time)0, 0,""));


            foreach (ProfileEventData.ProfileContext context in m_Data.Contexts.Values)
            {
                foreach (ProfileEventData.ProfileContext.Event profileEvent in context.Heads)
                {
                    ComputeContextSelectionTimes(context, profileEvent);
                }
            }

            // update dialog
            m_Summary.Update(this);
        }

		private void ProfileEventsViewer_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
            if (m_Data == null || ClientSize.Width == 0 || ClientSize.Height == 0)
				return;					

			if (m_OffScreenBitmap == null)
				UpdateOffscreenSurface();

			Int64 width = ClientSize.Width;
            Time timeStepPerPixel = TimeStepPerPixel();

			DrawEventContext drawContext = new DrawEventContext();
			drawContext.Graphics= m_OffScreenGraphics;			
			drawContext.m_TimeStepPerPixel = timeStepPerPixel;

			drawContext.Graphics.Clear(Color.LightGray);
            drawContext.m_Y = (Int32) m_YOffset;

			int contextIndex = 0;

			// draw the thing
			foreach (ProfileEventData.ProfileContext context in m_Data.Contexts.Values)
			{
				if (context.Heads.Count == 0)
					continue;

				Brush titleBrush;

				// Print context name
				if ((contextIndex & 1) == 0)
				{
					titleBrush = titleBrush1;
				}
				else
				{
					titleBrush = titleBrush2;
				}

				drawContext.Graphics.FillRectangle(titleBrush, 0, drawContext.m_Y, width, ((context.MaxLevel+1) * EventHeight) + TitleHeight);
				drawContext.Graphics.DrawString(context.Name, contextTitleFont, fontBrush, 0, drawContext.m_Y);

				drawContext.m_Y += TitleHeight;
                drawContext.overDraw.Reset();

				foreach (ProfileEventData.ProfileContext.Event profileEvent in context.Heads)
				{
					DrawEvent(profileEvent, ref drawContext);
				}

                drawContext.m_Y += ((context.MaxLevel + 1) * EventHeight);
                contextIndex++;

                if (m_CurrentMarker!=null)
                    DrawEvent(m_CurrentMarker, ref drawContext);
			}

			// draw the selection range			
			Int64 rangeX = ConvertTimeToX(m_RangeStart);
			Int64 rangeXE = ConvertTimeToX(m_RangeEnd);
			if (rangeXE == rangeX)
				rangeXE++;
			Int64 rangeWidth = rangeXE - rangeX;

			// check if the range is visilbe
			if ( (rangeX < ClientSize.Width) && 
				(rangeXE > 0) && rangeWidth != 0)
			{
				// display the selection range extent 
				drawContext.Graphics.FillRectangle(rangeBrush, rangeX, 0, rangeWidth, ClientSize.Height);

				// display range length
				drawContext.Graphics.DrawString(TimeToString((UInt64)( m_RangeEnd - m_RangeStart)), barFont , fontBrush, rangeX, ClientSize.Height - EventHeight);
			}
	
			
			e.Graphics.DrawImage(m_OffScreenBitmap, 0, 0);
		}

		private void ProfileEventsViewer_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			shiftIsDown = e.Shift;
			ctrlIsDown = e.Control;
		}

		private void ProfileEventsViewer_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			shiftIsDown = e.Shift;
			ctrlIsDown = e.Control;
		}

		private void ProfileEventsViewer_MouseEnter(object sender, System.EventArgs e)
		{
			if (!Capture)
			{
				shiftIsDown = false;
				ctrlIsDown = false;
				m_MouseMode = MouseMode.None;
			}

			m_LastX = 0;
			m_LastY = 0;
		}

		private void ProfileEventsViewer_Resize(object sender, System.EventArgs e)
		{
            if (m_Data == null || ClientSize.Width == 0 || ClientSize.Height == 0)
                return;					

			UpdateOffscreenSurface();
			Invalidate();
			Update();
		}

        private void menuItemSummary_Click(object sender, EventArgs e)
        {
            if (!m_Summary.Visible)
            {
                m_Summary.Show();
                m_Summary.PostShow(this);
                ComputeSelectionTimes();
            }
        }

        private void menuItemBookmarks_Click(object sender, EventArgs e)
        {
            if (!m_Bookmarks.Visible)
            {
                m_Bookmarks.Show();
                m_Bookmarks.PostShow(this);
            }
        }

        private void ProfileEventsViewer_Load(object sender, EventArgs e)
        {
        }

        private void menuItemReload_Click(object sender, ToolStripItemClickedEventArgs e)
        {
            if (openFileName != null)
            {
                OpenFile(openFileName);
            }
        }
	}
}
