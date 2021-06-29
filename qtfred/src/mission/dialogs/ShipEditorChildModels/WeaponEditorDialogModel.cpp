
#include "WeaponEditorDialogModel.h"

#include "ui/dialogs/ShipEditorChildDlgs/WeaponEditorDialog.h"

#include <globalincs/linklist.h>
#include <mission/object.h>

#include <QtWidgets>

namespace fso {
namespace fred {
namespace dialogs {

WeaponEditorDialogModel::WeaponEditorDialogModel(QObject* parent, EditorViewport* viewport, int multi_edit)
	: AbstractDialogModel(parent, viewport), multi_edit(multi_edit)
{
	initializeData();
}

bool WeaponEditorDialogModel::apply()
{
	update_data();
	_editor->missionChanged();
	initializeData();
	return true;
}

void WeaponEditorDialogModel::reject() {}

void WeaponEditorDialogModel::setCurItem(int m_cur_item) {

	modify(_m_cur_item, m_cur_item);
}

int WeaponEditorDialogModel::getCurItem()
{
	return _m_cur_item;
}

SCP_string WeaponEditorDialogModel::getName(int idx)
{
	return _m_names.at(idx);
}

ship_weapon* WeaponEditorDialogModel::getCurWeapon()
{
	if (_m_cur_item >= 0) {
		return &_m_weapons.at(_m_cur_item);
	}
	return nullptr;
}

int WeaponEditorDialogModel::getNumWeapons()
{
	return int(_m_weapons.size());
}

int WeaponEditorDialogModel::getNumPrimaries()
{
	return _m_weapons.at(_m_cur_item).num_primary_banks;
}

int WeaponEditorDialogModel::getNumSecondaries()
{
	return _m_weapons.at(_m_cur_item).num_secondary_banks;
}

void WeaponEditorDialogModel::setGunBank1(int m_gun1)
{
	modify(_m_weapons.at(_m_cur_item).primary_bank_weapons[0], m_gun1);
}

int WeaponEditorDialogModel::getGunBank1()
{
	return _m_weapons.at(_m_cur_item).primary_bank_weapons[0];
}

void WeaponEditorDialogModel::setGunBank2(int m_gun2) {
	modify(_m_weapons.at(_m_cur_item).primary_bank_weapons[1], m_gun2);
}

int WeaponEditorDialogModel::getGunBank2()
{
	return _m_weapons.at(_m_cur_item).primary_bank_weapons[1];
}

void WeaponEditorDialogModel::setGunBank3(int m_gun3) {
	modify(_m_weapons.at(_m_cur_item).primary_bank_weapons[2], m_gun3);
}

int WeaponEditorDialogModel::getGunBank3()
{
	return _m_weapons.at(_m_cur_item).primary_bank_weapons[2];
}

void WeaponEditorDialogModel::setAIClass(int m_ai_class) {
	modify(_m_weapons.at(_m_cur_item).ai_class, m_ai_class);
}

int WeaponEditorDialogModel::getAIClass()
{
	return _m_weapons.at(_m_cur_item).ai_class;
}

void WeaponEditorDialogModel::setMissileBank1(int m_missile1)
{
	if (getMissileBank1() != m_missile1) {
		if (getCurItem() == 0) {
			ammo_max1 = get_max_ammo_count_for_bank(ship_class, 0, m_missile1);
		} else {
			ammo_max1 = get_max_ammo_count_for_turret_bank(getCurWeapon(), 0, m_missile1);
		}
		getCurWeapon()->secondary_bank_ammo[0] = ammo_max1;
	}
	modify(_m_weapons.at(_m_cur_item).secondary_bank_weapons[0], m_missile1);
}

int WeaponEditorDialogModel::getMissileBank1()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_weapons[0];
}

void WeaponEditorDialogModel::setMissileBank2(int m_missile2)
{
	if (getMissileBank2() != m_missile2) {
		if (getCurItem() == 0) {
			ammo_max2 = get_max_ammo_count_for_bank(ship_class, 1, m_missile2);
		} else {
			ammo_max2 = get_max_ammo_count_for_turret_bank(getCurWeapon(), 1, m_missile2);
		}
		getCurWeapon()->secondary_bank_ammo[2] = ammo_max2;
	}
	modify(_m_weapons.at(_m_cur_item).secondary_bank_weapons[1], m_missile2);
}

int WeaponEditorDialogModel::getMissileBank2()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_weapons[1];
}

void WeaponEditorDialogModel::setMissileBank3(int m_missile3)
{
	if (getMissileBank3() != m_missile3) {
		if (getCurItem() == 0) {
			ammo_max3 = get_max_ammo_count_for_bank(ship_class, 2, m_missile3);
		} else {
			ammo_max3 = get_max_ammo_count_for_turret_bank(getCurWeapon(), 2, m_missile3);
		}
		getCurWeapon()->secondary_bank_ammo[2] = ammo_max3;
	}
	modify(_m_weapons.at(_m_cur_item).secondary_bank_weapons[2], m_missile3);
}

int WeaponEditorDialogModel::getMissileBank3()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_weapons[2];
}

