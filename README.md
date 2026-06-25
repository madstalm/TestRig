# TestRig
Codebase and documentation for the test rig.

## Getting the GUI running on the Raspberry Pi:
1. Open the terminal on the Raspberry Pi and enter:
```
cd ~/Documents/TestStation/build
```
3. From there run:
```
./TestRigControl
```

That will open the GUI.
Currently we have a small bug where the GUI does not enable the motors by default, so you will have to do that manually. 

1. Open another terminal and enter:
```
telnet 192.168.42.100 23
```
3. Then type `SHA` and press enter
4. To leave telnet press Ctrl+] then type `quit`
