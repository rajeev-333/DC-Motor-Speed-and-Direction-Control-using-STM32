# DC Motor Speed and Direction Control using STM32

## Project Overview

This project demonstrates **DC motor speed and direction control using PWM** with the **STM32 NUCLEO-F446RE microcontroller** and an **L298N motor driver**.

Motor speed is controlled using a **potentiometer**, whose analog voltage is read by the **STM32 ADC**. The ADC value is converted into a **PWM duty cycle**, which drives the motor through the L298N driver.

This project illustrates fundamental **embedded systems concepts** such as:

* Analog-to-Digital Conversion (ADC)
* Pulse Width Modulation (PWM)
* Timer peripherals
* GPIO-based motor direction control
* Interfacing external drivers with microcontrollers

---

# Hardware Used

| Component             | Description                         |
| --------------------- | ----------------------------------- |
| STM32 NUCLEO-F446RE   | ARM Cortex-M4 microcontroller board |
| L298N Motor Driver    | H-Bridge motor driver               |
| DC Motor              | 12V brushed DC motor                |
| Potentiometer         | Used for speed control              |
| External Power Supply | 9–12V for motor                     |
| Jumper Wires          | Hardware connections                |

---

# System Architecture

```
User rotates potentiometer
        ↓
Analog voltage (0–3.3V)
        ↓
STM32 ADC reads voltage
        ↓
ADC value mapped to PWM duty cycle
        ↓
Timer generates PWM signal
        ↓
PWM drives L298N Enable pin
        ↓
GPIO pins set motor direction
        ↓
Motor speed and direction change
```

---

# Pin Configuration

| STM32 Pin | Peripheral  | Connected To         |
| --------- | ----------- | -------------------- |
| PA0       | ADC1_IN0    | Potentiometer Output |
| PA6       | TIM3_CH1    | L298N ENA (PWM)      |
| PB0       | GPIO Output | L298N IN1            |
| PB1       | GPIO Output | L298N IN2            |
| GND       | Ground      | Common Ground        |

---

# Working Principle

## 1. Speed Control

The potentiometer generates an **analog voltage between 0V and 3.3V**.

The STM32 ADC converts this voltage into a **12-bit digital value**:

```
ADC Range = 0 → 4095
```

This value is scaled to the **PWM timer range**:

```
PWM = (ADC_value × Timer_Period) / 4095
```

The PWM duty cycle controls the **average voltage applied to the motor**, which determines its speed.

---

## 2. Direction Control

The L298N motor driver uses two input pins to determine the direction.

| IN1 | IN2 | Motor Direction |
| --- | --- | --------------- |
| 1   | 0   | Forward         |
| 0   | 1   | Reverse         |
| 0   | 0   | Stop            |

The STM32 GPIO pins **PB0 and PB1** control these signals.

---

# PWM Concept

PWM controls motor speed by varying the **duty cycle** of a digital signal.

```
Duty Cycle = TON / (TON + TOFF)
```

Example:

| Duty Cycle | Motor Speed   |
| ---------- | ------------- |
| 0%         | Motor OFF     |
| 25%        | Low speed     |
| 50%        | Medium speed  |
| 75%        | High speed    |
| 100%       | Maximum speed |

---


---

# Hardware Setup

1. Connect **Potentiometer Output → PA0**
2. Connect **L298N ENA → PA6 (PWM)**
3. Connect **L298N IN1 → PB0**
4. Connect **L298N IN2 → PB1**
5. Connect **Motor to L298N Output**
6. Provide **12V external supply to motor driver**
7. Ensure **common ground between STM32 and motor driver**


---


# Project Features

✔ Potentiometer-based speed control
✔ PWM motor speed modulation
✔ Forward / Reverse motor control
✔ STM32 HAL driver implementation
✔ Simple and modular firmware

---

# Repository Structure

```
STM32-DC-Motor-Control
│
├── README.md
├── main.c
└── hardware_setup.jpg
```

---

# Applications

* Robotics
* Electric vehicle control systems
* Industrial automation
* Embedded motor control systems
* PWM-based actuator control


