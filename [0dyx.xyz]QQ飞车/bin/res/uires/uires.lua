UI_CONFIG_FILE="res/uires/uiconfig.lua";
UI_ROOT_FILE="res/uires/uiroot.lua";
UI_ROOT_NAME="UI";

--widget_layer
normal_layer_init = 0;
focus_layer_plus  = 1000;
console_layer = 65535;
systooltip_layer = 65534;
msgbox_layer  = 65533 / 2;

--stytem_widget
buddy_widget_none=0;
buddy_widget_tipwgt=1;
buddy_widget_msgbox=2;
buddy_widget_console=3;
buddy_widget_host=4;
buddy_widget_infowgt=5;
buddy_widget_buddy=6;
buddy_widget_scroll_primary=7;
buddy_widget_scroll_secondary=8;
buddy_widget_block_button=9;

buddy_style_self=0;
buddy_style_father=1;
buddy_style_screen=2;
buddy_style_root=4;

edge_check_none	=0;
edge_check_show	=1;	
edge_check_update	=2;	
edge_check_auto	=4;	

wgterror_none		=-1;
wgterror_medit_font	= 0;
wgterror_medit_color	= 1;
wgterror_medit_insert	= 2;

--richmode
ui_richmode_none	 =0;--图标
ui_richmode_icon	 =1;--图标
ui_richmode_color 	 =2;--颜色
ui_richmode_font	 =4;--字体
ui_richmode_style 	 =8;--风格
ui_richmode_hlink 	 =16;--超链接
ui_richmode_field	 =32;--域
ui_richmode_all 	  	=63;--全部


--UIMsgType
msgtype_update		=0;
msgtype_paint		=1;
msgtype_char		=2;
msgtype_ime_char	=3;
msgtype_keypress	=4;
msgtype_keydown	=5;
msgtype_keyup		=6;
msgtype_leftbtndown	=7;
msgtype_leftbtnup	=8;
msgtype_lbtndblclk	=9;
msgtype_rightbtndown	=10;
msgtype_rightbtnup	=11;
msgtype_mousemove	=12;
msgtype_mousein	=13;
msgtype_mouseout	=14;
msgtype_gainfocus	=15;
msgtype_lostfocus	=16;
msgtype_timer		=17;
msgtype_mousewheel	=18;
msgtype_mouseidle	=19;

--widget_pattern
wgtpattern_none			= 0;
wgtpattern_rawvisible		= 1;
wgtpattern_alphavisible	= 2;
	
--widget_style
wgtstyle_none				= 0;
wgtstyle_flash				= 1;
wgtstyle_vertical				= 2;
wgtstyle_primary				= 4;
wgtstyle_clipself				= 8; 
wgtstyle_clipchild_window   		= 16;
wgtstyle_clipchild_client   		= 32;
wgtstyle_systemdraw			= 64;
wgtstyle_advance			= 128;
wgtstyle_popup				= 256;
wgtstyle_self_xmove			= 512;
wgtstyle_self_ymove			= 1024;
wgtstyle_self_xsize			= 2048;
wgtstyle_self_ysize			= 4096;
wgtstyle_static				= 8192;
wgtstyle_topmost			= 16384;
wgtstyle_alphawin			= 32768;
wgtstyle_flipdrag			= 65536;
wgtstyle_template			= 131072;
wgtstyle_adjustself			= 262144;
wgtstyle_ajustfollow			= 524288;
wgtstyle_cleanwin			= 1048576;
wgtstyle_editable			= 2097152;
wgtstyle_colortest			= 4194304;
wgtstyle_customclose			= 8388608;
wgtstyle_customalpha			= 16777216;
wgtstyle_renderbuffer			= 33554432;
wgtstyle_renderitem			= 67108864;
wgtstyle_areawake			= 134217728;
wgtstyle_fatherenable			= 268435456;
wgtstyle_alignfollow			= 536870912;
wgtstyle_autoactive			= 1073741824;

