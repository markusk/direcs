#include "aboutDialog.h"

AboutDialog::AboutDialog(QWidget* parent, Qt::WFlags fl) : QDialog(parent, fl)
{
	ui.setupUi(this);
}

void AboutDialog::on_btnAboutQt_clicked(bool checked)
{
	Q_UNUSED(checked);

	QApplication::aboutQt();
}
