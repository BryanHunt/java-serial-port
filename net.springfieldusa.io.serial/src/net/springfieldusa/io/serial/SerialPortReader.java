/*******************************************************************************
 * Copyright (c) 2017 Bryan Hunt
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

import java.io.IOException;
import java.io.InputStream;
import java.io.Reader;

/**
 * This class implements a simple reader that does not do any character decoding
 * and does not do any buffering.
 * 
 * @author bhunt
 *
 */
public class SerialPortReader extends Reader
{
  private InputStream inputStream;
  
  public SerialPortReader(InputStream inputStream)
  {
    this.inputStream = inputStream;
  }
  
  @Override
  public int read(char[] cbuf, int off, int len) throws IOException
  {
    byte[] buffer = new byte[cbuf.length];
    int bytesRead = inputStream.read(buffer, off, len);
    
    for(int i = 0; i < bytesRead; i++)
      cbuf[i] = (char) buffer[i];
    
    return bytesRead;
  }

  @Override
  public void close() throws IOException
  {
    inputStream.close();
  }
}
