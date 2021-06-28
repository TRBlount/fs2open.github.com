#include "ShipEditWeaponDialog.h"

#include "ui_ShipEditWeaponDialog.h"

#include <globalincs/linklist.h>
#include <ui/util/SignalBlockers.h>
#include "weapon/weapon.h"

#include <ui/widgets/SCP_ListWidgetItem.h>

#include <QCloseEvent>

#define BLANK_FIELD -99

namespace fso {
namespace fred {
namespace dialogs {

ShipEditWeaponDialog::ShipEditWeaponDialog(QWidget* parent, EditorViewport* viewport)
	: QDialog(parent), ui(new Ui::ShipEditWeaponDialog), _model(new ShipEditWeaponDialogModel(this, viewport)),
	  _viewport(viewport)
{
	this->setFocus();
	ui->setupUi(this);
	
	connect(_model.get(), &AbstractDialogModel::modelChanged, this, &ShipEditWeaponDialog::updateUI);
	connect(this, &QDialog::accepted, _model.get(), &ShipEditWeaponDialogModel::apply);

	// ship weapon list
	connect(ui->weaponSelect, 
		static_cast<void (QListWidget::*)(const int)>(&QListWidget::currentRowChanged), 
		this,
		&ShipEditWeaponDialog::weaponSelectChanged);

	// primary banks
	connect(ui->gunBank1Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::gunBank1Changed);
	connect(ui->gunBank2Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::gunBank2Changed);
	connect(ui->gunBank3Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::gunBank3Changed);

	// AI class
	connect(ui->aiClassCombo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::aiClassChanged);

	// secondary banks
	connect(ui->missileBank1Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::missileBank1Changed);
	connect(ui->missileBank2Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::missileBank2Changed);
	connect(ui->missileBank3Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::missileBank3Changed);
	connect(ui->missileBank4Combo,
		static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged),
		this,
		&ShipEditWeaponDialog::missileBank4Changed);
	connect(ui->missileBank1SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&ShipEditWeaponDialog::missileAmmo1Changed);
	connect(ui->missileBank2SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&ShipEditWeaponDialog::missileAmmo2Changed);
	connect(ui->missileBank3SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&ShipEditWeaponDialog::missileAmmo3Changed);
	connect(ui->missileBank4SpinBox,
		static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged),
		this,
		&ShipEditWeaponDialog::missileAmmo4Changed);