--listbox style
listbox_style_multi_sel		=  1;
listbox_style_itemheight_fix		=  2;
listbox_style_enum_item		=  8;
listbox_style_wheel			=  16;
listbox_style_directory			=  32;
listbox_style_file				=  64;
listbox_style_custom_arrange	=  128;
listbox_style_dbclick_sel		=256;
listbox_style_auto_comfort		=512;
listbox_style_richtext			=1024;
listbox_style_colorlist			=2048;	

--wgtlist style
wgtlist_style_none=			0;
wgtlist_style_hover_focus=		1;--鼠标停留
wgtlist_style_enum_item=		2;--mouse wheel消息在item间切换
wgtlist_style_delete_item=		4;--删除多余孩子
wgtlist_style_wheel=			8;--mouse wheel消息不切换

--textlist style
textlist_style_none		=0;
textlist_style_alphafont	=1;--绘制半透明字体
textlist_style_reverse		=2;--从上到下绘制文本
textlist_style_autoscroll	=4;--自动滚动到最新添加文本	
textlist_style_smartscroll	=8;--对齐文本首部
textlist_style_delayscroll	=16;--自动缓慢滚向最新添加文本
textlist_style_blockinmiddle	=32;--滑块不在最后的时候阻止自动滚动
	
--treectrl style
treectrl_style_auto_comfort	=1;--自动comfort
treectrl_style_drawgrid		=2;--是否绘制线条

--tabwin_style
tabwin_style_none		=0;
tabwin_style_manual_check	=1;--手动check

--scrollbar type
scrollbar_style_none		=	 0;
scrollbar_blockbtn_auto_size=	 1;
scrollbar_blockbtn_dock_flip	=	 2;
scrollbar_direction_flip		=	 4;
scrollbar_render_slot		=	 8;
scrollbar_auto_hide		=	 16;
scrollbar_auto_disable		=	 32;

--tabwin type
tabwin_type_default=0;
tabwin_type_down=1;
tabwin_type_up=2;
tabwin_type_left=3;
tabwin_type_right=4;

--patch_id
patchid_up_left=0;
patchid_up_right=1;
patchid_down_left=2;
patchid_down_right=3;
patchid_up_middle=4;
patchid_down_middle=5;
patchid_left_middle=6;
patchid_right_middle=7;
	

--btn_style
ui_btn_style_none		  =0;
ui_btn_style_popupmsg	  =1;
ui_btn_style_radio		  =2;
ui_btn_style_auto_toggle  	  =4;
ui_btn_style_auto_round        =8;
ui_btn_style_spin		  =16;

--EdgeFlag
edge_flag_none		=0;
edge_flag_up		=1;
edge_flag_down	=2;
edge_flag_left		=4;
edge_flag_right		=8;

--RectFlag
rect_flag_window	=0;
rect_flag_client		=1;
rect_flag_caption	=2;
rect_flag_drag		=4;

--text_style
dt_top   			=0;
dt_left             		=0;
dt_center           	=1;
dt_right            		=2;
dt_vcenter          	=4;
dt_bottom           	=8;
dt_magicback		= 1048576;
dt_magicedge		= 2097152;
dt_magicline		= 4194304;
dt_magicaxis		= 8388608;

--align_style
alignstyle_none   	   = 0;
alignstyle_top_in	   = 1;
alignstyle_bottom_in      = 2;
alignstyle_left_in     	   = 4;	
alignstyle_right_in    	   = 8;

alignstyle_top_out	   = 16;
alignstyle_bottom_out    = 32;
alignstyle_left_out         = 64;	
alignstyle_right_out   	   = 128;

alignstyle_hcenter          = 256;
alignstyle_vcenter          = 512;

alignstyle_hmid_left   	   = 1024;
alignstyle_hmid_right     = 2048;
	
alignstyle_vmid_top	   = 4096;
alignstyle_vmid_bottom  = 8192;
		
--enum align_type		
aligntype_none   		= 0;
aligntype_brother		= 1;
aligntype_father			= 2;
aligntype_uncle			= 4;
aligntype_grandpa		= 8;		
			
