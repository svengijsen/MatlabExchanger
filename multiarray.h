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

#ifndef MULTIARRAY_H
#define MULTIARRAY_H

#include <QObject>
#include <QtScript/QtScript>
#include <QStandardItemModel>

class MultiArray : public QStandardItemModel
{
	Q_OBJECT

public:
	MultiArray(QObject *parent = NULL);
	MultiArray(int nRows, int nColumns, QObject *parent = NULL);
	~MultiArray();

	static QScriptValue ctor__MultiArray(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue MultiArrayToScriptValue(QScriptEngine *engine, MultiArray* const &s);
	static void MultiArrayFromScriptValue(const QScriptValue &obj, MultiArray* &s);

public slots:

private:
	
};
Q_DECLARE_METATYPE(MultiArray*);

#endif // MULTIARRAY_H
