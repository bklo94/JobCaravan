#https://python-visualization.github.io/folium/docs-v0.5.0/plugins.html#folium-plugins
#https://nbviewer.jupyter.org/gist/anonymous/33f38b09ad3bfa277e2d9c06e4bb588c

import flask
import datetime
import folium
from folium.plugins import FastMarkerCluster
import os
import geojson

map_1 = folium.Map(location= [38, 119])
map_2 = folium.Map(location= [38, 119])

# data format
lines = [
    [[38,119],[39,120],[40,119]],
    [[38,119],[39,120],[40,119]]]

# map_1
feature_all = []
for i, each in enumerate(lines):
    polygon_i = geojson.Polygon(tuple([each, ]))
    property_i = {'Idx': str(i)}
    feature_i = geojson.Feature(id=str(i),
                                geometry=polygon_i,
                                properties=property_i)
    feature_all.append(feature_i)

feature_all = geojson.FeatureCollection(feature_all)
folium.GeoJson(feature_all,style_function=lambda feature:
               {'fillColor': None,'weight': 0.8,'line_opacity': 0.8, }).add_to(map_1)
# map_2
folium.PolyLine(lines,weight=0.8,line_opacity=0.8).add_to(map_2)
