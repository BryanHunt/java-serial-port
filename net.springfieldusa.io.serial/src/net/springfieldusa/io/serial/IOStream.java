package net.springfieldusa.io.serial;

import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import net.springfieldusa.io.serial.stream.SerialInputStream;
import net.springfieldusa.io.serial.stream.SerialOutputStream;

public class IOStream implements Closeable
{
	private long stream = 0;

	static
	{
		System.loadLibrary("io.serial.native");
	}

	public IOStream(String deviceName) throws IOException
	{
		if (stream != 0)
			throw new IOException("The stream is already open");

		stream = open(deviceName);
	}

	public InputStream createInputStream() throws IOException
	{
		checkStream();
		return new SerialInputStream(this);
	}
	
	public OutputStream createOuputStream() throws IOException
	{
		checkStream();
		return new SerialOutputStream(this);
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

	public void setReadMode(int numberBytesToWaitFor, int timeoutBetweenBytes) throws IOException
	{
		checkStream();
		setReadMode(stream, numberBytesToWaitFor, timeoutBetweenBytes);
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
	
	private native void setReadMode(long stream, int numberBytesToWaitFor, int timeoutBetweenBytes);
	
	private native int getStatus(long stream);
}
