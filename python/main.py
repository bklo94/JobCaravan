import folium
from folium.plugins import MarkerCluster
import os

m = folium.Map(location=[44, -73], zoom_start=5)

marker_cluster = MarkerCluster().add_to(m)


folium.Marker(
    location=[40.67, -73.94],
    popup='Add popup text here.',
    icon=folium.Icon(color='green', icon='ok-sign'),
).add_to(marker_cluster)

folium.Marker(
    location=[44.67, -73.94],
    popup='Add popup text here.',
    icon=folium.Icon(color='red', icon='remove-sign'),
).add_to(marker_cluster)

folium.Marker(
    location=[44.67, -71.94],
    popup='Add popup text here.',
    icon=None,
).add_to(marker_cluster)

m.save('1000_MarkerCluster0.html')
