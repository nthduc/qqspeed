/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
using System ;
using System.Drawing ;
using System.Collections ;
using System.ComponentModel ;
using System.Windows.Forms ;
using System.Data ;
using System.Runtime.InteropServices ;
using System.Text ;
using System.Diagnostics ;

namespace MemStress
{
    /// <summary>
    /// The main form for the MemStress application.
    /// </summary>
	public class FrmMain : System.Windows.Forms.Form
	{
	
        #region INI and other constants
        /// <summary>
        /// The name of the INI file.
        /// </summary>
        private const String INI_FILENAME = "MEMSTRESS.INI" ;

        /// <summary>
        /// The initialization section stuff.
        /// </summary>
        private const String INI_INITSECTION =
                                        "Memory Stress Initialization" ;
        /// <summary>
        /// The program count.
        /// </summary>
        private const String INI_INITNUMPROGRAMS = "Program Count" ;
        /// <summary>
        /// The "Program#" prefix.
        /// </summary>
        private const String INI_PROGNUMPREFIX = "Program#" ;

        /// <summary>
        /// The name of the default program.
        /// </summary>
        private const String INI_DEFAULTSECTION = "(Defaults For All)" ;

        /// <summary>
        /// The CRT main keys
        /// </summary>
        private const String INI_CRTCHECKMEM = "CRT Check Memory" ;
        /// <summary>
        /// The CRT main keys
        /// </summary>
        private const String INI_CRTDELAYMEM = "CRT Delay Memory Frees" ;
        /// <summary>
        /// The CRT main keys
        /// </summary>
        private const String INI_CRTCHECKLEAKS = "CRT Leak Checking" ;
        /// <summary>
        /// The CRT main keys
        /// </summary>
        private const String INI_GENFAILALLALLOCS =
                                             "GEN OPT Fail All Allocs" ;
        /// <summary>
        /// The general hook keys.
        /// </summary>
        private const String INI_GENFAILEVERYNALLOCS =
                                         "GEN OPT Fail Every N Allocs" ;
        /// <summary>
        /// The general hook keys.
        /// </summary>
        private const String INI_GENFAILAFTERXBYTES =
                                          "GEN OPT Fail After X Bytes" ;
        /// <summary>
        /// The general hook keys.
        /// </summary>
        private const String INI_GENFAILOVERYBYTES =
                                           "GEN OPT Fail Over Y Bytes" ;
        /// <summary>
        /// The general hook keys.
        /// </summary>
        private const String INI_GENASKONEACHALLOC =
                                           "GEN OPT Ask On Each Alloc" ;
        /// <summary>
        /// The general hook keys.
        /// </summary>
        private const String INI_GENNUMFILEFAILURES =
                                             "Number of files to fail" ;
        /// <summary>
        /// The specific file fail keys.
        /// </summary>
        private const String INI_GENFILEFAILPREFIX = "File#" ;

        /// <summary>
        /// The seperator for the file and line number for file entries.
        /// </summary>
        private const String SEP_FILEANDLINE = " & " ;
        
        /// <summary>
        /// The constants used to initialize the program listbox.
        /// </summary>
        private const String DEFAULT_PROGRAM = "(Defaults For All)" ;
        /// <summary>
        /// The zero string.
        /// </summary>
        private const String ZERO_STRING = "0" ;
        /// <summary>
        /// The one string.
        /// </summary>
        private const String ONE_STRING = "1" ;
        /// <summary>
        /// File fail display
        /// </summary>
        private const String STR_ALLALLOCATIONS = " all allocations" ;
        /// <summary>
        /// File fail display
        /// </summary>
        private const String STR_ATLINE = " at line " ;
        
        #endregion
	        
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListBox lstProgramList;
        private System.Windows.Forms.Button btnAddProgram;
        private System.Windows.Forms.Button btnRemoveProgram;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage CRTPageTab;
        private System.Windows.Forms.TabPage GenHookPageTab;
        private System.Windows.Forms.TabPage FileFailHookTab;
        private System.Windows.Forms.CheckBox chkMemOnEach;
        private System.Windows.Forms.CheckBox chkDelayFree;
        private System.Windows.Forms.CheckBox chkDoMemLeak;
        private System.Windows.Forms.CheckBox chkFailAllAllocs;
        private System.Windows.Forms.CheckBox chkFailNAllocations;
        private System.Windows.Forms.CheckBox chkFailAfterXBytes;
        private System.Windows.Forms.CheckBox chkFailOverYBytes;
        private System.Windows.Forms.CheckBox chkAskOnEachAlloc;
        private System.Windows.Forms.ListBox lstFilesToFail;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnAddFile;
        private System.Windows.Forms.Button btnRemoveFile;
        private System.Windows.Forms.Button btnEnd;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.NumericUpDown upFailNAllocations;
        private System.Windows.Forms.NumericUpDown upFailAfterXBytes;
        private System.Windows.Forms.NumericUpDown upFailOverYBytes;
        private System.ComponentModel.Container components = null;

