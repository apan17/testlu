I understand completely. I apologize for the previous confusion.

Here is the **FULL, COMPLETE README.md** source code.

You do not need to format anything yourself. Just **COPY** the code block below, **PASTE** it into your `README.md` file, and save it. It includes all the tables, bullet points, and code blocks exactly as they should appear.

```markdown
# Smart Ventilation & Smoke Detection System

**IoT-based intelligent ventilation system using ESP32-S3, MQTT, and Supabase Cloud hosted on Google Cloud Platform (GCP).**

![System Dashboard](Screenshot%202026-01-18%20224709.png)

## Project Overview
This project implements a real-time smart ventilation system designed for fire safety and air quality monitoring. It collects environmental data (temperature and smoke levels) via an ESP32-S3 node and transmits it to a cloud-based database through a secure Python middleware for storage and real-time visualization.

---
## CPC357 - IoT Architecture and Smart Applications

**Assignment 2**
**For the attention of:** Dr. Mohd Nadhir Ab Wahab.

**Group Members:**
- [YOUR NAME] (Matric ID)
- [PARTNER'S NAME] (Matric ID)
---

## System Components

### Hardware
* **Microcontroller:** Cytron Maker Feather AIOT S3 (ESP32-S3)
* **Sensors:**
    * MQ-2 Gas/Smoke Sensor
    * DHT11 Temperature Sensor
* **Actuators:**
    * 12V DC Exhaust Fan
    * Relay Module (5V)
    * I2C LCD Display (16x2)

### Software & Cloud
* **Google Cloud Platform (GCP):** Compute Engine (Ubuntu 24.04 VM)
* **MQTT Broker:** Mosquitto
* **Middleware:** Python 3.x Bridge Service (`mqtt_to_supabase.py`)
* **Database:** Supabase (PostgreSQL Cloud API)
* **Dashboard:** Custom Web Dashboard (HTML/CSS/JS/Supabase-js)

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

```

## Features

* **Automated Smoke Extraction:** The exhaust fan activates automatically via the relay when smoke levels exceed predefined safety thresholds.
* **Local Monitoring:** Real-time feedback is provided on a local LCD for immediate temperature and smoke status.
* **Data Normalization:** The Python middleware translates raw sensor keys (e.g., `gas`, `temp`) into standardized database column names (e.g., `smoke_level`, `temperature`).
* **Cloud-Native Storage:** Secure, persistent logging in Supabase with automatic server-side timestamps.
* **Interactive Dashboard:** Live gauges and synchronized line charts for environmental trend analysis.

## 🔧 Hardware Setup

### Pin Connections

| Component | ESP32-S3 Pin | Notes |
| --- | --- | --- |
| **MQ-2 VCC** | 5V | Power supply |
| **MQ-2 GND** | GND | Ground |
| **MQ-2 A0** | GPIO 1 | Analog Input for gas levels |
| **DHT11 DATA** | GPIO 4 | Digital data signal |
| **Relay IN** | GPIO 39 | Control signal for fan |
| **LCD SDA** | GPIO 8 | I2C Data |
| **LCD SCL** | GPIO 9 | I2C Clock |

## Software Setup

### 1. GCP VM Configuration (Manual Installation)

Run the following commands on your Ubuntu VM to set up the environment:

```bash
# Update system and install Mosquitto MQTT Broker
sudo apt update && sudo apt upgrade -y
sudo apt install -y mosquitto mosquitto-clients

# Install necessary Python libraries
pip3 install supabase paho-mqtt --break-system-packages

```

### 2. Middleware Deployment

1. Create the bridge script:
```bash
nano mqtt_to_supabase.py

```


2. Paste your Python code (ensure you update `SUPABASE_URL` and `SUPABASE_KEY`).
3. Test the script manually:
```bash
python3 mqtt_to_supabase.py

```



### 3. Setup as System Service

To ensure the middleware runs 24/7 (even after reboot):

```bash
# Create service file
sudo nano /etc/systemd/system/mqtt-bridge.service

# Enable and start service
sudo systemctl daemon-reload
sudo systemctl enable mqtt-bridge
sudo systemctl start mqtt-bridge

```

## Security Considerations

1. **VPC Firewall:** Ingress traffic is strictly limited to **TCP Port 1883** to protect the VM host.
2. **Credential Isolation:** The ESP32 node does not hold database keys; it only interacts with the local MQTT broker to prevent credential leakage.
3. **Encrypted Transport:** Data transmission from the GCP middleware to Supabase uses **HTTPS/TLS** encryption.
4. **Row Level Security (RLS):** Supabase RLS is utilized to manage data access and protect the backend table.

## Data Format

### MQTT Payload (From ESP32)

```json
{
  "temp": 26.7,
  "gas": 0.66,
  "fan": "OFF",
  "status": "SAFE"
}

```

### Supabase Table Structure (`readings`)

| Column | Type | Description |
| --- | --- | --- |
| `temperature` | float8 | Room temperature in °C |
| `smoke_level` | float8 | Measured smoke concentration |
| `fan_status` | text | Current state of the fan (ON/OFF) |
| `created_at` | timestamp | Automatic server-side timestamp |

## Visualization

The custom dashboard provides a **Live Overview** of system metrics and historical line graphs to study environmental patterns over time. It also includes a **Recent Activity** log for tracking the five most recent sensor updates.

```

```