aligntype_winrect		= 16;
aligntype_clientrect		= 32;
aligntype_captionrect		= 64;
aligntype_dragrect		= 128;	
			
--value channel
value_channel_state			=0; 
value_channel_style			=1;	
value_channel_dragtype		=2;	
value_channel_tabstop		=3;
value_channel_groupstop		=4;
value_channel_groupid			=5;
value_channel_textstyle		=6;
value_channel_textsize		=7;
value_channel_snaph			=8;
value_channel_snapv			=9;

value_channel_value			=10;
value_channel_minvalue		=11;
value_channel_maxvalue		=12;
value_channel_step			=13;
value_channel_page_h			=14;
value_channel_page_v			=15;
value_channel_pagenum_h		=16;
value_channel_pagenum_v		=17;
value_channel_sel			=18;
value_channel_pos			=19;

value_channel_templateid		=20;

value_channel_hosttype		=21;
value_channel_hostindex		=22;

value_channel_value_h		=23;
value_channel_value_v		=24;
value_channel_lower_h		=25;
value_channel_uper_h			=26;		
value_channel_lower_v		=27;
value_channel_uper_v			=28;
value_channel_method			=29;

value_channel_edge_check		=30;

value_channel_frame			=31;
value_channel_item_num		=32;

value_channel_winrect			=34;
value_channel_clientrect		=35;
value_channel_captionrect		=36;
value_channel_dragrect		=37;
value_channel_dragindex		=38;

value_channel_areaid			=39;

value_channel_textfont		=40;
value_channel_alphaspeed		=41;
value_channel_alphafactor		=42;

value_channel_color_magic		=43;

value_channel_soundid		=44;


value_channel_framescheme		=45;
value_channel_timerspace		=46;
value_channel_pathspeed		=47;
value_channel_framespeed		=48;

value_channel_styleex			=49;

		
value_channel_range_begin		=50;
value_channel_range_end		=51;
value_channel_color_back		=52;
value_channel_color_draw		=53;
value_channel_color_focus		=54;
value_channel_color_edge		=55;
value_channel_switch			=56;
value_channel_update_dirty		=57;		
value_channel_sort_state		=58;		
value_channel_hot			=59;

value_channel_ajust_edge		=60;		
value_channel_edge_thick		=61;
value_channel_ajust_flag		=62;
value_channel_ajust_state		=63;
value_channel_min_height		=64;
value_channel_max_height		=65;
value_channel_min_width		=66;
value_channel_max_width		=67;

value_channel_rectid			=68;

value_channel_mousedelta		=69;

value_channel_captionsize		=70;

value_channel_pathmove_pause	=73;
value_channel_pathmove_flip	=74;

value_channel_msgbox_id		=75;
value_channel_msgbox_info		=76;

value_channel_alphastate		=77;
value_channel_scalefactor		=78;
value_channel_alphamin		=79;
value_channel_alphamax		=80;

value_channel_itemcolor		=81;
value_channel_drawflag		=82;
value_channel_treeid			=83;

value_channel_align_dirty		=84;

value_channel_color_link		=85;

value_channel_scrollspace		=86;

value_channel_richmode		=87;
value_channel_richflag			=88;

value_channel_dragflag		=89;
value_channel_dropflag		=90;

value_channel_pattern			=91;

value_channel_querytype		=92;
value_channel_queryindex		=93;

value_channel_indexoffset		=94;	
	
--用于监测窗口状态变化
value_channel_moveable		=201;
value_channel_mouseover		=202;
value_channel_clickdown		=203;
value_channel_focused		=204;
value_channel_enabled		=205;
value_channel_checked		=206;
value_channel_discarded		=207;
value_channel_dragging		=208;
value_channel_visible			=209;
value_channel_actived			=210;
value_channel_editable		=211;

value_channel_captured		=212;
value_channel_childfocused		=213;
value_channel_descendfocused	=214;

value_channel_effectframe		=215;

value_channel_backpicpos		=216;
value_channel_coverpicpos		=217;
value_channel_effectpicpos		=218;
value_channel_checkcoverpos	=219;
value_channel_hotcoverpos		=220;

