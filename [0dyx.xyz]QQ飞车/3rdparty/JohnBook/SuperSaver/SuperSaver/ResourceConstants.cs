/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997 - 2003 John Robbins
----------------------------------------------------------------------*/
using System ;
namespace SuperSaver
{

    // The wrapper class for internationalization.  I haven't gotten
    // it done yet, but this way I only have to update this class to
    // make it happen.
    class ResConstant
    {
        // The command bar constants.
        public static String CommandBarName = "SuperSaver" ;
    
        // All the constants for the SuperSaverSaveAll
        // command.
        public static String SuperSaverSaveAll =
            "SuperSaverSaveAll" ;
        public static String SuperSaverSaveAllButtonText =
            "SuperSaver Save All" ;
        public static String SuperSaverSaveAllToolTip =
            "SuperSaver Save All (strips whitespace)" ;
        public static int    SuperSaverSaveAllBitmapID = 2 ;
        // The full command name.
        public static String SuperSaverSaveAllFullCommand =
            "SuperSaver.SuperSaverSaveAll" ;
            
        // The constants for the SuperSaverSave command.
        public static String SuperSaverSave = "SuperSaverSave" ;
        public static String SuperSaverSaveButtonText =
                                              "SuperSaver Save" ;
        public static String SuperSaverSaveToolTip =
                                              "SuperSaver Save" ;
        public static int SuperSaverSaveBitmapID = 1 ;
        public static String SuperSaverSaveFullCommand =
                                    "SuperSaver.SuperSaverSave" ;
                                    
        public static String VirginSaveFormat =
                        "All files (*.*)|*.*|{0} files (*.{0})|*.{0}"  ;
        public static String VirginDialogTitle = "Save As" ;
        
        public static String WindowMenuName = "Window" ;
        public static String ActiveWindowMenu = "&1 " ;
                                                    
    }
}