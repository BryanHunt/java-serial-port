package net.springfieldusa.io.serial;

import java.io.Closeable;
import java.io.IOException;

public class IOStream implements Closeable
{
	public static int B115200;
	public static int B9600;
	private long stream = 0;

	static
	{
		System.loadLibrary("net.springfieldusa.io.native");

		// TODO: make the modes enums
		B115200 = IOStream.B115200();
		B9600 = IOStream.B9600();
	}

	public static void main(String[] args) throws InterruptedException
	{
		byte[] magic = new byte[] {0x50, (byte) 0xbb, (byte) 0xff, 0x20, 0x12, 0x07, 0x25};
		
		try (IOStream stream = new IOStream("/dev/tty.usbserial"))
		{
			stream.setBaud(B9600);
			stream.write(magic);
			
			Thread.sleep(100);
			byte[] in = new byte[1024];
			int count = stream.read(in);
			
			System.out.println("Read " + count + " bytes");
			
			for(int i = 0; i < count; i++)
				System.out.printf("%x ", in[i]);
			
			System.out.println();
			
			byte[] out = new byte[] {0x02};
			stream.write(out);
			
			System.out.println("Read " + count + " bytes");
			
			for(int i = 0; i < count; i++)
				System.out.printf("%x ", in[i]);
			
			System.out.println();

			out = new byte[] {0x06};
			stream.write(out);
			
			System.out.println("Read " + count + " bytes");
			
			for(int i = 0; i < count; i++)
				System.out.printf("%x ", in[i]);
			
			System.out.println();
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
		if (stream == 0)
			throw new IOException("The stream is not open");

		return read(stream, buffer);
	}

	public int write(byte[] buffer) throws IOException
	{
		if (stream == 0)
			throw new IOException("The stream is not open");

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

	public void setBaud(int baud) throws IOException
	{
		if (stream == 0)
			throw new IOException("The stream is not open");

		setBaud(stream, baud);
	}

	public int getStatus()
	{
		return getStatus(stream);
	}
	
	public static native int B115200();
	public static native int B9600();

	private native long open(String deviceName) throws IOException;

	private native int read(long stream, byte[] buffer) throws IOException;

	private native int write(long stream, byte[] buffer) throws IOException;

	private native void close(long stream) throws IOException;

	private native void setBaud(long stream, int baud);
	
	private native int getStatus(long stream);
}
