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
 * The values of stop bits that can be set on the serial port.
 * 
 * @author bhunt
 *
 */
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