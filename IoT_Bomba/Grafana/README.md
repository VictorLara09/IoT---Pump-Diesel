# Accessing Grafana and Configuring Data Sources.

## Accessing Grafana.
Open your web browser and navigate to the following URL:

```bash
    http://localhost:3000
```
In this example, `localhost` refers to the local IP address of your machine. Typically, you would use the local IP address and port specified in **Grafana's Docker Compose configuration**.

Upon your first access to Grafana, you will be prompted to enter the `username` and `password`. The initial login screen requires you to change the initial password as a mandatory security measure. After setting the new password, you will be able to log in using this `updated password`.

The screen will appear as follows:
![alt text](<../Images/Grafana Server.jpeg>)

## Configuring Data Source.

Once logged in, navigate to the **Add your first data source** box to begin the initial setup for creating your dashboard.

In the Data source window, select the database service you wish to use. For this setup, choose [InfluxDB](https://www.influxdata.com/).

After selecting InfluxDB, a configuration window will appear as shown be:

![alt text](<../Images/InfluxDB in Grafana.jpeg>)

Proceed to configure the **HTTP** option:

![alt text](<../Images/Local IP.jpeg>)

Define the IP address of the network you are connected to and set the port of the database as specified in the `Telegraf's Docker Compose file`.

Next, go to `InfluxDB Details`. Enter the name of your database, username, and password, all of which are defined in the `Telegraf's Docker Compose file`.

![alt text](<../Images/User and Password - InfluxDB.jpeg>)

Click on **Save and Test**. You should see a confirmation message indicating that the database is working:

![alt text](<../Images/Validation.png>)

## Creating Your First Dashboard

Go to the `Home` page. To create your first dashboard, click on the Create your `first dashboard box`.

## Creating Queries in Grafana.

Next, we will create our **Queries**.

1) In the `FROM` section, under `Select Measurement`, choose the tag for the entries from the sensor reading, which in this case is `mqtt_consumer`. In the `Where` section, select `topic::tag`. The created tag(s) will then appear; for this example, choose Sensor/Voltage. Finally, the created tag(s) will appear. In this case, `Sensor/pressure`.
2) In the Select section, choose field. The data from the sensor, such as `pressure_in`, `pressure_out`, or other variables, will be displayed. Select one of these fields, then remove the `mean()` section. Add the `bottom` option in `Selectors` for accurate data visualization.
3) Follow the same configuration steps for your other variables and topics. For example:
    * For `pressure_in` and `pressure_out`, use the topic `"sensor/pressure"`.
    * For `flowRate` and `totalLitres`, use the topic `"sensor/waterflow"`.
    * For `fuelLevel`, use the topic `"sensor/fuellevel"`.
    * For `powerHydraulicW` and `powerElectricW`, use the topic `"sensor/power"`.
    * For `efficiency`, use the topic `"sensor/efficiency"`.

![alt text](<../Images/Query.jpeg>)

## Configuring Data Filters in Grafana
To set up a data filter in Grafana, follow these detailed steps. This filter enables the inclusion or exclusion of data based on specific conditions that you define.
To configure a data filter in Grafana, follow these streamlined steps:

1) **Navigate to the Filter Option**:

    Go to the Transform data tab and select Filter data by values.

2) **Select the Field**:

    Choose the field you wish to filter, such as mqtt_consumer.bottom.

3) **Set Filter Type and Condition**:

    Choose Include or Exclude depending on your needs.
    Set the condition, such as Is greater or equal to 0.
4) **Save and Apply**:

    Confirm your settings and apply the filter to display the relevant data.

![alt text](<../Images/Data Filter.jpeg>)

## Dashboard Visualization.
Finally, based on the dashboard configuration for your application, you should visualize something like the following:


![alt text](<../Images/FinalDashboard.jpeg>)