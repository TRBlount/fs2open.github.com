
#include "WeaponEditorDialogModel.h"

#include "ui/dialogs/ShipEditorChildDlgs/WeaponEditorDialog.h"

#include <globalincs/linklist.h>
#include <mission/object.h>

#include <QtWidgets>

namespace fso {
namespace fred {
namespace dialogs {

WeaponEditorDialogModel::WeaponEditorDialogModel(QObject* parent, EditorViewport* viewport) 
	: AbstractDialogModel(parent, viewport)
{
	initializeData();
}

bool WeaponEditorDialogModel::apply()
{
	_editor->missionChanged();
	initializeData();
	return true;
}

void WeaponEditorDialogModel::reject() {}

void WeaponEditorDialogModel::setCurrItem(int m_cur_item) {
	modify(_m_cur_item, m_cur_item);
}

int WeaponEditorDialogModel::getCurrItem()
{
	return _m_cur_item;
}

void WeaponEditorDialogModel::setGunBank1(int m_gun1)
{
	modify(_m_gun1, m_gun1);
}

int WeaponEditorDialogModel::getGunBank1()
{
	return _m_gun1;
}

void WeaponEditorDialogModel::setGunBank2(int m_gun2) {
	modify(_m_gun2, m_gun2);
}

int WeaponEditorDialogModel::getGunBank2()
{
	return _m_gun2;
}

void WeaponEditorDialogModel::setGunBank3(int m_gun3) {
	modify(_m_gun3, m_gun3);
}

int WeaponEditorDialogModel::getGunBank3()
{
	return _m_gun3;
}

void WeaponEditorDialogModel::setAIClass(int m_ai_class) {
	modify(_m_ai_class, m_ai_class);
}

int WeaponEditorDialogModel::getAIClass()
{
	return _m_ai_class;
}

void WeaponEditorDialogModel::setMissileBank1(int m_missile1)
{
	modify(_m_missile1, m_missile1);
}

int WeaponEditorDialogModel::getMissileBank1()
{
	return _m_missile1;
}

void WeaponEditorDialogModel::setMissileBank2(int m_missile2)
{
	modify(_m_missile2, m_missile2);
}

int WeaponEditorDialogModel::getMissileBank2()
{
	return _m_missile2;
}

void WeaponEditorDialogModel::setMissileBank3(int m_missile3)
{
	modify(_m_missile3, m_missile3);
}

int WeaponEditorDialogModel::getMissileBank3()
{
	return _m_missile3;
}

void WeaponEditorDialogModel::setMissileBank4(int m_missile4)
{
	modify(_m_missile4, m_missile4);
}

int WeaponEditorDialogModel::getMissileBank4()
{
	return _m_missile4;
}

void WeaponEditorDialogModel::setAmmo1(int m_ammo1) {
	modify(_m_ammo1, m_ammo1);
}

int WeaponEditorDialogModel::getAmmo1()
{
	return _m_ammo1;
}

void WeaponEditorDialogModel::setAmmo2(int m_ammo2)
{
	modify(_m_ammo2, m_ammo2);
}

int WeaponEditorDialogModel::getAmmo2()
{
	return _m_ammo2;
}

void WeaponEditorDialogModel::setAmmo3(int m_ammo3)
{
	modify(_m_ammo3, m_ammo3);
}

int WeaponEditorDialogModel::getAmmo3()
{
	return _m_ammo3;
}

void WeaponEditorDialogModel::setAmmo4(int m_ammo4)
{
	modify(_m_ammo4, m_ammo4);
}

int WeaponEditorDialogModel::getAmmo4()
{
	return _m_ammo4;
}

void WeaponEditorDialogModel::initializeData() {
	int mission_type = 0;
	int type = 0, i = 0;
	int escort_count = 0, pship_count = 0, player_count = 0, ship_count = 0;
	int base_player = 0, base_ship = 0, pvalid_count = 0;
	int player_ship = -1, single_ship = -1;
	object* objp;

	_m_ai_class = -1;
	_m_ammo1 = 0;
	_m_ammo2 = 0;
	_m_ammo3 = 0;
	_m_ammo4 = 0;
	_m_gun1 = -1;
	_m_gun2 = -1;
	_m_gun3 = -1;
	_m_missile1 = -1;
	_m_missile2 = -1;
	_m_missile3 = -1;
	_m_missile4 = -1;
	_m_cur_item = 0;
	last_item = -1;
	ship = -1;
	ship_class = -1;
	multi_edit = 0;

	ship = _editor->cur_ship;
	if (ship == -1)
		ship = Objects[_editor->currentObject].instance;

	if (The_mission.game_type & MISSION_TYPE_MULTI) {
		mission_type = 0; // multi player mission
	} else {
		mission_type = 1; // non-multiplayer mission (implies single player mission I guess)
	}

	auto ptr = GET_FIRST(&obj_used_list);
	while (ptr != END_OF_LIST(&obj_used_list)) {
		if ((ptr->type == OBJ_SHIP) && (Ships[ptr->instance].flags[Ship::Ship_Flags::Escort])) {
			escort_count++; // get a total count of escort ships
		}

		if (ptr->type == OBJ_START) {
			pship_count++; // count all player starts in mission
		}

		if (ptr->flags[Object::Object_Flags::Marked]) {
			type = ptr->type;
			if ((type == OBJ_START) && !mission_type) { // in multiplayer missions, starts act like ships
				type = OBJ_SHIP;
			}

			i = -1;
			if (type == OBJ_START) {
				player_count++;
				// if player_count is 1, base_player will be the one and only player
				i = base_player = ptr->instance;

			} else if (type == OBJ_SHIP) {
				ship_count++;
				// if ship_count is 1, base_ship will be the one and only ship
				i = base_ship = ptr->instance;
			}

			if (i >= 0) {
				if (Ship_info[Ships[i].ship_info_index].flags[Ship::Info_Flags::Player_ship]) {
					pvalid_count++;
				}
			}
		}
		ptr = GET_NEXT(ptr);
	}

	int total_count = ship_count + player_count; // get total number of objects being edited.
	if (total_count > 1) {
		multi_edit = 1;
	} else {
		multi_edit = 0;
	}

	if (ship_count) {
		base_player = 0;
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (((ptr->type == OBJ_START) || (ptr->type == OBJ_SHIP)) && ptr->flags[Object::Object_Flags::Marked]) {
				// do processing for both ships and players
				i = get_ship_from_obj(ptr);
				if (base_player >= 0) {
					ship_class = Ships[i].ship_info_index;
					base_player = -1;

				} else {
					if (Ships[i].ship_info_index != ship_class)
						ship_class = -1;
				}

				if (Ships[i].flags[Ship::Ship_Flags::Escort]) {
					escort_count--; // remove marked escorts from count
				}

				if (Objects[Ships[i].objnum].type == OBJ_START) {
					pship_count--; // removed marked starts from count
				}

				// do processing only for ships (plus players if in a multiplayer mission
				if ((ptr->type == OBJ_SHIP) || ((ptr->type == OBJ_START) && !mission_type)) {

					// process the first ship in group, else process the rest
					if (base_ship >= 0) {
						_m_ai_class = Ships[i].weapons.ai_class;

						base_ship = -1;
						if (!multi_edit)
							single_ship = i;

					} else {
						if (Ships[i].weapons.ai_class != _m_ai_class) {
							_m_ai_class = -1;
						}
					}
				}

				ptr = GET_NEXT(ptr);
			}
		}
	} else {                    // no ships selected, 0 or more player ships selected
		if (player_count > 1) { // multiple player ships selected
			Assert(base_player >= 0);
			ptr = GET_FIRST(&obj_used_list);
			while (ptr != END_OF_LIST(&obj_used_list)) {
				if ((ptr->type == OBJ_START) && (ptr->flags[Object::Object_Flags::Marked])) {
					i = ptr->instance;
					if (base_player >= 0) {
						ship_class = Ships[i].ship_info_index;
						base_player = -1;

					} else {
						if (Ships[i].ship_info_index != ship_class)
							ship_class = -1;
					}
				}

				ptr = GET_NEXT(ptr);
			}
			// only 1 player selected..
		} else if (query_valid_object(_editor->currentObject) && (Objects[_editor->currentObject].type == OBJ_START)) {
			// Assert((player_count == 1) && !multi_edit);
			player_ship = Objects[_editor->currentObject].instance;
			ship_class = Ships[player_ship].ship_info_index;
		} else { // no ships or players selected..
			ship = -1;
			ship_class = -1;
		}
	}

