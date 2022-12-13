#include "NxAllVehicles.h"

std::set<NxVehicle*>	NxAllVehicles::_allVehicles;
NxArray<NxVehicle*>		NxAllVehicles::_allVehiclesSequential;
std::set<NxVehicle*>	NxAllVehicles::_allChildVehicles;
NxI32					NxAllVehicles::_activeVehicle = -1;
NxVehicle*				NxAllVehicles::_activeVehicleP;

void NxAllVehicles::AddVehicle(NxVehicle* v) {
	if (!isVehicle(v)) {
		_allVehicles.insert(v);
		_allVehiclesSequential.pushBack(v);
	}
}

void NxAllVehicles::AddChildVehicle(NxVehicle* v) {
	if (!isVehicle(v)) {
		_allChildVehicles.insert(v);
	}
}
/*
void NxAllVehicles::RemoveVehicle(NxVehicle* v) {
	if (isVehicle(v)) {
		_allVehicles.erase(v);
		NxU32 i = 0;
		for (; i < _allVehiclesSequential.size(); i++)
			if (_allVehiclesSequential[i] == v)
				break;
		_allVehiclesSequential.replaceWithLast(i);
	} else {
		printf("Vehicle not in List\n");
	}
}
*/
void NxAllVehicles::updateAllVehicles(NxReal lastTimeStep) {
	std::set<NxVehicle*>::iterator it = _allVehicles.begin();
	for (;it != _allVehicles.end(); ++it) {
		(*it)->updateVehicle(lastTimeStep);
	}
	for (it = _allChildVehicles.begin(); it != _allChildVehicles.end(); ++it) {
		(*it)->updateVehicle(lastTimeStep);
	}
	//printf("\n");
}

bool NxAllVehicles::isVehicle(NxVehicle* v) {
	std::set<NxVehicle*>::iterator it = _allVehicles.find(v);
	if (it != _allVehicles.end())
		return true;

	it = _allChildVehicles.find(v);
	return it != _allChildVehicles.end();
}

void NxAllVehicles::setActiveVehicleP() {
	if (_activeVehicle < 0) {
		_activeVehicleP = NULL;
		return;
	}
	_activeVehicleP = _allVehiclesSequential[_activeVehicle];
}

void NxAllVehicles::drawVehicles(bool debug) {
	for (NxU32 i = 0; i < _allVehiclesSequential.size(); i++) {
		_allVehiclesSequential[i]->draw(debug);
	}
	std::set<NxVehicle*>::iterator it = _allVehicles.begin();
	for (it = _allChildVehicles.begin(); it != _allChildVehicles.end(); ++it) {
		(*it)->draw(debug);
	}
}

void NxAllVehicles::selectNext() {
	_activeVehicle++;
	if (_activeVehicle >= (NxI32)_allVehicles.size())
		_activeVehicle = -1;
	setActiveVehicleP();
}

void NxAllVehicles::handlePair(NxContactPair& pair, NxU32 events) {
	if (isVehicle((NxVehicle*)pair.actors[0]->userData)) {
		NxVehicle* v = (NxVehicle*)pair.actors[0]->userData;
		v->handleContactPair(pair, 0);
	}
	if (isVehicle((NxVehicle*)pair.actors[1]->userData)) {
		NxVehicle* v = (NxVehicle*)pair.actors[1]->userData;
		v->handleContactPair(pair, 1);
	}
}
