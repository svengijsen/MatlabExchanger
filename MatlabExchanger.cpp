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

QScriptValue MatlabExchanger::ctor_MatlabExchanger(QScriptContext* context, QScriptEngine* engine)
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

QScriptValue MatlabExchanger::getVariabele(const QString &sVarName)
{
	if (m_pEngine && currentScriptEngine)
	{
		mxArray *pMexResult = engGetVariable(m_pEngine, sVarName.toLatin1());
		if (pMexResult)
		{
			void *pResult = mxGetData(pMexResult);
			int nRows = mxGetM(pMexResult);
			int nCols = mxGetN(pMexResult);
			int nElementCount = nRows * nCols;
			if (nElementCount > 0)
			{
				QScriptValue retList = currentScriptEngine->newArray(nRows);
				int nCurrentElementCounter = 0;
				int nCurrentElementIndexNumber = 0;
				MEXPLUS_CHECK_NOTNULL(pMexResult);
				//MEXPLUS_CHECK_NOTNULL(value);
				mxClassID mxID = mxGetClassID(pMexResult);
				for (int nCurrentRow = 0; nCurrentRow < nRows; nCurrentRow++)
				{
					QScriptValue svCurrentRow;
					if (mxID == mxCHAR_CLASS)
						svCurrentRow = currentScriptEngine->newArray(1);
					else
						svCurrentRow = currentScriptEngine->newArray(nCols);
					for (int nCurrentColumn = 0; nCurrentColumn < nCols; nCurrentColumn++)
					{
						nCurrentElementIndexNumber = nCurrentRow + (nCurrentColumn * nRows);
						if (mxID == mxINT8_CLASS) { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((int8_t*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxUINT8_CLASS)  { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((uint8_t*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxINT16_CLASS)  { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((int16_t*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxUINT16_CLASS) { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((uint16_t*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxINT32_CLASS)  { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((int32_t*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxUINT32_CLASS) { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((uint32_t*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxINT64_CLASS)  { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(QVariant(((int64_t*)pResult)[nCurrentElementIndexNumber]).toString())); }
						else if (mxID == mxUINT64_CLASS) { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(QVariant(((uint64_t*)pResult)[nCurrentElementIndexNumber]).toString())); }
						else if (mxID == mxSINGLE_CLASS) { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((float*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxDOUBLE_CLASS) { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(((double*)pResult)[nCurrentElementIndexNumber])); }
						else if (mxID == mxCHAR_CLASS)   { svCurrentRow.setProperty(nCurrentColumn, QScriptValue(mxArrayToString(pMexResult))); break; }
						else { qDebug() << __FUNCTION__ << "Cannot convert %s.", mxGetClassName(pMexResult); break; }
						//else if (mxID == mxLOGICAL_CLASS {break; //assignTo<mxLogical, T>(array, value); }
						//else if (mxID == mxCELL_CLASS {break; //assignCellTo<T>(array, value); }
						//else if (mxID == mxSPARSE_CLASS {
						nCurrentElementCounter++;
					}
					retList.setProperty(nCurrentRow, svCurrentRow);
				}
				return retList;
			}
		}
	}
	return NULL;
}