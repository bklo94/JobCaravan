from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader

# Create your views here.

from rest_framework import generics
from .models import Indeed

def index(request):
    update = Indeed()
    update.updateDB()
    jobList = list(Indeed.objects.all())
    template = loader.get_template('indeed/index.html')
    context = {
        'jobList': jobList,
    }
    length = len(jobList)
    print ("Number of Jobs -", length)
    return HttpResponse(template.render(context, request))
