#!/bin/bash

working_dir=$(pwd)
echo "Attempting to build the project"
cd "$working_dir/build" || {
  echo "No build directory found ... exiting"
  exit
}

if cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..; then
  if make; then
    echo "Project built successfully"
  else
    echo "Make failed, check the configuration and logs"
    exit
  fi
else
  echo "Cmake failed, check the configuration and logs"
  exit
fi

# Find the device with the specified disk name
echo "Searching for a connected Pico"
device=$(lsblk -o NAME,MODEL | grep "RP2" | awk '{print $1}')

# Exit if no RPico is found
if [ -z "$device" ]; then
  echo "Device RP2 not found"
  exit 1
fi

echo "Pico found:"
sudo mount /dev/"$device"1 /home/travis/rpi-pico
echo "Device $device mounted at /home/travis/rpi-pico/"

while true; do
  echo "Flashing Master(1) or Slave(2)? (q to quit)"
  read -r pico
  if [ "$pico" = "1" ]; then
    echo 'Flashing Master to connected Pico'
    sudo cp ./src/master.uf2 /home/travis/rpi-pico/
    break
  elif [ "$pico" = "2" ]; then
    echo "Flashing Slave to connected Pico"
    sudo cp ./src/slave.uf2 /home/travis/rpi-pico/
    break
  elif [ "$pico" = "q" ]; then
    echo "Exiting"
    exit
  else
    echo "Incorrect choice. Try again."
    sleep 1
  fi
done

echo "Completed mounting and flashing Pico"