void WeaponEditorDialogModel::setMissileBank4(int m_missile4)
{
	if (getMissileBank4() != m_missile4) {
		if (getCurItem() == 0) {
			ammo_max4 = get_max_ammo_count_for_bank(ship_class, 3, m_missile4);
		} else {
			ammo_max4 = get_max_ammo_count_for_turret_bank(getCurWeapon(), 3, m_missile4);
		}
		getCurWeapon()->secondary_bank_ammo[3] = ammo_max4;
	}
	modify(_m_weapons.at(_m_cur_item).secondary_bank_weapons[3], m_missile4);
}

int WeaponEditorDialogModel::getMissileBank4()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_weapons[3];
}

void WeaponEditorDialogModel::setAmmo1(int m_ammo1) {
	modify(_m_weapons.at(_m_cur_item).secondary_bank_ammo[0], m_ammo1);
}

int WeaponEditorDialogModel::getAmmo1()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_ammo[0];
}

void WeaponEditorDialogModel::setAmmo2(int m_ammo2)
{
	modify(_m_weapons.at(_m_cur_item).secondary_bank_ammo[1], m_ammo2);
}

int WeaponEditorDialogModel::getAmmo2()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_ammo[1];
}

void WeaponEditorDialogModel::setAmmo3(int m_ammo3)
{
	modify(_m_weapons.at(_m_cur_item).secondary_bank_ammo[2], m_ammo3);
}

int WeaponEditorDialogModel::getAmmo3()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_ammo[2];
}

void WeaponEditorDialogModel::setAmmo4(int m_ammo4)
{
	modify(_m_weapons.at(_m_cur_item).secondary_bank_ammo[3], m_ammo4);
}

int WeaponEditorDialogModel::getAmmo4()
{
	return _m_weapons.at(_m_cur_item).secondary_bank_ammo[3];
}

void WeaponEditorDialogModel::initializeData() {

	_m_cur_item = 0;
	last_item = -1;
	ship = -1;
	ship_class = -1;

	ship = _editor->cur_ship;
	if (ship == -1)
		ship = Objects[_editor->currentObject].instance;
	
	big = 1;

	_m_names.clear();
	_m_weapons.clear();

	_m_names.push_back(SCP_string("Pilot"));
	_m_weapons.push_back(Ships[ship].weapons);

	if (multi_edit) {
		int inst = 0, flag = 0, i;
		object* objp = GET_FIRST(&obj_used_list);
		ship_weapon* swp = &_m_weapons.at(0);
		while (objp != END_OF_LIST(&obj_used_list)) {
			if (objp->flags[Object::Object_Flags::Marked])
				if ((objp->type == OBJ_SHIP) || (objp->type == OBJ_START)) {
					inst = objp->instance;
					if (!(Ship_info[Ships[inst].ship_info_index].is_big_or_huge()))
						big = 0;
					
					if (!flag) {
						*swp = Ships[inst].weapons;
						ship_class = Ships[inst].ship_info_index;
						flag = 1;

					} else {
						Assert(Ships[inst].ship_info_index == ship_class);
						if (swp->ai_class != Ships[inst].weapons.ai_class)
							swp->ai_class = -1;

						for (i = 0; i < MAX_SHIP_PRIMARY_BANKS; i++)
							if (swp->primary_bank_weapons[i] != Ships[inst].weapons.primary_bank_weapons[i])
								swp->primary_bank_weapons[i] = -1;

						for (i = 0; i < MAX_SHIP_SECONDARY_BANKS; i++) {
							if (swp->secondary_bank_weapons[i] != Ships[inst].weapons.secondary_bank_weapons[i])
								swp->secondary_bank_weapons[i] = -1;
							if (swp->secondary_bank_ammo[i] != Ships[inst].weapons.secondary_bank_ammo[i])
								swp->secondary_bank_ammo[i] = -1;
						}
					}
				}
			objp = GET_NEXT(objp);
		}
	} else {
		ship_subsys *ssl, *pss;

		if (!(Ship_info[Ships[ship].ship_info_index].is_big_or_huge()))
			big = 0;

		ship_class = Ships[ship].ship_info_index;
		ssl = &Ships[ship].subsys_list;
		for (pss = GET_FIRST(ssl); pss != END_OF_LIST(ssl); pss = GET_NEXT(pss)) {
			if (pss->system_info->type == SUBSYSTEM_TURRET) {
				_m_names.push_back(SCP_string(pss->system_info->subobj_name));
				_m_weapons.push_back(pss->weapons);
			}
		}
	}

	modelChanged();
}

