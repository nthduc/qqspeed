using System ;
using System.Collections ;
using System.ComponentModel ;
using System.Drawing ;
using System.Data ;
using System.Windows.Forms ;
using EnvDTE ;
using Microsoft.Win32 ;
using System.Runtime.InteropServices ;
using Wintellect ;

namespace SuperSaver
{

/// <summary>
/// Summary description for SuperSaverOptions.
/// </summary>
[ GuidAttribute ( "0B4F08D8-EA89-45dd-AF0A-431BB06DA2CD" ) ,
  ProgId ( "SuperSaver.OptionsControl" ) ]
public class SuperSaverOptions :
                    OptionPropPageBase
{
    private System.Windows.Forms.Label label4 ;
    private System.Windows.Forms.NumericUpDown TimeUpDown;
    private System.Windows.Forms.CheckBox TrimOnAutoSaveCheckBox;
    private System.Windows.Forms.Label TopLineBar;
    private System.Windows.Forms.Label SettingsLabel;
	/// <summary>
	/// Required designer variable.
	/// </summary>
	private System.ComponentModel.Container components = null;

	public SuperSaverOptions ( )
	{
		InitializeComponent ( ) ;

        TimeUpDown.ValueChanged +=
                    new EventHandler ( this.TimeUpDown_OnValueChanged ) ;
	}

	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	protected override void Dispose( bool disposing )
	{
		if( disposing )
		{
			if(components != null)
			{
				components.Dispose();
			}
		}
		base.Dispose( disposing );
	}

	#region Component Designer generated code
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	private void InitializeComponent()
	{
        this.SettingsLabel = new System.Windows.Forms.Label();
        this.TopLineBar = new System.Windows.Forms.Label();
        this.TimeUpDown = new System.Windows.Forms.NumericUpDown();
        this.label4 = new System.Windows.Forms.Label();
        this.TrimOnAutoSaveCheckBox = new System.Windows.Forms.CheckBox();
        ((System.ComponentModel.ISupportInitialize)(this.TimeUpDown)).BeginInit();
        this.SuspendLayout();
        //
        // SettingsLabel
        //
        this.SettingsLabel.AutoSize = true;
        this.SettingsLabel.Name = "SettingsLabel";
        this.SettingsLabel.Size = new System.Drawing.Size(44, 14);
        this.SettingsLabel.TabIndex = 0;
        this.SettingsLabel.Text = "Settings";
        //
        // TopLineBar
        //
        this.TopLineBar.Anchor = ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
            | System.Windows.Forms.AnchorStyles.Right);
        this.TopLineBar.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
        this.TopLineBar.Location = new System.Drawing.Point(8, 8);
        this.TopLineBar.Name = "TopLineBar";
        this.TopLineBar.Size = new System.Drawing.Size(472, 2);
        this.TopLineBar.TabIndex = 1;
        //
        // TimeUpDown
        //
        this.TimeUpDown.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
        this.TimeUpDown.Location = new System.Drawing.Point(16, 24);
        this.TimeUpDown.Name = "TimeUpDown";
        this.TimeUpDown.Size = new System.Drawing.Size(40, 21);
        this.TimeUpDown.TabIndex = 2;
        this.TimeUpDown.Value = new System.Decimal(new int[] {
                                                                 10,
                                                                 0,
                                                                 0,
                                                                 0});
        //
        // label4
        //
        this.label4.AutoSize = true;
        this.label4.Location = new System.Drawing.Point(72, 26);
        this.label4.Name = "label4";
        this.label4.Size = new System.Drawing.Size(181, 14);
        this.label4.TabIndex = 3;
        this.label4.Text = "Background save interval (minutes)";
        //
        // TrimOnAutoSaveCheckBox
        //
        this.TrimOnAutoSaveCheckBox.Location = new System.Drawing.Point(40, 56);
        this.TrimOnAutoSaveCheckBox.Name = "TrimOnAutoSaveCheckBox";
        this.TrimOnAutoSaveCheckBox.Size = new System.Drawing.Size(248, 16);
        this.TrimOnAutoSaveCheckBox.TabIndex = 4;
        this.TrimOnAutoSaveCheckBox.Text = "Trim white space when auto saving";
        //
        // SuperSaverOptions
        //
        this.Controls.AddRange(new System.Windows.Forms.Control[] {
                                                                      this.TrimOnAutoSaveCheckBox,
                                                                      this.label4,
                                                                      this.TimeUpDown,
                                                                      this.SettingsLabel,
                                                                      this.TopLineBar});
        this.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
        this.Name = "SuperSaverOptions";
        this.Size = new System.Drawing.Size(384, 150);
        ((System.ComponentModel.ISupportInitialize)(this.TimeUpDown)).EndInit();
        this.ResumeLayout(false);

    }
	#endregion
	
    public override void OnAfterCreated ( DTE DTEObject )
    {
        // Let the base class have at it to get the font set.
        base.OnAfterCreated ( DTEObject ) ;

        // While labels have the cool AutoSize property, check boxes
        // do not.  Grrrr.
        Graphics g = TrimOnAutoSaveCheckBox.CreateGraphics ( ) ;

        SizeF sizeF = g.MeasureString ( TrimOnAutoSaveCheckBox.Text ,
                                        TrimOnAutoSaveCheckBox.Font  );
        TrimOnAutoSaveCheckBox.Height = Convert.ToInt32 ( sizeF.Height);
        TrimOnAutoSaveCheckBox.Width  =
                    Convert.ToInt32 ( sizeF.Width ) + 20 ;
                    
        // Move the topbar to the middle of the Settings text.
        // (I am so like anal retentive...)
        int SetLabelHeight = SettingsLabel.Bottom -
                                SettingsLabel.Top ;
        int HalfHeight = SetLabelHeight / 2 ;
        TopLineBar.Top = SettingsLabel.Top + HalfHeight - 1 ;
        
        // Finally, set the current values into the dialog.
        TrimOnAutoSaveCheckBox.Checked = TrimAndSave.TrimWhiteSpace ;
        TimeUpDown.Value = TrimAndSave.AutoSaveInterval ;
    }

    public override void OnOK ( )
    {
        TrimAndSave.TrimWhiteSpace = TrimOnAutoSaveCheckBox.Checked ;
        TrimAndSave.AutoSaveInterval =
                    Convert.ToInt32 ( TimeUpDown.Value ) ;
                    
        TrimAndSave.WorkAroundLackOfShutdown ( ) ;
    }
    
    public override void OnHelp ( )
    {
        MessageBox.Show ( "Please see Chapter 9 of \"Debugging " +
                          "Microsoft .NET and Windows Applications\" " +
                          "for more information about SuperSaver." ,
                          "SuperSaver" ) ;
    }

    private void TimeUpDown_OnValueChanged ( object     Sender ,
                                             EventArgs  e       )
    {
        if ( 0 == TimeUpDown.Value )
        {
            TrimOnAutoSaveCheckBox.Enabled = false ;
        }
        else
        {
            TrimOnAutoSaveCheckBox.Enabled = true ;
        }
    }

}   // End SuperSaverOptions class.

}   // End SuperSaver namespace.
