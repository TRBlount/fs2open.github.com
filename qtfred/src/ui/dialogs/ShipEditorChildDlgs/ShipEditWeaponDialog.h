#ifndef SHIPEDITWEAPONDIALOG_H
#define SHIPEDITWEAPONDIALOG_H

#include <QtWidgets/QDialog>
#include <mission/dialogs/ShipEditorChildModels/ShipEditWeaponDialogModel.h>
#include <ui/FredView.h>

#include <QAbstractButton>

namespace fso {
namespace fred {
namespace dialogs {

namespace Ui {
class ShipEditWeaponDialog;
}

class ShipEditWeaponDialog : public QDialog {
	Q_OBJECT

  public:
	ShipEditWeaponDialog(QWidget* parent, EditorViewport* viewport);
	~ShipEditWeaponDialog() override;

  protected:
	void closeEvent(QCloseEvent*) override;
  private:
	std::unique_ptr<Ui::ShipEditWeaponDialog> ui;
	std::unique_ptr<ShipEditWeaponDialogModel> _model;
	EditorViewport* _viewport;

	void initDialog();

	void updateUI();
	void updateWeaponList();
	void updatePrimaries();
	void updateAIClass();
	void updateSecondaries();

	void change_selection();
	void weaponSelectChanged(int);

	void gunBank1Changed(int);
	void gunBank2Changed(int);
	void gunBank3Changed(int);

	void aiClassChanged(int);

	void missileBank1Changed(int);
	void missileBank2Changed(int);
	void missileBank3Changed(int);
	void missileBank4Changed(int);
	void missileAmmo1Changed(int);
	void missileAmmo2Changed(int);
	void missileAmmo3Changed(int);
	void missileAmmo4Changed(int);
};

} // namespace dialogs
} // namespace fred
} // namespace fso

#endif // SHIPEDITWEAPONDIALOG_H
