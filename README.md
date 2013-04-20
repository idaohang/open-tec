#Open TEC

Open Source Project (software and hardware) to get GPS data and calculate TEC ( Total Electron Content ) to be used in predicting the earthquake.

### Inspired by Prof. Kosuke Heki of Hokkaido University

<a href="http://www.youtube.com/watch?feature=player_embedded&v=yAVxsvE5ceM" target="_blank"><img src="http://img.youtube.com/vi/yAVxsvE5ceM/0.jpg" 
alt="Earthquake Predictor using GPS" width="640" height="360" border="10" /></a>

Professor Kosuke Heki of Hokkaido University in Japan believes he has found a way to predict earthquakes.

Heki analyses GPS signals by measuring the TEC, or Total Electron Content, in the upper atmosphere. Whilst measuring how the TEC was disrupted by sound waves after the Tohoku earthquake of 2011, he discovered - quite by accident - that the TEC was also disrupted in the 40 or so minutes before it.

Going back through GPS records he has found similar correlations for other major earthquakes, a discovery that is being heralded as a major breakthrough in our understanding of earthquake phenomena.

### Paper

<a href="http://www.ep.sci.hokudai.ac.jp/~heki/pdf/Heki_GRL2011.pdf" target="_blank">Heki, K., Ionospheric electron enhancement preceding the 2011 Tohoku-Oki earthquake, Geophys. Res. Lett. 38, L17312, doi:10.1029/2011GL047908, 2011</a>

### Open TEC implementation:

```
1. Get Raw GPS Data from Arduino + Itead Arduino GPS Shield in SiRF binary 
2. Convert SiRF binary to RINEX using 'sirfdump' 
3. Calculate TEC using 'gpstk'
```

### Installation

##### sirfdump

```
make
sudo make install
```

##### gpstk

System Requirements: (<a href='http://www.gpstk.org/bin/view/Documentation/SystemRequirements' target='_blank'>link</a>)

```
tar xvzf gpstk.tar.gz
cd gpstk/dev
./autogen.sh
./configure
make
sudo make install
```

##### GPS logger

```
Copy and Paste gps_logger.pde into Arduino IDE and upload it to the board to see the result 
```


### Misc 
Global TEC Map Provided by Trimble (updated every 5 minutes)
<img src='http://www.trimbleionoinfo.com/Images.svc/TEC' width='640'>