
/**
 * @brief Tenio �ǻ���������ҵ����������С���Ʒ���ܳ�
 */

namespace Tenio
{
};

/**
 * @defgroup network	ͨѶ/����/����/Web
 * @defgroup coding		�������/���ܽ���/ѹ��
 * @defgroup system		ϵͳ���/�洢/����/��Ƶ
 * @defgroup tenuilib	Tenio UI Lib
 * @defgroup business	����ҵ�����
 * @defgroup arithmetic	�㷨/�ʷ�����/xml
 */

/**
 * @mainpage Tenio ����ϵͳ
 * @section userdoc Tenio�û�����
 * <ol>
 * <li><a href="../OtherDoc/Using Tenio.htm">Tenio���ʹ��˵��</a></li>
 * <li><a href="../OtherDoc/Manual/AudioEditor.htm">��Ƶģ��ʹ���ֲ�</a></li>
 * </ol> 
 * @section deviler Tenio�����������
 * <ol>
 * <li><a href="../Deliver/QQPet2/QQPet2.htm">QQ����2���</a></li>
 * <li><a href="../Deliver/QQST/QQST.htm">QQʢ�����</a></li>
 * <li><a href="../Deliver/QQGame/QQGame.htm">QQGame���</a></li>
 * </ol>
 * @section uilib Tenio UI Lib
 * <ol>
 * <li><a href="../UI2/index.htm">Tenio UI On-line</a></li>
 * </ol>
 * @section workflow Tenio ��̨����ϵͳ
 * <ol>
 * <li><a href="../workflow/index.htm">���̽������</a></li>
 * <li><a href="../workflow/QQTang.htm">QQTang ��̨����ϵͳ</a></li>
 * <li><a href="../workflow/QQPet2.htm">QQPet2 ��̨����ϵͳ</a></li>
 * </ol>
 * @section developerdoc Tenio�����߰���
 * <ol>
 * <li><a href="../OtherDoc/CodingRegular.htm">Tenio ����淶 V1.0</a></li>
 * <li><a href="../OtherDoc/PublishBackup.htm">Tenio �������ָ�� V1.0</a></li>
 * <li><a href="../OtherDoc/Tenio_WrittingHelp.htm">Tenio ��������ĵ���дָ�� V1.0</a></li>
 * </ol>
 */

