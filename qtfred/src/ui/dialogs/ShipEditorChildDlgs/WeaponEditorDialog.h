#ifndef WEAPONEDITORDIALOG_H
#define WEAPONEDITORDIALOG_H

#include <QtWidgets/QDialog>
#include <mission/dialogs/ShipEditorChildModels/WeaponEditorDialogModel.h>
#include <ui/FredView.h>

#include <QListWidgetItem>

#include <QAbstractButton>

#define BLANK_FIELD -99

namespace fso {
namespace fred {
namespace dialogs {

namespace Ui {
class WeaponEditorDialog;
}

class WeaponEditorDialog : public QDialog {
	Q_OBJECT

  public:
	WeaponEditorDialog(QWidget* parent, EditorViewport* viewport);
	~WeaponEditorDialog() override;

  protected:
	void closeEvent(QCloseEvent*) override;

  private:
	std::unique_ptr<Ui::WeaponEditorDialog> ui;
	std::unique_ptr<WeaponEditorDialogModel> _model;
	EditorViewport* _viewport;

	void initDialog();

	void updateUI();
	void update_pilot();

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

class SWP_ListWidgetItem : public QListWidgetItem {
  public:
	SWP_ListWidgetItem(QListWidget* parent = nullptr);
	SWP_ListWidgetItem(const QString& text, QListWidget* parent = nullptr);

	ship_weapon* getUserData() const;
	void setUserData(ship_weapon* data);

  private:
	ship_weapon* userData;
};

} // namespace dialogs
} // namespace fred
} // namespace fso

#endif // WEAPONEDITORDIALOG_H
