#include "WeaponEditorDialog.h"

#include "ui_WeaponEditorDialog.h"

#include <globalincs/linklist.h>
#include <ui/util/SignalBlockers.h>
#include "weapon/weapon.h"

#include <QCloseEvent>

namespace fso {
namespace fred {
namespace dialogs {

WeaponEditorDialog::WeaponEditorDialog(QWidget* parent, EditorViewport* viewport)
	: QDialog(parent), ui(new Ui::WeaponEditorDialog), _model(new WeaponEditorDialogModel(this, viewport)),
	  _viewport(viewport)
{
	this->setFocus();
	ui->setupUi(this);
	
	connect(_model.get(), &AbstractDialogModel::modelChanged, this, &WeaponEditorDialog::updateUI);
	connect(this, &QDialog::accepted, _model.get(), &WeaponEditorDialogModel::apply);
	connect(this, &QDialog::rejected, _model.get(), &WeaponEditorDialogModel::apply);
	connect(ui->closeButton, &QPushButton::clicked, this, &WeaponEditorDialog::close);

	// ship weapon list
	connect(ui->weaponSelect, 
		static_cast<void (QListWidget::*)(const int)>(&QListWidget::currentRowChanged), 
		this,
		&WeaponEditorDialog::weaponSelectChanged);

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

	initDialog();
}

WeaponEditorDialog::~WeaponEditorDialog() = default;

void WeaponEditorDialog::closeEvent(QCloseEvent* event) {
	update_pilot();
	_model->apply();
	QDialog::closeEvent(event);
}

void WeaponEditorDialog::initDialog()
{
	util::SignalBlockers blockers(this);

	object* ptr;
	model_subsystem* psub;
	ship_subsys *ssl, *pss;
	int inst, big = 1, i, end1, end2;

	end1 = First_secondary_index;
	end2 = weapon_info_size();

	ui->weaponSelect->setEnabled(true);
	ui->weaponSelect->clear();

	SWP_ListWidgetItem* widgetItem = new SWP_ListWidgetItem("Pilot", ui->weaponSelect);
	if (_model->multi_edit) {
		widgetItem->setUserData(&_model->pilot);
		ui->weaponSelect->addItem(widgetItem);
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)) && (ptr->flags[Object::Object_Flags::Marked])) {
				inst = ptr->instance;
				if (!(Ship_info[Ships[inst].ship_info_index].is_big_or_huge()))
					big = 0;
			}
			ptr = GET_NEXT(ptr);
		}
	} else {
		if (!(Ship_info[Ships[_model->ship].ship_info_index].is_big_or_huge()))
			big = 0;

		ssl = &Ships[_model->ship].subsys_list;
		i = 0;
		for (pss = GET_FIRST(ssl); pss != END_OF_LIST(ssl); pss = GET_NEXT(pss)) {
			psub = pss->system_info;
			if (psub->type == SUBSYSTEM_TURRET) {
				widgetItem = new SWP_ListWidgetItem(psub->subobj_name, ui->weaponSelect);
				widgetItem->setUserData(&pss->weapons);
				ui->weaponSelect->addItem(widgetItem);
			}
		}
	}

	auto ai_idx = _model->getAIClass();
	ui->aiClassCombo->clear();
	for (i = 0; i < Num_ai_classes; i++) {
		ui->aiClassCombo->addItem(Ai_class_names[i], i);
	}
	ui->aiClassCombo->setCurrentIndex(ui->aiClassCombo->findData(ai_idx));

	for (i = 0; i < end1; i++) {
		if ((Weapon_info[i].wi_flags[Weapon::Info_Flags::Child]) ||
			(!big && (Weapon_info[i].wi_flags[Weapon::Info_Flags::Big_only]))) {
			end1 = i;
		}
	}

	auto gun1 = _model->getGunBank1();
	ui->gunBank1Combo->clear();
	ui->gunBank1Combo->addItem("None",-1);
	for (i = 0; i < end1; i++) {
		ui->gunBank1Combo->addItem(Weapon_info[i].name, i);
	}
	ui->gunBank1Combo->setCurrentIndex(ui->gunBank1Combo->findData(gun1));

	auto gun2 = _model->getGunBank2();
	ui->gunBank2Combo->clear();
	ui->gunBank2Combo->addItem("None",-1);
	for (i = 0; i < end1; i++) {
		ui->gunBank2Combo->addItem(Weapon_info[i].name, i);
	}
	ui->gunBank2Combo->setCurrentIndex(ui->gunBank1Combo->findData(gun2));

	auto gun3 = _model->getGunBank3();
	ui->gunBank3Combo->clear();
	ui->gunBank3Combo->addItem("None",-1);
	for (i = 0; i < end1; i++) {
		ui->gunBank3Combo->addItem(Weapon_info[i].name, i);
	}
	ui->gunBank3Combo->setCurrentIndex(ui->gunBank3Combo->findData(gun3));

	for (i = First_secondary_index; i < end2; i++) {
		if ((Weapon_info[i].wi_flags[Weapon::Info_Flags::Child]) ||
			(!big && (Weapon_info[i].wi_flags[Weapon::Info_Flags::Big_only]))) {
			end2 = i;
		}
	}

	auto missile1 = _model->getMissileBank1();
	ui->missileBank1Combo->clear();
	ui->missileBank1Combo->addItem("None",-1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank1Combo->addItem(Weapon_info[i].name, i);
	}
	ui->missileBank1Combo->setCurrentIndex(ui->missileBank1Combo->findData(missile1));

	auto missile2 = _model->getMissileBank2();
	ui->missileBank2Combo->clear();
	ui->missileBank2Combo->addItem("None",-1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank2Combo->addItem(Weapon_info[i].name, i);
	}
	ui->missileBank2Combo->setCurrentIndex(ui->missileBank2Combo->findData(missile2));

	auto missile3 = _model->getMissileBank3();
	ui->missileBank3Combo->clear();
	ui->missileBank3Combo->addItem("None",-1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank3Combo->addItem(Weapon_info[i].name, i);
	}
	ui->missileBank3Combo->setCurrentIndex(ui->missileBank3Combo->findData(missile3));

	auto missile4 = _model->getMissileBank4();
	ui->missileBank4Combo->clear();
	ui->missileBank4Combo->addItem("None",-1);
	for (i = First_secondary_index; i < end2; i++) {
		ui->missileBank4Combo->addItem(Weapon_info[i].name, i);
	}
	ui->missileBank4Combo->setCurrentIndex(ui->missileBank4Combo->findData(missile4));

	updateUI();
}

