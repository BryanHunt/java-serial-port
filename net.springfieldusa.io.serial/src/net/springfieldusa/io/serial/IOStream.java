package net.springfieldusa.io.serial;

import java.io.Closeable;
import java.io.IOException;

public class IOStream implements Closeable
{
	private long stream = 0;

	static
	{
		System.loadLibrary("io.serial.native");
	}

	public static void main(String[] args) throws InterruptedException
	{
		try (IOStream stream = new IOStream("/dev/tty.usbserial"))
		{
			stream.setBaud(BaudRate.B115200);
			stream.setParity(Parity.NONE);
			stream.setDataBits(DataBits.EIGHT);
			stream.setStopBits(StopBits.ONE);
			stream.setUseFlowControl(FlowControl.NO);
			
			byte[] out = new String("\n").getBytes();
			stream.write(out);

			Thread.sleep(100);
			byte[] in = new byte[1024];
			stream.read(in);
			
			System.out.println(new String(in));			
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}

	public IOStream(String deviceName) throws IOException
	{
		if (stream != 0)
			throw new IOException("The stream is already open");

		stream = open(deviceName);
	}

	public int read(byte[] buffer) throws IOException
	{
		checkStream();
		return read(stream, buffer);
	}

	public int write(byte[] buffer) throws IOException
	{
		checkStream();
		return write(stream, buffer);
	}

	@Override
	public void close() throws IOException
	{
		try
		{
			if (stream != 0)
				close(stream);
		}
		finally
		{
			stream = 0;
		}
	}

	public void setBaud(BaudRate baudRate) throws IOException
	{
		checkStream();
		setBaud(stream, baudRate.getNativeValue());
	}

	public void setParity(Parity parity) throws IOException
	{
		checkStream();
		setParity(stream, parity.getParity());
	}
	
	public void setDataBits(DataBits dataBits) throws IOException
	{
		checkStream();
		setDataBits(stream, dataBits.getValue());
	}
	
	public void setStopBits(StopBits stopBits) throws IOException
	{
		checkStream();
		setStopBits(stream, stopBits.getNumberBits());
	}
	
	public void setUseFlowControl(FlowControl mode) throws IOException
	{
		checkStream();
		setUseFlowControl(stream, mode.getFlowControl());
	}

	public int getStatus() throws IOException
	{
		checkStream();
		return getStatus(stream);
	}
	
	private void checkStream() throws IOException
	{
		if (stream == 0)
			throw new IOException("The stream is not open");		
	}
	
	private native long open(String deviceName) throws IOException;

	private native int read(long stream, byte[] buffer) throws IOException;

	private native int write(long stream, byte[] buffer) throws IOException;

	private native void close(long stream) throws IOException;

	private native void setBaud(long stream, int baudRate);
	
	private native void setParity(long stream, int parity);
	
	private native void setDataBits(long stream, int numberDataBits);
	
	private native void setStopBits(long stream, int numberStopBits);
	
	private native void setUseFlowControl(long stream, boolean mode);
	
	private native int getStatus(long stream);
}
