# Hardware

PCB, gerbers, BOM, and assembly notes for the Mega + 12× A4988 + WS2812 sculpture.

| Folder | Put |
| --- | --- |
| `pcb/` | KiCad / Eagle source |
| `gerbers/` | Fab zip |
| `photos/` | Board and install shots |

## Drivers (as built)

- A4988 modules with heatsink, **24 V**, current near **2 A** (chip max — they run hot)
- Microstep **1/8** strapped on copper (MS1=H, MS2=H, MS3=L). Firmware `MICROSTEPS` must match.
- Motors: [StepperOnline 17HS19-2004S1](https://www.omc-stepperonline.com/nema-17-bipolar-59ncm-84oz-in-2a-42x48mm-4-wires-w-1m-cable-connector-17hs19-2004s1) NEMA 17, 1.8°, 2 A
