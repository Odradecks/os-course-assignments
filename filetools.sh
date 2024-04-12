#!/bin/bash

menu() {
    echo "======= file tools  ======="
    echo "0. exit"
    echo "1. create file"
    echo "2. write file"
    echo "3. read file"
    echo "4. change permission"
    echo "5. view the current file's permission and exit"
    echo "==========================="
}

create_file() {
    echo "Please enter the name of the file to be created: "
    read filename
    touch $filename
    echo "$filename has been successfully created."
}

write_file() {
    echo "Please enter the name of the file to be written: "
    read filename
    echo "Please input the content: "
    read content
    echo $content > $filename
    echo "$filename has been successfully written."
}

read_file() {
    echo "Please enter the name of the file to be written: "
    read filename
    echo "The content of file $filename is: "
    cat $filename
}

change_permission() {
    echo "Please enter the name of the file whose permission is to be changed: "
    read filename
    echo "Please input the new permission (such as: 755): "
    read permission
    chmod $permission $filename
    echo "The permission of $filename has been successfully changed."
}

view_permission_and_exit() {
    echo "Please enter the name of the file which you want to view its permission: "
    read filename
    ls -l $filename
    exit
}

while true; do
    menu
    echo "Please enter the number [0-5]: "
    read choice
    case $choice in
        0) exit ;;
        1) create_file ;;
        2) write_file ;;
        3) read_file ;;
        4) change_permission ;;
        5) view_permission_and_exit ;;
        *) echo "Invalid input! Please enter again: " ;;
    esac
done

