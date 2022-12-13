/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Diagnostics ;

namespace TraceSwitchy
{
    class DaApp
    {
        public static void Main ( )
        {
            TraceSwitch TheSwitch = new TraceSwitch ( "SwitchyTheSwitch",
                                                      "Example Switch"  );
            Trace.WriteLineIf ( TheSwitch.TraceError ,
                                "SwitchyTheSwitch: Error tracing is on!" ) ;
            Trace.WriteLineIf ( TheSwitch.TraceWarning ,
                                "SwitchyTheSwitch: Warning tracing is on!" ) ;
            Trace.WriteLineIf ( TheSwitch.TraceInfo ,
                                "SwitchyTheSwitch: Info tracing is on!" ) ;
            Trace.WriteLineIf ( TheSwitch.TraceVerbose ,
                                "SwitchyTheSwitch: VerboseSwitching is on!" ) ;


            TraceSwitch TheSwitch2 = new TraceSwitch ( "TraceSwitchy.DaApp",
                                                       "Example Switch"  );
            Trace.WriteLineIf ( TheSwitch2.TraceError ,
                                "TraceSwitchy.DaApp: Error tracing is on!" ) ;
            Trace.WriteLineIf ( TheSwitch2.TraceWarning ,
                                "TraceSwitchy.DaApp: Warning tracing is on!" ) ;
            Trace.WriteLineIf ( TheSwitch2.TraceInfo ,
                                "TraceSwitchy.DaApp: Info tracing is on!" ) ;
            Trace.WriteLineIf ( TheSwitch2.TraceVerbose ,
                                "TraceSwitchy.DaApp: VerboseSwitching is on!" ) ;
        }
    }
}