	initDialog();
}

ShipEditWeaponDialog::~ShipEditWeaponDialog() = default;

void ShipEditWeaponDialog::closeEvent(QCloseEvent* event) {
	_model->apply();
	QDialog::closeEvent(event);
}

void ShipEditWeaponDialog::initDialog()
{
	object* ptr;
	model_subsystem* psub;
	ship_subsys *ssl, *pss;
	int inst, big = 1, flag = 0, i, end1, end2;

	ui->weaponSelect->clear();

	end1 = First_secondary_index;
	end2 = weapon_info_size();

	SCP_ListWidgetItem<ship_weapon*> *widgetItem = new SCP_ListWidgetItem<ship_weapon*>("Pilot", ui->weaponSelect);
	if (_model->multi_edit) {
		widgetItem->setUserData(&_model->pilot);
		ui->weaponSelect->addItem(widgetItem);
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)) && (ptr->flags[Object::Object_Flags::Marked])) {
				inst = ptr->instance;
				if (!(Ship_info[Ships[inst].ship_info_index].is_big_or_huge()))
					big = 0;

				if (!flag) {
					_model->pilot = Ships[inst].weapons;
					_model->ship_class = Ships[inst].ship_info_index;
					flag = 1;

				} else {
					Assert(Ships[inst].ship_info_index == _model->ship_class);
					if (_model->pilot.ai_class != Ships[inst].weapons.ai_class)
						_model->pilot.ai_class = BLANK_FIELD;

					for (i = 0; i < MAX_SHIP_PRIMARY_BANKS; i++)
						if (_model->pilot.primary_bank_weapons[i] != Ships[inst].weapons.primary_bank_weapons[i])
							_model->pilot.primary_bank_weapons[i] = BLANK_FIELD;

					for (i = 0; i < MAX_SHIP_SECONDARY_BANKS; i++) {
						if (_model->pilot.secondary_bank_weapons[i] != Ships[inst].weapons.secondary_bank_weapons[i])
							_model->pilot.secondary_bank_weapons[i] = -1;
						if (_model->pilot.secondary_bank_ammo[i] != Ships[inst].weapons.secondary_bank_ammo[i])
							_model->pilot.secondary_bank_ammo[i] = BLANK_FIELD;
					}
				}
			}
			ptr = GET_NEXT(ptr);
		}
	} else {
		if (!(Ship_info[Ships[_model->ship].ship_info_index].is_big_or_huge()))
			big = 0;

		_model->ship_class = Ships[_model->ship].ship_info_index;

		ssl = &Ships[_model->ship].subsys_list;
		for (pss = GET_FIRST(ssl); pss != END_OF_LIST(ssl); pss = GET_NEXT(pss)) {
			psub = pss->system_info;
			if (psub->type == SUBSYSTEM_TURRET) {
				widgetItem = new SCP_ListWidgetItem<ship_weapon*>(psub->subobj_name, ui->weaponSelect);
				widgetItem->setUserData(&pss->weapons);
				ui->weaponSelect->addItem(widgetItem);
			}
		}
	}

	ui->aiClassCombo->clear();
	for (i = 0; i < Num_ai_classes; i++) {
		ui->aiClassCombo->addItem(Ai_class_names[i]);
	}

	for (i = 0; i < end1; i++) {
		if ((Weapon_info[i].wi_flags[Weapon::Info_Flags::Child]) ||
			(!big && (Weapon_info[i].wi_flags[Weapon::Info_Flags::Big_only]))) {
			end1 = i;
		}
	}

	ui->gunBank1Combo->clear();
	ui->gunBank1Combo->addItem("None");
	for (i = 0; i < end1; i++) {
		ui->gunBank1Combo->addItem(Weapon_info[i].name);
	}

	ui->gunBank2Combo->clear();
	ui->gunBank2Combo->addItem("None");
	for (i = 0; i < end1; i++) {
		ui->gunBank2Combo->addItem(Weapon_info[i].name);
	}

	ui->gunBank3Combo->clear();
	ui->gunBank3Combo->addItem("None");
	for (i = 0; i < end1; i++) {
		ui->gunBank3Combo->addItem(Weapon_info[i].name);
	}

	for (i = First_secondary_index; i < end2; i++) {
		if ((Weapon_info[i].wi_flags[Weapon::Info_Flags::Child]) ||
			(!big && (Weapon_info[i].wi_flags[Weapon::Info_Flags::Big_only]))) {
			end2 = i;
		}
	}

	ui->missileBank1Combo->clear();
	ui->missileBank1Combo->addItem("None");
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank1Combo->addItem(Weapon_info[i].name);
	}

	ui->missileBank2Combo->clear();
	ui->missileBank2Combo->addItem("None");
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank2Combo->addItem(Weapon_info[i].name);
	}

	ui->missileBank3Combo->clear();
	ui->missileBank3Combo->addItem("None");
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank3Combo->addItem(Weapon_info[i].name);
	}

	ui->missileBank4Combo->clear();
	ui->missileBank4Combo->addItem("None");
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank4Combo->addItem(Weapon_info[i].name);
	}

	_model->setCurrItem(0);
	change_selection();
}

void ShipEditWeaponDialog::updateUI()
{
	util::SignalBlockers blockers(this);
	updateWeaponList();
	updatePrimaries();
	updateAIClass();
	updateSecondaries();
}

void ShipEditWeaponDialog::updateWeaponList() {
	
}

void ShipEditWeaponDialog::updatePrimaries() {}

void ShipEditWeaponDialog::updateAIClass() {}

void ShipEditWeaponDialog::updateSecondaries() {}

