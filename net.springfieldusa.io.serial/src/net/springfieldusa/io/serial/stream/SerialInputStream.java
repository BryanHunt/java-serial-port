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

package net.springfieldusa.io.serial.stream;

import java.io.IOException;
import java.io.InputStream;

import net.springfieldusa.io.serial.IOStream;

public class SerialInputStream extends InputStream
{
	private IOStream stream;
	
	public SerialInputStream(IOStream stream)
	{
		this.stream = stream;
	}
	
	@Override
	public int read(byte[] b) throws IOException
	{
		return stream.read(b);
	}

	@Override
	public int read() throws IOException
	{
		byte[] buffer = new byte[1];
		stream.read(buffer);
		return buffer[0];
	}
}
