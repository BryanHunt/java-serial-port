package net.springfieldusa.io.serial;

import java.io.IOException;
import java.util.Collection;

public interface SerialPortService {
  Collection<String> getAvailablePorts() throws IOException;
}
