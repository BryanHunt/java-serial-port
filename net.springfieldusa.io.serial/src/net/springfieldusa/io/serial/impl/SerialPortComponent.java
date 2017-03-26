package net.springfieldusa.io.serial.impl;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

import org.osgi.service.component.annotations.Component;

import net.springfieldusa.io.serial.SerialPortService;

@Component(service = SerialPortService.class)
public class SerialPortComponent implements SerialPortService {

  @Override
  public Collection<String> getAvailablePorts() throws IOException {
    Collection<String> ports = new ArrayList<>();
    File dev = new File("/dev");
    File[] files = dev.listFiles((dir, name) -> name.startsWith("cu.") && name.indexOf("Bluetooth") == -1);
    
    for(File file : files)
      ports.add(file.getAbsolutePath());
    
    return ports;
  }
}
