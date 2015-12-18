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


//This file defines the script binding interface, all below function are scriptable except for the destructor
#ifndef MATLABEXCHANGER_H
#define MATLABEXCHANGER_H

#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include "mainappinfo.h"
#include <engine.h>

#include "mexplus.h"
#include <mexplus/mxarray.h>
using mexplus::MxArray;

//!  The MatlabExchanger class. 
/*!
The MatlabExchanger can send commands and exchange data with MatLab.
*/
class MatlabExchanger : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "MatlabExchanger");//Can't use defines here!, moc doesn't handle defines, not needed here

public:
	MatlabExchanger(QObject *parent = NULL);
	~MatlabExchanger();
	MatlabExchanger(const MatlabExchanger& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor_MatlabExchanger(QScriptContext* context, QScriptEngine* engine);
	void setScriptEngine(QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	//! \brief openEngine slot
	/*!  This function tries to open a connection to a Matlab engine, this can take some seconds.
	* @return a boolean value determining whether the connection could be established.
	*/
	bool openEngine();
	//! \brief closeEngine slot.
	/*!  This function closes a opened Matlab engine connection, see openEngine().
	* @return a boolean value determining whether the connection could be closed.
	*/
	bool closeEngine();
	//! \brief setVisibility slot.
	/*!  This function configures whether the Matlab engine window to be opened should be visible.
	* @param bSetVisible a boolean value determining the visibility.
	*/
	void setVisibility(const bool &bSetVisible);
	//! \brief evaluateString slot.
	/*!  This function sends a command string to the opened Matlab engine connection to evaluate.
	* @param sCommand a string containing the command.
	* @return a boolean value determining whether the command could be evaluated.
	*/
	bool evaluateString(const QString &sCommand);
	//! \brief getOutput slot.
	/*!  This function retrieves the last Matlab engine output messages.
	* @return a string containing the latest messages.
	*/
	QString getOutput();
	//! \brief getVariabele slot.
	/*!  This function fetches the value of a Matlab variable as a script value.
	* @param sVarName the name of the Matlab variable.
	* @return a script value holding the value of the variabele.
	*/
	QScriptValue getVariabele(const QString &sVarName);

private:
	QScriptEngine* currentScriptEngine;
	Engine *m_pEngine;
	char m_output[4096];
};

#endif // MATLABEXCHANGER_H