        /// <summary>
        /// Calls the usual WinForms InitialzeComponent.
        /// </summary>
        public FrmMain()
        {
			InitializeComponent ( ) ;
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


        #region Interop Declares
        [DllImport ( "kernel32.Dll" )]
        private static extern
            Int32 GetPrivateProfileInt ( String lpAppName ,
                                         String lpKeyName ,
                                         int    nDefault  ,
                                         String lpFileName  ) ;
                                         
        [DllImport ( "kernel32.dll" )]
        private static extern
            Int32 GetPrivateProfileString
                                ( String        lpAppName        ,
                                  String        lpKeyName        ,
                                  String        lpDefault        ,
                                  StringBuilder lpReturnedString ,
                                  Int32         nSize            ,
                                  String        lpFileName        ) ;
                                  
        [DllImport ( "kernel32.dll" )]
        private static extern
            Int32 WritePrivateProfileSection ( String lpAppName  ,
                                               byte[] lpString   ,
                                               String lpFileName  ) ;
                                               
        [DllImport ( "kernel32.dll" )]
        private static extern
            Int32 WritePrivateProfileString ( String lpAppName  ,
                                              String lpKeyName  ,
                                              String lpString   ,
                                              String lpFileName  ) ;
        #endregion
        
		#region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
		private void InitializeComponent()
		{
            System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(FrmMain));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnRemoveProgram = new System.Windows.Forms.Button();
            this.btnAddProgram = new System.Windows.Forms.Button();
            this.lstProgramList = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.CRTPageTab = new System.Windows.Forms.TabPage();
            this.chkDoMemLeak = new System.Windows.Forms.CheckBox();
            this.chkDelayFree = new System.Windows.Forms.CheckBox();
            this.chkMemOnEach = new System.Windows.Forms.CheckBox();
            this.GenHookPageTab = new System.Windows.Forms.TabPage();
            this.upFailOverYBytes = new System.Windows.Forms.NumericUpDown();
            this.upFailAfterXBytes = new System.Windows.Forms.NumericUpDown();
            this.upFailNAllocations = new System.Windows.Forms.NumericUpDown();
            this.chkAskOnEachAlloc = new System.Windows.Forms.CheckBox();
            this.chkFailOverYBytes = new System.Windows.Forms.CheckBox();
            this.chkFailAfterXBytes = new System.Windows.Forms.CheckBox();
            this.chkFailNAllocations = new System.Windows.Forms.CheckBox();
            this.chkFailAllAllocs = new System.Windows.Forms.CheckBox();
            this.FileFailHookTab = new System.Windows.Forms.TabPage();
            this.btnRemoveFile = new System.Windows.Forms.Button();
            this.btnAddFile = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.lstFilesToFail = new System.Windows.Forms.ListBox();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnEnd = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.CRTPageTab.SuspendLayout();
            this.GenHookPageTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.upFailOverYBytes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.upFailAfterXBytes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.upFailNAllocations)).BeginInit();
            this.FileFailHookTab.SuspendLayout();
            this.SuspendLayout();
            //
            // groupBox1
            //
            this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                    this.btnRemoveProgram,
                                                                                    this.btnAddProgram,
                                                                                    this.lstProgramList});
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(360, 104);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Program";
            //
            // btnRemoveProgram
            //
            this.btnRemoveProgram.Location = new System.Drawing.Point(240, 64);
            this.btnRemoveProgram.Name = "btnRemoveProgram";
            this.btnRemoveProgram.Size = new System.Drawing.Size(112, 23);
            this.btnRemoveProgram.TabIndex = 2;
            this.btnRemoveProgram.Text = "&Remove Program";
            this.btnRemoveProgram.Click += new System.EventHandler(this.btnRemoveProgram_Click);
            //
            // btnAddProgram
            //
            this.btnAddProgram.Location = new System.Drawing.Point(240, 32);
            this.btnAddProgram.Name = "btnAddProgram";
            this.btnAddProgram.Size = new System.Drawing.Size(112, 23);
            this.btnAddProgram.TabIndex = 1;
            this.btnAddProgram.Text = "&Add Program";
            this.btnAddProgram.Click += new System.EventHandler(this.btnAddProgram_Click);
            //
            // lstProgramList
            //
            this.lstProgramList.Location = new System.Drawing.Point(8, 24);
            this.lstProgramList.Name = "lstProgramList";
            this.lstProgramList.Size = new System.Drawing.Size(224, 69);
            this.lstProgramList.TabIndex = 0;
            this.lstProgramList.SelectedIndexChanged += new System.EventHandler(this.lstProgramList_SelectedIndexChanged);
            //
            // groupBox2
            //
            this.groupBox2.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                    this.tabControl});
            this.groupBox2.Location = new System.Drawing.Point(8, 120);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(360, 200);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Selected Program Options";
            //
            // tabControl
            //
            this.tabControl.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                     this.CRTPageTab,
                                                                                     this.GenHookPageTab,
                                                                                     this.FileFailHookTab});
            this.tabControl.Location = new System.Drawing.Point(8, 24);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(344, 168);
            this.tabControl.TabIndex = 0;
            //
            // CRTPageTab
            //
            this.CRTPageTab.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                     this.chkDoMemLeak,
                                                                                     this.chkDelayFree,
                                                                                     this.chkMemOnEach});
            this.CRTPageTab.Location = new System.Drawing.Point(4, 22);
            this.CRTPageTab.Name = "CRTPageTab";
            this.CRTPageTab.Size = new System.Drawing.Size(336, 142);
            this.CRTPageTab.TabIndex = 0;
            this.CRTPageTab.Text = "CRT Options";
            //
            // chkDoMemLeak
            //
            this.chkDoMemLeak.Location = new System.Drawing.Point(16, 64);
            this.chkDoMemLeak.Name = "chkDoMemLeak";
            this.chkDoMemLeak.Size = new System.Drawing.Size(288, 24);
            this.chkDoMemLeak.TabIndex = 2;
            this.chkDoMemLeak.Text = "Perform memory leak checking";
            this.chkDoMemLeak.CheckedChanged += new System.EventHandler(this.chkDoMemLeak_CheckedChanged);
            //
            // chkDelayFree
            //
            this.chkDelayFree.Location = new System.Drawing.Point(16, 40);
            this.chkDelayFree.Name = "chkDelayFree";
            this.chkDelayFree.Size = new System.Drawing.Size(288, 24);
            this.chkDelayFree.TabIndex = 1;
            this.chkDelayFree.Text = "Delay freeing memory";
            this.chkDelayFree.CheckedChanged += new System.EventHandler(this.chkDelayFree_CheckedChanged);
            //
            // chkMemOnEach
            //
            this.chkMemOnEach.Location = new System.Drawing.Point(16, 16);
            this.chkMemOnEach.Name = "chkMemOnEach";
            this.chkMemOnEach.Size = new System.Drawing.Size(288, 24);
            this.chkMemOnEach.TabIndex = 0;
            this.chkMemOnEach.Text = "Check memory on each allocation and deallocation";
            this.chkMemOnEach.CheckedChanged += new System.EventHandler(this.chkMemOnEach_CheckedChanged);
            //
            // GenHookPageTab
            //
            this.GenHookPageTab.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                         this.upFailOverYBytes,
                                                                                         this.upFailAfterXBytes,
                                                                                         this.upFailNAllocations,
                                                                                         this.chkAskOnEachAlloc,
                                                                                         this.chkFailOverYBytes,
                                                                                         this.chkFailAfterXBytes,
                                                                                         this.chkFailNAllocations,
                                                                                         this.chkFailAllAllocs});
            this.GenHookPageTab.Location = new System.Drawing.Point(4, 22);
            this.GenHookPageTab.Name = "GenHookPageTab";
            this.GenHookPageTab.Size = new System.Drawing.Size(336, 142);
            this.GenHookPageTab.TabIndex = 1;
            this.GenHookPageTab.Text = "General Hook Options";
            //
            // upFailOverYBytes
            //
            this.upFailOverYBytes.Enabled = false;
            this.upFailOverYBytes.Location = new System.Drawing.Point(264, 88);
            this.upFailOverYBytes.Maximum = new System.Decimal(new int[] {
                                                                             100000,
                                                                             0,
                                                                             0,
                                                                             0});
            this.upFailOverYBytes.Minimum = new System.Decimal(new int[] {
                                                                             1,
                                                                             0,
                                                                             0,
                                                                             0});
            this.upFailOverYBytes.Name = "upFailOverYBytes";
            this.upFailOverYBytes.Size = new System.Drawing.Size(64, 20);
            this.upFailOverYBytes.TabIndex = 10;
            this.upFailOverYBytes.Value = new System.Decimal(new int[] {
                                                                           1,
                                                                           0,
                                                                           0,
                                                                           0});
            this.upFailOverYBytes.ValueChanged += new System.EventHandler(this.upFailOverYBytes_ValueChanged);
            //
            // upFailAfterXBytes
            //
            this.upFailAfterXBytes.Enabled = false;
            this.upFailAfterXBytes.Location = new System.Drawing.Point(264, 64);
            this.upFailAfterXBytes.Maximum = new System.Decimal(new int[] {
                                                                              100000,
                                                                              0,
                                                                              0,
                                                                              0});
            this.upFailAfterXBytes.Minimum = new System.Decimal(new int[] {
                                                                              1,
                                                                              0,
                                                                              0,
                                                                              0});
            this.upFailAfterXBytes.Name = "upFailAfterXBytes";
            this.upFailAfterXBytes.Size = new System.Drawing.Size(64, 20);
            this.upFailAfterXBytes.TabIndex = 9;
            this.upFailAfterXBytes.Value = new System.Decimal(new int[] {
                                                                            1,
                                                                            0,
                                                                            0,
                                                                            0});
            this.upFailAfterXBytes.ValueChanged += new System.EventHandler(this.upFailAfterXBytes_ValueChanged);
            //
            // upFailNAllocations
            //
            this.upFailNAllocations.Enabled = false;
            this.upFailNAllocations.Location = new System.Drawing.Point(264, 40);
            this.upFailNAllocations.Maximum = new System.Decimal(new int[] {
                                                                               100000,
                                                                               0,
                                                                               0,
                                                                               0});
            this.upFailNAllocations.Minimum = new System.Decimal(new int[] {
                                                                               1,
                                                                               0,
                                                                               0,
                                                                               0});
            this.upFailNAllocations.Name = "upFailNAllocations";
            this.upFailNAllocations.Size = new System.Drawing.Size(64, 20);
            this.upFailNAllocations.TabIndex = 8;
            this.upFailNAllocations.Value = new System.Decimal(new int[] {
                                                                             1,
                                                                             0,
                                                                             0,
                                                                             0});
            this.upFailNAllocations.ValueChanged += new System.EventHandler(this.upFailNAllocations_ValueChanged);
            //
            // chkAskOnEachAlloc
            //
            this.chkAskOnEachAlloc.Location = new System.Drawing.Point(16, 112);
            this.chkAskOnEachAlloc.Name = "chkAskOnEachAlloc";
            this.chkAskOnEachAlloc.Size = new System.Drawing.Size(240, 24);
            this.chkAskOnEachAlloc.TabIndex = 7;
            this.chkAskOnEachAlloc.Text = "Ask on each allocation";
            this.chkAskOnEachAlloc.CheckedChanged += new System.EventHandler(this.chkAskOnEachAlloc_CheckedChanged);
            //
            // chkFailOverYBytes
            //
            this.chkFailOverYBytes.Location = new System.Drawing.Point(16, 88);
            this.chkFailOverYBytes.Name = "chkFailOverYBytes";
            this.chkFailOverYBytes.Size = new System.Drawing.Size(240, 24);
            this.chkFailOverYBytes.TabIndex = 5;
            this.chkFailOverYBytes.Text = "Fail if request if over \'y bytes, where y =";
            this.chkFailOverYBytes.CheckedChanged += new System.EventHandler(this.chkFailOverYBytes_CheckedChanged);
            //
            // chkFailAfterXBytes
            //
            this.chkFailAfterXBytes.Location = new System.Drawing.Point(16, 64);
            this.chkFailAfterXBytes.Name = "chkFailAfterXBytes";
            this.chkFailAfterXBytes.Size = new System.Drawing.Size(240, 24);
            this.chkFailAfterXBytes.TabIndex = 3;
            this.chkFailAfterXBytes.Text = "Fail after \'x\' bytes, where x = ";
            this.chkFailAfterXBytes.CheckedChanged += new System.EventHandler(this.chkFailAfterXBytes_CheckedChanged);
            //
            // chkFailNAllocations
            //
            this.chkFailNAllocations.Location = new System.Drawing.Point(16, 40);
            this.chkFailNAllocations.Name = "chkFailNAllocations";
            this.chkFailNAllocations.Size = new System.Drawing.Size(240, 24);
            this.chkFailNAllocations.TabIndex = 1;
            this.chkFailNAllocations.Text = "Fail on every \'n\' allocations, where n =";
            this.chkFailNAllocations.CheckedChanged += new System.EventHandler(this.chkFailNAllocations_CheckedChanged);
            //
            // chkFailAllAllocs
            //
            this.chkFailAllAllocs.Location = new System.Drawing.Point(16, 16);
            this.chkFailAllAllocs.Name = "chkFailAllAllocs";
            this.chkFailAllAllocs.Size = new System.Drawing.Size(288, 24);
            this.chkFailAllAllocs.TabIndex = 0;
            this.chkFailAllAllocs.Text = "Fail all allocations";
            this.chkFailAllAllocs.CheckedChanged += new System.EventHandler(this.chkFailAllAllocs_CheckedChanged);
            //
            // FileFailHookTab
            //
            this.FileFailHookTab.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                                          this.btnRemoveFile,
                                                                                          this.btnAddFile,
                                                                                          this.label1,
                                                                                          this.lstFilesToFail});
            this.FileFailHookTab.Location = new System.Drawing.Point(4, 22);
            this.FileFailHookTab.Name = "FileFailHookTab";
            this.FileFailHookTab.Size = new System.Drawing.Size(336, 142);
            this.FileFailHookTab.TabIndex = 2;
            this.FileFailHookTab.Text = "Hook File Failures";
            //
            // btnRemoveFile
            //
            this.btnRemoveFile.Enabled = false;
            this.btnRemoveFile.Location = new System.Drawing.Point(240, 80);
            this.btnRemoveFile.Name = "btnRemoveFile";
            this.btnRemoveFile.Size = new System.Drawing.Size(80, 23);
            this.btnRemoveFile.TabIndex = 3;
            this.btnRemoveFile.Text = "Remove File";
            this.btnRemoveFile.Click += new System.EventHandler(this.btnRemoveFile_Click);
            //
            // btnAddFile
            //
            this.btnAddFile.Location = new System.Drawing.Point(240, 48);
            this.btnAddFile.Name = "btnAddFile";
            this.btnAddFile.Size = new System.Drawing.Size(80, 23);
            this.btnAddFile.TabIndex = 2;
            this.btnAddFile.Text = "Add File";
            this.btnAddFile.Click += new System.EventHandler(this.btnAddFile_Click);
            //
            // label1
            //
            this.label1.Location = new System.Drawing.Point(16, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(248, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "Fail allocations from the following files:";
            //
            // lstFilesToFail
            //
            this.lstFilesToFail.Location = new System.Drawing.Point(16, 32);
            this.lstFilesToFail.Name = "lstFilesToFail";
            this.lstFilesToFail.Size = new System.Drawing.Size(208, 95);
            this.lstFilesToFail.TabIndex = 0;
            this.lstFilesToFail.SelectedIndexChanged += new System.EventHandler(this.lstFilesToFail_SelectedIndexChanged);
            //
            // btnSave
            //
            this.btnSave.Location = new System.Drawing.Point(8, 328);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(168, 23);
            this.btnSave.TabIndex = 2;
            this.btnSave.Text = "&Save Program Settings";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            //
            // btnEnd
            //
            this.btnEnd.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnEnd.Location = new System.Drawing.Point(200, 328);
            this.btnEnd.Name = "btnEnd";
            this.btnEnd.Size = new System.Drawing.Size(168, 23);
            this.btnEnd.TabIndex = 3;
            this.btnEnd.Text = "&End";
            this.btnEnd.Click += new System.EventHandler(this.btnEnd_Click);
            //
            // FrmMain
            //
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(378, 359);
            this.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                          this.btnEnd,
                                                                          this.btnSave,
                                                                          this.groupBox2,
                                                                          this.groupBox1});
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FrmMain";
            this.Text = "C/C++ Memory Stress";
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.Closed += new System.EventHandler(this.FrmMain_Closed);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.tabControl.ResumeLayout(false);
            this.CRTPageTab.ResumeLayout(false);
            this.GenHookPageTab.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.upFailOverYBytes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.upFailAfterXBytes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.upFailNAllocations)).EndInit();
            this.FileFailHookTab.ResumeLayout(false);
            this.ResumeLayout(false);

        }
		#endregion

        #region State variables
        /// <summary>
        /// The last program selected so the data can be saved if it is
        /// dirty.
        /// </summary>
        private String strLastProg = "" ;
        /// <summary>
        /// The dirty flag.
        /// </summary>
        private Boolean bIsDirty = false ;
        /// <summary>
        /// The list of files and failure lines.
        /// </summary>
        private ArrayList m_FileList = new ArrayList ( ) ;
        #endregion

        /// <summary>
        /// Holds a file and a line.
        /// </summary>
        class FileLine
        {
            public FileLine ( String file , int line )
            {
                File = file ;
                Line = line ;
            }
            public String File ;
            public int Line ;
        }

        /// <summary>
        /// Ye ol' usual WinForm main.
        /// </summary>
		static void Main()
		{
			Application.Run ( new FrmMain ( ) ) ;
		}

        /// <summary>
        /// Initializes the lstProgramList list box.
        /// </summary>
        private void FrmMain_Load(object sender, System.EventArgs e)
        {
            // Load up all the programs.
            LoadAllPrograms ( ) ;
        }

        /// <summary>
        /// Saves any program settings when ending.
        /// </summary>
        private void FrmMain_Closed(object sender, System.EventArgs e)
        {
            SaveProgramSettings ( strLastProg , false ) ;
        }
                                       
        /// <summary>
        /// Processing switching between programs.
        private void lstProgramList_SelectedIndexChanged(object sender,
                                                     System.EventArgs e)
        {
            if ( true == bIsDirty )
            {
                SaveProgramSettings ( strLastProg , true ) ;
            }
            
            strLastProg = lstProgramList.Text ;
            
            LoadProgramSettings ( strLastProg ) ;
            
            bIsDirty = false ;
            
            if ( 0 == lstProgramList.SelectedIndex )
            {
                btnRemoveProgram.Enabled = false ;
            }
            else
            {
                btnRemoveProgram.Enabled = true ;
            }
        }
        
        /// <summary>
        /// Does the work to load the program names into lstProgramList.
        /// </summary>
        private void LoadAllPrograms ( )
        {
            // Remove everything from the list box.
            lstProgramList.Items.Clear ( ) ;
            
            // The first one is always the default program.
            lstProgramList.Items.Add ( DEFAULT_PROGRAM ) ;
            
            // Now load up the programs from the initialization section.
            Int32 iRet = GetPrivateProfileInt ( INI_INITSECTION       ,
                                                INI_INITNUMPROGRAMS   ,
                                                0                     ,
                                                INI_FILENAME          );
            if ( 0 != iRet )
            {
                StringBuilder CurrProg = new StringBuilder ( ) ;
                CurrProg.Append ( INI_PROGNUMPREFIX ) ;
                int InsertPos = CurrProg.Length ;
                
                // Holds the returned string.
                StringBuilder ReadString = new StringBuilder ( 260 ) ;
                
                int CurrNum = 1 ;
                int TotalNum = iRet + 1 ;
                while ( CurrNum < TotalNum )
                {
                    // Build the name to load.
                    CurrProg.Remove ( InsertPos                   ,
                                      CurrProg.Length - InsertPos  ) ;
                    CurrProg.Append ( CurrNum ) ;
                    
                    iRet =
                        GetPrivateProfileString ( INI_INITSECTION      ,
                                                  CurrProg.ToString ( ),
                                                  null                 ,
                                                  ReadString           ,
                                                  ReadString.Capacity  ,
                                                  INI_FILENAME        );
                    if ( 0 != iRet )
                    {
                        // Add this to the program list.
                        lstProgramList.Items.Add(ReadString.ToString());
                    }
                    else
                    {
                        MessageBox.Show ( this                        ,
                                          "Empty program string in " +
                                                CurrProg.ToString ( ) ,
                                          this.Text                   );
                    }
                    CurrNum++ ;
                }
            }
            
            // Set the name of the program to the default.
            strLastProg = DEFAULT_PROGRAM ;
            lstProgramList.SelectedIndex = 0 ;
            bIsDirty = false ;
        }
        
        /// <summary>
        /// Does the dirty work to load a program's settings and populate
        /// the tab pages data with it.
        /// </summary>
        /// <param name="strProgram">
        /// The program to load.
        /// </param>
        private void LoadProgramSettings ( String strProgram )
        {
            int Res = GetPrivateProfileInt ( strProgram         ,
                                             INI_CRTCHECKLEAKS    ,
                                             0                  ,
                                             INI_FILENAME        ) ;
            if ( 1 == Res )
            {
                chkDoMemLeak.Checked = true ;
            }
            else
            {
                chkDoMemLeak.Checked = false ;
            }
            Res = GetPrivateProfileInt ( strProgram         ,
                                         INI_CRTDELAYMEM    ,
                                         0                  ,
                                         INI_FILENAME        ) ;
            if ( 1 == Res )
            {
                chkDelayFree.Checked = true ;
            }
            else
            {
                chkDelayFree.Checked = false ;
            }
            Res = GetPrivateProfileInt ( strProgram         ,
                                         INI_CRTCHECKMEM    ,
                                         0                  ,
                                         INI_FILENAME        ) ;
            if ( 1 == Res )
            {
                chkMemOnEach.Checked = true ;
            }
            else
            {
                chkMemOnEach.Checked = false ;
            }
            Res = GetPrivateProfileInt ( strProgram             ,
                                         INI_GENFAILALLALLOCS   ,
                                         0                      ,
                                         INI_FILENAME            ) ;
            if ( 1 == Res )
            {
                chkFailAllAllocs.Checked = true ;
            }
            else
            {
                chkFailAllAllocs.Checked = false ;
            }
            
            Res = GetPrivateProfileInt ( strProgram             ,
                                         INI_GENASKONEACHALLOC  ,
                                         0                      ,
                                         INI_FILENAME            ) ;
            if ( 1 == Res )
            {
                chkAskOnEachAlloc.Checked = true ;
            }
            else
            {
                chkAskOnEachAlloc.Checked = false ;
            }
            Res = GetPrivateProfileInt ( strProgram             ,
                                         INI_GENFAILOVERYBYTES  ,
                                         0                      ,
                                         INI_FILENAME            ) ;
            if ( Res > 0 )
            {
                chkFailOverYBytes.Checked = true ;
                upFailOverYBytes.Value = Res ;
            }
            else
            {
                chkFailOverYBytes.Checked = false ;
                upFailOverYBytes.Value = 1 ;
            }
            Res = GetPrivateProfileInt ( strProgram             ,
                                         INI_GENFAILAFTERXBYTES ,
                                         0                      ,
                                         INI_FILENAME            ) ;
            if ( Res > 0 )
            {
                chkFailAfterXBytes.Checked = true ;
                upFailAfterXBytes.Value = Res ;
            }
            else
            {
                chkFailAfterXBytes.Checked = false ;
                upFailAfterXBytes.Value = 1 ;
            }
            Res = GetPrivateProfileInt ( strProgram              ,
                                         INI_GENFAILEVERYNALLOCS ,
                                         0                       ,
                                         INI_FILENAME             ) ;
            if ( Res > 0 )
            {
                chkFailNAllocations.Checked = true ;
                upFailNAllocations.Value = Res ;
            }
            else
            {
                chkFailNAllocations.Checked = false ;
                upFailNAllocations.Value = 1 ;
            }
            
            // Load the line settings.
            
            // Clear out the existing file items and the list box.
            m_FileList.Clear ( ) ;
            lstFilesToFail.Items.Clear ( ) ;
            
            // Indicates there was an error processing files, that way
            // I can clear out the data instead of possibly overwriting
            // it.
            bool SawProblems = false ;
            
            Res = GetPrivateProfileInt ( strProgram             ,
                                         INI_GENNUMFILEFAILURES ,
                                         0                      ,
                                         INI_FILENAME            ) ;
            if ( 0 != Res )
            {
                StringBuilder CurrFileStr = new StringBuilder ( ) ;
                CurrFileStr.Append ( INI_GENFILEFAILPREFIX ) ;
                int InsertPos = CurrFileStr.Length ;
                StringBuilder ReadString = new StringBuilder ( 260 ) ;
                
                // Loop through and get the files.
                for ( int Curr = 1 ; Curr < Res + 1 ; Curr++ )
                {
                    // Build the file item to load.
                    CurrFileStr.Remove (InsertPos                      ,
                                        CurrFileStr.Length - InsertPos ) ;
                    CurrFileStr.Append ( Curr ) ;
                    
                    int Res2 =
                        GetPrivateProfileString(strProgram               ,
                                                CurrFileStr.ToString ( ) ,
                                                null                     ,
                                                ReadString               ,
                                                260                      ,
                                                INI_FILENAME              ) ;
                    if ( 0 != Res2 )
                    {
                        // Does the string conform to the
                        // "Filename & Line" specification?
                        String[] Parts = ReadString.ToString().Split ( '&' ) ;
                        if ( 2 == Parts.Length )
                        {
                            // Trim any whitespace off the source file.
                            String Src = Parts[0].Trim ( ) ;
                            
                            // Try and convert the string to a number
                            // and add it to everything.
                            int Line = 0 ;
                            
                            try
                            {
                                Line = Convert.ToInt32 ( Parts[1] ) ;
                            }
                            catch ( ArgumentException )
                            {
                                SawProblems = true ;
                            }
                            catch ( FormatException )
                            {
                                SawProblems = true ;
                            }
                            catch ( OverflowException )
                            {
                                SawProblems = true ;
                            }
                            if ( true == SawProblems )
                            {
                                
                                MessageBox.Show ( this                       ,
                                                  "Invalid data in " +
                                                      strProgram +
                                                      " for "    +
                                                      CurrFileStr.ToString( ),
                                                  "MemStress"                );
                                break ;
                            }
                            FormatAndAddFilesLine ( Src , Line , false ) ;
                            
                        }
                        else
                        {
                            MessageBox.Show ( this                        ,
                                              "Invalid data in " +
                                                  strProgram +
                                                  " for "    +
                                                 CurrFileStr.ToString ( ) ,
                                              "MemStress"                  ) ;
                        }
                        
                    }
                    else
                    {
                        MessageBox.Show ( this ,
                                          "Missing file item " +
                                                CurrFileStr.ToString ( ) ,
                                          "MemStress" ) ;
                    }
                }
                lstFilesToFail.SelectedIndex = 0 ;
            }
            if ( true == SawProblems )
            {
                m_FileList.Clear ( ) ;
                lstFilesToFail.Items.Clear ( ) ;
            }
        }
        
        /// <summary>
        /// Saves a program's settings to the INI file.
        /// </summary>
        /// <param name="strProgram">
        /// The program to load.
        /// </param>
        /// <param name="bShowPrompt">
        /// True if it's supposed to prompt the user.
        /// </param>
        /// <returns>
        /// The DialogResult.
        /// </returns>
        private DialogResult SaveProgramSettings ( String strProgram  ,
                                                   Boolean bShowPrompt )
        {
            // If it's not dirty, nothing to do.
            if ( false == bIsDirty )
            {
                return ( DialogResult.OK ) ;
            }
            
            StringBuilder TempStr = new StringBuilder ( ) ;
            DialogResult Res =DialogResult.OK ;
            
            if ( true == bShowPrompt )
            {
                TempStr.Append ( "Save the settings for " ) ;
                TempStr.Append ( strProgram ) ;
                TempStr.Append ( "?" ) ;
                Res = MessageBox.Show ( this                    ,
                                        TempStr.ToString ( )    ,
                                        "Save"                  ,
                                        MessageBoxButtons.YesNo  ) ;
                if ( Res != DialogResult.Yes )
                {
                    return ( Res ) ;
                }
            }
            
            // Clear out the temp string in case it might have been
            // used.
            TempStr.Remove ( 0 , TempStr.Length ) ;
            
            // Do the CRT options.
            if ( true == chkMemOnEach.Checked )
            {
                TempStr.AppendFormat ( "{0} = 1\r\n"   ,
                                       INI_CRTCHECKMEM  ) ;
            }
            if ( true == chkDelayFree.Checked )
            {
                TempStr.AppendFormat ( "{0} = 1\r\n"   ,
                                       INI_CRTDELAYMEM  ) ;
            }
            if ( true == chkDoMemLeak.Checked )
            {
                TempStr.AppendFormat ( "{0} = 1\r\n"      ,
                                        INI_CRTCHECKLEAKS  ) ;
            }
            
            // Do the general options.
            if ( true == chkFailAllAllocs.Checked )
            {
                TempStr.AppendFormat ( "{0} = 1\r\n"         ,
                                        INI_GENFAILALLALLOCS  ) ;
            }
            if ( ( true == chkFailNAllocations.Checked ) &&
                 ( upFailNAllocations.Value > 0        )    )
            {
                TempStr.AppendFormat ( "{0} = {1}\r\n"          ,
                                       INI_GENFAILEVERYNALLOCS  ,
                               upFailNAllocations.Value.ToString ( ) ) ;
            }
            if ( ( true == chkFailAfterXBytes.Checked ) &&
                 ( upFailAfterXBytes.Value > 0        )    )
            {
                TempStr.AppendFormat ( "{0} = {1}\r\n"         ,
                                       INI_GENFAILAFTERXBYTES  ,
                               upFailAfterXBytes.Value.ToString ( ) ) ;
            }
            if ( ( true == chkFailOverYBytes.Checked ) &&
                 ( upFailOverYBytes.Value > 0        )    )
            {
                TempStr.AppendFormat ( "{0} = {1}\r\n"          ,
                                       INI_GENFAILOVERYBYTES    ,
                               upFailOverYBytes.Value.ToString ( ) ) ;
            }
            if ( true == chkAskOnEachAlloc.Checked )
            {
                TempStr.AppendFormat ( "{0} = 1\r\n"          ,
                                        INI_GENASKONEACHALLOC  ) ;
            
            }
            int FileCount = lstFilesToFail.Items.Count ;
            if ( FileCount > 0 )
            {
                TempStr.AppendFormat ( "{0} = {1}\r\n"        ,
                                       INI_GENNUMFILEFAILURES ,
                                       FileCount               ) ;
                int Curr = 1 ;
                foreach ( FileLine fl in m_FileList )
                {
                    TempStr.AppendFormat ( "{0}{1} = {2}{3}{4}\r\n" ,
                                           INI_GENFILEFAILPREFIX    ,
                                           Curr                     ,
                                           fl.File                  ,
                                           SEP_FILEANDLINE          ,
                                           fl.Line                   ) ;
                    Curr++ ;
                }
            }
            
            
            if ( TempStr.Length > 0 )
            {
                int ByteCount = TempStr.Length + 2 ;
                byte[] Bits = new byte [ ByteCount ] ;
                ASCIIEncoding.ASCII.GetBytes ( TempStr.ToString ( ) ,
                                               0                    ,
                                               TempStr.Length       ,
                                               Bits                 ,
                                               0                     ) ;
                int iRes = WritePrivateProfileSection ( strProgram   ,
                                                        Bits         ,
                                                        INI_FILENAME  );
                if ( 0 == iRes )
                {
                    MessageBox.Show ( this                    ,
                                      "Unable to save data!!" ,
                                      "MemStress"              ) ;
                }
                                                                                             
            }
            
            bIsDirty = false ;
            
            return ( Res ) ;
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes.
        /// </summary>
        private void chkMemOnEach_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes.
        /// </summary>
        private void chkDelayFree_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes.
        /// </summary>
        private void chkDoMemLeak_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes.
        /// </summary>
        private void chkFailAllAllocs_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes and enables
        /// it's matched updown.
        /// </summary>
        private void chkFailNAllocations_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
            if ( true == chkFailNAllocations.Checked )
            {
                upFailNAllocations.Enabled = true ;
            }
            else
            {
                upFailNAllocations.Enabled = false ;
            }
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes and enables
        /// it's matched updown.
        /// </summary>
        private void chkFailAfterXBytes_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
            if ( true == chkFailAfterXBytes.Checked )
            {
                upFailAfterXBytes.Enabled = true ;
            }
            else
            {
                upFailAfterXBytes.Enabled = false ;
            }
        }

        /// <summary>
        /// Tickles the dirty flag when the option changes and enables
        /// it's matched updown.
        /// </summary>
        private void chkFailOverYBytes_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
            if ( true == chkFailOverYBytes.Checked )
            {
                upFailOverYBytes.Enabled = true ;
            }
            else
            {
                upFailOverYBytes.Enabled = false ;
            }
        }

        /// <summary>
        /// Tickles the dirty flag.
        /// </summary>
        private void chkAskOnEachAlloc_CheckedChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Saves the program settings and closes the form.
        /// </summary>
        private void btnEnd_Click(object sender, System.EventArgs e)
        {
            SaveProgramSettings ( strLastProg , true ) ;
            this.Close ( ) ;
        }

        /// <summary>
        /// Saves the current program settings.
        private void btnSave_Click(object sender, System.EventArgs e)
        {
            SaveProgramSettings ( strLastProg , false ) ;
        }

        /// <summary>
        /// Adds a new program to the list and sets it to the default
        /// settings.
        /// </summary>
        private void btnAddProgram_Click(object sender, System.EventArgs e)
        {
            SaveProgramSettings ( strLastProg , true ) ;
            
            InputBox Box =
                new InputBox ( "MemStress" ,
                               "Enter the name of the program or the name passed to the InitializeMemStress function." ,
                               "" ) ;
            if ( DialogResult.OK == Box.ShowDialog ( this ) )
            {
                int iSel = 0 ;
                // Check to see this one does not exist.
                foreach ( String s in lstProgramList.Items )
                {
                    if ( 0 == String.Compare ( s             ,
                                               Box.InputText ,
                                               true           ) )
                    {
                        StringBuilder sb = new StringBuilder ( ) ;
                        sb.AppendFormat ( "The value '{0}' is already in the list" ,
                                          s ) ;
                        MessageBox.Show ( this           ,
                                          sb.ToString ( ) ,
                                          this.Text       ) ;
                        lstProgramList.SelectedIndex = iSel ;
                        return ;
                    }
                    iSel++ ;
                }
                
                strLastProg = Box.InputText ;
                
                int Ret = lstProgramList.Items.Add ( strLastProg ) ;
                lstProgramList.SelectedIndex = Ret ;
                
                int ProgCount = Ret ;
                int Ret2 =
                    WritePrivateProfileString ( INI_INITSECTION       ,
                                                INI_INITNUMPROGRAMS   ,
                                                ProgCount.ToString () ,
                                                INI_FILENAME          );
                Debug.Assert ( Ret2 > 0 ) ;
                
                Ret2 = WritePrivateProfileString ( INI_INITSECTION     ,
                                                   INI_PROGNUMPREFIX +
                                                      ProgCount.
                                                            ToString() ,
                                                    strLastProg        ,
                                                    INI_FILENAME      );
                Debug.Assert ( Ret2 > 0 ) ;

                // Load the defaults since this is a new project.
                LoadProgramSettings ( DEFAULT_PROGRAM ) ;
                
                // Save the settings so at least something is there.
                bIsDirty = true ;
                SaveProgramSettings ( strLastProg , false ) ;
            }
        }

        /// <summary>
        /// Removes the specified program from the form and the INI file.
        /// </summary>
        private void btnRemoveProgram_Click(object sender, System.EventArgs e)
        {
            int Index = lstProgramList.SelectedIndex ;
            if ( 0 == Index )
            {
                MessageBox.Show ( this ,
                                  "The default settings cannot be deleted." ,
                                  "MemStress" ) ;
                return ;
            }
            
            String StrProg = lstProgramList.Items[ Index ].ToString ( );
            DialogResult Res = DialogResult.Yes ;
            StringBuilder sb = new StringBuilder ( ) ;
            sb.AppendFormat ( "Remove the program '{0}'?"   ,
                              StrProg                        ) ;
            Res = MessageBox.Show ( this                    ,
                                    sb.ToString ( )         ,
                                    "MemStress"             ,
                                    MessageBoxButtons.YesNo  ) ;
            if ( DialogResult.Yes == Res )
            {
                lstProgramList.Items.Remove ( StrProg ) ;
                lstProgramList.SelectedIndex = 0 ;
                
                // Remove this section from the INI file.
                Int32 Ret2 = WritePrivateProfileString ( StrProg      ,
                                                         null         ,
                                                         null         ,
                                                         INI_FILENAME );
                                                         
                if ( 1 == lstProgramList.Items.Count )
                {
                    // No programs are left so kill the init section.
                    Ret2 = WritePrivateProfileString ( INI_INITSECTION ,
                                                       null            ,
                                                       null            ,
                                                       INI_FILENAME   );
                }
                else
                {
                    int Total = lstProgramList.Items.Count - 1 ;
                    int Curr = 1 ;
                    
                    StringBuilder SectStr = new StringBuilder ( ) ;
                    
                    SectStr.AppendFormat ( "{0} = {1}\n"       ,
                                           INI_INITNUMPROGRAMS ,
                                           Total                ) ;
                                           
                    while ( Curr <= Total )
                    {
                        SectStr.AppendFormat ( "{0}{1} = {2}\n"  ,
                                               INI_PROGNUMPREFIX ,
                                               Curr              ,
                                       lstProgramList.Items[Curr]);
                        Curr++ ;
                    }

                    int ByteCount = SectStr.Length + 2 ;
                    byte[] Bits = new byte [ ByteCount ] ;
                    ASCIIEncoding.ASCII.GetBytes(SectStr.ToString ( ) ,
                                                 0                    ,
                                                 SectStr.Length       ,
                                                 Bits                 ,
                                                 0                    );
                    
                    Ret2 =
                        WritePrivateProfileSection ( INI_INITSECTION ,
                                                     Bits            ,
                                                     INI_FILENAME     );
                }
                
            }
            
        }

        /// <summary>
        /// Does the UI to add an individual file.
        /// </summary>
        private void btnAddFile_Click(object sender, System.EventArgs e)
        {
            FileAndLineDlg Dlg = new FileAndLineDlg ( ) ;
            if ( DialogResult.OK == Dlg.ShowDialog ( ) )
            {
                FormatAndAddFilesLine ( Dlg.FileName ,
                                        Dlg.Line     ,
                                        true          ) ;
            }
        }

        /// <summary>
        /// Adds a file to the lstFilesToFail list box formatted
        /// appropriately.  Also adds it to the m_FileList
        /// </summary>
        private void FormatAndAddFilesLine ( String File , int Line , bool MakeDirty )
        {
            StringBuilder sb = new StringBuilder ( ) ;
            
            if ( -1 == Line )
            {
                sb.AppendFormat ( "{0}{1}" , File , STR_ALLALLOCATIONS);
            }
            else
            {
                sb.AppendFormat ( "{0}{1}{2}"   ,
                                  File          ,
                                  STR_ATLINE    ,
                                  Line           ) ;
            }
            m_FileList.Add ( new FileLine ( File , Line ) ) ;
            int i = lstFilesToFail.Items.Add ( sb.ToString ( ) ) ;
            lstFilesToFail.SelectedIndex = i ;
            bIsDirty = MakeDirty ;
        }

        /// <summary>
        /// Enables and disables the Remove file button.
        /// </summary>
        private void lstFilesToFail_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if ( -1 == lstFilesToFail.SelectedIndex )
            {
                btnRemoveFile.Enabled = false ;
            }
            else
            {
                btnRemoveFile.Enabled = true ;
            }
        }

        /// <summary>
        /// Tickles the dirty flag.
        /// </summary>
        private void upFailNAllocations_ValueChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Tickles the dirty flag.
        /// </summary>
        private void upFailAfterXBytes_ValueChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        /// <summary>
        /// Tickles the dirty flag.
        /// </summary>
        private void upFailOverYBytes_ValueChanged(object sender, System.EventArgs e)
        {
            bIsDirty = true ;
        }

        private void btnRemoveFile_Click(object sender, System.EventArgs e)
        {
            int Index = lstFilesToFail.SelectedIndex ;
            if ( -1 != Index )
            {
                m_FileList.RemoveAt ( Index ) ;
                lstFilesToFail.Items.RemoveAt ( Index ) ;
                bIsDirty = true ;
            }
        }

	}
}