void WeaponEditorDialogModel::set_modified()
{
	if (!_modified) {
		_modified = true;
	}
}

bool WeaponEditorDialogModel::update_weapons(int sid)
{
	int i = 0;
	ship_weapon *swp, shp_wep;

	swp = &Ships[sid].weapons;
	shp_wep = _m_weapons.at(0);

	swp->ai_class = shp_wep.ai_class;

	for (i = 0; i < MAX_SHIP_PRIMARY_BANKS; i++) {
		swp->primary_bank_weapons[i] = shp_wep.primary_bank_weapons[i];
	}

	for (i = 0; i < MAX_SHIP_SECONDARY_BANKS; i++) {
		swp->secondary_bank_weapons[i] = shp_wep.secondary_bank_weapons[i];
		swp->secondary_bank_ammo[i] = shp_wep.secondary_bank_ammo[i];
	}
	
	if (!multi_edit) {
		int j = 1;
		ship_subsys *ssl, *pss;
		ssl = &Ships[ship].subsys_list;
		for (pss = GET_FIRST(ssl); pss != END_OF_LIST(ssl); pss = GET_NEXT(pss)) {
			if (pss->system_info->type == SUBSYSTEM_TURRET) {
				swp = &pss->weapons;
				shp_wep = _m_weapons.at(j++);

				swp->ai_class = shp_wep.ai_class;

				for (i = 0; i < MAX_SHIP_PRIMARY_BANKS; i++) {
					swp->primary_bank_weapons[i] = shp_wep.primary_bank_weapons[i];
				}

				for (i = 0; i < MAX_SHIP_SECONDARY_BANKS; i++) {
					swp->secondary_bank_weapons[i] = shp_wep.secondary_bank_weapons[i];
					swp->secondary_bank_ammo[i] = shp_wep.secondary_bank_ammo[i];
				}
			}
		}
	}

	_editor->missionChanged();
	return true;
}

bool WeaponEditorDialogModel::update_data()
{
	if (multi_edit) {
		object* ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_START) || (ptr->type == OBJ_SHIP)) && (ptr->flags[Object::Object_Flags::Marked]))
				update_weapons(get_ship_from_obj(ptr));

			ptr = GET_NEXT(ptr);
		}
	} else {
		update_weapons(ship);
	}
	return true;
}

void WeaponEditorDialogModel::update_ammo(int bank) {
	int ammo = 0;
	if (_m_cur_item == 0) {
		ammo = get_max_ammo_count_for_bank(ship_class, 2, _m_weapons.at(_m_cur_item).secondary_bank_weapons[bank]);
	} else {
		ammo = get_max_ammo_count_for_turret_bank(getCurWeapon(),
			2,
			_m_weapons.at(_m_cur_item).secondary_bank_weapons[bank]);
	}

	_m_weapons.at(_m_cur_item).secondary_bank_ammo[bank] = ammo;
}

} // namespace dialogs
}
}
