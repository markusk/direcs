<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>aboutDialog.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>aboutDialog_8cpp</filename>
    <includes id="aboutDialog_8h" name="aboutDialog.h" local="yes" imported="no">aboutDialog.h</includes>
  </compound>
  <compound kind="file">
    <name>aboutDialog.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>aboutDialog_8h</filename>
    <class kind="class">AboutDialog</class>
  </compound>
  <compound kind="file">
    <name>camThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>camThread_8cpp</filename>
    <includes id="camThread_8h" name="camThread.h" local="yes" imported="no">camThread.h</includes>
  </compound>
  <compound kind="file">
    <name>camThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>camThread_8h</filename>
    <class kind="class">CamThread</class>
    <class kind="struct">KOORD_T</class>
  </compound>
  <compound kind="file">
    <name>carmenserial.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>carmenserial_8cpp</filename>
    <member kind="define">
      <type>#define</type>
      <name>_POSIX</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>6ab09c9b8c5aa53912c6036751339752</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>READ_TIMEOUT</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>18e13c9ac88403c42395bb0af487eb2d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_ClearInputBuffer</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>d092f2db3deff1ddf680dfde3b2c64a7</anchor>
      <arglist>(int dev_fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_close</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>b375bfd311c675992307095d2b05ebf1</anchor>
      <arglist>(int dev_fd)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_serial_configure</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>f0f7fcb43e15373b71321a451f907305</anchor>
      <arglist>(int dev_fd, int baudrate, char *parity)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_connect</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>46e1020ebbb96f9cc613e8f86952b16b</anchor>
      <arglist>(int *dev_fd, char *dev_name)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>carmen_serial_numChars</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>51c32b643f5f968325dff6c1def47d39</anchor>
      <arglist>(int dev_fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_readn</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>c3c38db3b1580e3c25fb52f8a9e18d23</anchor>
      <arglist>(int dev_fd, unsigned char *buf, int nChars)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_set_low_latency</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>4c38f7955d2405b1d129f8c78466724c</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_serial_setparms</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>8fabdc86921791f80b82820190b41aa2</anchor>
      <arglist>(int fd, char *baudr, char *par, char *bits, int hwf, int swf)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_serial_setrts</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>ad325bcb5cf2555a6bc3fa88b629128a</anchor>
      <arglist>(int fd __attribute__((unused)))</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_writen</name>
      <anchorfile>carmenserial_8cpp.html</anchorfile>
      <anchor>f9c0cc8d7e0bce59783543be03cfd201</anchor>
      <arglist>(int dev_fd, unsigned char *buf, int nChars)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>carmenserial.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>carmenserial_8h</filename>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_ClearInputBuffer</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>d092f2db3deff1ddf680dfde3b2c64a7</anchor>
      <arglist>(int dev_fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_close</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>b375bfd311c675992307095d2b05ebf1</anchor>
      <arglist>(int dev_fd)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_serial_configure</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>f0f7fcb43e15373b71321a451f907305</anchor>
      <arglist>(int dev_fd, int baudrate, char *parity)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_connect</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>46e1020ebbb96f9cc613e8f86952b16b</anchor>
      <arglist>(int *dev_fd, char *dev_name)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>carmen_serial_numChars</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>51c32b643f5f968325dff6c1def47d39</anchor>
      <arglist>(int dev_fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_readn</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>c3c38db3b1580e3c25fb52f8a9e18d23</anchor>
      <arglist>(int dev_fd, unsigned char *buf, int nChars)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_set_low_latency</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>4c38f7955d2405b1d129f8c78466724c</anchor>
      <arglist>(int fd)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_serial_writen</name>
      <anchorfile>carmenserial_8h.html</anchorfile>
      <anchor>f9c0cc8d7e0bce59783543be03cfd201</anchor>
      <arglist>(int dev_fd, unsigned char *buf, int nChars)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>circuit.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>circuit_8cpp</filename>
    <includes id="circuit_8h" name="circuit.h" local="yes" imported="no">circuit.h</includes>
  </compound>
  <compound kind="file">
    <name>circuit.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>circuit_8h</filename>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
    <class kind="class">Circuit</class>
  </compound>
  <compound kind="file">
    <name>gui.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>gui_8cpp</filename>
    <includes id="gui_8h" name="gui.h" local="yes" imported="no">gui.h</includes>
  </compound>
  <compound kind="file">
    <name>gui.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>gui_8h</filename>
    <includes id="joystickDialog_8h" name="joystickDialog.h" local="yes" imported="no">joystickDialog.h</includes>
    <includes id="settingsDialog_8h" name="settingsDialog.h" local="yes" imported="no">settingsDialog.h</includes>
    <includes id="aboutDialog_8h" name="aboutDialog.h" local="yes" imported="no">aboutDialog.h</includes>
    <includes id="laserScene_8h" name="laserScene.h" local="yes" imported="no">laserScene.h</includes>
    <includes id="QtGLContext_8h" name="QtGLContext.h" local="yes" imported="no">QtGLContext.h</includes>
    <class kind="class">Gui</class>
  </compound>
  <compound kind="file">
    <name>head.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>head_8cpp</filename>
    <includes id="head_8h" name="head.h" local="yes" imported="no">head.h</includes>
  </compound>
  <compound kind="file">
    <name>head.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>head_8h</filename>
    <includes id="servo_8h" name="servo.h" local="yes" imported="no">servo.h</includes>
    <class kind="class">Head</class>
  </compound>
  <compound kind="file">
    <name>heartbeat.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>heartbeat_8cpp</filename>
    <includes id="heartbeat_8h" name="heartbeat.h" local="yes" imported="no">heartbeat.h</includes>
  </compound>
  <compound kind="file">
    <name>heartbeat.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>heartbeat_8h</filename>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
    <class kind="class">Heartbeat</class>
  </compound>
  <compound kind="file">
    <name>inifile.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>inifile_8cpp</filename>
    <includes id="inifile_8h" name="inifile.h" local="yes" imported="no">inifile.h</includes>
  </compound>
  <compound kind="file">
    <name>inifile.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>inifile_8h</filename>
    <class kind="class">Inifile</class>
  </compound>
  <compound kind="file">
    <name>interfaceAvr.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>interfaceAvr_8cpp</filename>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
  </compound>
  <compound kind="file">
    <name>interfaceAvr.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>interfaceAvr_8h</filename>
    <includes id="qextserialport_8h" name="qextserialport.h" local="yes" imported="no">qextserialport.h</includes>
    <class kind="class">InterfaceAvr</class>
  </compound>
  <compound kind="file">
    <name>joystick.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>joystick_8cpp</filename>
    <includes id="joystick_8h" name="joystick.h" local="yes" imported="no">joystick.h</includes>
  </compound>
  <compound kind="file">
    <name>joystick.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>joystick_8h</filename>
    <class kind="class">Joystick</class>
    <member kind="define">
      <type>#define</type>
      <name>NAME_LENGTH</name>
      <anchorfile>joystick_8h.html</anchorfile>
      <anchor>f71324c57f05ff9e24bd384925dd6b17</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>joystickDialog.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>joystickDialog_8cpp</filename>
    <includes id="joystickDialog_8h" name="joystickDialog.h" local="yes" imported="no">joystickDialog.h</includes>
  </compound>
  <compound kind="file">
    <name>joystickDialog.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>joystickDialog_8h</filename>
    <class kind="class">JoystickDialog</class>
  </compound>
  <compound kind="file">
    <name>laser_main.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laser__main_8cpp</filename>
    <includes id="laser__main_8h" name="laser_main.h" local="yes" imported="no">laser_main.h</includes>
    <member kind="function">
      <type>int</type>
      <name>carmen_laser_run</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>579804f22317c981c69d89ac5ae6d073</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_laser_shutdown</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>c68fc7def0ebc520761f4abd947e2f8e</anchor>
      <arglist>(int signo __attribute__((unused)))</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_laser_start</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>128de68f067d6b64c615fea4ad512c23</anchor>
      <arglist>(short int laserScanner)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>check_parameter_settings</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>7c47d98087a9da4c6e03a90290fa20de</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getLaserDistance</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>c5eef2fc918c4bc4fb4986a4f83ddd67</anchor>
      <arglist>(int laser, int angle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getLaserNumReadings</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>dd9d33e8152263e8957a39ebf87c39b0</anchor>
      <arglist>(int laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>interpret_params</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>d9cae1f7f21f5c49e067561aa4d69a38</anchor>
      <arglist>(sick_laser_p laser, char *dev, char *type, double res, char *rem, double fov)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>read_parameters</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>c82408dda1cfa4043aad3b766e115a68</anchor>
      <arglist>(short int laserScanner)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_default_parameters</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>2e97511c1ee18058bb50ce45e8dc8b48</anchor>
      <arglist>(sick_laser_p laser, int laser_num)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_laser_config_structure</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>14ea4039b51d0e3949122cc0dd81d8b1</anchor>
      <arglist>(sick_laser_p laser, carmen_laser_laser_config_t *config)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDevicePort</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>85ff16a78dd86b6982379c82601bcf1e</anchor>
      <arglist>(short int laser, QString serialPort)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>shutdown_laser</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>a7166e8d0bbdf1abbf37fd954a2d6fdb</anchor>
      <arglist>(int x)</arglist>
    </member>
    <member kind="variable">
      <type>sick_laser_t</type>
      <name>laser1</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>7394940c7e22f1f661c8d5ff376c7504</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_laser_config_t</type>
      <name>laser1_config</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>7355de2b68f429b412fdb0fcaf7d0c85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sick_laser_t</type>
      <name>laser2</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>f878506cbfd4593438df0e189244662a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_laser_config_t</type>
      <name>laser2_config</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>e2204d6a6a5ab59e6e94d060d68d5942</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sick_laser_t</type>
      <name>laser3</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>60dec8f931582d45182076ae29250129</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_laser_config_t</type>
      <name>laser3_config</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>3d626bddfe14f9cf8192821e95f91cf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sick_laser_t</type>
      <name>laser4</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>6b7453a1e4248dbe31a64d14c2edc696</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_laser_config_t</type>
      <name>laser4_config</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>6bed6943bdbb9be34ab7b9074d2922ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>sick_laser_t</type>
      <name>laser5</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>21bf3d32d4f2a2ff840b77f574cffda3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_laser_config_t</type>
      <name>laser5_config</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>b14a2d112866b2f052ce75671b3eadff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QString</type>
      <name>laserSerialPort1</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>9c7ab26f8cf399a4c7d7f736985f9394</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>QString</type>
      <name>laserSerialPort2</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>945487d83efb8fc67bf802526d5862ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>quit_signal</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>9fb5fa91e75f81eaf202d61700390dbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_laser1</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>c6b542107e3512a5aca2c0cc13ccdddf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_laser2</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>5f5e550b223ee966ed6f0cfcd76cd369</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_laser3</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>c26b0a905f065a6a61579f94cf18ad91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_laser4</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>c3b66c99184c75a59efb7f33648de5b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_laser5</name>
      <anchorfile>laser__main_8cpp.html</anchorfile>
      <anchor>e9ae28e5a2594f26cc4590f915852218</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>laser_main.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laser__main_8h</filename>
    <includes id="sick_8h" name="sick.h" local="yes" imported="no">sick.h</includes>
    <includes id="laser__messages_8h" name="laser_messages.h" local="yes" imported="no">laser_messages.h</includes>
    <includes id="param__interface_8h" name="param_interface.h" local="yes" imported="no">param_interface.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_FRONT_LASER_NUM</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>b0e399ec74edc2465e6fb70b8cfb62c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER3_NUM</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>6e7d9cb8c65647c957e9a0840a9f515d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER4_NUM</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>474099b2ff4c21254a836f79ae0ae3a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER5_NUM</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>e845db817573e08d21687740e3563838</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_REAR_LASER_NUM</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>f5b4fd4696ac588026ec8a0547b977df</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>carmen_degrees_to_radians</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>33a1f57c7ce5205eb49395f0c2bf62d1</anchor>
      <arglist>(double theta)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_laser_run</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>579804f22317c981c69d89ac5ae6d073</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_laser_shutdown</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>e6551f72066adba5686a7a1f9fa58c26</anchor>
      <arglist>(int x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_laser_start</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>128de68f067d6b64c615fea4ad512c23</anchor>
      <arglist>(short int laserScanner)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>carmen_radians_to_degrees</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>e34f83ac9eeeabbfe91a92791a965442</anchor>
      <arglist>(double theta)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getLaserDistance</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>c5eef2fc918c4bc4fb4986a4f83ddd67</anchor>
      <arglist>(int laser, int angle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getLaserNumReadings</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>dd9d33e8152263e8957a39ebf87c39b0</anchor>
      <arglist>(int laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDevicePort</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>85ff16a78dd86b6982379c82601bcf1e</anchor>
      <arglist>(short int laser, QString serialPort)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>shutdown_laser</name>
      <anchorfile>laser__main_8h.html</anchorfile>
      <anchor>a7166e8d0bbdf1abbf37fd954a2d6fdb</anchor>
      <arglist>(int x)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>laser_messages.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laser__messages_8h</filename>
    <class kind="struct">carmen_laser_laser_config_t</class>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_ALIVE_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>d4df17211ad572fd588f1cfdc1e6d290</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_ALIVE_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>3dea48b5501f07763c627ac02ae142d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_FRONTLASER_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>63748ffae7b8228b38df07ffd81d4dba</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_FRONTLASER_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>8b70a1d079928af1a0b27f7e5cd3d2fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER1_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>57e11ba327e982b1b7e5dd9f7ca21786</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER1_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>f66323a3a0e0bc415656effdc3c49395</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER2_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>196466e829f2ed02b2e2d6109d46c792</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER2_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>4c0bc67dcafe960b3433b80beeb6d5cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER3_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>c98b5a16b3aefa2b69ef1e2a29fb42d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER3_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>d66da7bffc042ecce2a1549e88b1456f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER4_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>2633270f451119e118b24206e4f1a8c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER4_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>3e0c4b2190b4ebfe8dbfcba1263b38dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER5_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>cc58501e46d511e0bc7ccf7080ce7921</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER5_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>c3f4bb56ec494ff5c47bd5014cf0da68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LASER_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>3b91859b8ef01783fe1e0bd99a2aa7d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_REARLASER_FMT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>bdf6c13128ed27d174ef6c3f12065028</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_REARLASER_NAME</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>8c2c53fdb527de3bdad81fedd35ca65a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>carmen_laser_laser_type_t</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>ff50c9c81356e47c89cb022978023382</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_LMS</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>ff50c9c81356e47c89cb022978023382583deb58cbb8f158b364aa1ccb848172</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_PLS</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>ff50c9c81356e47c89cb022978023382759af1cb8264258d45043c005242d2f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HOKUYO_URG</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>ff50c9c81356e47c89cb022978023382fef8f9f1b741eedec33b7bac1e9f1a64</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SIMULATED_LASER</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>ff50c9c81356e47c89cb022978023382c6c7e17f1f92b2b4d0327d37b7a71ec4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UMKNOWN_PROXIMITY_SENSOR</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>ff50c9c81356e47c89cb022978023382d314c06de34d94558209a55654f7658e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>carmen_laser_remission_type_t</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>9d8be3286848d061bed89eac99a9ba59</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REMISSION_NONE</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>9d8be3286848d061bed89eac99a9ba5935452d736c9dd8042a9adbec7e10b911</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REMISSION_DIRECT</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>9d8be3286848d061bed89eac99a9ba5912f1da7bb906b6908d5f476d29234b95</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>REMISSION_NORMALIZED</name>
      <anchorfile>laser__messages_8h.html</anchorfile>
      <anchor>9d8be3286848d061bed89eac99a9ba59aef132ef17df7fb74d9d3411547f6813</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>laserScene.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laserScene_8cpp</filename>
    <includes id="laserScene_8h" name="laserScene.h" local="yes" imported="no">laserScene.h</includes>
  </compound>
  <compound kind="file">
    <name>laserScene.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laserScene_8h</filename>
    <class kind="class">LaserScene</class>
  </compound>
  <compound kind="file">
    <name>laserThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laserThread_8cpp</filename>
    <includes id="laserThread_8h" name="laserThread.h" local="yes" imported="no">laserThread.h</includes>
  </compound>
  <compound kind="file">
    <name>laserThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>laserThread_8h</filename>
    <includes id="laser__main_8h" name="laser_main.h" local="yes" imported="no">laser_main.h</includes>
    <class kind="class">LaserThread</class>
  </compound>
  <compound kind="file">
    <name>motor.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>motor_8cpp</filename>
    <includes id="motor_8h" name="motor.h" local="yes" imported="no">motor.h</includes>
  </compound>
  <compound kind="file">
    <name>motor.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>motor_8h</filename>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
    <class kind="class">Motor</class>
  </compound>
  <compound kind="file">
    <name>mrs.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>mrs_8cpp</filename>
    <includes id="mrs_8h" name="mrs.h" local="yes" imported="no">mrs.h</includes>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchorfile>mrs_8cpp.html</anchorfile>
      <anchor>0ddf1224851353fc92bfbff6f499fa97</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>mrs.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>mrs_8h</filename>
    <includes id="gui_8h" name="gui.h" local="yes" imported="no">gui.h</includes>
    <includes id="joystickDialog_8h" name="joystickDialog.h" local="yes" imported="no">joystickDialog.h</includes>
    <includes id="settingsDialog_8h" name="settingsDialog.h" local="yes" imported="no">settingsDialog.h</includes>
    <includes id="circuit_8h" name="circuit.h" local="yes" imported="no">circuit.h</includes>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
    <includes id="heartbeat_8h" name="heartbeat.h" local="yes" imported="no">heartbeat.h</includes>
    <includes id="sensorThread_8h" name="sensorThread.h" local="yes" imported="no">sensorThread.h</includes>
    <includes id="plotThread_8h" name="plotThread.h" local="yes" imported="no">plotThread.h</includes>
    <includes id="camThread_8h" name="camThread.h" local="yes" imported="no">camThread.h</includes>
    <includes id="motor_8h" name="motor.h" local="yes" imported="no">motor.h</includes>
    <includes id="servo_8h" name="servo.h" local="yes" imported="no">servo.h</includes>
    <includes id="obstacleCheckThread_8h" name="obstacleCheckThread.h" local="yes" imported="no">obstacleCheckThread.h</includes>
    <includes id="inifile_8h" name="inifile.h" local="yes" imported="no">inifile.h</includes>
    <includes id="networkThread_8h" name="networkThread.h" local="yes" imported="no">networkThread.h</includes>
    <includes id="laserThread_8h" name="laserThread.h" local="yes" imported="no">laserThread.h</includes>
    <includes id="joystick_8h" name="joystick.h" local="yes" imported="no">joystick.h</includes>
    <includes id="head_8h" name="head.h" local="yes" imported="no">head.h</includes>
    <class kind="class">Mrs</class>
  </compound>
  <compound kind="file">
    <name>networkThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>networkThread_8cpp</filename>
    <includes id="networkThread_8h" name="networkThread.h" local="yes" imported="no">networkThread.h</includes>
  </compound>
  <compound kind="file">
    <name>networkThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>networkThread_8h</filename>
    <class kind="class">NetworkThread</class>
  </compound>
  <compound kind="file">
    <name>obstacleCheckThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>obstacleCheckThread_8cpp</filename>
    <includes id="obstacleCheckThread_8h" name="obstacleCheckThread.h" local="yes" imported="no">obstacleCheckThread.h</includes>
  </compound>
  <compound kind="file">
    <name>obstacleCheckThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>obstacleCheckThread_8h</filename>
    <includes id="sensorThread_8h" name="sensorThread.h" local="yes" imported="no">sensorThread.h</includes>
    <includes id="laserThread_8h" name="laserThread.h" local="yes" imported="no">laserThread.h</includes>
    <class kind="class">ObstacleCheckThread</class>
  </compound>
  <compound kind="file">
    <name>param_interface.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>param__interface_8h</filename>
    <class kind="struct">carmen_param_t</class>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_DIR</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>023483e7ae8c0412fdcfd12ae51b47b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_DOUBLE</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>8d1d74aa0d7a32ed1d137d6ca36d65a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_EXPERT</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>a5caac8f6645c10b8a0d208752c2daa7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_FILE</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>842b05a91c8d3791b11ab47d2171d095</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>carmen_param_handle_error</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>ae4377f77a7abf3cb18ca233c3f7c54b</anchor>
      <arglist>(error, usage, progname)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_INT</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>65761656ba88692aa7d928889f24f918</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_ONOFF</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>abecfcc04727b66737c3d2a3acd792c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_PARAM_STRING</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>ed95486c0e6beec0bd42f24094b871d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>carmen_param_change_handler_t</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>70e2da521b5ca7833c8167042adcef60</anchor>
      <arglist>)(char *module, char *variable, char *value)</arglist>
    </member>
    <member kind="typedef">
      <type>struct carmen_param_t *</type>
      <name>carmen_param_p</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>f956e38e55b7981a7f722719a585dc51</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>char</type>
      <name>carmen_param_type_t</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>9ddcf993c688fd6e7f6f4bf9f19129ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_allow_unfound_variables</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>6e41918c19891cddb3a11574940a4892</anchor>
      <arglist>(int new_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_are_unfound_variables_allowed</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>2cf1e38389cab1f37ccae662ac0a0bb8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_check_unhandled_commandline_args</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>2f1129e3340a8757c2bb28376b51b1e6</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_check_version</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>76c377f0f31d69e217212c2980ec384a</anchor>
      <arglist>(char *prog_name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_all</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>4b87ab17bd22c166339d8606a252a4d0</anchor>
      <arglist>(char *module, char ***variables, char ***values, int **expert, int *list_length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_double</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>993228c09ce559bc0aed204e1d8aa540</anchor>
      <arglist>(char *variable, double *return_value, int *expert)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>carmen_param_get_error</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>21f09959d582f0ca5bdcc5cc25cb6c38</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_filename</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>bccca79bf82aac8b56ba2afce1b4c2a7</anchor>
      <arglist>(char *variable, char **return_value, int *expert)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_int</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>f39a4611245b8b647fd83a6328304ad6</anchor>
      <arglist>(char *variable, int *return_value, int *expert)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>carmen_param_get_module</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>be6da8aeaecce6a130fd71e45e4f2e92</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_modules</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>2bbd02d191244040f8361aea00148642</anchor>
      <arglist>(char ***modules, int *num_modules)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_onoff</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>e0e3fcf0255e21cb993e72af644ff94b</anchor>
      <arglist>(char *variable, int *return_value, int *expert)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_paramserver_host</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>78dc26760b882e91a7b4db059dc8c3fc</anchor>
      <arglist>(char **hostname)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>carmen_param_get_robot</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>468efe079c86aa5b89d84cfb703b9d4e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_get_string</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>a9be38fe3c51c7f27e8838ad25dc1181</anchor>
      <arglist>(char *variable, char **return_value, int *expert)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_install_params</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>5087616bb0c8b29a8a677b1659fc6098</anchor>
      <arglist>(int argc, char *argv[], carmen_param_p param_list, int num_items)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_load_paramfile</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>97b87a327b12755859c6274551f72c0e</anchor>
      <arglist>(char *filename, char *param_set)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_send_reread</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>6c3c77c0a4cd1167e541c50c3c53b1d6</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_set_double</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>d80bfd7f676be4f7fcd2acd2dd6ce5fd</anchor>
      <arglist>(char *variable, double new_value, double *return_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_set_filename</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>b8cab65ab2d8b8041cf3e34b6641c850</anchor>
      <arglist>(char *variable, char *new_value, char **return_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_set_int</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>ce8759298bfa27deb7bf238f75d089f6</anchor>
      <arglist>(char *variable, int new_value, int *return_value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_set_module</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>6471b2c214afcccf2a77254f30107755</anchor>
      <arglist>(char *new_module_name)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_set_onoff</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>a7c206fd8a944a350e355c745494725b</anchor>
      <arglist>(char *variable, int new_value, int *return_value)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_set_string</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>734024c5e63c5fb219b1a79bb5d05357</anchor>
      <arglist>(char *variable, char *new_value, char **return_value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_set_usage_line</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>e6f535073fe89c25a9307e701e4c3d6c</anchor>
      <arglist>(char *fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>carmen_param_set_variable</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>40a8090a04c34e1bf79b3f908d3caa62</anchor>
      <arglist>(char *variable, char *new_value, char **return_value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_subscribe_dir</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>310d7c8f6e938371f23bd5213f7a6fc9</anchor>
      <arglist>(char *module, char *variable, char **variable_address, carmen_param_change_handler_t handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_subscribe_double</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>1fec7d99465cd708b05b8b5be46f761d</anchor>
      <arglist>(char *module, char *variable, double *variable_address, carmen_param_change_handler_t handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_subscribe_file</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>6a173f4f2ad4a70fc7d3fc79990a79f3</anchor>
      <arglist>(char *module, char *variable, char **variable_address, carmen_param_change_handler_t handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_subscribe_int</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>39fda7c661153204e8ce03257f12bef9</anchor>
      <arglist>(char *module, char *variable, int *variable_address, carmen_param_change_handler_t handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_subscribe_onoff</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>74dd47b67bc12517b199826b0a45ef20</anchor>
      <arglist>(char *module, char *variable, int *variable_address, carmen_param_change_handler_t handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_subscribe_string</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>6cd832c8dea3d64668ed0b65540a5d19</anchor>
      <arglist>(char *module, char *variable, char **variable_address, carmen_param_change_handler_t handler)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>carmen_param_usage</name>
      <anchorfile>param__interface_8h.html</anchorfile>
      <anchor>6be46e1d426c072565728d73606a291f</anchor>
      <arglist>(char *progname, carmen_param_p param_list, int num_items, char *fmt,...)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>plotThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>plotThread_8cpp</filename>
    <includes id="plotThread_8h" name="plotThread.h" local="yes" imported="no">plotThread.h</includes>
  </compound>
  <compound kind="file">
    <name>plotThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>plotThread_8h</filename>
    <includes id="sensorThread_8h" name="sensorThread.h" local="yes" imported="no">sensorThread.h</includes>
    <class kind="class">PlotThread</class>
  </compound>
  <compound kind="file">
    <name>posix_qextserialport.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>posix__qextserialport_8h</filename>
    <includes id="qextserialbase_8h" name="qextserialbase.h" local="yes" imported="no">qextserialbase.h</includes>
    <class kind="class">Posix_QextSerialPort</class>
  </compound>
  <compound kind="file">
    <name>qextserialbase.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>qextserialbase_8h</filename>
    <class kind="struct">_PortSettings</class>
    <class kind="class">QextSerialBase</class>
    <member kind="define">
      <type>#define</type>
      <name>E_BREAK_CONDITION</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>19bf37831728ff3d4e4cd896f90d1ec8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_BUFFER_OVERRUN</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>6fe68fa1ebcf4b57f197a3f4197aa4ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_CAUGHT_NON_BLOCKED_SIGNAL</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>38fe9ecbec8c1ef8ae913c829c5fdcfe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_FRAMING_ERROR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>762322ccbcd786286ff049ad159481b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_INVALID_DEVICE</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>2bdea3e664206ef35c0a092dac553cae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_INVALID_FD</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>2abf2bf23007ebc781ab5b0b165c3436</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_IO_ERROR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>3b76bea6be89989cf24e4576d8a51a48</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_NO_ERROR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>a1f0dfda864fa9d2630645f233e1fdb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_NO_MEMORY</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>f38d07f1af1bfdc44efa5c2ba5f4e1fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_PORT_TIMEOUT</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>2b4bd10fa885cf9b82ef3ebb0c1ff266</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_READ_FAILED</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>193a92bab0339d238bc2cb398df9bba9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_RECEIVE_OVERFLOW</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>057242de580197703ea39d48c94e3e88</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_RECEIVE_PARITY_ERROR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>069d1d0863b70b907afb4ad3003c075f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_TRANSMIT_OVERFLOW</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>6d11119247071eadf061186234c7df8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>E_WRITE_FAILED</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>d1fd3402e19645e257825928ef932869</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LOCK_MUTEX</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>301635981a1c72bb6916fe420670560f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_CTS</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>83fc3db4bb8889b2c07f1665f1540b11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_DCD</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>fd03add890f7d053482e0fd8113be790</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_DSR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>3b54cdd81a74bd55e93a134364f422fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_DTR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>4e5cfd3a911530446f459d9a4407247f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_RI</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>774d5d294f10ce8178b4a0abfd3f795a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_RTS</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>e97aed572f33c519589b8747b0a8165f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_SR</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>5202e173ceac9088b9fe53a37d84a8b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LS_ST</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>d53af40a7a037fe3413faf4ebbf530eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TTY_PORTABILITY_WARNING</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>f8809f6099edfccc43866b97baecc337</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TTY_WARNING</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>444a6a5b44e2621f5c3cb10e5ce058e5</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UNLOCK_MUTEX</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>d8a6dd978fe92b143b15c9643e05b8b1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="typedef">
      <type>enum _BaudRateType</type>
      <name>BaudRateType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>144aa559c986c7aafa4dfe894d63d4da</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _DataBitsType</type>
      <name>DataBitsType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>a0be000e6dc9bb35b8525546bf85d206</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _FlowType</type>
      <name>FlowType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>95773b8a905f0cbaa7b68b67ec4252db</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _NamingConvention</type>
      <name>NamingConvention</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>d7ee3396be7887b05d5581592d3c11a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _ParityType</type>
      <name>ParityType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>f5dbbc19263088a48c7a01b9254d6719</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct _PortSettings</type>
      <name>PortSettings</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>89f2e054e194d4924fd8a058e6865617</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>enum _StopBitsType</type>
      <name>StopBitsType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>7d75bd6211a18c272413dc204a703a88</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_BaudRateType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d48904</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD50</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489040721b0e666074772b33c2ff05f03f6e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD75</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489041cd023b44dceb4b59798e5203534b65b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD110</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890410174371591f621ab26540af3ac992a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD134</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d48904192cb47e215d8642f3e26cd877eb904e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD150</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d48904e7b4b2d0b7e6e309e57c64364328aff8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD200</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489041eff76dffd9e137cbfd3b0563e17348b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD300</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890441f8610ff3d721295c8692d6331bbc86</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD600</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489042a05e5b7c7d350afe149a629af7f07a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD1200</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890486057a69b2752b06b4302ae83c36e7a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD1800</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489041815c0670961c2e5c20cfbd81954d3f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD2400</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489040e653cf3163c75f11560ad0616049076</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD4800</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489041de6d151ff739cebfa6f3aab0d76ec9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD9600</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d48904782c55daa9030c53e266213401c3707a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD14400</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d48904d208c844993c394ce55ed88a7da71d31</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD19200</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890416d650adf414e7a4fceb3f72661a0420</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD38400</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890456bb974293e65da8c97422abd0c514b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD56000</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489046b00b8cf6939c8b517deada3110de1ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD57600</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489040e6f70f222bc4d91276a4a934ef66c48</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD76800</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489049a7f6b7993e77c3603399cb0a58987be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD115200</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890454a5ab3e880bc1722d455e420c0351e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD128000</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d4890435d815a00139f06782b178b82f355f04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>BAUD256000</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>bca0272419e823ceaf8f702e66d489042149280b354c383e6af48949ce3b807f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_DataBitsType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>4323be92b7801daaf65c07afa80b0e1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DATA_5</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>4323be92b7801daaf65c07afa80b0e1ebdcd3759eeee2ffb002bbd8816afc5cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DATA_6</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>4323be92b7801daaf65c07afa80b0e1e26d01dd98519521e747c3716b95b039f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DATA_7</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>4323be92b7801daaf65c07afa80b0e1ec0ea91406eb909c082025299d879eed5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DATA_8</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>4323be92b7801daaf65c07afa80b0e1e719dc653cb3f7c9fbf6c8b8c36d78a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_FlowType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>088153090a7439b1964e366c7585a3b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FLOW_OFF</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>088153090a7439b1964e366c7585a3b3d4c112d4c8487384e429edeb50b01f6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FLOW_HARDWARE</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>088153090a7439b1964e366c7585a3b337eb6a580629a080ce39792149cf4976</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FLOW_XONXOFF</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>088153090a7439b1964e366c7585a3b30d28447e3d97e184d5582f874d3dd92f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_NamingConvention</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WIN_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff29cf5f25a349b0221d19f4426c6b291f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IRIX_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff25976e2bc06bc1760e9d6cdc847888534</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>HPUX_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff2c18cd3077fdfe16ccd394358c61d2b21</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SUN_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff2cd9ef2a2954fab2bcecb73251a4f7270</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DIGITAL_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff2777096b65c86bda9730984939945c1fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FREEBSD_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff2f295939a7a4047a1e80ca61d5663bb1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LINUX_NAMES</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b94fabd37397619e3b5b0922ff41fff26db6275766f3dd87f1662075d062e934</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_ParityType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b6003fc734aec159767817ba749cd5cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PAR_NONE</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b6003fc734aec159767817ba749cd5cfe0df16cbf90ff8b05d96d37094b3980a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PAR_ODD</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b6003fc734aec159767817ba749cd5cf53059ce3f8053f409937ff89a90afee9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PAR_EVEN</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b6003fc734aec159767817ba749cd5cfdcf0e7e3469256fa6eadd03f93e9a5aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PAR_MARK</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b6003fc734aec159767817ba749cd5cf8e6eade7ff411bc2d325be080ce4ad10</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PAR_SPACE</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>b6003fc734aec159767817ba749cd5cf1dd539202244b7b46ea83c168d3a447d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>_StopBitsType</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>dabe044901e1994c4ec63a1c7f618959</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STOP_1</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>dabe044901e1994c4ec63a1c7f6189595a97cec5fc7aa14a4caae4b53d97fcfc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STOP_1_5</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>dabe044901e1994c4ec63a1c7f618959d5199e453f150adf605f4aa44254d31b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>STOP_2</name>
      <anchorfile>qextserialbase_8h.html</anchorfile>
      <anchor>dabe044901e1994c4ec63a1c7f6189599d9d66053c21f5142750606c4af19b61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>qextserialport.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>qextserialport_8h</filename>
    <includes id="win__qextserialport_8h" name="win_qextserialport.h" local="yes" imported="no">win_qextserialport.h</includes>
    <class kind="class">QextSerialPort</class>
    <member kind="define">
      <type>#define</type>
      <name>QextBaseType</name>
      <anchorfile>qextserialport_8h.html</anchorfile>
      <anchor>08dd61f1403f6c1d554b82ce7600bfbf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>qrc_mrs.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>qrc__mrs_8cpp</filename>
    <member kind="function">
      <type></type>
      <name>Q_CONSTRUCTOR_FUNCTION</name>
      <anchorfile>qrc__mrs_8cpp.html</anchorfile>
      <anchor>784ede001d04b31b614a6db16f0f7cf3</anchor>
      <arglist>(QT_MANGLE_NAMESPACE(qInitResources_mrs)) int QT_MANGLE_NAMESPACE(qCleanupResources_mrs)()</arglist>
    </member>
    <member kind="function">
      <type>QT_END_NAMESPACE int QT_MANGLE_NAMESPACE()</type>
      <name>qInitResources_mrs</name>
      <anchorfile>qrc__mrs_8cpp.html</anchorfile>
      <anchor>b55aae3c149e0c206a95d32a9576422a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QT_BEGIN_NAMESPACE bool</type>
      <name>qRegisterResourceData</name>
      <anchorfile>qrc__mrs_8cpp.html</anchorfile>
      <anchor>bd8d83733ca9800d36d9bb41f512929e</anchor>
      <arglist>(int, const unsigned char *, const unsigned char *, const unsigned char *)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>qUnregisterResourceData</name>
      <anchorfile>qrc__mrs_8cpp.html</anchorfile>
      <anchor>54b96c9f44d004fc0ea13bb581f97a71</anchor>
      <arglist>(int, const unsigned char *, const unsigned char *, const unsigned char *)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>QtGLContext.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>QtGLContext_8cpp</filename>
    <includes id="QtGLContext_8h" name="QtGLContext.h" local="yes" imported="no">QtGLContext.h</includes>
  </compound>
  <compound kind="file">
    <name>QtGLContext.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>QtGLContext_8h</filename>
    <class kind="class">QtGLContext</class>
  </compound>
  <compound kind="file">
    <name>sensorThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>sensorThread_8cpp</filename>
    <includes id="sensorThread_8h" name="sensorThread.h" local="yes" imported="no">sensorThread.h</includes>
  </compound>
  <compound kind="file">
    <name>sensorThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>sensorThread_8h</filename>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
    <class kind="class">SensorThread</class>
  </compound>
  <compound kind="file">
    <name>servo.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>servo_8cpp</filename>
    <includes id="servo_8h" name="servo.h" local="yes" imported="no">servo.h</includes>
  </compound>
  <compound kind="file">
    <name>servo.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>servo_8h</filename>
    <includes id="interfaceAvr_8h" name="interfaceAvr.h" local="yes" imported="no">interfaceAvr.h</includes>
    <class kind="class">Servo</class>
  </compound>
  <compound kind="file">
    <name>settingsDialog.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>settingsDialog_8cpp</filename>
    <includes id="settingsDialog_8h" name="settingsDialog.h" local="yes" imported="no">settingsDialog.h</includes>
  </compound>
  <compound kind="file">
    <name>settingsDialog.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>settingsDialog_8h</filename>
    <class kind="class">SettingsDialog</class>
  </compound>
  <compound kind="file">
    <name>sick.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>sick_8cpp</filename>
    <includes id="sick_8h" name="sick.h" local="yes" imported="no">sick.h</includes>
    <member kind="function">
      <type>double</type>
      <name>carmen_get_time</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>169b2f0d20ff1caea6cd20a96978f769</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>cBaudrate</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>8fd04f88e714c3b6bf1c919ee87a3d0f</anchor>
      <arglist>(int baudrate)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>cDataSize</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>e065e67a55e0d97b26ff78dcebb251ea</anchor>
      <arglist>(int numbits)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>cFlowControl</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>448701e3f7da3fd2380cfa71f5487687</anchor>
      <arglist>(int flowcontrol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>cParity</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>53fc82d6e4209513b42a7213ff8060d1</anchor>
      <arglist>(parity_t par)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>cStopSize</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>59dc156aa11e54d12281f65205d51393</anchor>
      <arglist>(int numbits)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>iParity</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>d13108aa60ddfd79a95280f37c3e7072</anchor>
      <arglist>(parity_t par)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>iSoftControl</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>ad8d08affa42f2aa1d78f8ea2599f989</anchor>
      <arglist>(int flowcontrol)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>kernel_minimum_version</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>2b1167e3d16e673aca082a33a6635a8c</anchor>
      <arglist>(int a, int b, int c)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>shutdown_laser</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>a7166e8d0bbdf1abbf37fd954a2d6fdb</anchor>
      <arglist>(int x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_allocate_laser</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>2226630521c0f1f0237ca86abfe50cef</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_check_baudrate</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>78c8f8112a565b743d7af6e5245caa4c</anchor>
      <arglist>(sick_laser_p laser, int brate)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_connect_device</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>e9d0988f86deb12f1c3e182829cdb97b</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_detect_baudrate</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>f10ca37c78f4f416d70e8ed15eb1dd6a</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_handle_laser</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>15193f7f50b3e6e8c9ed6cd528cf7935</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_install_settings</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>acbb3d211d10ce661e823070ded019e1</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_parse_conf_data</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>00a7185241f39820cd9859c94a0a2032</anchor>
      <arglist>(sick_laser_p laser, unsigned char *buf, int length)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_read_data</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>ea834f59d3ae74765e3c0b32acf7a8e7</anchor>
      <arglist>(sick_laser_p laser, unsigned char *data, double timeout)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_request_lms_config</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>2bbeff287864e5a12af73972c93000ef</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_request_sensor</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>54e2f07d33026df0510cc391508da720</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_request_status</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>158cac074fd52527c5b683c015b476a4</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_serial_connect</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>e2f1121210a343aa50dec54ec4f598cd</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_set_baudrate</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>ae041781325376ff20c31a74f4f943a4</anchor>
      <arglist>(sick_laser_p laser, int brate)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_set_config_mode</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>efe5bb8bd552d5112a3c6303794b683e</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_set_laser_baudrate</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>cb00b3c93450611354e3c59519f9a359</anchor>
      <arglist>(sick_laser_p laser, int brate)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_set_lms_config</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>961e2a085e089e9adb5b659371a90cab</anchor>
      <arglist>(sick_laser_p laser, unsigned char *data, int len)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_set_lms_range</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>650914d3f5ef0f2cdb06ef573f5c910e</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_set_lms_resolution</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>dc8ace99627b14a98752e2117f1916d2</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_set_serial_params</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>39c9338a475c80cfaea96c25cd05f363</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_start_continuous_mode</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>7ed5ebf0750d6d0b0885c9de88dea0e2</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_start_continuous_remission_part_mode</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>4b66efbcc090b5e10109dfba608d61b6</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_start_laser</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>e57ea324cbf268f90b7aeff7e8ab515f</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_stop_continuous_mode</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>831c29947ced8804b06e7f621291094a</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_stop_laser</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>22bdfa8f6b4ad0075473f838698c715d</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_testBaudrate</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>c197ec746b7b981c8a9c17ebac2e3146</anchor>
      <arglist>(sick_laser_p laser, int brate)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_valid_packet</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>d9fe592584cb637066d44a7b26816e69</anchor>
      <arglist>(unsigned char *data, long size, long *offset, long *len)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_write_command</name>
      <anchorfile>sick_8cpp.html</anchorfile>
      <anchor>ca3f20381a30059ba593762c2cb46111</anchor>
      <arglist>(sick_laser_p laser, unsigned char command, unsigned char *argument, int arg_length)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>sick.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>sick_8h</filename>
    <includes id="carmenserial_8h" name="carmenserial.h" local="yes" imported="no">carmenserial.h</includes>
    <class kind="struct">laser_device_t</class>
    <class kind="struct">laser_settings_t</class>
    <class kind="struct">sick_laser_t</class>
    <member kind="define">
      <type>#define</type>
      <name>ACK</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>6f6489887e08bff4887d0bc5dcf214d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BUFFER_SIZE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>6b20d41d6252e9871430c242cb1a56e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_LOW_LATENCY</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>59d4cd110942db278fd7e76c4dbfd9cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CARMEN_LASER_USE_SELECT</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>c5b5f9795e6ff7e29533ceb3adcc1b7f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CBREAK</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>cd5bddee635eadaa90cb5eae04c7d251</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CRC16_GEN_POL</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>087c979734116b73cd94fd4f73abdeab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CRC16_GEN_POL0</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>d14020cd74f4d9c6ea05ceb5e4aadce9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CRC16_GEN_POL1</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5ac17411b79307731c39bb5fbcf4f926</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DLE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>dd7018db64fb17dd1e4664b4494be0ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FALSE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>a93f0eb578d23995850d61f7d61c55c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INI</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>b3d6f7cc7deaab046f7412f5b52b25f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LASER_BUFFER_SIZE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>fa8b0f191cda4acad112c52bab164664</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LID</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>59358fb1dba36cd01d0a501e73529a2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LMS_PASSWORD</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>856336d040b00743dc99649dbc2d5b68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>e71449b1cc6e6250b91f539153a7a0d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_COMMAND_SIZE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>b95c4556ea9e5b815392d36c3d7b2ec0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_NAME_LENGTH</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>0c397a708cec89c74029582574516b30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_ACK</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>045fc4fee1a761d49aff6570c3e69a59</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_ANSWER</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>6a92aaaaa6c06fdfaca57596164ee5c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_CLEAR</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7717d12f94bff795035f5a45b7eba4e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_CONFIG</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5ff4a2b00c83dd024502a78bf011fdad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_DATA</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8d1e0eac8071cb4278d1f532b34d97c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_GETTING_CONF</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>1ed04d6b28f58358699b20ce9039caa7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_SENSOR_DATA</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8d832170b04f16fbcebd20289108c660</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TIME_FOR_TESTING_BAUDRATE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>a300b8487be50d891e669485d1b8e36f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAXDOUBLE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>ce724a9256f4e65b352465e5feb24a21</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAXFLOAT</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5e9e29217f6ec61105a4520ec5942225</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NAK</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7ff3e502ffb5d509612c6c6741de45cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PLS_PASSWORD</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7772a0e1d97c97b635af4e4d53c26a87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RAW</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8cbc8c74f4ed0a3d6a415c8d37630e6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RES_0_25_DEGREE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>9585676c9f7e5f9dcd9e22919d9fca87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RES_0_50_DEGREE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>dca115212a39974c0d037a0d1681e97f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RES_1_00_DEGREE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7ea14bcea192e028e4eacb6719e5094a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>STX</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>acd744a917e61146ec8b7175b4761683</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIO</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>a8c9953b722874371f3ba39c08658fdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIOCGETP</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>48cdfd4dc0eadfd6694466b21b232929</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TIOCSETP</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>9f4aefaeb94c4c17ead72493c9786394</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRUE</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>a8cecfc5c5c054d2875c03e77b7be15d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UKN</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>cab34ad4a1eef1ea50c1424981fa52ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>va_copy</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>b1bc77186597ff91cc8ee54e7a56c23d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct laser_device_t *</type>
      <name>laser_device_p</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>e82a4587b7d51b47703134765d2c4d9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct sick_laser_t *</type>
      <name>sick_laser_p</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>73e3d54550d82446e80568e3dce756d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>laser_model_t</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>c20b7bd66071422d59f9304bc7a28d1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PLS</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>c20b7bd66071422d59f9304bc7a28d1cb5cba5a75c4814e67c9eee4dc84877a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LMS</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>c20b7bd66071422d59f9304bc7a28d1c8dcd554f50c053b9aa9b175caa59497f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>parity_t</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8c9f47bdc80c9f4db63b7bf8b1ad5290</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>N</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8c9f47bdc80c9f4db63b7bf8b1ad52902c63acbe79d9f41ba6bb7766e9c37702</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>E</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8c9f47bdc80c9f4db63b7bf8b1ad5290b199e021998d49b1f09338d8b9b18ecb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>O</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>8c9f47bdc80c9f4db63b7bf8b1ad5290f4b8b7698dfedf5562a7db2bdb07010a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>range_dist_t</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5849629b36fe65fbd7d5b41531894842</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_RANGE80M</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5849629b36fe65fbd7d5b41531894842b7c7c62474b722568c46a33171bac054</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_RANGE160M</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5849629b36fe65fbd7d5b4153189484263fa6e60736a90bf86ce6044366f7dd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_RANGE320M</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5849629b36fe65fbd7d5b4153189484280f6eec040a6dbde4e5c4bd0ca61ac72</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_REMISSION_NORM</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5849629b36fe65fbd7d5b41531894842148447e20f23e362735ec3a09c421558</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>SICK_REMISSION_DIRECT</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>5849629b36fe65fbd7d5b41531894842aa2b736f31415becf3f59ff6603342da</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>range_res_t</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7ef7b1da108afad3a5f5cd2f05a0e470</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CM</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7ef7b1da108afad3a5f5cd2f05a0e470e256357c69514a4c50b9a9c857dbe7f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MM</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7ef7b1da108afad3a5f5cd2f05a0e470519a4a8945cef24205e65dbbc404da73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DM</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>7ef7b1da108afad3a5f5cd2f05a0e470999a690381b7476ed2263b21c0dcdadc</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>carmen_get_time</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>169b2f0d20ff1caea6cd20a96978f769</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_connect_device</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>e9d0988f86deb12f1c3e182829cdb97b</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_handle_laser</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>15193f7f50b3e6e8c9ed6cd528cf7935</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sick_start_laser</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>e57ea324cbf268f90b7aeff7e8ab515f</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sick_stop_laser</name>
      <anchorfile>sick_8h.html</anchorfile>
      <anchor>22bdfa8f6b4ad0075473f838698c715d</anchor>
      <arglist>(sick_laser_p laser)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>speakThread.cpp</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>speakThread_8cpp</filename>
    <includes id="speakThread_8h" name="speakThread.h" local="yes" imported="no">speakThread.h</includes>
  </compound>
  <compound kind="file">
    <name>speakThread.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>speakThread_8h</filename>
    <class kind="class">SpeakThread</class>
  </compound>
  <compound kind="file">
    <name>win_qextserialport.h</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>win__qextserialport_8h</filename>
    <includes id="qextserialbase_8h" name="qextserialbase.h" local="yes" imported="no">qextserialbase.h</includes>
    <class kind="class">Win_QextSerialPort</class>
  </compound>
  <compound kind="struct">
    <name>_PortSettings</name>
    <filename>struct__PortSettings.html</filename>
    <member kind="variable">
      <type>BaudRateType</type>
      <name>BaudRate</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>ea4fd44b828e18d59c104d7235065d6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DataBitsType</type>
      <name>DataBits</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>3b8440e0b6725e344c28995683a45c05</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>FlowType</type>
      <name>FlowControl</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>cbe0294e2d614406a7a567088051997d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ParityType</type>
      <name>Parity</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>88351bb498f9a78af58ed68a113e16f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>StopBitsType</type>
      <name>StopBits</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>c38a82e4ecdc567b5278d6ccdfbc69ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ulong</type>
      <name>Timeout_Millisec</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>7c616df72bdc8d28735e95f4817b76a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ulong</type>
      <name>Timeout_Sec</name>
      <anchorfile>struct__PortSettings.html</anchorfile>
      <anchor>b48c1339bdddc8362dac76650daa8579</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>AboutDialog</name>
    <filename>classAboutDialog.html</filename>
    <member kind="function">
      <type></type>
      <name>AboutDialog</name>
      <anchorfile>classAboutDialog.html</anchorfile>
      <anchor>627e1b83dbeb4d2b4f5d77ef3c128fef</anchor>
      <arglist>(QWidget *parent=0, Qt::WFlags fl=0)</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_btnAboutQt_clicked</name>
      <anchorfile>classAboutDialog.html</anchorfile>
      <anchor>c055c4b8cabd6fea533ced88c9c45269</anchor>
      <arglist>(bool checked)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Ui::aboutDialog</type>
      <name>ui</name>
      <anchorfile>classAboutDialog.html</anchorfile>
      <anchor>09973e2e1adc7a6ee6271d2c6f67796a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>CamThread</name>
    <filename>classCamThread.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>drawContactAlarm</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>6c1e768a2a61440b20fb68cd3f2c8a16</anchor>
      <arglist>(char position, bool state)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>enableFaceDetection</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>765e53b83e63b9cc7d7ea7d7988276a4</anchor>
      <arglist>(int state)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>test</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>f1a3ff8f6f634aa46cd48ee2b44cfac9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>camDataComplete</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>3aad7aacf47d8aace3c08cf6b834711e</anchor>
      <arglist>(IplImage *imgPtr)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>disableCamera</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>49ac2987a69bee2c8a8ece915f2e189e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>disableFaceDetection</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>abf6ebe717abb5c2705fd238104e1224</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>faceDetected</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>594a897b3f8e43c7e0f8a07ead2a9604</anchor>
      <arglist>(int faces, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CamThread</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>dc6544efeb00ce345f1200425398f2ab</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>imageHeight</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>8ac063f3a77007fbc7a367cf030494a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>imagePixelDepth</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>b8a1bbea4402430612ad81e65402ce0f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>imageWidth</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>8dbe49e4e201a560717ad0e4715e4362</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>init</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>56d8c5d1f359772a8ac9f61a65e16fa4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>675b1989afd40bd4ecfcc21c9e66ce1e</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>343567ede62f3455081180ac53331134</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCameraDevice</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>f2506cb760e15f6aca44b57f04a8f011</anchor>
      <arglist>(int device)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCascadePath</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>e5e3950aa21c498178408bc26c5a8551</anchor>
      <arglist>(QString haarClassifierCascade)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>1f8e882f462cda28caceb7b9b921d0d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~CamThread</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>f011f0fe45e09c79bd7732bf3bee7b00</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>CvScalar</type>
      <name>hsv2rgb</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>148aac5097a6c2225775c03a3dea1d3b</anchor>
      <arglist>(float hue)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>QImage *</type>
      <name>IplImageToQImage</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>d99c7532ef7a802aa21b145c34abc904</anchor>
      <arglist>(const IplImage *iplImage)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>cameraDevice</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>53f0fe4a2e09fc034b8fdcf1f85ac6d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>cameraIsOn</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>7177809f7f8fe169d41760e0e2f275a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>CvCapture *</type>
      <name>capture</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>d137913f6f404902ad05280a8e314c97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>CvHaarClassifierCascade *</type>
      <name>cascade</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>24f0759c48d748321aa0dbc3e3737198</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>contactAlarmBottom</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>0d2efca7f8ae8ddb47b66bf9f6ac31f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>contactAlarmLeft</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>7719ac2c22f0fa5809db90e7247fafd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>contactAlarmRight</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>89118efcff164ddd487d35e6b63b963d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>contactAlarmTop</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>ab93e05aae1900596deef4e0970cfa30</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QVector&lt; KOORD_T &gt;</type>
      <name>detectedFaces</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>d16cfdc0e16527e214344ac99588301e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>faceDetectionIsEnabled</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>987c212b28c789f3b5bf58e1fb8b1073</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>faceDetectionWasActive</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>1afef6f700c50b92da9f17cf4d6b6a83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>IplImage *</type>
      <name>gray</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>fd21bbcf0d24a5065401f1472f94a7a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>haarClassifierCascadeFilename</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>e367e8a75516ea79bd30532f0ab114b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>height</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>1578c0db1e0a6c6e07651e4836ea8060</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>IplImage *</type>
      <name>imgPtr</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>ea80e2b3eaea8ead1624b6a5552dda14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initDone</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>8cb2ced6f839b727be461b2b035804f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>pixeldepth</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>3b7a2836e3cfa45cc99793eeaad18400</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>IplImage *</type>
      <name>small_img</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>fa4f790edf444c61df7841edf675ff8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>f42a1967e4668e9edbf1657c931c6810</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>CvMemStorage *</type>
      <name>storage</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>a4f412a436b3f3da0d5bbdb28c3c984e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>width</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>4bbddf5b6eb62e424748ffa01de0c764</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>BOTTOM</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>5bf0e24d6bcea1175b3311915cea2f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>FACEARRAYSIZE</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>c66a1f55f32dbd5634b5a33e4cc42b4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>LEFT</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>00b03952be46576c4318a72705ec9030</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>RIGHT</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>d41ca22b9ab1ae1857bc1e30235653fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const double</type>
      <name>scale</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>8d4f5e2f84aef91fbbc165d245252115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>TOP</name>
      <anchorfile>classCamThread.html</anchorfile>
      <anchor>6b954181c0410a88f21442120d94294a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>carmen_laser_laser_config_t</name>
    <filename>structcarmen__laser__laser__config__t.html</filename>
    <member kind="variable">
      <type>double</type>
      <name>accuracy</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>2ad7b5e7c92df50d2a4ca21d01da6530</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>angular_resolution</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>b46cb25e872c1575f5538405302fe9fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>fov</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>d0adcbcd962308826f8221339d5c99c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_laser_type_t</type>
      <name>laser_type</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>5b8ef961de72409d47839c5ea41aefdc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>maximum_range</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>cf8c0df8959510a96ece5a2c01ec683e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_laser_remission_type_t</type>
      <name>remission_mode</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>1a1ce822417cd3ad296bcbdc764ec047</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>start_angle</name>
      <anchorfile>structcarmen__laser__laser__config__t.html</anchorfile>
      <anchor>2087fa866b4f9fba5c6bed3badd92056</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>carmen_param_t</name>
    <filename>structcarmen__param__t.html</filename>
    <member kind="variable">
      <type>carmen_param_change_handler_t</type>
      <name>handler</name>
      <anchorfile>structcarmen__param__t.html</anchorfile>
      <anchor>98e5a7c84ab1f32ac71f17c9040e128a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>module</name>
      <anchorfile>structcarmen__param__t.html</anchorfile>
      <anchor>ea65e30d6bc85cb236b4b66904536e1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>subscribe</name>
      <anchorfile>structcarmen__param__t.html</anchorfile>
      <anchor>7a5e836866be629c033159b855c4f2bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>carmen_param_type_t</type>
      <name>type</name>
      <anchorfile>structcarmen__param__t.html</anchorfile>
      <anchor>cd179e7e236b1485583d85420c66329a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>void *</type>
      <name>user_variable</name>
      <anchorfile>structcarmen__param__t.html</anchorfile>
      <anchor>0c851fc14cb5e9454ab7f84b42f9bcd2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>variable</name>
      <anchorfile>structcarmen__param__t.html</anchorfile>
      <anchor>e251f7b78de44481dde5640c81268440</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Circuit</name>
    <filename>classCircuit.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>initCircuit</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>7dc3e20cb3709fb2712759bfc50c8acf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>robotState</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>250854aff90477205cc17206ab9c3652</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Circuit</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>83ab4b49c69ef7f09bddf8e14a19ded7</anchor>
      <arglist>(InterfaceAvr *i, QMutex *m)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>97d003f5dc42059649b895d405110b7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Circuit</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>1134e16f26a878377f4d502bff111b9e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>firstInitDone</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>5d4420c5bec13161937cc52b62908e6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterfaceAvr *</type>
      <name>interface1</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>a97c4209ada586fa3c19281d32b503f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QMutex *</type>
      <name>mutex</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>3dba8e1656340fc0789d37491cfadfba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotIsOn</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>8b7692fb69b51bffbc72e6c5485f3ae4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>INIT</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>1dc27f8c56cc1ff3fd27684216276bfd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>INITANSWER</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>a9055d4cb42ce59acbaed0900bb8bea3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const bool</type>
      <name>OFF</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>c3c47abe00823c056651a49bbd728691</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const bool</type>
      <name>ON</name>
      <anchorfile>classCircuit.html</anchorfile>
      <anchor>1e9f513b37c42e97d6855165af8a3687</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Gui</name>
    <filename>classGui.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>appendLog</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>3a7ba597de98c24f2d3fef95a87ba6d0</anchor>
      <arglist>(QString text, bool CR=true, bool sayIt=false)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>appendNetworkLog</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>2fa0fff1cae68e8e85c74f439e4f4cb7</anchor>
      <arglist>(QString text, bool CR=true, bool sayIt=false)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disableCamera</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>514a620397a3eedf2f9789eda780c4ba</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>disableFaceDetection</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>879b018073c869d6c66e93a0f97186c0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>laserSplash</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d97b27756611d445d617944d830f4429</anchor>
      <arglist>(bool status, short int laserScanner)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>refreshLaserViewFront</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9660e3b78a58fe239f81ca9a34753bf8</anchor>
      <arglist>(float *laserScannerValues, int *laserScannerFlags)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>refreshLaserViewRear</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>fb229f99efac2e5a33733f3d29db5c3f</anchor>
      <arglist>(float *laserScannerValues, int *laserScannerFlags)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setCamImage</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>17a330f17a38776f7b68d23b96e82bfe</anchor>
      <arglist>(IplImage *frame)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setPlotData1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>b2d8888747107818ca18ebab14bbdbc6</anchor>
      <arglist>(double *xval, double *yval, int size)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setPlotData2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5b31d6ac9b1483de19a22254c71be591</anchor>
      <arglist>(double *xval, double *yval, int size)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setPlotData3</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d72f8feedaec0c685ce1df6c439b31f6</anchor>
      <arglist>(double *xval, double *yval, int size)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setPlotData4</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>938a76716d27c179b272e9b012380cb0</anchor>
      <arglist>(double *xval, double *yval, int size)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setRobotControls</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>16020df7f6cc720a8c36561d83c5d1ec</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setRobotPosition</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>fae9bee62ab4dc720aaf2f53369331df</anchor>
      <arglist>(QGraphicsSceneMouseEvent *mouseEvent)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showFaceTrackData</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>bff407d1d1b0fafe6f6847da5ce2bc6f</anchor>
      <arglist>(int faces, int faceX, int faceY, int faceRadius, int lastFaceX, int lastFaceY)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showFaceTrackDirection</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d22b9f6fe9aacdb789fc81d06689bc63</anchor>
      <arglist>(QString direction)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showJoystickAxes</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c79bc0dfaafc5d799b31e5593f8998e0</anchor>
      <arglist>(int axisNumber, int axisValue)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showJoystickButtons</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>cca2a0b5bb22c57c1f6612391b0853d3</anchor>
      <arglist>(int buttonNumber, bool buttonState)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showLaserFrontAngles</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>e9fd3f772d839eea2f5c490828c5c311</anchor>
      <arglist>(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showPreferredDirection</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c222754552bafb16f80e86df5b7770a0</anchor>
      <arglist>(QString direction)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>zoomLaserView</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>94685c726c39963a7f769545cf831ac2</anchor>
      <arglist>(QGraphicsSceneWheelEvent *wheelEvent)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>drive</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>deadc75e73e0e0f3ebaf44217bab9c45</anchor>
      <arglist>(const unsigned char command)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>enableFaceDetection</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>b984ac49b8a6b02991c516e10575441e</anchor>
      <arglist>(int state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>enableFaceTracking</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>acbcb6ee104b3c9654b7977e5db0c3c1</anchor>
      <arglist>(int state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>enableRemoteControlListening</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>1e9ad885280690a7565e85d35c183c6b</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>initCircuit</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>ca40ebafd97214b72c5bbf26bf28355d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>initServos</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>953ae5151f6154952f7c9b557f14373f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>resetDrivenDistance</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>03f3f8c2e27605b6e96f2c8ec9dabde9</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>shutdown</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>da58feae70f01eb9cacc012d4b2f7d56</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>simulate</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>dc572fe913d4aafb92a57dcd4ee14f32</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>speak</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>e9ee6d7ba99be194a580bd734b3fcc26</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>test</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>588226e185e5a2b60be455301ee6c63d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Gui</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>11a970f8f89ea513c2090696344606fe</anchor>
      <arglist>(SettingsDialog *s, JoystickDialog *j, QMainWindow *parent=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>initLaserView</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>02bbcdef63bb227bfbde2f01b54b79bd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCamImageData</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>e72a1ab8522c3deb3b60f3d54aff5c0c</anchor>
      <arglist>(int width, int height, int pixeldepth)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showAlarm</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>3e39b4a62def8238cb3f7c2eb10f481e</anchor>
      <arglist>(short int sensor, bool state)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showDistanceGraphical</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>86d1bddf051989ebf381ff01cbd4674b</anchor>
      <arglist>(int sensor, int distance)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showDrivenDistance</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>fc544b6af6462545c60f3faabce4f86b</anchor>
      <arglist>(int sensor, int distance)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>showMotorStatus</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>f89c21d71ce6cf0b45941099759f06e7</anchor>
      <arglist>(unsigned char motor, bool power, unsigned char direction)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Gui</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>4fd8485d226f9b8a2ac2d81d7f0f3598</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionAbout_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>af464406c3a796127b651de8af60ef2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionAll_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>8179b0a0f2f7f9c3a22b10d880e2f886</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionCamera_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>e03e9b482981f706a0a248ccf7b4eaf1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionDrive_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>016b879a781b48e9d627540d7c3ede06</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionExit_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>74da420f5b0dc77f4d0b8b543a90da5c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionJoystick_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5a58209a3f07e83ff1deff8cfea37e98</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionLog_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9bd98bfbc1e68d40a4672ac9170bf424</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionOdometrie_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>48e4bbff7c1606e1da05ea1c31b27959</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionPower_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c78a0ccaca16c8299412450919f630ca</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionRemote_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>75e17fa14484f11d5b91111fb95c1f7e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionReset_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>ac4ab0f5a50fa56afe9cf0dfc05b4e2b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionSettings_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>0b1a60d94955c0827190bf5cc3f9a65f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionSimulate_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>30d134efd1a0f20fbb972bc027dcd49c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="protected">
      <type>void</type>
      <name>on_actionTest_activated</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d390cd3fd32ceca263574e47c7016285</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>closeEvent</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d977f0b2050bdfd4e8181b8772482df9</anchor>
      <arglist>(QCloseEvent *event)</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_btnResetMovement1_clicked</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>629ede4418dc64661c4e4d8373f63eae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_btnResetMovement2_clicked</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>38f6394291247c15fdf3ebc7cf8e6205</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_btnSavePicture_clicked</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>0f028ee5ebc9826fa0003c10ccd7328d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_checkBoxFaceDetection_stateChanged</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9e2493d2341c545c02218e8950777975</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_checkBoxFaceTracking_stateChanged</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c8b8ccd8bfb3f1e66f9cc296248fe1a3</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_checkBoxMirror_stateChanged</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>49c01cf888eb42806b6eef3fb1d5de79</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_sliderZoom_valueChanged</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c3d8d8f0370969d35bc2181ad0f5175c</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>saveCamImage</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>1f9211fbbe2e4d42c728813b10d304b8</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>createLaserDistanceObjects</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>499b536c2938b98965bbbf681db16736</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>createLaserScannerObjects</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>8e6dd3b29999de41a196aa773ff34053</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>initializePlots</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>aedb600ea0daff1abf0e0af735fc41fc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QPixmap</type>
      <name>cameraPicToSave</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>b1e07db13da264f9fe10eb83f1375fcf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorGraphicsSceneBackground</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>11959f20f6bc526db702daafeb4c6d17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorHelpLine</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>aaae8be4efac0c840456db49122b29bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorHelpLineText</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>f0f0062196ae8c6df85df828aeeba0c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorLaserCenterDrivingDirection</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>148c15fcfd0dd7083f4e1d39b94f1b50</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorLaserFreeWay</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>cbe3d40fc19f63b7a7a71c7930e638fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorLaserObstacle</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5010766c685d3afdb676769e8a45d039</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>colorLaserPreferredDrivingDirection</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>f2fdc3742f6ca52dfb0e28f314394c64</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QwtPlotCurve</type>
      <name>curve1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d1e2b0384a23e4ea49a08440e3c5de78</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QwtPlotCurve</type>
      <name>curve2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>7fe0baa0365ddac27d5d42a9d126a252</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QwtPlotCurve</type>
      <name>curve3</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d920e1932d5a8c3ef1c92dcb78fdd32c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QwtPlotCurve</type>
      <name>curve4</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>4f1fa5ac2cf8d9dcf91b3b4cb35c06b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>JoystickDialog *</type>
      <name>joystickDialog</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>4afe48427f3e827ac2ea4db81a67f9a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>labelFillColorBlue</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>474dcf4ce11e5a6857470b1ac2382477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>labelFillColorGreen</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>7e5366c3c014071e1ca82093f5801da6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QColor</type>
      <name>labelFillColorRed</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>8573e9441716a227e307a82a333660a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; QGraphicsEllipseItem * &gt; *</type>
      <name>laserDistanceLineListFront</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5c0d87e6536d27fdda607c5e678dda4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; QGraphicsEllipseItem * &gt; *</type>
      <name>laserDistanceLineListRear</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>a8f99a855df96c248cfc01e9598f28e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; QGraphicsSimpleTextItem * &gt; *</type>
      <name>laserDistanceTextFront</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>aa6b2972164cedcb7edab37d8afca0b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; QGraphicsSimpleTextItem * &gt; *</type>
      <name>laserDistanceTextRear</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>03d86e41e1cf751fa19caed49be582e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; QGraphicsLineItem * &gt; *</type>
      <name>laserLineListFront</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9e678bd77cc7d8339cf73900bfed8ba1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; QGraphicsLineItem * &gt; *</type>
      <name>laserLineListRear</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>4715932eb743c69c3aac665c1880ce37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QPen</type>
      <name>laserLinePen</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9ab94baa2db6a5bee1be3e1e4fd7e22a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>qreal</type>
      <name>laserXPos</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>0ad880e776c8f1c62b0cbdef5904e776</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>qreal</type>
      <name>laserYPos</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>f35f116e8b82ff045b6d2eb2f4d10eae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>lastZoom</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>1f2a9619757ed317e0ccfe859bbce2ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QPixmap</type>
      <name>pixmap</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>72c1a93cc03be1f37e9d48c20100e068</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QGraphicsPixmapItem *</type>
      <name>pixmapBot1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>2944f3693168e40e9f768fcc76d797d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QGraphicsPixmapItem *</type>
      <name>pixmapBot2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9b3cf973260035ae06b9108d39436cf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotIsOn</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>512ba9345ec438a124970cb00da32672</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QGraphicsPixmapItem *</type>
      <name>scannerFrontSplash</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>53b75eb64e8432bc2f94059808224dda</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QGraphicsPixmapItem *</type>
      <name>scannerRearSplash</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c98c0ae9835b37565d78073702480efe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QGraphicsScene *</type>
      <name>scene</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d3202ef7852a1ee7f6d20a71e1feb0d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SettingsDialog *</type>
      <name>settingsDialog</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>f658f642e79fea947cb6abdde144187f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>qreal</type>
      <name>startScale</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>0dee3114c4e79685d6518b805669bd3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Ui::mainWindow</type>
      <name>ui</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>8fcadbbe5e95e759b8e5ebae7e226b6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>CENTEROFLARGESTFREEWAY</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>269da7ef387dc4b5a39b4ddfbeea8681</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CLOCKWISE</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>7cc3b48653b6d087bc662620583a585a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>COUNTERCLOCKWISE</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>39a25f6920692a94918b0e1be3644228</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>FITTOFRAMEFACTOR</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>18233f32cdf9e41d5ae86d931f7e93b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>FREEWAY</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>0641c07e2d89365aa100c89f05a351e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>INITIALLASERYPOSFRONT</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5d0fe5de9cfb6989ae73b7ad8f49fb38</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>INITIALLASERYPOSREAR</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>8b5e25ecc5403738abe5de76e182116a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXIS2X</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>67dd9a59ceca7cf55204b38ab1bb93eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXIS2Y</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>dbc0969c418d4e705f3cd409f6713a62</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXISX</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>83a49e88080927652360b251de16e74a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXISY</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>f208357209de9efb9736a2fa73ed292d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>LARGESTFREEWAY</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>df37d30cc0112ffa5f7ddd5799a7e5de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>dbd64b30238070b5b43880ac14933a98</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>36040cd4463aa6766964218b049c7d55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER3</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c1631a66ec7aaf3e0d29111793116da7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER4</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>efa9d5338fc6854c416eadefb8f2438b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER5</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>4bfc1a2ce55cbd8a951e59163e7dd000</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>LASERDISTANCECIRCLES</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>46f67500194869c56681940341021892</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>LASERDISTANCEDISTANCE</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5d4ddcc963c7b3b427685b8bb55817d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>LASERDISTANCEFIRSTCIRCLE</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>051bdbb688c63637b0a87746a1ee6fff</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>3ba1f9d883d744c1582274d47772cafc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>c7391aacc5fbd55081635213d69ed477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>d0a83bdafa7172192be12b3d28fd4301</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>172b724173e48fbd4f7135a37b40a758</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>NONE</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>65e5038a9b3c58a2a2037e9bd18f5117</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>OBSTACLE</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>9d8cfe47d5b063bdd788279a284944a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const bool</type>
      <name>OFF</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>878064227e2aa78b088b9a6d91b9f364</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const bool</type>
      <name>ON</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>bd4157f83efe9a2a7fa730a2af05ac91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SAME</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>de3aaedc2e411b560f673c0e7ee37d07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR1</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>588fe1888c8bb29325c540305310f290</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR16</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>50da7c925a3bb850b9d491f2ede7a259</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR2</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5bab9c58a92db4da262e2d68a96c0e6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR3</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>66d1ecf174397fd22b52d3b17b09f794</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR4</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>e35437929bbd8eedfa3bf72c77995bc9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR5</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>b61609a8795105f6c28dad8aeb03b56a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR6</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>00735ca46d4c26b13b70b78cfa8f7b4c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR7</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>842b0a3ee4221758555e3ddfe5c828a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR8</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>5405519fb7f34285a34be34ff0202853</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>SENSORPROGRESSBARMAXIR</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>1fe15d73a693819421b15497ee69665f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>SENSORPROGRESSBARMAXUS</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>b58d11ad6d281fba737c3f7cc890662d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>START</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>21abca2f35b19b268ad46dccb4e8e608</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>STARTZOOMLEVEL</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>e3674ede477f06b819d5bcddc1d43f3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>STOP</name>
      <anchorfile>classGui.html</anchorfile>
      <anchor>bf87fb7ce331c464db716e6920c2d028</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Head</name>
    <filename>classHead.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>look</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>58d87bcb5434b3205210082431187177</anchor>
      <arglist>(QString direction)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Head</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>f8e009f1af8adc66f6656bee8f27fcce</anchor>
      <arglist>(Servo *s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Head</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>bf7f71ba29dd0f6e1de17e3329a3c745</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>eye</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>a5cc7e1578a6eb64429ba371c8033d64</anchor>
      <arglist>(unsigned char whichEye, QString direction)</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>eyebrow</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>845adeb986105edb98286bb8149c0b8e</anchor>
      <arglist>(unsigned char whichBrow, QString direction)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Servo *</type>
      <name>servos</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>1984f612c158b3d59e55f19cf1e0e88f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>LEFTBROW</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>6d8c0f8f6174ef10c4152e02fcafbd0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>LEFTEYE</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>b7885d2957017f85b2588d7652c3f56b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>RIGHTBROW</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>f35d2c934c09e46433d9cce8b8501c25</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>RIGHTEYE</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>4c97cd1891ab106faa6e44ba241b9373</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO1</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>e93720344970a161faee107915892ba5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO2</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>ac6f0cdd50e402bb71ff0627e9074111</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO3</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>5bcdcf2b59abe5b5b69f1d72120a071d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO4</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>b7032aee0534c829fadd3ddafc4ed428</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO5</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>bd5bd0001ca79f6f82a87fb7889080f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO6</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>cd77f76e0344111774298bd4b67e8a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVCURRENT</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>6a8bc7847016b816e9be4f8eb78807aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVDEFAULT</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>524cea6131d3b9733c87fa9c749a81ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVEND</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>11c4597e075b6c4ff0422fd9db0c86bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVMAX</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>1fbac5a0c04a2ab5adb11c2c9f1180ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVMIN</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>c3f94ae06e240cea8dc8d052c8548417</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVSTART</name>
      <anchorfile>classHead.html</anchorfile>
      <anchor>024af4c7cc0d0b122d39cfd088f58124</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Heartbeat</name>
    <filename>classHeartbeat.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setRobotState</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>2b99d807c51f8dbba311fd4ed9f1535c</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>alarm</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>97c66027e11fa161be990254a4c104b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Heartbeat</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>e3ef1553a75c78027d4a8527e3545a1c</anchor>
      <arglist>(InterfaceAvr *i, QMutex *m)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>init</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>523ddc266779e15d2e93be98791e8065</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>25fb1827e12a071feff19185867d0896</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>a6561134284777b6e170db4eeac6711c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>a348a8b9551a256ee00eaf4700cc157a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Heartbeat</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>3197be04a732dc01af98a15bd2df9a92</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>initDone</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>18770462c885665df2e76f2a4e96c601</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterfaceAvr *</type>
      <name>interface1</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>68c6bbc033a8e20b69478b5b80f1f631</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QMutex *</type>
      <name>mutex</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>682995d36fa78f66d481cce5ee5d2d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotIsOn</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>7b43bb30e40e17c1785ebdd2f44def5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>f028fe201c78006058eb35392881e6f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classHeartbeat.html</anchorfile>
      <anchor>a9d7233ad4ad87c03355e838f9b4feb5</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Inifile</name>
    <filename>classInifile.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>checkFiles</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>6b9d3e84321c07e6bdaf688dbc856135</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>checkPath</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>a4a67d81413ffb8528328e4bba14c695</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>getInifileName</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>de9156a703cad4909ee606f184fd92eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Inifile</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>dd0eef63fa7a622050e8e854c9441e12</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>readSetting</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>51b660bf0b6602253a6ad5fe7edafd27</anchor>
      <arglist>(QString group, QString name)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>readString</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>0665d31dd08c0cde50b733faeceaf526</anchor>
      <arglist>(QString group, QString name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sync</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>83aecfea8b631b72171d7c16bfd61a63</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>writeSetting</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>bd4f9da76277e7b3c2ce1472058a1b18</anchor>
      <arglist>(QString group, QString name, int value)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Inifile</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>5b7f8d5c72e6b7c61773e00252ee347d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>mainIniFilename</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>07f63f27490b670158575b84bc067e6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>programPath</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>61ab2948317f793fb46dca3e516096c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QSettings *</type>
      <name>settings</name>
      <anchorfile>classInifile.html</anchorfile>
      <anchor>234a5175278063bd5918c9dba13c16e1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>InterfaceAvr</name>
    <filename>classInterfaceAvr.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>tooMuchErrors</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>034c0c0154327f9fcc041b848de9417b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>closeComPort</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>f00e7032bbb719e58833cf234b6d9650</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>InterfaceAvr</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>30e01ca6d42f540dd8b68d4003567c17</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>openComPort</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>0ebd694eac29eadadb0db268eb19aef0</anchor>
      <arglist>(QString comPort)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>receiveChar</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>2e5ce2e0ac51ee52c46aef54215710fb</anchor>
      <arglist>(char *character)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>receiveInt</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>a283340c1a4421b8f18fd5437100403d</anchor>
      <arglist>(int *value)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>sendChar</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>239d759eb6f6e89c1f1ec02d3563ffbb</anchor>
      <arglist>(char character)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~InterfaceAvr</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>1aa07ee29d21890542ae5a87e281e76b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QextSerialPort *</type>
      <name>serialPort</name>
      <anchorfile>classInterfaceAvr.html</anchorfile>
      <anchor>6e35d7fd5f68ff89566c293991d1e92b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Joystick</name>
    <filename>classJoystick.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>emitMessage</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>d7891ce6e3ffbd95443dd74c7b8c893d</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>joystickButtonPressed</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>9150a264d554bcf337598a0d95ed52ae</anchor>
      <arglist>(int axisNumber, bool buttonState)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>joystickMoved</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>a13c6fe31d0236e22779b62b1fcbcd2b</anchor>
      <arglist>(int axisNumber, int axisValue)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>308093303466c7dfb65adee01ef8c6f5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Joystick</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>158b1f77b78717efbf1b8fac43b1fcef</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>7a943201c7358d731d536c972d3380dd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPort</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>a48604f47378b41c842e3eae91c8c83a</anchor>
      <arglist>(QString port)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>663bef4817dc643d2e54fe5083ae6177</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Joystick</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>23429c0470e1a32b8de61e1ad7c251c1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned char</type>
      <name>axes</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>b4b4ec8236fcc9b075e76d2a9c163a36</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int *</type>
      <name>axis</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>10f47afd7488a4d631540b515353a402</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>short int</type>
      <name>axisButtonNumber</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>96435b02639a93f9b484763cb4edd73b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>short int</type>
      <name>axisButtonValue</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>4f8646a8a102bcf027005551ccb25073</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint8_t</type>
      <name>axmap</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>acee9cb7b749ad840019b168588880fd</anchor>
      <arglist>[ABS_MAX+1]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>uint16_t</type>
      <name>btnmap</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>801c405af54672f56d703fe5cc3de3bf</anchor>
      <arglist>[KEY_MAX-BTN_MISC+1]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char *</type>
      <name>button</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>29fd7a97a2b219df7dac44f7aa6bbcc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned char</type>
      <name>buttons</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>b4e31435e98041da2e2414fe41953abe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>fd</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>e068cd4085254c19631c716606fa040e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>i</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>84a6b7eee22801184a6a217f09c76292</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>joystickPort</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>c1ecd2b4d0ddf4b4bf6af62f4d6efab5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>struct js_event</type>
      <name>js</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>1e909c15809a86efece99e110fff7325</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>name</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>9d247a27f05e2adf8cfdff267b26e98a</anchor>
      <arglist>[NAME_LENGTH]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>d269fa8f1641ef86dc5c55dd9ad49a0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>version</name>
      <anchorfile>classJoystick.html</anchorfile>
      <anchor>8b648918424dd7045ae8c8af95c51166</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>JoystickDialog</name>
    <filename>classJoystickDialog.html</filename>
    <member kind="function">
      <type></type>
      <name>JoystickDialog</name>
      <anchorfile>classJoystickDialog.html</anchorfile>
      <anchor>ec96ae0d4ab987378ced47ea79f18a2e</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Ui::joystickDialog</type>
      <name>ui</name>
      <anchorfile>classJoystickDialog.html</anchorfile>
      <anchor>26db690906eac7062d92daab37667809</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>KOORD_T</name>
    <filename>structKOORD__T.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>radius</name>
      <anchorfile>structKOORD__T.html</anchorfile>
      <anchor>2450a48668c90526088c2913a7b6ffde</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>rank</name>
      <anchorfile>structKOORD__T.html</anchorfile>
      <anchor>d7ebc66bb528d0771ae2e84851584bbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>x</name>
      <anchorfile>structKOORD__T.html</anchorfile>
      <anchor>78818678937072457c7a8ba449e63d66</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>y</name>
      <anchorfile>structKOORD__T.html</anchorfile>
      <anchor>11fc65f11b8996127c9c53291f8b7a61</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>laser_device_t</name>
    <filename>structlaser__device__t.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>baudrate</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>fba2431db21ba44bdb5964fba7cabaf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>databits</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>a88d29f069583137f407d19c8318a5c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>fd</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>eae5070926bf7142121fa1b962854860</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>hwf</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>42182adfa96b1d0702ecdf00aae52b86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>laser_num</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>138a0562a8a539497c4925a68a67330e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>parity_t</type>
      <name>parity</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>a2bd193424dd3445ee503a9f611e0575</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>passwd</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>ba0173be4d8de132d4a5f584ccf301c8</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>stopbits</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>070df0047628093971a84a143c0e8385</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>swf</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>7d435351141e97090f048ba534de2bdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>ttyport</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>52be36c4d75973dc7db9c7afefaf1bc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>laser_model_t</type>
      <name>type</name>
      <anchorfile>structlaser__device__t.html</anchorfile>
      <anchor>fe5008b0c15e85f227a93423c1f5fd74</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>laser_settings_t</name>
    <filename>structlaser__settings__t.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>angle_range</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>eb987d9eef68be9368cafb9b8a260f14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>angle_resolution</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>7e81f3a1c18af109752689d7ef0ec24f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>databits</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>caf01d407a2dc24d56999d7fc3e81060</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>detect_baudrate</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>5fe66662562606d2c52796c67641db5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>device_name</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>ed20f4ef56c89968de1e11b106167732</anchor>
      <arglist>[MAX_NAME_LENGTH]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>hwf</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>59d4bc1c2999e0bd650c98d3f50e8b80</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>laser_flipped</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>84fb9bfc364541de6c9f4648b9ad656b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>laser_num</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>65ec312ba97a3350c60b5f67f43c25e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>num_values</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>b90d23be8894674ae7c8c0dfada3fbfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>parity_t</type>
      <name>parity</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>adccc43c208cd65b73d306abfe7b6860</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>unsigned char</type>
      <name>password</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>44549f1469de01db8655ea975dabffca</anchor>
      <arglist>[8]</arglist>
    </member>
    <member kind="variable">
      <type>range_dist_t</type>
      <name>range_dist</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>46ceec5a40ba34d540ec6d753f5d9ef1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>range_res_t</type>
      <name>range_res</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>10978b9e3788dbf42b301e84eb4d83e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>rem_values</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>dac701b1f1e4235afc7eb68dd3e8e440</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>set_baudrate</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>09f6455d5b181cb2bf0a112776d9bb49</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>start_baudrate</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>3f8280cfe69a7207f898b24c5525b21d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>stopbits</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>dbf5b8edebcf8771d6a5d0fec1fb0352</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>swf</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>48b353463af703b92a901911f832b9be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>laser_model_t</type>
      <name>type</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>6ed0a092f7656dfddd28c8357a2763bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_highspeed</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>fa23d653b9e239e18c5942c98adb36d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>use_remission</name>
      <anchorfile>structlaser__settings__t.html</anchorfile>
      <anchor>7f38685432ff954ca429825cd96d9005</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LaserScene</name>
    <filename>classLaserScene.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>robotPositionChanged</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>d788049b47711cdd71cc35b6fd9b6219</anchor>
      <arglist>(QGraphicsSceneMouseEvent *mouseEvent)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>wheelZoom</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>c99d3a420bf65c280b40e52c9740a29f</anchor>
      <arglist>(QGraphicsSceneWheelEvent *wheelEvent)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LaserScene</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>9cc1cebae5091bd607b8599ba2198248</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~LaserScene</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>a8d04c3143429d622db62117a04f4200</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>contextMenuEvent</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>0981c6713eaac7b9c756bdcdb4f3fda2</anchor>
      <arglist>(QGraphicsSceneContextMenuEvent *event)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>keyPressEvent</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>c4141a0153042876815f4fba37832718</anchor>
      <arglist>(QKeyEvent *keyEvent)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>mouseMoveEvent</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>2632a1968a152add5993b73ef33edab5</anchor>
      <arglist>(QGraphicsSceneMouseEvent *mouseEvent)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>mousePressEvent</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>4188de0deb193179ef2d1f99d309afdf</anchor>
      <arglist>(QGraphicsSceneMouseEvent *mouseEvent)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>mouseReleaseEvent</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>10607bc5fe42f379355fb343a169239d</anchor>
      <arglist>(QGraphicsSceneMouseEvent *mouseEvent)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>wheelEvent</name>
      <anchorfile>classLaserScene.html</anchorfile>
      <anchor>7403a81cb4feccaec50acb7d98917454</anchor>
      <arglist>(QGraphicsSceneWheelEvent *wheelEvent)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>LaserThread</name>
    <filename>classLaserThread.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setSimulationMode</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>d8108c3193f742b2830022b11a96e653</anchor>
      <arglist>(bool status)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>laserDataCompleteFront</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>5ae0a39cb9d3d6e9eb2dd2adf474fcc0</anchor>
      <arglist>(float *laserScannerValuesFront, int *laserScannerFlagsFront)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>laserDataCompleteRear</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>618e1e825d8aab62f86c62db8bff2554</anchor>
      <arglist>(float *laserScannerValuesRear, int *laserScannerFlagsRear)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getLaserScannerFlag</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>a70b6bacb9b95191f235108d23ca370b</anchor>
      <arglist>(short int laserScanner, int angle)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getLaserScannerValue</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>6b69ba278e031b9d578da0f3deabdae3</anchor>
      <arglist>(short int laserScanner, int angle)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getNumReadings</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>a408c135ef7e6384c0d5ee2169090c40</anchor>
      <arglist>(short int laserScanner)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>700c104a2f0f4c90dba912010a49d0c0</anchor>
      <arglist>(short int laserScanner)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LaserThread</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>ccdc82334c67b22a543d19df40f09517</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>d0faa43145bb8b1b67dbc4e85e6b3401</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLaserScannerFlag</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>7d45044151382b649215e3448c992abe</anchor>
      <arglist>(short int laserScanner, int angle, int flag)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSerialPort</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>f5a93d16f3e2167689734b24f2fea893</anchor>
      <arglist>(short int laserScanner, QString serialPort)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>862d0a7ef298eb943514f95d43028d14</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~LaserThread</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>6c507236ea8bf9633bc60681a235aae9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>getAndStoreLaserValuesFront</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>5c6d0545b7991163026b96ded3ba5107</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>getAndStoreLaserValuesRear</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>363315362a9ec36b299db17020d3264e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; int &gt;</type>
      <name>laserScannerFlagsFront</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>9c570547b1e01d35a9958721df4e28c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; int &gt;</type>
      <name>laserScannerFlagsRear</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>5ace8f63b6716bc90cdfe5b233803b70</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>laserScannerFrontIsConnected</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>c531d8bb00997ad5440fa7002182c580</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>laserScannerRearIsConnected</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>8628ca87816183ff5cd23a24f48e616f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; float &gt;</type>
      <name>laserScannerValuesFront</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>b298e805687be36b2f2211e197c9205b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; float &gt;</type>
      <name>laserScannerValuesRear</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>46146ce4efee22487d294c12094dfeb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>numReadingsFront</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>6d345ce5e7bb494c9e85611bf7844c4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>numReadingsRear</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>4e114eda5545b50cf1921623e815372e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>simulationMode</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>42cb840c5ba3e3d0a40ac403e1443b8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>ce014c606e6ba9e30f55afd59182aaf2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>DISTANCE</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>87ee6318504163f01bd0b9a73d37c150</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>FLAG</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>b4245dddd23314f3f0b2b389ad498adb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER1</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>7fd49ba6f918071389b29eae2e97b6ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER2</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>dd9f72ab94dfbc4ba8b517b5ef39be13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER3</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>c277137fd77f807d3afc849c391f1140</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER4</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>2dded8c50a554253b246e591b43f2986</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER5</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>acbf45052761c5f604d4b47c8f003ed5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>LASERSCANNERARRAYSIZE</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>e0730fee0b5703fcc380ca53933bd025</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classLaserThread.html</anchorfile>
      <anchor>e61092068af9a842180e46b446d7a1bf</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Motor</name>
    <filename>classMotor.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setMotorSpeed</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>ba184ab7772ff11d8ceff18299bb4343</anchor>
      <arglist>(int motor, int speed)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flashlight</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>02ba87f4d1c5c5db37188e109f1ddea5</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>getDrivenDistance</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>ec6e772898860691147921ff600a9100</anchor>
      <arglist>(unsigned char motor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getMotorSpeed</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>b71b3cd0a4b93cab436d7c606ab9de44</anchor>
      <arglist>(int motor)</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>getRevolutions</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>361a5ddf97b2c00b4597b6a78861b108</anchor>
      <arglist>(unsigned char motor)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Motor</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>d72b6c55375ac77d5c7bf64d863f8fe0</anchor>
      <arglist>(InterfaceAvr *i, QMutex *m)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>motorControl</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>06314a242203b58ea5bf449a4ac63831</anchor>
      <arglist>(unsigned char motor, bool power, unsigned char direction)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>resetMovementCounter</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>4c69cc80f65085b2a78b066175839a86</anchor>
      <arglist>(short int motor)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Motor</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>2e57c7b2681efea1d3b7f253ee88ecd4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>calculateMovement</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>06d643e086f7af4c94cfa25420e915e9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>drivenDistance1</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>b5e1caf16469b41c3c171672624b2ce2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>drivenDistance2</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>e28140df4768d074ee9c01aa6ad1e7eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterfaceAvr *</type>
      <name>interface1</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>63ee1ae17789facd23940829c599e4ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>motor1Speed</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>5cb5583c5e05cca7606ab8ac7f8ae21f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>motor2Speed</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>8a8d371f08d42cc40180088704e3e5e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>motor3Speed</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>3cacac3e10c9eccc7335948ee229b0ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>motor4Speed</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>35bce5a0c7ffaf34f496e62c233f4c4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QMutex *</type>
      <name>mutex</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>dcdbdb28262fd871fb9705b1b1884af6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>revolutions1</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>cde9a30df0e7d26d6c2caf4fc7de6c83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>revolutions2</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>ae03f71bd88ef85b3eef572f3fb6d997</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT0</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>fba2c9bb937514a7101f42245724bce1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT1</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>767e80653ea3cf894145e5ed51fdbcf7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT2</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>07cae1009d35405bcf7661e649b3eb16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT3</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>21fc09f6f78d3cb8ed53932e4ebf2bc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT4</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>e8c895bf00484f0643116afd8572210c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT5</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>9019e669ec7a9f26cdbd00071d470585</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT6</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>f356cee861ef48c4d05ea23c75193211</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BIT7</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>57eb4cd2b90c94551ead705d63274fec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>CLOCK</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>58171270b182425da37e4f4a3f5c7426</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>c1ab49b884905d33777b222d7e880cb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>COUNTERCLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>0616ee2b7b91d2827e79e60de1bebd9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>FLASHLIGHT_OFF</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>f9467edad6f2586f6a25e246c98ce9dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>FLASHLIGHT_ON</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>81e8b285f5f39cdcefbd220d063f71b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>HIGH</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>52158f2b726acb0a2bc122f56b7c4a67</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>LOW</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>ad1bc1274748d7849984e6ab31d1915b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>aaf78f2f87f1b6f2287dd3690e71f4b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1_CLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>d7959c33690585ccf9e2f88cb4585ff1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1_COUNTERCLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>2968d78792ff9a6b4c06520a845bf115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1_OFF</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>815929711bbeb40d769a4cf9d485180f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1_SPEED_SET</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>3b85463a6a76b9238449598b559109ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1A</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>80fe384d3c6ed405be4b4eccf31ff0ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1B</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>2372bd13a43ea7a9c88f3d63ee0ece2a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>bf54e569fdcd415ab3b1bec3f19fca07</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2_CLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>b82d9da67a34e91c1ae601bd9de27593</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2_COUNTERCLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>d433bcd4f29e5144e7208815fbaab9f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2_OFF</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>aca89e0c3d9e1fdd3eae1adb07f757cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2_SPEED_SET</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>5b3decb109900b262a2e1a849845e153</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2A</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>b8b400b2001dd823f9df9eacd93b5294</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2B</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>9118affdb1eb4cf53620874acbe8935b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR3</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>9671e47d5ebd6b867a332ff9da9c3963</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR3_CLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>5b2fdbd19fe2cd38c53959b5351e6052</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR3_COUNTERCLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>48bdd3509c8ac472fc83889e9d7c4a94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR3_OFF</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>f02d1cecf9ef29703b1862c094398f86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR3_SPEED_SET</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>2ba126b10937b0116b88ed3f14809016</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR4</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>9291b449831c2cb3c41852d369c2e2a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR4_CLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>7d5fabac55e1ae59fcab89a8f3f8d328</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR4_COUNTERCLOCKWISE</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>1d19be0b39d3caf23511659acf91e249</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR4_OFF</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>8367ff8235e8c8a1a4851adc8335105e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR4_SPEED_SET</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>3c98346e45d0e667290329eb6f5aa64a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTORCLOCK</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>8cb3c6c9c96f10ae9c35d1f0a45f1ff2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>OFF</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>e1d147747a5f5bc40a546dd64633d7c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>ON</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>3f57a36b927a0c95b1cf103d7220c644</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SAME</name>
      <anchorfile>classMotor.html</anchorfile>
      <anchor>0908be292908516f3333ac6ab3cacc34</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Mrs</name>
    <filename>classMrs.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>drive</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>92a211fb225b61bf58e2cb9b832844b3</anchor>
      <arglist>(const unsigned char command)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>enableFaceTracking</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>497a634a07c385d3e3e88fb34b6c2f40</anchor>
      <arglist>(int state)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>enableRemoteControlListening</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>eb75572475a0e46165d104c25bfc08a0</anchor>
      <arglist>(bool status)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>executeJoystickCommand</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>8e4476b8247351d8779c98a4ab450e03</anchor>
      <arglist>(int buttonNumber, bool buttonState)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>executeJoystickCommand</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fdb037993bf6f17f7dea9a458a1d1bb5</anchor>
      <arglist>(int axisNumber, int axisValue)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>executeRemoteCommand</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>26e07d110b5fadd912ccc2e65cf182ec</anchor>
      <arglist>(QString command)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>faceTracking</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>54000877fe426da78a98744427633f87</anchor>
      <arglist>(int faces, int faceX, int faceY, int faceRadius)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>finishSplash</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>902576bc27b86eead1eff4f706e7dade</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>logicalUnit</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>98a62c6d33e28df4d517b77cb7567c27</anchor>
      <arglist>(int sensorAlarm, QDateTime timestamp)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setRobotState</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>58f786734bd22b2505636fefb5e08aae</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setSimulationMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>11eec8254e49dc7d33fa4f0cd232308a</anchor>
      <arglist>(bool status)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showExitDialog</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>93447869b7a26386d152c20e35671892</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>showSensorData</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>7d2497810c8292cb4a276716570bab53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>shutdown</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>7da5a966bf956e7f042f9d48fb1be833</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>speak</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>aed2d46f15387f16f1f458f9a8d38093</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>test</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fec132e05a4623abdd9ec9c22eb2cba6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>initCircuit</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>a6d2b5d94a93968b37bc193c6d34a7b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>look</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>df259f7091a4b51359c7962af4b4cd5c</anchor>
      <arglist>(QString direction)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>showFaceTrackDirection</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4fcc5018e5168b5ef982caf963d6299b</anchor>
      <arglist>(QString direction)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>showPreferredDirection</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fc2a933be46cd50cdac500f1f4f565ed</anchor>
      <arglist>(QString direction)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>init</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>82b5a5d392f700b8a207614b4f4a085a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Mrs</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>63514b780b8473060e86870daa18b4c1</anchor>
      <arglist>(QSplashScreen *splash)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>simulationMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>f38c8bef2e158756505f462b91d0ea3f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Mrs</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>bd48db9a93559a0b2c046d58765f9e8d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>exitDialog</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>2dfaa0e663f8fc14d8311154b9fa9b79</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>noHardwareErrorMessages</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>52328e3cd1c6651273b6b69f28b12b0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const QColor</type>
      <name>splashColor</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>9bed58d58d45348e460b15fd46940075</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const Qt::Alignment</type>
      <name>splashPosition</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>e6f29d52024023063ac7529336050ed0</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="private">
      <type>void</type>
      <name>readSettings</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>75a36a51c05f7aca9bcd9e87912fca1a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>cameraTestMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4dc9b8619c278510cef8adf16059e2be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>CamThread *</type>
      <name>camThread</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>dcdd6da50f4be2db5eb80bd921722777</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Circuit *</type>
      <name>circuit1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>644f63a293ffe03a6be9059e3b3079f5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>commaSeparator</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>b9ba9c9ec9617ac550b59bf7e5bca45e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>currentTestServo</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>93378bf949cae634ba6cd827cddd4972</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>dontUseCamera</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>ae5ddb2b1eda79624a33fba9776d4a39</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>eyeTestMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>e33469e0f66020cf90fcdfaa26b412a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>faceTrackingIsEnabled</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>98c80e06de90925933ef9fd9e873bdbd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Gui *</type>
      <name>gui</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>f4b0cbb238d377d382b2e968f82283dd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Head *</type>
      <name>head</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>e244d13c59ec8f99c079f8c3cfd73621</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Heartbeat *</type>
      <name>heartbeat</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>233c5d9ecdacb7c6c0657e03054d3944</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Inifile *</type>
      <name>inifile1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>3b4d976745cdfbad81dbd993d2b1e340</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterfaceAvr *</type>
      <name>interface1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>86fc2ea6b1d999d6516aa9b2e055cde3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Joystick *</type>
      <name>joystick</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>e14cfe881082a5b28eefc4ed89abad3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>JoystickDialog *</type>
      <name>joystickDialog</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>8ba11197f6213120fdb0f59202b92db8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>laserScanner1Found</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>ca4793a1070d11b1221c3f051653de2c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>laserScanner2Found</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>650e9b4d825c2c6801171dbe461b1231</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LaserThread *</type>
      <name>laserThread</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fedcc5b5cf321212279bb4a515467410</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>maximumSpeed</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fce4f1ce7277c9c961134848a5189a46</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>minimumSpeed</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>f7e7966db9f8ff91d1409944f81fe5c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>mot1Speed</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>84aa10f50c0d0c83808e01af9911945e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>mot2Speed</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>adff709a990959a67c921d32179c309a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>mot3Speed</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>905943867ea28062b77d924302b026ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>mot4Speed</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>0d87c06c1e3ee0e4f42755e91435bce6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Motor *</type>
      <name>motors</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>77fc17260dfb7127a5b16e7412fb1b02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QMutex *</type>
      <name>mutex</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>e7ab795c2826104366a4439fbb54fc00</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>NetworkThread *</type>
      <name>netThread</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>7ab0c8f4ca294fe55306b57dcb8eb544</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; int &gt;</type>
      <name>obstacleAlarmFrontLeftList</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4f5fdf975adbab3a2022786d94fbf3b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QList&lt; int &gt;</type>
      <name>obstacleAlarmFrontRightList</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>831ae320600ef5c90b60306fb2315c2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>ObstacleCheckThread *</type>
      <name>obstCheckThread</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4ff05ef9eca6341dac714826c5157679</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>PlotThread *</type>
      <name>plotThread</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>5cbec6ff563d789610e39cc3c9b198eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotDrives</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4d5f6def8893eab495819c9ce0ccaedd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotIsOn</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>bd3673bc42637e132f8a467088fd81bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotRemoteMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>28881fafdd3bb62bf53ba33215c85800</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>robotSimulationMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>20e042e77f111cf34951645272142985</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SensorThread *</type>
      <name>sensorThread</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>69713f2fd1db55a59cb732e72e3f6d76</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>serialPortLaserscannerFront</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>39fb0dbf472ccb4d96f5e3305356e00b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>serialPortLaserscannerRear</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fb6880da1dfc70b0c3339844aadbe9e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QString</type>
      <name>serialPortMicrocontroller</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>92588e4354a5db3e9d345d9f43666c31</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Servo *</type>
      <name>servos</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>6e7e7c58ed8965fe7c4fbd17edb4291f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>servoTestMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>17d9508858bed16df0c99d96cbace281</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SettingsDialog *</type>
      <name>settingsDialog</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>3370c648ea8d712790a7e7e94808c1c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QSplashScreen *</type>
      <name>splash</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>1c2eacc18ff43d871ee65031c08afb7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>testDriveMode</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>6f12177e670e807798f172cca4332743</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>BACKWARD</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>6287356390b04d6c3ea6afc43e7e61a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CLOCKWISE</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>a1f3cb3fe14346dcf07f5eac11ec05e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>COUNTERCLOCKWISE</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fb42d8d29bcbf3b2704bf54053f619af</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>FORWARD</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>8a65de5f3171c3d28e2eb1540961f652</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXIS2X</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4355b13d79c921038dfc15af4c599bf4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXIS2Y</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>cccdf3320b79615aae922e51ffda8abe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXISX</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>3a67b1e6a2f6c4f47af5f8d77f87f6c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKAXISY</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>375f717a7f9762b76ba0a2be41b11366</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>JOYSTICKDIVISOR</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>b1e569fdd748068f4235fe47be54806a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>88cca471f52d46644a86e6959834131b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER2</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>b3e4034d98190c9b9e7a145570d3b737</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER3</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>1f59785402b8d40750d62b4a198db2d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER4</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>03abe75d1d73a109abc950bb2111beb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER5</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>75ff2459bbf9067afd5f064411820eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>LEFT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>cd4e30e3ff13f40f27b7cb077fc314ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>250bb4aa8b1beacaaa1f62d86de8d8c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR2</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>154cb190940a9d2bf8ab7de29afb5bb9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR3</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>aed1c8cbf4944e5f6c621de0736f0271</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTOR4</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>69a3c8f7178c323fd8b9791dceabe93f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>91a958b137b86b20d7f76e42f531a0ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR2</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>57be79152b5bf9be7be78e050c4ed8ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>NONE</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>3400f76eaba5f09438fdc7d158c4c11d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>NUMBEROFSERVOS</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>025f874bbc2753b92640f8c14947ebec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>OBSTACLEFRONTLEFT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>9ac71e70dc820b12f05493dd7556ddbf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>OBSTACLEFRONTRIGHT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>5efa69e4eb9cce9b0430462b81baa2e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>OBSTACLESEVERYWHEREINFRONT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>e22e6e50105f6e18ee6017de8788db7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const bool</type>
      <name>OFF</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>adc63eb8a20afe59f561b943c8d9ac65</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const bool</type>
      <name>ON</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>fb9e2d85ab8a4a9cea6c00843c462570</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>RIGHT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>ae8da94680e31de8526036e1fc63dc60</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SAME</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>8c3833f3a50ee3c1ed9c1fff14cbc8e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>7f91a651803042066c5ae9f344831cc7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR16</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>c7c7829afc9850d8b87269beed4ea6ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR2</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>8a7f122ee498fbcd5e1744ada0cb2600</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR3</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>80bdecd3f5e35d77d61c9bdf2603db33</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR4</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>1344ca1d85ee85b60d750c16152ee79d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR5</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>6598373f6d0b4ecad153adbafcb33804</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR6</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>d72d7f7398c39a18ef2a76cd7b866b6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR7</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>1d74f264053822ca03a7653d8c921766</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR8</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>7aededb18d585794dd06ee21a68196b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO1</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>25588bbc39f6b82ed9e056f864a0d532</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO2</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>647d9fa9244e5ad6a4d8786a1610c03b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO3</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>516ce27f316563484fdf14e3c4ada3b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO4</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>3e73c1af579252cb8a324a4f70a8150c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO5</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>a80619f7671510672ce7e7888ea33865</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO6</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>67e2996d2c9a34c83a36178a0cc67658</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned int</type>
      <name>SPLASHTIME</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>098a7d5aaa5342ab3fdeef946013125e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>START</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4df276d1e8832ce3d6f57b39eae6a32a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>STOP</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>75bad120ec931a156b645e5f83e2b2df</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVCURRENT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>829f44f8bb10c9296c4d0dd991311fbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVDEFAULT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>a28f3c40a733bed69f5c3a3759bfced1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVEND</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>103cb19ab62448eb1578bf8c926ad82f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVMAX</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>bcf2216d8894e7c7666cdf4e965daf0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVMIN</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>4e190f92c8b7ddaa5002b7f502642259</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVSTART</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>b48cdc184669a6b96d2629b10543a2f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>WAIT</name>
      <anchorfile>classMrs.html</anchorfile>
      <anchor>b6b4e2e5b504bea298e824ea1d08a416</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>NetworkThread</name>
    <filename>classNetworkThread.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>dataReceived</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>840ca71214490cfdeb32af1fe8a8da6f</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NetworkThread</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>19bdf760c822f879a1f8a6700ee19e52</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>8659a516d3561c486844bf7bc9aff8b0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>d00403b687698c2f3e1ed09a09bf1b41</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~NetworkThread</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>daed788a9be6fbb4ed1aef9905e3e18f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>processPendingDatagrams</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>5bf5e13c1dffceb7596f3ea56663f38f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>a67b4824948c49fad55f33c437f3f44b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QUdpSocket *</type>
      <name>udpSocket</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>892986032dd0a523de81da28c68001db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>PORT</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>f802aa5814b2104e4d3e0507e916e060</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classNetworkThread.html</anchorfile>
      <anchor>bc3ca8fd5f403c3994e7ce021256d438</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>ObstacleCheckThread</name>
    <filename>classObstacleCheckThread.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>setMinObstacleDistance</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>01a259d27606cec9083bae0c839690fb</anchor>
      <arglist>(int distance)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setMinObstacleDistanceLaser</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>cddf0c088ba034accd01fba3fdcb00ec</anchor>
      <arglist>(int distance)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setRobotSlot</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>80a8ed404e08e4218cc373a138f45077</anchor>
      <arglist>(int angle)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setSimulationMode</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>3cdd3b6a253e33749a0860f9f433fd05</anchor>
      <arglist>(bool status)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setStraightForwardDeviation</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>ec86a7362668ea3c7512f180251f2123</anchor>
      <arglist>(int deviation)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>newDrivingAngleSet</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>40b26541d5c1e52d1a7cde055529f7fe</anchor>
      <arglist>(int largestFreeAreaStart, int largestFreeAreaEnd, int centerOfFreeWay)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>obstacleDetected</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>d96ba282ff73a614a57f817548279034</anchor>
      <arglist>(int sensors, QDateTime timestamp)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ObstacleCheckThread</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>2928e78639b2c3ab50b8f5b82f2ec58d</anchor>
      <arglist>(SensorThread *s, LaserThread *l)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>ff6a46914c6c9e32ed43665099658cc3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>de87cc956b2efbaa27600ca7a486c75c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~ObstacleCheckThread</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>4ee218c717c38cf23df5fd3ca4f44164</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>actualFreeAreaEnd</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>4fa5299fd79c26c0c42eeac0f8b64fbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>actualFreeAreaStart</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>49cbb6954c1696667fa7ac49975ab2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>centerOfFreeWay</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>1ac1c32da69b57689749600b73d50255</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>largestFreeAreaEnd</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>00d0ac6cf1c308892c21aa867a0e871a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>largestFreeAreaStart</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>7955b2eb3d6e8832ee6e221d71b157b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>LaserThread *</type>
      <name>laserThread</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>0db16c927e4395101f1d65de8cd634c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>minObstacleDistance</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>672b5af1c2f3127c41ff90e379003c50</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>minObstacleDistanceLaserScanner</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>b49de7cf3adbbf50cfabe308a0cbeab3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>robotSlot</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>21bc45e106322880f241e0f1141c4aae</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned char</type>
      <name>sensorValue</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>239be17420736c608db9b58a741e2e8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SensorThread *</type>
      <name>sensThread</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>15fa2f67bc79a072df806fa1786a118e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>simulationMode</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>08ff9abcff97bdb4e62380daf9a245b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>e74bfc017b70def48da0cc5f5077f957</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>straightForwardDeviation</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>26744426d1243a185c1947798bbcb714</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>CENTEROFLARGESTFREEWAY</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>a7918f0cbc15ef6f49e3da72132721e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>FREEWAY</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>9856a29d47d4eb97320cdc9d1d077bef</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>LARGESTFREEWAY</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>7433e7699bbd48a6af1d40e156b041e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER1</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>65b6bd073d17b0157f3b007dce24f886</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER2</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>b5338385c50d5aa17f9a3f1feeb32bb4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER3</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>f272090d2c4050adf9390f01a001d461</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER4</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>7ea622d2a9233b4c73de2d3c4a04ca3d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>LASER5</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>33826e3827f7ab11092abaa6603a78a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>NONE</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>be259d8c5e70c9d2a9cb2334bb703ddf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>OBSTACLE</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>92de564fdfbcc4b3641929f7c8268222</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>OBSTACLEFRONTLEFT</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>4a7163490a613e7fdabf17f8b30642b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>OBSTACLEFRONTRIGHT</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>eac925c62afc1afd5b1c039368ebde65</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>OBSTACLESEVERYWHEREINFRONT</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>8d9df0a340788ea5e15dfb23023e88bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR1</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>b7282866dfea2060f9833aa68c7926e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR16</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>3c8fe3c1db212fc907d889092be72555</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR2</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>54941b2e6c7b00edbce5ac63c5a8c407</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR3</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>a17117b2757cbf27b7e8cc09751ee9c0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR4</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>54e77f56b7740ca0634024636efefd5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR5</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>f4e0329e2afdf3e6f7a6be1773af715c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR6</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>1e85c713e5a07b9aefcbb7e51384d6c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR7</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>13632f91334dd24fe817f6482ec99c18</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR8</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>6667064bbcca5416b187804f54fe414c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classObstacleCheckThread.html</anchorfile>
      <anchor>8eba4ee9621eaf354721c98b03257344</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>PlotThread</name>
    <filename>classPlotThread.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>plotDataComplete1</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>86555eddb1b96a3adc23e1887c20f18f</anchor>
      <arglist>(double *xval1, double *yval1, int size)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>plotDataComplete2</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>ebc173ca16b2d6fc21bb631ca5889297</anchor>
      <arglist>(double *xval2, double *yval2, int size)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>plotDataComplete3</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>0d2bd3ac664c88bc295d6f9397da74ad</anchor>
      <arglist>(double *xval3, double *yval3, int size)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>plotDataComplete4</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>4d66408e8e8e483f2d93e08960e43e87</anchor>
      <arglist>(double *xval4, double *yval4, int size)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>PlotThread</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>688ad508b82746e76c7d51db327ebb08</anchor>
      <arglist>(SensorThread *s)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>c9a437bc868c26940283eb8cfec39417</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>e8085ea446d0e72f8d9b964848b28f18</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~PlotThread</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>7f3b795b1bbb59a6dc345acde1e272f3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>SensorThread *</type>
      <name>sensThread</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>ac9ef904b23db3b18fc12fc478bbb04a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>a8984c63dce297892ec2ae1206535fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>xval1</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>ae6a25fb75626b8d4d6dd6cac9cb9502</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>xval2</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>fd757f0138a8ee2b0d07c4b2cc3b1e01</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>xval3</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>6e126b80a41bcc433abca3f0a4de3833</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>xval4</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>ebed6d9c21feaf9bf6c5afa80772e0cc</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>yval1</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>62c82974fcd144269534a88219ec66d1</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>yval2</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>bc0f72a4d67fc713588c4c14dbdea262</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>yval3</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>7f5db010b0d65d1212f14bfa75bc57d0</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>double</type>
      <name>yval4</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>ee909500e93aa7bd53317beb1247d65b</anchor>
      <arglist>[SIZE]</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR1</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>eee3e4c301c2508855d3b29935fa80e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR2</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>445c873d28210129cc66ae884de7f23c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR3</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>1fb9ce044993b5cbf2af30232f3f45e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR4</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>f6ecd6afbf79a8f60c07283389b5355b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const int</type>
      <name>SIZE</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>369bbd5b794354d495a37f19542e9839</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classPlotThread.html</anchorfile>
      <anchor>d291d31bf1cc108ea797e975b421ea09</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Posix_QextSerialPort</name>
    <filename>classPosix__QextSerialPort.html</filename>
    <base>QextSerialBase</base>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>bytesAvailable</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>7545a927a327a4cc71954cdd6153d918</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>close</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>1a25f47c57b6330bd0eba4884e507523</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>flush</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>9cd779827a2deb430865d15d9f9bd776</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>getChar</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>33653b5cb8691ef824faa91e4a443a5a</anchor>
      <arglist>(char *c)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ulong</type>
      <name>lineStatus</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>1625aa0479a2524e4d14c24d113b650d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>open</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>e224dd26bb045d22f5b70f016f4ad011</anchor>
      <arglist>(OpenMode mode=0)</arglist>
    </member>
    <member kind="function">
      <type>Posix_QextSerialPort &amp;</type>
      <name>operator=</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>03ad18b4dd907e4bfd66f564c6698d33</anchor>
      <arglist>(const Posix_QextSerialPort &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Posix_QextSerialPort</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>574b415cde61cf693449338aef692211</anchor>
      <arglist>(const QString &amp;name, const PortSettings &amp;settings)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Posix_QextSerialPort</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>8ade17fa6605245162661b75b9e71105</anchor>
      <arglist>(const PortSettings &amp;settings)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Posix_QextSerialPort</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>07e9e48ffb75ad02b3cfaeb14ccbf384</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Posix_QextSerialPort</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>c409b3d891500be663894c9910749390</anchor>
      <arglist>(const Posix_QextSerialPort &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Posix_QextSerialPort</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>e6e0dda91cd654352c26686e88a416ff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>putChar</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>70cfe0ddedbe9b89cde6a6a7ca068b2a</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>readData</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>8689478ea33b21d93674e7763500be6f</anchor>
      <arglist>(char *data, qint64 maxSize)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setBaudRate</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>a381dead5691f53cd5a7e46c047617bd</anchor>
      <arglist>(BaudRateType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setDataBits</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>241f7429a251f3cf56c2a81e7bc2da41</anchor>
      <arglist>(DataBitsType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setDtr</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>6655b6b4522ca1036950304db9897b68</anchor>
      <arglist>(bool set=true)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setFlowControl</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>53736a0e771b45f670256c4654049b4e</anchor>
      <arglist>(FlowType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setParity</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>abfec4d1f51f70ee19103b3a65c3190a</anchor>
      <arglist>(ParityType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setRts</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>40e8e5d33f45e8de286c79bdaf9ea299</anchor>
      <arglist>(bool set=true)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setStopBits</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>df6a98c5e0b51a3cdbd96bdb46ba198b</anchor>
      <arglist>(StopBitsType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setTimeout</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>4e19d9132e0b07464ae229c595a0e8b4</anchor>
      <arglist>(ulong=0, ulong=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>size</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>43f714cc4f59e9d766f5ea4b6b15635d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>translateError</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>a515f7b9b06cd30d49944f43406a7d81</anchor>
      <arglist>(ulong error)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>ungetChar</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>f7077e568e154b176c6adb7315816155</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>writeData</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>927c6fbee7852b78c34b746f2131238c</anchor>
      <arglist>(const char *data, qint64 maxSize)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Posix_QextSerialPort</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>2ee76ef46c18f0eb7cdff9efa8842c76</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>struct termios</type>
      <name>Posix_CommConfig</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>4946a554d3cfcd190b4df72f6f44f6eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>struct timeval</type>
      <name>Posix_Copy_Timeout</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>e5813ea1fd76c8c5a8948c4085d3e84e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QFile *</type>
      <name>Posix_File</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>c4fcc5ae9850c3bc9952a13e4ef82273</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>struct timeval</type>
      <name>Posix_Timeout</name>
      <anchorfile>classPosix__QextSerialPort.html</anchorfile>
      <anchor>0a876397917fbf082b56ae43ab2b4b7b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>QextSerialBase</name>
    <filename>classQextSerialBase.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>atEnd</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>2a073b06559b1b52b0aa1622080a8282</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual BaudRateType</type>
      <name>baudRate</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>3b1b8bc8aae0ae95eacb768d376a5230</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>bytesAvailable</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>819a441e50eb9565077a577dd5761687</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>close</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>2a4aea428bc05a88f86849bb7478c082</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>construct</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>a576b712003a90a9897ff6b22ab0f3c2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DataBitsType</type>
      <name>dataBits</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>26455210b60ca825c9551ddd09428bdd</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual FlowType</type>
      <name>flowControl</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>37b410a25530b1bfd83b4936696ac484</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>flush</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>58df9d786aa70dfe9f547d4118103f39</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>getChar</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>204eb61395e803ffb04274c37adc90b4</anchor>
      <arglist>(char *c)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isOpen</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>eaa6af8fcc39719df424aa92ca780c2b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ulong</type>
      <name>lastError</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>5728adee70c62a231c6d29bb54a2e81f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual ulong</type>
      <name>lineStatus</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>28b87dd41160a61e2953dca38aa332d6</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>open</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>e7fadc74c97f903f6b499e6155236b3e</anchor>
      <arglist>(OpenMode mode=0)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual ParityType</type>
      <name>parity</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>e783d337d80fee91d6e064af4e935043</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual QString</type>
      <name>portName</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>924cd89932d45c60b908ec5b54f5c572</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>putChar</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>ceab4a6e39cbd3ccd059dc7e9fca4d19</anchor>
      <arglist>(char c)=0</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialBase</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>136b187c6684be0bac22e616b053f7b7</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialBase</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>ba0fc6d8b5c675c91ebff90fc6415748</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual qint64</type>
      <name>readData</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>d36903893a43ad4468e25243117dea73</anchor>
      <arglist>(char *data, qint64 maxSize)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>readLine</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>3d267326a9000f3a99f84639fa7fa9ba</anchor>
      <arglist>(char *data, qint64 maxSize)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setBaudRate</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>05b59a48366ae452bf8e033f83402543</anchor>
      <arglist>(BaudRateType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setDataBits</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>651f67ec7818b55c0053b47546c56ab5</anchor>
      <arglist>(DataBitsType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setDtr</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>14c7fc36f9e7b7e5a9e798c2c1b35505</anchor>
      <arglist>(bool set=true)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setFlowControl</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>976c031b3a51bc71abe7804953e0d366</anchor>
      <arglist>(FlowType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setParity</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>d57f8cb19a8a48012c1f607155eddedf</anchor>
      <arglist>(ParityType)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setPortName</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>a2fa8370e8c9bc6a69563f49efb77f00</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setRts</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>aa23d343095f16e7d7b37e7b61022b3a</anchor>
      <arglist>(bool set=true)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setStopBits</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>fafca39575da5ab668671bff89fdd4bc</anchor>
      <arglist>(StopBitsType)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>setTimeout</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>02812d40f1b114983ca9c25ea83d159d</anchor>
      <arglist>(ulong, ulong)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual qint64</type>
      <name>size</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>86fc9d6239b7d136a9d80b93a727e093</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual StopBitsType</type>
      <name>stopBits</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>82191fe9cc079555efeb40d7dc772bb2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>translateError</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>a9c2e9272cff3bd576560335308a8033</anchor>
      <arglist>(ulong error)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>ungetChar</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>d4564cb1b1fdfb317e6a12cb5b351adf</anchor>
      <arglist>(char c)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual qint64</type>
      <name>writeData</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>39a3aaff93640b767fd44dd7ce705e01</anchor>
      <arglist>(const char *data, qint64 maxSize)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~QextSerialBase</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>c6a30dd787c28036dbf54bb2804e5680</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ulong</type>
      <name>lastErr</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>31816951ba89902fa1dfdb6f18b80f41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>QString</type>
      <name>port</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>7459f3818391091877c88595a48a950a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>portOpen</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>dd0393f4063e7d61fdcc9dfadcb423f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>PortSettings</type>
      <name>Settings</name>
      <anchorfile>classQextSerialBase.html</anchorfile>
      <anchor>2d796e3080f78ff63e821f528439db57</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>QextSerialPort</name>
    <filename>classQextSerialPort.html</filename>
    <member kind="function">
      <type>QextSerialPort &amp;</type>
      <name>operator=</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>28e03b58b5631e8dfc6bacdb2c463c9c</anchor>
      <arglist>(const QextSerialPort &amp;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialPort</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>207af0672a6a954e6621933e93b9923e</anchor>
      <arglist>(const QextSerialPort &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialPort</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>4b30a03285f32254076bf2ee6e579709</anchor>
      <arglist>(const QString &amp;name, PortSettings const &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialPort</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>3227cce266f97fc6066239a301fdc251</anchor>
      <arglist>(PortSettings const &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialPort</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>d134e1a6ec2d0ebc4c48660d057999e5</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QextSerialPort</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>6aba59fc97222d64653f94c491747ffd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~QextSerialPort</name>
      <anchorfile>classQextSerialPort.html</anchorfile>
      <anchor>de18ba025da5d9a1bdeda45e378d7a75</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>QtGLContext</name>
    <filename>classQtGLContext.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>boxCreated</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>f7ed3f35587e02e3b1c735943cf7f9e8</anchor>
      <arglist>(int x, int y, int x2, int y2)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>saveImage</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>dd02ccada1e58d7f5755cb30f5989f05</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>enableMirrorMode</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>7374cc9df5fd83eab3f273265565cca1</anchor>
      <arglist>(int state)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QtGLContext</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>27795554b49c306bfe9398e8c4513769</anchor>
      <arglist>(const QGLFormat &amp;format, QWidget *parent=0, const QGLWidget *sharedWidget=0, Qt::WFlags f=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>QtGLContext</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>53f346d90357afcf2928a4020f80c720</anchor>
      <arglist>(QWidget *parent=0, const QGLWidget *sharedWidget=0, Qt::WFlags f=0)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setImage</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>6aef70e5c441d2257b1871e9971505bc</anchor>
      <arglist>(unsigned char *imgP, const bool flipped=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setImageData</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>f5fbd8ac65f940c5deec844cd0acb7a6</anchor>
      <arglist>(const int width, const int height, const int pixeldepth)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setZoomRect</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>08dee09990c202f6106192963e114766</anchor>
      <arglist>(float ulX, float ulY, float lrX, float lrY)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~QtGLContext</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>ccc5549118f4be8176773a7a383e157b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>drawTexture</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>a414ab44f3d1d3ee1e83159297df9517</anchor>
      <arglist>(float ul, float ur, float ll, float lr)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>initializeGL</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>4a8bd7df6ba168c4fcf3b976ba017a31</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>unsigned int</type>
      <name>NextLargerPowerOfTwo</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>f517c609b92d1535f463e6f8148806ee</anchor>
      <arglist>(unsigned int n)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>paintGL</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>31521e2405afaa418715f95fb4a064d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>resizeGL</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>40fdcb776772f707cc7e535764828b65</anchor>
      <arglist>(int w, int h)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QPoint</type>
      <name>m_entry</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>899e3fb44edac140bef851a11bfa3b28</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QPoint</type>
      <name>m_exit</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>aa79d7c16304db27c7785838e5ced32e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>m_flipped</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>3a6c301bdcd3d194c952dbaecd949b89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>m_height</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>16b0f033014f18b740783f4720cb6836</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned char *</type>
      <name>m_imgP</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>4c4e78a29b468f63396977d6df21f14f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>m_mirrored</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>19e3abb438d1a4bf05092d50e79fa10c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>m_mousePressed</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>19c2abceee8096dce0ebdd821532426c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>m_pixeldepth</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>f4fd20162a59b078ae60ce03ef8cf6d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>m_texHeight</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>def5b3af6ac265e28d6a5f0edd783501</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>m_texLowerRightX</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>eb83c976d214cce790d88993597c8f83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>m_texLowerRightY</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>86e11c51d2f6c089bdef27470618d649</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>m_texNameGL</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>d29f9fa8d9b0a786fa48c19df9613ef8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>m_texUpperLeftX</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>09e4043aa7d6230e8f0b27d3fa30c06a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>float</type>
      <name>m_texUpperLeftY</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>875fb3287d2ae50bba9aa2c5b7351985</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>m_texWidth</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>c9e80ff2499441cfae0b965f0a6ce4be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned int</type>
      <name>m_width</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>78d9200262f2fa36672590c76e45ac11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>unsigned char *</type>
      <name>textureGL</name>
      <anchorfile>classQtGLContext.html</anchorfile>
      <anchor>119e18e76086267a340f824ff50cc360</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SensorThread</name>
    <filename>classSensorThread.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>resetDrivenDistance</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>e19eaf3f93d668d187f91872429eec7d</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>setSimulationMode</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>5b088b4b348ede7b4ef26c7214ec492e</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>contactAlarm</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>a26a189c66a12c662276a53f737f8d57</anchor>
      <arglist>(char position, bool state)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>sensorDataComplete</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>184faa1dc7f377931953d1c2bf16b7cb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>convertToDistance</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>542a7e45676a73c31176063564f904f5</anchor>
      <arglist>(int sensorValue)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>convertToSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>560aec8ef4adc3398bf3fedfe2e080ab</anchor>
      <arglist>(int distance)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getContactValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>7c8b8670bcbfa54c377e6397c3855d02</anchor>
      <arglist>(int contact)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getDistance</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>9c267070b70106a31ebfb505bd1630aa</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getDrivenDistance</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>b5631f7e5eb5f83927d4daec7881e695</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getIrSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>d537703677bcea531378e97fade3834d</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getMAmpere</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>3496bfa24ef59cc8ce1667bd04e131d4</anchor>
      <arglist>(int motor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getMotorSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>fa0dbf40fd6d225ada4786e109c9701e</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getUsSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>87206c0146c8157b7985098d015134ef</anchor>
      <arglist>(int sensor)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>7f07b1cb000c5de8b88c7a44d637ec47</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SensorThread</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>3e7a27d26b408092a43c366b34a5f10e</anchor>
      <arglist>(InterfaceAvr *i, QMutex *m)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>26436b23d31390a5b6f4346f6b29eab3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SensorThread</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>1964356150f7b6d6fbd83a788ff4c830</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>char</type>
      <name>contactValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>9e83fe8c569c5ad897dae8f2ad02021b</anchor>
      <arglist>[CONTACTARRAYSIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>drivenDistance</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>030a1a72a3d497910d22fcf709a4282d</anchor>
      <arglist>[DRIVENDISTANCEARRAYSIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterfaceAvr *</type>
      <name>interface1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>be1081a8594220eb14bf584385c8abb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>iRDistance</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>1f7df7b80d8d3635f0a3f699823b4f7d</anchor>
      <arglist>[IRSENSORARRAYSIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>iRSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>81b60b68047faee318d3ec42a6ccd0fa</anchor>
      <arglist>[SENSOR8+1]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>motorSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>d079a5218785bd5d43c0c35ce61aeac6</anchor>
      <arglist>[MOTORSENSORARRAYSIZE]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QMutex *</type>
      <name>mutex</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>6f0da1af08f993e3b91214ca70822392</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>simulationMode</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>76615ff6f8017119dfcfb20c814e0c73</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>aa7092875b87a4e66ff0062159554daf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>usSensorValue</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>b6f77a6a03a31e1deea60fa5a9ba3ea6</anchor>
      <arglist>[USSENSORARRAYSIZE]</arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>BOTTOM</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>181a0ffd5cb0030dca7c16709c85ecab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CONTACT1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>dc2b9c984ff914befaeaaed41f093449</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CONTACT2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>94eec87d6e1fa53c80092a3d2c84595a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CONTACT3</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>ae475a5c347cf89fd19bd30e7cca5c24</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CONTACT4</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>18e88833225a6d1d139fda2c71fa1b1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CONTACTARRAYSIZE</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>9731494cf228a50434ef27cc97fa2b03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>CONVERSIONFACTORMOTORSENSOR</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>d395f072f6fe9d3d40cf989d9d1c8feb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>DRIVENDISTANCEARRAYSIZE</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>971243920dcccac8424f5652528ef7a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>IRSENSORARRAYSIZE</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>eec5f9bca8706d70aac2783fd9421932</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>LEFT</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>88c927f60115b9d35013f4aa34151220</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>244e2f3d43791030b916e44ee3b3b014</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>cca1b414e351e11e5f2bd0f86f3d21b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR3</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>cb0352c0efc06e286e6e6c1f4e69c568</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>MOTORSENSOR4</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>605a6404353f332648545fa26aba33b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>MOTORSENSORARRAYSIZE</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>179463273b2dd085d76b62a4011a570b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_CONTACT1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>1b1d7054f7f00370c8e682c9066af878</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_CONTACT2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>bdc7d25cea3335bfaec8afd04a7c1c22</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_CONTACT3</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>7a6377340a2f65c9ddf712ac48678144</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_CONTACT4</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>4baedc814d2590e1b15ed5a906245c12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_MOTOR_DISTANCE1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>06e30a4c47446e619ce8f91f9c77450f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_MOTOR_DISTANCE2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>a25728784ba3213288bfb7eae362b6f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_MOTOR_SENSOR1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>3824de4ee67b9ad372078c2d29475a48</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_MOTOR_SENSOR2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>314d63f55294f67708a5001595f5137c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>d207e79968dd711e914813508e28b978</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_16</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>86fb7d1d3ef71a5668ab54cd13b9ae61</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>f3414f8174f5a77ef3b0deb25b5a2359</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_3</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>fb27707277dbd06bc1781906b9e3b433</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_4</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>24359c2b7f63904fc89a844afe3bed71</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_5</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>2fc39b4a82ff643f47469c5f13fc25bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_6</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>d928c37a038f4c75c660b467d4fd0df1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_7</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>a4e993f9eafe71778bb0da6aac67b517</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>READ_SENSOR_8</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>079e8d0fe10d4e4c075fd2a8d90a243a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>RESET_MOTOR_DISTANCE1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>3ec352ce2f50f3952bcc1358b6c50703</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>RESET_MOTOR_DISTANCE2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>8689b973776c08534a953fb35a2faf9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>RIGHT</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>109f1619b82f3b64c4b23fbce493b912</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR1</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>40f56add640ab42b804c855217fc14c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR16</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>7beb53c5418b3248a7de843a080259ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR2</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>daf6eafcf9958f3be83d6cc66749a51f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR3</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>8f93c4fca9b5a66d4b84f40b7e5e5ef6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR4</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>c69dc4c7457569e7adfb78caa9937b11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR5</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>5f940fed52ffd28253c53b9f49daa68b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR6</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>6a0a9a630da3196a9b209f2d7991cda5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR7</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>d34164a0d0b27a67a1979678394b6d49</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const short int</type>
      <name>SENSOR8</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>78f7281a8b0ad36f45bb124b0920f866</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>c2fe499ef5c97b3590662c812969bc1f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const char</type>
      <name>TOP</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>93ef6591cc2aa44ce169cf9eb90f7cf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>USSENSORARRAYSIZE</name>
      <anchorfile>classSensorThread.html</anchorfile>
      <anchor>04259edfae6ad0137ef928140472a890</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Servo</name>
    <filename>classServo.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>init</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>84cf6d98d86f9f2424c0a40f4e533e3f</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="slot">
      <type>void</type>
      <name>moveServo</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>50019278e68334d583893770567b1e4e</anchor>
      <arglist>(unsigned char servo, unsigned char position)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>message</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>90eb4ee576e5c4fdddd90926123492e7</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="function">
      <type>unsigned char</type>
      <name>getServoPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>e96ce9ca7e632339a915c9bec552d01e</anchor>
      <arglist>(int servo, unsigned char type=SVCURRENT)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>462745dc4bfea892a5a65603590ba748</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Servo</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>2f6d934c0173a58a2d7b7627a15283e4</anchor>
      <arglist>(InterfaceAvr *i, QMutex *m)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setServoPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>421751a5ec3282134387b8ab4c615562</anchor>
      <arglist>(int servo, unsigned char type, unsigned char position)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>ef590ce5ad82d3373abecd04c521d163</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Servo</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>cb51bf4d970b071741ba76349a431fb0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>InterfaceAvr *</type>
      <name>interface1</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>2f4da82204f93b5e13cc383c644f8895</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>QMutex *</type>
      <name>mutex</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>a0c2f1a37bd94068a02033d974fa75a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>servoDefaultPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>5fe0bb3d0c2cc8b057935a20ab110ef5</anchor>
      <arglist>[NUMBEROFSERVOS]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>servoEndPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>5447a5e19f79887f9505d8dae30dd452</anchor>
      <arglist>[NUMBEROFSERVOS]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>servoMaxPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>5a70c2a8da020f5426c7974fdccef215</anchor>
      <arglist>[NUMBEROFSERVOS]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>servoMinPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>8bdf16240cab781e7fe6f151d1a39751</anchor>
      <arglist>[NUMBEROFSERVOS]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>servoPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>67ea5204379678e6ce662c0a15973f72</anchor>
      <arglist>[NUMBEROFSERVOS]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>int</type>
      <name>servoStartPosition</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>6c977559f3bf0f15d57c8555e5195340</anchor>
      <arglist>[NUMBEROFSERVOS]</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>6ed495f6287c5598851367c893cf2f46</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>NUMBEROFSERVOS</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>d816c8183857fe2bf612c1dcbd2529f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO1</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>3759627db3ffa9746cd31c08a2b57a41</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO2</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>21c0acd07b2747cbf3e1f5beb3890272</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO3</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>5e5766246f7c5d558a7a4522bcc25235</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO4</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>c3a26bf18a1213734d2126ab8273b422</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO5</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>9b72935acb3bec002788d3ce86c38349</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SERVO6</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>8b52c8bfd5746831b911bcb3f46ee6a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SET_SERVO1</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>7ac8da58e98007c5f353d2b031f66c91</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SET_SERVO2</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>6b270e24b415ec7c332e8fe654360104</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SET_SERVO3</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>c3377f8f08374c4648acb83c7bcc7094</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SET_SERVO4</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>1e8867e721eff63c303771235d6836f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SET_SERVO5</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>ec4fe11c9dda4ce717eb6b636d1185bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SET_SERVO6</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>edadb90c21af9d60889d1de736a05d30</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVCURRENT</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>9260bbeb3c3124c5b4645d82dc123477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVDEFAULT</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>c03fc488dcb690e5968773e9250410fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVEND</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>2f525e9a0f25e2eed80c5f2f4115b48c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVMAX</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>1839133cc1e1b8fbdba4df3a4b6d056d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVMIN</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>8530fae0305ebd41a7fe2478d7cd078f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>SVSTART</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>0dba9a06c84e18998850df3f5e40d9d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classServo.html</anchorfile>
      <anchor>f47e5a401df0c77deca1701e2b881870</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SettingsDialog</name>
    <filename>classSettingsDialog.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>setMinObstacleDistance</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>4932b70be3ea9fd872da24ad315081e5</anchor>
      <arglist>(int distance)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>setMinObstacleDistanceLaser</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>b3a4e77660cbf192d0a60087b3955128</anchor>
      <arglist>(int distance)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>setMotorSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>ea5de54f9aed9fdeec0df93531dc02e8</anchor>
      <arglist>(int motor, int speed)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>setRobotSlot</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>fc79edf2ec4e0c69ca6cd40b9c36a951</anchor>
      <arglist>(int angle)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>setStraightForwardDeviation</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>66b707ec3aebfa1fe6be4b3392b8eacc</anchor>
      <arglist>(int deviation)</arglist>
    </member>
    <member kind="function">
      <type>Qt::CheckState</type>
      <name>getCheckBoxSaveSettings</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>9e6e4e60d65cda1c6c5040f5f985169d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderMaximumSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>ffe03a179c88a46553bbaf3f683b9598</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderMinimumSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>cfeac613ac82c7257c06069e389f9808</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderMotorSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>0fc82a8fc93bf32f090d82f45133840d</anchor>
      <arglist>(int motor)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderObstacleLaserScannerValue</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>e31c3f2d0d29e79453d5ebc57524df5c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderObstacleValue</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>2031f880d2080d26acb5b18e7846069a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderRobotSlotValue</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>4b4c6dfa8e39b1afad7592f97b47698e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getSliderStraightForwardDeviationValue</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>86bc70bfa74f611bcb8ad739def923d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCheckBoxSaveSettings</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>e077ddac225b1f1772e7123d5dea10bc</anchor>
      <arglist>(Qt::CheckState state)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderMaximumSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>3120d94481be4465b6ce6511a4f17e10</anchor>
      <arglist>(int speed)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderMinimumSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>9e01a050256ff5e86e9ce3ef0e985006</anchor>
      <arglist>(int speed)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderMotorSpeed</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>206d376c2f57d69c8e18006813a1f58f</anchor>
      <arglist>(int motor, int value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderObstacleLaserScannerValue</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>01130fb732be674626b582d7eacd5b88</anchor>
      <arglist>(int value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderObstacleValue</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>640e6dab89c605b864f116e293b5a441</anchor>
      <arglist>(int value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderRobotSlot</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>578c896e09a1c5d4b52e8043c381e2e9</anchor>
      <arglist>(int angle)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSliderStraightForwardDeviation</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>a7c38d3d4c3d647487ae82c4162cb93d</anchor>
      <arglist>(int angle)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SettingsDialog</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>9933956b777b2c0451e9119581cc22fb</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_sliderMotor1Speed_sliderReleased</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>315ebe2593acc644334deb59b8536cd9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="slot" protection="private">
      <type>void</type>
      <name>on_sliderMotor2Speed_sliderReleased</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>fdaff5fef4219c978912b6a8dd2fea56</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>Ui::settingsDialog</type>
      <name>ui</name>
      <anchorfile>classSettingsDialog.html</anchorfile>
      <anchor>106922a08ddff514e9efcdff95818686</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>sick_laser_t</name>
    <filename>structsick__laser__t.html</filename>
    <member kind="variable">
      <type>unsigned char *</type>
      <name>buffer</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>bfc5f86a78c10574d02d4e46b289f0fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long int</type>
      <name>buffer_position</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>9acab26665207eba2cddd5384f8f0c8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>laser_device_t</type>
      <name>dev</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>5281934a6d7e6c26a74ea3c2010836c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int *</type>
      <name>glare</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>1b68b604667bcfb1d0c6961a53246a3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>new_reading</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>17616de42e91dc308cf895b4d3b419c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>numvalues</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>eec2066ed178f7498ec2e823d4bd9855</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long int</type>
      <name>packet_length</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>e0884d50a40bbd35002a681e17266f10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long int</type>
      <name>packet_offset</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>18f8647072dd79125a04e63f74aeb8b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>packet_timestamp</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>335123f98e83bfc8787ffa4ead8bdac8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long int</type>
      <name>processed_mark</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>d7558c72fa8ecfb4860e8a9a51804ee3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double *</type>
      <name>range</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>f996d6ba958bb16cb343603013b8b1f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double *</type>
      <name>remission</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>4bce98b35d85479db5a7de1201909516</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>remvalues</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>999262f2e5b93677305d145ce60b6451</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>laser_settings_t</type>
      <name>settings</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>05022a56830253affb18bb187ec17ac2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int *</type>
      <name>sfv</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>ad006bb240a6a7e38fef4aa4fe0e0583</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>double</type>
      <name>timestamp</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>e990095ed9ae65a63dc33c107dc10323</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int *</type>
      <name>wfv</name>
      <anchorfile>structsick__laser__t.html</anchorfile>
      <anchor>4a55c36c0522b5d309c9021fb1b4db85</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SpeakThread</name>
    <filename>classSpeakThread.html</filename>
    <member kind="slot">
      <type>void</type>
      <name>speak</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>e106a8077974e14f58b4e7a71c9f600a</anchor>
      <arglist>(QString text)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>run</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>1685e5e2cc12fa0c16baecd93b8ea173</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SpeakThread</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>29b11e84a6c29a0b3fb7e9a5b9591c13</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>9fd186acc5321af0ad5962362ffb3db9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~SpeakThread</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>4906c6a1c937a29243499091e0b26ac1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="private">
      <type>QString</type>
      <name>removeHTML</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>08c456178b91c97dc9aca4556046f024</anchor>
      <arglist>(QString string)</arglist>
    </member>
    <member kind="variable" protection="private">
      <type>bool</type>
      <name>speaking</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>e22206a994b0fc9cbf14c82f7c076c68</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private">
      <type>volatile bool</type>
      <name>stopped</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>b9a88f86dc937a91799bbe2bb22431b8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned char</type>
      <name>FLAG</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>ce1b4fa63327524eeb7e0c1e5116857e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="private" static="yes">
      <type>static const unsigned long</type>
      <name>THREADSLEEPTIME</name>
      <anchorfile>classSpeakThread.html</anchorfile>
      <anchor>cc961092560da7cef725ef4a76fccb7e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Win_QextSerialPort</name>
    <filename>classWin__QextSerialPort.html</filename>
    <base>QextSerialBase</base>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>bytesAvailable</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>2a0178755262efecbbb6b33882557a53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>close</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>2acfb52a3868f397e00c3e27aff9430d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>flush</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>a0807a25dd860456cb9ceb86f23ce1bc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>getChar</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>20b2a1c5b67cb1b7674039e50f6f3d32</anchor>
      <arglist>(char *c)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual unsigned long</type>
      <name>lineStatus</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>680c8d9a96f4f1c8c2352ee4852ae1f3</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>open</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>fb67edc7663928f29b07d0959a5f4529</anchor>
      <arglist>(OpenMode mode=0)</arglist>
    </member>
    <member kind="function">
      <type>Win_QextSerialPort &amp;</type>
      <name>operator=</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>c06db0dfd8ee9ff32f0146c955d315cc</anchor>
      <arglist>(Win_QextSerialPort const &amp;s)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>putChar</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>483c379c28f5c1378ae9dc7b7f13e636</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>readData</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>4076b0eef05c6fbcc199ad241cb6d786</anchor>
      <arglist>(char *data, qint64 maxSize)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setBaudRate</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>9862cac1b101ae5ff2f2fa6ada0f3270</anchor>
      <arglist>(BaudRateType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setDataBits</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>786c84a915d0eaf8585ceacdfb0123c4</anchor>
      <arglist>(DataBitsType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setDtr</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>9b85b664b4e6ef83355c8cc220ee7a56</anchor>
      <arglist>(bool set=true)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setFlowControl</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>3ebe2a53575ece86a36deb96c1a71950</anchor>
      <arglist>(FlowType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setParity</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>6c5969578d976f6e3f31d8dbed832b81</anchor>
      <arglist>(ParityType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setRts</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>50b14c6401ac4b0fa432cc50b5a5ba62</anchor>
      <arglist>(bool set=true)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setStopBits</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>5d2370137e18e9a1557f134eb59f3f16</anchor>
      <arglist>(StopBitsType)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setTimeout</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>0370601df91e0376e17209612c0d5c9d</anchor>
      <arglist>(unsigned long=0, unsigned long=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>size</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>4834c4712a26014e07eb28a60cc51372</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>translateError</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>de2f62d7c94c8b5a58357f8dedb6a1cc</anchor>
      <arglist>(unsigned long)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>ungetChar</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>810733b183c9353e7faa26a54a7ba73a</anchor>
      <arglist>(char c)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Win_QextSerialPort</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>5701350b1c29e6187e4f50946b88a855</anchor>
      <arglist>(const QString &amp;name, const PortSettings &amp;settings)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Win_QextSerialPort</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>8eebc2f042fe2b9b782fcac8cee7689c</anchor>
      <arglist>(const PortSettings &amp;settings)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Win_QextSerialPort</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>b88b54d3b5d528b75b7a5fbfe83ca673</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Win_QextSerialPort</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>a2968038a33b91c0f21e43f2a267a35c</anchor>
      <arglist>(Win_QextSerialPort const &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Win_QextSerialPort</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>3f96ad148dd04e48d2251d85866f2bb7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual qint64</type>
      <name>writeData</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>f19e3f91521f067153ec4cd1fa67ea0a</anchor>
      <arglist>(const char *data, qint64 maxSize)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Win_QextSerialPort</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>3ccf56b7795903500f3aa11e1ad33c87</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>COMMCONFIG</type>
      <name>Win_CommConfig</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>a51a3047aa92b01531219215bb415dbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>COMMTIMEOUTS</type>
      <name>Win_CommTimeouts</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>a3909dbcf96493e43d79e075e626d68b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>HANDLE</type>
      <name>Win_Handle</name>
      <anchorfile>classWin__QextSerialPort.html</anchorfile>
      <anchor>f6dae6bb1c6b51e766e551bc11f1721d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>/home/markus/develop/mrs/mrs/</name>
    <path>/home/markus/develop/mrs/mrs/</path>
    <filename>dir_888f1a24b07b8718275397a16edc9252.html</filename>
    <dir>/home/markus/develop/mrs/mrs/trunk/</dir>
  </compound>
  <compound kind="dir">
    <name>/home/markus/develop/mrs/</name>
    <path>/home/markus/develop/mrs/</path>
    <filename>dir_20ec99836b7c11db194f0e90693901f0.html</filename>
    <dir>/home/markus/develop/mrs/mrs/</dir>
  </compound>
  <compound kind="dir">
    <name>/home/markus/develop/mrs/mrs/trunk/src/</name>
    <path>/home/markus/develop/mrs/mrs/trunk/src/</path>
    <filename>dir_31bb2ffc3ea2c3c0101206aa95f93310.html</filename>
    <file>aboutDialog.cpp</file>
    <file>aboutDialog.h</file>
    <file>camThread.cpp</file>
    <file>camThread.h</file>
    <file>carmenserial.cpp</file>
    <file>carmenserial.h</file>
    <file>circuit.cpp</file>
    <file>circuit.h</file>
    <file>gui.cpp</file>
    <file>gui.h</file>
    <file>head.cpp</file>
    <file>head.h</file>
    <file>heartbeat.cpp</file>
    <file>heartbeat.h</file>
    <file>inifile.cpp</file>
    <file>inifile.h</file>
    <file>interfaceAvr.cpp</file>
    <file>interfaceAvr.h</file>
    <file>joystick.cpp</file>
    <file>joystick.h</file>
    <file>joystickDialog.cpp</file>
    <file>joystickDialog.h</file>
    <file>laser_main.cpp</file>
    <file>laser_main.h</file>
    <file>laser_messages.h</file>
    <file>laserScene.cpp</file>
    <file>laserScene.h</file>
    <file>laserThread.cpp</file>
    <file>laserThread.h</file>
    <file>motor.cpp</file>
    <file>motor.h</file>
    <file>mrs.cpp</file>
    <file>mrs.h</file>
    <file>networkThread.cpp</file>
    <file>networkThread.h</file>
    <file>obstacleCheckThread.cpp</file>
    <file>obstacleCheckThread.h</file>
    <file>param_interface.h</file>
    <file>plotThread.cpp</file>
    <file>plotThread.h</file>
    <file>posix_qextserialport.h</file>
    <file>qextserialbase.h</file>
    <file>qextserialport.h</file>
    <file>qrc_mrs.cpp</file>
    <file>QtGLContext.cpp</file>
    <file>QtGLContext.h</file>
    <file>sensorThread.cpp</file>
    <file>sensorThread.h</file>
    <file>servo.cpp</file>
    <file>servo.h</file>
    <file>settingsDialog.cpp</file>
    <file>settingsDialog.h</file>
    <file>sick.cpp</file>
    <file>sick.h</file>
    <file>speakThread.cpp</file>
    <file>speakThread.h</file>
    <file>win_qextserialport.h</file>
  </compound>
  <compound kind="dir">
    <name>/home/markus/develop/mrs/mrs/trunk/</name>
    <path>/home/markus/develop/mrs/mrs/trunk/</path>
    <filename>dir_e9a18e227c2249476d6947fae9d5ad7e.html</filename>
    <dir>/home/markus/develop/mrs/mrs/trunk/src/</dir>
  </compound>
</tagfile>
