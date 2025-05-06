# Club monitoring program
This application allows ypu to parse computer club'logs and show any error and income at the and of the day

## Usage
Program requires one cli argument -- text file of logs
```bash
./club_monitor log_file.txt
```

## Build
```bash
mkdir build
cd build
cmake ..
make
cp ../example.txt bin
./bin/club_monitor example.txt
```