# UCO DWM1001-DEV GUI

<p align="justify">
An open-source, standalone GUI for control, processing and data recording of the DWM1001-DEV development boards developed by Qorvo company.
This GUI has been developed in c++ using Qt 6.6.1 framework and Qt Creator 12.0.1 for open source development.

This repository contains the source code required to build and run this software.

</p>
</p>

<p align="center">
  <img src="https://github.com/Antoi11/DWM1001-DEV-GUI/blob/main/media/mainwindow_example.png" alt="DWM1001-DEV: GUI main window example"/>
</p>

<h2>Table of contents</h2>
<p align="justify">
<ul>
<li><a href="#MainFeatures">Main features</a></li>
<li><a href="#Installation">Installation</a></li>
<li><a href="#Usage">Usage</a></li>
<li><a href="#LicenseContributing">License and contributing</a></li>
</ul>
</p>

<h2 name="MainFeatures">Main features</h2>

- Ready to use graphical user interface.
- No DWM1001-DEV firmware modification required.
- DWM1001-DEV configuration options available.
- Real time distance graphical representation up to four ranging anchors in the network.
- Real time positioning graphical representation from a tag connected to the users PC.
- Real time positioning graphical representation from a Passive Anchor connected to the users PC.
- Media filtering of the received data
- Capable of saving data in a 'csv' file.
- Statistics estimation from received data.
- Possibility to assign an alarm to different distance or position values and associating it with the activation of a DWM1001-DEV's GPIO.
- Terminal to provide a direct communication with the device through UART Shell mode.

<h2 name="Installation">Installation</h2>

To debug and build this source code into a executable binary, Qt for open source development is available in https://www.qt.io/download-open-source . As the project has been developed in Qt version 6.6.1, this version is recommended.

<h2 name="Usage">Usage</h2>

Detailed documentation, including a user guide with some usage examples is available [here](https://github.com/AntonioRuizR/UCO-DWM1001-DEV/blob/main/User%20Guide.pdf).

<h2 name="LicenseContributing">License and contributing</h2>

New contributions to this project are welcome.
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
