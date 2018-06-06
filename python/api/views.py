from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader
import pprint
import re

# Create your views here.

from rest_framework import generics
from .models import Indeed


def getWords(text):
    return re.compile('\w+').findall(text)

def convertMonth(text):
    return{
        'Jan': '01',
        'Feb': '02',
        'Mar': '03',
        'Apr': '04',
        'May': '05',
        'Jun': '06',
        'Jul': '07',
        'Aug': '08',
        'Sep': '09',
        'Oct': '10',
        'Nov': '11',
        'Dec': '12',
    }[text]

def table(request):
    update = Indeed()
    update.updateDB()
    #TODO Parallelize this for loop
    #filters the bolded snippets and formats dates
    for target in Indeed.objects.all():
        #Cleanse the snippet
        temp = target.snippet
        temp = temp.replace('<b>','').replace('</b>','')
        target.snippet = temp
        #Format the date
        temp = target.postdate
        tempArr = getWords(temp)
        if (len(tempArr) == 3):
            target.save()
            break
        else:
            tempArr[2] = convertMonth(tempArr[2])
            dateFormat = tempArr[2] + "-" + tempArr[1] +"-"+ tempArr[3]
            target.postdate= dateFormat
            #save the data
            target.save()

    jobList = list(Indeed.objects.all())
    template = loader.get_template('indeed/table.html')
    context = {
        'jobList': jobList,
    }
    length = len(jobList)
    print ("Number of Jobs -", length)
    return HttpResponse(template.render(context, request))

def main(request):
    return render(request,'indeed/main.html')

def index(request):
    return render(request,'indeed/index.html')

def leaflet(request):
    update = Indeed()
    update.updateDB()
    jobList = list(Indeed.objects.all())
    template = loader.get_template('indeed/leaflet.html')
    context = {
        'jobList': jobList,
    }
    length = len(jobList)
    print ("Number of Jobs -", length)
    return HttpResponse(template.render(context, request))
