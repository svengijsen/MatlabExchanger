//Construct a new objects
var objMatlabExchanger = new MatlabExchanger();
var MultiArrayobj = new MultiArray(3,2);//Rows, Columns
var tCleanupTimer = new QTimer();

tCleanupTimer.singleshot = true;
tCleanupTimer.timeout.connect(this, CleanupScript);

objMatlabExchanger.openEngine();
Log("Rows: " + MultiArrayobj.rowCount());
Log("Columns: " + MultiArrayobj.columnCount());
MultiArrayobj.setColumnCount(8);
Log("Columns: " + MultiArrayobj.columnCount());
MultiArrayobj.removeColumn(4);
Log("Columns: " + MultiArrayobj.columnCount());
var arrItem = new QStandardItem();
arrItem.setData(true);
arrItem.setText("abc");
MultiArrayobj.setItem(1,1,arrItem);
if(MultiArrayobj.item(1,1))
{
	Log("-Data: " + MultiArrayobj.item(1,1).data());
	Log("-Text: " + MultiArrayobj.item(1,1).text());
}

//objMatlabExchanger.setVisibility(false);
//objMatlabExchanger.putVariabele("X",MultiArrayobj,MultiArrayobj.rowCount(),MultiArrayobj.columnCount());
//Log("-Data: " + objMatlabExchanger.getVariabele("X",MultiArrayobj,MultiArrayobj.rowCount(),MultiArrayobj.columnCount()).item(0,0).data());

objMatlabExchanger.evaluateString("Test = 1 + 3;");
tCleanupTimer.start(2000);

function CleanupScript()
{
	tCleanupTimer.timeout.disconnect(this, CleanupScript);
	Log(objMatlabExchanger.getOutput());
	objMatlabExchanger.closeEngine();
	//Set the constructed objects to null
	MultiArrayobj = null;
	objMatlabExchanger = null;
	tCleanupTimer = null;
	CleanupScript = null;
}