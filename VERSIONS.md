# Robot Control Box versions

Which control box you have, and which firmware, software and manual go with it.

## Releases

There is no release yet. The control box is in beta: the files on the main branch of this repository (the STEP
files, the BOM, the assembly manual, the mainboard firmware) are the latest, and the instructions, the code and the
3D print files may still change.

## Mainboard revisions

The runtime supports two mainboard PCBs, v1 and v2. The version decides how the E-stop channels on the connector
panel are read:

| PCB | E-stop channels the runtime reads |
|---|---|
| v1 | E-Stop 1 only (terminals E2 and E4). E-Stop 2 always reads pressed, a fault in the board's layout, so the runtime ignores it: on a v1 board, wire the E-stop button to E-Stop 1 |
| v2 | both, cross-checked: either channel pressed stops the robot. The two disagreeing for more than 200 ms is a channel fault, logged as `E-stop CHANNEL FAULT` and treated as pressed |

More in the manual: [Emergency stop wiring](https://docs.source-robotics.com/par6/latest/rcb/02-safety/emergency-stop/).

## The beta

| Part | In the beta | More |
|---|---|---|
| Mainboard | the beta batch, shipped flashed with the RCB code release v0; it has cut traces and jumper wires that fix known mistakes of the batch: do not remove them | [PAR6 beta guide](https://github.com/Source-Robotics/PAR6-Collaborative-Robot-Arm/blob/main/beta_guide.md) |
| Mainboard firmware | the [`RCB motherboard firmware`](RCB%20motherboard%20firmware/) folder on the main branch; no release yet | [Downloads and files](https://docs.source-robotics.com/par6/latest/par6/cad-files/) |
| Operating system | [ros-realtime-rpi4-image 24.04.2_v6.8.4-rt11-raspi_ros2_jazzy](https://github.com/ros-realtime/ros-realtime-rpi4-image/releases/tag/24.04.2_v6.8.4-rt11-raspi_ros2_jazzy), on the box's Raspberry Pi 5 | [Setting up the software](https://docs.source-robotics.com/par6/latest/rcb/05-software-setup/) |
| Runtime | the manual describes [RCB-Runtime v1.1.1](https://github.com/Source-Robotics/RCB-Runtime/releases/tag/v1.1.1), for PCB v1 and v2 | [Downloads and files](https://docs.source-robotics.com/par6/latest/par6/cad-files/) |
| Manual | Robot Control Box Manual SR-2001, revision 2026.A, a draft; for control box PCB v1 and v2 | [the manual](https://docs.source-robotics.com/par6/latest/rcb/) |

## Which version do I have?

| | Where to see it |
|---|---|
| Mainboard PCB | the label inside the box; Dashboard → System Info. The board does not report its version: the runtime takes it from `<pcb_version>` in `config/system.xml`, so keep that number right, or the runtime reads the E-stops with the wrong policy |
| Beta mainboard | it carries the fixes the [PAR6 beta guide](https://github.com/Source-Robotics/PAR6-Collaborative-Robot-Arm/blob/main/beta_guide.md) lists (K1, Q1, D11 and D12, the E-stop connectors, R63, the trace cuts) |
| Runtime | Dashboard → System Info → Versions |

## What works together

The runtime, the Python client, the drive firmware, the control box PCB, the arm and grippers, the models and the
operating system image of each runtime release:
[Versions and compatibility](https://docs.source-robotics.com/par6/latest/support/versions/) in the manual.
