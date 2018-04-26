from django.conf.urls import url, include
from django.urls import path
from django.contrib import admin
from rest_framework.urlpatterns import format_suffix_patterns
from .views import CreateView
from .views import DetailsView
from . import views


urlpatterns = {
    path('', views.index, name='index'),
    path('admin/', admin.site.urls),
    url(r'^bucketlists/$', CreateView.as_view(), name="create"),
    url(r'^bucketlists/(?P<pk>[0-9]+)/$',DetailsView.as_view(), name="details"),
}

urlpatterns = format_suffix_patterns(urlpatterns)
