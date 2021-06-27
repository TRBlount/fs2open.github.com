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
	ShipEditWeaponDialog(QWidget* parent = nullptr);
	~ShipEditWeaponDialog() override;

  protected:
	void closeEvent(QCloseEvent*) override;
  private:
	std::unique_ptr<Ui::ShipEditWeaponDialog> ui;
	std::unique_ptr<ShipEditWeaponDialogModel> _model;
	EditorViewport* _viewport;
};

} // namespace dialogs
} // namespace fred
} // namespace fso

#endif // SHIPEDITWEAPONDIALOG_H
