#Open TEC

Open Source Project (software and hardware) to get GPS data and calculate TEC ( Total Electron Content ) to be used in predicting the earthquake.

### This is how it works:

##### 1. Get Raw GPS Data from Arduino + Itead Arduino GPS Shield in SiRF binary 
##### 2. Convert SiRF binary to RINEX using 'sirfdump' 
##### 3. Calculate TEC using 'gpstk'