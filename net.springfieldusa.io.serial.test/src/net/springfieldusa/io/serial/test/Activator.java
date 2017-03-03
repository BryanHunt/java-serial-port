package net.springfieldusa.io.serial.test;

import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;

import net.springfieldusa.io.serial.BaudRate;
import net.springfieldusa.io.serial.IOStream;

public class Activator implements BundleActivator {

  @Override
  public void start(BundleContext context) throws Exception {
    System.out.println("Starting serial port test");
    IOStream stream = new IOStream("/dev/ttyAMA0");
    System.out.println("Opened stream");
    stream.setBaud(BaudRate.B115200);
    String greeting = "Hello serial port";
    stream.write(greeting.getBytes());
    System.out.println("Sent greeting");
    stream.close();
    System.out.println("All done");
  }

  @Override
  public void stop(BundleContext context) throws Exception {
    System.out.println("Stopping serial port test");
  }

}
