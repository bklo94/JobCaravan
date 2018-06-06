from django.conf.urls import url, include
from django.urls import path
from django.contrib import admin
from rest_framework.urlpatterns import format_suffix_patterns
from . import views
from JobCaravan import settings


urlpatterns = {
    path('', views.index),
    path('main', views.main),
    path('table',views.table),
    path('leaflet',views.leaflet)
}

urlpatterns = format_suffix_patterns(urlpatterns)
