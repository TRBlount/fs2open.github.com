
#include "ShipEditWeaponDialogModel.h"

#include "ui/dialogs/ShipEditorChildDlgs/ShipEditWeaponDialog.h"

#include <globalincs/linklist.h>
#include <mission/object.h>

#include <QtWidgets>

namespace fso {
namespace fred {
namespace dialogs {

ShipEditWeaponDialogModel::ShipEditWeaponDialogModel(QObject* parent, EditorViewport* viewport) 
	: AbstractDialogModel(parent, viewport)
{
	initializeData();
}

bool ShipEditWeaponDialogModel::apply()
{
	return false;
}

void ShipEditWeaponDialogModel::reject() {}

void ShipEditWeaponDialogModel::setCurrItem(int) {}

int ShipEditWeaponDialogModel::getCurrItem()
{
	return 0;
}

void ShipEditWeaponDialogModel::setGunBank1(int) {}

int ShipEditWeaponDialogModel::getGunBank1()
{
	return 0;
}

void ShipEditWeaponDialogModel::setGunBank2(int) {}

int ShipEditWeaponDialogModel::getGunBank2()
{
	return 0;
}

void ShipEditWeaponDialogModel::setGunBank3(int) {}

int ShipEditWeaponDialogModel::getGunBank3()
{
	return 0;
}

void ShipEditWeaponDialogModel::setAIClass(int) {}

int ShipEditWeaponDialogModel::getAIClass()
{
	return 0;
}

void ShipEditWeaponDialogModel::setMissileBank1(int) {}

int ShipEditWeaponDialogModel::getMissileBank1()
{
	return 0;
}

void ShipEditWeaponDialogModel::setMissileBank2(int) {}

int ShipEditWeaponDialogModel::getMissileBank2()
{
	return 0;
}

void ShipEditWeaponDialogModel::setMissileBank3(int) {}

int ShipEditWeaponDialogModel::getMissileBank3()
{
	return 0;
}

void ShipEditWeaponDialogModel::setMissileBank4(int) {}

int ShipEditWeaponDialogModel::getMissileBank4()
{
	return 0;
}

void ShipEditWeaponDialogModel::setAmmo1(int) {}

int ShipEditWeaponDialogModel::getAmmo1()
{
	return 0;
}

void ShipEditWeaponDialogModel::setAmmo2(int) {}

int ShipEditWeaponDialogModel::getAmmo2()
{
	return 0;
}

void ShipEditWeaponDialogModel::setAmmo3(int) {}

int ShipEditWeaponDialogModel::getAmmo3()
{
	return 0;
}

void ShipEditWeaponDialogModel::setAmmo4(int) {}

int ShipEditWeaponDialogModel::getAmmo4()
{
	return 0;
}

void ShipEditWeaponDialogModel::initializeData() {

}

void ShipEditWeaponDialogModel::set_modified()
{
	if (!_modified) {
		_modified = true;
	}
}

}
}
}
