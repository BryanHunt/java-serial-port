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
import java.io.OutputStream;

import net.springfieldusa.io.serial.IOStream;

public class SerialOutputStream extends OutputStream
{
	private IOStream stream;

	public SerialOutputStream(IOStream stream)
	{
		this.stream = stream;
	}

	@Override
	public void write(byte[] b) throws IOException
	{
		stream.write(b);
	}

	@Override
	public void write(int b) throws IOException
	{
		byte[] buffer = new byte[] { (byte) b };
		stream.write(buffer);
	}
}
