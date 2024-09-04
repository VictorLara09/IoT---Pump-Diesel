# Diesel Pump IoT Project
![Project Overview](<IoT_Pump/Images/FinalDashboard.jpeg>)
## Project Description

The project involves a monitoring and control system for diesel pumps, utilizing IoT technologies to improve operational efficiency and ensure optimal performance.

## Project Components

- **Sensors**: Used to measure various parameters of the system, such as pressure, fuel level, flow rate, and more.
- **Data Transmitters**: Send data from sensors to the monitoring platform.
- **Monitoring Platform**: Includes services like MQTT, InfluxDB, Telegraf, and Grafana for data visualization.

## System Configuration

1. **Sensor Configuration**
   - **Pressure Sensors**: To measure pressure at different points in the system.
   - **Fuel Level Sensors**: To monitor the fuel level in the pump.
   - **Flow Sensors**: To measure the flow rate of the liquid in the system.

2. **Data Services**
   - **MQTT**: For real-time data transmission.
   - **InfluxDB**: Database for storing sensor data.
   - **Telegraf**: Collects and sends data to InfluxDB.
   - **Grafana**: For visualizing the collected data.

## Grafana Configuration

1. **Creating the Dashboard**
   - Go to the **Home** section and select **Create your first dashboard**.
   - Add panels to visualize sensor data.

2. **Configuring Queries**
   - **FROM**: Select the corresponding tag (e.g., `mqtt_consumer`).
   - **WHERE**: Select the sensor topic (e.g., `sensor/pressure`).
   - **SELECT**: Choose the data field (e.g., `pressure_in`, `pressure_out`, `flowRate`, etc.). Remove the `mean()` function and add the `bottom` option in `Selectors` for correct data visualization.

   **Example Topics and Fields:**
   - `sensor/waterflow` - Field: `flowRate`
   - `sensor/fuellevel` - Field: `fuelLevel`
   - `sensor/pressure` - Fields: `pressure_in`, `pressure_out`
   - `sensor/power` - Fields: `powerHydraulicW`, `powerElectricW`
   - `sensor/efficiency` - Field: `efficiency`

## Contact

- **Victor Lara**
  - Email: [victor962000@hotmail.com](mailto:victor962000@hotmail.com)
  - GitHub: [VictorLara09](https://github.com/VictorLara09)
  - LinkedIn: [Victor Lara](https://www.linkedin.com/in/victor-lara0906)

- **Anthony Lopez**
  - Email: [zzianthony532@gmail.com](mailto:zzianthony532@gmail.com)
  - GitHub: [andalope](https://github.com/andalope)