/**
 * network
 * 001	XMLParser				Season
 * 034	ConnectController		Coolhom
 * 035	ConnectManager			Coolhom
 * 036	IPAndHostConnector		Coolhom
 * 037	HostListConnector		Coolhom
 * 038	ServerConnectManager	Coolhom
 * 039	NetSpeedDetector		Edward
 * 053	TCPSocket				Coolhom
 * 054	UDPSocket				Coolhom
 * 055	HttpProxySocket			Coolhom
 * 056	Sock5TCPProxySocket		Coolhom
 * 066	HostLookup				Coolhom
 * 117	DownloadProxy			Edward
 * 118	DownloadProcessMessage	Edward
 * 119	DownloadImplement		Edward
 * 120	-----------------		Edward
 * 121	-----------------		Edward
 * 128	-----------------		Edward
 * 129 	-----------------		Edward
 * 130	-----------------		Edward
 * 131	-----------------		Edward
 * 135	-----------------		Edward
 * 136	-----------------		Edward
 * 137	-----------------		Edward
 * 138	-----------------		Edward
 * 139	-----------------		Edward
 * 140	-----------------		Edward
 * 141	-----------------		Edward
 * 142	-----------------		Edward
 * 275	VersionDownload			Edward
 * 276	ResourceStorage			Edward
 * 322	Connector				Coolhom/Edwoard
 * 323	ShortLinkManager		Coolhom/Edwoard
 * 324	LongLinkManager			Coolhom/Edwoard
 * 378	DataPool				Robin
 * 379	DataPool				Robin
 * 383	ATLTenWeb				Living
 * 
 *
 * coding
 * 007	Base64Coding			Season
 * 031	NetEncodeHelper			Living
 * 032	NetDecodeHelper			Living
 * 033	WindowMsgDispatcher		Living
 * 064	SymmetryCrypt			Season
 * 065	MD5 Coding				Season
 * 067	Zip Coding				Season
 * 179	FolderCompress			Season
 * 355	ZipUtilityThread		Coolhom
 *
 * system
 * 003	MemoryAlloctor			Living
 * 050	Interval				Coolhom
 * 051	Timerout				Coolhom
 * 097	MySQL Operator			Ruby
 * 113	INI Operator			Ruby
 * 115	ProcessMessageObject	Edward
 * 116	ProcessMessageThread	Edward
 * 132	Thread					Edward
 * 133	Thread					Edward
 * 134	Thread					Edward
 * 178	FileOperator			Ruby
 * 183	TextFileOperator		Ruby
 * 256	VFS_Buffer				Season
 * 257	VFS_BufferCache			Season
 * 258	VFS_XXXX				Season
 * 259	VFS_XXXX				Season
 * 260	VFS_XXXX				Season
 * 261	VFS_XXXX				Season
 * 262	VFS_XXXX				Season
 * 263	VFS_XXXX				Season
 * 327	PipeServer				Robin
 * 328	PipeClient				Robin
 * 329	DumpObject				Robin
 * 343	LogObject				Robin
 * 360	RuntimeDebugObject		Robin
 * 374	MouseStateInspector		Edward
 * 375	ShareMemSocket			Robin
 * 376	PositionControler		Edward
 * 380	GlobalInterfaceManager	Living
 * 381	TrackInspector			Edward
 * 387	MouseAcitionInspector	Edward
 * 389	AudioEditor				Olive
 * 393	MouseActionJudger		Edward
 *
 *
 * tenuilib
 * 005	GifFileMaker			Season
 * 006	GifReader				Season
 * 153	Dib2Gif					Olive
 * 181	UILayoutParser			Season
 * 185	BoundaryManager			Season
 * 182	UIClassManager			Season
 * 184	UINodeInfoManager		Season
 * 186	TopControl				Season
 * 187	ValueEditorFactory		Season
 * 188	UIBoundaryEditor		Season
 * 189	UIBoundaryDraw			Season
 * 209	WinDcCanvas				Season
 * 210	xImageLib				Season
 * 211	NormalRegion			Season
 * 212	xImageMixer				Season
 * 213	MixerFactory			Season
 * 214	ImageBox				Season
 * 215	Scene					Season
 * 216	SlideShow				Season
 * 217	NormalText				Fisker
 * 273	TWindow					Season
 * 289	TDialog					Season
 * 290	TButton					Season
 * 291	TTopWindow				Season
 * 282	VariableParam			Season
 * 283	ControlFactory			Season
 * 288	InputBox				Fisker
 * 294	TopWindowInfo			Season
 * 295	TWindowCore				Season
 * 296	TDialogCore				Season
 * 297	TButtonCore				Season
 * 298	TTopWindowCore			Season
 * 299	WinControl				Season
 * 300	WinlessControl			Season
 * 284	BackGroundFactory		Season
 * 285	BG_9Image				Season
 * 286	BG_3HImage				Season
 * 287	BG_3VImage				Season
 * 332	BG_Color				Season
 * 292	TextLayer				Fisker
 * 330	ImageSetBox				Season
 * 331	ImageFactory			Season
 * 334	MultiStateBGLayer		Season
 * 335	SceneFactory			Season
 * 336	UIProperty				Season
 * 337	BGLayer_OneImage		Season
 * 338	NormalTextMixer			Fisker
 * 339	TStatic					Fisker
 * 340	TStaticCore				Fisker
 * 341	StyleParser				Season
 * 342	TextFactory				Fisker
 * 344	FontFactory				Fisker
 * 345	Container				Season
 * 346	UIContainerCore			Season
 * 347	UIExcluder				Living
 * 348	UIOverlapContainer		Living
 * 349	SignaledSwitch			Living
 * 350  TitleBar				Season
 * 351	TWinList				Season
 * 352	TListCore				Season
 * 353	TEdit					Season
 * 354	TEditCore				Season
 * 356	NormalCaret				Fisker
 * 357	NormalCaretMixer		Fisker
 * 358	CaretLayer				Fisker
 * 359	CaretFactory			Fisker
 * 363	WindowController		Olive
 * 364	TScrollBar				Season
 * 365	TScrollBarCore			Season
 * 366	TListBoxCore			Season
 * 368	TListBox				Season
 * 369	TCheckBoxCore			Lorin
 * 370	TCheckBox				Lorin
 * 371	BorderLayer				Season
 * 372	TComboBox				Season
 * 373	TComboBoxCore			Season
 * 385	TLevel					Lorin
 * 386	TLevelCore				Lorin
 * 388	WinListBox				Season
 * 390	MultiStateTextLayer		Fisker
 * 392	MultiStateImageBox		Season
 * 394	TSlider					Lorin
 * 395	TSliderCore				Lorin
 *
 *
 * business
 * 002	QQGameAdIndexFileParser	Living
 * 004	QQLoginStorage			Living
 * 049	QQGameAdManager			Living
 * 052	QQGameAdMaterialCenter	Living
 * 068	QQBaseProtocolHandler	Coolhom
 * 069	QQLoginProtocolHandler	Coolhom
 * 070	QQLogin					Coolhom
 * 095	QQGameAdBannerCollector	Living
 * 096	QQGameAdConfigStorage	Living
 * 151	QQGameAdEncryptStorage	Living
 * 155	QQGamePubGameShowObject	Living
 * 156	QQGamePubGameShowShop	Living
 * 157	QQGamePubGameShowObjCol	Living
 * 158	QQGamePubPublishPlan	Living
 * 159	QQGamePubGameShowSuit	Living
 * 160	QQGamePubSuitCollection	Living
 * 161	QQGamePubPlanCollection	Living
 * 305	ComponentOperator		Coolhom
 * 361	TenioComponentFactory	Living	
 * 367	Pet2ShopUI				Fisker
 * 377	Name2IDParser			Season
 * 384	Pet2LoginUI				Fisker
 * 391	Pet2CommunityUI			Fisker
 * 396	Pet2TSettingUI			Fisker
 * 397	Pet2Adopt				Fisker
 * 398	Pet2GoodsBox			Fisker
 *
 *
 * arithmetic
 * 114	AbstractTree			Ruby
 * 154	CPlusClassAnalyzer		Edward
 * 177	CPlusCodeGenerator		Edward
 * 180	XSLT Parser				Living
 * 274	TaskManager				Coolhom
 * 321	Greta					Living
 * 362	CommandLineParser		Lorin
 */