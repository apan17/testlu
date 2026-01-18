# Smart Ventilation & Smoke Detection System

IoT-based intelligent ventilation system using **ESP32-S3**, **MQTT**, and **Supabase Cloud** hosted on **Google Cloud Platform (GCP)**.

## Project Overview
This project implements a real-time smart ventilation system designed for fire safety and air quality monitoring. It collects environmental data (temperature and smoke levels) via an ESP32-S3 node and transmits it to a cloud-based database through a secure Python middleware for storage and real-time visualization.

---
## CPC357 - IoT Architecture and Smart Applications

**Assignment 2** **For the attention of:** Dr. Mohd Nadhir Ab Wahab.  

**Group Members:**
- [YOUR NAME]
- [PARTNER'S NAME]
---

### System Components

**Hardware:**
- **Microcontroller:** Cytron Maker Feather AIOT S3 (ESP32-S3)
- **Sensors:** - MQ-2 Gas/Smoke Sensor
  - DHT11 Temperature Sensor
- **Actuators:**
  - 12V DC Exhaust Fan
  - Relay Module (5V)
  - I2C LCD Display (16x2)

**Software & Cloud:**
- **Google Cloud Platform (GCP):** Compute Engine (Ubuntu 24.04 VM)
- **MQTT Broker:** Mosquitto
- **Middleware:** Python 3.x Bridge Service (`mqtt_to_supabase.py`)
- **Database:** Supabase (PostgreSQL Cloud API)
- **Dashboard:** Custom Web Dashboard (HTML/CSS/JS/Supabase-js)

## System Architecture

```text
┌─────────────────┐
│   ESP32-S3      │
│   - MQ-2 Sensor │
│   - DHT11       │
│   - LCD Display │
│   - Relay & Fan │
└────────┬────────┘
         │ WiFi/MQTT (Port 1883)
         ↓
┌─────────────────────────────────┐
│        GCP VM Instance          │
│  ┌───────────┐   ┌────────────┐ │
│  │ Mosquitto │   │ Python     │ │
│  │   MQTT    │──>│ Middleware │ │
│  └───────────┘   │ (Bridge)   │ │
└────────────────────────┬────────┘
                         │ HTTPS/API
                         ↓
┌─────────────────────────────────┐
│         Supabase Cloud          │
│  ┌───────────┐   ┌────────────┐ │
│  │ Database  │   │   Web      │ │
│  │ (Postgres)│<──│ Dashboard  │ │
│  └───────────┘   └────────────┘ │
└─────────────────────────────────┘
