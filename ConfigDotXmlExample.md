# Introduction #

It should be simple to figure out the format from this.


# Example #

```
<?xml version="1.0" encoding="UTF-8" ?>

<user table="bws2009.xml" password="cheese cake">test1</user>

<table name="BWS Sports Day 2009" file="bws2009.xml"><groups number="5"><group>Year 7</group><group>Year 8</group><group>Year 9</group><group>Year 10</group><group>Year 12</group></groups>
<events group="Year 7"><event name="100m sprint" type="TIMELOW" competitors="12"><competitor rank="1" points="13" /><competitor rank="2" points="11" /><!-- and so on like this --></event>
<event name="Javelin" type="DISTANCE" competitors="12"><!-- competitors as above --></event>
<event name="Relay" type="TIMELOW" competitors="28"><competitor rank="1" points="28" /><!-- etc --></event>
</events>
<!-- etc -->
</table>
```