# Running Ansible on the Raspberry Pi 4 8gb

```bash
# Get Python3 installed
sudo apt install python3

# Install Arduino
# Go to https://www.arduino.cc/en/software
# Get a 32 bit ARM version because currently Raspian only works on 32 bit
# (You can confirm this by running `uname -m`. "armv7l" is 32 bit.)
cd ~
wget https://downloads.arduino.cc/arduino-1.8.13-linuxarm.tar.xz
tar -xvf arduino-1.8.13-linuxarm.tar.xz
sudo arduino-1.8.13
sudo ./install.sh
mv arduino-1.8.13-linuxarm.tar.xz ~/Downloads

# Download the right version from: https://www.pjrc.com/teensy/td_download.html
# It should prompt you to install the udev rules you need.
cd ~/Downloads
wget https://www.pjrc.com/teensy/td_153/TeensyduinoInstall.linuxarm
chmod 755 TeensyduinoInstall.linuxarm
./TeensyduinoInstall.linuxarm

# Use pip to install Ansible
python3 -m pip install --user ansible
python3 -m pip install "ansible-lint[community,yamllint]" 

# You won't need to do this after running ansible, since the playbook
# will add a bashrc extras file that will put it into the path in the future
export PATH=${PATH}:${HOME}/.local/bin
# Install prerequisites for the playbook
ansible-galaxy collection install community.general
# Run the playbook
ansible-playbook --inventory inventory.yml pvo-rpi.yml

# Get the most recent version of JOrgan for raspberry pi at:
# https://sourceforge.net/projects/jorgan/files/jorgan-rpi/
cd ~/Downloads
# This will also install the jackd prerequisites
sudo apt install ./jorgan_4.0.Beta2_armhf.deb
```

## Downloading Dispositions

Many dispositions are here: <https://jorgan.info/base/Shared_Dispositions.html>

```bash
# To extract a 7z file if needed
# (Don't use `7z e`, since that won't preserve the folder hierarchy)
7z x file.7z
```

## Helpful Sites

* <https://sites.google.com/site/savirtualorgans/jorganpi>
* <http://home.exetel.com.au/reimerorgans/ART/RPi_TUTORIAL.pdf>