void WeaponEditorDialog::updateUI()
{
	util::SignalBlockers blockers(this);

	int a1, a2, a3, a4;

	// Check if there was a previously loaded item
	if (_model->last_item >= 0) {
		_model->cur_weapon->ai_class = _model->getAIClass();

		_model->cur_weapon->primary_bank_weapons[0] = _model->getGunBank1();
		_model->cur_weapon->primary_bank_weapons[1] = _model->getGunBank2();
		_model->cur_weapon->primary_bank_weapons[2] = _model->getGunBank3();

		_model->cur_weapon->secondary_bank_weapons[0] = _model->getMissileBank1();
		_model->cur_weapon->secondary_bank_weapons[1] = _model->getMissileBank2();
		_model->cur_weapon->secondary_bank_weapons[2] = _model->getMissileBank3();
		_model->cur_weapon->secondary_bank_weapons[3] = _model->getMissileBank4();
		_model->cur_weapon->secondary_bank_ammo[0] = _model->ammo_max1 ? fl2ir(_model->getAmmo1() * 100.0f / _model->ammo_max1) : 0;
		_model->cur_weapon->secondary_bank_ammo[1] = _model->ammo_max2 ? fl2ir(_model->getAmmo2() * 100.0f / _model->ammo_max2) : 0;
		_model->cur_weapon->secondary_bank_ammo[2] = _model->ammo_max3 ? fl2ir(_model->getAmmo3() * 100.0f / _model->ammo_max3) : 0;
		_model->cur_weapon->secondary_bank_ammo[3] = _model->ammo_max4 ? fl2ir(_model->getAmmo4() * 100.0f / _model->ammo_max4) : 0;
		if (_model->multi_edit) {
			if (!a1)
				_model->cur_weapon->secondary_bank_ammo[0] = 0;
			if (!a2)
				_model->cur_weapon->secondary_bank_ammo[1] = 0;
			if (!a3)
				_model->cur_weapon->secondary_bank_ammo[2] = 0;
			if (!a4)
				_model->cur_weapon->secondary_bank_ammo[3] = 0;
		}
	}

	// Clear attributes
	ui->gunBank1Combo->setCurrentIndex(ui->gunBank1Combo->findData(-1));
	ui->gunBank2Combo->setCurrentIndex(ui->gunBank2Combo->findData(-1));
	ui->gunBank3Combo->setCurrentIndex(ui->gunBank3Combo->findData(-1));
	
	ui->missileBank1Combo->setCurrentIndex(ui->missileBank1Combo->findData(-1));
	ui->missileBank2Combo->setCurrentIndex(ui->missileBank2Combo->findData(-1));
	ui->missileBank3Combo->setCurrentIndex(ui->missileBank3Combo->findData(-1));
	ui->missileBank4Combo->setCurrentIndex(ui->missileBank4Combo->findData(-1));

	ui->missileBank1SpinBox->setValue(0);
	ui->missileBank2SpinBox->setValue(0);
	ui->missileBank3SpinBox->setValue(0);
	ui->missileBank4SpinBox->setValue(0);

	_model->ammo_max1 = _model->ammo_max2 = _model->ammo_max3 = _model->ammo_max4 = 0;

	// If no item is currently selected, disable the UI elements
	if (_model->getCurrItem() < 0) {
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

	ship_weapon* cur_weapon = static_cast<SWP_ListWidgetItem*>(ui->weaponSelect->item(_model->getCurrItem()))->getUserData();
	if (cur_weapon != nullptr) {
		_model->cur_weapon = cur_weapon;
	}

	ui->aiClassCombo->setEnabled(true);
	ui->aiClassCombo->setCurrentIndex(ui->aiClassCombo->findData(_model->cur_weapon->ai_class));

	// Enable the primary banks
	if (_model->cur_weapon->num_primary_banks > 0) {
		ui->gunBank1Combo->setEnabled(true);
		ui->gunBank1Combo->setCurrentIndex(ui->gunBank1Combo->findData(_model->cur_weapon->primary_bank_weapons[0]));
	} else {
		ui->gunBank1Combo->setEnabled(false);
		ui->gunBank1Combo->setCurrentIndex(ui->gunBank1Combo->findData(-1));
	}

	if (_model->cur_weapon->num_primary_banks > 1) {
		ui->gunBank2Combo->setEnabled(true);
		ui->gunBank2Combo->setCurrentIndex(ui->gunBank2Combo->findData(_model->cur_weapon->primary_bank_weapons[1]));
	} else {
		ui->gunBank2Combo->setEnabled(false);
		ui->gunBank2Combo->setCurrentIndex(ui->gunBank2Combo->findData(-1));
	}

	if (_model->cur_weapon->num_primary_banks > 2) {
		ui->gunBank3Combo->setEnabled(true);
		ui->gunBank3Combo->setCurrentIndex(ui->gunBank3Combo->findData(_model->cur_weapon->primary_bank_weapons[2]));
	} else {
		ui->gunBank3Combo->setEnabled(false);
		ui->gunBank3Combo->setCurrentIndex(ui->gunBank3Combo->findData(-1));
	}

	// Set the secondary banks
	if (_model->cur_weapon->num_secondary_banks > 0) {
		ui->missileBank1Combo->setCurrentIndex(
			ui->missileBank1Combo->findData(_model->cur_weapon->secondary_bank_weapons[0]));
		if (_model->cur_weapon->secondary_bank_weapons[0] > 0) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max1 = get_max_ammo_count_for_bank(_model->ship_class, 
					0, 
					_model->cur_weapon->secondary_bank_weapons[0]);
			} else {
				_model->ammo_max1 = get_max_ammo_count_for_turret_bank(_model->cur_weapon,
					0,
					_model->cur_weapon->secondary_bank_weapons[0]);
			}
		}

		ui->missileBank1SpinBox->setValue(_model->getAmmo1());

		ui->missileBank1Combo->setEnabled(true);
		ui->missileBank1SpinBox->setEnabled(true);
	} else {
		ui->missileBank1Combo->setEnabled(false);
		ui->missileBank1SpinBox->setEnabled(false);
	}

	if (_model->cur_weapon->num_secondary_banks > 1) {
		ui->missileBank2Combo->setCurrentIndex(
			ui->missileBank2Combo->findData(_model->cur_weapon->secondary_bank_weapons[1]));
		if (_model->cur_weapon->secondary_bank_weapons[1] > 0) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max2 = get_max_ammo_count_for_bank(_model->ship_class, 
					1, 
					_model->cur_weapon->secondary_bank_weapons[1]);
			} else {
				_model->ammo_max2 = get_max_ammo_count_for_turret_bank(_model->cur_weapon,
					1,
					_model->cur_weapon->secondary_bank_weapons[1]);
			}
		}

		ui->missileBank2Combo->setEnabled(true);
		ui->missileBank2SpinBox->setEnabled(true);
	} else {
		ui->missileBank2Combo->setEnabled(false);
		ui->missileBank2SpinBox->setEnabled(false);
	}
	ui->missileBank2SpinBox->setRange(0, _model->ammo_max2);
	ui->missileBank2SpinBox->setValue(_model->ammo_max2);

	if (_model->cur_weapon->num_secondary_banks > 2) {
		ui->missileBank3Combo->setCurrentIndex(
			ui->missileBank3Combo->findData(_model->cur_weapon->secondary_bank_weapons[2]));
		if (_model->cur_weapon->secondary_bank_weapons[2] != -1) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max3 = get_max_ammo_count_for_bank(_model->ship_class, 
					2, 
					_model->cur_weapon->secondary_bank_weapons[2]);
			} else {
				_model->ammo_max3 = get_max_ammo_count_for_turret_bank(_model->cur_weapon,
					2,
					_model->cur_weapon->secondary_bank_weapons[2]);
			}
		} 

		ui->missileBank3Combo->setEnabled(true);
		ui->missileBank3SpinBox->setEnabled(true);
	} else {
		ui->missileBank3Combo->setEnabled(false);
		ui->missileBank3SpinBox->setEnabled(false);
	}
	ui->missileBank3SpinBox->setRange(0, _model->ammo_max3);
	ui->missileBank3SpinBox->setValue(_model->ammo_max3);

	if (_model->cur_weapon->num_secondary_banks > 3) {
		ui->missileBank4Combo->setCurrentIndex(
			ui->missileBank4Combo->findData(_model->cur_weapon->secondary_bank_weapons[3]));
		if (_model->cur_weapon->secondary_bank_weapons[3] != -1) {
			if (_model->getCurrItem() == 0) {
				_model->ammo_max4 = get_max_ammo_count_for_bank(_model->ship_class, 
					3, 
					_model->cur_weapon->secondary_bank_weapons[3]);
			} else {
				_model->ammo_max4 = get_max_ammo_count_for_turret_bank(_model->cur_weapon,
					3,
					_model->cur_weapon->secondary_bank_weapons[3]);
			}
		}

		ui->missileBank4Combo->setEnabled(true);
		ui->missileBank4SpinBox->setEnabled(true);
	} else {
		ui->missileBank4Combo->setEnabled(false);
		ui->missileBank4SpinBox->setEnabled(false);
	}
	ui->missileBank4SpinBox->setRange(0, _model->ammo_max4);
	ui->missileBank4SpinBox->setValue(_model->ammo_max4);

	// Copy the current item into the last item
	_model->last_item = _model->getCurrItem();
}

