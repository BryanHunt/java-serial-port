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
 * The values of the flow control mode that can be set on the serial port.
 * 
 * @author bhunt
 * @since 1.0
 *
 */
public enum FlowControl
{
	YES(true), NO(false);
	
	private boolean flowControl;
	
	private FlowControl(boolean flowControl)
	{
		this.flowControl = flowControl;
	}
	
	public boolean getFlowControl()
	{
		return flowControl;
	}
}