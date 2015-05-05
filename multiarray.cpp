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

#include "multiarray.h"

MultiArray::MultiArray(QObject *parent) : QStandardItemModel(parent)
{

}

MultiArray::MultiArray(int nRows, int nColumns, QObject *parent) : QStandardItemModel(nRows,nColumns,parent)
{

}

MultiArray::~MultiArray()
{

}

QScriptValue MultiArray::ctor__MultiArray(QScriptContext* context, QScriptEngine* engine)
{
	int nArgCount = context->argumentCount();
	QScriptValue scriptObj = NULL;
	//Now call the below real Object constructor
	if (nArgCount < 2)
		scriptObj = engine->newQObject(new MultiArray(), QScriptEngine::ScriptOwnership);
	else
		scriptObj = engine->newQObject(new MultiArray(context->argument(0).toInteger(), context->argument(1).toInteger()), QScriptEngine::ScriptOwnership);
	return scriptObj;
}

QScriptValue MultiArray::MultiArrayToScriptValue(QScriptEngine *engine, MultiArray* const &s)
{
	int nItemCount = s->rowCount();
	QScriptValue objScriptVal = engine->newArray(nItemCount);
	for (int i = 0; i < nItemCount; i++)
	{
		QVariant tmpVar = s->data(s->index(i, 0));
		objScriptVal.setProperty(i, QScriptValue(tmpVar.toString()));
	}
	return objScriptVal;
}

void MultiArray::MultiArrayFromScriptValue(const QScriptValue &obj, MultiArray* &s)
{
	//QScriptValueIterator itr(obj);
	//while (itr.hasNext())
	//{
	//	itr.next();
		//(*s)[itr.name()] = qscriptvalue_cast<MultiArray::mapped_type>(itr.value());
	//}
	s = qobject_cast<MultiArray*>(obj.toQObject());
	int nRows = s->rowCount();
	int nColumns = s->columnCount();
}

//QVariant MultiArray::getEntry(const int &nRow, const int &nCol)
//{
//	int nOffset = nRow * (nRows * nColumns) + nCol;
//	return vecMatrixStorage[nOffset];
//}

//QVariant MultiArray::getEntry(const int &nRow, const int &nCol)
//{
//	int nOffset = nRow * (nRows * nColumns) + nCol;
//	return vecMatrixStorage[nOffset];
//}

//void MultiArray::insert(const QString& key, const QString& value)
//{
	//QMap<QString, QVariant>::insert(key, value);
	//QList<QList<QVariant> >::insert()
//}