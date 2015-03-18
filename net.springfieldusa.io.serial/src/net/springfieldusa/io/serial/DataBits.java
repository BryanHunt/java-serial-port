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
 * The number of data bits that can be set on the serial port.
 * 
 * @author bhunt
 * @since 1.0
 *
 */
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