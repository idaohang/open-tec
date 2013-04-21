#Open TEC

Open Source Project (software and hardware) to get GPS data and calculate TEC ( Total Electron Content ) to be used in predicting the earthquake.

### Inspired by Prof. Kosuke Heki of Hokkaido University

<a href="http://www.youtube.com/watch?feature=player_embedded&v=yAVxsvE5ceM" target="_blank"><img src="https://dl.dropboxusercontent.com/u/79120458/kosuke-heki.jpg" 
alt="Earthquake Predictor using GPS" width="640" height="360" border="10" /></a>

Professor Kosuke Heki of Hokkaido University in Japan believes he has found a way to predict earthquakes.

Heki analyses GPS signals by measuring the TEC, or Total Electron Content, in the upper atmosphere. Whilst measuring how the TEC was disrupted by sound waves after the Tohoku earthquake of 2011, he discovered - quite by accident - that the TEC was also disrupted in the 40 or so minutes before it.

Going back through GPS records he has found similar correlations for other major earthquakes, a discovery that is being heralded as a major breakthrough in our understanding of earthquake phenomena.

##### His Abtract:

The Japanese dense network of Global Positioning System (GPS) detected preseismic enhancement of ionospheric total electron content (TEC) over the region within 200-300 km from the focal region. It started ~60-40 minutes before the 2011 March 11 NE Japan earthquake (Mw 9.0) , and reached ~8 % of the background TEC. It lasted until atmospheric waves arrived at the ionosphere. Similar preseismic TEC anomalies, with amplitudes dependent on magnitudes, were seen in the 2010 Chile (Mw 8.8), the 2004 Sumatra-Andaman (Mw 9.2), the 2007 Bengkulu (Mw 8.6), and 
the 1994 Hokkaido-Toho-Oki (Mw 8.3) earthquakes, but not before earthquakes with Mw 8 or less.

### Paper

<a href="http://www.ep.sci.hokudai.ac.jp/~heki/pdf/Heki_GRL2011.pdf" target="_blank">Heki, K., Ionospheric electron enhancement preceding the 2011 Tohoku-Oki earthquake, Geophys. Res. Lett. 38, L17312, doi:10.1029/2011GL047908, 2011.</a>

### Poster

<a href="http://www.ep.sci.hokudai.ac.jp/~heki/pdf/2011AGU_Poster.pdf" target="_blank">Precursory TEC enhancement immediately before the 2011 Tohoku-Oki earthquake.</a> 

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
Use gps_logger.pde with Arduino IDE and upload it to the board to store 
SiRF Binary output into MiniSD card. 
```
