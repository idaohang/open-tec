open-tec
========

Open Source Project (software and hardware) to get GPS data and calculate the TEC to be used in predicting the earthquake.

This is how it works:

Get Raw GPS Data from Arduino + Itead Arduino GPS Shield in Sirf III binary --> Convert Sirf III binary to RINEX using 'sirfdump' ---> Calculate TEC using 'gpstk'