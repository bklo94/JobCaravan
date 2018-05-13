#https://python-visualization.github.io/folium/docs-v0.5.0/plugins.html#folium-plugins
#https://nbviewer.jupyter.org/gist/anonymous/33f38b09ad3bfa277e2d9c06e4bb588c

import flask
import datetime
import folium
from folium.plugins import MarkerCluster
import os
import geojson
import psycopg2
import sys
import pprint
import dbkeys
import urllib.request
import json

def getIP():
    with urllib.request.urlopen("https://geoip-db.com/json") as url:
        data = json.loads(url.read().decode())
    return [data['latitude'], data['longitude']]

def getCoord():
    list = []
    conn_string = dbkeys.conn_string
    conn = psycopg2.connect(conn_string)
    cursor = conn.cursor()
    cursor.execute("SELECT indeed.latitude, indeed.longitude FROM indeed")
    arr = cursor.fetchall()
    for i in arr:
        list.append([[i[0]],[i[1]]])
    return list

def replaceApo(text):
    return text.replace("'","")

def getPop():
    list = []
    conn_string = dbkeys.conn_string
    conn = psycopg2.connect(conn_string)
    cursor = conn.cursor()
    cursor.execute("SELECT indeed.jobtitle, indeed.company, indeed.snippet, indeed.url, indeed.postdate FROM indeed")
    arr = cursor.fetchall()
    #TODO Attempt to Parallelize here
    for i in arr:
        dict = {}
        dict['jobtitle'] = replaceApo(i[0])
        dict['company'] = replaceApo(i[1])
        dict['url'] = i[3]
        dict['postdate'] = i[4]
        list.append(dict)
    return list

def getMap(coordArr,popDict):
    userLocation = getIP()
    #tiles='https://api.mapbox.com/styles/v1/bklo94/cjgnfcaf300052rqnzy8vryd4/tiles/256/{z}/{x}/{y}?access_token='+dbkeys.mapboxAPI
    if userLocation:
        map_1 = folium.Map(location= userLocation, tiles='cartodbdark_matter', zoom_start=10, prefer_canvas=True,attr='Maxbox Data Attribution')
    else:
        map_1 = folium.Map(location= [39.8283,-98.5795], tiles='https://api.mapbox.com/styles/v1/bklo94/cjgnfcaf300052rqnzy8vryd4/tiles/256/{z}/{x}/{y}?access_token='+dbkeys.mapboxAPI, zoom_start=4, prefer_canvas=True,attr='Maxbox Data Attribution')
    mc = MarkerCluster().add_to(map_1)
    #Another possible place to Parallelize
    for i in range(0,len(coordArr)):
        job = "<b>Job:</b>{}</br>".format(popDict[i]['jobtitle'])
        company = "<b>Company:</b>{}</br>".format(popDict[i]['company'])
        #snippet = "<b>Snippet:</b>{}</br>".format(popDict[i]['snippet'])
        url = "<a href=\"{}\" target=\"_blank\">Job Link</a></br>".format(popDict[i]['url'])
        date = "<b>Posted Date:</b>{}</br>".format(popDict[i]['postdate'])
        html = job + company + url + date
        folium.Marker(
            location=[float(coordArr[i][0][0]),float(coordArr[i][1][0])],
            popup=folium.Popup(html)).add_to(mc)
    map_1.save('templates/indeed/map.html')

def main():
    coords = getCoord()
    popups = getPop()
    getMap(coords,popups)

if __name__ == "__main__":
    main()
