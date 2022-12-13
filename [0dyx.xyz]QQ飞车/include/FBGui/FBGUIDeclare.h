#pragma once

// Start of FEGUIRenderLayer namespace section
namespace FBGUI
{

namespace Sink
{

class CGUISinkWindowUpdate;
class CGUISinkWindowRender;
class CGUISinkSheetActive;
class CGUISinkWindowCreate;
class CGUISinkWindowDestory;
class CGUISinkMButtonDown;
class CGUISinkMButtonUp;
class CGUISinkRButtonDown;
class CGUISinkRButtonUp;
class CGUISinkLButtonDown;
class CGUISinkLButtonUp;
class CGUISinkMouseMove;
class CGUISinkMouseLeave;
class CGUISinkMouseEnter;
class CGUISinkWindowVisible;
class CGUISinkWindowEnable;
class CGUISinkWindowActive;
class CGUISinkWindowFocus;
class CGUISinkWindowMoved;
class CGUISinkWindowSized;
class CGUISinkWindowMoving;
class CGUISinkWindowSizing;
class CGUISinkParentMoved;
class CGUISinkParentSized;
class CGUISinkCommand;
class CGUISinkThumbChanged;
class CGUISinkListBoxSelChanged;

};

namespace Element
{

class CGUISheet;
class CGUISheetFactory;
class CGUIFrame;
class CGUIFrameFactory;
class CGUIButton;
class CGUIButtonFactory;
class CGUICheck;
class CGUICheckFactory;
class CGUIImageButton;
class CGUIImageButtonFactory;
class CGUIImageCheck;
class CGUIImageCheckFactory;
class CGUIScrollBar;
class CGUIScrollBarFactory;
class CGUIListBox;
class CGUIListBoxFactory;
class CGUIEdit;
class CGUIEditFactory;
class CGUIMenu;
class CGUIMenuFactory;
class CGUITree;
class CGUITreeFactory;
class CGUIList;
class CGUIListFactory;
};

namespace Lua
{

class CGUILuaInterface;
class CGUILuaModule;

};// End of Lua namespace section

namespace Memory
{

class CGUIMemoryManager;
class CGUIMemoryInfo;

};// End of MemoryManager namespace section

namespace Exception
{

class CGUIException;

};

// *************************************************************** //
// Forward reference declarations of all core GUI system classes
// *************************************************************** //

class CGUISize;
class CGUIPoint;
class CGUIRect;
class CGUITexture;
class CGUIRenderDriver;
class CGUIPlatform;
class CGUISystem;
class CGUIWindow;
class CGUICanvas;
class CGUIWindowCanvas;
class CGUIClientCanvas;
class CGUIWindowManager;
class CGUIWindowFactory;
class CGUIWindowFactoryManager;
class CGUICodePage;
class CGUIGB2312Page;
class CGUIGlyphData;
class CGUIFont;
class CGUIFontManager;
class CGUISink;
class CGUIEvent;
class CGUIEventSet;
class CGUIDataType;

};// End of FBGUILogicLayer namespace section


using namespace FBGUI;
using namespace FBGUI::Sink;
using namespace FBGUI::Element;
using namespace FBGUI::Lua;
using namespace FBGUI::Memory;
using namespace FBGUI::Exception;
