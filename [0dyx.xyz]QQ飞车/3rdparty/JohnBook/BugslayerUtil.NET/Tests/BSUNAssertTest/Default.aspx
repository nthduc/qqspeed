<%@ Register TagPrefix="cc2" Namespace="BugslayerUtil.NET" Assembly="BugslayerUtil.NET" %>
<%@ Page language="c#" 
    Codebehind="Default.aspx.cs" 
    AutoEventWireup="false" 
    Inherits="Assert.WebForm1" 
    Trace = "true" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
    <HEAD>
        <TITLE>Bugslayer.NET ASP.NET Assertion Test</TITLE>
        <META name="GENERATOR" content="Microsoft Visual Studio 7.0">
        <META name="CODE_LANGUAGE" content="C#">
        <META name="vs_defaultClientScript" content="JavaScript (ECMAScript)">
        <META name="vs_targetSchema" content="http://schemas.microsoft.com/intellisense/ie5">
    </HEAD>
    <BODY>
        <FORM id="Form1" method="post" runat="server">
            <H2>BugslayerUtil.NET <FONT color="red">Assertion</FONT> Test Page</H2>
            <P><br>
                <ASP:BUTTON id="ButtonClearASPAssertTest" runat="server" style="Z-INDEX: 101; LEFT: 11px; TOP: 59px"
                    Text="Clear the assertion!" Height="24" Width="238"></ASP:BUTTON>&nbsp;
                <ASP:BUTTON id="ButtonDoDebugAssert" runat="server" Width="238" Height="24" Text="Do a Debug.Assert!"
                    style="Z-INDEX: 102; LEFT: 264px; TOP: 59px"></ASP:BUTTON><BR>
                <BR>
                <asp:Button id="ButtonDoTraceWriteLine" runat="server" style="Z-INDEX: 104; LEFT: 11px; TOP: 89px"
                    Text="Do a Trace.WriteLine" Height="24px" Width="238px"></asp:Button>&nbsp;
                <asp:Button id="ButtonDoASPTraceWrite" runat="server" Text="Do an ASP Trace.Write" Height="24px"
                    Width="238px"></asp:Button></P>
            <P>
            </P>
            <P>
                <ASP:BUTTON id="ButtonAnotherAssert" runat="server" Width="238" Height="24" Text="Do another Debug.Assert!"></ASP:BUTTON>&nbsp;
                <asp:Button id="ButtonDoMoreTraceWriteLine" runat="server" Width="238px" Height="24px" Text="Do more Trace.WriteLines"></asp:Button></P>
            <P>
                <asp:CheckBox id="ShowInDbgCheckBox" runat="server" Text="Show in debugger"></asp:CheckBox><BR>
                <asp:CheckBox id="ShowWithODSCheckBox" runat="server" Text="Show with OutputDebugString"></asp:CheckBox><BR>
                <asp:CheckBox id="ShowInEvtLogCheckBox" runat="server" Text="Show in Event Log (no permission testing done)"></asp:CheckBox><BR>
                <asp:CheckBox id="ShowInFileCheckBox" runat="server" Text="Show in a text file (written to c:\log.txt for this example)"></asp:CheckBox><BR>
                <BR>
                <asp:CheckBox id="StartDbgCheckBox" runat="server" Text="Start debugger on assertion (only if debugger already attached)"></asp:CheckBox></P>
            <P>
                <cc2:AssertControl id="AssertDisplay" runat="server" Font-Size="XX-Small"></cc2:AssertControl></P>
        </FORM>
    </BODY>
</HTML>
