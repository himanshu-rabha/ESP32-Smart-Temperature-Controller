# Troubleshooting

## ESP32 Upload Error

Problem:

Failed to connect to ESP32

Solution:

Hold BOOT button during upload.

---

## Relay Not Switching

Problem:

ESP32 GPIO could not drive relay directly.

Solution:

Added 2N2222 transistor driver stage.

---

## Temperature Overshoot

Problem:

500W heater rapidly heated 150ml water.

Solution:

Increase water volume and improve sensor placement.
