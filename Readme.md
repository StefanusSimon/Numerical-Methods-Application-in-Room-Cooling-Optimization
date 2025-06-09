# ❄️ Room Cooling Optimization Using Numerical Methods

This project demonstrates the use of three essential **numerical methods**—**Gauss-Seidel**, **Golden Section Search**, and **Runge-Kutta 4th Order**—to optimize indoor cooling in a room using real temperature sensor data. It models wall heat distribution, determines the optimal air conditioning (AC) temperature for energy efficiency, and simulates how a room cools down over time.

---

## 📘 Overview

Modern buildings rely on smart HVAC systems to balance **thermal comfort** and **energy efficiency**. This project simulates that process using real sensor data and applies numerical analysis to:

1. **Estimate wall temperatures**
2. **Find the energy-optimal cooling setpoint**
3. **Simulate cooling over time**

This program is written in **C++** and reads temperature data from a CSV file.

---

## 🔢 Numerical Methods

### 1. Gauss-Seidel Iteration
Used to approximate **steady-state wall temperatures** by solving a linear system derived from discretized heat conduction equations.

### 2. Golden Section Search
Used to find the **optimal AC temperature** that minimizes the following energy consumption cost function:
E(T) = (T - 22)² + 5

### 3. Runge-Kutta 4th Order (RK4)
Simulates **time-based cooling** of the room using Newton's Law of Cooling:
dT/dt = -k(T - T_env)
where `T_env` is the optimal AC temperature and `k` is the cooling coefficient.

---

## 📊 Dataset

- Source: [Kaggle - Room Occupancy Detection Data (IoT Sensor)](https://www.kaggle.com/datasets)
- Required column: `"Temperature"`

## Output

### === Proyek UAS: Optimasi Pendinginan Ruangan ===

#### Estimasi Suhu Dinding Ruangan (Rata-Rata Antar Sensor)

| Node | Suhu (°C) |
|------|-----------|
| 1    | 23.709    |
| 2    | 23.718    |
| 3    | 23.724    |

#### Suhu Optimal AC untuk Konsumsi Energi Minimum

| Suhu Optimal (°C) |
|-------------------|
| 22                |

#### Simulasi Pendinginan Ruangan (Runge-Kutta Orde 4)

| Waktu (s) | Suhu (°C) |
|-----------|-----------|
| 0         | 21.0000   |
| 10        | 21.3932   |
| 20        | 21.6318   |
| 30        | 21.7766   |
| 40        | 21.8644   |
| 50        | 21.9177   |
| 60        | 21.9501   |
| 70        | 21.9697   |
| 80        | 21.9816   |
| 90        | 21.9888   |
| 100       | 21.9932   |



