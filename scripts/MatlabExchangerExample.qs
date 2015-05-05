BrainStim.addOutputWindow("MATLAB");
BrainStim.clearOutputWindow("MATLAB");
//Construct a new objects
var mainDialog;
var objMatlabExchanger = new MatlabExchanger();
//var MultiArrayobj = new MultiArray(3,2);//Rows, Columns
//var tCleanupTimer = new QTimer();
//var KeyBoardCaptureObj = new KeyBoardCapture();
//KeyBoardCaptureObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
//KeyBoardCaptureObj.StartCaptureThread(0, true);//, lAllowedKeyCodeList);
//var arrItem = new QStandardItem();
//BrainStim.enableActiveDocument(false);
//tCleanupTimer.singleshot = true;
//tCleanupTimer.timeout.connect(this, CleanupScript);

function Dialog(parent)
{
	QDialog.call(this, parent);
	var frameStyle = QFrame.Sunken | QFrame.Panel;
	var layout = new QGridLayout;
	layout.setColumnStretch(1, 1);	
	layout.setColumnMinimumWidth(1, 500);
	
	this.openEngineLabel = new QLabel;
	this.openEngineLabel.setFrameStyle(frameStyle);
	this.openEngineButton = new QPushButton(tr("Open Matlab Engine"));	
	layout.addWidget(this.openEngineButton, 0, 0);
	layout.addWidget(this.openEngineLabel, 0, 1);
	/////////////////////////////////////////////////////
	this.evaluateStringLineEdit = new QLineEdit;
	this.evaluateStringButton = new QPushButton(tr("Evaluate(matlab script)"));
	layout.addWidget(this.evaluateStringButton, 1, 0);
	layout.addWidget(this.evaluateStringLineEdit, 1, 1);
	this.evaluateStringLineEdit.text = "x=[12, 62, 93, -8, 22; 16, 2, 87, 43, 91; -4, 17, -72, 95, 6]";
	//Examples:
	//	whos x
	/////////////////////////////////////////////////////
	this.getVariableLineEdit = new QLineEdit;
	this.getVariableButton = new QPushButton(tr("Get Variabele"));	
	layout.addWidget(this.getVariableButton, 2, 0);
	layout.addWidget(this.getVariableLineEdit, 2, 1);
	this.getVariableLineEdit.text = "x";
	/*
	/////////////////////////////////////////////////////
	this.showNodeLabel = new QLabel;
	this.showNodeLabel.setFrameStyle(frameStyle);
	this.showGraphEditorButton = new QPushButton(tr("Show Experiment(graph editor)"));	
	layout.addWidget(this.showGraphEditorButton, 3, 0);
	layout.addWidget(this.showNodeLabel, 3, 1);
	/////////////////////////////////////////////////////
	this.showTreeLabel = new QLabel;
	this.showTreeLabel.setFrameStyle(frameStyle);
	this.showTreeEditorButton = new QPushButton(tr("Show Experiment(table editor)"));	
	layout.addWidget(this.showTreeEditorButton, 4, 0);
	layout.addWidget(this.showTreeLabel, 4, 1);
	/////////////////////////////////////////////////////	
	this.runLabel = new QLabel;
	this.runLabel.setFrameStyle(frameStyle);
	this.runButton = new QPushButton(tr("Run Experiment"));	
	layout.addWidget(this.runButton, 5, 0);
	layout.addWidget(this.runLabel, 5, 1);
	/////////////////////////////////////////////////////
	this.saveLabel = new QLabel;
	this.saveLabel.setFrameStyle(frameStyle);
	this.saveButton = new QPushButton(tr("Save Experiment"));	
	layout.addWidget(this.saveButton, 6, 0);
	layout.addWidget(this.saveLabel, 6, 1);
*/
	/////////////////////////////////////////////////////
	this.exitLabel = new QLabel;
	this.exitLabel.setFrameStyle(frameStyle);
	this.exitButton = new QPushButton(tr("Exit"));	
	layout.addWidget(this.exitButton, 7, 0);
	layout.addWidget(this.exitLabel, 7, 1);

	this.setLayout(layout);
	this.windowTitle = tr("Menu Dialog");
}

Dialog.prototype = new QDialog();

function tr(s) { return s; }

Dialog.prototype.openEngine = function()
{
	ChangeDialogState("Opening");
	if(objMatlabExchanger.openEngine())
	{
		mainDialog.openEngineLabel.text = "Opened";
	}
	else
	{
		mainDialog.openEngineLabel.text = "";
		ChangeDialogState("Closed");
		return;
	}
	ChangeDialogState("Idle");
}

Dialog.prototype.evaluateMatlabScript = function()
{
	ChangeDialogState("Evaluate");
	if(objMatlabExchanger.evaluateString(mainDialog.evaluateStringLineEdit.text))
		BrainStim.write2OutputWindow(objMatlabExchanger.getOutput(), "MATLAB");
	ChangeDialogState("Idle");
}