void WeaponEditorDialog::update_pilot() {
	int i;
	object* ptr;
	ship_weapon* weapon;

	if (_model->multi_edit) {
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_SHIP) || (ptr->type == OBJ_START)) && (ptr->flags[Object::Object_Flags::Marked])) {
				weapon = &Ships[ptr->instance].weapons;

				if (_model->pilot.ai_class >= 0)
					weapon->ai_class = _model->pilot.ai_class;

				for (i = 0; i < MAX_SHIP_PRIMARY_BANKS; i++)
					if (_model->pilot.primary_bank_weapons[i] != -2)
						weapon->primary_bank_weapons[i] = _model->pilot.primary_bank_weapons[i];

				for (i = 0; i < MAX_SHIP_SECONDARY_BANKS; i++) {
					if (_model->pilot.secondary_bank_weapons[i] != -2)
						weapon->secondary_bank_weapons[i] = _model->pilot.secondary_bank_weapons[i];
					if (_model->pilot.secondary_bank_ammo[i] >= 0)
						weapon->secondary_bank_ammo[i] = _model->pilot.secondary_bank_ammo[i];
				}
			}

			ptr = GET_NEXT(ptr);
		}
	}
}

void WeaponEditorDialog::weaponSelectChanged(int row) {
	ui->weaponSelect->setCurrentItem(ui->weaponSelect->item(row));
	_model->setCurrItem(row);
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
	if (_model->getMissileBank1() == -1) {
		_model->ammo_max1 = 0;
	} else {
		if (_model->getCurrItem() == 0) {
			_model->ammo_max1 = get_max_ammo_count_for_bank(_model->ship_class,
				0,
				_model->getMissileBank1());
		} else {
			_model->ammo_max1 = get_max_ammo_count_for_turret_bank(_model->cur_weapon,
				0,
				_model->getMissileBank1());
		}
	}
	_model->setAmmo1(_model->ammo_max1);
}