void ShipEditWeaponDialog::change_selection() {
	int a1, a2, a3, a4;

	a1 = ui->missileBank1SpinBox->text().length();
	a2 = ui->missileBank1SpinBox->text().length();
	a3 = ui->missileBank1SpinBox->text().length();
	a4 = ui->missileBank1SpinBox->text().length();

	// Check if there was a previously loaded item
	if (_model->last_item >= 0) {
		_model->cur_weapon->ai_class = _model->getAIClass();
		_model->cur_weapon->primary_bank_weapons[0] = _model->getGunBank1() - 1;
		_model->cur_weapon->primary_bank_weapons[1] = _model->getGunBank2() - 1;
		_model->cur_weapon->primary_bank_weapons[2] = _model->getGunBank3() - 1;
		int missile = _model->getMissileBank1();
		if (missile > 0)
			_model->setMissileBank1(missile + First_secondary_index);

		_model->cur_weapon->secondary_bank_weapons[0] = _model->getMissileBank1() - 1;
		missile = _model->getMissileBank2();
		if (missile > 0)
			_model->setMissileBank2(missile + First_secondary_index);

		_model->cur_weapon->secondary_bank_weapons[1] = _model->getMissileBank2() - 1;
		missile = _model->getMissileBank3();
		if (missile > 0)
			_model->setMissileBank3(missile + First_secondary_index);

		_model->cur_weapon->secondary_bank_weapons[2] = _model->getMissileBank3() - 1;
		missile = _model->getMissileBank4();
		if (missile > 0)
			_model->setMissileBank4(missile + First_secondary_index);

		_model->cur_weapon->secondary_bank_weapons[3] = _model->getMissileBank4() - 1;
		_model->cur_weapon->secondary_bank_ammo[0] = _model->ammo_max1 ? fl2ir(_model->getAmmo1() * 100.0f / _model->ammo_max1) : 0;
		_model->cur_weapon->secondary_bank_ammo[1] = _model->ammo_max2 ? fl2ir(_model->getAmmo2() * 100.0f / _model->ammo_max2) : 0;
		_model->cur_weapon->secondary_bank_ammo[2] = _model->ammo_max3 ? fl2ir(_model->getAmmo3() * 100.0f / _model->ammo_max3) : 0;
		_model->cur_weapon->secondary_bank_ammo[3] = _model->ammo_max4 ? fl2ir(_model->getAmmo4() * 100.0f / _model->ammo_max4) : 0;
		if (_model->multi_edit) {
			if (!a1)
				_model->cur_weapon->secondary_bank_ammo[0] = BLANK_FIELD;
			if (!a2)
				_model->cur_weapon->secondary_bank_ammo[1] = BLANK_FIELD;
			if (!a3)
				_model->cur_weapon->secondary_bank_ammo[2] = BLANK_FIELD;
			if (!a4)
				_model->cur_weapon->secondary_bank_ammo[3] = BLANK_FIELD;
		}
	}

	// Clear model attributes
	_model->setGunBank1(-1);
	_model->setGunBank2(-1);
	_model->setGunBank3(-1);
	_model->setMissileBank1(-1);
	_model->setMissileBank2(-1);
	_model->setMissileBank3(-1);
	_model->setMissileBank4(-1);
	_model->setAmmo1(BLANK_FIELD);
	_model->setAmmo2(BLANK_FIELD);
	_model->setAmmo3(BLANK_FIELD);
	_model->setAmmo4(BLANK_FIELD);
	_model->ammo_max1 = _model->ammo_max2 = _model->ammo_max3 = _model->ammo_max4 = 0;

	// If no item is currently selected, disable the UI elements
	if (_model->getCurrItem() < 0) {
		ui->gunBank1Combo->setDisabled(true);
		ui->gunBank2Combo->setDisabled(true);
		ui->gunBank3Combo->setDisabled(true);
		ui->aiClassCombo->setDisabled(true);
		ui->missileBank1Combo->setDisabled(true);
		ui->missileBank1SpinBox->setDisabled(true);
		ui->missileBank2Combo->setDisabled(true);
		ui->missileBank2SpinBox->setDisabled(true);
		ui->missileBank3Combo->setDisabled(true);
		ui->missileBank3SpinBox->setDisabled(true);
		ui->missileBank4Combo->setDisabled(true);
		ui->missileBank4SpinBox->setDisabled(true);
		return;
	}

	// Set the correct ai class
	_model->cur_weapon = static_cast<SCP_ListWidgetItem<ship_weapon*>*>(ui->weaponSelect->currentItem())->getUserData();
	_model->setAIClass(_model->cur_weapon->ai_class);

	// Set the primary banks
	if (_model->cur_weapon->num_primary_banks > 0) {
		_model->setGunBank1(_model->cur_weapon->primary_bank_weapons[0] + 1);
		ui->gunBank1Combo->setEnabled(true);
	} else
		ui->gunBank1Combo->setDisabled(true);

	if (_model->cur_weapon->num_primary_banks > 1) {
		_model->setGunBank2(_model->cur_weapon->primary_bank_weapons[1] + 1);
		ui->gunBank2Combo->setEnabled(true);
	} else
		ui->gunBank2Combo->setDisabled(true);

	if (_model->cur_weapon->num_primary_banks > 2) {
		_model->setGunBank3(_model->cur_weapon->primary_bank_weapons[2] + 1);
		ui->gunBank3Combo->setEnabled(true);
	} else
		ui->gunBank3Combo->setDisabled(true);

	// Set the secondary banks
	if (_model->cur_weapon->num_secondary_banks > 0) {
		_model->setMissileBank1(_model->cur_weapon->secondary_bank_weapons[0] + 1);
		if (_model->getMissileBank1() > 0) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max1 = get_max_ammo_count_for_bank(_model->ship_class, 0, _model->getMissileBank1() - 1);
			} else {
				_model->ammo_max1 =
					get_max_ammo_count_for_turret_bank(_model->cur_weapon, 0, _model->getMissileBank1() - 1);
			}
			if (_model->cur_weapon->secondary_bank_ammo[0] != BLANK_FIELD)
				_model->setAmmo1(fl2ir(_model->cur_weapon->secondary_bank_ammo[0] * _model->ammo_max1 / 100.0f));
			_model->setMissileBank1(_model->getMissileBank1() - First_secondary_index);
		}

		ui->missileBank1Combo->setEnabled(true);
		ui->missileBank1SpinBox->setEnabled(true);
		ui->missileBank1SpinBox->setRange(0, _model->ammo_max1);
	} else {
		ui->missileBank1Combo->setDisabled(true);
		ui->missileBank1SpinBox->setDisabled(true);
	}

	if (_model->cur_weapon->num_secondary_banks > 1) {
		_model->setMissileBank2(_model->cur_weapon->secondary_bank_weapons[1] + 1);
		if (_model->getMissileBank2() > 0) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max2 = get_max_ammo_count_for_bank(_model->ship_class, 1, _model->getMissileBank2() - 1);
			} else {
				_model->ammo_max2 =
					get_max_ammo_count_for_turret_bank(_model->cur_weapon, 1, _model->getMissileBank2() - 1);
			}
			if (_model->cur_weapon->secondary_bank_ammo[1] != BLANK_FIELD)
				_model->setAmmo2(fl2ir(_model->cur_weapon->secondary_bank_ammo[1] * _model->ammo_max2 / 100.0f));
			_model->setMissileBank2(_model->getMissileBank2() - First_secondary_index);
		}

		ui->missileBank2Combo->setEnabled(true);
		ui->missileBank2SpinBox->setEnabled(true);
		ui->missileBank2SpinBox->setRange(0, _model->ammo_max2);
	} else {
		ui->missileBank2Combo->setDisabled(true);
		ui->missileBank2SpinBox->setDisabled(true);
	}

	if (_model->cur_weapon->num_secondary_banks > 2) {
		_model->setMissileBank3(_model->cur_weapon->secondary_bank_weapons[2] + 1);
		if (_model->getMissileBank3() > 0) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max3 = get_max_ammo_count_for_bank(_model->ship_class, 2, _model->getMissileBank3() - 1);
			} else {
				_model->ammo_max3 =
					get_max_ammo_count_for_turret_bank(_model->cur_weapon, 2, _model->getMissileBank3() - 1);
			}
			if (_model->cur_weapon->secondary_bank_ammo[2] != BLANK_FIELD)
				_model->setAmmo3(fl2ir(_model->cur_weapon->secondary_bank_ammo[2] * _model->ammo_max3 / 100.0f));
			_model->setMissileBank3(_model->getMissileBank3() - First_secondary_index);
		}

		ui->missileBank3Combo->setEnabled(true);
		ui->missileBank3SpinBox->setEnabled(true);
		ui->missileBank3SpinBox->setRange(0, _model->ammo_max3);
	} else {
		ui->missileBank3Combo->setDisabled(true);
		ui->missileBank3SpinBox->setDisabled(true);
	}

	if (_model->cur_weapon->num_secondary_banks > 3) {
		_model->setMissileBank4(_model->cur_weapon->secondary_bank_weapons[3] + 1);
		if (_model->getMissileBank4() > 0) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max4 = get_max_ammo_count_for_bank(_model->ship_class, 3, _model->getMissileBank4() - 1);
			} else {
				_model->ammo_max4 =
					get_max_ammo_count_for_turret_bank(_model->cur_weapon, 3, _model->getMissileBank4() - 1);
			}
			if (_model->cur_weapon->secondary_bank_ammo[3] != BLANK_FIELD)
				_model->setAmmo4(fl2ir(_model->cur_weapon->secondary_bank_ammo[3] * _model->ammo_max4 / 100.0f));
			_model->setMissileBank4(_model->getMissileBank4() - First_secondary_index);
		}

		ui->missileBank4Combo->setEnabled(true);
		ui->missileBank4SpinBox->setEnabled(true);
		ui->missileBank4SpinBox->setRange(0, _model->ammo_max4);
	} else {
		ui->missileBank4Combo->setDisabled(true);
		ui->missileBank4SpinBox->setDisabled(true);
	}

	// Copy the current item into the last item
	_model->last_item = _model->getCurrItem();
	if (_model->multi_edit) {
		if (_model->getAmmo1() == BLANK_FIELD)
			ui->missileBank1SpinBox->setSpecialValueText("");
		if (_model->getAmmo2() == BLANK_FIELD)
			ui->missileBank2SpinBox->setSpecialValueText("");
		if (_model->getAmmo3() == BLANK_FIELD)
			ui->missileBank3SpinBox->setSpecialValueText("");
		if (_model->getAmmo4() == BLANK_FIELD)
			ui->missileBank4SpinBox->setSpecialValueText("");
	}
}