Dialog.prototype.getMatlabVariabele = function()
{
	ChangeDialogState("Retrieve");
	var tmpArray = objMatlabExchanger.getVariabele(mainDialog.getVariableLineEdit.text);
	if(tmpArray)
	{
		Log("Returned Array: " + tmpArray);
		if(tmpArray.length>0)
		{
			for(var i=0;i<tmpArray.length;i++)
			{
				Log(tmpArray[i]);
			}
		}
	}	
	ChangeDialogState("Idle");
}
/*
Dialog.prototype.showTreeViewEditor = function()
{
	Log("showTreeViewEditor()");
	if(!ExperimentManagerObj.showTreeviewExperimentDialog(cExperimentStructure_Object))
	{
		Log("~~Failed to show the Experiment Structure");
	}	
//	LogState(cExperimentStructure_Object.getCurrentExperimentState());	
}

Dialog.prototype.showGraphEditor = function()
{
	if(!ExperimentManagerObj.showVisualExperimentDialog(cExperimentStructure_Object)) //cExperimentStructure_Object1
	{
		Log("~~Failed to show the Experiment Structure");
	}	
//	LogState(cExperimentStructure_Object.getCurrentExperimentState());	
}

Dialog.prototype.runExperiment = function()
{
	ChangeDialogState("Run");
	if(!ExperimentManagerObj.runExperiment()) //Try to run the experiment
	{
		Log("~~Failed to run the Experiment");
		ChangeDialogState("Idle");
	}
	else
	{
		ConnectDisconnectExperimentStructure(true);
	}	
}

Dialog.prototype.saveExperiment = function()
{
	ChangeDialogState("Save");
	if(!ExperimentManagerObj.saveExperiment(sExperimentFilePathSaved)) //Try to run the experiment
	{
		Log("~~Failed to save the Experiment");
	}
	else
	{
		Log("Experiment Saved!");
	}	
	ChangeDialogState("Idle");
}
*/

Dialog.prototype.keyPressEvent = function(e /*QKeyEvent e*/)
{
	if(e.key() == Qt.Key_Escape)
	{
		this.close();
	}
	else
	{
		QDialog.keyPressEvent(e);
	}
}

Dialog.prototype.closeEvent = function() 
{
	Log("Dialog closeEvent() detected!");
	CleanupScript();
}

function ConnectDisconnectScriptFunctions(Connect)
//This function can connect or disconnect all signal/slot connections defined by the boolean parameter 
{
	if(Connect) //This parameter defines whether we should connect or disconnect the signal/slots.
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			mainDialog.openEngineButton["clicked()"].connect(mainDialog, mainDialog.openEngine);
			mainDialog.evaluateStringButton["clicked()"].connect(mainDialog, mainDialog.evaluateMatlabScript);
			mainDialog.getVariableButton["clicked()"].connect(mainDialog, mainDialog.getMatlabVariabele);
			/*mainDialog.showGraphEditorButton["clicked()"].connect(mainDialog, mainDialog.showGraphEditor);
			mainDialog.showTreeEditorButton["clicked()"].connect(mainDialog, mainDialog.showTreeViewEditor);			
			mainDialog.runButton["clicked()"].connect(mainDialog, mainDialog.runExperiment);
			mainDialog.saveButton["clicked()"].connect(mainDialog, mainDialog.saveExperiment);*/
			mainDialog.exitButton["clicked()"].connect(this, this.CleanupScript);			
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e); //If a connection fails warn the user!
		}
	}
	else
	{
		Log("... Disconnecting Signal/Slots");
		try 
		{	
			mainDialog.openEngineButton["clicked()"].disconnect(mainDialog, mainDialog.openEngine);
			mainDialog.evaluateStringButton["clicked()"].disconnect(mainDialog, mainDialog.evaluateMatlabScript);
			mainDialog.getVariableButton["clicked()"].disconnect(mainDialog, mainDialog.getMatlabVariabele);
			/*mainDialog.showGraphEditorButton["clicked()"].disconnect(mainDialog, mainDialog.showGraphEditor);
			mainDialog.showTreeEditorButton["clicked()"].disconnect(mainDialog, mainDialog.showTreeViewEditor);				
			mainDialog.runButton["clicked()"].disconnect(mainDialog, mainDialog.runExperiment);
			mainDialog.saveButton["clicked()"].disconnect(mainDialog, mainDialog.saveExperiment);*/
			mainDialog.exitButton["clicked()"].disconnect(this, this.CleanupScript);			
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e); //If a disconnection fails warn the user!
		}		
	}	
}

