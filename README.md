# LabWork 1
## Author
Никитенко Тимур Романович, группа 24.Б81-мм
## Contacts
st128375@student.spbu.ru
## Description
First lab work for loading and processing bitmap image

Also compares serial vs parallel implementations of image operations.
## Build 
make serial   # Build serial version
make parallel # Build parallel version
## Run
./image_processor_serial # Run serial version
./image_processor_parallel  # Run parallel version
## Perfomance comparison
make benchmark # Build and run both version comparing their perfomance
### Requirements
- GCC (with OpenMP support)
- ImageMagick (for PNG conversion)