--自定义数值数据
value_channel_customstyle		=300;
value_channel_customstate		=301;
value_channel_customtype		=302;
value_channel_customindex		=303;
value_channel_customid		=304;		


--从此之后是文本
value_channel_caption			=400;
value_channel_seltext			=401;
value_channel_pathname		=402;
value_channel_dirname		=403;

value_channel_edithistory		=404;
value_channel_itemtext		=405;
value_channel_tiptext			=406;
value_channel_typename		=407;
value_channel_iconlist			=408;

value_channel_backpic			=409;
value_channel_coverpic		=410;
value_channel_adornpic		=411;
value_channel_patchpic		=412;
value_channel_effectpic		=413;

value_channel_scripttext		=414;

value_channel_tipwgt			=415;
value_channel_msgbox		=416;
value_channel_console			=417;
value_channel_host			=418;
value_channel_infowgt			=419;
value_channel_buddy			=420;
value_channel_primescroll		=421;
value_channel_secondscroll		=422;
value_channel_blockbutton 		=423;

--自定义文本数据
value_channel_customname		=500;
value_channel_customtext		=501;
value_channel_customstring		=502;

--从此之后是表数据
value_channel_itemdata		=600;

--UITouchFlag
ui_touch_none   	  	= 0;
ui_touch_top_out	= 1;
ui_touch_bottom_out   = 2;
ui_touch_left_out    	= 4;	
ui_touch_right_out   	= 8;

--扩展窗口形状
shape_type_rect		=0;--type,ox,oy,width,height
shape_type_circle	=1;--type,cx,cy,radius
shape_type_triangle	=2;--type,x0,y0,x1,y1,x2,y2
shape_param_max	=6;--max parmameter num

--UIAlphaState
alpha_state_decrease	= 0;--需要让窗口的透明度减少		
alpha_state_increase	= 1;--需要让窗口的透明度增加
alpha_state_pause	= 2;

	
--edit_method
ui_editmethod_english	=1;--英文
ui_editmethod_number=2;--数字
ui_editmethod_symbol	=4;--符号
ui_editmethod_chinese=8;--中文
ui_editmethod_value	=16;--数值
ui_editmethod_coder	=32;--密码

edit_style_none	      	 =0;
edit_style_keepsel   	 =1;
edit_style_keepcursor =2;
edit_style_flashcursor =4;
edit_style_smartcursor=8;

--UIPathCmd
ui_pathcmd_none	=0;
ui_pathcmd_alpha	=1;
ui_pathcmd_scale	=2;
ui_pathcmd_size		=4;
ui_pathcmd_pause	=8;
ui_pathcmd_cycle	=16;
ui_pathcmd_flip		=32;
ui_pathcmd_relative	=64;

--UIDrawFlag
drawflag_none			=0;
drawflag_win_edge		=1;
drawflag_win_fill			=2;
drawflag_client_edge		=4;
drawflag_client_fill		=8;
drawflag_caption_edge	=16;
drawflag_caption_fill		=32;
drawflag_drag_edge		=64;
drawflag_drag_fill		=128;
drawflag_enable_effect	=256;
drawflag_focus_effect		=512;
drawflag_check_effect		=1024;
drawflag_draw_effect		=2048;
drawflag_clientprior		=4096;
drawflag_focuscaption		=8192;
drawflag_hotcaption		=16384;
drawflag_backprior		=32768;


--商品主类型，用于车库显示分类，以ItemList.lua定义为准
ECBT_BASE_KART 		=1;	--赛车
ECBT_BASE_CHARACTOR	=2;	--角色
ECBT_BASE_KARTEQP	=4;	--赛车装备
ECBT_BASE_CHARACTOREQP	=8;	--角色装备
ECBT_BASE_PET		=16;	--宠物
ECBT_BASE_EFFECT	=32;	--特效
ECBT_BASE_OTHER		=64;	--其他
ECBT_BASE_HOTRECOMMAND	=128;	--人气推荐（商品专用）