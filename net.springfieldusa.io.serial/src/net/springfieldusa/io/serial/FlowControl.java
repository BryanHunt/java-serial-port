package net.springfieldusa.io.serial;

public enum FlowControl
{
	YES(true), NO(false);
	
	private boolean flowControl;
	
	private FlowControl(boolean flowControl)
	{
		this.flowControl = flowControl;
	}
	
	public boolean getFlowControl()
	{
		return flowControl;
	}
}