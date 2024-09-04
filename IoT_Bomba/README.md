 # IoT: Diesel Pump 

## Implementation of Docker and Docker-Compose in a Server Environment.

In the process of configuring containerization tools within a server environment, it is essential first to ascertain the presence of  [docker](https://docs.docker.com/) and [docker-compose](https://docs.docker.com/compose/). This can be verified by executing the commands docker and docker-compose in the terminal. Should these tools not be installed, their installation can be performed with relative ease.

For systems running on Windows or Mac OS X, the most straightforward approach is to utilize [Docker for Desktop](https://www.docker.com/products/docker-desktop/), which provides a comprehensive solution for managing Docker environments. As an alternative for Mac OS X users, installation can be facilitated via [Homebrew](https://brew.sh/) by executing the command `brew install docker docker-compose` \.

In the case of Linux-based systems, particularly those utilizing (**Ubuntu or other Debian-based**) distributions, the tools can be installed using the package manager with the following commands:

```bash
   sudo apt install docker.io
   sudo apt install docker-compose
```
For installations on other operating systems or for configurations requiring specific setups, it is advisable to consult the [official Docker documentation](https://docs.docker.com/get-docker/). to ensure proper installation and configuration procedures are followed.


## Executing a Shell Script for Project Configuration.

To begin, open a terminal and navigate to the directory containing the project files. It is necessary to grant execution permissions to the script file `auto-compose`.sh by using the following command:

```bash
   sudo chmod +x auto-compose.sh  
```

Once the permissions have been adjusted, the script can be executed by entering the specified script name:

```bash
   ./auto-compose.sh  - <Options>
```

Subsequently, to display the help menu and verify available options, execute the following command:

```bash
   ./auto-compose.sh  -h
```

This command will load the main menu, showcasing the predefined configurations set within the script. The interface should resemble the following:

![alt text](<Images/MainMenuScript.jpeg>)


In the following section, each option presented in the image above will be detailed comprehensively.


## Starting the Services.

To initiate the program, the first step involves executing the following command in the terminal: `auto-compose.sh -a`. Upon successful execution, the terminal output should resemble the following:


![alt text](<Images/StartingServices1.0.jpeg>)
![alt text](<Images/StartingServices2.0.jpeg>)

## Stopping/Removing the Services.

To halt and remove the running services, the following command should be entered into the terminal: `auto-compose.sh -d`. The terminal output should reflect the process, as illustrated below:

![alt text](<Images/StoppingServices.jpeg>)

## Removing the Service Images.

In order to remove the Docker images associated with each service, the command below must be executed: `auto-compose.sh -r`. Upon completion, the output should resemble the following:


![alt text](<Images/RemovingServices.jpeg>)