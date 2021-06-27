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
	int _m_cur_item;

	void set_modified();

  public:
	ShipEditWeaponDialogModel(QObject* parent, EditorViewport* viewport);

	bool apply() override;
	void reject() override;

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
