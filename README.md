#Open TEC

Open Source Project (software and hardware) to get GPS data and calculate TEC ( Total Electron Content ) to be used in predicting the earthquake.

### Inspired by Prof. Kosuke Heki of Hokkaido University

<a href="http://www.youtube.com/watch?feature=player_embedded&v=yAVxsvE5ceM" target="_blank"><img src="http://img.youtube.com/vi/yAVxsvE5ceM/0.jpg" 
alt="Earthquake Predictor using GPS" width="640" height="360" border="10" /></a>

### Paper

<a href="http://www.ep.sci.hokudai.ac.jp/~heki/pdf/Heki_GRL2011.pdf" target="_blank">Heki, K., Ionospheric electron enhancement preceding the 2011 Tohoku-Oki earthquake, Geophys. Res. Lett. 38, L17312, doi:10.1029/2011GL047908, 2011</a>

### This is how it works:

```
1. Get Raw GPS Data from Arduino + Itead Arduino GPS Shield in SiRF binary 
2. Convert SiRF binary to RINEX using 'sirfdump' 
3. Calculate TEC using 'gpstk'
```

##### Misc: Global TEC Map Provided by Trimble (updated every 5 minutes)
<img src='http://www.trimbleionoinfo.com/Images.svc/TEC' width='640'>