void WeaponEditorDialog::missileBank2Changed(int index) {
	auto widx = ui->missileBank2Combo->itemData(index).value<int>();
	_model->setMissileBank2(widx);
	if (_model->getMissileBank2() == -1) {
		_model->ammo_max2 = 0;
	} else {
		if (_model->getCurrItem() == 0) {
			_model->ammo_max2 = get_max_ammo_count_for_bank(_model->ship_class, 1, _model->getMissileBank2());
		} else {
			_model->ammo_max2 = get_max_ammo_count_for_turret_bank(_model->cur_weapon, 1, _model->getMissileBank2());
		}
	}
	_model->setAmmo2(_model->ammo_max2);
}

void WeaponEditorDialog::missileBank3Changed(int index) {
	auto widx = ui->missileBank3Combo->itemData(index).value<int>();
	_model->setMissileBank3(widx);
	if (_model->getMissileBank3() == -1) {
		_model->ammo_max3 = 0;
	} else {
		if (_model->getCurrItem() == 0) {
			_model->ammo_max3 = get_max_ammo_count_for_bank(_model->ship_class, 2, _model->getMissileBank3());
		} else {
			_model->ammo_max3 = get_max_ammo_count_for_turret_bank(_model->cur_weapon, 2, _model->getMissileBank3());
		}
	}
	_model->setAmmo3(_model->ammo_max3);
}

