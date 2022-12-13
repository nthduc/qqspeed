/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

using System ;
using System.Data ;
using System.Data.SqlClient ;
using System.Text ;
using System.Diagnostics ;


namespace Wintellect
{
    /// <summary>
    /// A simple wrapper class around the AssertTableExists method.
    /// </summary>
    class TableExists
    {
        /// <summary>
        /// Tests to see if the specified table name exists.
        /// </summary>
        [Conditional("DEBUG")]
        static public void AssertTableExists ( string ConnStr ,
                                               string TableName )
        {
            SqlConnection Conn = new SqlConnection ( ConnStr ) ;

            StringBuilder sBuildCmd = new StringBuilder ( ) ;

            sBuildCmd.Append ( "select * from dbo.sysobjects where " );
            sBuildCmd.Append ( "id = object_id('" ) ;
            sBuildCmd.Append ( TableName ) ;
            sBuildCmd.Append ( "')" ) ;

            // Make the command.
            SqlCommand Cmd = new SqlCommand ( sBuildCmd.ToString ( ) ,
                                              Conn                    ) ;

            try
            {
                // Open the database.
                Conn.Open ( ) ;

                // Create a dataset to fill.
                DataSet TableSet = new DataSet ( ) ;

                // Create the data adapter.
                SqlDataAdapter TableDataAdapter = new SqlDataAdapter();

                // Set the command to do the select.
                TableDataAdapter.SelectCommand = Cmd ;

                // Fill the dataset from the adapter.
                TableDataAdapter.Fill ( TableSet ) ;

                // If anything showed up, the table exists.
                if ( 0 == TableSet.Tables[0].Rows.Count )
                {
                    String sMsg = "Table : '" + TableName +
                                    "' does not exist!\r\n" ;

                    Debug.Assert ( false , sMsg ) ;
                }
            }
            catch ( Exception e )
            {
                Debug.Assert ( false , e.Message ) ;
            }
            finally
            {
                Conn.Close ( ) ;
            }
        }
    }
	/// <summary>
	/// The main startup class.
	/// </summary>
	class DaMain
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static void Main(string[] args)
		{
		    if ( 2 != args.Length )
		    {
		        Console.WriteLine ( "TableExists <full connection string in quotes> <table>" ) ;
		    }
		    else
		    {
		        TableExists.AssertTableExists ( args[0] , args[1] ) ;
		    }
		}
	}
}
