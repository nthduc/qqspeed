GIF文件解码器类使用说明
    本解码器类支持GIF文件格式的89a版本，可支持动画GIF文件！
本原代码未经严格测试，欢迎广大朋友批评指正。

1.使用授权
    本原代码文档包括下列文件：
    gif89a.cpp   解码器类原程序
    gif89a.h     解码器类头文件
    gif89a.doc   GIF文件格式说明
    lzw.doc      GIF文件所用压缩算法的说明
    readme.txt   自述文件
    gifinfo.exe  使用本原代码的例子程序
    gifinfo.cpp  例子程序原文件  
    如果您要在各种媒体刊载、引用本文档，请不要对本文档内容进行任何修改。如果您在自己的编程中
引用本文档内容，可对引用内容进行修改。在您引用本文档时如能注明出处，并通过下面方式通知本人一
下，本人将非常高兴！

联系方式：
e-mail:cproom@163.net
http://cproom.yeah.net
通讯地址：陕西省西安市太白南路202号(骊山微电子公司)陈鹏（收）
邮编　　：710065
电话　　：(029)-8234582

2.GIF文件解码器类参考
    请参考GIF文件格式说明文件gif89a.doc

    (1)结构类型说明：
        typedef struct                //GIF文件的全局参数
        {    UINT frames;             //文件中图象帧数
             WORD scrWidth,scrHeight; //逻辑屏幕的宽度和高度（单位像素）
             BOOL gFlag;              //是否有全局调色板（决定其他调色板参数是否有效）
             UINT colorRes;           //色彩分辨率（不使用）
             BOOL gSort;              //全局调色板是否按优先排序
             UINT gSize;              //全局调色板大小（有多少个实际入口）
             UINT BKColorIdx;         //背景色的调色板索引
             UINT pixelAspectRatio;   //像素长宽比例
             BYTE *gColorTable;       //指向全局调色板的指针（256个入口，每个入口三字节）
                                      //调色板格式请参看gif89a.doc
        }GLOBAL_INFO;

        typedef struct                //一帧图象的参数
        {    WORD imageLPos;          //图象左边沿到逻辑屏幕的距离（单位像素）
             WORD imageTPos;          //图象上边沿到逻辑屏幕的距离（单位像素）
             WORD imageWidth;         //图象的宽度（单位像素）
             WORD imageHeight;        //图象的高度（单位像素）
             BOOL lFlag;              //是否有局部调色板（决定其他调色板参数是否有效）
             BOOL interlaceFlag;      //图象数据是否交错
             BOOL sortFlag;           //局部调色板数据是否按优先排序
             UINT lSize;              //局部调色板大小（有多少个实际入口）
             BYTE *pColorTable;       //指向局部调色板的指针（256个入口，每个入口三字节）
                                      //调色板格式请参看gif89a.doc
             BYTE *dataBuf;           //图象数据指针
             GCTRLEXT ctrlExt;        //图象扩展参数（与透明背景和动画有关）
        }FRAME;

        typedef struct                //图象扩展参数
        {    BOOL active;             //本结构中的其它参数是否可用
             UINT disposalMethod;     //处理方法（见gif89a.doc，可忽略）
             BOOL userInputFlag;      //是否期待用户输入
             BOOL trsFlag;            //是否有透明色
             WORD delayTime;          //延时时间（单位1/100秒）
             UINT trsColorIndex;      //透明色调色板索引
        }GCTRLEXT;

    （2）公有数据成员
        无！

    （3）私有数据成员
        ifstream ifs;                 //用于读文件的文件流
	    char version[4];              //版本字符串
        BOOL error;                   //类实例变量创建时是否出错的标志
        BOOL opened;                  //是否处于打开状态
        BOOL inMem;                   //图象数据是否一次读入内存
        BYTE gColorTable[256*3];      //全局调色板
        BYTE lColorTable[256*3];      //局部调色板（inMem为假时用）
        streampos dataStart;          //保存文件流中图象数据开始的地方
        FRAME *allFrames;             //指向所有图象帧的指针（inMem为真时用）
        UINT curIndex;                //当前帧的索引（inMem为真时用）
        GLOBAL_INFO gInfo;            //GIF文件的全局参数
        FRAME curFrame;               //当前帧的参数（inMem为假时用）
        GCTRLEXT ctrlExt;             //图象扩展参数（读入数据时临时使用）

    （4）私有成员函数
        UINT CGif89a::checkFrames(ifstream& ifs)
        功能  ：检查文件中图象帧数。
        参数  ：ifs:对文件流的引用。
        返回值：文件中图象帧数。

        BOOL CGif89a::getAllFrames(ifstream& ifs)
        功能  ：将所有图象帧数据读入内存。
        参数  ：ifs:对文件流的引用。
        返回值：操作是否成功，为真成功，为假失败。

        BOOL CGif89a::extractData(FRAME* f,ifstream& ifs)
        功能  ：解压缩一帧图象数据。
        参数  ：f:指向用于保存图象数据的结构。
                ifs:对文件流的引用。
        返回值：操作是否成功，为真成功，为假失败。

        BOOL CGif89a::initStrTable(STRING_TABLE_ENTRY* strTable,UINT rootSize)
        功能  ：初始化字符串表。
        参数  ：strTable:指向字符串表的指针。
                rootSize:初始化的入口数。
        返回值：操作是否成功，为真成功，为假失败。

        BOOL CGif89a::addStrTable(STRING_TABLE_ENTRY* strTable,UINT addIdx,UINT idx,unsigned char c)
        功能  ：在字符串表中增加一项。
        参数  ：strTable:指向字符串表的指针。
                addIdx:增加的入口索引。
                idx:用于构造要增加的字符串的入口索引。
                c:用于构造要增加的字符串的字符。
        返回值：操作是否成功，为真成功，为假失败。

    （5）公有成员函数
        CGif89a::CGif89a()
        功  能：构造函数
        参  数：无
        返回值：无
        说  明：用于构造一个空的实例，以便以后用open()函数打开文件。

        CGif89a::CGif89a(LPCSTR fileName,BOOL inMem)
        功  能：构造函数
        参  数：fileName:要打开的文件名字符串
                inMem:是否将图象数据一次读入内存
        返回值：无
        说  明：内部调用open()函数，请参考open()函数。

        CGif89a::~CGif89a()
        功  能：析构函数
        参  数：无
        返回值：无
        说  明：内部调用close()函数进行清理工作。

        BOOL CGif89a::operator!()
        功  能：判断构造是否成功
        参  数：无
        返回值：为真则构造成功，为假则失败。
        说  明：在构造函数调用之后，一定要调用此函数确定构造是否成功。

        BOOL CGif89a::open(LPCSTR fileName,BOOL inMem)
        功  能：打开文件
        参  数：fileName:要打开的文件名字符串
                inMem:是否将图象数据一次读入内存
        返回值：为真则成功，为假则失败
        说  明：如果inMem为真，则将所有图象帧数据一次全部读入内存，然后关闭文件。此方式
                用于文件较小，需要读取数据较快的情况。此方式占用内存较多。
                如果inMem为假，则保持文件为打开状态，以后需要时由文件读入数据。此方式用
                于文件较大，对读取数据不要求很快的情况。此方式占用内存较少。
                使用其它函数前，应首先调用此函数。
                本函数对GIF文件完整性要求较为严格，对于从Internet上下载的GIF文件，如果未
                能完整下载，则打开将失败。

        void CGif89a::close()
        功  能：将状态有打开切换到关闭，以便打开新的文件
        参  数：无
        返回值：无
        说  明：关闭文件（inMem为假时），释放内存，可以再次用open()打开新的文件。

        LPCSTR CGif89a::getVer()
        功  能：读取版本字符串
        参  数：无
        返回值：指向版本字符串的指针
        说  明：无

        LPCFRAME CGif89a::getNextFrame()
        功  能：取得一帧图象
        参  数：无
        返回值：指向FRAME结构的指针，保存了图象数据，及参数,成功时返回非0值，失败时返回NULL。
        说  明：每次调用返回一帧新的数据，图象数据是已经解压的。当读取最后一帧数据后，再
                次调用此函数，返回第一帧的数据，如此循环。使用返回的数据时要注意各种参数，
                特别是数据是否交错。
                如果图象帧数大于1，循环调用此函数可得到动画GIF文件中的所有图象帧数据。
                一定要检查返回值是否有效！

        LPCGLOBAL_INFO CGif89a::getGlobalInfo()
        功  能：取得全局参数
        参  数：无
        返回值：指向GLOBAL_INFO结构的指针，保存了GIF文件的全局参数。
        说  明：无

3.gifinfo.exe使用说明
    功能：显示GIF文件全局参数和每一帧图象的参数
    用法：gifinfo GIF文件名
    说明：输出文字中有汉字，请在DOS窗口运行。如果文件中有多帧图象，显示内容可能看不全，可
          用下面格式将输出保存到文件，再打开文件观看。
          gifinfo GIF文件名 > tp.txt

“全文完”