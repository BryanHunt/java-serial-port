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
 * The values of parity that can be set on the serial port.
 * 
 * @author bhunt
 *
 */
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