using System;
using System.IO;
using System.Text;
using System.Collections;
using System.Drawing;

// Keep in sync with ProfileEventViewerApp.cs
using Time = System.Double;
using System.Xml;

namespace ProfileEventViewer
{
    public class TimeExtent
    {
        public Time Min;
        public Time Max;

        public TimeExtent()
        {
            Min = Time.MaxValue;
            Max = Time.MinValue;
        }

        public void Merge(TimeExtent other)
        {
            Min = Math.Min(other.Min, Min);
            Max = Math.Max(other.Max, Max);
        }

        public void Merge(Time start, Time end)
        {
            Min = Math.Min(start, Min);
            Max = Math.Max(end, Max);
        }
    }

    struct EventFileHeader
    {
        public uint m_NbThreads;
        public UInt64 m_Frequency;

        public void Serialize(BinaryReader reader, ProfileEventData data)
        {
            m_NbThreads = reader.ReadUInt32();
            m_Frequency = reader.ReadUInt64();
            EventFileThreadDesc threadDesc = new EventFileThreadDesc();

            for (int i = 0; i < m_NbThreads; i++)
            {
                threadDesc.Serialize(reader);

                // register the context
                data.AddContext(threadDesc.m_ThreadID, threadDesc.m_Name);
            }
        }
    }

    struct EventFileBlock
    {
        public uint m_NbProfiles;
        public uint m_ThreadID;
        public uint m_NbEvents;

        public void Serialize(BinaryReader reader, ProfileEventData data)
        {
            m_NbProfiles = reader.ReadUInt32();
            m_ThreadID = reader.ReadUInt32();
            m_NbEvents = reader.ReadUInt32();

            // read all the profiles
            EventFileProfileDesc profileDesc = new EventFileProfileDesc();
            for (int i = 0; i < m_NbProfiles; i++)
            {
                profileDesc.Serialize(reader);
                data.AddProfile(profileDesc.m_ProfileID, profileDesc.m_Name);
            }

            ProfileEventData.ProfileContext eventsContext = data.GetEventContext(m_ThreadID);

            // read all the events
            ProfileEvent profileEvent = new ProfileEvent();
            for (int i = 0; i < m_NbEvents; i++)
            {
                profileEvent.Serialize(reader);

                string name = data.GetProfileName( profileEvent.m_ProfileID);
                eventsContext.AddEvent( profileEvent.m_Time, profileEvent.m_ProfileID, profileEvent.m_Type);
            }

            // Send stop events with the last recorded time
            while( !eventsContext.IsTerminated)
            {
                eventsContext.AddEvent(profileEvent.m_Time, 0, ProfileEventData.ProfileContext.Event.EventStop);
            }
        }
    }

    struct EventFileProfileDesc
    {
        public string m_Name;
        public uint m_ProfileID;
        static ASCIIEncoding asciiEncoder = new ASCIIEncoding();

        public void Serialize(BinaryReader reader)
        {
            byte[] name = reader.ReadBytes(128);

            // decode the name
            m_Name = asciiEncoder.GetString(name);
            m_Name = m_Name.Substring(0, m_Name.IndexOf("\0"));
            m_ProfileID = reader.ReadUInt32();
        }
    }

    struct EventFileThreadDesc
    {
        static ASCIIEncoding asciiEncoder = new ASCIIEncoding();
        public string m_Name;
        public uint m_ThreadID;

        public void Serialize(BinaryReader reader)
        {
            byte[] name = reader.ReadBytes(128);

            // decode the name
            m_Name = asciiEncoder.GetString(name);
            m_Name = m_Name.Substring(0, m_Name.IndexOf("\0"));
            m_ThreadID = reader.ReadUInt32();
        }
    }

    struct ProfileEvent
    {
        public UInt64 m_Time;
        public uint m_ProfileID;
        public Byte m_Type;

        public void Serialize(BinaryReader reader)
        {
            m_Time = reader.ReadUInt64();
            m_ProfileID = reader.ReadUInt32();
            m_Type = reader.ReadByte();
        }
    }

