# UCO DWM-GUI

<p align="justify">
An open-source, standalone GUI for control, processing and data recording of the DWM1001-DEV development boards developed by Qorvo company.
This GUI has been developed in c++ using Qt 6.6.1 framework and Qt Creator 12.0.1 for open source development.

This repository contains the source code required to build and run this software. Also, in 'releases' section, a windows build for 64 bits is available.

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
  
- DWM1001-DEV and GUI configuration options available.

  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/config_wind.png" alt="DWM1001-DEV: GUI main window example"/>
  </p>
  
- Real time distance graphical representation up to four ranging anchors in the network.
  
  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/Graph_4_distances.gif" alt="Distance representation: 4 devices"/>
  </p>

- Real time positioning graphical representation from a tag connected to the users PC.
- Real time positioning graphical representation from a Passive Anchor connected to the users PC.

  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/Graph_position.gif" alt="Position data representation example (video)"/>
  </p>

- Media filtering of the received data and its graphical representation.
  
- Capable of saving data in a 'csv' file (Index | Date and time | Device ID | Distance).

    |1,16/04/2024 12:29:50.108,8889,1.44,C7AD,0.94,5A1E,3.83,569A,3.34,0  
    |2,16/04/2024 12:29:50.201,8889,1.46,C7AD,1.02,5A1E,3.82,569A,3.42,0  
    |3,16/04/2024 12:29:50.302,8889,1.41,C7AD,1.01,5A1E,3.79,569A,3.38,0  
    |4,16/04/2024 12:29:50.402,8889,1.5,C7AD,1.04,5A1E,3.79,569A,3.29,0  
    |5,16/04/2024 12:29:50.504,8889,1.48,C7AD,0.97,5A1E,3.81,569A,3.27,0  
    |6,16/04/2024 12:29:50.602,8889,1.42,C7AD,0.96,5A1E,3.81,569A,3.35,0  
    |7,16/04/2024 12:29:50.702,8889,1.43,C7AD,1,5A1E,3.83,569A,3.34,0  
    |8,16/04/2024 12:29:50.803,8889,1.53,C7AD,1,5A1E,3.83,569A,3.38,0  


- Statistics estimation from received data.

  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/stats.png" alt="Distance statistics example (video)"/>
  </p>
  
- Graphical representation of all received data in a window.

  -- Distance
  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/graph_history.png" alt="Data history example"/>
  </p>
  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/graph_history_video.gif" alt="Data history example (video)"/>
  </p>

  -- Position
  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/graph_history_pos.gif" alt="Data history example (video)"/>
  </p>

- Possibility to assign an alarm to different distance or position values and associating it with the activation of a DWM1001-DEV's GPIO.

  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/alarm_dist.gif" alt="Distance alarm example"/>
  </p>
  <p align="center">
    <img src="https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/media/alarm_pos.gif" alt="Position alarm example (video)"/>
  </p>


- Terminal to provide a direct communication with the device through UART Shell mode.

<h2 name="Installation">Installation</h2>

To debug and build the source code into a executable binary, Qt for open source development is available in https://www.qt.io/download-open-source . As the project has been developed in Qt version 6.6.1, this version is recommended.

<h2 name="Usage">Usage</h2>

Detailed documentation, including a user guide with some usage examples is available [here](https://github.com/AntonioRuizR/UCO-DWM-GUI/blob/main/User%20Guide.pdf).

<h2 name="LicenseContributing">License and contributing</h2>

New contributions to this project are welcome.
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
