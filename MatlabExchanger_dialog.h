//MatlabExchanger
//Copyright (C) 2015  Sven Gijsen
//
//This file is part of BrainStim.
//BrainStim is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef MATLABEXCHANGER_DIALOG_H
#define MATLABEXCHANGER_DIALOG_H

#include <QDialog>
#include "ui_MatlabExchanger_dialog.h"
#include <QtWidgets>
#include <Windows.h>
#include <QDialog>

class MatlabExchanger_Dialog : public QDialog, private Ui::MatlabExchanger_DialogClass
{
	Q_OBJECT

public:
	MatlabExchanger_Dialog(QWidget *parent = 0);
	~MatlabExchanger_Dialog();

private:
	Ui::MatlabExchanger_DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void on_okButton_clicked();
};

#endif // MATLABEXCHANGER_DIALOG_H