void ShipEditWeaponDialog::weaponSelectChanged(int index) {
	_model->setCurrItem(index);
	change_selection();
}

void ShipEditWeaponDialog::gunBank1Changed(int index) {
	
}

void ShipEditWeaponDialog::gunBank2Changed(int index) {}

void ShipEditWeaponDialog::gunBank3Changed(int index) {}

void ShipEditWeaponDialog::aiClassChanged(int index) {}

void ShipEditWeaponDialog::missileBank1Changed(int index) {
	_model->setMissileBank1(index);
	if (_model->getMissileBank1() == 0) {
		_model->ammo_max1 = 0;
	} else {
		if (_model->getCurrItem() == 0) {
			_model->ammo_max1 = get_max_ammo_count_for_bank(_model->ship_class,
				0,
				_model->getMissileBank1() + First_secondary_index - 1);
		} else {
			_model->ammo_max1 = get_max_ammo_count_for_turret_bank(_model->cur_weapon,
				0,
				_model->getMissileBank1() + First_secondary_index - 1);
		}
	}
	_model->setAmmo1(_model->ammo_max1);
}

void ShipEditWeaponDialog::missileBank2Changed(int index) {}

void ShipEditWeaponDialog::missileBank3Changed(int index) {}

void ShipEditWeaponDialog::missileBank4Changed(int index) {}

void ShipEditWeaponDialog::missileAmmo1Changed(int count) {}

void ShipEditWeaponDialog::missileAmmo2Changed(int count) {}

void ShipEditWeaponDialog::missileAmmo3Changed(int count) {}

void ShipEditWeaponDialog::missileAmmo4Changed(int count) {}

} // namespace dialogs
} // namespace fred
} // namespace fso
