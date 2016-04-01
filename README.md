# atmega-dht22-oled

Platine mit Atmega328, angeschlossen an ein oLED Display und einem DHT22.

Stromversorgung über eine USB Buchse.

## Bauteile

  * 1x Atmega328
  * 1x 0,96" oLED Display
  * 1x DHT22
  * 2x 22pF Transistor
  * 1x 16MHz Quarz
  * 2x Widerstand 10k Ohm
  * 1x Widerstand 220 Ohm
  * 1x USB B Buchse (Lötmontage)


## Gehäuse

Geplant ist ein Gehäuse mit 2mm Plastikplatten.

## Gerber Plot von Kicad zu OSH Park

```
front=$(ls  *-F.Cu.gbr)
projname=$(basename $front \-F.Cu.gbr)

mv *-F.Cu.gbr $projname.GTL
mv *-B.Cu.gbr $projname.GBL
mv *-F.Mask.gbr $projname.GTS
mv *-B.Mask.gbr $projname.GBS
mv *-F.SilkS.gbr $projname.GTO
mv *-B.SilkS.gbr $projname.GBO
mv *-Edge.Cuts.gbr $projname.GKO
mv *.drl $projname.XLN
```
