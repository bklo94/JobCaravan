from django.conf.urls import url, include
from django.urls import path
from django.contrib import admin
from rest_framework.urlpatterns import format_suffix_patterns
from . import views


urlpatterns = {
    path('', views.index, name='index'),
    path('adminpanel/', admin.site.urls),
}

urlpatterns = format_suffix_patterns(urlpatterns)