	int big = 1, flag = 0;
	if (multi_edit) {
		objp = GET_FIRST(&obj_used_list);
		while (objp != END_OF_LIST(&obj_used_list)) {
			if (objp->flags[Object::Object_Flags::Marked] && ((objp->type == OBJ_SHIP) || (objp->type == OBJ_START))) {
				i = objp->instance;

				if (!flag) {
					pilot = Ships[i].weapons;
					ship_class = Ships[i].ship_info_index;
					flag = 1;

				} else {
					Assert(Ships[i].ship_info_index == ship_class);
					if (pilot.ai_class != Ships[i].weapons.ai_class)
						pilot.ai_class = BLANK_FIELD;

					for (i = 0; i < MAX_SHIP_PRIMARY_BANKS; i++)
						if (pilot.primary_bank_weapons[i] != Ships[i].weapons.primary_bank_weapons[i])
							pilot.primary_bank_weapons[i] = BLANK_FIELD;

					for (i = 0; i < MAX_SHIP_SECONDARY_BANKS; i++) {
						if (pilot.secondary_bank_weapons[i] != Ships[i].weapons.secondary_bank_weapons[i])
							pilot.secondary_bank_weapons[i] = -1;
						if (pilot.secondary_bank_ammo[i] != Ships[i].weapons.secondary_bank_ammo[i])
							pilot.secondary_bank_ammo[i] = BLANK_FIELD;
					}
				}
			}
					objp = GET_NEXT(objp);
		}

	} else {
		if (!(Ship_info[Ships[ship].ship_info_index].is_big_or_huge()))
			big = 0;

		ship = single_ship;
		if (ship < 0)
			ship = player_ship;
		Assert(ship >= 0);
		ship_class = Ships[ship].ship_info_index;
		pilot = Ships[ship].weapons;
		cur_weapon = &pilot;
	}

	_m_ai_class = pilot.ai_class;

	_m_gun1 = pilot.primary_bank_weapons[0];
	_m_gun2 = pilot.primary_bank_weapons[1];
	_m_gun3 = pilot.primary_bank_weapons[2];

	_m_missile1 = pilot.secondary_bank_weapons[0];
	_m_missile2 = pilot.secondary_bank_weapons[1];
	_m_missile3 = pilot.secondary_bank_weapons[2];
	_m_missile4 = pilot.secondary_bank_weapons[3];

	modelChanged();
}

void WeaponEditorDialogModel::set_modified()
{
	if (!_modified) {
		_modified = true;
	}
}

bool WeaponEditorDialogModel::update_weapons(int ship_id)
{
	
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

} // namespace dialogs
}
}
