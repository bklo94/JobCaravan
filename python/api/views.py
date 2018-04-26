from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader

# Create your views here.

from rest_framework import generics
from .serializers import BucketlistSerializer
from .models import Bucketlist

class CreateView(generics.ListCreateAPIView):
    """This class defines the create behavior of our rest api."""
    queryset = Bucketlist.objects.all()
    serializer_class = BucketlistSerializer

    def perform_create(self, serializer):
        """Save the post data when creating a new bucketlist."""
        serializer.save()

class DetailsView(generics.RetrieveUpdateDestroyAPIView):
    """This class handles the http GET, PUT and DELETE requests."""

    queryset = Bucketlist.objects.all()
    serializer_class = BucketlistSerializer

from .models import Indeed

def index(request):
    jobList = Indeed.objects.all()
    template = loader.get_template('indeed/index.html')
    context = {
        'jobList': jobList,
    }
    return HttpResponse(template.render(context, request))