void WeaponEditorDialog::missileBank4Changed(int index) {
	auto widx = ui->missileBank4Combo->itemData(index).value<int>();
	_model->setMissileBank4(widx);
	if (_model->getMissileBank4() == -1) {
		_model->ammo_max4 = 0;
	} else {
		if (_model->getCurrItem() == 0) {
			_model->ammo_max4 = get_max_ammo_count_for_bank(_model->ship_class, 3, _model->getMissileBank4());
		} else {
			_model->ammo_max4 = get_max_ammo_count_for_turret_bank(_model->cur_weapon, 3, _model->getMissileBank4());
		}
	}
	_model->setAmmo4(_model->ammo_max4);
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

SWP_ListWidgetItem::SWP_ListWidgetItem(QListWidget* parent) : QListWidgetItem(parent, QListWidgetItem::UserType)
{
	userData = nullptr;
}

SWP_ListWidgetItem::SWP_ListWidgetItem(const QString& text, QListWidget* parent)
	: QListWidgetItem(text, parent, QListWidgetItem::UserType)
{
	userData = nullptr;
}

ship_weapon* SWP_ListWidgetItem::getUserData() const
{
	return userData;
}

void SWP_ListWidgetItem::setUserData(ship_weapon* data)
{
	this->userData = data;
}

} // namespace dialogs
} // namespace fred
} // namespace fso
