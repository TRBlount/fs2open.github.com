#pragma once

#include "../AbstractDialogModel.h"

#include "ship/ship.h"
#include "ui/widgets/sexp_tree.h"

namespace fso {
namespace fred {
namespace dialogs {

class ShipEditWeaponDialogModel : public AbstractDialogModel {
  private:
	void initializeData();

	template <typename T>
	void modify(T& a, const T& b);

	bool _modified = false;

	int _m_cur_item;
	int _m_ai_class;
	int _m_ammo1;
	int _m_ammo2;
	int _m_ammo3;
	int _m_ammo4;
	int _m_gun1;
	int _m_gun2;
	int _m_gun3;
	int _m_missile1;
	int _m_missile2;
	int _m_missile3;
	int _m_missile4;

	void set_modified();

  public:
	ShipEditWeaponDialogModel(QObject* parent, EditorViewport* viewport);

	bool apply() override;
	void reject() override;

	void setCurrItem(int);
	int getCurrItem();

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
	ship_weapon pilot, *cur_weapon;
};

template <typename T>
inline void ShipEditWeaponDialogModel::modify(T& a, const T& b)
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
