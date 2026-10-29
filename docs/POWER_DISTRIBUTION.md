# Power Distribution

No power schematic, rail voltages, supply current ratings, regulator selection, grounding strategy, fuse, protection, connector pinout or power sequencing is included in the repository. These details cannot be inferred safely from the firmware.

The servo library configuration and GPIO assignments do not establish that the board can power the servos directly. Verify the exact board/peripheral datasheets, logic-level compatibility, supply capacity, common-ground requirements, transient handling and mechanical load with the hardware owner before connecting or operating the system.

This page records an evidence gap; it is not a wiring or power-design recommendation.
