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
#ifndef MatlabExchanger_H
#define MatlabExchanger_H

#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include "mainappinfo.h"
#include <engine.h>
//#include <multiarray.h>

#include "mexplus.h"
#include <mexplus/mxarray.h>
using mexplus::MxArray;

class MatlabExchanger : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "MatlabExchanger");//Can't use defines here!, moc doesn't handle defines, not needed here

public:
	MatlabExchanger(QObject *parent = NULL);
	~MatlabExchanger();
	MatlabExchanger(const MatlabExchanger& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);
	void setScriptEngine(QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	bool openEngine();
	bool closeEngine();
	void setVisibility(const bool &bSetVisible);
	bool evaluateString(const QString &sCommand);
	QString getOutput();
	//bool putVariabele(const QString &sVarName, MultiArray *pArray, const int &nSizeM, const int &nSizeN);
	QScriptValue getVariabele(const QString &sVarName);

private:
	QScriptEngine* currentScriptEngine;
	Engine *m_pEngine;
	char m_output[4096];
};

#endif // MatlabExchanger_H
