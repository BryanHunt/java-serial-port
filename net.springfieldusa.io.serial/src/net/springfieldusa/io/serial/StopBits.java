package net.springfieldusa.io.serial;

public enum StopBits
{
	ONE(1), TWO(2);
	
	private int numberBits;
	
	private StopBits(int numberBits)
	{
		this.numberBits = numberBits;
	}
	
	public int getNumberBits()
	{
		return numberBits;
	}
}