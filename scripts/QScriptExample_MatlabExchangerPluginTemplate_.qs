//Construct a new Plugin object
var MatlabExchangerobject = new MatlabExchanger();

//Test whether we can invoke some default implemented Slots
MatlabExchangerobject.setExampleProperty(99);
Log(MatlabExchangerobject.getExampleProperty());
MatlabExchangerobject.ExampleProperty = 0;
Log(MatlabExchangerobject.ExampleProperty);

//Set the constructed object to null
MatlabExchangerobject = null;