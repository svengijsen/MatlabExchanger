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


#include "MatlabExchanger.h"
#include <QModelIndex>

QScriptValue MatlabExchanger::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
	return engine->newQObject(new MatlabExchanger(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

MatlabExchanger::MatlabExchanger(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	m_pEngine = NULL;
}

MatlabExchanger::~MatlabExchanger()
{
	closeEngine();
}

bool MatlabExchanger::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool MatlabExchanger::openEngine()
{
	if (m_pEngine)
		return true;
	m_pEngine = engOpen(NULL);
	if (m_pEngine == NULL)
		return false;
	engOutputBuffer(m_pEngine, m_output, sizeof(m_output));
	return true;
}

bool MatlabExchanger::closeEngine()
{
	if (m_pEngine)
	{
		engClose(m_pEngine);
		m_pEngine = NULL;
	}
	return true;
}

void::MatlabExchanger::setVisibility(const bool &bSetVisible)
{
	if (m_pEngine)
	{
		engSetVisible(m_pEngine, bSetVisible);
	}
}

QString MatlabExchanger::getOutput()
{
	QString sResult = "";
	//if (strncmp(output, "???", 3) == 0)
	//	gl_error("[MATLAB] %s", output + 4);
	//else 
	if (m_output[0] != '\0')
	{
		//	gl_debug("[MATLAB] %s", m_output);
		sResult = QString("%1").arg(m_output);
	}
	return sResult;
}

bool MatlabExchanger::evaluateString(const QString &sCommand)
{
	if (m_pEngine)
	{
		engEvalString(m_pEngine, sCommand.toLatin1());
		return true;
	}
	return false;
}

bool MatlabExchanger::putVariabele(const QString &sVarName, MultiArray *pArray, const int &nSizeM, const int &nSizeN)
{
	if (m_pEngine)
	{
		mxArray *m_X;
		//Create a two - dimensional array to hold double - precision floating - point data; initialize each data element to 0.
		m_X = mxCreateDoubleMatrix(nSizeM, nSizeN, mxREAL);
		memcpy((void *)mxGetPr(m_X), (void *)pArray, sizeof(double) * nSizeM * nSizeN);
		engPutVariable(m_pEngine, sVarName.toLatin1(), m_X);
		return true;
	}
	return false;
}

MultiArray *MatlabExchanger::getVariabele(const QString &sVarName)
{
	if (m_pEngine)
	{
		MultiArray *pMultiArrayResult = new MultiArray(1,0, this);
		double *pDoubleResult;
		mxArray *pMexResult;
		pMexResult = engGetVariable(m_pEngine, sVarName.toLatin1());
		pDoubleResult = mxGetPr(pMexResult);
		pMultiArrayResult->setData(pMultiArrayResult->index(0,0), *pDoubleResult);
		return pMultiArrayResult;
	}
	return NULL;
}

/*

//engSetVisible(m_pEngine, false);
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

/*
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
	}

*/

