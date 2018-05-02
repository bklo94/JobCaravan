#https://python-visualization.github.io/folium/docs-v0.5.0/plugins.html#folium-plugins
#https://nbviewer.jupyter.org/gist/anonymous/33f38b09ad3bfa277e2d9c06e4bb588c

import flask
import datetime
import folium
from folium.plugins import FastMarkerCluster
import os
import geojson
import psycopg2
import sys
import pprint
import dbkeys

def map(arr):
    map_1 = folium.Map(location= [37.7749,-122.4194], tiles='https://api.mapbox.com/styles/v1/bklo94/cjgnfcaf300052rqnzy8vryd4/tiles/256/{z}/{x}/{y}?access_token='+dbkeys.mapboxAPI, zoom_start=8, prefer_canvas=True,attr='Maxbox Data Attribution')
    list = []
    for i in arr:
        list.append([[i[0]],[i[1]]])
    #print (list)
    FastMarkerCluster(list).add_to(map_1)
    map_1.save('templates/indeed/map.html')

def main():
    conn_string = dbkeys.conn_string
    conn = psycopg2.connect(conn_string)
    cursor = conn.cursor()
    cursor.execute("SELECT indeed.latitude, indeed.longitude FROM indeed")
    records = cursor.fetchall()
    map(records)

if __name__ == "__main__":
    main()