    public class ProfileEventData
    {
        // map from profileID to name
        Hashtable m_Profiles = new Hashtable();

        // map from contextID to it's ProfileContextEvents
        Hashtable m_Contexts = new Hashtable();

        // clock frequency
        UInt64 m_Frequency = 1;

        System.Random rnd = new System.Random(42);

        public class ProfileContext
        {
            ArrayList m_Markers = new ArrayList();
            ArrayList m_Heads = new ArrayList();
            Event m_LastEvent;
            int m_MaxLevel;
            UInt32 m_ContextID;
            string m_Name;

            public class Event
            {
                public const int EventStart = 0;
                public const int EventStop = 1;
                public const int MarkerStart = 2;
                public const int MarkerStop = 3;

                Time m_Time;
                Time m_Length;
                Event m_Child;
                Event m_Sibling;
                Event m_Father;

                UInt32 m_ProfileID;
                Byte m_EventType;
                UInt32 m_Level;

                public Event(Time time, UInt32 profileID, Byte type)
                {
                    // set event data
                    m_Time = time;
                    m_ProfileID = profileID;
                    m_EventType = type;
                }


                public Byte EventType
                {
                    get { return m_EventType; }
                    set { m_EventType = value; }    
                }

                public Event Sibling
                {
                    get { return m_Sibling; }
                    set { m_Sibling = value; }
                }

                public Event Father
                {
                    get { return m_Father; }
                    set { m_Father = value; }
                }

                public Event Child
                {
                    get { return m_Child; }
                    set { m_Child = value; }
                }

                public Time EndTime
                {
                    set { m_Length = (value - m_Time); }
                    get { return m_Time + m_Length; }
                }

                public Time Time
                {
                    get { return m_Time; }
                }

                public Time Length
                {
                    get { return m_Length; }
                }

                public UInt32 Level
                {
                    get { return m_Level; }
                    set { m_Level = value; }
                }

                public UInt32 ProfileID
                {
                    get { return m_ProfileID; }
                }

                public bool Inside(Time min, Time max)
                {
                    if (m_Length == 0)
                        return false;

                    if ((m_Time < max) &&
                        (m_Time + m_Length) > min)
                        return true;

                    return false;
                }
            };

            public ProfileContext(string name, UInt32 contextID)
            {
                m_Name = name;
                m_ContextID = contextID;
            }

            public UInt32 ContextID
            {
                get { return m_ContextID; }
            }

            public string Name
            {
                get { return m_Name; }
            }

            public int MaxLevel
            {
                get { return m_MaxLevel; }
            }

            public ArrayList Heads
            {
                get { return m_Heads; }
            }

            public ArrayList Markers
            {
                get { return m_Markers; }
            }

            public void AddEvent(UInt64 time, UInt32 profileID, Byte type)
            {
                // create the event
                Event profileEvent = null;

                if( type == Event.EventStart || type == Event.MarkerStart)
                {
                    profileEvent = new Event((Time)time, profileID, type);
                }
                else if( type == Event.EventStop)
                {
                    if( m_LastEvent == null)
                        return;
                }

                if( type == Event.EventStart || type == Event.EventStop)
                {
                    if (m_LastEvent == null)
                    {
                        // new head event
                        m_Heads.Add(profileEvent);
                        m_LastEvent = profileEvent;
                    }
                    else
                    {
                        if( type == Event.EventStart)
                        {
                            profileEvent.Father = m_LastEvent;
                            profileEvent.Level = profileEvent.Father.Level + 1;

                            m_MaxLevel = (int)Math.Max( profileEvent.Level, m_MaxLevel);

                            // link to last event
                            if( m_LastEvent.Child == null)
                            {
                                m_LastEvent.Child = profileEvent;
                                m_LastEvent = profileEvent;
                            }
                            else
                            {
                                // find the last sibling
                                Event lastSibling = m_LastEvent.Child;

                                while( lastSibling.Sibling != null)
                                {
                                    lastSibling = lastSibling.Sibling;
                                }

                                lastSibling.Sibling = profileEvent;
                                m_LastEvent = profileEvent;
                            }
                        }
                        else if( type == Event.EventStop)
                        {
                            // close the last event
                            m_LastEvent.EndTime = (Int64)time;
                            m_LastEvent = m_LastEvent.Father;
                        }
                    }
                }
                else if( type == Event.MarkerStart)
                {
                    m_Markers.Add(profileEvent);
                }
                else if( type == Event.MarkerStop)
                {
                    for( int idxMark = m_Markers.Count-1; idxMark >= 0; idxMark--)
                    {
                        Event lastEvent = (Event)m_Markers[idxMark];
                        if( lastEvent.ProfileID == profileID && lastEvent.EventType == Event.MarkerStart)
                        {
                            lastEvent.EndTime = (Int64)time;
                            break;
                        }
                    }
                }
            }

