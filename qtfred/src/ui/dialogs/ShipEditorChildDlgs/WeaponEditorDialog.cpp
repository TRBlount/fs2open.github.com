#include "WeaponEditorDialog.h"

#include "ui_WeaponEditorDialog.h"

#include <globalincs/linklist.h>
#include <ui/util/SignalBlockers.h>
#include "weapon/weapon.h"

#include <QCloseEvent>

namespace fso {
namespace fred {
namespace dialogs {

WeaponEditorDialog::WeaponEditorDialog(QWidget* parent, EditorViewport* viewport, int m_multi_edit)
	: QDialog(parent), ui(new Ui::WeaponEditorDialog), _model(new WeaponEditorDialogModel(this, viewport, m_multi_edit)),
	  _viewport(viewport)
{
	this->setFocus();
	ui->setupUi(this);
	
	connect(_model.get(), &AbstractDialogModel::modelChanged, this, &WeaponEditorDialog::updateUI);
	connect(this, &QDialog::accepted, _model.get(), &WeaponEditorDialogModel::apply);
	connect(this, &QDialog::rejected, _model.get(), &WeaponEditorDialogModel::apply);
	connect(ui->closeButton, &QPushButton::clicked, this, &WeaponEditorDialog::close);

	// ship weapon list
	connect(ui->weaponSelect, &QListWidget::currentItemChanged, this, &WeaponEditorDialog::weaponSelectChanged);

	// primary banks
	connect(ui->gunBank1Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::gunBank1Changed);
	connect(ui->gunBank2Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::gunBank2Changed);
	connect(ui->gunBank3Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::gunBank3Changed);

	// AI class
	connect(ui->aiClassCombo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::aiClassChanged);

	// secondary banks
	connect(ui->missileBank1Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::missileBank1Changed);
	connect(ui->missileBank2Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::missileBank2Changed);
	connect(ui->missileBank3Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::missileBank3Changed);
	connect(ui->missileBank4Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&WeaponEditorDialog::missileBank4Changed);
	connect(ui->missileBank1SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&WeaponEditorDialog::missileAmmo1Changed);
	connect(ui->missileBank2SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&WeaponEditorDialog::missileAmmo2Changed);
	connect(ui->missileBank3SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&WeaponEditorDialog::missileAmmo3Changed);
	connect(ui->missileBank4SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&WeaponEditorDialog::missileAmmo4Changed);

	updateUI();
}

WeaponEditorDialog::~WeaponEditorDialog() = default;

void WeaponEditorDialog::closeEvent(QCloseEvent* event) {
	_model->apply();
	QDialog::closeEvent(event);
}

void WeaponEditorDialog::updateUI()
{
	util::SignalBlockers blockers(this);

	int i, end1, end2;

	end1 = First_secondary_index;
	end2 = weapon_info_size();

	// If no item is currently selected, disable the UI elements
	if (_model->getCurItem() < 0) {
		ui->gunBank1Combo->setEnabled(false);
		ui->gunBank2Combo->setEnabled(false);
		ui->gunBank3Combo->setEnabled(false);
		ui->aiClassCombo->setEnabled(false);
		ui->missileBank1Combo->setEnabled(false);
		ui->missileBank1SpinBox->setEnabled(false);
		ui->missileBank2Combo->setEnabled(false);
		ui->missileBank2SpinBox->setEnabled(false);
		ui->missileBank3Combo->setEnabled(false);
		ui->missileBank3SpinBox->setEnabled(false);
		ui->missileBank4Combo->setEnabled(false);
		ui->missileBank4SpinBox->setEnabled(false);
		return;
	}

	auto cur_sel = _model->getCurItem();
	ui->weaponSelect->clear();
	for (i = 0; i < _model->getNumWeapons(); i++) {
		auto item = new QListWidgetItem(QString::fromStdString(_model->getName(i)), ui->weaponSelect);
		ui->weaponSelect->addItem(item);
	}
	ui->weaponSelect->setCurrentRow(cur_sel);

	ui->aiClassCombo->setEnabled(true);
	auto ai_idx = _model->getAIClass();
	ui->aiClassCombo->clear();
	for (i = 0; i < Num_ai_classes; i++) {
		ui->aiClassCombo->addItem(Ai_class_names[i], i);
	}
	ui->aiClassCombo->setCurrentIndex(ui->aiClassCombo->findData(ai_idx));

	// Process primary weapons
	for (i = 0; i < end1; i++) {
		if ((Weapon_info[i].wi_flags[Weapon::Info_Flags::Child]) ||
			(!_model->big && (Weapon_info[i].wi_flags[Weapon::Info_Flags::Big_only]))) {
			end1 = i;
		}
	}

	// Enable the primary banks
	auto gun1 = _model->getGunBank1();
	ui->gunBank1Combo->clear();
	if (_model->getNumPrimaries() > 0) {
		ui->gunBank1Combo->setEnabled(true);
		ui->gunBank1Combo->addItem("None", -1);
		for (i = 0; i < end1; i++) {
			ui->gunBank1Combo->addItem(Weapon_info[i].name, i);
		}
		ui->gunBank1Combo->setCurrentIndex(ui->gunBank1Combo->findData(gun1));
	} else {
		ui->gunBank1Combo->setEnabled(false);
	}

	auto gun2 = _model->getGunBank2();
	ui->gunBank2Combo->clear();
	if (_model->getNumPrimaries() > 1) {
		ui->gunBank2Combo->setEnabled(true);
		ui->gunBank2Combo->addItem("None", -1);
		for (i = 0; i < end1; i++) {
			ui->gunBank2Combo->addItem(Weapon_info[i].name, i);
		}
		ui->gunBank2Combo->setCurrentIndex(ui->gunBank2Combo->findData(gun2));
	} else {
		ui->gunBank2Combo->setEnabled(false);
	}

	auto gun3 = _model->getGunBank3();
	ui->gunBank3Combo->clear();
	if (_model->getNumPrimaries() > 2) {
		ui->gunBank3Combo->setEnabled(true);
		ui->gunBank3Combo->addItem("None", -1);
		for (i = 0; i < end1; i++) {
			ui->gunBank3Combo->addItem(Weapon_info[i].name, i);
		}
		ui->gunBank3Combo->setCurrentIndex(ui->gunBank3Combo->findData(gun3));
	} else {
		ui->gunBank3Combo->setEnabled(false);
	}

	// Process secondary weapons
	for (i = First_secondary_index; i < end2; i++) {
		if ((Weapon_info[i].wi_flags[Weapon::Info_Flags::Child]) ||
			(!_model->big && (Weapon_info[i].wi_flags[Weapon::Info_Flags::Big_only]))) {
			end2 = i;
		}
	}

	// Set the secondary banks
	auto missile1 = _model->getMissileBank1();
	auto ammo1 = _model->getAmmo1();
	ui->missileBank1Combo->clear();
	ui->missileBank1SpinBox->clear();
	ui->missileBank1Combo->addItem("None", -1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank1Combo->addItem(Weapon_info[i].name, i);
	}
	if (_model->getNumSecondaries() > 0) {

		if (missile1 >= 0) {
			if (_model->getCurItem() == 0) {
				_model->ammo_max1 = get_max_ammo_count_for_bank(_model->ship_class, 0, missile1);
			} else {
				_model->ammo_max1 = get_max_ammo_count_for_turret_bank(_model->getCurWeapon(), 0, missile1);
			}

			ui->missileBank1SpinBox->setRange(0, _model->ammo_max1);
			ui->missileBank1SpinBox->setValue(ammo1);

			ui->missileBank1SpinBox->setEnabled(true);
		}
		ui->missileBank1Combo->setCurrentIndex(ui->missileBank1Combo->findData(missile1));
		ui->missileBank1Combo->setEnabled(true);
	} else {
		ui->missileBank1Combo->setEnabled(false);
		ui->missileBank1SpinBox->setEnabled(false);
		ui->missileBank1Combo->setCurrentIndex(-1);
	}

	auto missile2 = _model->getMissileBank2();
	auto ammo2 = _model->getAmmo2();
	ui->missileBank2Combo->clear();
	ui->missileBank2SpinBox->clear();
	ui->missileBank2Combo->addItem("None", -1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank2Combo->addItem(Weapon_info[i].name, i);
	}
	if (_model->getNumSecondaries() > 1) {

		if (missile2 >= 0) {
			if (_model->getCurItem() == 0) {
				_model->ammo_max2 = get_max_ammo_count_for_bank(_model->ship_class, 1, missile2);
			} else {
				_model->ammo_max2 = get_max_ammo_count_for_turret_bank(_model->getCurWeapon(), 1, missile2);
			}
			ui->missileBank2SpinBox->setRange(0, _model->ammo_max2);
			ui->missileBank2SpinBox->setValue(ammo2);

			ui->missileBank2SpinBox->setEnabled(true);
		}
		ui->missileBank2Combo->setCurrentIndex(ui->missileBank2Combo->findData(missile2));
		ui->missileBank2Combo->setEnabled(true);
	} else {
		ui->missileBank2Combo->setEnabled(false);
		ui->missileBank2SpinBox->setEnabled(false);
		ui->missileBank2Combo->setCurrentIndex(-1);
	}

	auto missile3 = _model->getMissileBank3();
	auto ammo3 = _model->getAmmo3();
	ui->missileBank3Combo->clear();
	ui->missileBank3SpinBox->clear();
	ui->missileBank3Combo->addItem("None", -1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank3Combo->addItem(Weapon_info[i].name, i);
	}
	if (_model->getNumSecondaries() > 2) {

		if (missile3 >= 0) {
			if (_model->getCurItem() == 0) {
				_model->ammo_max3 = get_max_ammo_count_for_bank(_model->ship_class, 2, missile3);
			} else {
				_model->ammo_max3 = get_max_ammo_count_for_turret_bank(_model->getCurWeapon(), 2, missile3);
			}

			ui->missileBank3SpinBox->setRange(0, _model->ammo_max3);
			ui->missileBank3SpinBox->setValue(ammo3);

			ui->missileBank3SpinBox->setEnabled(true);
		}
		ui->missileBank3Combo->setCurrentIndex(ui->missileBank3Combo->findData(missile3));
		ui->missileBank3Combo->setEnabled(true);
	} else {
		ui->missileBank3Combo->setEnabled(false);
		ui->missileBank3SpinBox->setEnabled(false);
		ui->missileBank3Combo->setCurrentIndex(-1);
	}

	auto missile4 = _model->getMissileBank4();
	auto ammo4 = _model->getAmmo4();
	ui->missileBank4Combo->clear();
	ui->missileBank4SpinBox->clear();
	ui->missileBank4Combo->addItem("None", -1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank4Combo->addItem(Weapon_info[i].name, i);
	}
	if (_model->getNumSecondaries() > 3) {

		if (missile4 >= 0) {
			if (_model->getCurItem() == 0) {
				_model->ammo_max4 = get_max_ammo_count_for_bank(_model->ship_class, 3, missile4);
			} else {
				_model->ammo_max4 = get_max_ammo_count_for_turret_bank(_model->getCurWeapon(), 3, missile4);
			}

			ui->missileBank4SpinBox->setRange(0, _model->ammo_max4);
			ui->missileBank4SpinBox->setValue(ammo4);

			ui->missileBank4SpinBox->setEnabled(true);
		}
		ui->missileBank4Combo->setCurrentIndex(ui->missileBank4Combo->findData(missile4));
		ui->missileBank4Combo->setEnabled(true);
	} else {
		ui->missileBank4Combo->setEnabled(false);
		ui->missileBank4SpinBox->setEnabled(false);
		ui->missileBank4Combo->setCurrentIndex(-1);
	}

	// Copy the current item into the last item
	_model->last_item = _model->getCurItem();
}

void WeaponEditorDialog::weaponSelectChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	auto idx = current->data(Qt::UserRole).value<int>();
	_model->setCurItem(idx);
}

void WeaponEditorDialog::gunBank1Changed(int index) {
	auto widx = ui->gunBank1Combo->itemData(index).value<int>();
	_model->setGunBank1(widx);
}

void WeaponEditorDialog::gunBank2Changed(int index) {
	auto widx = ui->gunBank2Combo->itemData(index).value<int>();
	_model->setGunBank2(widx);
}

void WeaponEditorDialog::gunBank3Changed(int index) {
	auto widx = ui->gunBank3Combo->itemData(index).value<int>();
	_model->setGunBank3(widx);
}

void WeaponEditorDialog::aiClassChanged(int index) {
	auto ai_class = ui->aiClassCombo->itemData(index).value<int>();
	_model->setAIClass(ai_class);
}

void WeaponEditorDialog::missileBank1Changed(int index) {
	auto widx = ui->missileBank1Combo->itemData(index).value<int>();
	_model->setMissileBank1(widx);
}

void WeaponEditorDialog::missileBank2Changed(int index) {
	auto widx = ui->missileBank2Combo->itemData(index).value<int>();
	_model->setMissileBank2(widx);
}

void WeaponEditorDialog::missileBank3Changed(int index) {
	auto widx = ui->missileBank3Combo->itemData(index).value<int>();
	_model->setMissileBank3(widx);
}

void WeaponEditorDialog::missileBank4Changed(int index) {
	auto widx = ui->missileBank4Combo->itemData(index).value<int>();
	_model->setMissileBank4(widx);
}

void WeaponEditorDialog::missileAmmo1Changed(int count) {
	_model->setAmmo1(count);
}

void WeaponEditorDialog::missileAmmo2Changed(int count) {
	_model->setAmmo2(count);
}

void WeaponEditorDialog::missileAmmo3Changed(int count) {
	_model->setAmmo3(count);
}

void WeaponEditorDialog::missileAmmo4Changed(int count) {
	_model->setAmmo4(count);
}

} // namespace dialogs
} // namespace fred
} // namespace fso
