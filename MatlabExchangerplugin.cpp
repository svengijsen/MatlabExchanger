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


#include "MatlabExchangerplugin.h"
#include "MatlabExchanger.h"
#include "defines.h"

Q_DECLARE_METATYPE(MatlabExchanger*)
Q_DECLARE_METATYPE(MatlabExchanger)

MatlabExchangerPlugin::MatlabExchangerPlugin(QObject *parent)
{
	MatlabExchangerObject = new MatlabExchanger(parent);
	MatlabExchangerDiagObject = new MatlabExchanger_Dialog();	
	strPluginInformation = QString(PLUGIN_INTERNAL_NAME) + " Plugin" + "(v" + PLUGIN_FILE_VERSION_STRING + ")";// +" by " + PLUGIN_AUTHOR_NAME;
	strPluginInternalName = PLUGIN_INTERNAL_NAME;
}

MatlabExchangerPlugin::~MatlabExchangerPlugin()
{
	delete MatlabExchangerDiagObject;
	delete MatlabExchangerObject;
}

int MatlabExchangerPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue MatlabExchangerProto = engine.newQObject(MatlabExchangerObject);
	engine.setDefaultPrototype(qMetaTypeId<MatlabExchanger*>(), MatlabExchangerProto);
	QScriptValue MatlabExchangerCtor = engine.newFunction(MatlabExchanger::ctor__extensionname, MatlabExchangerProto);
	engine.globalObject().setProperty(PLUGIN_SCRIPTOBJECT_NAME, MatlabExchangerCtor);
	int nMetaType = qRegisterMetaType<MatlabExchanger>(PLUGIN_SCRIPTOBJECT_CLASS);
	//FinalizeScriptObjectScriptContext(engine, MatlabExchangerObject);
	return nMetaType;
}

bool MatlabExchangerPlugin::ShowGUI()
{
	int returnVal;
	MatlabExchangerDiagObject->setWindowTitle(strPluginInformation);
	returnVal = MatlabExchangerDiagObject->exec();

	switch (returnVal) {
	case QMessageBox::Save:
	   // Save was clicked
	   break;
	case QMessageBox::Discard:
		// Don't Save was clicked
	    break;
	case QMessageBox::Cancel:
	    // Cancel was clicked
	    break;
	default:
	    // should never be reached
	    break;
	}		
	return true;
}