            public TimeExtent TimeExtent
            {
                get
                {
                    TimeExtent extent = new TimeExtent();

                    foreach( Event profileEvent in m_Heads)
                    {
                        if( profileEvent.Time != 0)
                        {
                            extent.Merge(profileEvent.Time, profileEvent.EndTime);
                        }
                    }

                    return extent;
                }
            }

            public bool IsTerminated
            {
                get { return m_LastEvent == null; }
            }
        }

        public ProfileEventData(Stream fromStream)
        {
            BinaryReader reader = new BinaryReader(fromStream);

            // construct data from the file
            EventFileHeader header = new EventFileHeader();
            header.Serialize(reader, this);

            m_Frequency = header.m_Frequency;

            // while the stream isn't all read continue reading Blocks			
            try
            {

                while (fromStream.Length != fromStream.Position)
                {
                    EventFileBlock block = new EventFileBlock();

                    block.Serialize(reader, this);
                }
            }
            catch (Exception /*e*/)
            {
            }
        }

        void SetFrequency(UInt64 frequency)
        {
            m_Frequency = frequency;
        }

        public UInt64 GetFrequency()
        {
            return m_Frequency;
        }

        struct ProfileDesc
        {
            public string Name;
            public Color Color;
        };

        public void AddProfile(UInt32 profileID, string name)
        {
            if (!m_Profiles.Contains(profileID))
            {
                ProfileDesc desc = new ProfileDesc();
                desc.Name = name;

                desc.Color = Color.FromArgb(141 + (((int)profileID & 0xff) / 3), 140 + ((((int)profileID & 0xff00) >> 8) / 3), 140 + ((((int)profileID & 0xff0000) >> 16) / 3));
                m_Profiles[profileID] = desc;
            } 
        }

        public string GetProfileName(UInt32 profileID)
        {
            if (m_Profiles.Contains(profileID))
            {
                return ((ProfileDesc)m_Profiles[profileID]).Name;
            }

            return "Unknown profile";
        }

        public Color GetProfileColor(UInt32 profileID)
        {
            if (m_Profiles.Contains(profileID))
            {
                return ((ProfileDesc)m_Profiles[profileID]).Color;
            }

            return Color.DarkGray;
        }

        public void AddContext(UInt32 contextID, string name)
        {
            if (!m_Contexts.Contains(contextID))
            {
                ProfileContext context = new ProfileContext(name, contextID);

                m_Contexts[contextID] = context;
            }
        }

        public ProfileContext GetEventContext(UInt32 contextID)
        {
            return (ProfileContext)m_Contexts[contextID];
        }

        public TimeExtent TimeExtent
        {
            get
            {
                TimeExtent extent = new TimeExtent();

                foreach (ProfileContext context in m_Contexts.Values)
                {
                    extent.Merge(context.TimeExtent);
                }

                return extent;
            }
        }

        public Hashtable Contexts
        {
            get { return m_Contexts; }
        }

        public Hashtable Profiles
        {
            get { return m_Profiles; }
        }
    }

}
