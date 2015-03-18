/*******************************************************************************
 * Copyright (c) 2015 Bryan Hunt
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Bryan Hunt - initial API and implementation
 *******************************************************************************/

package net.springfieldusa.io.serial;

import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import net.springfieldusa.io.serial.stream.SerialInputStream;
import net.springfieldusa.io.serial.stream.SerialOutputStream;

/**
 * This class provides an interface to a native serial IO port.  You may configure
 * the serial port with typical settings: baud rate, parity, data bits, stop bits,
 * and flow control.  This class also provides wrappers to the standard
 * java.io.InputStream and java.io.OutputStream.
 * <p>
 * Example usage setting the serial port to 9600 baud and N81:
 *
 * <pre>
 * <code>
 * IOStream stream = new IOStream("/dev/cu.udbserial");
 * stream.setBaud(BaudRate.B9600);
 * stream.setDataBits(DataBits.EIGHT);
 * stream.setParity(Parity.NONE);
 * stream.setStopBits(StopBits.ONE);
 * stream.setUseFlowControl(FlowControl.NO);
 * stream.setReadMode(1, 10);
 * </code>
 * </pre>
 * 
 * @author bhunt
 * @since 1.0
 *
 */
public class IOStream implements Closeable
{
	private long stream = 0;

	static
	{
		System.loadLibrary("io.serial.native");
	}

	/**
	 * Constructs a new IOStream and connects to the serial port.
	 * 
	 * @param deviceName the system device name - for example: /dev/cu.usbserial
	 * @throws IOException if there is a problem opening the device
	 */
	public IOStream(String deviceName) throws IOException
	{
		if (stream != 0)
			throw new IOException("The stream is already open");

		stream = open(deviceName);
	}

	/**
	 * Creates a java.io.InputStream wrapper around the IOStream.
	 * Calling close on the input stream will NOT close the IOStream.
	 * 
	 * @return the input stream wraper
	 * @throws IOException if the IOStream is closed
	 */
	public InputStream createInputStream() throws IOException
	{
		checkStream();
		return new SerialInputStream(this);
	}
	
	/**
	 * Creates a java.io.OutputStream wrapper around the IOStream.
	 * Calling close on the output stream will NOT close the IOStream.
	 * 
	 * @return the output stream wrapper
	 * @throws IOException if the IOStream is closed
	 */
	public OutputStream createOuputStream() throws IOException
	{
		checkStream();
		return new SerialOutputStream(this);
	}
	
	/**
	 * Reads data from the serial port.  The size of the buffer is
	 * the number of bytes requested from the serial port.
	 * 
	 * @param buffer the data read from the serial port
	 * @return the number of bytes actually read from the serial port
	 * @throws IOException if there was a problem reading the serial port
	 */
	public int read(byte[] buffer) throws IOException
	{
		checkStream();
		return read(stream, buffer);
	}

	/**
	 * Writes data to the serial port.  The size of the buffer is
	 * the number of bytes written to the serial port.
	 * 
	 * @param buffer the data to write to the serial port
	 * @return the number of bytes actually written to the serial port
	 * @throws IOException if there was a problem writing to the serial port
	 */
	public int write(byte[] buffer) throws IOException
	{
		checkStream();
		return write(stream, buffer);
	}

	/**
	 * Flushes the serial port based on the mode.
	 * 
	 * @param mode the flush mode
	 * @throws IOException if there was a problem flushing the serial port
	 */
	public void flush(FlushMode mode) throws IOException
	{
		checkStream();
		flush(stream, mode.ordinal());
	}
	
	/**
	 * Closes the serial port.
	 */
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

	/**
	 * Sets the baud rate on the serial port.
	 * 
	 * @param baudRate the baud rate to set
	 * @throws IOException if there was a problem setting the baud rate
	 */
	public void setBaud(BaudRate baudRate) throws IOException
	{
		checkStream();
		setBaud(stream, baudRate.getNativeValue());
	}

	/**
	 * Sets the parity on the serial port.
	 * 
	 * @param parity the parity to set
	 * @throws IOException if there was a problem setting the parity
	 */
	public void setParity(Parity parity) throws IOException
	{
		checkStream();
		setParity(stream, parity.getParity());
	}
	
	/**
	 * Sets the number of data bits on the serial port.
	 * 
	 * @param dataBits the number of data bits to set
	 * @throws IOException if there was a problem setting the data bits
	 */
	public void setDataBits(DataBits dataBits) throws IOException
	{
		checkStream();
		setDataBits(stream, dataBits.getValue());
	}
	
	/**
	 * Sets the number of stop bits on the serial port.
	 * 
	 * @param stopBits the number of stop bits to set
	 * @throws IOException if there was a problem setting the stop bits
	 */
	public void setStopBits(StopBits stopBits) throws IOException
	{
		checkStream();
		setStopBits(stream, stopBits.getNumberBits());
	}
	
