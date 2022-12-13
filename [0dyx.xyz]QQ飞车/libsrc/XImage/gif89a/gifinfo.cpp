#include "gif89a.h"
void main(int argc,char **argv)
{	LPCGLOBAL_INFO gi;
	LPCFRAME fm;
	if(argc != 2)
	{	cout<<"Usage: gifinfo filename"<<endl;
		return;
	}
	CGif89a gif(argv[1],FALSE);
	if(!gif)
	{	cout<<"find error!"<<endl;
		return;
	}
	cout<<"GIF文件信息提取器 版本1.0 作者：陈鹏"<<endl;
	cout<<"版本＝"<<gif.getVer()<<endl;
	gi = gif.getGlobalInfo();
	cout<<"逻辑屏幕宽度="<<gi->scrWidth<<endl;
	cout<<"逻辑屏幕高度="<<gi->scrHeight<<endl;
	cout<<"图象帧数="<<gi->frames<<endl;
	if(gi->gFlag)
	{	cout<<"全局调色板=有"<<endl;
		cout<<"全局调色板入口数="<<gi->gSize<<endl;
		if(gi->gSort)
			cout<<"全局调色板是否按优先排序=是"<<endl;
		else
			cout<<"全局调色板是否按优先排序=否"<<endl;
	}
	else
		cout<<"全局调色板=无"<<endl;
	for(UINT i=0;i<gi->frames;i++)
	{	fm = gif.getNextFrame();
		if(fm == NULL)
		{	cout<<"find error!"<<endl;
			return;
		}
		cout<<"第"<<i+1<<"帧"<<endl;
		cout<<"图像宽度＝"<<fm->imageWidth<<endl;
		cout<<"图像高度＝"<<fm->imageHeight<<endl;
		cout<<"图象左上角X坐标="<<fm->imageLPos<<endl;
		cout<<"图象左上角Y坐标="<<fm->imageTPos<<endl;
		if(fm->interlaceFlag)
			cout<<"数据是否交错=是"<<endl;
		else
			cout<<"数据是否交错=否"<<endl;
		if(fm->lFlag)
		{	cout<<"是否有局部调色板=有"<<endl;
			cout<<"局部调色板入口数="<<fm->lSize<<endl;
			if(fm->sortFlag)
				cout<<"局部调色板是否按优先排序=是"<<endl;
			else
				cout<<"局部调色板是否按优先排序=否"<<endl;
		}
		else
			cout<<"是否有局部调色板=无"<<endl;
		if(fm->ctrlExt.active)
		{	cout<<"处理方式="<<fm->ctrlExt.disposalMethod<<endl;
			if(fm->ctrlExt.userInputFlag)
				cout<<"是否等待用户输入=是"<<endl;
			else
				cout<<"是否等待用户输入=否"<<endl;
			cout<<"延时（毫秒）="<<fm->ctrlExt.delayTime*10<<endl;
			if(fm->ctrlExt.trsFlag)
				cout<<"是否有透明色=是"<<endl;
			else
				cout<<"是否有透明色=否"<<endl;
		}
	}
}