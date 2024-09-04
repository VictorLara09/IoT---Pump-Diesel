#!/bin/bash

# Directorio principal donde se encuentran las subcarpetas
SCRIPT=$(readlink -f $0);
main_directory=`dirname $SCRIPT`;
echo "El script se encuentra en $main_directory"

# Función para ejecutar la funcion de ayuda
function print_help() {
    cat <<EOF
REQUIRED:
 -d 	   Detiene los servicios(docker-compose down) 
 -h        Menú de ayuda	
 -r	   Remueve las imágenes
 -a 	   Levanta los servicios (docker-compose up -d) 
EOF
    exit 2
}

# Función para ejecutar docker-compose en un directorio
function run_docker_compose() {
    local compose_file="$1/docker-compose.yml"
    if [ -f "$compose_file" ]; then
        echo "Ejecutando docker-compose en $1"
        (cd "$1" && sudo docker-compose up -d)
    fi
}

# Función para recorrer recursivamente las subcarpetas
function process_directory() {
    local dir="$1"
    for item in "$dir"/*; do
        if [ -d "$item" ]; then
            run_docker_compose "$item"
            process_directory "$item"
        fi
    done
}

# Función para recorrer recursivamente las subcarpetas
function process_directory1() {
    local dir="$1"
    for item in "$dir"/*; do
        if [ -d "$item" ]; then
            down_docker_compose "$item"
            process_directory "$item"
        fi
    done
}

# Función para ejecutar docker-compose down en un directorio
function down_docker_compose() {
    local compose_file="$1/docker-compose.yml"
    if [ -f "$compose_file" ]; then
        echo "Ejecutando docker-compose down en $1"
        (cd "$1" && sudo docker-compose down)
    fi
}

# Función para eliminar imágenes de docker
function deleted_image(){
         (sudo docker rmi eclipse-mosquitto)
         (sudo docker rmi telegraf)
         (sudo docker rmi influxdb:1.8-alpine)
         (sudo docker rmi grafana/grafana)	
}


# Procesar los argumentos
while getopts ":du:hu:vu:au:ru:pu:" OPTIONS; 
do
    case $OPTIONS in
        d)
            process_directory1 "$main_directory"
            ;;
        a)
            process_directory "$main_directory" 
            ;;
        r)
            deleted_image
            ;;    
 
        h)
            print_help
            ;;
            
        \?)
            echo "Opción inválida: -$OPTARG" >&2
            print_help
            ;;
    esac
done




