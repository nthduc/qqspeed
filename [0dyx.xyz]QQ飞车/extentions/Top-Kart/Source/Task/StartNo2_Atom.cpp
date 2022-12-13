#include "Top-KartPCH.h"
#include "StartNo2_Atom.h"
#include "../NO2Controller.h"

CStartNo2_Atom::CStartNo2_Atom()
{

}

CStartNo2_Atom::~CStartNo2_Atom()
{

}


void CStartNo2_Atom::Start()
{
	CALL(CNO2Controller::Get(), ActivateNO2());
}