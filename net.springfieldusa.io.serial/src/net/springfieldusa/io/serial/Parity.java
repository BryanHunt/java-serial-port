package net.springfieldusa.io.serial;

public enum Parity
{
	NONE(0), EVEN(2), ODD(1);
	
	private int parity;
	
	private Parity(int parity)
	{
		this.parity = parity;
	}
	
	public int getParity()
	{
		return parity;
	}
}