
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
