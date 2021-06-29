#pragma once

#include "../AbstractDialogModel.h"

#include "ship/ship.h"
#include "ui/widgets/sexp_tree.h"

namespace fso {
namespace fred {
namespace dialogs {

class WeaponEditorDialogModel : public AbstractDialogModel {
  private:
	void initializeData();

	template <typename T>
	void modify(T& a, const T& b);

	bool _modified = false;

	int _m_cur_item;

	SCP_vector<SCP_string> _m_names;
	SCP_vector<ship_weapon> _m_weapons;

	void set_modified();

	bool update_weapons(int);
	bool update_data();

	void update_ammo(int bank);

  public:
	WeaponEditorDialogModel(QObject* parent, EditorViewport* viewport, int multi_edit);

	bool apply() override;
	void reject() override;

	void setCurItem(int);
	int getCurItem();

	SCP_string getName(int idx);
	ship_weapon* getCurWeapon();

	int getNumWeapons();
	int getNumPrimaries();
	int getNumSecondaries();

	void setGunBank1(int);
	int getGunBank1();
	void setGunBank2(int);
	int getGunBank2();
	void setGunBank3(int);
	int getGunBank3();

	void setAIClass(int);
	int getAIClass();

	void setMissileBank1(int);
	int getMissileBank1();
	void setMissileBank2(int);
	int getMissileBank2();
	void setMissileBank3(int);
	int getMissileBank3();
	void setMissileBank4(int);
	int getMissileBank4();

	void setAmmo1(int);
	int getAmmo1();
	void setAmmo2(int);
	int getAmmo2();
	void setAmmo3(int);
	int getAmmo3();
	void setAmmo4(int);
	int getAmmo4();

	int ammo_max1;
	int ammo_max2;
	int ammo_max3;
	int ammo_max4;
	int last_item;
	int ship;
	int ship_class;
	int multi_edit;
	int big;
};

template <typename T>
inline void WeaponEditorDialogModel::modify(T& a, const T& b)
{
	if (a != b) {
		a = b;
		set_modified();
		modelChanged();
	}
}

} // namespace dialogs
} // namespace fred
} // namespace fso
