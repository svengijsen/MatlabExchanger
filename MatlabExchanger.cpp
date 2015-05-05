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
//#include <QModelIndex>

QScriptValue MatlabExchanger::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
	MatlabExchanger *tmpObject = new MatlabExchanger();
	tmpObject->setScriptEngine(engine);
	return engine->newQObject(tmpObject, QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
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

void MatlabExchanger::setScriptEngine(QScriptEngine* engine)
{
	currentScriptEngine = engine;
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

//bool MatlabExchanger::putVariabele(const QString &sVarName, MultiArray *pArray, const int &nSizeM, const int &nSizeN)
//{
//	if (m_pEngine)
//	{
//		mxArray *m_X;
//		//Create a two - dimensional array to hold double - precision floating - point data; initialize each data element to 0.
//		m_X = mxCreateDoubleMatrix(nSizeM, nSizeN, mxREAL);
//		memcpy((void *)mxGetPr(m_X), (void *)pArray, sizeof(double) * nSizeM * nSizeN);
//		engPutVariable(m_pEngine, sVarName.toLatin1(), m_X);
//		return true;
//	}
//	return false;
//}

QScriptValue MatlabExchanger::getVariabele(const QString &sVarName)
{
	if (m_pEngine && currentScriptEngine)
	{
		mxArray *pMexResult = engGetVariable(m_pEngine, sVarName.toLatin1());
		if (pMexResult)
		{
			//Use mxGetPr on arrays of type double only. Use mxIsDouble to validate the mxArray type. 
			//For other mxArray types, use mxGetData.
			//Call mxGetPr to access the real data in the mxArray that pm points to.
			//Once you have the starting address, you can access any other element in the mxArray.

			if (mxIsDouble(pMexResult))
			{
				double *pDoubleResult1 = mxGetPr(pMexResult);
				//double pDoubleResult2 = MxArray::to<double>(pMexResult);
				//QVariant aa = MxArray::to<QVariant>(pMexResult);
				//pDoubleResult2 = pDoubleResult2;
				//vector<double> value = MxArray::to<vector<double> >(pMexResult);
				/*
				
				 inline bool isCell() const { return mxIsCell(array_); }
  /** Determine whether input is string array.
   */
				//inline bool isChar() const { return mxIsChar(array_); }
				///** Determine whether array is member of specified class.
				//*/
				//inline bool isClass(const char* name) const {
				//	return mxIsClass(array_, name);
				//}
				///** Determine whether data is complex.
				//*/
				//inline bool isComplex() const { return mxIsComplex(array_); }
				///** Determine whether mxArray represents data as double-precision,
				//* floating-point numbers.
				//*/
				//inline bool isDouble() const { return mxIsDouble(array_); }
				///** Determine whether array is empty.
				//*/
				//inline bool isEmpty() const { return mxIsEmpty(array_); }
				///** Determine whether input is finite.
				//*/
				//static inline bool IsFinite(double value) { return mxIsFinite(value); }
				///** Determine whether array was copied from MATLAB global workspace.
				//*/
				//inline bool isFromGlobalWS() const { return mxIsFromGlobalWS(array_); };
				///** Determine whether input is infinite.
				//*/
				//static inline bool IsInf(double value) { return mxIsInf(value); }
				///** Determine whether array represents data as signed 8-bit integers.
				//*/
				//inline bool isInt8() const { return mxIsInt8(array_); }
				///** Determine whether array represents data as signed 16-bit integers.
				//*/
				//inline bool isInt16() const { return mxIsInt16(array_); }
				///** Determine whether array represents data as signed 32-bit integers.
				//*/
				//inline bool isInt32() const { return mxIsInt32(array_); }
				///** Determine whether array represents data as signed 64-bit integers.
				//*/
				//inline bool isInt64() const { return mxIsInt64(array_); }
				///** Determine whether array is of type mxLogical.
				//*/
				//inline bool isLogical() const { return mxIsLogical(array_); }
				///** Determine whether scalar array is of type mxLogical.
				//*/
				//inline bool isLogicalScalar() const { return mxIsLogicalScalar(array_); }
				///** Determine whether scalar array of type mxLogical is true.
				//*/
				//inline bool isLogicalScalarTrue() const {
				//	return mxIsLogicalScalarTrue(array_);
				//}
				///** Determine whether array is numeric.
				//*/
				//inline bool isNumeric() const { return mxIsNumeric(array_); }
				///** Determine whether array represents data as single-precision,
				//* floating-point numbers.
				//*/
				//inline bool isSingle() const { return mxIsSingle(array_); }
				///** Determine whether input is sparse array.
				//*/
				//inline bool isSparse() const { return mxIsSparse(array_); }
				///** Determine whether input is structure array.
				//*/
				//inline bool isStruct() const { return mxIsStruct(array_); }
				///** Determine whether array represents data as unsigned 8-bit integers.
				//*/
				//inline bool isUint8() const { return mxIsUint8(array_); }
				///** Determine whether array represents data as unsigned 16-bit integers.
				//*/
				//inline bool isUint16() const { return mxIsUint16(array_); }
				///** Determine whether array represents data as unsigned 32-bit integers.
				//*/
				//inline bool isUint32() const { return mxIsUint32(array_); }
				///** Determine whether array represents data as unsigned 64-bit integers.
				//*/
				//inline bool isUint64() const { return mxIsUint64(array_); }
				///** Determine whether a struct array has a specified field.
				//*/
				//bool hasField(const std::string& field_name, mwIndex index = 0) const {
				//	return isStruct() &&
				//		mxGetField(array_, index, field_name.c_str()) != NULL;
				//}
				///** Determine whether input is NaN (Not-a-Number).
				//*/
				//static inline bool IsNaN(double value) { return mxIsNaN(value); }
				///** Value of infinity.
				//*/


			}

			double *pDoubleResult = mxGetPr(pMexResult);
			int nRows = mxGetM(pMexResult);
			int nCols = mxGetN(pMexResult);
			int nElementCount = nRows * nCols;
			if (nElementCount > 0)
			{
				QScriptValue retList = currentScriptEngine->newArray(nCols);
				int nCurrentElementIndexNumber = 0;
				for (int n = 0; n < nCols; n++)
				{
					QScriptValue svCurrentColumn = currentScriptEngine->newArray(nRows);
					for (int m = 0; m < nRows; m++)
					{
						svCurrentColumn.setProperty(m, QScriptValue(pDoubleResult[nCurrentElementIndexNumber]));
						nCurrentElementIndexNumber++;
					}
					retList.setProperty(n, svCurrentColumn);
				}
				return retList;
			}
		}
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

