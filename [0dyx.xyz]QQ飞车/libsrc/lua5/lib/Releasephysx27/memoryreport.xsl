<?xml version="1.0" encoding="iso-8859-1"?>

<xsl:stylesheet
version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:key name="sizes" match="alloc_unit" use="@size"/>
<xsl:key name="files" match="alloc_unit" use="@long_file"/>
<xsl:key name="funcs" match="alloc_unit" use="@func"/>
<xsl:template match="/">
        
<html>
<head>
        <title>Memory Report</title>
</head>

<body>
        <h1 align="center">Memory Report</h1>
        <h2 align="center">Date: <xsl:value-of select="memory_log/@date"/></h2>
        <h3 align="center">Summary:</h3>
        
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Statistic</th><th>Value</th>   
                </tr>
                <tr>
                    <td >Application Lifetime</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@timestamp"/> seconds</td>
                </tr>
                <tr>
                    <td >Total Active Size</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@TotalActiveSize"/> bytes</td>
                </tr>
                <tr>
                    <td>Peak Active Size</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@PeakActiveSize"/> bytes</td>
                </tr>
                <tr>
                    <td>Accumulated Size</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@AccumulatedSize"/> bytes</td>
                </tr>
                <tr>
                    <td>Allocated But Unused Size</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@AllocatedButUnusedSize"/> bytes</td>
                </tr>
                <tr>
                    <td>Percent of Allocations Unused</td>
                    <td align="center"> <xsl:value-of select="format-number(memory_log/memory_summary/@AllocatedButUnusedSize div memory_log/memory_summary/@AccumulatedSize * 100.0, '#.##')"/>%</td>
                </tr>
                <tr>
                    <td>Active Alloc Count</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@ActiveAllocCount"/></td>
                </tr>
                <tr>
                    <td>Peak Active Alloc Count</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@PeakActiveAllocCount"/></td>
                </tr>
                <tr>
                    <td>Total Alloc Count</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@TotalAllocCount"/></td>
                </tr>
                <tr>
                    <td>Total Tracker Overhead</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@TotalTrackerOverhead"/> bytes</td>
                </tr>
                <tr>
                    <td>Peak Tracker Overhead</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@PeakTrackerOverhead"/> bytes</td>
                </tr>
                <tr>
                    <td>Accumulated Overhead</td>
                    <td align="center"> <xsl:value-of select="memory_log/memory_summary/@AccumulatedTrackerOverhead"/> bytes</td>
                </tr>
                <tr>
                    <td>Allocation Sizes &lt; 1024 bytes</td>
                    <td align="center"> <xsl:value-of select="count(memory_log/memory_dump/alloc_unit[@size &lt; 1024])"/></td>
                </tr>
                <tr>
                    <td>Allocation Life Spans &lt; 1 second</td>
                    <td align="center"> <xsl:value-of select="count(memory_log/memory_dump/alloc_unit[@life_span &lt; 1.0])"/></td>
                </tr>
                <tr>
                    <td>Percent Life Spans &lt; 1 second</td>
                    <td align="center"> <xsl:value-of select="format-number(count(memory_log/memory_dump/alloc_unit[@life_span &lt; 1.0]) div memory_log/memory_summary/@TotalAllocCount * 100.0, '#.##')"/>%</td>
                </tr>
                 <tr>
                    <td>Percent Sizes &lt; 1024 bytes</td>
                    <td align="center"> <xsl:value-of select="format-number(count(memory_log/memory_dump/alloc_unit[@size &lt; 1024]) div memory_log/memory_summary/@TotalAllocCount * 100.0, '#.##')"/>%</td>
                </tr>
                
            </tbody>
        </table>
        
        <h3 align="center">Top 50 Largest Allocations:</h3>
        
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Allocation #</th>
                    <th>Size in bytes</th>
                    <th>Life Span</th>
                    <th>File</th>
                    <th>Line</th>
                    <th>Function</th>
                </tr>
                    <xsl:for-each select="memory_log/memory_dump/alloc_unit">
                    <xsl:sort select="@size" data-type="number" order="descending"/>
                        <xsl:if test="position() &lt; 50">
                        <tr>
                            <td><xsl:value-of select="@id"/></td>
                            <td><xsl:value-of select="@size"/></td>
                            <td><xsl:value-of select="@life_span"/></td>
                            <td><xsl:value-of select="@file"/></td>
                            <td><xsl:value-of select="@line"/></td>
                            <td><xsl:value-of select="@func"/></td>
                        </tr>
                        </xsl:if>
                    </xsl:for-each>
                
            </tbody>
        </table>
        
        <h3 align="center">Top 50 Smallest Allocations:</h3>
        
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Allocation #</th>
                    <th>Size in bytes</th>
                    <th>Life Span</th>
                    <th>File</th>
                    <th>Line</th>
                    <th>Function</th>
                </tr>
                    <xsl:for-each select="memory_log/memory_dump/alloc_unit">
                    <xsl:sort select="@size" data-type="number" order="ascending"/>
                        <xsl:if test="position() &lt; 50">
                        <tr>
                            <td><xsl:value-of select="@id"/></td>
                            <td><xsl:value-of select="@size"/></td>
                            <td><xsl:value-of select="@life_span"/></td>
                            <td><xsl:value-of select="@file"/></td>
                            <td><xsl:value-of select="@line"/></td>
                            <td><xsl:value-of select="@func"/></td>
                        </tr>
                        </xsl:if>
                    </xsl:for-each>
                
            </tbody>
        </table>
        
        <h3 align="center">Top 50 Shortest-lived Allocations:</h3>
        
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Allocation #</th>
                    <th>Size in bytes</th>
                    <th>Life Span</th>
                    <th>File</th>
                    <th>Line</th>
                    <th>Function</th>
                </tr>
                    <xsl:for-each select="memory_log/memory_dump/alloc_unit">
                    <xsl:sort select="@life_span" data-type="number" order="ascending"/>
                        <xsl:if test="position() &lt; 50">
                        <tr>
                            <td><xsl:value-of select="@id"/></td>
                            <td><xsl:value-of select="@size"/></td>
                            <td><xsl:value-of select="@life_span"/></td>
                            <td><xsl:value-of select="@file"/></td>
                            <td><xsl:value-of select="@line"/></td>
                            <td><xsl:value-of select="@func"/></td>
                        </tr>
                        </xsl:if>
                    </xsl:for-each>
                
            </tbody>
        </table>
        
        <h3 align="center">Number of Allocations by File</h3>
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Filename</th>
                    <th>Count</th>
                </tr>
                <xsl:for-each select="memory_log/memory_dump/alloc_unit[generate-id() = generate-id(key('files',@long_file)[1])]">
                <xsl:sort select="count(../alloc_unit[@long_file=current()/@long_file])" data-type="number" order="descending"/>
                <tr>