function CleanupScript()
{
	//Pre
	if(objMatlabExchanger.closeEngine())
	{
		ChangeDialogState("Closed");
	}
	mainDialog.close();
	ConnectDisconnectScriptFunctions(false);
	//Functions
	ChangeDialogState = null;
	ConnectDisconnectScriptFunctions = null;
	CleanupScript = null;	
	//Objects
	mainDialog = null;
	//Dialog
	Dialog.prototype.keyPressEvent = null;
	Dialog.prototype.closeEvent = null;	
	Dialog.prototype.openEngine = null;
	Dialog.prototype.evaluateMatlabScript = null;
	Dialog.prototype.getMatlabVariabele = null;
	/*Dialog.prototype.showGraphEditor = null;
	Dialog.prototype.showTreeViewEditor = null;
	Dialog.prototype.runExperiment = null;
	Dialog.prototype.saveExperiment = null;
	*/
	Dialog.prototype = null;
	Dialog = null;
	tr = null;	
	//Post
	Log("\nFinished script cleanup, ready for garbage collection!");
	//Beep();
	BrainStim.cleanupScript();	
}

function ChangeDialogState(sType)
{
	//sType corresponds to a button:
	if(sType == "Opening")
	{
		mainDialog.setWindowTitle("BrainStim - Opening...");
		mainDialog.setEnabled(false);
	}
	else if(sType == "Evaluate")
	{
		mainDialog.setWindowTitle("BrainStim - Evaluating...");
		mainDialog.setEnabled(false);
	}
	else if(sType == "Retrieve")
	{
		mainDialog.setWindowTitle("BrainStim - Retrieving...");
		mainDialog.setEnabled(false);
	}
	/*else if(sType == "Run")
	{
		mainDialog.setWindowTitle("BrainStim - Running...");
		mainDialog.setEnabled(false);
	}
	else if(sType == "Save")
	{
		mainDialog.setWindowTitle("BrainStim - Saving...");
		mainDialog.setEnabled(false);
	}
	*/
	else if((sType == "Idle")||(sType == "Closed"))
	{
		mainDialog.setEnabled(true);
		var bEnableControls = false;
		if(sType == "Idle")
		{
			mainDialog.setWindowTitle("BrainStim - Idle...");
			bEnableControls = true;
		}
		else if(sType == "Closed")
		{
			mainDialog.setWindowTitle("BrainStim - No Connection...");
			bEnableControls = false;
		}
		mainDialog.evaluateStringLineEdit.setEnabled(bEnableControls);
		mainDialog.evaluateStringButton.setEnabled(bEnableControls);
		mainDialog.getVariableButton.setEnabled(bEnableControls);
		mainDialog.getVariableLineEdit.setEnabled(bEnableControls);
	}
}

mainDialog = new Dialog();
mainDialog.show();
ChangeDialogState("Closed");
ConnectDisconnectScriptFunctions(true);



















/*
objMatlabExchanger.openEngine();
Log("Rows: " + MultiArrayobj.rowCount());
Log("Columns: " + MultiArrayobj.columnCount());
MultiArrayobj.setColumnCount(8);
Log("Columns: " + MultiArrayobj.columnCount());
MultiArrayobj.removeColumn(4);
Log("Columns: " + MultiArrayobj.columnCount());
arrItem.setData(true);
arrItem.setText("abc");
MultiArrayobj.setItem(1,1,arrItem);
if(MultiArrayobj.item(1,1))
{
	Log("-Data: " + MultiArrayobj.item(1,1).data());
	Log("-Text: " + MultiArrayobj.item(1,1).text());
}

//objMatlabExchanger.setVisibility(false);

function KeyCaptureDetectFunction(keyCode)
{
	//press keys here: gerrooeoooeeeooopgeg
	Log("A key press was detected: " + keyCode);
	if(keyCode==27)//escape
	{
		CleanupScript();
	}
	else if(keyCode==69)//'e'
	{
		if(objMatlabExchanger.evaluateString("X = 1 + 3"))
			Log(objMatlabExchanger.getOutput());
	}
	else if(keyCode==79)//'o'
	{
		Log(objMatlabExchanger.getOutput());
	}
	else if(keyCode==80)//'p'
	{
		objMatlabExchanger.putVariabele("X",MultiArrayobj,MultiArrayobj.rowCount(),MultiArrayobj.columnCount());
	}
	else if(keyCode==71)//'g'
	{
		Log("-Data: " + objMatlabExchanger.getVariabele("X",MultiArrayobj,MultiArrayobj.rowCount(),MultiArrayobj.columnCount()).item(0,0).data());
	}
}

function CleanupScript()
{
	Log("Cleanup - started");
	//tCleanupTimer.timeout.disconnect(this, CleanupScript);
	KeyBoardCaptureObj.StopCaptureThread();
	//BrainStim.enableActiveDocument(true);
	KeyBoardCaptureObj.CaptureThreadKeyPressed.disconnect(this, this.KeyCaptureDetectFunction);
	objMatlabExchanger.closeEngine();
	//Set the constructed objects to null
	arrItem = null;
	MultiArrayobj = null;
	objMatlabExchanger = null;
	//tCleanupTimer = null;
	KeyBoardCaptureObj = null;
	KeyCaptureDetectFunction = null;
	CleanupScript = null;
	BrainStim.restartScriptEngine();
	Log("Cleanup - stopped");
}
*/
