package net.springfieldusa.io.serial;

public enum DataBits
{
	FIVE(5), SIX(6), SEVEN(7), EIGHT(8);
	
	private int value;
	
	private DataBits(int value)
	{
		this.value = value;
	}
	
	public int getValue()
	{
		return value;
	}
}