<!--                   <td><a><xsl:attribute name="href">file:///<xsl:value-of select="@long_file"/></xsl:attribute><xsl:value-of select="@long_file"/></a>
                    </td>-></!-->
                    <td><xsl:value-of select="@long_file"/></td>
                    <td><xsl:value-of select="count(../alloc_unit[@long_file=current()/@long_file])"/></td>
                </tr>
                </xsl:for-each>

            </tbody>
        </table>
        
        <h3 align="center">Number of Allocations by Function</h3>
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Function</th>
                    <th>Count</th>
                </tr>
                <xsl:for-each select="memory_log/memory_dump/alloc_unit[generate-id() = generate-id(key('funcs',@func)[1])]">
                <xsl:sort select="count(../alloc_unit[@func=current()/@func])" data-type="number" order="descending"/>
                <tr>
                    <td><xsl:value-of select="@func"/></td>
                    <td><xsl:value-of select="count(../alloc_unit[@func=current()/@func])"/></td>
                </tr>
                </xsl:for-each>

            </tbody>
        </table>
        
        <h3 align="center">Number of Allocations by Size</h3>
        <table cellpadding="5" border="2" align="center" style="font-family:Arial,sans-serif; font-size:10pt">
            <tbody>
                <tr style="color:black; background-color:#e9d6e7">
                    <th>Size in bytes</th>
                    <th>Count</th>
                </tr>
                <xsl:for-each select="memory_log/memory_dump/alloc_unit[generate-id() = generate-id(key('sizes',@size)[1])]">
                <xsl:sort select="@size" data-type="number"/>
                <tr>
                    <td><xsl:value-of select="@size"/></td>
                    <td><xsl:value-of select="count(../alloc_unit[@size=current()/@size])"/></td>
                </tr>
                </xsl:for-each>

            </tbody>
        </table>

</body>

</html>

</xsl:template>

</xsl:stylesheet>
