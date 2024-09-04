 # IoT: Diesel Pump

## Docker

**Starting and managing Containers**

Executing the specified command will initialize four containers on your server, with Mosquitto being one of them. To verify that these containers are running as expected, you can use the following command:

```bash
    sudo docker ps
```
**Note: If you encounter permission issues, prepend sudo to the commands.**

This command will list all running containers.The output should show all containers operational continuously without any restarts, as illustrated in the image below:

![alt text](<../Images/Docker.jpeg>)

If multiple containers are active, they should appear continuously operational without any restarts. Should you observe any container restarting unexpectedly, you can investigate further using docker logs <container-name> to review its logs. Alternatively, you can access a shell within the container by executing:

``` bash
    sudo docker exec -it <container-name> sh
```

