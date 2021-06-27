#include "ShipEditWeaponDialog.h"

#include "ui_ShipEditWeaponDialog.h"

namespace fso {
namespace fred {
namespace dialogs {

ShipEditWeaponDialog::ShipEditWeaponDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ShipEditWeaponDialog)
{
	ui->setupUi(this);
}

ShipEditWeaponDialog::~ShipEditWeaponDialog() = default;

void ShipEditWeaponDialog::closeEvent(QCloseEvent*) {}

} // namespace dialogs
} // namespace fred
} // namespace fso
