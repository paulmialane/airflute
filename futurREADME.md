# Airflute 

:notes:

Brève présentation du projet.
Lorem Impsum sit amet dolor...

# Table of Contents

- Airflute
  - [Table of Contents](#table-of-contents)
  - [Creating the flute](#creating-the-flute)
  - [Building the project](#building-the-project)
    - [Requirements](#requirements)
    - [Building the project](#building-the-project-1)
    - [Playing the flute](#playing-the-flute)
  - [Contact us](#contact-us)


# Creating the flute

A simple presentation of the hardware and the creation of the flute. Further explaination on the hardware in the hardware file.



# Building the project

## Requirements

- [Zephyr RTOS and SDK](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)

> **Note:** The project was developed using [Zephyr RTOS v.3.6](https://github.com/zephyrproject-rtos/zephyr/releases/tag/v3.6.0) and [SDK v0.16.5-1](https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v0.16.5-1). If you encounter any issues with other versions, please try using this version.



## Building the project

Here are the steps to build the project for the [Nordic XIAO BLE SENSE](https://wiki.seeedstudio.com/XIAO_BLE/) board:

1. Clone the repository:

```bash
git clone https://github.com/paulmialane/airflute.git
```

2. Navigate to the zephyr project directory:

```bash
cd path/to/zephyrproject
```

3. Source the Zephyr environment:

On linux or MacOS:
```bash
source path/to/zephyr-env/bin/activate
```
On WindowsPowerShell:
```bash
path/to/zephyr-env\Scripts\Activate.ps1
```

4. Build the project:

```bash
west build -b xiao_ble_sense path/to/airflute
```

5. Flash the project:

On the XIAO BLE SENSE board, press the reset button twice to enter the bootloader mode. Then, copy the generated `zephyr.uf2` file of the `build` folder to the device.

On Linux or MacOS:
```bash
cp build/zephyr/zephyr.uf2 /media/$(whoami)/DeviceName
```

On Windows:

You can directly drag and drop the `zephyr.uf2` file to the device using the file explorer.



# Playing the flute

1. Install a synthesizer on your computer that can connect to MIDI devices. Depending on the software you might need to install a soundfont to get the flute sound. For a simple use on Linux we recommend installing `qsynth` and adding the soundfont of your choice.

Alternatively, you can use an android app that can connect to MIDI devices. We recommend using `FluidSynth MIDI Synthesizer` available on the Play Store.

> Note: We also tested on MAC OS with GarageBand but found that the latency was quite high compared to qsynth on Linux.

> Fun part : Don't restrict yourself to flute sounds, you can use any soundfont you like. You can play the trumpet, the saxophone, the piano, the drums, etc. The flute act as a synthesizer and can play any sound you want.

2. Be sure the flute is powered on, whether it is connected to the computer with the USB cable or powered by the battery.

3. Connect to the XIAO BLE SENSE board using Bluetooth on Linux or Mac. The device should appear as `Airflute` in the list of available devices. Pair and connect to it. 

4. Open the synthesizer software and select the MIDI device as the input.

5. Have fun playing the flute!

6. Optionally, if you are connected to the computer with the USB cable, you can also use the serial port to see the logs and debug the project.

On Linux with `tio` :
```bash
tio /dev/ttyACM0
```

On Windows with `PuTTY` for instance.



> Note: Sadly, we were not able to make the Bluetooth connection work on Windows. If you have any idea on how to make it work, feel free to contribute.

# Contact us

If you have any questions, suggestions, or feedback regarding this project, feel free to reach out to us through GitHub issues or discussions. However, if you don't receive a response, you can also contact us directly at our academic email addresses:

- Justine OGER: [justine.oger@telecom-paris.fr](mailto:justine.oger@telecom-paris.fr)
- Lucas MICHEL: [lucas.michel@telecom-paris.fr](mailto:lucas.michel@telecom-paris.fr)
- Paul MIALANE: [paul.mialane@telecom-paris.fr](mailto:paul.mialane@telecom-paris.fr)

> Please note that while we welcome your messages, we cannot guarantee a prompt response due to our academic commitments. We appreciate your understanding.

