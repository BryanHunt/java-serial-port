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

/**
 * The baud rate values that may be selected for the serial port.
 * 
 * @author bhunt
 * @since 1.0
 *
 */
public enum BaudRate
{
	B0(0),
	B50(50),
	B75(75),
	B110(110),
	B134(134),
	B150(150),
	B200(200),
	B300(300),
	B600(600),
	B1200(1200),
	B1800(1800),
	B2400(2400),
	B4800(4800),
	B9600(9600),
	B7200(7200),
	B14400(14400),
	B19200(19200),
	B28800(28800),
	B38400(38400),
	B57600(57600),
	B115200(115200),
	B230400(230400);

	private BaudRate(int nativeValue)
	{
		this.nativeValue = nativeValue;
	}
	
	public int getNativeValue()
	{
		return nativeValue;
	}
	
	private int nativeValue;
}