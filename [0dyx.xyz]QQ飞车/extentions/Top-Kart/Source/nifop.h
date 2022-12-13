#ifndef __NIFOP_H__
#define __NIFOP_H__

class CNifOp
{
public:
	static NiNode*		FindParent(NiAVObject* pkObj, const char* ParentName);
	static NiCamera*	FindCamera(NiAVObject* pkRoot);
	static NiGeometry*	FindGeometry(NiAVObject* pkRoot);
	static NiCamera*	FindCameraByName(NiNode* pkRoot, const char* pstrCameraName);
	static NiMatrix3&	AdjustRotate(NiMatrix3&);
	static void			AttachChild(NiNode* pkNode, NiAVObject* pkObj);
};
#endif