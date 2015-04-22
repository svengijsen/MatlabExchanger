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


#include "MatlabExchanger_dialog.h"
#include <engine.h>

MatlabExchanger_Dialog::MatlabExchanger_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

MatlabExchanger_Dialog::~MatlabExchanger_Dialog()
{

}

void MatlabExchanger_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void MatlabExchanger_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void MatlabExchanger_Dialog::cleanUp()
{
	return;
}

void MatlabExchanger_Dialog::exampleButton_Pressed()
{
	//QMessageBox msgBox;
	//msgBox.setText("Example button pressed.");
	//msgBox.exec();

	Engine *m_pEngine = engOpen(NULL);
	if (m_pEngine == NULL)
	{
		printf("Error: Matlab not found.");
	}
	else 
	{
		printf("Success. Matlab is there.");
		//engEvalString(m_pEngine, "1+3");
		//engEvalString(m_pEngine, "1+3");
		//engClose(m_pEngine);
		engSetVisible(m_pEngine, false);
		double x[1000];
		double y[1000];
		double z[1000];

		double t = 0;
		const double dt = 0.001;
		int i, j;
		double a, b;

		mxArray *z_array = mxCreateDoubleMatrix(1000, 1, mxREAL);
		mxArray *a_array = mxCreateDoubleMatrix(1, 1, mxREAL);
		mxArray *b_array = mxCreateDoubleMatrix(1, 1, mxREAL);

		double *pz = mxGetPr(z_array);
		double *pa = mxGetPr(a_array);
		double *pb = mxGetPr(b_array);

		for (i = 0; i < 1000; i++)
		{
			x[i] = cos(2 * M_PI*t);
			y[i] = sin(2 * M_PI*t);
			t += dt;
		}
		a = 1;
		b = 0;
		for (j = 0; j < 100; j++)
		{
			for (i = 0; i < 1000; i++)
			{
				z[i] = a*x[i] + b*y[i];
				pz[i] = z[i];
			}
			pa[0] = a;
			pb[0] = b;
			engPutVariable(m_pEngine, "z", z_array);
			engPutVariable(m_pEngine, "a", a_array);
			engPutVariable(m_pEngine, "b", b_array);
			engEvalString(m_pEngine, "testPlot");
			a = a - 0.01;
			b = b + 0.01;

			//returning a array, max is 2GB!
			mxArray *return_array = engGetVariable(m_pEngine, "z");
			if (return_array != NULL)
			{
				double *test = mxGetPr(return_array);
				if (test)
				{
					double a = *test;
					a = a;
				}
			}
		}
		engClose(m_pEngine);
	}
}