	/**
	 * Enable or disable RTS/CTS flow control based on the mode.
	 * 
	 * @param mode the flow control mode to set
	 * @throws IOException if there was a problem setting the flow control
	 */
	public void setUseFlowControl(FlowControl mode) throws IOException
	{
		checkStream();
		setUseFlowControl(stream, mode.getFlowControl());
	}

	/**
	 * Sets the read mode based on the termios man page where MIN is the 
	 * numberBytesToWaitFor and TIME is the timeoutBetweenBytes
	 * <p>
	 * From man termios:
	 * <p>
	 *   MIN represents the minimum number of bytes that should be received when the read
   *   function successfully returns.  TIME is a timer of 0.1 second granularity that is
   *   used to time out bursty and short term data transmissions.  If MIN is greater than {
   *   MAX_INPUT}, the response to the request is undefined.  The four possible values for
   *   MIN and TIME and their interactions are described below.
   * <p>
   * Case A: MIN &gt; 0, TIME &gt; 0
   * <p>
   *   In this case TIME serves as an inter-byte timer and is activated after the first byte
   *   is received.  Since it is an inter-byte timer, it is reset after a byte is received.
   *   The interaction between MIN and TIME is as follows:  as soon as one byte is received,
   *   the inter-byte timer is started.  If MIN bytes are received before the inter-byte
   *   timer expires (remember that the timer is reset upon receipt of each byte), the read
   *   is satisfied.  If the timer expires before MIN bytes are received, the characters
   *   received to that point are returned to the user.  Note that if TIME expires at least
   *   one byte is returned because the timer would not have been enabled unless a byte was
   *   received.  In this case (MIN &gt; 0, TIME &gt; 0) the read blocks until the MIN and TIME
   *   mechanisms are activated by the receipt of the first byte, or a signal is received.
   *   If data is in the buffer at the time of the read(), the result is as if data had been
   *   received immediately after the read().
   * <p>
   * Case B: MIN &gt; 0, TIME = 0
   * <p>
   *   In this case, since the value of TIME is zero, the timer plays no role and only MIN
   *   is significant.  A pending read is not satisfied until MIN bytes are received (i.e.,
   *   the pending read blocks until MIN bytes are received), or a signal is received.  A
   *   program that uses this case to read record-based terminal I/O may block indefinitely
   *   in the read operation.
   * <p>
   * Case C: MIN = 0, TIME &gt; 0
   * <p>
   *   In this case, since MIN = 0, TIME no longer represents an inter-byte timer.  It now
   *   serves as a read timer that is activated as soon as the read function is processed.
   *   A read is satisfied as soon as a single byte is received or the read timer expires.
   *   Note that in this case if the timer expires, no bytes are returned.  If the timer
   *   does not expire, the only way the read can be satisfied is if a byte is received.  In
   *   this case the read will not block indefinitely waiting for a byte; if no byte is
   *   received within TIME*0.1 seconds after the read is initiated, the read returns a
   *   value of zero, having read no data.  If data is in the buffer at the time of the
   *   read, the timer is started as if data had been received immediately after the read.
   * <p>
   * Case D: MIN = 0, TIME = 0
   * <p>
   *   The minimum of either the number of bytes requested or the number of bytes currently
   *   available is returned without waiting for more bytes to be input.  If no characters
   *   are available, read returns a value of zero, having read no data.
   * 
	 * @param numberBytesToWaitFor the number of bytes to wait for before returning from a read
	 * @param timeoutBetweenBytes the time to wait for the next byte
	 * @throws IOException if there was a problem setting the read mode
	 */
	public void setReadMode(int numberBytesToWaitFor, int timeoutBetweenBytes) throws IOException
	{
		checkStream();
		setReadMode(stream, numberBytesToWaitFor, timeoutBetweenBytes);
	}
	
	/**
	 * Get the value of the serial port control lines.  Available mainly for debug
	 * since the value is platform specific.
	 * 
	 * DSR
	 * DTR
	 * RTX
	 * Secondary TXD
	 * Secondary RXD
	 * CTS
	 * DCD
	 * RNG
	 * DSR
	 * 
	 * @return the native bit encoded status
	 * @throws IOException if there was a problem reading the status
	 */
	public int getStatus() throws IOException
	{
		checkStream();
		return getStatus(stream);
	}
	
	/**
	 * Prints the serial port configuration to the console.  Available mainly for debug.
	 * 
	 * @throws IOException if the stream is not open
	 */
	public void dumpConfig() throws IOException
	{
		checkStream();
		dumpConfig(stream);
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

	private native void flush(long stream, int mode) throws IOException;
	
	private native void setBaud(long stream, int baudRate);
	
	private native void setParity(long stream, int parity);
	
	private native void setDataBits(long stream, int numberDataBits);
	
	private native void setStopBits(long stream, int numberStopBits);
	
	private native void setUseFlowControl(long stream, boolean mode);
	
	private native void setReadMode(long stream, int numberBytesToWaitFor, int timeoutBetweenBytes);
	
	private native int getStatus(long stream);
	
	private native void dumpConfig(long stream);
}
