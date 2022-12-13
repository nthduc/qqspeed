<!-- 
<?xml:stylesheet href="doc.xsl" type="text/xsl"?> 
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">
<xsl:script>
function isType(node) {
    return node.GetAttribute("name").charAt(0) == 'T';
}

function isMethod(node) {
    return node.GetAttribute("name").charAt(0) == 'M';
}

function isProperty(node) {
    return node.getAttribute("name").charAt(0) == "P";
}

function isField(node) {
    return node.getAttribute("name").charAt(0) == "F";
}

function crefName(node) {
    return node.getAttribute("cref").substr(2);
}

function fullName(node) {
    return node.getAttribute("name").substr(2);
}

function memberName(node)
{
    var name = node.getAttribute("name").substr(2);
	var p = name.indexOf("#ctor");
	if (p >= 0) {
		basename = name.substr(0, p - 1);
		var params = name.substr(p + 5);
		if (params == "") params = "()";
		return basename.substr(basename.lastIndexOf(".") + 1) + params;
	}
	else {
		var p = name.indexOf("(");
		if (p == -1) p = name.length + 1;
		var basename = name.substr(0, p);
		var member = basename.substr(basename.lastIndexOf(".") + 1);
		return member + name.substr(p);
	}	
}
</xsl:script>

<xsl:template match="/">
<HTML>
<HEAD>
<TITLE><xsl:value-of select="doc/assembly/name"/></TITLE>
<LINK rel="stylesheet" type="text/css" href="doc.css"/>
</HEAD>
<BODY>
    <xsl:apply-templates select="doc/members/member"/>
</BODY>
</HTML>
</xsl:template>

<xsl:template match="member">
    <xsl:choose>
		<xsl:when expr="isType(this)">
			<h1><xsl:eval>fullName(this)</xsl:eval></h1>
			<xsl:apply-templates select="summary"/>
			<xsl:apply-templates select="remarks"/>
			<xsl:apply-templates select="example"/>
			<xsl:if test="seealso">
				<h4>See Also</h4>
				<xsl:apply-templates select="seealso"/>
			</xsl:if>
		</xsl:when>
		<xsl:when expr="isMethod(this)">
			<h2><xsl:eval>memberName(this)</xsl:eval> method</h2>
			<xsl:apply-templates select="summary"/>
			<xsl:if test="param">
				<h4>Parameters</h4>
				<dl><xsl:apply-templates select="param"/></dl>
			</xsl:if>
			<xsl:apply-templates select="returns"/>
			<xsl:if test="exception">
				<h4>Exceptions</h4>
				<dl><xsl:apply-templates select="exception"/></dl>
			</xsl:if>
			<xsl:apply-templates select="remarks"/>
			<xsl:apply-templates select="example"/>
			<xsl:if test="seealso">
				<h4>See Also</h4>
				<xsl:apply-templates select="seealso"/>
			</xsl:if>
		</xsl:when>
		<xsl:when expr="isProperty(this)">
			<h2><xsl:eval>memberName(this)</xsl:eval> property</h2>
			<xsl:apply-templates select="summary"/>
			<xsl:if test="param">
				<h4>Parameters</h4>
				<dl><xsl:apply-templates select="param"/></dl>
			</xsl:if>
			<xsl:apply-templates select="value"/>
			<xsl:if test="exception">
				<h4>Exceptions</h4>
				<dl><xsl:apply-templates select="exception"/></dl>
			</xsl:if>
			<xsl:apply-templates select="remarks"/>
			<xsl:apply-templates select="example"/>
			<xsl:if test="seealso">
				<h4>See Also</h4>
				<xsl:apply-templates select="seealso"/>
			</xsl:if>
		</xsl:when>
		<xsl:when expr="isField(this)">
			<h2><xsl:eval>memberName(this)</xsl:eval> field</h2>
			<xsl:apply-templates select="summary"/>
			<xsl:apply-templates select="value"/>
			<xsl:apply-templates select="remarks"/>
			<xsl:apply-templates select="example"/>
			<xsl:if test="seealso">
				<h4>See Also</h4>
				<xsl:apply-templates select="seealso"/>
			</xsl:if>
		</xsl:when>
	</xsl:choose>
</xsl:template>

<xsl:template match="summary"><p><xsl:apply-templates/></p></xsl:template>

<xsl:template match="param">
    <dt><i><xsl:value-of select="@name"/></i></dt>
	<dd><xsl:apply-templates/></dd>
</xsl:template>

<xsl:template match="value">
    <h4>Value</h4>
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="returns">
    <h4>Returns</h4>
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="exception">
    <dt><i><xsl:eval>crefName(this)</xsl:eval></i></dt>
	<dd><xsl:apply-templates/></dd>
</xsl:template>

<xsl:template match="remarks">
    <h4>Remarks</h4>
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="example">
    <h4>Example</h4>
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="seealso">
    <a><xsl:attribute name="href"><xsl:value-of select="@cref"/></xsl:attribute><xsl:eval>crefName(this)</xsl:eval></a>&#160;
</xsl:template>

<xsl:template match="text()"><xsl:value-of/></xsl:template>

<xsl:template match="para"><p><xsl:apply-templates/></p></xsl:template>

<xsl:template match="code"><pre><xsl:value-of/></pre></xsl:template>

<xsl:template match="see"><a><xsl:attribute name="href"><xsl:value-of select="@cref"/></xsl:attribute><xsl:eval>crefName(this)</xsl:eval></a></xsl:template>

<xsl:template match="list"><table><xsl:apply-templates/></table></xsl:template>

<xsl:template match="listheader"><tr><xsl:apply-templates/></tr></xsl:template>

<xsl:template match="item"><tr><xsl:apply-templates/></tr></xsl:template>

<xsl:template match="term"><td><xsl:apply-templates/></td></xsl:template>

<xsl:template match="description"><td><xsl:apply-templates/></td></xsl:template>

<xsl:template match="c"><code><xsl:apply-templates/></code></xsl:template>

<xsl:template match="paramref"><i><xsl:value-of select="@name"/></i></xsl:template>

</xsl:stylesheet>