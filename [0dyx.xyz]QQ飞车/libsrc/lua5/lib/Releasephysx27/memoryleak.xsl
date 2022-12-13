<?xml version="1.0" encoding="iso-8859-1"?>

<xsl:stylesheet
version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
        
<html>
<head>
        <title>Memory Leak Report</title>
</head>

<body>
        <h1 align="center">Memory Leak Report</h1>
        <h2 align="center">Date: <xsl:value-of select="memory_log/@date"/></h2>
        <h3 align="center">Summary:</h3>
        
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
               <tr style="color:black; background-color:#e9d6e7">
                    <th>Statistic</th><th>Value</th>    
                </tr>
                <tr>
                    <td >Total Active Size</td>
                    <td align="center"> <xsl:value-of select="sum(memory_log/active_memory_dump/alloc_unit/@size)"/> bytes</td>
                </tr>
                <tr>
                    <td>Active Alloc Count</td>
                    <td align="center"> <xsl:value-of select="count(memory_log/active_memory_dump/alloc_unit)"/></td>
                </tr>
            </tbody>
        </table>
        
        <h3 align="center">Leaked Allocations:</h3>
        
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
               <tr style="color:black; background-color:#e9d6e7">
                    <th>Allocation #</th>
                    <th>Size in bytes</th>
                    <th>Birth Time</th>
                    <th>File</th>
                    <th>Line</th>
                    <th>Function</th>
                    <th>Long Filename</th>
                </tr>
                <xsl:for-each select="memory_log/active_memory_dump/alloc_unit">
                <xsl:sort select="@id" data-type="number" order="ascending"/>
                    <tr>
                        <td><xsl:value-of select="@id"/></td>
                        <td><xsl:value-of select="@size"/></td>
                        <td><xsl:value-of select="@alloc_time"/></td>
                        <td><xsl:value-of select="@file"/></td>
                        <td><xsl:value-of select="@line"/></td>
                        <td><xsl:value-of select="@func"/></td>
                        <td><a><xsl:attribute name="href">file:///<xsl:value-of select="@long_file"/>
                            </xsl:attribute><xsl:value-of select="@long_file"/></a></td>
                    </tr>
                </xsl:for-each>
            </tbody>
        </table>
        
</body>

</html>

</xsl:template>
</xsl:stylesheet>
